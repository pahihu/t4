/*
 * Copyright (c) 1993-1996 Julian Highfield. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Julian Highfield.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define INLINE


/*
 * p.c - hand inlined processor.c!
 *
 * The transputer emulator.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "processor.h"
#include "arithmetic.h"
#include "server.h"
#include "opcodes.h"
#ifdef __MWERKS__
#include "mac_input.h"
#endif
#undef TRUE
#undef FALSE
#define TRUE  0x0001
#define FALSE 0x0000

#define INT(x)          ((int32_t)(x))

/* Memory space. */
#define MEM_SIZE (2*1024*1024)
#define MEM_WORD_MASK ((uint32_t)0x001ffffc)
#define MEM_BYTE_MASK ((uint32_t)0x001fffff)
unsigned char *mem;
uint32_t readword (uint32_t);

/* Registers. */
uint32_t IPtr;
uint32_t WPtr;
uint32_t AReg;
uint32_t BReg;
uint32_t CReg;
uint32_t OReg;

/* Other registers. */
uint32_t ClockReg0;
uint32_t ClockReg1;
uint32_t TNextReg0;
uint32_t TNextReg1;
uint32_t TPtrLoc0;              /* XXX 0x80000024 */
uint32_t TPtrLoc1;              /* XXX 0x80000028 */

uint32_t FPtrReg0;
uint32_t BPtrReg0;
uint32_t FPtrReg1;
uint32_t BPtrReg1;

uint32_t STATUSReg;             /* Processor flags: GotoSNPBit, HaltOnError, Error */

uint32_t IntEnabled;            /* Interrupt enabled */
#define ClearInterrupt          writeword (0x8000002C, MostNeg + 1)
#define ReadInterrupt           (word (0x8000002C) != (MostNeg + 1))

#define GotoSNPBit              0x00000001
#define HaltOnErrorFlag         0x00000080
#define ErrorFlag               0x80000000

#define SetGotoSNP              STATUSReg |= GotoSNPBit
#define ClearGotoSNP            STATUSReg &= ~GotoSNPBit
#define ReadGotoSNP             (STATUSReg & GotoSNPBit)

#define SetError                STATUSReg |= ErrorFlag
#define ClearError              STATUSReg &= ~ErrorFlag
#define ReadError               (STATUSReg & ErrorFlag)

#define SetHaltOnError          STATUSReg |= HaltOnErrorFlag
#define ClearHaltOnError        STATUSReg &= ~HaltOnErrorFlag
#define ReadHaltOnError         (STATUSReg & HaltOnErrorFlag)


#define Iptr_s          (-1)
#define Link_s          (-2)
#define State_s         (-3)
#define Pointer_s       (-3)
#define TLink_s         (-4)
#define Time_s          (-5)


#define GetDescPriority(wdesc)  ((wdesc) & 0x00000001)
#define GetDescWPtr(wdesc)      ((wdesc) & 0xfffffffe)


/* Internal variables. */
unsigned char Instruction;
unsigned char Icode;
unsigned char Idata;
int  Timers;
uint32_t t4_overflow;
uint32_t t4_carry;
uint32_t t4_normlen;
uint32_t ProcPriority;
#define TimersGo   1
#define TimersStop 0
int loop;
int count1;
int count2;
int count3;
int timeslice;
int32_t quit = FALSE;
int32_t quitstatus;

/* XXX Wdesc contains MostNeg + 1 in idle state */
#define Wdesc   (WPtr | ProcPriority)

/* External variables. */
extern int analyse;
extern int exitonerror;
extern int FromServerLen;
extern int profiling;
extern int32_t profile[10];
extern int emudebug;
extern int emumem;
extern int msgdebug;

LinkIface Link[4];

/* Macros. */
#define index(a,b)		((a)+(4*(b)))

/* Profile information structure and head. */
struct prof
{
	int32_t instruction;
	int32_t count;
	struct prof *next;
	struct prof *prev;
};

struct prof *profile_head = NULL;

/* Signal handler. */
void handler (int);

struct timeval LastTOD;         /* Time-of-day */

/* Update time-of-day. */
void update_tod (struct timeval *tp)
{
        int rc;

        rc = gettimeofday (tp, (void *)0);
        if (rc < 0)
        {
                printf ("-W-EMU414: Failed to get time value.\n");

                *tp = LastTOD;
                tp->tv_usec++;
                if (0 == tp->tv_usec)
                        tp->tv_sec++;
        }
}

/* Reset a link channel */
void reset_channel (uint32_t addr)
{
        Channel *chan;


        /* Reset channel control word. */
        writeword (addr, NotProcess_p);

        chan = (Channel *)0;
        if (addr == Link0In)
                chan = &Link[0].In;
        else if (addr == Link0Out)
                chan = &Link[0].Out;
        
        if (chan)
        {
                chan->Address = MostNeg;
                chan->Length  = 0;
        }
}

/* Print processor state. */
void processor_state (void)
{
        printf ("-I-EMU414: Processor state\n");
        printf ("\tIptr          #%08X\n", IPtr);
        printf ("\tWptr          #%08X\n", WPtr);
        printf ("\tAreg          #%08X\n", AReg);
        printf ("\tBreg          #%08X\n", BReg);
        printf ("\tCreg          #%08X\n", CReg);
        printf ("\tError         %s\n", ReadError ? "Set" : "Clear");
        printf ("\tHalt on Error %s\n", ReadHaltOnError ? "Set" : "Clear");
        printf ("\tFptr1 (Low    #%08X\n", FPtrReg1);
        printf ("\tBptr1  queue) #%08X\n", BPtrReg1);
        printf ("\tFptr0 (High   #%08X\n", FPtrReg0);
        printf ("\tBptr0  queue) #%08X\n", BPtrReg0);
        printf ("\tTptr1 (Timer  #%08X\n", TPtrLoc1);
        printf ("\tTptr0 (queues #%08X\n", TPtrLoc0);
}

void save_dump (void)
{
        FILE *fout;
        unsigned int bytesWritten;

        fout = fopen ("dump", "wb");
        if (fout == NULL)
        {
                printf ("-E-EMU404: Error - failed to open dump file.\n");
                handler (-1);
        }
        bytesWritten = fwrite (mem, sizeof (unsigned char), MEM_SIZE, fout);
        if (bytesWritten != MEM_SIZE)
        {
                printf ("-E-EMU414: Error - failed to write dump file.\n");
                fclose (fout);
                unlink ("dump");
                handler (-1);
        }
        fclose (fout);
}

char *mnemonic(unsigned char icode, uint32_t oreg)
{
        char *mnemo;
        char bad[16];
        static char str[32];

        if (icode > 239)
        {
                if (oreg < 0xFA)
                        mnemo = Secondaries[oreg];
                else
                {
                        sprintf (bad, "--%X--", oreg);
                        mnemo = bad;
                }
                sprintf (str, "%s", mnemo);
                return str;
        }
        sprintf (str, "%-7s #%X", Primaries[icode >> 4], oreg);
        return str;
}

void init_processor (void)
{
        /* M.Bruestle 15.2.2012 */
        reset_channel (Link0Out);
        reset_channel (Link0In);

        IPtr = MemStart;
        CReg = Link0In;
        TPtrLoc0 = NotProcess_p;
        TPtrLoc1 = NotProcess_p;
        ClearInterrupt; /* XXX not required ??? */

        IntEnabled = TRUE;

        /* Init TOD. */
        LastTOD.tv_sec  = 0;
        LastTOD.tv_usec = 0;
        update_tod (&LastTOD);

        /* ErrorFlag is in an indeterminate state on power up. */
}

#define FLAG(x,y)       ((x) ? (y) : '-')

void mainloop (void)
{
        uint32_t temp, temp2;
        uint32_t otherWdesc, otherWPtr, otherPtr, altState;
        uint32_t PrevError;

        int printIPtr, instrBytes;
        int asmLines;


        printIPtr  = TRUE;
        instrBytes = 0;
        asmLines   = 0;

        init_processor ();

	count1 = 0;
	count2 = 0;
	count3 = 0;
	timeslice = 0;
	Timers = TimersStop;


	while (1)
	{
#ifndef NDEBUG
                temp = temp2 = 0xdeadbeef;
                otherWdesc = otherWPtr = otherPtr = altState = 0xdeadbeef;
#endif
                /* Save current value of Error flag */
                PrevError = ReadError;

		/* Move timers on if necessary, and increment timeslice counter. */
		update_time ();

                if (ReadGotoSNP)
                        start_process ();

		/* Execute an instruction. */
	Instruction = byte (IPtr);
	Icode = Instruction & 0xf0;
	Idata = Instruction & 0x0f;
	OReg  = OReg | Idata;

        /* Disable interrupts on PFIX or NFIX. */
        IntEnabled = IntEnabled && ((Icode != 0x20) && (Icode != 0x60));

        if (emudebug)
        {
	        /* General debugging messages. */
                if (printIPtr)
                {
                        if (0 == asmLines++ % 25)
                                printf ("-IPtr------Code-----------------------Mnemonic------------HE---AReg-----BReg-----CReg-------WPtr-----WPtr[0]-\n");
                        printf ("%c%08X: ", HiPriority == ProcPriority ? 'H' : ' ', IPtr);
                        printIPtr = FALSE;
                        instrBytes = 0;
                }
                printf("%02X ", Instruction); instrBytes++;
                if ((0x20 == Icode) || (0x60 == Icode))
                        ;
                else
                {
                        for (; instrBytes < 9; instrBytes++)
                                printf("   ");
                        printf("%-17s", mnemonic (Icode, OReg));
	                printf ("   %c%c   %8X %8X %8X   %08X %8X\n", 
                                FLAG(ReadHaltOnError, 'H'),
                                FLAG(      ReadError, 'E'),
                                AReg, BReg, CReg, WPtr, readword (WPtr));
                        printIPtr = TRUE;
                }
        }

#ifdef PROFILE
	if (profiling)
		add_profile (INT(Icode) - 0x100);
#endif

	switch (Icode)
	{
		case 0x00: /* j     */
			   IPtr++;
			   IPtr = IPtr + OReg;
			   OReg = 0; IntEnabled = TRUE;
			   D_check();
			   break;
		case 0x10: /* ldlp  */
			   CReg = BReg;
			   BReg = AReg;
			   AReg = index (WPtr, OReg);
			   IPtr++;
			   OReg = 0; IntEnabled = TRUE;
			   break;
		case 0x20: /* pfix  */
			   OReg = OReg << 4;
			   IPtr++;
			   break;
		case 0x30: /* ldnl  */
			   AReg = word (index (AReg, OReg));
			   IPtr++;
			   OReg = 0; IntEnabled = TRUE;
			   break;
		case 0x40: /* ldc   */
			   CReg = BReg;
			   BReg = AReg;
			   AReg = OReg;
			   IPtr++;
			   OReg = 0; IntEnabled = TRUE;
			   break;
		case 0x50: /* ldnlp */
			   AReg = index (AReg, OReg);
			   IPtr++;
			   OReg = 0; IntEnabled = TRUE;
			   break;
		case 0x60: /* nfix  */
			   OReg = (~OReg) << 4;
			   IPtr++;
			   break;
		case 0x70: /* ldl   */
			   CReg = BReg;
			   BReg = AReg;
			   AReg = word (index (WPtr, OReg));
			   IPtr++;
			   OReg = 0; IntEnabled = TRUE;
			   break;
		case 0x80: /* adc   */
			   t4_overflow = FALSE;
			   t4_carry = 0;
			   AReg = t4_eadd32 (AReg, OReg);
			   if (t4_overflow == TRUE)
				SetError;
			   IPtr++;
			   OReg = 0; IntEnabled = TRUE;
			   break;
		case 0x90: /* XXX call  */
			   IPtr++;
			   writeword (index (WPtr, -1), CReg);
			   writeword (index (WPtr, -2), BReg);
			   writeword (index (WPtr, -3), AReg);
			   writeword (index (WPtr, -4), IPtr);
			   WPtr = index ( WPtr, -4);
			   AReg = IPtr;
			   IPtr = IPtr + OReg;
			   OReg = 0; IntEnabled = TRUE;
			   break;
		case 0xa0: /* cj    */
			   IPtr++;
			   if (AReg != 0)
			   {
				AReg = BReg;
				BReg = CReg;
			   }
			   else
			   {
				IPtr = IPtr + OReg;
			   }
			   OReg = 0; IntEnabled = TRUE;
			   break;
		case 0xb0: /* ajw   */
			   WPtr = index (WPtr, OReg);
			   IPtr++;
			   OReg = 0; IntEnabled = TRUE;
			   break;
		case 0xc0: /* eqc   */
			   if (AReg == OReg)
			   {
				AReg = true_t;
			   }
			   else
			   {
				AReg = false_t;
			   }
			   IPtr++;
			   OReg = 0; IntEnabled = TRUE;
			   break;
		case 0xd0: /* stl   */
			   writeword (index (WPtr, OReg), AReg);
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   OReg = 0; IntEnabled = TRUE;
			   break;
		case 0xe0: /* XXX stnl  */
			   writeword (index (AReg, OReg), BReg);
			   AReg = CReg;
			   IPtr++;
			   OReg = 0; IntEnabled = TRUE;
			   break;
		case 0xf0: /* opr   */

        IntEnabled = TRUE;

#ifdef PROFILE
	if (profiling)
		add_profile (OReg);
#endif

	switch (OReg)
	{
		case 0x00: /* rev         */
			   temp = AReg;
			   AReg = BReg;
			   BReg = temp;
			   IPtr++;
			   break;
		case 0x01: /* lb          */
			   AReg = byte (AReg);
			   IPtr++;
			   break;
		case 0x02: /* bsub        */
			   AReg = AReg + BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x03: /* XXX endp        */
			   temp = word (index (AReg, 1));
			   if (temp == 1)
			   {
                                if (emudebug)
                                        printf ("-I-EMUDBG: endp: Do successor process.\n");

				/* Do successor process. */
				WPtr = AReg;
				IPtr = word (index (AReg, 0));
			   }
			   else
			   {
				/* Have not finished all parallel branches. */
                                if (emudebug)
                                        printf ("-I-EMUDBG: endp: Waiting for parallel branches (%d).\n", temp);

				/* start_process (); */
                                SetGotoSNP;
			   }
                           temp--;
                           writeword (index (AReg, 1), temp);
			   break;
		case 0x04: /* diff        */
			   AReg = BReg - AReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x05: /* add         */
			   t4_overflow = FALSE;
			   t4_carry = 0;
			   AReg = t4_eadd32 (BReg, AReg);
			   if (t4_overflow == TRUE)
				SetError;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x06: /* gcall       */
			   IPtr++;
			   temp = AReg;
			   AReg = IPtr;
			   IPtr = temp;
			   break;
		case 0x07: /* in          */
OprIn:                     if (BReg == Link0Out) /* M.Bruestle 22.1.2012 */
                           {
                                if (msgdebug || emudebug)
                                        printf ("-W-EMUDBG: Doing in on Link0Out.\n");
                                goto OprOut;
                           }
                           if (msgdebug || emudebug)
			        printf ("-I-EMUDBG: In(1): Channel=#%08X, to memory at #%08X, length #%X.\n", BReg, CReg, AReg);
			   IPtr++;
			   if (BReg != Link0In)
			   {
				/* Internal communication. */
				otherWdesc = word (BReg);
                                if (msgdebug || emudebug)
                                        printf ("-I-EMUDBG: In(2): Internal communication. Channel word=#%08X.\n", otherWdesc);
				if (otherWdesc == NotProcess_p)
				{
					/* Not ready. */
                                        if (msgdebug || emudebug)
                                                printf ("-I-EMUDBG: In(3): Not ready.\n");
					writeword (BReg, Wdesc);
					writeword (index (WPtr, Pointer_s), CReg);
                                        deschedule ();
				}
				else
				{
					/* Ready. */
                                        otherWPtr = GetDescWPtr(otherWdesc);
					otherPtr = word (index (otherWPtr, Pointer_s));
                                        if (msgdebug || emudebug)
					        printf ("-I-EMUDBG: In(3): Transferring message from #%08X.\n", otherPtr);
					for (loop=0;loop<AReg;loop++)
					{
						writebyte ((CReg + loop), byte (otherPtr + loop));
					}
					writeword (BReg, NotProcess_p);
					schedule (otherWdesc);
				}
			   }
			   else
			   {
				/* Link communication. */
                                if (msgdebug || emudebug)
                                        printf ("-I-EMUDBG: In(2): Link communication. Old channel word=#%08X.\n", word (BReg));
				writeword (BReg, Wdesc);
				Link0InDest   = CReg;
				Link0InLength = AReg;
                                deschedule ();
			   }
			   break;
		case 0x08: /* prod        */
			   AReg = BReg * AReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x09: /* gt          */
			   if (INT(BReg) > INT(AReg))
			   {
				AReg = true_t;
			   }
			   else
			   {
				AReg = false_t;
			   }
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x0a: /* wsub        */
			   AReg = index (AReg, BReg);
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x0b: /* out         */
OprOut:                    if (BReg == Link0In) /* M.Bruestle 22.1.2012 */
                           {
                                if (msgdebug || emudebug)
                                        printf ("-W-EMUDBG: Doing out on Link0In.\n");
                                goto OprIn;
                           }
                           if (msgdebug || emudebug)
			        printf ("-I-EMUDBG: out(1): Channel=#%08X, length #%X, from memory at #%08X.\n", BReg, AReg, CReg);
			   IPtr++;
			   if (BReg != Link0Out)
			   {
				/* Internal communication. */
				otherWdesc = word (BReg);
                                if (msgdebug || emudebug)
                                        printf ("-I-EMUDBG: out(2): Internal communication. Channel word=#%08X.\n", otherWdesc);
				if (otherWdesc == NotProcess_p)
				{
					/* Not ready. */
                                        if (msgdebug || emudebug)
                                                printf ("-I-EMUDBG: out(3): Not ready.\n");
					writeword (BReg, Wdesc);
					writeword (index (WPtr, Pointer_s), CReg);
                                        deschedule ();
				}
				else
				{
					/* Ready. */
                                        otherWPtr  = GetDescWPtr(otherWdesc);
					altState = otherPtr = word (index (otherWPtr, State_s));
                                        if (msgdebug || emudebug)
                                                printf ("-I-EMUDBG: out(3): Memory address/ALT state=#%08X.\n", altState);
					if ((altState & 0xfffffffc) == MostNeg)
					{
						/* ALT guard test - not ready to communicate. */
                                                if (msgdebug || emudebug)
                                                        printf ("-I-EMUDBG: out(4): ALT guard test - not ready to communicate.\n");

						writeword (BReg, Wdesc);
						writeword (index (WPtr, Pointer_s), CReg);
                                                deschedule ();

						/* The alt is waiting. Rechedule it? */
						if (altState != Ready_p)
						{
							/* The alt has not already been rescheduled. */
                                                        if (msgdebug || emudebug)
                                                        {
                                                                printf ("-I-EMUDBG: out(5): ALT state=Ready_p.\n");
                                                                printf ("-I-EMUDBG: out(6): Reschedule ALT process (Wdesc=#%08X, IPtr=#%08X).\n",
                                                                                                otherWdesc, word (index (otherWPtr, Iptr_s)));
                                                        }
							writeword (index (otherWPtr, State_s), Ready_p);
							schedule (otherWdesc);
						}
          				}
					else
					{
						/* Ready. */
                                                if (msgdebug || emudebug)
                                                        printf ("-I-EMUDBG: out(4): Ready, communicate.\n");
						for (loop = 0;loop < AReg; loop++)
						{
							writebyte ((otherPtr + loop), byte (CReg + loop));
						}
						writeword (BReg, NotProcess_p);
						schedule (otherWdesc);
					}
				}
			   }
			   else
			   {
				/* Link communication. */
                                if (msgdebug || emudebug)
                                        printf ("-I-EMUDBG: out(2): Link communication. Old channel word=#%08X.\n", word (BReg));
				writeword (BReg, Wdesc);
				Link0OutSource = CReg;
				Link0OutLength = AReg;
                                deschedule ();
			   }
			   break;
		case 0x0c: /* sub         */
			   t4_overflow = FALSE;
			   t4_carry = 0;
			   AReg = t4_esub32 (BReg, AReg);
			   if (t4_overflow == TRUE)
				SetError;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x0d: /* startp      */
			   temp = GetDescWPtr(AReg);
			   IPtr++;
			   writeword (index (temp, Iptr_s), (IPtr + BReg));
			   schedule (temp | ProcPriority);
			   break;
		case 0x0e: /* outbyte     */
                           if (msgdebug || emudebug)
			        printf ("-I-EMUDBG: outbyte: Channel=#%08X.\n", BReg);
			   IPtr++;
                           if (BReg == Link0In) /* M.Bruestle 22.1.2012 */
                           {
                                if (msgdebug || emudebug)
			                printf ("-W-EMUDBG: Doing outbyte on Link0In.\n");
				/* Link communication. */
				writeword (BReg, Wdesc);
				writeword (WPtr, AReg);
				Link0InDest = WPtr;
				Link0InLength = 1;
                                deschedule ();
                           }
			   else if (BReg != Link0Out)
			   {
				/* Internal communication. */
				otherWdesc = word (BReg);
				if (otherWdesc == NotProcess_p)
				{
					/* Not ready. */
					writeword (BReg, Wdesc);
					writeword (WPtr, AReg);
					writeword (index (WPtr, Pointer_s), WPtr);
                                        deschedule ();
				}
				else
				{
					/* Ready. */
                                        otherWPtr = GetDescWPtr(otherWdesc);
					altState = otherPtr = word (index (otherWPtr, Pointer_s));
					if ((altState & 0xfffffffc) == MostNeg)
					{
						/* ALT guard test - not ready to communicate. */

						writeword (BReg, Wdesc);
						writeword (WPtr, AReg);
						writeword (index (WPtr, Pointer_s), WPtr);
                                                deschedule ();

						/* The alt is waiting. Rechedule it? */
						if (altState != Ready_p)
						{
							/* The alt has not already been rescheduled. */
							writeword (index (otherWPtr, State_s), Ready_p);
							schedule (otherWdesc);
						}
          				}
					else
					{
						/* Ready. */
						writebyte (otherPtr, AReg);
						writeword (BReg, NotProcess_p);
						schedule (otherWdesc);
					}
				}
			   }
			   else
			   {
				/* Link communication. */
				writeword (BReg, Wdesc);
				writeword (WPtr, AReg);
				Link0OutSource = WPtr;
				Link0OutLength = 1;
                                deschedule ();
			   }
			   break;
		case 0x0f: /* outword     */
                           if (msgdebug || emudebug)
			        printf ("-I-EMUDBG: outword(1): Channel=#%08X.\n", BReg);
			   IPtr++;
                           if (BReg == Link0In) /* M.Bruestle 22.1.2012 */
                           {
                                if (msgdebug || emudebug)
			                printf ("-W-EMUDBG: outword(2): Doing outword on Link0In. Old channel word=#%08X.\n", word (BReg));
				/* Link communication. */
				writeword (BReg, Wdesc);
				writeword (WPtr, AReg);
				Link0InDest   = WPtr;
				Link0InLength = 4;
                                deschedule ();
                           }
			   else if (BReg != Link0Out)
			   {
				/* Internal communication. */
				otherWdesc = word (BReg);
                                if (msgdebug || emudebug)
                                        printf ("-I-EMUDBG: outword(2): Internal communication. Channel word=#%08X.\n", otherWdesc);
				if (otherWdesc == NotProcess_p)
				{
                                        if (msgdebug || emudebug)
                                                printf ("-I-EMUDBG: outword(3): Not ready.\n");
					/* Not ready. */
					writeword (BReg, Wdesc);
					writeword (WPtr, AReg);
					writeword (index (WPtr, Pointer_s), WPtr);
                                        deschedule ();
				}
				else
				{
					/* Ready. */
                                        otherWPtr = GetDescWPtr(otherWdesc);
					altState = otherPtr =  word (index (otherWPtr, State_s));
					if ((altState & 0xfffffffc) == MostNeg)
					{
						/* ALT guard test - not ready to communicate. */
                                                if (msgdebug || emudebug)
                                                        printf ("-I-EMUDBG: outword(3): ALT guard test - not ready.\n");

						writeword (BReg, Wdesc);
						writeword (WPtr, AReg);
						writeword (index (WPtr, Pointer_s), WPtr);
                                                deschedule ();

						/* The alt is waiting. Rechedule it? */
						if (altState != Ready_p)
						{
                                                        if (msgdebug || emudebug)
                                                        {
                                                                printf ("-I-EMUDBG: outword(4): ALT state=Ready_p.\n");
                                                                printf ("-I-EMUDBG: outword(5): Reschedule ALT process (Wdesc=#%08X, IPtr=#%08X).\n",
                                                                                                otherWdesc, word (index (otherWPtr, Iptr_s)));
                                                        }
							/* The alt has not already been rescheduled. */
							writeword (index (otherWPtr, State_s), Ready_p);
							schedule (otherWdesc);
						}
          				}
					else
					{
                                                if (msgdebug || emudebug)
                                                        printf ("-I-EMUDBG: outword(3): Ready.\n");

						/* Ready. */
						writeword (otherPtr, AReg);
						writeword (BReg, NotProcess_p);
						schedule (otherWdesc);
					}
				}
			   }
			   else
			   {
				/* Link communication. */
                                if (msgdebug || emudebug)
                                        printf ("-I-EMUDBG: out(2): Link communication. Old channel word=#%08X.\n", word (BReg));
				writeword (BReg, Wdesc);
				writeword (WPtr, AReg);
				Link0OutSource = WPtr;
				Link0OutLength = 4;
                                deschedule ();
			   }
			   break;
		case 0x10: /* seterr      */
			   SetError;
			   IPtr++;
			   break;
		case 0x12: /* XXX resetch     */
			   temp = AReg;
			   AReg = word (temp);
			   writeword (temp, NotProcess_p);
                           reset_channel (temp);
			   IPtr++;
			   break;
		case 0x13: /* csub0       */
			   if (BReg >= AReg)
			   {
				SetError;
			   }
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x15: /* stopp       */
			   IPtr++;
                           deschedule ();
			   break;
		case 0x16: /* ladd        */
			   t4_overflow = FALSE;
			   t4_carry = CReg & 0x00000001;
			   AReg = t4_eadd32 (BReg, AReg);
			   if (t4_overflow == TRUE)
				SetError;
			   IPtr++;
			   break;
		case 0x17: /* stlb        */
			   BPtrReg1 = AReg;
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x18: /* sthf        */
			   FPtrReg0 = AReg;
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x19: /* norm        */
			   AReg = t4_norm64 (BReg, AReg);
			   BReg = t4_carry;
			   CReg = t4_normlen;
			   IPtr++;
			   break;
		case 0x1a: /* XXX ldiv        */
			   if (CReg >= AReg)
			   {
				SetError;
			   }
			   else if (CReg != 0)
			   {
				t4_carry = 0;
				AReg = t4_longdiv (CReg, BReg, AReg);
				BReg = t4_carry;
			   }
			   else
			   {
				temp = BReg / AReg;
				temp2 = BReg % AReg;
				AReg = temp;
				BReg = temp2;
			   }
			   IPtr++;
			   break;
		case 0x1b: /* ldpi        */
			   IPtr++;
			   AReg = IPtr + AReg;
			   break;
		case 0x1c: /* stlf        */
			   FPtrReg1 = AReg;
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x1d: /* XXX xdble       */
			   CReg = BReg;
			   if (INT(AReg) < 0)
			   {
				BReg = -1;
			   }
			   else
			   {
				BReg = 0;
			   }
			   IPtr++;
			   break;
		case 0x1e: /* ldpri       */
			   CReg = BReg;
			   BReg = AReg;
			   AReg = ProcPriority;
			   IPtr++;
			   break;
		case 0x1f: /* XXX rem         */
			   if ((AReg==0) || ((AReg==-1) && (BReg==0x80000000)))
				SetError;
			   else
				AReg = INT(BReg) % INT(AReg);
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x20: /* ret         */
			   IPtr = word (WPtr);
			   WPtr = index (WPtr, 4);
			   break;
		case 0x21: /* lend        */ /****/
			   temp = word (index (BReg, 1));
			   IPtr++;
			   if (temp > 1)
			   {
				writeword (index (BReg, 1), (temp - 1));
				writeword (BReg, (word (BReg) + 1));
				IPtr =  IPtr - AReg;
				D_check();
			   }
			   else
			   {
				writeword (index (BReg, 1), (temp - 1));
			   }
			   break;
		case 0x22: /* ldtimer     */
			   CReg = BReg;
			   BReg = AReg;
			   if (ProcPriority == HiPriority)
			   {
				AReg = ClockReg0;
			   }
			   else
			   {
				AReg = ClockReg1;
			   }
			   IPtr++;
			   break;
		case 0x29: /* testerr     */
			   CReg = BReg;
			   BReg = AReg;
			   if (ReadError)
			   {
				AReg = false_t;
			   }
			   else
			   {
				AReg = true_t;
			   }
			   ClearError;
			   IPtr++;
			   break;
		case 0x2a: /* testpranal  */
			   CReg = BReg;
			   BReg = AReg;
			   if (analyse) AReg = true_t; else AReg = false_t;
			   IPtr++;
			   break;
		case 0x2b: /* XXX: missing Waiting_p tin         */
			   IPtr++;
			   if (ProcPriority == HiPriority)
			   {
				if (INT(ClockReg0 - AReg) > 0)
					;
				else
				{
					insert (AReg);
                                        deschedule ();;
				}
			   }
			   else
			   {
				if (INT(ClockReg1 - AReg) > 0)
					;
				else
				{
					insert (AReg);
                                        deschedule ();
				}
			   }
			   break;
		case 0x2c: /* XXX div         */
			   if ((AReg==0) || ((AReg==-1)&&(BReg==0x80000000)))
				SetError;
			   else
				AReg = INT(BReg) / INT(AReg);
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x2e: /* XXX dist        */
                           if (emudebug)
			        printf ("-I-EMUDBG: dist(1): Time=%8X.\n", CReg);
			   if (ProcPriority == HiPriority)
				temp = ClockReg0;
			   else
				temp = ClockReg1;
			   if ((BReg==true_t) && (INT(temp-CReg)>=0) && (word(index(WPtr,0))==NoneSelected_o))
			   {
                                if (emudebug)
				        printf ("-I-EMUDBG: dist(2): Taking branch #%8X.\n", AReg);
				writeword (index (WPtr, 0), AReg);
				AReg = true_t;
			   }
			   else
			   {
                                if (emudebug)
				        printf ("-I-EMUDBG: dist(2): Not taking this branch.\n");
				AReg = false_t;
			   }
			   IPtr++;
			   break;
		case 0x2f: /* XXX: support ALT construct on Link0 disc        */
                           if (emudebug)
			        printf ("-I-EMUDBG: disc(1): Channel=#%08X.\n", CReg);
          		   if (CReg == Link0In)
          		   {
                                if (emudebug)
				        printf ("-I-EMUDBG: disc(2): Link.\n");

				/* External link. */
          			if (FromServerLen > 0)
          				temp = TRUE;
          			else
          				temp = FALSE;
          		   }
          		   else
          		   {
                                otherWdesc = word (CReg);
                                if (emudebug)
				        printf ("-I-EMUDBG: disc(2): Internal channel. Channel word=#%08X.\n", otherWdesc);
				/* Internal channel. */
          			if (otherWdesc == NotProcess_p)
				{
                                        if (emudebug)
                                                printf ("-I-EMUDBG: disc(3): Channel not ready.\n");

					/* Channel not ready. */
					temp = FALSE;
				}
				else if (otherWdesc == Wdesc)
				{
                                        if (emudebug)
                                                printf ("-I-EMUDBG: disc(3): Channel not ready, but this process enabled it.\n");

					/* Channel not ready, but was initialised by this process's enbc. */
          				temp = FALSE;

					/* Reset channel word to NotProcess_p to avoid confusion. */
					writeword (CReg, NotProcess_p);
				}
          			else
				{
                                        if (emudebug)
                                                printf ("-I-EMUDBG: disc(3): Channel ready.\n");

					/* Channel ready. */
          				temp = TRUE;
				}
          		   }
			   if ((BReg == true_t) && (temp == TRUE) && (word (index (WPtr, 0)) == NoneSelected_o))
			   {
                                if (emudebug)
				        printf ("-I-EMUDBG: disc(4): Taking branch #%8X.\n", AReg);
				writeword (index (WPtr, 0), AReg);
				AReg = true_t;
			   }
			   else
			   {
                                if (emudebug)
				        printf ("-I-EMUDBG: disc(3): Not taking this branch.\n");
				AReg = false_t;
			   }
			   IPtr++;
			   break;
		case 0x30: /* diss        */
			   if ((BReg==true_t) && (word(index(WPtr,0))==NoneSelected_o))
			   {
				writeword (index (WPtr, 0), AReg);
				AReg = true_t;
			   }
			   else
			   {
				AReg = false_t;
			   }
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x31: /* lmul        */
			   t4_overflow = FALSE;
			   t4_carry = CReg;
			   AReg = t4_mul32 (BReg, AReg);
			   BReg = t4_carry;
			   IPtr++;
			   break;
		case 0x32: /* not         */
			   AReg = ~ AReg;
			   IPtr++;
			   break;
		case 0x33: /* xor         */
			   AReg = BReg ^ AReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x34: /* bcnt        */
			   AReg = AReg * 4;
			   IPtr++;
			   break;
		case 0x35: /* lshr        */
			   AReg = t4_shr64 (CReg, BReg, AReg);
			   BReg = t4_carry;
			   IPtr++;
			   break;
		case 0x36: /* lshl        */
			   AReg = t4_shl64 (CReg, BReg, AReg);
			   BReg = t4_carry;
			   IPtr++;
			   break;
		case 0x37: /* lsum        */
			   t4_overflow = FALSE;
			   t4_carry = CReg & 0x00000001;
			   AReg = t4_add32 (BReg, AReg);
			   BReg = t4_carry;
			   IPtr++;
			   break;
		case 0x38: /* lsub        */
			   t4_overflow = FALSE;
			   t4_carry = CReg & 0x00000001;
			   AReg = t4_esub32 (BReg, AReg);
			   if (t4_overflow == TRUE)
				SetError;
			   IPtr++;
			   break;
		case 0x39: /* runp        */
			   IPtr++;
			   schedule (AReg);
			   break;
		case 0x3a: /* XXX xword       */
			   if ((AReg>BReg) && (INT(BReg) >= 0))
			   {
				AReg = BReg;
			   }
			   else
			   {
				AReg = BReg - (2 * AReg);
			   }
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x3b: /* XXX sb          */
			   writebyte (AReg, BReg);
			   AReg = CReg;
			   IPtr++;
			   break;
		case 0x3c: /* gajw        */
                           /* XXX: proc prio toggle trick of AReg lsb=1       */
			   temp = AReg;
			   AReg = WPtr;
			   WPtr = temp;
			   IPtr++;
			   break;
		case 0x3d: /* savel       */
			   writeword (index (AReg, 0), FPtrReg1);
			   writeword (index (AReg, 1), BPtrReg1);
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x3e: /* saveh       */
			   writeword (index (AReg, 0), FPtrReg0);
			   writeword (index (AReg, 1), BPtrReg0);
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x3f: /* XXX wcnt        */
			   CReg = BReg;
			   BReg = AReg & 0x00000003;
			   AReg = AReg >> 2;
			   IPtr++;
			   break;
		case 0x40: /* shr         */
			   if (AReg < 32)
				AReg = BReg >> AReg;
			   else
				AReg = 0;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x41: /* shl         */
			   if (AReg < 32)
				AReg = BReg << AReg;
			   else
				AReg = 0;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x42: /* mint        */
			   CReg = BReg;
			   BReg = AReg;
			   AReg = 0x80000000;
			   IPtr++;
			   break;
		case 0x43: /* alt         */
                           if (emudebug)
			        printf ("-I-EMUDBG: alt: (W-3)=Enabling_p\n");
			   writeword (index (WPtr, State_s), Enabling_p);
			   IPtr++;
			   break;
		case 0x44: /* altwt       */
                           if (emudebug)
                           {
			        printf ("-I-EMUDBG: altwt(1): (W  )=NoneSelected_o\n");
			        printf ("-I-EMUDBG: altwt(2): (W-3)=#%08X\n", word (index (WPtr, State_s)));
                           }
			   writeword (index (WPtr, 0), NoneSelected_o);
			   IPtr++;
			   if ((word (index (WPtr, State_s))) != Ready_p)
			   {
				/* No guards are ready, so deschedule process. */
                                if (emudebug)
				        printf ("-I-EMUDBG: altwt(3): (W-3)=Waiting_p\n");
				writeword (index (WPtr, State_s), Waiting_p);
                                deschedule ();
			   }
			   break;
		case 0x45: /* altend      */
                           if (emudebug)
			        printf ("-I-EMUDBG: altend: IPtr+#%8X.\n", word (index (WPtr,0)));
			   IPtr++;
			   IPtr = IPtr + word (index (WPtr, 0));
			   break;
		case 0x46: /* and         */
			   AReg = BReg & AReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x47: /* enbt        */
                           if (emudebug)
			        printf ("-I-EMUDBG: enbt(1): Channel=%08X.\n", BReg);
			   if ((AReg == true_t) && (word (index (WPtr, TLink_s)) == TimeNotSet_p))
			   {
                                if (emudebug)
				        printf ("-I-EMUDBG: enbt(2): Time not yet set.\n");
				/* Set ALT time to this guard's time. */
				writeword (index (WPtr, TLink_s), TimeSet_p);
				writeword (index (WPtr, Time_s), BReg);
			   }
			   else if ((AReg == true_t) &&
                                    (word (index (WPtr, TLink_s)) == TimeSet_p) &&
                                    (INT(BReg - word (index (WPtr, Time_s))) >= 0))
			   {
                                if (emudebug)
				        printf ("-I-EMUDBG: enbt(2): Time already set earlier than or equal to this one.\n");
				/* ALT time is before this guard's time. Ignore. */
			   }
			   else if ((AReg == true_t) && (word (index (WPtr, TLink_s)) == TimeSet_p))
			   {
                                if (emudebug)
				        printf ("-I-EMUDBG: enbt(2): Time already set, but later than this one.\n");
				/* ALT time is after this guard's time. Replace ALT time. */
				writeword (index (WPtr, Time_s), BReg);
			   }
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x48: /* XXX: support ALT construct on Link0  enbc        */
                           if (emudebug)
			        printf ("-I-EMUDBG: enbc(1): Channel=#%08X.\n", BReg);
			   if ((AReg == true_t) && (word(BReg) == NotProcess_p))
			   {
                                if (emudebug)
				        printf ("-I-EMUDBG: enbc(2): Link or non-waiting channel.\n");
				/* Link or unwaiting channel. */
				if (BReg == Link0In)
				{
                                        if (emudebug)
					        printf ("-I-EMUDBG: enbc(3): Link.\n");
					/* Link. */
					if (FromServerLen > 0)
					{
                                                if (emudebug)
						        printf ("-I-EMUDBG: enbc(4): Ready link: (W-3)=Ready_p\n");
						writeword (index (WPtr, State_s), Ready_p);
					}
					else
					{
                                                if (emudebug)
						        printf ("-I-EMUDBG: enbc(4): Empty link: Initialise link.\n");
						writeword (BReg, Wdesc);
					}
				}
				else
					writeword (BReg, Wdesc);
			   }
			   else if ((AReg == true_t) && (word(BReg) == Wdesc))
			   {
                                if (emudebug) 
				        printf ("-I-EMUDBG: enbc(2): This process enabled the channel.\n");
				/* This process initialised the channel. Do nothing. */
				;
			   }
			   else if (AReg == true_t)
			   {
                                if (emudebug)
				        printf ("-I-EMUDBG: enbc(2): Waiting internal channel: (W-3)=Ready_p\n");
				/* Waiting internal channel. */
				writeword (index (WPtr, State_s), Ready_p);
			   }
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x49: /* enbs        */
			   if (AReg == true_t) writeword (index (WPtr, State_s), Ready_p);
			   IPtr++;
			   break;
		case 0x4a: /* move        */
			   for (temp=0;temp<AReg;temp++)
			   {
				writebyte ((BReg+temp), byte (CReg+temp));
			   }
			   IPtr++;
			   break;
		case 0x4b: /* or          */
			   AReg = BReg | AReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x4c: /* XXX csngl       */
			   if (((INT(AReg)<0) && (INT(BReg)!=-1)) ||
                               ((INT(AReg)>=0) && (BReg!=0)))
			   {
				SetError;
			   }
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x4d: /* ccnt1       */
			   if (BReg == 0)
			   {
				SetError;
			   }
			   else if (BReg > AReg)
			   {
				SetError;
			   }
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x4e: /* talt        */
			   writeword (index (WPtr, State_s), Enabling_p);
			   writeword (index (WPtr, TLink_s), TimeNotSet_p);
			   IPtr++;
			   break;
		case 0x4f: /* ldiff       */
			   t4_overflow = FALSE;
			   t4_carry = CReg & 0x00000001;
			   AReg = t4_sub32 (BReg, AReg);
			   BReg = t4_carry;
			   IPtr++;
			   break;
		case 0x50: /* sthb        */
			   BPtrReg0 = AReg;
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x51: /* taltwt      */
                           if (emudebug)
                           {
			        printf ("-I-EMUDBG: taltwt(1): (W  )=NoneSelected_o\n");
			        printf ("-I-EMUDBG: taltwt(2): (W-3)=#%08X\n", word (index (WPtr, State_s)));
                           }
			   writeword (index (WPtr, 0), NoneSelected_o);
			   IPtr++;
			   if ((word (index (WPtr, State_s))) != Ready_p)
			   {
				/* No guards are ready, so deschedule process, after putting time in timer queue. */
                                if (emudebug)
                                {
				        printf ("-I-EMUDBG: taltwt(3): (W-3)=Waiting_p\n");
				        printf ("-I-EMUDBG: taltwt(3): Waiting until #%8X.\n", word (index (WPtr, Time_s)));
                                }
				/* Put time into timer queue. */
				temp = word (index (WPtr, Time_s));
				insert (temp);

				writeword (index (WPtr, State_s), Waiting_p);
                                deschedule ();
			   }
			   break;
		case 0x52: /* sum         */
			   AReg = BReg + AReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x53: /* XXX mul         */
			   t4_overflow = FALSE;
			   t4_carry = 0;
			   AReg = t4_emul32 (BReg, AReg);
			   if (t4_overflow == TRUE)
				SetError;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x54: /* sttimer     */
			   ClockReg0 = AReg;
			   ClockReg1 = AReg;
			   Timers = TimersGo;
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x55: /* XXX stoperr     */
			   IPtr++;
			   if (ReadError)
			   {
                                deschedule ();
			   }
			   break;
		case 0x56: /* XXX cword       */
			   if ((BReg>=AReg) || (BReg>=(-AReg)))
			   {
				SetError;
			   }
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x57: /* clrhalterr  */
                           ClearHaltOnError;
			   IPtr++;
			   break;
		case 0x58: /* sethalterr  */
                           SetHaltOnError;
			   IPtr++;
			   break;
		case 0x59: /* testhalterr */
			   CReg = BReg;
			   BReg = AReg;
			   if (ReadHaltOnError)
			   {
				AReg = true_t;
			   }
			   else
			   {
				AReg = false_t;
			   }
			   IPtr++;
			   break;
		case 0x63: /* XXX unpacksn    */
			   temp = AReg;
			   CReg = BReg << 2;
			   AReg = (temp & 0x007fffff) << 8;
			   AReg = AReg | 0x80000000;
			   BReg = (temp & 0x7f800000) >> 23;
			   if (t4_iszero (temp))
				temp2 = 0x00000000;
			   else if (t4_isinf (temp))
				temp2 = 0x00000002;
			   else if (t4_isnan (temp))
				temp2 = 0x00000003;
			   else
				temp2 = 0x00000001;
			   CReg = (CReg & 0xfffffffc) | temp2;
			   IPtr++;
			   break;
		case 0x6c: /* XXX postnormsn  */
			   temp = (INT(word (index (WPtr, 0))) - INT(CReg));
			   if (temp > 0x000000ff)
				CReg = 0x000000ff;
			   else if (temp <= 0)
			   {
				temp = 1 - temp;
				CReg = 0;
				AReg = t4_shr64 (BReg, AReg, temp);
				BReg = t4_carry;
			   }
			   else
				CReg = temp;
			   IPtr++;
			   break;
		case 0x6d: /* XXX roundsn     */
			   temp = BReg & 0x00000080;
			   AReg = BReg & 0x7fffffff;
			   if (temp != 0)
			   {
				AReg = (BReg & 0x7fffffff) + 0x00000100;
				if ((AReg & 0x80000000) != 0)
					CReg++;
			   }
			   if (((CReg & 0x80000000) == 0) && (CReg >= 0x000000ff))
			   {
				AReg = t4_infinity ();
				CReg = BReg << 1;
			   }
			   else
			   {
				AReg = (AReg & 0x7fffff00) >> 8;
				temp = (CReg & 0x000001ff) << 23;
				AReg = AReg | temp;
				BReg = AReg;
				CReg = CReg >> 9;
			   }
			   IPtr++;
			   break;
		case 0x71: /* ldinf       */
			   CReg = BReg;
			   BReg = AReg;
			   AReg = t4_infinity ();
			   IPtr++;
			   break;
		case 0x72: /* XXX: missing round-to-nearest  fmul        */
			   t4_overflow = FALSE;
			   t4_carry = 0;
			   if ((AReg==0x80000000)&&(BReg==0x80000000))
				SetError;
			   AReg = t4_mul32 (AReg, BReg);
			   AReg = t4_shr64 (t4_carry, AReg, (uint32_t)31);
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x73: /* cflerr      */
			   if ((t4_isinf (AReg)) || (t4_isnan (AReg)))
				SetError;
			   IPtr++;
			   break;
		default  : printf ("-E-EMU414: Error - bad Icode!.\n");
	                   printf ("-E-EMU414: IPtr=%08X, Instr=%2X [%6s], OReg=%8X, AReg=%8X, BReg=%8X, CReg=%8X, WPtr=%08X. WPtr[0]=%8X\n", 
                                        IPtr, Instruction, 
                                        mnemonic (Icode, OReg),
                                        OReg, AReg, BReg, CReg, WPtr,word(WPtr));
			   handler (-1);
			   break;
	}
			   OReg = 0;
			   break;
		default  : printf ("-E-EMU414: Error - bad Icode!.\n");
	                   printf ("-E-EMU414: IPtr=%08X, Instr=%2X [%6s], OReg=%8X, AReg=%8X, BReg=%8X, CReg=%8X, WPtr=%08X. WPtr[0]=%8X\n", 
                                        IPtr, Instruction, 
                                        mnemonic (Icode, OReg),
                                        OReg, AReg, BReg, CReg, WPtr,word(WPtr));
			   handler (-1);
			   break;
	}
#ifdef PROFILE
		if (profiling)
			profile[0]++;
#endif
                /* Halt when Error flag was set */
		if ((!PrevError && ReadError) &&
                    (exitonerror || (ReadHaltOnError)))
			break;
		if (quit == TRUE)
			break;
	}

	if (ReadError)
	{
                if (ReadHaltOnError)
                        printf ("-I-EMU414: Transputer HaltOnError flag was set.\n");
		printf ("-I-EMU414: Transputer Error flag is set.\n");

                processor_state ();

		/* Save dump file for later debugging if needed. *****/
                printf ("-I-EMU414: Saving memory dump.\n");
                save_dump ();
                printf ("-I-EMU414: Done.\n");
	}
}


/* Add a process to the relevant priority process queue. */
void schedule (uint32_t wdesc)
{
        uint32_t wptr, pri;
	uint32_t ptr;
	uint32_t temp;

        wptr = GetDescWPtr(wdesc);
        pri  = GetDescPriority(wdesc);

        if (emudebug)
                printf ("-I-EMUDBG: Schedule(1): Process = #%08X at priority = %s\n", wptr, pri ? "Lo" : "Hi");

	/* Remove from timer queue if a ready alt. */
	temp = word (index (wptr, State_s));
	if (temp == Ready_p)
		purge_timer ();

	/* If a high priority process is being scheduled */
	/* while a low priority process runs, interrupt! */
	if ((pri == HiPriority) && (ProcPriority == LoPriority))
	{
                if (emudebug)
                        printf ("-I-EMUDBG: Schedule(2): Interrupt LoPriority process.\n");

		interrupt ();

                /* Preserve Error and HaltOnError flags only. */
                STATUSReg &= (ErrorFlag | HaltOnErrorFlag);

                /* ??? HaltOnErrorFlag is cleared before the process starts. */
                ClearHaltOnError;

		ProcPriority = HiPriority;
		WPtr = wptr;
		IPtr = word (index (WPtr, Iptr_s));

	}
	else
	{
		/* Get front of process list pointer. */
                if (emudebug)
                        printf ("-I-EMUDBG: Schedule(2): Get front of process list pointer.\n");
		if (pri == HiPriority)
		{
			ptr = FPtrReg0;
		}
		else
		{
			ptr = FPtrReg1;
		}

		if (ptr == NotProcess_p)
		{
                        if (emudebug)
                                printf ("-I-EMUDBG: Schedule(3): Empty process list, create.\n");

			/* Empty process list. Create. */
			if (pri == HiPriority)
			{
				FPtrReg0 = wptr;
				BPtrReg0 = wptr;
			}
			else
			{
				FPtrReg1 = wptr;
				BPtrReg1 = wptr;
			}
		}
		else
		{
			/* Process list already exists. Update. */
                        if (emudebug)
                                printf ("-I-EMUDBG: Schedule(3): Update process list.\n");

			/* Get workspace pointer of last process in list. */
			if (pri == HiPriority)
			{
				ptr = BPtrReg0;
			}
			else
			{
				ptr = BPtrReg1;
			}

			/* Link new process onto end of list. */
			writeword (index (ptr, Link_s), wptr);

			/* Update end-of-process-list pointer. */
			if (pri == HiPriority)
			{
				BPtrReg0 = wptr;
			}
			else
			{
				BPtrReg1 = wptr;
			}
		}
	}
}

/* Run a process, HiPriority if available. */
int run_process (void)
{
	uint32_t ptr;
	uint32_t lastptr;


        /* Let the current priority be unknown. */
        ProcPriority = NotProcess_p;

	/* Is the HiPriority process list non-empty? */
	if (FPtrReg0 != NotProcess_p)
	{
                if (emudebug)
	                printf ("-I-EMUDBG: RunProcess: HiPriority process list non-empty.\n");
		/* There is a HiPriority process available. */
		ProcPriority = HiPriority;
	}
        /* Is there an interrupted LoPriority process? */
        else if (ReadInterrupt)
        {
                if (emudebug)
	                printf ("-I-EMUDBG: RunProcess: There is an interrupted LoPriority process.\n");
                ProcPriority = LoPriority;
        }
	else if (FPtrReg1 != NotProcess_p)
	{
                if (emudebug)
	                printf ("-I-EMUDBG: RunProcess: LoPriority process list non-empty.\n");
		/* There are only LoPriority processes available. */
		ProcPriority = LoPriority;
	}

        /* Check current priority. */
        if (ProcPriority == NotProcess_p)
        {
                if (emudebug)
                        printf ("-I-EMUDBG: RunProcess: Empty process list. Cannot start!\n");
		/* Empty process list. Cannot start! */
                return (-1);
        }


	/* Get front of process list pointer. */
	if (ProcPriority == HiPriority)
	{
		ptr = FPtrReg0;
		lastptr = BPtrReg0;
	}
	else
	{
		ptr = FPtrReg1;
		lastptr = BPtrReg1;
	}

        if (emudebug)
	        printf ("-I-EMUDBG: RunProcess: ProcPriority = %s, ptr = #%08X. FPtrReg0 (Hi) = #%08X, FPtrReg1 (Lo) = #%08X.\n", 
                        ProcPriority ? "Lo" : "Hi",
                        ptr, FPtrReg0, FPtrReg1);

	if ((ProcPriority == LoPriority) && (ReadInterrupt))
	{
		/* Return to interrupted LoPriority process. */
		WPtr = GetDescWPtr(word (index (MostNeg, 11)));
		IPtr = word (index (MostNeg, 12));
		AReg = word (index (MostNeg, 13));
		BReg = word (index (MostNeg, 14));
		CReg = word (index (MostNeg, 15));
		STATUSReg = word (index (MostNeg, 16));
		/*EReg = word (index (MostNeg, 17));*/
                ClearInterrupt; /* XXX Not necessary ??? */
	}  
	else if (ptr == NotProcess_p)
	{
		/* Empty process list. Cannot start! */
		return (-1);
	}
	else
	{
		if (ptr == lastptr)
		{
			/* Only one process in list. */
			WPtr = ptr;

			/* Get Iptr. */
			IPtr = word (index (WPtr, Iptr_s));

			/* Empty list now. */
			if (ProcPriority == HiPriority)
			{
				FPtrReg0 = NotProcess_p;
			}
			else
			{
				FPtrReg1 = NotProcess_p;
			}
		}
		else
		{
			/* List. */
			WPtr = ptr;

			/* Get Iptr. */
			IPtr = word (index (WPtr, Iptr_s));

			/* Point at second process in chain. */
			if (ProcPriority == HiPriority)
			{
				FPtrReg0 = word (index (WPtr, Link_s));
			}
			else
			{
				FPtrReg1 = word (index (WPtr, Link_s));
			}
		}
	}

	return (0);
}

/* Start a process. */
void start_process (void)
{
        int active;

        if ((ProcPriority == LoPriority) && !IntEnabled)
                return;

        /* First, clear GotoSNP flag. */
        ClearGotoSNP;

	/* Second, handle any host link communication. */
        do
        {
                active = TRUE;
                if (0 == run_process ())
                        break;

                if (emudebug)
		        printf ("-I-EMUDBG: StartProcess: Empty process list. Update comms.\n");

                /* Update comms. */
		active = 0 != server ();

		/* Update timers, check timer queues. */
                active = active ||
                        (TPtrLoc0 != NotProcess_p) ||
                        (TPtrLoc1 != NotProcess_p);
		update_time ();
        } while (active);

        if (!active)
        {
                printf ("-E-EMU414: Error - stopped no Link/Process/Timer activity!\n");
                processor_state ();
                handler (-1);
        }

	/* Reset timeslice counter. */
	timeslice = 0;

#ifdef PROFILE
	if (profiling)
		profile[3]++;
#endif
}

/* Save the current process and start a new process. */
void deschedule (void)
{
        if (emudebug)
                printf ("-I-EMUDBG: Deschedule process #%08X.\n", Wdesc);
        /* Write Iptr into workspace */
	writeword (index (WPtr, Iptr_s), IPtr);

        /* Set StartNewProcess flag. */
        SetGotoSNP;
}

/* Save the current process and place it on the relevant priority process queue. */
void reschedule (void)
{
	/* Write Iptr into worksapce. */
	writeword (index (WPtr, Iptr_s), IPtr);

	/* Put on process list. */
	schedule (WPtr | ProcPriority);
}

/* Check whether the current process needs rescheduling,  */
/* i.e. has executed for a timeslice period.              */
void D_check (void)
{
	/* Called only from 'j' and 'lend'. */

	/* First, handle any host link communication. */
        if ((ProcPriority == HiPriority) || IntEnabled)
	        server ();

        /* High priority processes never timesliced. */
        if (ProcPriority == HiPriority)
                return;

	/* Check for timeslice. */
	if (timeslice > 1)
	{
                if (emudebug)
                        printf ("-I-EMUDBG: Timeslice process #%08X.\n", Wdesc);

		/* Must change process! */
		timeslice = 0;

		/* reschedule really moves the process to the end of the queue! */
		reschedule ();

		/* Set StartNewProcess flag. */
                SetGotoSNP;
	}
#ifdef PROFILE
	if (profiling)
		profile[1]++;
#endif
}

/* Interrupt a low priority process.                    */
/* Can only occur due to comms or timer becoming ready. */
void interrupt (void)
{
	/* A high priority process has become ready, interrupting a low priority one. */

	/* Sanity check. */
	if (ReadInterrupt)
	{
		printf ("-E-EMU414: Error - multiple interrupts of low priority processes!\n");
		handler (-1);
	}

	/* Store the registers. */
	writeword (index (MostNeg, 11), Wdesc);
	writeword (index (MostNeg, 12), IPtr);
	writeword (index (MostNeg, 13), AReg);
	writeword (index (MostNeg, 14), BReg);
	writeword (index (MostNeg, 15), CReg);
	writeword (index (MostNeg, 16), STATUSReg);
	/*writeword (index (MostNeg, 17), EReg);*/

        /* Note: that an interrupted process is not placed onto the scheduling lists. */
}

/* Insert a process into the relevant priority process queue. */
void insert (uint32_t time)
{
	uint32_t ptr;
	uint32_t nextptr;
	uint32_t timetemp;

	writeword (index (WPtr, Time_s), (time + 1));

	if (ProcPriority == HiPriority)
		ptr = TPtrLoc0;
	else
		ptr = TPtrLoc1;

	if (ptr == NotProcess_p)
	{
		/* Empty list. */
		/*writeword (ptr, WPtr); Strange! */
		writeword (index (WPtr, TLink_s), NotProcess_p);
		if (ProcPriority == HiPriority)
			TPtrLoc0 = WPtr;
		else
			TPtrLoc1 = WPtr;
	}
	else
	{
		/* One or more entries. */
		timetemp = word (index (ptr, Time_s));
		if (INT(timetemp - time) > 0)
		{
			/* Put in front of first entry. */
			writeword (index (WPtr, TLink_s), ptr);
			if (ProcPriority == HiPriority)
				TPtrLoc0 = WPtr;
			else
				TPtrLoc1 = WPtr;
		}
		else
		{
			/* Somewhere after the first entry. */
			/* Run along list until ptr is before the time and nextptr is after it. */
			nextptr = word (index (ptr, TLink_s));
			if (nextptr != NotProcess_p)
				timetemp = word (index (nextptr, Time_s));
			while ((INT(time - timetemp) > 0) && (nextptr != NotProcess_p))
			{
				ptr = nextptr;
				nextptr = word (index (ptr, TLink_s));
				if (nextptr != NotProcess_p)
					timetemp = word (index (ptr, Time_s));
			}

			/* Insert into list. */
			writeword (index (ptr, TLink_s), WPtr);
			writeword (index (WPtr, TLink_s), nextptr);
		}
	}
}

/* Purge a process from the timer queue, if it is there. */
void purge_timer (void)
{
	uint32_t ptr;
	uint32_t oldptr;

	/* Delete any entries at the beginning of the list. */
	if (ProcPriority == HiPriority)
	{
		while (TPtrLoc0 == WPtr)
		{
			TPtrLoc0 = word (index (WPtr, TLink_s));
		}

		ptr = TPtrLoc0;
		oldptr = ptr;
	}
	else
	{
		while (TPtrLoc1 == WPtr)
		{
			TPtrLoc1 = word (index (WPtr, TLink_s));
		}

		ptr = TPtrLoc1;
		oldptr = ptr;
	}

	/* List exists. */
	while (ptr != NotProcess_p)
	{
		if (ptr == WPtr)
		{
			ptr = word (index (ptr, TLink_s));
			writeword (index (oldptr, TLink_s), ptr);
		}
		else
		{
			oldptr = ptr;
			ptr = word (index (ptr, TLink_s));
		}
	}	
}


/* XXX Update time, check timer queues. */
INLINE void update_time (void)
{
        uint32_t temp3;
        struct timeval tv;
        unsigned long elapsed_usec;

	/* Move timers on if necessary, and increment timeslice counter. */
	count1++;
	if (count1 > 10)
	{
		count1 = 0;

                /* Check TOD clock, on UNIX ~ 1us resolution. */
                update_tod (&tv);

                /* Calculate elapsed usecs. */
                elapsed_usec = (tv.tv_sec  - LastTOD.tv_sec) * 1000000 +
                               (tv.tv_usec - LastTOD.tv_usec);
                
                /* Time not lapsed ? Return. */
                if (0 == elapsed_usec)
                        return;

                /* printf ("-I-EMUDBG: Elapsed time %lu.\n", elapsed_usec); */

                /* Update last known TOD clock. */
                LastTOD = tv;

		if (Timers == TimersGo)
                        ClockReg0 += elapsed_usec;

		count2 += elapsed_usec;

		/* Check high priority timer queue. */
                if ((ProcPriority == HiPriority) || IntEnabled)
                {
		        temp3 = word (index (TPtrLoc0, Time_s));
		        while ((INT(ClockReg0 - temp3) > 0) && (TPtrLoc0 != NotProcess_p))
		        {
			        schedule (TPtrLoc0 | HiPriority);

			        TPtrLoc0 = word (index (TPtrLoc0, TLink_s));
			        temp3 = word (index (TPtrLoc0, Time_s));
		        }
                }

		if (count2 > 64) /* ~ 64us */
		{
			if (Timers == TimersGo)
                                ClockReg1 += (count2 / 64);
			count3 += (count2 / 64);
			count2  =  count2 & 63;

			/* Check low priority timer queue. */
                        if ((ProcPriority == HiPriority) || IntEnabled)
                        {
			        temp3 = word (index (TPtrLoc1, Time_s));
			        while ((INT(ClockReg1 - temp3) > 0) && (TPtrLoc1 != NotProcess_p))
			        {
				        schedule (TPtrLoc1 | LoPriority);

				        TPtrLoc1 = word (index (TPtrLoc1, TLink_s));
				        temp3 = word (index (TPtrLoc1, Time_s));
			        }
                        }

			if (count3 > 16) /* ~ 1024us */
			{
				timeslice += (count3 / 16);
				count3     = count3 & 15;
			}
				
#ifdef __MWERKS__
			/* Check for events. */
			check_input();
#endif
		}
	}

}

/* Read a word from memory. */
uint32_t readword (uint32_t ptr)
{
	uint32_t result;
#ifdef LITTLE_ENDIAN
#warning Using little-endian access!
        uint32_t *wptr;

        wptr = (uint32_t *)(mem + (MEM_WORD_MASK & ptr));
        result = *wptr;
#else
	unsigned char val[4];

	/* Get bytes, ensuring memory references are in range. */
	val[0] = mem[(ptr & MEM_WORD_MASK)];
	val[1] = mem[(ptr & MEM_WORD_MASK)+1];
	val[2] = mem[(ptr & MEM_WORD_MASK)+2];
	val[3] = mem[(ptr & MEM_WORD_MASK)+3];

	result = (val[0]) | (val[1]<<8) | (val[2]<<16) | (val[3]<<24);
#endif

	return (result);
}

uint32_t word (uint32_t ptr)
{
        uint32_t result;

        result = readword (ptr);

        if (emumem)
                printf ("\tRD: Mem[%08X] ? %08X\n", ptr, result);
	return (result);
}

/* Write a word to memory. */
void writeword (uint32_t ptr, uint32_t value)
{
#ifdef LITTLE_ENDIAN
#warning Using little-endian access!
        uint32_t *wptr;

        wptr = (uint32_t *) (mem + (MEM_WORD_MASK & ptr));
        *wptr = value;
#else
	unsigned char val[4];

	val[0] = (value & 0x000000ff);
	val[1] = ((value & 0x0000ff00)>>8);
	val[2] = ((value & 0x00ff0000)>>16);
	val[3] = ((value & 0xff000000)>>24);

	/* Write bytes, ensuring memory references are in range. */
	mem[(ptr & MEM_WORD_MASK)]   = val[0];
	mem[(ptr & MEM_WORD_MASK)+1] = val[1];
	mem[(ptr & MEM_WORD_MASK)+2] = val[2];
	mem[(ptr & MEM_WORD_MASK)+3] = val[3];
#endif

        if (emumem)
                printf ("\tWR: Mem[%08X] ! %08X\n", ptr, value);

}

/* Read a byte from memory. */
unsigned char byte (uint32_t ptr)
{
	unsigned char result;


	/* Get byte, ensuring memory reference is in range. */
	result = mem[(ptr & MEM_BYTE_MASK)];

	return (result);
}

/* Write a byte to memory. */
INLINE void writebyte (uint32_t ptr, unsigned char value)
{
	/* Write byte, ensuring memory reference is in range. */
	mem[(ptr & MEM_BYTE_MASK)]   = value;
}

/* Add an executing instruction to the profile list. */
void add_profile (int32_t instruction)
{
	struct prof *current;
	struct prof *newprof;

	current = profile_head;

	if (current == NULL)
	{
		/* Create first entry in list. */
		newprof = (struct prof *) malloc (/*1, */sizeof (struct prof));
		if (newprof == NULL)
		{
			printf ("-E-EMU414: Error - ran out of memory in add_profile.\n");
			handler (-1);
		}
		profile_head = newprof;
		current = newprof;
		current->instruction = instruction;
		current->count = 1;
		current->prev = NULL;
		current->next = NULL;
	}
	else
	{
		/* Find right point in list. */
		while ((current->instruction < instruction) && (current->next != NULL))
		{
			current = current->next;
		}

		/* Either this is the correct entry, or one must be added into the list. */
		if (current->instruction == instruction)
		{
			/* Correct entry, increment count field. */
			current->count++;
		}
		else if (current->instruction > instruction)
		{
			/* Overshot, go back one element and splice in newprof element. */
			if (current == profile_head)
			{
				/* Add element into start of list. */
				newprof = (struct prof *) malloc (/*1, */sizeof (struct prof));
				if (newprof == NULL)
				{
					printf ("-E-EMU414: Error - ran out of memory in add_profile.\n");
					handler (-1);
				}
				newprof->instruction = instruction;
				newprof->count = 1;
				newprof->next = current;
				newprof->prev = NULL;
				current->prev = newprof;
				profile_head = newprof;
			}
			else
			{
				/* Insert newprof element into middle of list. */
				current = current->prev;
				newprof = (struct prof *) malloc (/*1, */sizeof (struct prof));
				if (newprof == NULL)
				{
					printf ("-E-EMU414: Error - ran out of memory in add_profile.\n");
					handler (-1);
				}
				newprof->instruction = instruction;
				newprof->count = 1;
				newprof->next = current->next;
				newprof->prev = current;
				current->next->prev = newprof;
				current->next = newprof;
			}
		}
		else if (current->next == NULL)
		{
			/* Add newprof element to the end of the list. */
			newprof = (struct prof *) malloc (/*1, */sizeof (struct prof));
			if (newprof == NULL)
			{
				printf ("-E-EMU414: Error - ran out of memory in add_profile.\n");
				handler (-1);
			}
			newprof->instruction = instruction;
			newprof->count = 1;
			newprof->next = NULL;
			newprof->prev = current;
			current->next = newprof;
		}
	}
}

void print_profile (void)
{
	struct prof *current;
	struct prof *old;
	extern FILE *ProfileFile;

	current = profile_head;

	if (current == NULL)
		printf ("-W-EMU414: No profile list!.\n");
	else
	{
		while (current->next != NULL)
		{
			if (current->instruction < 0)
				fprintf (ProfileFile, "Instruction %2X, executed %6d times.\n", (current->instruction + 0x100), current->count);
			else
				fprintf (ProfileFile, "Extended Instruction %4X, executed %6d times.\n", current->instruction, current->count);
			old = current;
			current = current->next;
			free (old);
		}
		if (current->instruction < 0)
			fprintf (ProfileFile, "Instruction %2X, executed %6d times.\n", (current->instruction + 0x100), current->count);
		else
			fprintf (ProfileFile, "Extended Instruction %4X, executed %6d times.\n", current->instruction, current->count);
		free (current);
	}
}
