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
#include "t4debug.h"

/*
 * p.c - hand inlined processor.c!
 *
 * The transputer emulator.
 *
 */
#ifdef _MSC_VER
#include "gettimeofday.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#include <sys/time.h>
#include <unistd.h>
#endif
#include <math.h>
#ifdef SPYNET
#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>
#else
#include <errno.h>
#define AF_SP 		0
#define NN_PUSH         0
#define NN_PULL         0
#define NN_POLLIN       0
#define NN_POLLOUT      0
#define NN_SOL_SOCKET   0 
#define NN_SNDTIMEO     0
#define NN_DONTWAIT     0
int nn_errno() { return EINVAL; }
char *nn_strerror(int errnum) { return strerror (errnum); }
int nn_socket(int domain, int protocol) { return EINVAL; }
int nn_close(int s) { return EBADF; }
int nn_bind(int s, const char *addr) { return EINVAL; }
int nn_connect(int s, const char *addr) { return EINVAL; }
int nn_shutdown(int s, int how) { return EINVAL; }
int nn_send(int s, const void *buf, size_t len, int flags) { return EINVAL; }
int nn_recv(int s, void *buf, size_t len, int flags) { return EINVAL; }
int nn_setsockopt(int s, int lvl, int opt, const void *optval, size_t optvallen) { return EINVAL; }
struct nn_pollfd {
        int fd;
        short events, revents;
};
int nn_poll(struct nn_pollfd *fds, int nfds, int opt) { return EINVAL; }
#endif
#include "netcfg.h"
#ifdef SHLINKS
#include "shlink.h"
#else
void* shlink_attach (const char *fnm, int size){return NULL;}
int shlink_detach (void *addr){return EINVAL;}
void* shlink_alloc (const char *fnm, int size){return NULL;}
int shlink_free (void){return EINVAL;}
#endif

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


/* Processor specific parameters. */
int Txxx = 414;
uint32_t MemStart    = 0x80000048;

/* Memory space. */
unsigned char *core;
uint32_t CoreSize    = 2 * 1024;
uint32_t ExtMemStart = 0x80000800;

unsigned char *mem;
uint32_t MemSize     = 1 << 21;
uint32_t MemWordMask = ((uint32_t)0x001ffffc);
uint32_t MemByteMask = ((uint32_t)0x001fffff);

#define InvalidInstr_p  ((uint32_t)0x2ffa2ffa)
#define Undefined_p     ((uint32_t)0xdeadbeef)

uint32_t word_int (uint32_t);

/* Registers. */
uint32_t IPtr;
uint32_t WPtr;
uint32_t AReg;
uint32_t BReg;
uint32_t CReg;
uint32_t OReg;

uint32_t WdescReg;

uint32_t DReg;                  /* undocumented DReg/EReg */
uint32_t EReg;

#define FP_UNKNOWN       0
#define FP_REAL32       32
#define FP_REAL64       64

typedef struct _REAL {
        uint32_t length;        /* FP_REAL32 or FP_REAL64 */
        uint32_t rsvd;
        union {
                fpreal32_t  sn;
                fpreal64_t  db;
        } u;
} REAL;

#define SN(reg)   (reg.u.sn)
#define DB(reg)   (reg.u.db)

REAL  FAReg;
REAL  FBReg;
REAL  FCReg;
REAL  FARegSave;
REAL  FBRegSave;
REAL  FCRegSave;
int   FP_Error;                 /* not preserved over descheduling */
int   RoundingMode;             /* current rounding mode */
int   ResetRounding;            /* reset rounding mode ? */

uint32_t m2dSourceStride;       /* move2d source stride */
uint32_t m2dDestStride;         /* move2d destination stride */
uint32_t m2dLength;             /* move2d length (no. of rows) */

/* Other registers. */
uint32_t ClockReg[2];
uint32_t TNextReg[2];
uint32_t TPtrLoc[2];            /* XXX 0x80000024 0x80000028 */

uint32_t FPtrReg[2];
uint32_t BPtrReg[2];

#define ProcessQEmpty           ((NotProcess_p == FPtrReg[0]) && (NotProcess_p == FPtrReg[1]))
#define TimerQEmpty             ((NotProcess_p == TPtrLoc[0]) && (NotProcess_p == TPtrLoc[1]))

uint32_t STATUSReg;             /* Processor flags: GotoSNPBit, HaltOnError, Error */

#define ClearInterrupt          writeword (0x8000002C, IdleProcess_p)
#define ReadInterrupt           (word (0x8000002C) != IdleProcess_p)

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


#define Temp_s          ( 0)
#define Iptr_s          (-1)
#define Link_s          (-2)
#define State_s         (-3)
#define Pointer_s       (-3)
#define TLink_s         (-4)
#define Time_s          (-5)


#define GetDescPriority(wdesc)  ((wdesc) & 0x00000001)
#define GetDescWPtr(wdesc)      ((wdesc) & 0xfffffffe)

#define BitsPerByte             8
#define BytesPerWord            4
#define ByteSelectMask          0x00000003
#define BitsPerWord             (BitsPerByte * BytesPerWord)
#define WordsRead(addr,len)     (((addr&(BytesPerWord-1))?1:0)+(len+(BytesPerWord-1))/BytesPerWord)
#define BytesRead(addr,len)     (WordsRead(addr,len)*BytesPerWord)

#ifdef NDEBUG
#define writeword(a,x)  writeword_int(a,x)
#define writebyte(a,x)  writebyte_int(a,x)
#define byte(a)         byte_int(a)
#define word(a)         word_int(a)
#endif

/* Internal variables. */
unsigned char Instruction;
unsigned char Icode;
unsigned char Idata;
int  Timers;
uint32_t t4_overflow;
uint32_t t4_carry;
uint32_t t4_normlen;
uint32_t t4_carry64;            /* shl64 shifted out bit */
uint32_t ProcPriority;
#define TimersGo   1
#define TimersStop 0
int loop;
int count1;
int count2;
int count3;
int timeslice;
int delayCount1 = 5;
int32_t quit = FALSE;
int32_t quitstatus;

/* Signal handler. */
void handler (int);

unsigned char *SharedLinks;

#define StrPrio(p)      ((p) ? "Lo" : "Hi")

void UpdateWdescReg (uint32_t wdesc)
{
        WdescReg     = wdesc;
        WPtr         = GetDescWPtr(wdesc);
        ProcPriority = GetDescPriority(wdesc);
}

#define Wdesc           WdescReg
#define Idle            (NotProcess_p == WPtr)
#define IdleProcess_p   (NotProcess_p | LoPriority)

/* External variables. */
extern int analyse;
extern int nodeid;
extern int verbose;
extern int serve;
extern int exitonerror;
extern int FromServerLen;
extern int emudebug;
extern int memdebug;
extern int memnotinit;
extern int msgdebug;
extern int cachedebug;
extern char NetConfigName[256];

LinkIface Link[4];

/* Macros. */
#define index(a,b)		((a)+(BytesPerWord*(b)))

/* Profile information. */
uint32_t instrprof[0x400];
/*
        #00 - #FF       primary instr.
        #100 - #2FF     secondary instr. OReg
        #300 - #3FF     fpentry
*/
uint32_t combinedprof[0x400][0x400];

typedef struct _DecoInstr {
  uint32_t IPtr;
  uint32_t NextIPtr;
  uint32_t OReg;
  unsigned char Icode;
  unsigned char Instruction;
} DecodedInstr;

#define IC_NOADDR       0xDEADBEEFU
#define MAX_ICACHE      16384
/* #define IHASH(x)        ((0xDEADBEEFU^(x))&(MAX_ICACHE-1)) */
#define IHASH(x)        ((x)&(MAX_ICACHE-1))
DecodedInstr Icache[MAX_ICACHE];
static void InvalidateAddr(uint32_t a)
{
        uint32_t x;
        // if (a == Icache[x = IHASH(a)].IPtr)
                Icache[IHASH(a)].IPtr = IC_NOADDR;
}

static void InvalidateRange(uint32_t a, uint32_t n)
{
        int i;
        for (i = 0; i < n; i++)
                InvalidateAddr(a++);
}

/* Support functions. */

#ifdef _MSC_VER
#define t4_bitcount(x)			__popcnt (x)
#endif

#ifdef __GNUC__
#define t4_bitcount(x)			__builtin_popcount (x)
#endif

#ifndef t4_bitcount
uint32_t t4_bitcount(uint32_t x)
{
        uint32_t result;

        result = 0;
        while (x)
        {
                if (x & 1)
                        result++;
                x >>= 1;
        }
        return result;
}
#endif

#ifdef __clang__
#define t4_bitreverse(x)        __builtin_bitreverse32 (x)
#endif

#ifndef t4_bitreverse
uint32_t t4_bitreverse (uint32_t x)
{
	unsigned int s = BitsPerWord;
	uint32_t mask = ~0;
	while ((s >>= 1) > 0)
	{
		mask ^= mask << s;
		x = ((x >> s) & mask) | ((x << s) & ~mask);
	}
	return x;
}
#endif

void fp_drop (void)
{
        FAReg = FBReg;
        FBReg = FCReg;
}

void fp_drop2 (void)
{
        FAReg = FCReg;
        FBReg = FCReg;
}

/* Pop a REAL64 from the floating point stack. */
void fp_popdb (fpreal64_t *fp)
{
#ifndef NDEBUG
        if (FAReg.length == FP_REAL64)
#endif
                *fp = DB(FAReg);
#ifndef NDEBUG
        else
        {
                printf ("-W-EMUFPU: Warning - FAReg is not REAL64! (fp_popdb)\n");
                *fp = DUndefined;
        }
#endif
        fp_drop ();
}


/* Peek two REAL64s on the floating point stack. */
void fp_peek2db (fpreal64_t *fb, fpreal64_t *fa)
{
#ifndef NDEBUG
        if (FBReg.length == FP_REAL64 && FAReg.length == FP_REAL64)
        {
#endif
                *fb = DB(FBReg);
                *fa = DB(FAReg);
#ifndef NDEBUG
        }
        else
        {
                printf ("-W-EMUFPU: Warning - FBReg/FAReg are not REAL64! (fp_peek2db)\n");
                *fb = DUndefined;
                *fa = DUndefined;
        }
#endif
}


/* Pop two REAL64s from the floating point stack. */
void fp_pop2db (fpreal64_t *fb, fpreal64_t *fa)
{
        fp_peek2db (fb, fa);
        fp_drop2 ();
}


/* Push a REAL64 to the floating point stack. */
void fp_pushdb (fpreal64_t fp)
{
        FCReg = FBReg;
        FBReg = FAReg;
        FAReg.length = FP_REAL64;
        DB(FAReg) = fp;
}


/* Pop a REAL32 from the floating point stack. */
void fp_popsn (fpreal32_t *fp)
{
#ifndef NDEBUG
        if (FP_REAL32 == FAReg.length)
#endif
                *fp = SN(FAReg);
#ifndef NDEBUG
        else
        {
                printf ("-W-EMUFPU: Warning - FAReg is not REAL32! (fp_popsn)\n");
                *fp = RUndefined;
        }
#endif
        fp_drop ();
}


/* Peek two REAL32s on the floating point stack. */
void fp_peek2sn (fpreal32_t *fb, fpreal32_t *fa)
{
#ifndef NDEBUG
        if (FBReg.length == FP_REAL32 && FAReg.length == FP_REAL32)
        {
#endif
                *fb = SN(FBReg);
                *fa = SN(FAReg);
#ifndef NDEBUG
        }
        else
        {
                printf ("-W-EMUFPU: Warning - FBReg/FAReg are not REAL64!\n");
                *fb = RUndefined;
                *fa = RUndefined;
        }
#endif
}


/* Pop two REAL32s from the floating point stack. */
void fp_pop2sn (fpreal32_t *fb, fpreal32_t *fa)
{
        fp_peek2sn (fb, fa);
        fp_drop2 ();
}


/* Push a REAL32 to the floating point stack. */
void fp_pushsn (fpreal32_t fp)
{
        FCReg = FBReg;
        FBReg = FAReg;
        FAReg.length = FP_REAL32;
        SN(FAReg) = fp;
}


/* Do a binary floating point operation. */
#ifdef T4RELEASE
#define fp_dobinary(dbop, snop) \
{ \
        fpreal64_t dbtemp1, dbtemp2; \
        fpreal32_t sntemp1, sntemp2; \
 \
        ResetRounding = TRUE; \
 \
        if (FP_REAL64 == FAReg.length) \
        { \
                fp_pop2db (&dbtemp1, &dbtemp2); \
                fp_pushdb (dbop (dbtemp1, dbtemp2)); \
        } \
        else \
        { \
                fp_pop2sn (&sntemp1, &sntemp2); \
                fp_pushsn (snop (sntemp1, sntemp2)); \
        } \
}
#else
void fp_dobinary (fpreal64_t (*dbop)(fpreal64_t,fpreal64_t),
                  fpreal32_t (*snop)(fpreal32_t,fpreal32_t))
{
        fpreal64_t dbtemp1, dbtemp2;
        fpreal32_t sntemp1, sntemp2;

        ResetRounding = TRUE;

        switch (FAReg.length)
        {
                case FP_REAL64:
                        fp_pop2db (&dbtemp1, &dbtemp2);
                        fp_pushdb (dbop (dbtemp1, dbtemp2));
                        break;
                case FP_REAL32:
                        fp_pop2sn (&sntemp1, &sntemp2);
                        fp_pushsn (snop (sntemp1, sntemp2));
                        break;
                default       :
                        /* Just pop 2 items and set FAReg to unknown. */
                        printf ("-W-EMUFPU: Warning - FAReg is undefined! (fp_dobinary)\n");
                        fp_drop2 ();
                        fp_pushdb (DUndefined);
                        FAReg.length = FP_UNKNOWN;
                        break;
        }
}
#endif


/* Do a binary floating point operation. */
int fp_binary2word (int (*dbop)(fpreal64_t,fpreal64_t),
                    int (*snop)(fpreal32_t,fpreal32_t))
{
        fpreal64_t dbtemp1, dbtemp2;
        fpreal32_t sntemp1, sntemp2;
        int result;

        ResetRounding = TRUE;

        switch (FAReg.length)
        {
                case FP_REAL64:
                        fp_pop2db (&dbtemp1, &dbtemp2);
                        result = dbop (dbtemp1, dbtemp2);
                        break;
                case FP_REAL32:
                        fp_pop2sn (&sntemp1, &sntemp2);
                        result = snop (sntemp1, sntemp2);
                        break;
                default       :
                        /* Just pop 2 items and set FAReg to unknown. */
                        printf ("-W-EMUFPU: Warning - FAReg is undefined! (fp_binary2word)\n");
                        fp_drop2 ();
                        result = FALSE;
                        break;
        }
        return result;
}


/* Do an unary floating point operation. */
#ifdef T4RELEASE
#define fp_dounary(dbop, snop) \
{ \
        fpreal64_t dbtemp; \
        fpreal32_t sntemp; \
 \
        ResetRounding = TRUE; \
 \
        if (FP_REAL64 == FAReg.length) \
        { \
                fp_popdb (&dbtemp); \
                fp_pushdb (dbop (dbtemp)); \
        } \
        else \
        { \
                fp_popsn (&sntemp); \
                fp_pushsn (snop (sntemp)); \
        } \
}
#else
void fp_dounary (fpreal64_t (*dbop)(fpreal64_t), fpreal32_t (*snop)(fpreal32_t))
{
        fpreal64_t dbtemp;
        fpreal32_t sntemp;

        ResetRounding = TRUE;

        switch (FAReg.length)
        {
                case FP_REAL64:
                        fp_popdb (&dbtemp);
                        fp_pushdb (dbop (dbtemp));
                        break;
                case FP_REAL32:
                        fp_popsn (&sntemp);
                        fp_pushsn (snop (sntemp));
                        break;
                default       :
                        /* Just pop 2 items and set FAReg to unknown. */
                        printf ("-W-EMUFPU: Warning - FAReg is undefined! (fp_dounary)\n");
                        fp_drop ();
                        fp_pushdb (DUndefined);
                        FAReg.length = FP_UNKNOWN;
                        break;
        }
}
#endif


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

#define C_UNKNOWN -1
#define C_POKE  0
#define C_PEEK  1
#define C_BOOT  2

static uint32_t BootLink = 0;
static int CtrlByte = C_UNKNOWN;

int handleboot (Channel *chan, unsigned char *data, int ndata)
{
        uint32_t address, value;
        Channel *outchan;

        EMUDBG2 ("-I-EMUDBG: Handle boot, control byte = %d.\n", CtrlByte);

        if (C_UNKNOWN == CtrlByte)
        {
                CtrlByte = data[0];
                chan->Address = MemStart;
                if (C_POKE == CtrlByte)
                        chan->Length = 8;
                else if (C_PEEK == CtrlByte)
                        chan->Length = 4;
                else
                {
                        chan->Length = CtrlByte;
                        CtrlByte = C_BOOT;
                        BootLink = chan->LinkAddress;
                }
                EMUDBG2 ("-I-EMUDBG: Control byte #%02X.\n", CtrlByte);
                data++; ndata--;
        }
        if (chan->Length)
        {
                int len = ndata;
                if (chan->Length < len)
                        len = chan->Length;
                writebytes_int (chan->Address, data, len);
                chan->Address += len; chan->Length -= len;
        }
        if (0 == chan->Length)
        {
                switch (CtrlByte)
                {
                        case C_POKE:
                                address = word_int (MemStart);
                                value   = word_int (MemStart + 4);
                                writeword_int (address, value);
                                CtrlByte = C_UNKNOWN;
                                break;
                        case C_PEEK:
                                address = word_int (MemStart);
                                value = word_int (address);
                                data[0] = value & 255; value >>= 8;
                                data[1] = value & 255; value >>= 8;
                                data[2] = value & 255; value >>= 8;
                                data[3] = value & 255;
                                outchan = &Link[chan->Link].Out;
                                ndata = nn_send (outchan->sock, data, ndata, 0);
                                if (4 != ndata)
                                {
                                        printf ("-E-EMU414: Failed to send %d bytes on Link%dOut (%s) @ handleboot()\n", ndata, outchan->Link, nn_strerror (nn_errno ()));
                                        handler (-1);
                                }
                                CtrlByte = C_UNKNOWN;
                                break;
                        case C_BOOT:
                                UpdateWdescReg (chan->Address | ProcPriority);
                                CtrlByte = C_UNKNOWN;
                                return 0;
                }
        }
        return 1;
}

int channel_ready (Channel *chan)
{
        struct nn_pollfd pfd[1];
        int ret;

        EMUDBG2 ("-I-EMUDBG: ChannelReady. Link%dIn ready ?\n", chan->Link);

        if (chan->schbuf)
        {
                ret = 0 != chan->schbuf[SCH_LEN];
                goto Exit;
        }

        pfd[0].fd = chan->sock;
        pfd[0].events = NN_POLLIN;
        pfd[0].revents = 0;

        EMUDBG2 ("-I-EMUDBG: Polling Link%dIn.\n", chan->Link);

        ret = nn_poll (pfd, 1, LTO_POLL);
        if (-1 == ret) /* error */
        {
                printf ("-E-EMU414: Failed polling Link%dIn (%s)\n", chan->Link, nn_strerror (nn_errno ()));
                handler (-1);
        }
Exit:
        if (0 == ret) /* timeout */
        {
                MSGDBG2 ("-I-EMUDBG: Link%dIn timeout.\n", chan->Link);
                return 1;
        }

        return 0;
}

int channel_recvP (Channel *chan, unsigned char *data, int doWait)
{
        int ret;

        if (chan->schbuf)
        {
                if (0 == chan->schbuf[SCH_LEN])
                {
                        errno = EAGAIN;
                        ret = -1;
                }
                else
                {
                        ret = chan->schbuf[SCH_LEN];
                        memcpy (data, &chan->schbuf[SCH_DATA], ret);
                        chan->schbuf[SCH_LEN] = 0;
                }
        }
        else
                ret = nn_recv (chan->sock, data, MAX_DATA, doWait ? 0 :NN_DONTWAIT);
        if (-1 == ret)
        {
                if ((EAGAIN == errno) && !doWait)
                        return -1;

                printf ("-E-EMU414: channel_recvP: Receive failed on Link%dIn (%s)\n",
                        chan->Link,
                        nn_strerror (nn_errno ()));
                handler (-1);
        }
        MSGDBG4 ("-I-EMUDBG: channel_recvP: Received %d bytes on Link%dIn (#%08X).\n",
                ret,
                chan->Link,
                chan->LinkAddress);
        return ret;
}

int channel_recvmemP (Channel *chan, unsigned char *data, int doMemWrite, int doWait)
{
        int ret;
        unsigned char *buf;

        buf = doMemWrite ? memrange (chan->Address, data, MAX_DATA) : data;
        ret = channel_recvP (chan, buf, doWait);
        if (ret < 0)
                return ret;

        if (doMemWrite)
        {
                if (buf == data)
                        writebytes_int (chan->Address, data, ret);
                else
                        InvalidateRange(chan->Address, ret);
                chan->Address += ret; chan->Length -= ret;
        }
        return ret;
}


int channel_sendP (Channel *chan, unsigned char *data, int ndata, int doWait)
{
        int ret;

        ret = 0;
        if (chan->schbuf)
        {
                if (chan->schbuf[SCH_LEN])
                {
                        errno = EAGAIN;
                        ret = -1;
                }
                else
                {
#ifndef NDEBUG
                        if (ndata > MAX_DATA)
                        {
                                printf ("-E-EMU414: channel_sendP: schbuf[] overflow! (%d).\n", ndata);
                        }
#endif
                        memcpy (&chan->schbuf[SCH_DATA], data, ndata);
                        chan->schbuf[SCH_LEN] = ndata;
                        ret = ndata;
                }
        }
        else
                ret = nn_send (chan->sock, data, ndata, doWait ? 0 : NN_DONTWAIT);

        if (-1 == ret)
        {
                if ((EAGAIN == errno) && !doWait)
                        return -1;

                printf ("-E-EMU414: channel_sendP: Send failed on Link%dOut (%s).\n",
                        chan->Link,
                        nn_strerror (nn_errno ()));
                handler (-1);
        }
        if (ret != ndata)
        {
                printf ("-E-EMU414: channel_sendP: Failed to send %d bytes on Link%dOut (%s).\n",
                        ndata,
                        chan->Link,
                        nn_strerror (nn_errno ()));
                handler (-1);
        }
        MSGDBG4 ("-I-EMUDBG: channel_sendP: Sent %d bytes on Link%dOut (#%08X).\n",
                ndata,
                chan->Link,
                chan->LinkAddress);
        return ndata;
}

int channel_sendmemP (Channel *chan, int doWait)
{
        unsigned char data[MAX_DATA];
        int ndata;
        unsigned char *buf;

        ndata = MAX_DATA;
        if (chan->Length < MAX_DATA)
                ndata = chan->Length;

        buf = bytes_int (chan->Address, data, ndata);
        chan->Address += ndata; chan->Length -= ndata;
        return channel_sendP (chan, buf, ndata, doWait);
}

/* Receive at most MAX_DATA bytes without waiting. */
int Precv_channel (Channel *chan, uint32_t address, uint32_t len)
{
        unsigned char data[MAX_DATA];

        if (len > MAX_DATA)
                return 1;

        chan->Address = address;
        chan->Length  = len;
#ifdef PROFILE
        if (profiling)
                chan->IOBytes += len;
#endif
        return channel_recvmemP (chan, data, TRUE, FALSE) < 0;
}

/* Send at most MAX_DATA bytes without waiting. */
int Psend_channel (Channel *chan, uint32_t address, uint32_t len)
{
        if (len > MAX_DATA)
                return 1;

        chan->Address = address;
        chan->Length  = len;
#ifdef PROFILE
        if (profiling)
                chan->IOBytes += len;
#endif
        return channel_sendmemP (chan, FALSE) < 0;
}

#define recv_channel(c,a,n)     1
#define send_channel(c,a,n)     Psend_channel(c,a,n)


void alt_channel(Channel *chan)
{
        uint32_t linkWdesc, linkWPtr, linkPtr, altState;
        linkWdesc = word (chan->LinkAddress);
	/* Ready. */
        linkWPtr  = GetDescWPtr(linkWdesc);
	altState = linkPtr = word (index (linkWPtr, State_s));
        MSGDBG2 ("-I-EMUDBG: Link(1): Memory address/ALT state=#%08X.\n", altState);
	if ((altState & 0xfffffffc) == MostNeg)
	{
	        /* ALT guard test - not ready to communicate. */
                MSGDBG ("-I-EMUDBG: Link(2): ALT guard test - not ready to communicate.\n");

	        /* The alt is waiting. Reschedule it? */
		if (altState != Ready_p)
		{
		        /* The alt has not already been rescheduled. */
#ifdef EMUDEBUG
                        if (msgdebug || emudebug)
                        {
                                printf ("-I-EMUDBG: Link(3): ALT state=Ready_p.\n");
                                printf ("-I-EMUDBG: Link(4): Reschedule ALT process (Wdesc=#%08X, IPtr=#%08X).\n",
                                        linkWdesc,
                                        word (index (linkWPtr, Iptr_s)));
                        }
#endif
                        /* NB. HiPrio process executes J between ENBC and ALTWT.
                         *     D_check() calls linkcomms().
                         * if (Wdesc == linkWdesc)
                         * {
                         *        printf ("-E-EMU414: schedule Wdesc=#%08X is running.\n", Wdesc);
                         *         handler (-1);
                         * }
                         */
                        /* Mark channel control word */
                        writeword_int (chan->LinkAddress, IdleProcess_p);

			writeword (index (linkWPtr, State_s), Ready_p);
                        if (Waiting_p == altState)
                                schedule (linkWdesc);
                }
        }
        else
        {
	        /* Ready. */
                MSGDBG ("-I-EMUDBG: Link(5): Ready, communicate.\n");
        }
}

#define SCH_POLL        50

int linkcomms (char *where, int doBoot, int timeOut)
{
        struct nn_pollfd pfd[8];
        int npfd;
        uint32_t linkWdesc;
        Channel *channels[8];
        unsigned char data[MAX_DATA];
        int ndata;
        int i, ret;
        int poll;
#ifdef EMUDEBUG
        char chnames[128], tmp[16];

        chnames[0] = '\0';
#endif

        if (nodeid < 0)
                return 0;

        MSGDBG2 ("-I-EMUDBG: Link comms %s.\n", where);
        npfd = 0;
        poll = TRUE;
        for (i = 0; i < 4; i++)
        {
                if ((0 == i) && serve) /* skip Host link */
                        continue;

                linkWdesc = word (Link[i].In.LinkAddress);
                if (doBoot || (linkWdesc != NotProcess_p))
                {
                        /* Select only BootLink */
                        if (doBoot && BootLink)
                        {
                                if (Link[i].In.LinkAddress != BootLink)
                                        continue;
                        }
                        pfd[npfd].fd = Link[i].In.sock;
                        pfd[npfd].events = NN_POLLIN;
                        pfd[npfd].revents = 0;
                        channels[npfd++] = &Link[i].In;
#ifdef EMUDEBUG
                        if (msgdebug || emudebug)
                        {
                                sprintf (tmp, " Link%dIn", i);
                                strcat (chnames, tmp);
                        }
#endif
                        poll = poll && (0 == Link[i].In.Length);
                }
                if (doBoot)
                        continue;

                linkWdesc = word (Link[i].Out.LinkAddress);
                if ((linkWdesc != NotProcess_p) && Link[i].Out.Length)
                {
                        pfd[npfd].fd = Link[i].Out.sock;
                        pfd[npfd].events = NN_POLLOUT;
                        pfd[npfd].revents = 0;
                        channels[npfd++] = &Link[i].Out;
#ifdef EMUDEBUG
                        if (msgdebug || emudebug)
                        {
                                sprintf (tmp, " Link%dOut", i);
                                strcat (chnames, tmp);
                        }
#endif
                        poll = FALSE;
                }
        }
        if (0 == npfd)
        {
                MSGDBG ("-I-EMUDBG: No active channels.\n");
                return 0;
        }
#ifdef EMUDEBUG
        if (msgdebug || emudebug)
        {       printf ("-I-EMUDBG: Channels:%s\n", chnames);
                printf ("-I-EMUDBG: Checking %d channels, timeout = %dms%s.\n",
                        npfd,
                        timeOut,
                        poll ? " (poll)" : "");
        }
#endif
        if (channels[0]->schbuf) /* shared channels? */
        {
                timeOut *= 1000;
                do
                {
                        ret = 0;
                        for (i = 0; i < npfd; i++)
                        {
                                int doadd = FALSE;
                                if (pfd[i].events & NN_POLLIN)
                                        doadd = 0 != channels[i]->schbuf[SCH_LEN];
                                else
                                        doadd = 0 == channels[i]->schbuf[SCH_LEN];
                                if (doadd)
                                {
                                        pfd[i].revents = pfd[i].events;
                                        ret++;
                                }
                        }
                        if (ret)
                                break;
                        if (timeOut)
                        {
                                usleep (SCH_POLL); timeOut -= SCH_POLL;
                                if (timeOut < 0) timeOut = 0;
                        }
                } while ((0 == ret) && (timeOut));
        }
        else
        {
                ret = nn_poll (pfd, npfd, timeOut);
        }
        if (0 == ret) /* timeout */
        {
                MSGDBG ("-I-EMUDBG: Comms timeout.\n");
                return doBoot ? 0 : 1;
        }
        if (-1 == ret) /* error */
        {
                printf ("-E-EMU414: Failed polling Links (%s)\n", nn_strerror (nn_errno ()));
                handler (-1);
        }
        for (i = 0; i < npfd; i++)
        {
                int revents;

                revents = 0;
                ndata = 0;
                if (0 == pfd[i].revents)
                        continue;

                if (pfd[i].revents & NN_POLLIN)
                {
                        revents = NN_POLLIN;
                        if (doBoot || channels[i]->Length)
                        {
                                ndata = ret = channel_recvmemP (channels[i], data, !doBoot, TRUE);
                                if (doBoot)
                                {
                                        if (0 == handleboot (channels[i], data, ret))
                                        {
                                                CReg = BootLink;
                                                return 1;
                                        }
                                        return 0;
                                }
                        }
#ifdef EMUDEBUG
                        else if (msgdebug || emudebug)
                                printf ("-I-EMUDBG: Polled on Link%dIn (#%08X).\n", channels[i]->Link, channels[i]->LinkAddress);
#endif
                }
                else if (pfd[i].revents & NN_POLLOUT)
                {
                        revents = NN_POLLOUT;
                        ndata = channel_sendmemP (channels[i], TRUE);
                }
                /* Still processing message? */
                if (channels[i]->Length)
                        continue;

                linkWdesc = word (channels[i]->LinkAddress);
                if ((0 == ndata) && (NN_POLLIN == revents))
                {
                        alt_channel (channels[i]);
                }
                else
                {
                        reset_channel (channels[i]->LinkAddress);
#ifndef NDEBUG
                        if (Wdesc == linkWdesc)
                        {
                                printf ("-E-EMU414: schedule Wdesc=#%08X is running.\n", Wdesc);
                                handler (-1);
                        }
#endif
                        schedule (linkWdesc);
                }
        }
        return 1;
}


/* Close link channels */
void close_channels (void)
{
        int i;

        /* see comment for NN_LINGER at
         *      https://nanomsg.org/v1.1.5/nn_setsockopt.html
         */
        sleep (1);

        if (SharedLinks)
        {
                shlink_detach (SharedLinks);
                if (0 == nodeid)
                        shlink_free ();
        }

        for (i = 0; i < 4; i++)
                if (-1 != Link[i].Out.sock)
                        nn_close (Link[i].Out.sock);

        for (i = 0; i < 4; i++)
                if (-1 != Link[i].In.sock)
                        nn_close (Link[i].In.sock);
}


/* Reset a link channel */
Channel *reset_channel (uint32_t addr)
{
        Channel *chan;
        int theLink;


        /* Reset channel control word. */
        writeword (addr, NotProcess_p);

        chan = (Channel *)0;
        theLink = TheLink(addr);
        if (IsLinkIn(addr))
        {
                chan = &Link[theLink].In;
        }
        else if (IsLinkOut(addr))
                chan = &Link[theLink].Out;

        if (!chan)
                return NULL;

        chan->Address = MostNeg;
        chan->Length  = 0;

        return chan;
}

/* Open a link channel */
void open_channel (uint32_t addr)
{
        Channel *chan;
        int chanIn, theLink;
        int othernode, otherlink;
        int ret;
        unsigned char *nodeBase;

        chan = reset_channel (addr);
        chanIn = IsLinkIn(addr);
        theLink = TheLink(addr);

        chan->LinkAddress = addr;
        chan->Link = theLink;
        chan->url[0] = '\0';
        chan->sock = -1;
        chan->schbuf = NULL;
        chan->IOBytes = 0;

        if (serve && 0 == theLink) /* host link */
                return;
        if (nodeid < 0) /* no Node ID */
                return;

        if (sharedLinks ())
        {
                int sharedSize;
                int maxnode;

                maxnode = maxNodeID ();
                sharedSize = 8 * SCH_SIZE * (1 + maxnode);
                if (NULL == SharedLinks)
                {
                        if ((1 == nodeid) || (0 == maxnode))
                                SharedLinks = shlink_alloc (NetConfigName, sharedSize);
                        else
                                SharedLinks = shlink_attach (NetConfigName, sharedSize);
                        if (NULL == SharedLinks)
                                handler (-1);
                }
                if (chanIn)
                {
                        nodeBase = SharedLinks + (nodeid * 8 * SCH_SIZE);
                        chan->schbuf = nodeBase + (4 + theLink) * SCH_SIZE;
                        if (verbose)
                                printf ("-I-EMU414: Link%dIn  at #%08lX %d:%d.\n",
                                        theLink,
                                        chan->schbuf - SharedLinks,
                                        nodeid, theLink);
                }
                else if (0 == connectedNetLink(nodeid, theLink, &othernode, &otherlink))
                {
                        nodeBase = SharedLinks + (othernode * 8 * SCH_SIZE);
                        chan->schbuf = nodeBase + (4 + otherlink) * SCH_SIZE;
                        if (verbose)
                                printf ("-I-EMU414: Link%dIn  at #%08lX %d:%d.\n",
                                        theLink,
                                        chan->schbuf - SharedLinks,
                                        othernode, otherlink);
                }
                return;
        }
        if (chanIn)
        {
                strcpy (&chan->url[0], netLinkURL (nodeid, theLink));
                if ((chan->sock = nn_socket (AF_SP, NN_PULL)) < 0)
                {
                        printf ("-E-EMU414: Error - Cannot create socket for Link%dIn\n", theLink);
                        handler (-1);
                }
                if ((ret = nn_bind (chan->sock, chan->url)) < 0)
                {
                        printf ("-E-EMU414: Error - Cannot bind Link%dIn to %s\n", theLink, chan->url);
                        handler (-1);
                }
                if (verbose)
                        printf ("-I-EMU414: Link%dIn  at %s\n", theLink, chan->url);
        }
        else if (0 == connectedNetLink(nodeid, theLink, &othernode, &otherlink)) /* only if connected to other node */
        {
                int send_timeout = 5000;

                strcpy (&chan->url[0], netLinkURL (othernode, otherlink));
                if ((chan->sock = nn_socket(AF_SP, NN_PUSH)) < 0)
                {
                        printf ("-E-EMU414: Error - Cannot create socket for Link%dOut (%s).\n", theLink, nn_strerror (nn_errno ()));
                        handler(-1);
                }
                if ((ret = nn_connect (chan->sock, chan->url)) < 0)
                {
                        printf ("-E-EMU414: Error - Cannot connect Link%dOut to %s (%s)\n", theLink, chan->url, nn_strerror (nn_errno ()));
                        handler (-1);
                }
                if ((ret = nn_setsockopt (chan->sock, NN_SOL_SOCKET, NN_SNDTIMEO, &send_timeout, sizeof(int))) < 0)
                {
                        printf ("-E-EMU414: Error - Cannot set SNDTIMEO on Link%dOut (%s)\n", theLink, nn_strerror (nn_errno ()));
                        handler (-1);
                }
                if (verbose)
                        printf ("-I-EMU414: Link%dOut at %s\n", theLink, chan->url);
        }
}

void print_fpreg (char *ident, char name, REAL *fpreg, int printempty)
{
        fpreal64_t r64;
        fpreal32_t r32;
        char tmp[32];

        /* Sync FP_Error with native FPU exceptions. */
        fp_syncexcept ();

        /* Here FP_Error is synchronized. */
        /* NativeFPU exceptions MAY or MAY NOT be cleared. */

#ifndef NDEBUG
        fp_chkexcept ("Enter print_fpreg ()");
#endif

        tmp[0] = '\0';
        if (fpreg->length == FP_REAL64)
        {
                r64 = fpreg->u.db;
                if (db_inf (r64.bits))
                        strcpy (tmp, "(inf)");
                else if (db_nan (r64.bits))
                        strcpy (tmp, "(nan)");
/*
                else
                        sprintf (tmp, "%.15le", r64.fp);
*/
                printf ("%sF%cReg          #%016llX   (%s)\n", ident, name, r64.bits, tmp);
        }
        else if (fpreg->length == FP_REAL32)
        {
                r32 = fpreg->u.sn;
                if (sn_inf (r32.bits))
                        strcpy (tmp, "(inf)");
                else if (sn_nan (r32.bits))
                        strcpy (tmp, "(nan)");
/*
                else
                        sprintf (tmp, "%.7e", r32.fp);
*/
                printf ("%sF%cReg                  #%08X   (%s)\n", ident, name, r32.bits, tmp);
        } 
        else if (printempty)
        {
                r64 = fpreg->u.db;
                printf ("%sF%cReg          #%016llX   (Empty)\n", ident, name, r64.bits);
        }

        fp_clrexcept ();

#ifndef NDEBUG
        fp_chkexcept ("Leave print_fpreg ()");
#endif
}

/* Print processor state. */
void processor_state (void)
{
        printf ("-I-EMU414: Processor state\n");
        printf ("\tIPtr           #%08X\n", IPtr);
        printf ("\tWPtr           #%08X\n", WPtr);
        printf ("\tAReg           #%08X\n", AReg);
        printf ("\tBReg           #%08X\n", BReg);
        printf ("\tCReg           #%08X\n", CReg);
        printf ("\tError          %s\n", ReadError ? "Set" : "Clear");
        printf ("\tHalt on Error  %s\n", ReadHaltOnError ? "Set" : "Clear");
        if (IsT800 || IsTVS)
        {
                print_fpreg ("\t", 'A', &FAReg, 1);
                print_fpreg ("\t", 'B', &FBReg, 1);
                print_fpreg ("\t", 'C', &FCReg, 1);
                fp_syncexcept ();
                printf ("\tFP_Error       %s\n", FP_Error ? "Set" : "Clear");
        }
        printf ("\tFPtr1 (Low     #%08X\n", FPtrReg[1]);
        printf ("\tBPtr1  queue)  #%08X\n", BPtrReg[1]);
        printf ("\tFPtr0 (High    #%08X\n", FPtrReg[0]);
        printf ("\tBPtr0  queue)  #%08X\n", BPtrReg[0]);
        printf ("\tTPtr1 (Timer   #%08X\n", TPtrLoc[1]);
        printf ("\tTPtr0  queues) #%08X\n", TPtrLoc[0]);
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
        bytesWritten = fwrite (mem, sizeof (unsigned char), MemSize, fout);
        if (bytesWritten != MemSize)
        {
                printf ("-E-EMU414: Error - failed to write dump file.\n");
                fclose (fout);
                unlink ("dump");
                handler (-1);
        }
        fclose (fout);
}

char *mnemonic(unsigned char icode, uint32_t oreg, uint32_t fpuentry, int onlymnemo)
{
        char *mnemo;
        char bad[16];
        static char str[32];

        mnemo = 0;
        if ((icode > 239) && (oreg != MostNeg))
        {
                if ((oreg == 0xab) && (fpuentry != MostNeg))
                {
                        if (fpuentry == 0x9c)
                                mnemo = "FPUCLRERR";
                        else if (fpuentry < 0x24)
                                mnemo = FpuEntries[fpuentry];
                        else
                        {
                                sprintf (bad, "--FPU%02X--", fpuentry);
                                mnemo = bad;
                        }
                }
                else if (oreg == 0x17c)
                        mnemo = "LDDEVID";
                else if (oreg == 0x1FF)
                        mnemo = "START";
                else if (oreg < 0xfa)
                        mnemo = Secondaries[oreg];

                if (mnemo == NULL)
                {
                        sprintf (bad, "--%02X--", oreg);
                        mnemo = bad;
                }
                sprintf (str, "%s", mnemo);
                return str;
        }

        if (onlymnemo)
                return Primaries[icode >> 4];

        sprintf (str, "%-7s #%X", Primaries[icode >> 4], oreg);
        return str;
}

void init_memory (void)
{
#ifndef NDEBUG
        unsigned int i;

        for (i = 0; i < CoreSize; i += 4)
                writeword_int (MostNeg + i, InvalidInstr_p);

        for (i = 0; i < MemSize; i += 4)
                writeword_int (ExtMemStart + i, InvalidInstr_p);
#endif
        return;
}

void init_processor (void)
{
        int i, j;

        for (i = 0; i < MAX_ICACHE; i++)
                Icache[i].IPtr = IC_NOADDR;

        SharedLinks = NULL;

        /* M.Bruestle 15.2.2012 */
        open_channel (Link0In);
        open_channel (Link1In);
        open_channel (Link2In);
        open_channel (Link3In);

        open_channel (Link0Out);
        open_channel (Link1Out);
        open_channel (Link2Out);
        open_channel (Link3Out);

        IPtr = MemStart;
        CReg = Link0In;
        TPtrLoc[0] = NotProcess_p;
        TPtrLoc[1] = NotProcess_p;
        ClearInterrupt; /* XXX not required ??? */

        /* Init TOD. */
        LastTOD.tv_sec  = 0;
        LastTOD.tv_usec = 0;
        update_tod (&LastTOD);


        if (IsT800 || IsTVS)
        {
                fp_init ();
                FAReg.length = FP_UNKNOWN;
                FBReg.length = FP_UNKNOWN;
                FCReg.length = FP_UNKNOWN;
        }

        /* ErrorFlag is in an indeterminate state on power up. */

#ifdef PROFILE
        if (profiling)
                for (i = 0; i < 0x400; i++)
                {
                        instrprof[i] = 0;
                        for (j = 0; j < 0x400; j++)
                                combinedprof[i][j] = 0;
                }
#endif
}

#define FLAG(x,y)       ((x) ? (y) : '-')


#ifdef EMUDEBUG
void checkWPtr (char *where, uint32_t wptr)
{
        if (wptr & ByteSelectMask)
        {
                if (emudebug)
                {
                        printf ("-W-EMU414: Warning - byte selector of WPtr should be zero! (%s)\n", where);
                        // handler (-1);
                }
        }
        if (NotProcess_p == wptr)
        {
                printf ("-E-EMU414: WPtr = NotProcess_p.\n");
                handler (-1);
        }
}

void checkWordAligned (char *where, uint32_t ptr)
{
        if (ptr & ByteSelectMask)
        {
                if (emudebug)
                {
                        printf ("-W-EMU414: Warning - byte selector of register should be zero! (%s)\n", where);
                        // handler (-1);
                }
        }
}
#endif

#ifdef PROFILE
static struct timeval StartTOD, EndTOD;
double ElapsedSecs;
#endif


void mainloop (void)
{
        uint32_t temp, temp2;
        uint32_t otherWdesc, otherWPtr, otherPtr, altState;
        uint32_t PrevError;
        unsigned char pixel;
        fpreal32_t sntemp1, sntemp2;
        fpreal64_t dbtemp1, dbtemp2;
        REAL       fptemp;
        unsigned short x;

#ifdef EMUDEBUG
        fpreal32_t r32temp;
        int   printIPtr, instrBytes;
        int   asmLines;
        int   currFPInstr, prevFPInstr;
        char *mnemo;

        printIPtr   = TRUE;
        prevFPInstr = FALSE;
        instrBytes  = 0;
        asmLines    = 0;
#endif
        m2dSourceStride = m2dDestStride = m2dLength = Undefined_p;

	count1 = 0;
	count2 = 0;
	count3 = 0;
	timeslice = 0;
	Timers = TimersStop;


#ifdef PROFILE
        if (profiling)
                update_tod (&StartTOD);
#endif
	while (1)
	{
#ifndef NDEBUG
                temp = temp2 = Undefined_p;
                otherWdesc = otherWPtr = otherPtr = altState = Undefined_p;
                dbtemp1 = dbtemp2 = DUndefined;
                sntemp1 = sntemp2 = RUndefined;
#endif
#ifdef EMUDEBUG
                r32temp = RUndefined;
#endif
                /* Save current value of Error flag */
                PrevError = ReadError;

		/* Move timers on if necessary, and increment timeslice counter. */
                if (++count1 > delayCount1)
		        update_time ();

                if (ReadGotoSNP)
                        start_process ();

		/* Execute an instruction. */
        ResetRounding = FALSE;

        if (IPtr == Icache[x = IHASH(IPtr)].IPtr)
        {
#ifdef PROFILE
                if (profiling)
                        profile[PRO_ICHIT]++;
#endif

                Icode = Icache[x].Icode;
                OReg  = Icache[x].OReg;
                IPtr  = Icache[x].NextIPtr;

#ifdef EMUDEBUG
                Instruction = Icache[x].Instruction;
                if (cachedebug)
                        printf ("-I-EMU414: Icache hit @ #%08X Icode = #%02X OReg = #%08X\n",
                                IPtr, Icode, OReg);
#endif
        }
        else
        {
#ifdef PROFILE
                if (profiling)
                        profile[PRO_ICMISS]++;
#endif

                x = IHASH(IPtr);
                Icache[x].IPtr = IPtr;
#ifdef EMUDEBUG
                if (cachedebug)
                        printf ("-I-EMU414: Icache miss @ #%08X\n", IPtr);
#endif

FetchNext:      Instruction = byte_int (IPtr);
	        Icode = Instruction & 0xf0;
	        Idata = Instruction & 0x0f;
	        OReg  = OReg | Idata;

#ifdef EMUDEBUG
                if (cachedebug)
                        printf ("-I-EMU414: Fetched @ #%08X Icode = #%02X Idata = #%02X OReg = #%08X\n",
                                        IPtr, Icode, Idata, OReg);
#endif

                if (0x20 == Icode)
                {
		        OReg = OReg << 4;
			IPtr++;
                        goto FetchNext;
                }
                else if (0x60 == Icode)
                {
			OReg = (~OReg) << 4;
			IPtr++;
                        goto FetchNext;
                }
                Icache[x].Icode = Icode;
                Icache[x].OReg  = OReg;
                Icache[x].NextIPtr = IPtr;

#ifdef EMUDEBUG
                Icache[x].Instruction = Instruction;
                if (cachedebug)
                        printf ("-I-EMU414: Cached Icode = #%02X OReg = #%08X\n", Icode, OReg);
#endif
        }

        /* Disable interrupts on PFIX or NFIX. */
        /* Using Icache IntEnabled is always TRUE. */

#ifndef NDEBUG
        if (Idle)
        {
                printf ("-E-EMU414: Processor is in IDLE state.\n");
                handler (-1);
        }
#endif

#ifdef EMUDEBUG
        if (emudebug)
        {
	        /* General debugging messages. */
                if (printIPtr)
                {
                        char statusCode = ' ';
                        if (0 == asmLines++ % 25)
                        {
                                if (IsT414)
                                        printf ("-IPtr------Code-----------------------Mnemonic------------HE---AReg-----BReg-----CReg-------WPtr-----WPtr[0]-\n");
                                else
                                        printf ("-IPtr------Code-----------------------Mnemonic------------HEFR---AReg-----BReg-----CReg-------WPtr-----WPtr[0]-\n");
                        }
                        if (HiPriority == ProcPriority)
                                statusCode = 'H';
                        if (Idle)
                                statusCode = 'I';
                        printf ("%c%08X: ", statusCode, IPtr);
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
                        mnemo = mnemonic (Icode, OReg, AReg, 0);
                        printf("%-17s", mnemo);
	                printf ("   %c%c", FLAG(ReadHaltOnError, 'H'), FLAG(      ReadError, 'E'));
                        if (IsT800 || IsTVS)
                        {
                                fp_syncexcept ();
	                        printf ("%c%c", FLAG(FP_Error, 'F'), RMODE[RoundingMode-1]);
                        }
                        printf ("   %8X %8X %8X   %08X %8X\n", 
                                AReg, BReg, CReg, WPtr, word_int (WPtr));
                        if (IsT800 || IsTVS)
                        {
                                currFPInstr = 0 == strncmp (mnemo, "FP", 2);
                                if (currFPInstr || prevFPInstr)
                                {
                                        print_fpreg("\t\t\t\t\t\t\t  ", 'A', &FAReg, 0);
                                        print_fpreg("\t\t\t\t\t\t\t  ", 'B', &FBReg, 0);
                                        print_fpreg("\t\t\t\t\t\t\t  ", 'C', &FCReg, 0);
                                        prevFPInstr = currFPInstr;
                                }
                        }
                        printIPtr = TRUE;
                }
                fflush (stdout);
        }
#endif

#ifdef PROFILE
	if (profiling && (0xF0 != Icode))
		add_profile (Icode);
#endif

	switch (Icode)
	{
		case 0x00: /* j     */
			   IPtr++;
			   IPtr = IPtr + OReg;
			   OReg = 0;
			   D_check();
			   break;
		case 0x10: /* ldlp  */
			   CReg = BReg;
			   BReg = AReg;
			   AReg = index (WPtr, OReg);
			   IPtr++;
			   OReg = 0;
			   break;
		case 0x20: /* pfix  */
			   OReg = OReg << 4;
			   IPtr++;
			   break;
		case 0x30: /* ldnl  */
                           T4DEBUG(checkWordAligned ("LDNL", AReg));
			   AReg = word (index (AReg, OReg));
			   IPtr++;
			   OReg = 0;
			   break;
		case 0x40: /* ldc   */
			   CReg = BReg;
			   BReg = AReg;
			   AReg = OReg;
			   IPtr++;
			   OReg = 0;
			   break;
		case 0x50: /* ldnlp */
                           /* NB. Minix demo uses unaligned AReg! */
                           T4DEBUG(checkWordAligned ("LDNLP", AReg));
			   AReg = index (AReg, OReg);
			   IPtr++;
			   OReg = 0;
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
			   OReg = 0;
			   break;
		case 0x80: /* adc   */
			   t4_overflow = FALSE;
			   t4_carry = 0;
			   AReg = t4_eadd32 (AReg, OReg);
			   if (t4_overflow == TRUE)
				SetError;
			   IPtr++;
			   OReg = 0;
			   break;
		case 0x90: /* call  */
			   IPtr++;
			   writeword (index (WPtr, -1), CReg);
			   writeword (index (WPtr, -2), BReg);
			   writeword (index (WPtr, -3), AReg);
			   writeword (index (WPtr, -4), IPtr);
                           UpdateWdescReg (index ( WPtr, -4) | ProcPriority);
                           T4DEBUG(checkWPtr ("CALL", WPtr));
			   AReg = IPtr;
                           /* Pop BReg. */
                           BReg = CReg;
			   IPtr = IPtr + OReg;
			   OReg = 0;
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
			   OReg = 0;
			   break;
		case 0xb0: /* ajw   */
                           UpdateWdescReg (index (WPtr, OReg) | ProcPriority);
                           T4DEBUG(checkWPtr ("AJW", WPtr));
			   IPtr++;
			   OReg = 0;
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
			   OReg = 0;
			   break;
		case 0xd0: /* stl   */
			   writeword (index (WPtr, OReg), AReg);
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   OReg = 0;
			   break;
		case 0xe0: /* XXX stnl  */
                           T4DEBUG(checkWordAligned ("STNL", AReg));
			   writeword (index (AReg, OReg), BReg);
			   AReg = CReg;
			   IPtr++;
			   OReg = 0;
			   break;
		case 0xf0: /* opr   */

#ifdef PROFILE
	if (profiling)
		add_profile (0x100 + OReg);
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
                                EMUDBG ("-I-EMUDBG: endp: Do successor process.\n");

				/* Do successor process. */
				UpdateWdescReg (AReg | ProcPriority);
                                T4DEBUG(checkWPtr ("ENDP", WPtr));
				IPtr = word (index (AReg, 0));
			   }
			   else
			   {
				/* Have not finished all parallel branches. */
                                EMUDBG2 ("-I-EMUDBG: endp: Waiting for parallel branches (%d).\n", temp);

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
                                MSGDBG ("-W-EMUDBG: Warning - doing IN on Link0Out.\n");
                                goto OprOut;
                           }
#ifdef PROFILE
                           if (profiling)
                                profile[PRO_CHANIN] += AReg;
#endif
			   MSGDBG4 ("-I-EMUDBG: in(1): Channel=#%08X, to memory at #%08X, length #%X.\n", BReg, CReg, AReg);
			   IPtr++;
			   if (!IsLinkIn(BReg))
			   {
				/* Internal communication. */
				otherWdesc = word (BReg);
                                MSGDBG2 ("-I-EMUDBG: in(2): Internal communication. Channel word=#%08X.\n", otherWdesc);
				if (otherWdesc == NotProcess_p)
				{
					/* Not ready. */
                                        MSGDBG ("-I-EMUDBG: in(3): Not ready.\n");
					writeword (BReg, Wdesc);
					writeword (index (WPtr, Pointer_s), CReg);
                                        deschedule ();
				}
				else
				{
					/* Ready. */
                                        otherWPtr = GetDescWPtr(otherWdesc);
                                        T4DEBUG(checkWPtr ("IN", otherWPtr));
					otherPtr = word (index (otherWPtr, Pointer_s));
					MSGDBG2 ("-I-EMUDBG: in(3): Transferring message from #%08X.\n", otherPtr);

                                        if (1 == AReg)
                                                writebyte_int (CReg, byte_int (otherPtr));
                                        else if (4 == AReg)
                                                writeword_int (CReg, word_int (otherPtr));
                                        else
                                                movebytes_int (CReg, otherPtr, AReg);
                                        CReg = CReg + BytesRead(CReg, AReg);
					writeword (BReg, NotProcess_p);
					schedule (otherWdesc);
				}
			   }
			   else
			   {
				/* Link communication. */
                                MSGDBG3 ("-I-EMUDBG: in(2): Link%d communication. Old channel word=#%08X.\n", TheLink(BReg), word (BReg));
                                if (serve && (Link0In == BReg))
                                        goto DescheduleIn;

                                if (recv_channel (&Link[TheLink(BReg)].In, CReg, AReg))
                                {
DescheduleIn:
				        writeword (BReg, Wdesc);
                                        Link[TheLink(BReg)].In.Address = CReg;
                                        Link[TheLink(BReg)].In.Length  = AReg;
#ifdef PROFILE
                                        if (profiling)
                                                Link[TheLink(BReg)].In.IOBytes += AReg;
#endif
                                        deschedule ();
                                }
			   }
			   break;
		case 0x08: /* prod        */
			   AReg = BReg * AReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x09: /* gt          */
			   if (INT32(BReg) > INT32(AReg))
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
                                MSGDBG ("-W-EMUDBG: Warning - doing OUT on Link0In.\n");
                                goto OprIn;
                           }
#ifdef PROFILE
                           if (profiling)
                                profile[PRO_CHANOUT] += AReg;
#endif
			   MSGDBG4 ("-I-EMUDBG: out(1): Channel=#%08X, length #%X, from memory at #%08X.\n", BReg, AReg, CReg);
			   IPtr++;
			   if (!IsLinkOut(BReg))
			   {
				/* Internal communication. */
				otherWdesc = word (BReg);
                                MSGDBG2 ("-I-EMUDBG: out(2): Internal communication. Channel word=#%08X.\n", otherWdesc);
				if (otherWdesc == NotProcess_p)
				{
					/* Not ready. */
                                        MSGDBG ("-I-EMUDBG: out(3): Not ready.\n");
					writeword (BReg, Wdesc);
					writeword (index (WPtr, Pointer_s), CReg);
                                        deschedule ();
				}
				else
				{
					/* Ready. */
                                        otherWPtr  = GetDescWPtr(otherWdesc);
					altState = otherPtr = word (index (otherWPtr, State_s));
                                        MSGDBG2 ("-I-EMUDBG: out(3): Memory address/ALT state=#%08X.\n", altState);
					if ((altState & 0xfffffffc) == MostNeg)
					{
						/* ALT guard test - not ready to communicate. */
                                                MSGDBG ("-I-EMUDBG: out(4): ALT guard test - not ready to communicate.\n");

						writeword (BReg, Wdesc);
						writeword (index (WPtr, Pointer_s), CReg);
                                                deschedule ();

						/* The alt is waiting. Rechedule it? */
						if (altState != Ready_p)
						{
							/* The alt has not already been rescheduled. */
#ifdef EMUDEBUG
                                                        if (msgdebug || emudebug)
                                                        {
                                                                printf ("-I-EMUDBG: out(5): ALT state=Ready_p.\n");
                                                                printf ("-I-EMUDBG: out(6): Reschedule ALT process (Wdesc=#%08X, IPtr=#%08X).\n",
                                                                                                otherWdesc, word (index (otherWPtr, Iptr_s)));
                                                        }
#endif
							writeword (index (otherWPtr, State_s), Ready_p);
							schedule (otherWdesc);
						}
          				}
					else
					{
						/* Ready. */
                                                MSGDBG ("-I-EMUDBG: out(4): Ready, communicate.\n");
                                                movebytes_int (otherPtr, CReg, AReg);
                                                CReg = CReg + BytesRead(CReg, AReg);
						writeword (BReg, NotProcess_p);
						schedule (otherWdesc);
					}
				}
			   }
			   else
			   {
				/* Link communication. */
                                MSGDBG3 ("-I-EMUDBG: out(2): Link%d communication. Old channel word=#%08X.\n", TheLink(BReg), word (BReg));

                                if (serve && (Link0Out == BReg))
                                        goto DescheduleOut;

                                if (send_channel (&Link[TheLink(BReg)].Out, CReg, AReg))
                                {
DescheduleOut:
				        writeword (BReg, Wdesc);
                                        Link[TheLink(BReg)].Out.Address = CReg;
                                        Link[TheLink(BReg)].Out.Length  = AReg;
#ifdef PROFILE
                                        if (profiling)
                                                Link[TheLink(BReg)].Out.IOBytes += AReg;
#endif
                                        deschedule ();
                                }
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
#ifdef PROFILE
                           if (profiling)
                                profile[PRO_CHANOUT]++;
#endif
			   MSGDBG2 ("-I-EMUDBG: outbyte: Channel=#%08X.\n", BReg);
			   IPtr++;
                           if (BReg == Link0In) /* M.Bruestle 22.1.2012 */
                           {
			        MSGDBG ("-W-EMUDBG: Warning - doing OUTBYTE on Link0In.\n");
				/* Link communication. */
				writeword (BReg, Wdesc);
				writeword (WPtr, AReg);
                                Link0InDest   = WPtr;
                                Link0InLength = 1;
#ifdef PROFILE
                                if (profiling)
                                        Link[0].In.IOBytes++;
#endif
                                deschedule ();
                           }
			   else if (!IsLinkOut(BReg))
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
                                        T4DEBUG(checkWPtr ("OUTBYTE", otherWPtr));
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
                                                CReg = otherPtr + BytesPerWord;
						schedule (otherWdesc);
					}
				}
			   }
			   else
			   {
				/* Link communication. */
				writeword (WPtr, AReg);

                                if (serve && (Link0Out == BReg))
                                        goto DescheduleOutByte;

                                if (send_channel (&Link[TheLink(BReg)].Out, WPtr, 1))
                                {
DescheduleOutByte:
				        writeword (BReg, Wdesc);
                                        Link[TheLink(BReg)].Out.Address = WPtr;
                                        Link[TheLink(BReg)].Out.Length  = 1;
#ifdef PROFILE
                                        if (profiling)
                                                Link[TheLink(BReg)].Out.IOBytes++;
#endif
                                        deschedule ();
                                }
			   }
			   break;
		case 0x0f: /* outword     */
#ifdef PROFLE
                           if (profiling)
                                profile[PRO_CHANOUT] += 4;
#endif
			   MSGDBG2 ("-I-EMUDBG: outword(1): Channel=#%08X.\n", BReg);
			   IPtr++;
                           if (BReg == Link0In) /* M.Bruestle 22.1.2012 */
                           {
			        MSGDBG2 ("-W-EMUDBG: Warning - doing OUTWORD on Link0In. Old channel word=#%08X.\n", word (BReg));
				/* Link communication. */
				writeword (BReg, Wdesc);
				writeword (WPtr, AReg);
                                Link0InDest   = WPtr;
                                Link0InLength = 4;
#ifdef PROFILE
                                if (profiling)
                                        Link[0].In.IOBytes += 4;
#endif
                                deschedule ();
                           }
			   else if (!IsLinkOut(BReg))
			   {
				/* Internal communication. */
				otherWdesc = word (BReg);
                                MSGDBG2 ("-I-EMUDBG: outword(2): Internal communication. Channel word=#%08X.\n", otherWdesc);
				if (otherWdesc == NotProcess_p)
				{
                                        MSGDBG ("-I-EMUDBG: outword(3): Not ready.\n");
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
                                        T4DEBUG(checkWPtr ("OUTWORD", otherWPtr));
					altState = otherPtr =  word (index (otherWPtr, State_s));
					if ((altState & 0xfffffffc) == MostNeg)
					{
						/* ALT guard test - not ready to communicate. */
                                                MSGDBG ("-I-EMUDBG: outword(3): ALT guard test - not ready.\n");

						writeword (BReg, Wdesc);
						writeword (WPtr, AReg);
						writeword (index (WPtr, Pointer_s), WPtr);
                                                deschedule ();

						/* The alt is waiting. Rechedule it? */
						if (altState != Ready_p)
						{
#ifdef EMUDEBUG
                                                        if (msgdebug || emudebug)
                                                        {
                                                                printf ("-I-EMUDBG: outword(4): ALT state=Ready_p.\n");
                                                                printf ("-I-EMUDBG: outword(5): Reschedule ALT process (Wdesc=#%08X, IPtr=#%08X).\n",
                                                                                                otherWdesc, word (index (otherWPtr, Iptr_s)));
                                                        }
#endif
							/* The alt has not already been rescheduled. */
							writeword (index (otherWPtr, State_s), Ready_p);
							schedule (otherWdesc);
						}
          				}
					else
					{
                                                MSGDBG ("-I-EMUDBG: outword(3): Ready.\n");

						/* Ready. */
						writeword (otherPtr, AReg);
						writeword (BReg, NotProcess_p);
                                                CReg = otherPtr + BytesPerWord;
						schedule (otherWdesc);
					}
				}
			   }
			   else
			   {
				/* Link communication. */
                                MSGDBG3 ("-I-EMUDBG: out(2): Link%d communication. Old channel word=#%08X.\n", TheLink(BReg), word (BReg));
				writeword (WPtr, AReg);
                                if (serve && (Link0Out == BReg))
                                        goto DescheduleOutWord;
                                if (send_channel (&Link[TheLink(BReg)].Out, WPtr, 4))
                                {
DescheduleOutWord:
				        writeword (BReg, Wdesc);
                                        Link[TheLink(BReg)].Out.Address = WPtr;
                                        Link[TheLink(BReg)].Out.Length  = 4;
#ifdef PROFILE
                                        if (profiling)
                                                Link[TheLink(BReg)].Out.IOBytes += 4;
#endif
                                        deschedule ();
                                }
			   }
			   break;
		case 0x10: /* seterr      */
			   SetError;
			   IPtr++;
			   break;
		case 0x12: /* XXX resetch     */
			   temp = AReg;
			   AReg = word (temp);
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
			   BPtrReg[1] = AReg;
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x18: /* sthf        */
			   FPtrReg[0] = AReg;
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
		case 0x1a: /* ldiv        */
			   if (CReg >= AReg)
			   {
                                AReg = BReg;
                                BReg = CReg;
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
                           CReg = BReg;
			   IPtr++;
			   break;
		case 0x1b: /* ldpi        */
			   IPtr++;
			   AReg = IPtr + AReg;
			   break;
		case 0x1c: /* stlf        */
			   FPtrReg[1] = AReg;
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x1d: /* xdble       */
			   CReg = BReg;
			   if (INT32(AReg) < 0)
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
		case 0x1f: /* rem         */
			   if (AReg==0)
                           {
                                AReg = BReg;
                                temp = CReg;
				SetError;
                           }
                           else if ((INT32(AReg)==-1) && (BReg==0x80000000))
                           {
                                AReg = 0x00000000;
                                temp = AReg;
                           }
			   else
                           {
				AReg = INT32(BReg) % INT32(AReg);
                                temp = abs (INT32(AReg));
                           }
                           BReg = CReg;
                           CReg = temp;
			   IPtr++;
			   break;
		case 0x20: /* ret         */
			   IPtr = word (WPtr);
			   UpdateWdescReg (index (WPtr, 4) | ProcPriority);
                           T4DEBUG(checkWPtr ("RET", WPtr));
			   break;
		case 0x21: /* lend        */ /****/
			   temp = word (index (BReg, 1));
			   IPtr++;
			   if (temp > 1)
			   {
				writeword (index (BReg, 1), (temp - 1));
                                CReg = word (BReg) + 1;
				writeword (BReg, CReg);
				IPtr =  IPtr - AReg;
				D_check();
			   }
			   else
			   {
                                CReg = 0;
				writeword (index (BReg, 1), (temp - 1));
			   }
			   break;
		case 0x22: /* ldtimer     */
			   CReg = BReg;
			   BReg = AReg;
			   AReg = ClockReg[ProcPriority];
			   IPtr++;
			   break;
                case 0x24: /* testlde     */
                           CReg = BReg;
                           BReg = AReg;
                           AReg = EReg;
                           IPtr++;
                           break;
                case 0x25: /* testldd     */
                           CReg = BReg;
                           BReg = AReg;
                           AReg = DReg;
                           IPtr++;
                           break;
                case 0x27: /* testste     */
                           EReg = AReg;
                           AReg = BReg;
                           BReg = CReg;
                           IPtr++;
                           break;
                case 0x28: /* teststd     */
                           DReg = AReg;
                           AReg = BReg;
                           BReg = CReg;
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
			   if (INT32(ClockReg[ProcPriority] - AReg) > 0)
			        ;
			   else
			   {
				insert (AReg);
                                deschedule ();;
			   }
			   break;
		case 0x2c: /* div         */
			   if ((AReg==0) || ((INT32(AReg)==-1)&&(BReg==0x80000000)))
                           {
                                temp = CReg;
				SetError;
                           }
			   else
                           {
                                temp  = abs (INT32(AReg));
                                temp2 = abs (INT32(BReg));
				AReg  = INT32(BReg) / INT32(AReg);
                                /* kudos to M.Bruestle */
                                temp  = temp2 - (abs (INT32(AReg)) | 1) * temp;
                           }
			   BReg = CReg;
                           CReg = temp;
			   IPtr++;
			   break;
		case 0x2e: /* XXX dist        */
			   EMUDBG2 ("-I-EMUDBG: dist(1): Time=%8X.\n", CReg);
			   temp = ClockReg[ProcPriority];
			   if ((BReg == true_t) &&
                               (INT32(temp - CReg)>=0) &&
                               (word (index (WPtr, Temp_s)) == NoneSelected_o))
			   {
				EMUDBG2 ("-I-EMUDBG: dist(2): Taking branch #%8X.\n", AReg);
				writeword (index (WPtr, Temp_s), AReg);
				AReg = true_t;
                                CReg = TimeNotSet_p;
			   }
			   else
			   {
				EMUDBG ("-I-EMUDBG: dist(2): Not taking this branch.\n");
				AReg = false_t;
			   }
			   IPtr++;
			   break;
		case 0x2f: /* XXX: support ALT construct on Link0 disc        */
			   EMUDBG2 ("-I-EMUDBG: disc(1): Channel=#%08X.\n", CReg);
                           if (serve && (CReg == Link0In))
          		   {
				EMUDBG ("-I-EMUDBG: disc(2): Link.\n");

				/* External link. */
          			if (FromServerLen > 0)
          				temp = TRUE;
          			else
          				temp = FALSE;
          		   }
          		   else
          		   {
                                otherWdesc = word (CReg);
#ifdef EMUDEBUG
                                if (emudebug)
                                {
				        printf ("-I-EMUDBG: disc(2): ");
                                        if (IsLinkIn(CReg))
				                printf ("Link%dIn.", TheLink(CReg));
                                        else
				                printf ("Internal channel.");
                                        printf (" Channel word=#%08X.\n", otherWdesc);
                                }
#endif
				/* Internal/Link channel. */
          			if (otherWdesc == NotProcess_p)
				{
                                        EMUDBG ("-I-EMUDBG: disc(3): Channel not ready.\n");

					/* Channel not ready. */
					temp = FALSE;
				}
				else if (otherWdesc == Wdesc)
				{
                                        EMUDBG ("-I-EMUDBG: disc(3): Channel not ready, but this process enabled it.\n");

					/* Channel not ready, but was inited by this process's enbc. */
                                        temp = FALSE;

					/* Reset channel word to NotProcess_p to avoid confusion. */
					writeword (CReg, NotProcess_p);
				}
          			else
				{
                                        EMUDBG ("-I-EMUDBG: disc(3): Channel ready.\n");

					/* Channel ready. */
          				temp = TRUE;
				}
          		   }
			   if ((BReg == true_t) &&
                               (temp == TRUE) &&
                               (word (index (WPtr, Temp_s)) == NoneSelected_o))
			   {
				EMUDBG2 ("-I-EMUDBG: disc(4): Taking branch #%8X.\n", AReg);
				writeword (index (WPtr, Temp_s), AReg);
				AReg = true_t;
			   }
			   else
			   {
				EMUDBG ("-I-EMUDBG: disc(3): Not taking this branch.\n");
				AReg = false_t;
			   }
			   IPtr++;
			   break;
		case 0x30: /* diss        */
			   if ((BReg == true_t) && (word (index (WPtr, Temp_s)) == NoneSelected_o))
			   {
				writeword (index (WPtr, Temp_s), AReg);
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
                           CReg = BReg;
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
			   AReg = AReg * BytesPerWord;
			   IPtr++;
			   break;
		case 0x35: /* lshr        */
			   AReg = t4_shr64 (CReg, BReg, AReg);
			   BReg = t4_carry;
                           CReg = BReg;
			   IPtr++;
			   break;
		case 0x36: /* lshl        */
			   AReg = t4_shl64 (CReg, BReg, AReg);
			   BReg = t4_carry;
                           CReg = BReg;
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
		case 0x3a: /* xword       */
                           /* ACWG preconditions */
                           /*   bitcount(AReg) = 1 /\ 2*AReg > BReg */
                           if (t4_bitcount (AReg) != 1)
                           {
                                AReg = Undefined_p;
                           }
                           else if (0 == (BReg & (~(AReg | (AReg - 1)))))
                           {
                                /* Bits are clear above the sign bit (AReg). */
                                if (AReg > BReg)
                                        AReg = BReg;
                                else
                                        AReg = BReg - (2*AReg);
                           }
                           else
                           {
                                /* T425 implementation. */
                                EMUDBG ("-W-EMU414: Warning - XWORD undefined behavior!\n");
                                if (0 == (AReg & BReg))
				        AReg = BReg;
			        else
                                        AReg = BReg | ~(AReg - 1);
                           }
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x3b: /* XXX sb          */
			   writebyte (AReg, BReg);
			   AReg = CReg;
                           CReg = 0;
			   IPtr++;
			   break;
		case 0x3c: /* gajw        */
                           /* XXX: proc prio toggle trick of AReg lsb=1       */
                           T4DEBUG(checkWordAligned ("GAJW", AReg));
			   temp = AReg;
			   AReg = WPtr;
			   UpdateWdescReg (temp | ProcPriority);
                           T4DEBUG(checkWPtr ("GAJW", WPtr));
			   IPtr++;
			   break;
		case 0x3d: /* savel       */
			   writeword (index (AReg, 0), FPtrReg[1]);
			   writeword (index (AReg, 1), BPtrReg[1]);
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x3e: /* saveh       */
			   writeword (index (AReg, 0), FPtrReg[0]);
			   writeword (index (AReg, 1), BPtrReg[0]);
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x3f: /* wcnt        */
			   CReg = BReg;
			   BReg = AReg & ByteSelectMask;
			   AReg = INT32(AReg) >> 2;
			   IPtr++;
			   break;
		case 0x40: /* shr         */
			   if (AReg < BitsPerWord)
				AReg = BReg >> AReg;
			   else
				AReg = 0;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x41: /* shl         */
			   if (AReg < BitsPerWord)
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
			   EMUDBG ("-I-EMUDBG: alt: (W-3)=Enabling_p\n");
			   writeword (index (WPtr, State_s), Enabling_p);
			   IPtr++;
			   break;
		case 0x44: /* altwt       */
#ifdef EMUDEBUG
                           if (emudebug)
                           {
			        printf ("-I-EMUDBG: altwt(1): (W  )=NoneSelected_o\n");
			        printf ("-I-EMUDBG: altwt(2): (W-3)=#%08X\n", word (index (WPtr, State_s)));
                           }
#endif
			   writeword (index (WPtr, Temp_s), NoneSelected_o);
			   IPtr++;
			   if ((word (index (WPtr, State_s))) != Ready_p)
			   {
				/* No guards are ready, so deschedule process. */
				EMUDBG ("-I-EMUDBG: altwt(3): (W-3)=Waiting_p\n");
				writeword (index (WPtr, State_s), Waiting_p);
                                deschedule ();
			   }
			   break;
		case 0x45: /* altend      */
			   EMUDBG2 ("-I-EMUDBG: altend: IPtr+#%8X.\n", word (index (WPtr,0)));
			   IPtr++;
			   IPtr = IPtr + word (index (WPtr, Temp_s));
			   break;
		case 0x46: /* and         */
			   AReg = BReg & AReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x47: /* enbt        */
			   EMUDBG2 ("-I-EMUDBG: enbt(1): Channel=%08X.\n", BReg);
			   if ((AReg == true_t) && (word (index (WPtr, TLink_s)) == TimeNotSet_p))
			   {
				EMUDBG ("-I-EMUDBG: enbt(2): Time not yet set.\n");
				/* Set ALT time to this guard's time. */
				writeword (index (WPtr, TLink_s), TimeSet_p);
				writeword (index (WPtr, Time_s), BReg);
			   }
			   else if ((AReg == true_t) &&
                                    (word (index (WPtr, TLink_s)) == TimeSet_p) &&
                                    (INT32(BReg - word (index (WPtr, Time_s))) >= 0))
			   {
				EMUDBG ("-I-EMUDBG: enbt(2): Time already set earlier than or equal to this one.\n");
				/* ALT time is before this guard's time. Ignore. */
			   }
			   else if ((AReg == true_t) && (word (index (WPtr, TLink_s)) == TimeSet_p))
			   {
				EMUDBG ("-I-EMUDBG: enbt(2): Time already set, but later than this one.\n");
				/* ALT time is after this guard's time. Replace ALT time. */
				writeword (index (WPtr, Time_s), BReg);
			   }
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x48: /* XXX: support ALT construct on Link0  enbc        */
			   EMUDBG2 ("-I-EMUDBG: enbc(1): Channel=#%08X.\n", BReg);
                           otherWdesc = word (BReg);
			   if ((AReg == true_t) && (otherWdesc == NotProcess_p))
			   {
				EMUDBG ("-I-EMUDBG: enbc(2): Link or non-waiting channel.\n");
				/* Link or unwaiting channel. */
                                int link = TheLink(BReg);
				if (serve && (BReg == Link0In))
				{
					EMUDBG ("-I-EMUDBG: enbc(3): Link0In.\n");
					/* Link. */
					if (FromServerLen > 0)
					{
						EMUDBG ("-I-EMUDBG: enbc(4): Ready link: (W-3)=Ready_p\n");
						writeword (index (WPtr, State_s), Ready_p);
					}
					else
					{
						EMUDBG ("-I-EMUDBG: enbc(4): Empty link: Initialise link.\n");
						writeword (BReg, Wdesc);
					}
				}
				else if (IsLinkIn(BReg))
                                {

					EMUDBG2 ("-I-EMUDBG: enbc(3): Link%dIn.\n", link);
                                        if (0 == channel_ready (&Link[link].In))
                                        {
						EMUDBG ("-I-EMUDBG: enbc(4): Ready link: (W-3)=Ready_p\n");
						writeword (index (WPtr, State_s), Ready_p);
                                                /* Mark channel control word */
					        writeword (BReg, IdleProcess_p);
                                        }
                                        else
                                        {
						EMUDBG ("-I-EMUDBG: enbc(4): Empty link: Initialise link.\n");
					        writeword (BReg, Wdesc);
                                        }
                                }
#ifndef NDEBUG
                                else if (IsLinkOut(BReg))
                                {
                                        printf ("-E-EMU414: enbc on Link%dOut.\n", link);
                                        handler (-1);
                                }
#endif
                                else
                                {
                                        /* Initialise with Wdesc. */
				        writeword (BReg, Wdesc);
                                }
			   }
			   else if ((AReg == true_t) && (otherWdesc == Wdesc))
			   {
				EMUDBG ("-I-EMUDBG: enbc(2): This process enabled the channel.\n");
				/* This process initialised the channel. Do nothing. */
				;
			   }
			   else if (AReg == true_t)
			   {
#ifndef NDEBUG
                                if (IsLinkIn(BReg))
                                {
                                        /* XXX Link hardware already marked it ??? */
                                        if (IdleProcess_p != otherWdesc)
                                        {
                                                printf ("-E-EMU414: enbc(2): Waiting Wdesc=#%08X on external Link%dIn.\n", otherWdesc, TheLink(BReg));
                                                handler (-1);
                                        }
                                }
#endif
				EMUDBG ("-I-EMUDBG: enbc(2): Waiting internal channel: (W-3)=Ready_p\n");
				/* Waiting internal channel. */
				writeword (index (WPtr, State_s), Ready_p);
			   }
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x49: /* enbs        */
                           /* Any non-zero value is valid. */
			   if (AReg)
                                writeword (index (WPtr, State_s), Ready_p);
			   IPtr++;
			   break;
		case 0x4a: /* move        */
                           if (INT32(AReg) > 0)
                           {
                                movebytes_int (BReg, CReg, AReg);
                                CReg = CReg + WordsRead(CReg, AReg) * BytesPerWord;
                           }
                           else
                           {
                                AReg = BReg = CReg = Undefined_p;
                           }
			   IPtr++;
			   break;
		case 0x4b: /* or          */
			   AReg = BReg | AReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x4c: /* csngl       */
			   if (((INT32(AReg)<0) && (INT32(BReg)!=-1)) ||
                               ((INT32(AReg)>=0) && (BReg!=0)))
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
			   BPtrReg[0] = AReg;
			   AReg = BReg;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x51: /* taltwt      */
#ifdef EMUDEBUG
                           if (emudebug)
                           {
			        printf ("-I-EMUDBG: taltwt(1): (W  )=NoneSelected_o\n");
			        printf ("-I-EMUDBG: taltwt(2): (W-3)=#%08X\n", word (index (WPtr, State_s)));
                           }
#endif
			   writeword (index (WPtr, Temp_s), NoneSelected_o);
			   IPtr++;
			   if ((word (index (WPtr, State_s))) != Ready_p)
			   {
				/* No guards are ready, so deschedule process, after putting time in timer queue. */
#ifdef EMUDEBUG
                                if (emudebug)
                                {
				        printf ("-I-EMUDBG: taltwt(3): (W-3)=Waiting_p\n");
				        printf ("-I-EMUDBG: taltwt(3): Waiting until #%8X.\n", word (index (WPtr, Time_s)));
                                }
#endif
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
		case 0x53: /* mul         */
			   t4_overflow = FALSE;
			   t4_carry = 0;
			   AReg = t4_emul32 (BReg, AReg);
			   if (t4_overflow == TRUE)
				SetError;
			   BReg = CReg;
			   IPtr++;
			   break;
		case 0x54: /* sttimer     */
			   ClockReg[0] = AReg;
			   ClockReg[1] = AReg;
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
		case 0x56: /* cword       */
                           if (t4_bitcount (AReg) != 1)
                                ;
                           else if (AReg==MostNeg)
                                ;
                           else if ((INT32(BReg)>=INT32(AReg)) || (INT32(BReg)<INT32(-AReg)))
			   {
                                /* ST20CORE implementation. */
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
		case 0x5a: /* dup    */
		           if (IsT414)
		               goto BadCode;
                           CReg = BReg;
                           BReg = AReg;
		           IPtr++;
		           break;
		case 0x5b: /* XXX move2dinit    */
		           if (IsT414)
		               goto BadCode;
                           m2dLength = AReg;
                           m2dDestStride = BReg;
                           m2dSourceStride = CReg;
		           IPtr++;
		           break;

#define m2dWidth                AReg
#define m2dDestAddress          BReg
#define m2dSourceAddress        CReg

		case 0x5c: /* XXX move2dall    */
		           if (IsT414)
		               goto BadCode;
                           if (INT32(m2dWidth) >= 0 && INT32(m2dLength) >= 0)
                           {
                                for (temp = 0; temp < m2dLength; temp++)
                                {
                                        movebytes_int (m2dDestAddress, m2dSourceAddress, m2dWidth);
                                        m2dSourceAddress += m2dSourceStride;
                                        m2dDestAddress   += m2dDestStride;
                                }
                           }
#ifndef NDEBUG
                           m2dSourceStride = m2dDestStride = m2dLength = Undefined_p;
#endif
		           IPtr++;
		           break;
		case 0x5d: /* XXX move2dnonzero    */
		           if (IsT414)
		               goto BadCode;
                           if (INT32(m2dWidth) >= 0 && INT32(m2dLength) >= 0)
                           {
                                for (temp = 0; temp < m2dLength; temp++)
                                {
                                        for (temp2 = 0; temp2 < m2dWidth; temp2++)
                                        {
                                                pixel = byte_int (m2dSourceAddress + temp2);
                                                if (pixel)
                                                        writebyte_int (m2dDestAddress + temp2, pixel);
                                        }
                                        m2dSourceAddress += m2dSourceStride;
                                        m2dDestAddress   += m2dDestStride;
                                }
                           }
#ifndef NDEBUG
                           m2dSourceStride = m2dDestStride = m2dLength = Undefined_p;
#endif
		           IPtr++;
		           break;
		case 0x5e: /* XXX move2dzero    */
		           if (IsT414)
		               goto BadCode;
                           if (INT32(m2dWidth) >= 0 && INT32(m2dLength) >= 0)
                           {
                                for (temp = 0; temp < m2dLength; temp++)
                                {
                                        for (temp2 = 0; temp2 < m2dWidth; temp2++)
                                        {
                                                pixel = byte_int (m2dSourceAddress + temp2);
                                                if (0 == pixel)
                                                        writebyte_int (m2dDestAddress + temp2, pixel);
                                        }
                                        m2dSourceAddress += m2dSourceStride;
                                        m2dDestAddress   += m2dDestStride;
                                }
                           }
#ifndef NDEBUG
                           m2dSourceStride = m2dDestStride = m2dLength = Undefined_p;
#endif
		           IPtr++;
		           break;

#undef m2dWidth
#undef m2dDestAddress
#undef m2dSourceAddress

		case 0x63: /* unpacksn    */
                           if (IsT800)
                                goto BadCode;
#ifdef EMUDEBUG
                           if (emudebug)
                           {
                                r32temp.bits = AReg;
                                printf ("\t\t\t\t\t\t\t  AReg %08X (%.7e)\n", AReg, r32temp.fp);
                           }
#endif
			   temp = AReg;
			   CReg = BReg << 2;
			   AReg = (temp & 0x007fffff) << 8;
			   BReg = (temp & 0x7f800000) >> 23;
			   if (t4_iszero (temp))
				temp2 = 0x00000000;
			   else if (t4_isinf (temp))
				temp2 = 0x00000002;
			   else if (t4_isnan (temp))
				temp2 = 0x00000003;
			   else if ((0 == BReg) && (0 != AReg))
                           {
                                /* Denormalised. */
                                temp2 = 0x00000001;
                                BReg = 1;
                           }
                           else
                           {
                                /* Normalised. */
				temp2 = 0x00000001;
                                AReg  = AReg | 0x80000000;
                           }
			   CReg = CReg | temp2;
			   IPtr++;
			   break;
		case 0x6c: /* postnormsn  */
                           if (IsT800)
                                goto BadCode;
			   temp = (INT32(word (index (WPtr, Temp_s))) - INT32(CReg));
                           if (INT32(temp) <= -BitsPerWord)
                           {
                                /* kudos to M.Bruestle: too small. */
                                AReg = BReg = CReg = 0;
                           }
                           else if (INT32(temp) > 0x000000ff)
				CReg = 0x000000ff;
			   else if (INT32(temp) <= 0)
			   {
				temp  = 1 - INT32(temp);
				CReg  = 0;
                                temp2 = AReg;
				AReg  = t4_shr64 (BReg, AReg, temp);
                                AReg  = AReg | temp2;
				BReg  = t4_carry;
			   }
			   else
				CReg = temp;
			   IPtr++;
			   break;
		case 0x6d: /* roundsn     */
                           if (IsT800)
                                goto BadCode;
			   if (INT32(CReg) >= 0x000000ff)
			   {
				AReg = t4_infinity ();
				CReg = BReg << 1;
			   }
			   else
			   {
                                /* kudos to M.Bruestle */
                                temp  = ((CReg & 0x000001ff) << 23)|((BReg & 0x7fffff00) >> 8);
                                if ((BReg & 0x80) == 0)
                                        AReg = temp;
                                else if ((AReg | (BReg & 0x7f)) != 0)
                                        AReg = temp + 1;
                                else
                                        AReg = temp + (temp & 1);
				BReg = AReg;
				CReg = CReg >> 9;
			   }
#ifdef EMUDEBUG
                           if (emudebug)
                           {
                                r32temp.bits = AReg;
                                printf ("\t\t\t\t\t\t\t  AReg %08X (%.7e)\n", AReg, r32temp.fp);
                           }
#endif
			   IPtr++;
			   break;
		case 0x71: /* ldinf       */
                           if (IsT800)
                                goto BadCode;
			   CReg = BReg;
			   BReg = AReg;
			   AReg = t4_infinity ();
			   IPtr++;
			   break;
		case 0x72: /* fmul        */
			   t4_overflow = FALSE;
			   t4_carry = 0;
			   if ((AReg==0x80000000)&&(BReg==0x80000000))
                           {
                                t4_carry = AReg;
				SetError;
                           }
                           else
                                AReg = t4_fmul (AReg, BReg);
			   BReg = CReg;
                           CReg = t4_carry;
			   IPtr++;
			   break;
		case 0x73: /* cflerr      */
                           if (IsT800)
                                goto BadCode;
			   if ((t4_isinf (AReg)) || (t4_isnan (AReg)))
				SetError;
			   IPtr++;
			   break;
		case 0x74: /* crcword    */
		           if (IsT414)
		               goto BadCode;
                           for (temp = 0; temp < BitsPerWord; temp++)
                           {
			        AReg = t4_shl64 (BReg, AReg, 1);
                                BReg = t4_carry;
                                if (t4_carry64)
                                        BReg = BReg ^ CReg;
                           }
                           AReg = BReg;
                           BReg = CReg;
                           CReg = AReg;
		           IPtr++;
		           break;
		case 0x75: /* crcbyte    */
		           if (IsT414)
		               goto BadCode;
                           /* Data must be in the most significant byte of the word. */
                           for (temp = 0; temp < BitsPerByte; temp++)
                           {
			        AReg = t4_shl64 (BReg, AReg, 1);
                                BReg = t4_carry;
                                if (t4_carry64)
                                        BReg = BReg ^ CReg;
                           }
                           AReg = BReg;
                           BReg = CReg;
                           CReg = AReg;
		           IPtr++;
		           break;
		case 0x76: /* bitcnt    */
		           if (IsT414)
		               goto BadCode;
                           temp = t4_bitcount (AReg);
                           AReg = temp + BReg;
                           BReg = CReg;
		           IPtr++;
		           break;
		case 0x77: /* bitrevword    */
		           if (IsT414)
		               goto BadCode;
                           AReg = t4_bitreverse (AReg);
		           IPtr++;
		           break;
		case 0x78: /* bitrevnbits    */
		           if (IsT414)
		               goto BadCode;
                           if (AReg == 0)
                                AReg = 0;
                           else if (AReg <= BitsPerWord)
                                AReg = t4_bitreverse (BReg) >> (BitsPerWord - AReg);
                           else
                           {
                                /* kudos to M.Bruestle */
                                EMUDBG ("-W-EMU414: Warning - BITREVNBITS undefined behavior!\n");
                                if (AReg >= 2 * BitsPerWord)
                                        temp = 0;
                                else
                                        temp = t4_bitreverse (BReg) << (AReg - BitsPerWord);
                                AReg = temp;
                           }
                           BReg = CReg;
		           IPtr++;
		           break;
                case 0x80: /* XXX fpsttest -- M.Bruestle  */
                           temp = FAReg.length;
                           if (FAReg.length == FP_REAL64)
                           {
                                fp_popdb (&dbtemp1);
                           }
                           else if (FAReg.length == FP_REAL32)
                           {
                                fp_popsn (&sntemp1);
                                dbtemp1.bits = sntemp1.bits;
                           }
                           else
                           {
                                printf ("-W-EMUFPU: Warning - FAReg is undefined! (fpsttest)\n");
                                temp    = FP_REAL64;
                                dbtemp1 = DUndefined;
                           }
                           dbtemp2 = fp_state (temp, dbtemp1, &temp2);
                           writereal64 (AReg, dbtemp2);
                           writeword (index (AReg, 2), temp2);
                           AReg = BReg;
                           BReg = CReg;
                           ResetRounding = TRUE;
                           IPtr++;
                           break;
		case 0x81: /* wsubdb    */
		           if (IsT414)
		               goto BadCode;
			   AReg = index (AReg, 2*BReg);
			   BReg = CReg;
		           IPtr++;
		           break;
		case 0x82: /* XXX fpldnldbi    */
		           if (IsT414)
		               goto BadCode;
                           T4DEBUG(checkWordAligned ("FPLDNLDBI", AReg));
                           fp_pushdb (real64 (index (AReg, 2*BReg)));
                           AReg = CReg;
                           BReg = CReg;
		           IPtr++;
		           break;
		case 0x83: /* fpchkerr    */
		           if (IsT414)
		               goto BadCode;
                           fp_syncexcept ();
                           if (FP_Error)
                           {
                                SetError;
                           }
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x84: /* fpstnldb    */
		           if (IsT414)
		               goto BadCode;
                           T4DEBUG(checkWordAligned ("FPSTNLDB", AReg));
#ifndef EMUDEBUG
                           if (FAReg.length == FP_REAL64)
#endif
                                fp_popdb (&dbtemp1);
#ifndef EMUDEBUG
                           else
                           {
                                printf ("-W-EMUFPU: Warning - FAReg is not REAL64! (fpstnldb)\n");
                                dbtemp1 = DUndefined;
                           }
#endif
                           writereal64 (AReg, dbtemp1);
                           AReg = BReg;
                           BReg = CReg;
		           IPtr++;
		           break;
                case 0x85: /* XXX fpldtest -- M.Bruestle */
                           dbtemp1 = real64 (AReg);
                           temp    = word (index (AReg, 2));
                           fp_setstate (dbtemp1, temp);
                           AReg = BReg;
                           BReg = CReg;
                           ResetRounding = TRUE; /* XXX */
                           IPtr++;
                           break;
		case 0x86: /* XXX fpldnlsni    */
		           if (IsT414)
		               goto BadCode;
                           T4DEBUG(checkWordAligned ("FPLDNLSNI", AReg));
                           fp_pushsn (real32 (index (AReg, BReg)));
                           AReg = CReg;
                           BReg = CReg;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x87: /* fpadd    */
		           if (IsT414)
		               goto BadCode;
                           fp_dobinary (fp_adddb, fp_addsn);
		           IPtr++;
		           break;
		case 0x88: /* fpstnlsn    */
		           if (IsT414)
		               goto BadCode;
                           T4DEBUG(checkWordAligned ("FPSTNLSN", AReg));
#ifndef EMUDEBUG
                           if (FAReg.length == FP_REAL32)
#endif
                                fp_popsn (&sntemp1);
#ifndef EMUDEBUG
                           else
                           {
                                printf ("-W-EMUFPU: Warning - FAReg is not REAL32! (fpstnlsn)\n");
                                sntemp1 = RUndefined;
                           }
#endif
                           writereal32 (AReg, sntemp1);
                           AReg = BReg;
                           BReg = CReg;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x89: /* fpsub    */
		           if (IsT414)
		               goto BadCode;
                           fp_dobinary (fp_subdb, fp_subsn);
		           IPtr++;
		           break;
		case 0x8a: /* fpldnldb    */
		           if (IsT414)
		               goto BadCode;
                           T4DEBUG(checkWordAligned ("FPLDNLDB", AReg));
                           fp_pushdb (real64 (AReg));
                           AReg = BReg;
                           BReg = CReg;
		           IPtr++;
		           break;
		case 0x8b: /* fpmul    */
		           if (IsT414)
		               goto BadCode;
                           fp_dobinary (fp_muldb, fp_mulsn);
		           IPtr++;
		           break;
		case 0x8c: /* fpdiv    */
		           if (IsT414)
		               goto BadCode;
                           fp_dobinary (fp_divdb, fp_divsn);
		           IPtr++;
		           break;
		case 0x8e: /* fpldnlsn    */
		           if (IsT414)
		               goto BadCode;
                           T4DEBUG(checkWordAligned ("FPLDNLSN", AReg));
                           fp_pushsn (real32 (AReg));
                           AReg = BReg;
                           BReg = CReg;
		           IPtr++;
		           break;
		case 0x8f: /* XXX fpremfirst    */
		           if (IsT414)
		               goto BadCode;
                           /* Do calculation at fpremfirst. Push true to AReg, to execute one more fpremstep. */
                           fp_dobinary (fp_remfirstdb, fp_remfirstsn);
                           CReg = BReg;
                           BReg = AReg;
                           AReg = true_t;
		           IPtr++;
		           break;
		case 0x90: /* XXX fpremstep    */
		           if (IsT414)
		               goto BadCode;
                           /* Do nothing here. Terminate loop with false. */
                           CReg = BReg;
                           BReg = AReg;
                           AReg = false_t;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x91: /* fpnan    */
		           if (IsT414)
		               goto BadCode;
                           temp = true_t;
                           if (FAReg.length == FP_REAL64)
                                temp = fp_nandb (DB(FAReg));
                           else if (FAReg.length == FP_REAL32)
                                temp = fp_nansn (SN(FAReg));
#ifndef EMUDEBUG
                           else
                                printf ("-W-EMUFPU: Warning - FAReg is undefined! (fpnan)\n");
#endif
                           CReg = BReg;
                           BReg = AReg;
                           AReg = temp ? true_t : false_t;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x92: /* fpordered    */
		           if (IsT414)
		               goto BadCode;
                           temp = false_t;
                           if (FAReg.length == FP_REAL64)
                           {
                                fp_peek2db (&dbtemp1, &dbtemp2);
                                temp = fp_ordereddb (dbtemp1, dbtemp2);
                           }
                           else if (FAReg.length == FP_REAL32)
                           {
                                fp_peek2sn (&sntemp1, &sntemp2);
                                temp = fp_orderedsn (sntemp1, sntemp2);
                           }
#ifndef EMUDEBUG
                           else
                                printf ("-W-EMUFPU: Warning - FAReg is undefined! (fpordered)\n");
#endif
                           CReg = BReg;
                           BReg = AReg;
                           AReg = temp;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x93: /* fpnotfinite    */
		           if (IsT414)
		               goto BadCode;
                           temp = true_t;
                           if (FAReg.length == FP_REAL64)
                                temp = fp_notfinitedb (DB(FAReg));
                           else if (FAReg.length == FP_REAL32)
                                temp = fp_notfinitesn (SN(FAReg));
#ifndef EMUDEBUG
                           else
                                printf ("-W-EMUFPU: Warning - FAReg is undefined! (fpnotfinite)\n");
#endif
                           CReg = BReg;
                           BReg = AReg;
                           AReg = temp ? true_t : false_t;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x94: /* fpgt    */
		           if (IsT414)
		               goto BadCode;
                           temp = fp_binary2word (fp_gtdb, fp_gtsn);
                           CReg = BReg;
                           BReg = AReg;
                           AReg = temp;
		           IPtr++;
		           break;
		case 0x95: /* fpeq    */
		           if (IsT414)
		               goto BadCode;
                           temp = fp_binary2word (fp_eqdb, fp_eqsn);
                           CReg = BReg;
                           BReg = AReg;
                           AReg = temp;
		           IPtr++;
		           break;
		case 0x96: /* fpi32tor32    */
		           if (IsT414)
		               goto BadCode;
                           temp = word (AReg);
                           fp_pushsn (fp_i32tor32 (temp));
                           AReg = BReg;
                           BReg = CReg;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x98: /* fpi32tor64    */
		           if (IsT414)
		               goto BadCode;
                           temp = word (AReg);
                           fp_pushdb (fp_i32tor64 (temp));
                           AReg = BReg;
                           BReg = CReg;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x9a: /* fpb32tor64    */
		           if (IsT414)
		               goto BadCode;
                           temp = word (AReg);
                           fp_pushdb (fp_b32tor64 (temp));
                           AReg = BReg;
                           BReg = CReg;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x9c: /* fptesterr    */
		           if (IsT414)
		               goto BadCode;
                           fp_syncexcept ();            /* Sync FP_Error with native FPU excp. */
                           if (FP_Error)
                                temp = false_t;
                           else
                                temp = true_t;
                           fp_clrexcept ();             /* Clear native FPU excp. */
                           FP_Error = FALSE;
                           CReg = BReg;
                           BReg = AReg;
                           AReg = temp;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x9d: /* fprtoi32    */
		           if (IsT414)
		               goto BadCode;
                           if (FAReg.length == FP_REAL64)
                                DB(FAReg) = fp_rtoi32db (DB(FAReg));
                           else if (FAReg.length == FP_REAL32)
                                SN(FAReg) = fp_rtoi32sn (SN(FAReg));
#ifndef EMUDEBUG
                           else
                           {
                                printf ("-W-EMUFPU: Warning - FAReg is undefined! (fprtoi32)\n");
                                FAReg.length = FP_UNKNOWN;
                                SN(FAReg)    = RUndefined;
                           }
#endif
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x9e: /* fpstnli32    */
		           if (IsT414)
		               goto BadCode;
                           if (FAReg.length == FP_REAL64)
                           {
                                fp_popdb (&dbtemp1);
                                temp = fp_stnli32db (dbtemp1);
                           }
                           else if (FAReg.length == FP_REAL32)
                           {
                                fp_popsn (&sntemp1);
                                temp = fp_stnli32sn (sntemp1);
                           }
#ifndef EMUDEBUG
                           else
                           {
                                printf ("-W-EMUFPU: Warning - FAReg is undefined! (fpstnli32)\n");
                                fp_drop ();
                                temp = Undefined_p;
                           }
#endif
                           writeword (AReg, temp);
                           AReg = BReg;
                           BReg = CReg;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0x9f: /* fpldzerosn    */
		           if (IsT414)
		               goto BadCode;
                           fp_pushsn (Zero);
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0xa0: /* fpldzerodb    */
		           if (IsT414)
		               goto BadCode;
                           fp_pushdb (DZero);
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0xa1: /* fpint    */
		           if (IsT414)
		               goto BadCode;
                           if (FAReg.length == FP_REAL64)
                                DB(FAReg) = fp_intdb (DB(FAReg));
                           else if (FAReg.length == FP_REAL32)
                                SN(FAReg) = fp_intsn (SN(FAReg));
#ifndef EMUDEBUG
                           else
                           {
                                printf ("-W-EMUFPU: Warning - FAReg is undefined! (fpint)\n");
                                FAReg.length = FP_UNKNOWN;
                                DB(FAReg)    = DUndefined;
                           }
#endif
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0xa3: /* fpdup    */
		           if (IsT414)
		               goto BadCode;
                           FCReg = FBReg;
                           FBReg = FAReg;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0xa4: /* fprev    */
		           if (IsT414)
		               goto BadCode;
                           fptemp = FAReg;
                           FAReg  = FBReg;
                           FBReg  = fptemp;
                           ResetRounding = TRUE;
		           IPtr++;
		           break;
		case 0xa6: /* fpldnladddb    */
		           if (IsT414)
		               goto BadCode;
                           T4DEBUG(checkWordAligned ("FPLDNLADDDB", AReg));
                           fp_pushdb (real64 (AReg));
#ifndef EMUDEBUG
                           if (FAReg.length == FP_REAL64)
#endif
                           {
                                fp_pop2db (&dbtemp1, &dbtemp2);
                                fp_pushdb (fp_adddb (dbtemp1, dbtemp2));
                           }
#ifndef EMUDEBUG
                           else
                           {
                                printf ("-W-EMUFPU: Warning - FAReg is not REAL64! (fpldnladddb)\n");
                                fp_drop ();
                                FAReg.length = FP_UNKNOWN;
                                DB(FAReg)    = DUndefined;
                           }
#endif
                           AReg = BReg;
                           BReg = CReg;
		           IPtr++;
		           break;
		case 0xa8: /* fpldnlmuldb    */
		           if (IsT414)
		               goto BadCode;
                           T4DEBUG(checkWordAligned ("FPLDNLMULDB", AReg));
                           fp_pushdb (real64 (AReg));
#ifndef EMUDEBUG
                           if (FAReg.length == FP_REAL64)
#endif
                           {
                                fp_pop2db (&dbtemp1, &dbtemp2);
                                fp_pushdb (fp_muldb (dbtemp1, dbtemp2));
                           }
#ifndef EMUDEBUG
                           else
                           {
                                printf ("-W-EMUFPU: Warning - FAReg is not REAL64! (fpldnlmuldb)\n");
                                fp_drop ();
                                FAReg.length = FP_UNKNOWN;
                                DB(FAReg)    = DUndefined;
                           }
#endif
                           AReg = BReg;
                           BReg = CReg;
		           IPtr++;
		           break;
		case 0xaa: /* fpldnladdsn    */
		           if (IsT414)
		               goto BadCode;
                           T4DEBUG(checkWordAligned ("FPLDNLADDSN", AReg));
                           fp_pushsn (real32 (AReg));
#ifndef EMUDEBUG
                           if (FAReg.length == FP_REAL32)
#endif
                           {
                                fp_pop2sn (&sntemp1, &sntemp2);
                                fp_pushsn (fp_addsn (sntemp1, sntemp2));
                           }
#ifndef EMUDEBUG
                           else
                           {
                                printf ("-W-EMUFPU: Warning - FAReg is not REAL32! (fpldnladdsn)\n");
                                fp_drop ();
                                FAReg.length = FP_UNKNOWN;
                                SN(FAReg)    = RUndefined;
                           }
#endif
                           AReg = BReg;
                           BReg = CReg;
		           IPtr++;
		           break;
		case 0xab: /* fpentry    */
		           if (IsT414)
		               goto BadCode;
                           temp = AReg;
                           AReg = BReg;
                           BReg = CReg;
		           IPtr++;

#ifdef PROFILE
                           if (profiling)
                                add_profile (0x300 + temp);
#endif

                           switch (temp) {
			   case 0x01: /* fpusqrtfirst    */
                                      if (FAReg.length == FP_REAL64)
                                          DB(FAReg) = fp_sqrtfirstdb (DB(FAReg));
                                      else if (FAReg.length == FP_REAL32)
                                          SN(FAReg) = fp_sqrtfirstsn (SN(FAReg));
#ifndef EMUDEBUG
                                      else
                                      {
                                          printf ("-W-EMUFPU: Warning - FAReg is undefined! (fpusqrtfirst)\n");
                                          FAReg.length = FP_UNKNOWN;
                                          DB(FAReg)    = DUndefined;
                                      }
#endif
                                      FBReg.length = FP_UNKNOWN;
                                      FCReg.length = FP_UNKNOWN;
                                      ResetRounding = TRUE;
			              break;
			   case 0x02: /* fpusqrtstep    */
                                      FBReg.length = FP_UNKNOWN;
                                      FCReg.length = FP_UNKNOWN;
                                      ResetRounding = TRUE;
			              break;
			   case 0x03: /* fpusqrtlast    */
                                      fp_dounary (fp_sqrtlastdb, fp_sqrtlastsn);
			              break;
			   case 0x04: /* fpurp    */
                                      fp_setrounding ("fpurp", ROUND_P);
                                      /* Do not reset rounding mode. */
			              break;
			   case 0x05: /* fpurm    */
                                      fp_setrounding ("fpurm", ROUND_M);
                                      /* Do not reset rounding mode. */
			              break;
			   case 0x06: /* fpurz    */
                                      fp_setrounding ("fpurz", ROUND_Z);
                                      /* Do not reset rounding mode. */
			              break;
			   case 0x07: /* fpur32tor64    */
#ifndef EMUDEBUG
                                      if (FAReg.length == FP_REAL32)
#endif
                                      {
                                          FAReg.length = FP_REAL64;
                                          DB(FAReg)    = fp_r32tor64 (SN(FAReg));
                                      }
#ifndef EMUDEBUG
                                      else
                                      {
                                          printf ("-W-EMUFPU: Warning - FAReg is not REAL32! (fpur32tor64)\n");
                                          FAReg.length = FP_UNKNOWN;
                                          DB(FAReg)    = DUndefined;
                                      }
#endif
                                      ResetRounding = TRUE;
			              break;
			   case 0x08: /* fpur64tor32    */
#ifndef EMUDEBUG
                                      if (FAReg.length == FP_REAL64)
#endif
                                      {
                                          FAReg.length = FP_REAL32;
                                          SN(FAReg) = fp_r64tor32 (DB(FAReg));
                                      }
#ifndef EMUDEBUG
                                      else
                                      {
                                          printf ("-W-EMUFPU: Warning - FAReg is not REAL64! (fpur64tor32)\n");
                                          FAReg.length = FP_UNKNOWN;
                                          SN(FAReg)    = RUndefined;
                                      }
#endif
                                      ResetRounding = TRUE;
			              break;
			   case 0x09: /* fpuexpdec32    */
                                      fp_dounary (fp_expdec32db, fp_expdec32sn);
			              break;
			   case 0x0a: /* fpuexpinc32    */
                                      fp_dounary (fp_expinc32db, fp_expinc32sn);
			              break;
			   case 0x0b: /* fpuabs    */
                                      fp_dounary (fp_absdb, fp_abssn);
			              break;
			   case 0x0d: /* fpunoround    */
#ifndef EMUDEBUG
                                      if (FAReg.length == FP_REAL64)
#endif
                                      {
                                          FAReg.length = FP_REAL32;
                                          SN(FAReg) = fp_norounddb (DB(FAReg));
                                      }
#ifndef EMUDEBUG
                                      else
                                      {
                                          printf ("-W-EMUFPU: Warning - FAReg is not REAL64! (fpunoround)\n");
                                          FAReg.length = FP_UNKNOWN;
                                          SN(FAReg)    = RUndefined;
                                      }
#endif
                                      ResetRounding = TRUE;
			              break;
			   case 0x0e: /* fpuchki32    */
                                      if (FAReg.length == FP_REAL64)
                                          fp_chki32db (DB(FAReg));
                                      else if (FAReg.length == FP_REAL32)
                                          fp_chki32sn (SN(FAReg));
#ifndef EMUDEBUG
                                      else
                                          printf ("-W-EMUFPU: Warning - FAReg is undefined! (fpuchki32)\n");
#endif
                                      ResetRounding = TRUE;
			              break;
			   case 0x0f: /* fpuchki64    */
                                      if (FAReg.length == FP_REAL64)
                                          fp_chki64db (DB(FAReg));
                                      else if (FAReg.length == FP_REAL32)
                                          fp_chki64sn (SN(FAReg));
#ifndef EMUDEBUG
                                      else
                                          printf ("-W-EMUFPU: Warning - FAReg is undefined! (fpuchki64)\n");
#endif
                                      ResetRounding = TRUE;
			              break;
			   case 0x11: /* fpudivby2    */
                                      fp_dounary (fp_divby2db, fp_divby2sn);
			              break;
			   case 0x12: /* fpumulby2    */
                                      fp_dounary (fp_mulby2db, fp_mulby2sn);
			              break;
			   case 0x22: /* fpurn    */
                                      fp_setrounding ("fpurn", ROUND_N);
                                      /* Do not reset rounding mode. */
			              break;
			   case 0x23: /* fpuseterr    */
                                      FP_Error = TRUE;
                                      ResetRounding = TRUE;
			              break;
			   case 0x9c: /* fpuclrerr    */
                                      fp_clrexcept ();
                                      FP_Error = FALSE;
                                      ResetRounding = TRUE;
			              break;
                           default  :  
                                      printf ("-E-EMU414: Error - bad Icode! (#%02X - %s)\n", OReg, mnemonic (Icode, OReg, AReg, 0));
                                      processor_state ();
			              handler (-1);
			              break;
                           }
		           break;
		case 0xac: /* fpldnlmulsn    */
		           if (IsT414)
		               goto BadCode;
                           T4DEBUG(checkWordAligned ("FPLDNLMULSN", AReg));
                           fp_pushsn (real32 (AReg));
#ifndef EMUDEBUG
                           if (FAReg.length == FP_REAL32)
#endif
                           {
                                fp_pop2sn (&sntemp1, &sntemp2);
                                fp_pushsn (fp_mulsn (sntemp1, sntemp2));
                           }
#ifndef EMUDEBUG
                           else
                           {
                                printf ("-W-EMUFPU: Warning - FAReg is not REAL32! (fpldnlmulsn)\n");
                                fp_drop ();
                                FAReg.length = FP_UNKNOWN;
                                SN(FAReg)    = RUndefined;
                           }
#endif
                           AReg = BReg;
                           BReg = CReg;
		           IPtr++;
		           break;
                case 0x17c: /* XXX lddevid    */
                           if (IsT800) /* TTH */
                                BReg = CReg;
                           IPtr++;
                           break;
                case 0x1ff: /* XXX start    */
                           quit = TRUE;
                           IPtr++;
                           break;
		default  : 
BadCode:
                           printf ("-E-EMU414: Error - bad Icode! (#%02X - %s)\n", OReg, mnemonic (Icode, OReg, AReg, 0));
                           processor_state ();
			   handler (-1);
			   break;
	} /* switch OReg */
			   OReg = 0;
			   break;
		default  : 
                           printf ("-E-EMU414: Error - bad Icode! (#%02X - %s)\n", OReg, mnemonic (Icode, OReg, AReg, 0));
                           processor_state ();
			   handler (-1);
			   break;
	} /* switch (Icode) */
                /* Reset rounding mode to round nearest. */
                if ((IsT800 || IsTVS) && ResetRounding && (RoundingMode != ROUND_N))
                        fp_setrounding ("reset", ROUND_N);

#ifdef PROFILE
		if (profiling)
			profile[PRO_INSTR]++;
#endif

                /* Halt when Error flag was set */
		if ((!PrevError && ReadError) &&
                    (exitonerror || (ReadHaltOnError)))
			break;
		if (quit == TRUE)
			break;

#ifndef NDEBUG
                fp_chkexcept ("mainloop");
#endif
	}
#ifdef PROFILE
        if (profiling)
        {
                update_tod (&EndTOD);
                ElapsedSecs = (EndTOD.tv_sec - StartTOD.tv_sec) * 1000 +
                              (EndTOD.tv_usec - StartTOD.tv_usec) / 1000;
                profile[PRO_ELAPSEDMS] = ElapsedSecs;
        }
#endif
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


void Enqueue (uint32_t wptr, uint32_t pri)
{
	uint32_t ptr;

        /* Get front of process list pointer. */
        EMUDBG2 ("-I-EMUDBG: Enqueue(1): Get front of %s priority process list pointer.\n",
                        StrPrio(pri));

        ptr = FPtrReg[pri];
	if (ptr == NotProcess_p)
	{
                EMUDBG ("-I-EMUDBG: Enqueue(2): Empty process list, create.\n");

	        /* Empty process list. Create. */
	        FPtrReg[pri] = wptr;
	        BPtrReg[pri] = wptr;
	}
	else
	{
	        /* Process list already exists. Update. */
                EMUDBG ("-I-EMUDBG: Enqueue(2): Update process list.\n");

		/* Get workspace pointer of last process in list. */
		ptr = BPtrReg[pri];

		/* Link new process onto end of list. */
		writeword (index (ptr, Link_s), wptr);

		/* Update end-of-process-list pointer. */
		BPtrReg[pri] = wptr;
	}
}

/* Add a process to the relevant priority process queue. */
void schedule (uint32_t wdesc)
{
        uint32_t wptr, pri;
	uint32_t temp;

        wptr = GetDescWPtr(wdesc);
#ifndef NDEBUG
        if (NotProcess_p == wptr)
        {
                printf ("-E-EMU414: Wdesc = #%08X. Cannot schedule NotProcess_p.\n", wdesc);
                handler (-1);
        }
#endif
        pri  = GetDescPriority(wdesc);

#ifdef EMUDEBUG
        if (emudebug)
        {
                printf ("-I-EMUDBG: Schedule(1): Process = #%08X at priority %s\n", wptr, StrPrio(pri));
                printf ("-I-EMUDBG: Schedule(1): %s Idle=%d FPtrReg[Hi]=#%08X FPtrReg[Lo]=#%08X Int=#%08X\n",
                        StrPrio(ProcPriority), Idle, FPtrReg[0], FPtrReg[1], word (0x8000002C));
        }
#endif

	/* Remove from timer queue if a ready alt. */
        /* !!! XXX - READ OF NOT INITIALIZED MEMORY !!! */
	temp = word_int (index (wptr, State_s));
	if (temp == Ready_p)
		purge_timer ();

        if (HiPriority == ProcPriority)
        {
                EMUDBG ("-I-EMUDBG: Processor at Hi priority. Enqueue\n");
                Enqueue (wptr, pri);
        }
        else if (LoPriority == ProcPriority)
        {
                if (HiPriority == pri)
                {
                        EMUDBG ("-I-EMUDBG: Processor at Lo priority. Activate Hi priority process\n");

		        interrupt ();

                        /* Preserve Error and HaltOnError flags only. */
                        STATUSReg &= (ErrorFlag | HaltOnErrorFlag);

                        /* ??? HaltOnErrorFlag is cleared before the process starts. */
                        /* ClearHaltOnError; */

                        EMUDBG2 ("-I-EMU414: Activate Hi priority process Wdesc=#%08X\n", wdesc);

                        UpdateWdescReg (wdesc);
                        T4DEBUG(checkWPtr ("Schedule", WPtr));
		        IPtr = word (index (WPtr, Iptr_s));
                }
                else if (LoPriority == pri)
                {
                        EMUDBG3 ("-I-EMUDBG: Processor at Lo priority. Idle=%d WPtr=#%08X\n", Idle, WPtr);
                        if (Idle)
                        {
                                EMUDBG2 ("-I-EMU414: Lo priority process list empty. Activate process Wdesc=#%08X\n", wdesc);
                                UpdateWdescReg (wdesc);
                                T4DEBUG(checkWPtr ("Schedule", WPtr));
		                IPtr = word (index (WPtr, Iptr_s));
                        }
                        else
                                Enqueue (wptr, LoPriority);
                }
        }
}

/* Run a process, HiPriority if available. */
int run_process (void)
{
	uint32_t ptr;
	uint32_t lastptr;

	/* Is the HiPriority process list non-empty? */
	if (NotProcess_p != FPtrReg[0])
	{
	        EMUDBG ("-I-EMUDBG: RunProcess: HiPriority process list non-empty.\n");
		/* There is a HiPriority process available. */
		ProcPriority = HiPriority;
	}
        /* Is there an interrupted LoPriority process? */
        else if (ReadInterrupt)
        {
	        EMUDBG ("-I-EMUDBG: RunProcess: There is an interrupted LoPriority process.\n");
                ProcPriority = LoPriority;
        }
	else if (NotProcess_p != FPtrReg[1])
	{
	        EMUDBG ("-I-EMUDBG: RunProcess: LoPriority process list non-empty.\n");
		/* There are only LoPriority processes available. */
		ProcPriority = LoPriority;
	}
        else
        {
                EMUDBG ("-I-EMUDBG: RunProcess: Empty process list. Cannot start!\n");

		/* Empty process list. Cannot start! */
                UpdateWdescReg (IdleProcess_p);
                return (-1);
        }

	/* Get front of process list pointer. */
	ptr = FPtrReg[ProcPriority];
	if ((ProcPriority == LoPriority) && (ReadInterrupt))
	{
		/* Return to interrupted LoPriority process. */
		UpdateWdescReg (GetDescWPtr(word (index (MostNeg, 11))) | LoPriority);
                T4DEBUG(checkWPtr ("RunProcess(1)", WPtr));
	        EMUDBG3 ("-I-EMUDBG: RunProcess: ProcPriority = %s, WPtr = #%08X.\n",
                                StrPrio(ProcPriority), WPtr);

		IPtr = word (index (MostNeg, 12));
		AReg = word (index (MostNeg, 13));
		BReg = word (index (MostNeg, 14));
		CReg = word (index (MostNeg, 15));
		STATUSReg = word (index (MostNeg, 16));
		/*EReg = word (index (MostNeg, 17));*/

                if (IsT800 || IsTVS)
                {
                        FAReg = FARegSave;
                        FBReg = FBRegSave;
                        FCReg = FCRegSave;
                }
                ClearInterrupt; /* XXX Not necessary ??? */
	}  
	else
	{
	        lastptr = BPtrReg[ProcPriority];

	        EMUDBG5 ("-I-EMUDBG: RunProcess: ProcPriority = %s, ptr = #%08X. FPtrReg[Hi] = #%08X, FPtrReg[Lo] = #%08X.\n",
                                StrPrio(ProcPriority), ptr, FPtrReg[0], FPtrReg[1]);

		if (ptr == lastptr)
		{
			/* Only one process in list. */
			UpdateWdescReg (ptr | ProcPriority);
                        T4DEBUG(checkWPtr ("RunProcess(2)", WPtr));

			/* Get Iptr. */
			IPtr = word (index (WPtr, Iptr_s));

			/* Empty list now. */
			FPtrReg[ProcPriority] = NotProcess_p;
		}
		else
		{
			/* List. */
			UpdateWdescReg (ptr | ProcPriority);
                        T4DEBUG(checkWPtr ("RunProcess(3)", WPtr));

			/* Get Iptr. */
			IPtr = word (index (WPtr, Iptr_s));

			/* Point at second process in chain. */
			FPtrReg[ProcPriority] = word (index (WPtr, Link_s));
		}
	}

	return (0);
}

/* Start a process. */
void start_process (void)
{
        int active, links_active;

        /* IntEnabled is always TRUE.
         * if ((ProcPriority == LoPriority) && !IntEnabled)
         *        return;
         */

        /* First, clear GotoSNP flag. */
        ClearGotoSNP;

	/* Second, handle any host link communication. */
        do
        {
                active = TRUE;
                if (0 == run_process ())
                        break;

#ifdef EMUDEBUG
                if (emudebug)
                {
		        printf ("-I-EMUDBG: StartProcess: Empty process list. Update comms.\n");
                        fflush (stdout);
                }
#endif

                /* Update host comms. */
                active = FALSE;
                if (serve)
                {
		        active = 0 != server ();
                        if (!Idle)
                        {
                                active = TRUE;
                                break;
                        }
                }

                if (Idle && ProcessQEmpty && TimerQEmpty)
                        links_active = (0 != linkcomms ("idle", FALSE, LTO_BOOT));
                else
                        links_active = (0 != linkcomms ("running", FALSE, LTO_COMM));
                active = active || links_active;
                if (!Idle)
                {
                        active = TRUE;
                        break;
                }

		/* Check timer queue, update timers. */
                active = active || (!TimerQEmpty);
                if (++count1 > delayCount1)
		        update_time ();
                if (!Idle)
                {
                        active = TRUE;
                        break;
                }
        } while (active);

        if (!active)
        {
                printf ("-E-EMU414: Node%d Error - stopped no Link/Process/Timer activity!\n", nodeid < 0 ? 0 : nodeid);
                processor_state ();
                handler (-1);
        }

	/* Reset timeslice counter. */
	timeslice = 0;

#ifdef PROFILE
	if (profiling)
		profile[PRO_STARTP]++;
#endif
}

/* Save the current process and start a new process. */
void deschedule (void)
{
#ifndef NDEBUG
        if (NotProcess_p == WPtr)
        {
                printf ("-E-EMU414: Cannot deschedule NotProcess_p.\n");
                handler (-1);
        }
#endif
        EMUDBG2 ("-I-EMUDBG: Deschedule process #%08X.\n", Wdesc);
        /* Write Iptr into workspace */
	writeword (index (WPtr, Iptr_s), IPtr);

        /* Set StartNewProcess flag. */
        SetGotoSNP;
}

/* Save the current process and place it on the relevant priority process queue. */
void reschedule (void)
{
#ifndef NDEBUG
        if (NotProcess_p == WPtr)
        {
                printf ("-E-EMU414: Cannot reschedule NotProcess_p.\n");
                handler (-1);
        }
#endif
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
        /* IntEnabled is always TRUE.
         * if ((ProcPriority == HiPriority) || IntEnabled)
         */
        {
	        if (serve) server ();
                linkcomms ("pri", FALSE, LTO_HI);
        }

        /* High priority processes never timesliced. */
        if (ProcPriority == HiPriority)
                return;

	/* Check for timeslice. */
	if (timeslice > 1)
	{
                EMUDBG2 ("-I-EMUDBG: Timeslice process #%08X.\n", Wdesc);

		/* Must change process! */
		timeslice = 0;

		/* reschedule really moves the process to the end of the queue! */
		reschedule ();

		/* Set StartNewProcess flag. */
                SetGotoSNP;
	}
#ifdef PROFILE
	if (profiling)
		profile[PRO_DCHECK]++;
#endif
}

/* Interrupt a low priority process.                    */
/* Can only occur due to comms or timer becoming ready. */
void interrupt (void)
{
	/* A high priority process has become ready, interrupting a low priority one. */

#ifndef NDEBUG
	/* Sanity check. */
	if (ReadInterrupt)
	{
		printf ("-E-EMU414: Error - multiple interrupts of low priority processes!\n");
		handler (-1);
	}
#endif

	/* Store the registers. */
	writeword (index (MostNeg, 11), Wdesc);
        if (IdleProcess_p != Wdesc)
        {
                EMUDBG ("-I-EMUDBG: Interrupt LoPriority process.\n");

	        writeword (index (MostNeg, 12), IPtr);
	        writeword (index (MostNeg, 13), AReg);
	        writeword (index (MostNeg, 14), BReg);
	        writeword (index (MostNeg, 15), CReg);
	        writeword (index (MostNeg, 16), STATUSReg);
	        /*writeword (index (MostNeg, 17), EReg);*/

                if (IsT800 || IsTVS)
                {
                        FARegSave = FAReg;
                        FBRegSave = FBReg;
                        FCRegSave = FCReg;
                }
        }
        /* Note: that an interrupted process is not placed onto the scheduling lists. */
}

/* Insert a process into the relevant priority process queue. */
void insert (uint32_t time)
{
        uint32_t ptr;
	uint32_t nextptr;
	uint32_t timetemp;

	writeword (index (WPtr, Time_s), (time + 1));

        ptr = TPtrLoc[ProcPriority];
	if (ptr == NotProcess_p)
	{
		/* Empty list. */
		/*writeword (ptr, WPtr); Strange! */
		writeword (index (WPtr, TLink_s), NotProcess_p);
                TPtrLoc[ProcPriority] = WPtr;
	}
	else
	{
		/* One or more entries. */
		timetemp = word (index (ptr, Time_s));
		if (INT32(timetemp - time) > 0)
		{
			/* Put in front of first entry. */
			writeword (index (WPtr, TLink_s), ptr);
                        TPtrLoc[ProcPriority] = WPtr;
		}
		else
		{
			/* Somewhere after the first entry. */
			/* Run along list until ptr is before the time and nextptr is after it. */
			nextptr = word (index (ptr, TLink_s));
			if (nextptr != NotProcess_p)
				timetemp = word (index (nextptr, Time_s));
			while ((INT32(time - timetemp) > 0) && (nextptr != NotProcess_p))
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

        if (Idle)
                return;

	/* Delete any entries at the beginning of the list. */
	while (TPtrLoc[ProcPriority] == WPtr)
	{
	        TPtrLoc[ProcPriority] = word (index (WPtr, TLink_s));
	}

	ptr = TPtrLoc[ProcPriority];
	oldptr = ptr;

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


void schedule_timerq (int prio)
{
        uint32_t temp3;

        /* IntEnabled is always TRUE. */
        if ((TPtrLoc[prio] != NotProcess_p)
                /* && ((ProcPriority == HiPriority) || IntEnabled) */
           )
        {
	        temp3 = word (index (TPtrLoc[prio], Time_s));
		while ((INT32(ClockReg[prio] - temp3) > 0) && (TPtrLoc[prio] != NotProcess_p))
		{
		        schedule (TPtrLoc[prio] | prio);

			TPtrLoc[prio] = word (index (TPtrLoc[prio], TLink_s));
                        if (TPtrLoc[prio] != NotProcess_p)
			        temp3 = word (index (TPtrLoc[prio], Time_s));
		}
        }
}

/* XXX Update time, check timer queues. */
INLINE void update_time (void)
{
        struct timeval tv;
        unsigned long elapsed_usec;

	/* Move timers on if necessary, and increment timeslice counter. */
	count1 = 0;

        /* Check TOD clock, on UNIX ~ 1us resolution. */
        update_tod (&tv);

        /* Calculate elapsed usecs. */
        elapsed_usec = (tv.tv_sec  - LastTOD.tv_sec) * 1000000 +
                       (tv.tv_usec - LastTOD.tv_usec);
                
        /* Time not lapsed ? Return. */
        if (0 == elapsed_usec)
        {
                delayCount1++;
                return;
        }
        else if (elapsed_usec > 1)
        {
                delayCount1--;
                if (delayCount1 < 5)
                        delayCount1 = 5;
        }

        /* printf ("-I-EMUDBG: Elapsed time %lu.\n", elapsed_usec); */

        /* Update last known TOD clock. */
        LastTOD = tv;

	if (Timers == TimersGo)
                ClockReg[0] += elapsed_usec;

	count2 += elapsed_usec;

	/* Check high priority timer queue if HiPriority or interrupts enabled. */
        /* ??? Timers may be not enabled. */
        schedule_timerq (HiPriority);

	if (count2 > 64) /* ~ 64us */
	{
	        if (Timers == TimersGo)
                        ClockReg[1] += (count2 / 64);
		count3 += (count2 / 64);
		count2  =  count2 & 63;

		/* Check low priority timer queue if HiPriority or interrupts enabled. */
                schedule_timerq (LoPriority);

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

#define CoreAddr(a)     (INT32(a) < INT32(ExtMemStart))
#define ExtMemAddr(a)   (INT32(ExtMemStart) <= INT32(a))
#define CoreRange(a,n)  (CoreAddr(a) && CoreAddr((a)+(n)))
#define ExtMemRange(a,n)(ExtMemAddr(a) && ExtMemAddr((a)+(n)))

/* Read a word from memory. */
uint32_t word_int (uint32_t ptr)
{
	uint32_t result;
#if BYTE_ORDER==1234
#ifndef _MSC_VER
#warning Using little-endian access!
#endif
        uint32_t *wptr;

        if (CoreAddr(ptr))
                wptr = (uint32_t *)(core + (MemWordMask & ptr));
        else
                wptr = (uint32_t *)(mem + (MemWordMask & ptr));
        result = *wptr;
#else
	unsigned char val[4];

	/* Get bytes, ensuring memory references are in range. */
        if (CoreAddr(ptr))
        {
	        val[0] = core[(ptr & MemWordMask)];
	        val[1] = core[(ptr & MemWordMask)+1];
	        val[2] = core[(ptr & MemWordMask)+2];
	        val[3] = core[(ptr & MemWordMask)+3];
        }
        else
        {
	        val[0] = mem[(ptr & MemWordMask)];
	        val[1] = mem[(ptr & MemWordMask)+1];
	        val[2] = mem[(ptr & MemWordMask)+2];
	        val[3] = mem[(ptr & MemWordMask)+3];
        }

	result = (val[0]) | (val[1]<<8) | (val[2]<<16) | (val[3]<<24);
#endif

	return (result);
}

void word_notinit (void)
{
        printf ("-E-EMU414: Error - read of not initialized memory!\n");
        handler (-1);
}

#ifndef NDEBUG
uint32_t word (uint32_t ptr)
{
        uint32_t result;

        result = word_int (ptr);

        if (memdebug)
                printf ("-I-EMUMEM: RW: Mem[%08X] ? %08X\n", ptr, result);

        if ((memnotinit) && (result == InvalidInstr_p))
                word_notinit ();

	return (result);
}
#endif


/* Write a word to memory. */
void writeword_int (uint32_t ptr, uint32_t value)
{
#if BYTE_ORDER==1234
#ifndef _MSC_VER
#warning Using little-endian access!
#endif
        uint32_t *wptr;

        if (CoreAddr(ptr))
                wptr = (uint32_t *) (core + (MemWordMask & ptr));
        else
                wptr = (uint32_t *) (mem + (MemWordMask & ptr));
        *wptr = value;

        InvalidateRange(ptr,4);
#else
	unsigned char val[4];

	val[0] = (value & 0x000000ff);
	val[1] = ((value & 0x0000ff00)>>8);
	val[2] = ((value & 0x00ff0000)>>16);
	val[3] = ((value & 0xff000000)>>24);

	/* Write bytes, ensuring memory references are in range. */
        if (CoreAddr(ptr))
        {
	        core[(ptr & MemWordMask)]   = val[0];
	        core[(ptr & MemWordMask)+1] = val[1];
	        core[(ptr & MemWordMask)+2] = val[2];
	        core[(ptr & MemWordMask)+3] = val[3];
        }
        else
        {
	        mem[(ptr & MemWordMask)]   = val[0];
	        mem[(ptr & MemWordMask)+1] = val[1];
	        mem[(ptr & MemWordMask)+2] = val[2];
	        mem[(ptr & MemWordMask)+3] = val[3];
        }
#endif

}


#ifndef NDEBUG
void writeword (uint32_t ptr, uint32_t value)
{
        writeword_int (ptr, value);

        if (memdebug)
                printf ("-I-EMUMEM: WW: Mem[%08X] ! %08X\n", ptr, value);
}
#endif


/* Read a byte from memory. */
unsigned char byte_int (uint32_t ptr)
{
	unsigned char result;

	/* Get byte, ensuring memory reference is in range. */
        if (CoreAddr(ptr))
	        result = core[(ptr & MemByteMask)];
        else
	        result = mem[(ptr & MemByteMask)];

	return (result);
}

#ifndef NDEBUG
unsigned char byte (uint32_t ptr)
{
        unsigned char result;

        result = byte_int (ptr);
        if (memdebug)
                printf ("-I-EMUMEM: RB: Mem[%08X] ! %02X\n", ptr, result);

        return result;
}
#endif

/* Write a byte to memory. */
INLINE void writebyte_int (uint32_t ptr, unsigned char value)
{
	/* Write byte, ensuring memory reference is in range. */
        if (CoreAddr(ptr))
                core[(ptr & MemByteMask)] = value;
        else
	        mem[(ptr & MemByteMask)] = value;

        InvalidateAddr(ptr);
}

/* Return pointer to memory or data[] */
INLINE unsigned char* memrange (uint32_t ptr, unsigned char *data, uint32_t len)
{
        unsigned char *dst;
        if (CoreRange(ptr,len))
        {
                dst = core + (ptr & MemByteMask);
                return dst;
        }
        else if (ExtMemAddr(ptr))
        {
                dst = mem + (ptr & MemByteMask);
                return dst;
        }
        return data;
}

/* Write bytes to memory. */
INLINE void writebytes_int (uint32_t ptr, unsigned char *data, uint32_t len)
{
        unsigned char *dst;
	/* Write byte, ensuring memory reference is in range. */
        if (CoreRange(ptr,len))
        {
                dst = core + (ptr & MemByteMask);
                memcpy (dst, data, len);
                InvalidateRange(ptr,len);
        }
        else if (ExtMemAddr(ptr))
        {
                dst = mem + (ptr & MemByteMask);
                memcpy (dst, data, len);
                InvalidateRange(ptr,len);
        }
        else
        {
                int i;
                for (i = 0; i < len; i++)
                        writebyte_int (ptr++, data[i]);
        }
}

/* Move bytes to memory. */
INLINE void movebytes_int (uint32_t dst, uint32_t src, uint32_t len)
{
        unsigned char *p, *q;

	/* Write byte, ensuring memory reference is in range. */
        if (CoreRange(src,len) && CoreRange(dst,len))
        {
                p = core + (dst & MemByteMask);
                q = core + (src & MemByteMask);
                memmove (p, q, len);
                InvalidateRange(dst,len);
        }
        else if (ExtMemAddr(src) && ExtMemAddr(dst))
        {
                p = mem + (dst & MemByteMask);
                q = mem + (src & MemByteMask);
                memmove (p, q, len);
                InvalidateRange(dst,len);
        }
        else
        {
                int i;
                for (i = 0; i < len; i++)
                        writebyte_int (dst++, byte_int (src++));
        }
}

/* Read bytes from memory. */
/* If range entirely in core/extmem then return pointer to memory */
/* Otherwise copy to data[] and return data[] */
INLINE unsigned char* bytes_int (uint32_t ptr, unsigned char *data, uint32_t len)
{
        unsigned char *dst;
	/* Write byte, ensuring memory reference is in range. */
        if (CoreRange(ptr,len))
        {
                dst = core + (ptr & MemByteMask);
                // memcpy (data, dst, len);
                return dst;
        }
        else if (ExtMemAddr(ptr))
        {
                dst = mem + (ptr & MemByteMask);
                // memcpy (data, dst, len);
                return dst;
        }
        else
        {
                int i;
                for (i = 0; i < len; i++)
                        data[i] = byte_int (ptr++);
                return data;
        }
}

#ifndef NDEBUG
void writebyte (uint32_t ptr, unsigned char value)
{
        if (memdebug)
                printf ("-I-EMUMEM: WB: Mem[%08X] ! %02X\n", ptr, value);

        writebyte_int (ptr, value);
}
#endif

/* Read a REAL32 from memory. */
fpreal32_t real32 (uint32_t ptr)
{
        fpreal32_t x;

        ResetRounding = TRUE;

        x.bits = word (ptr);
        return x;
}

/* Write a REAL32 to memory. */
void writereal32 (uint32_t ptr, fpreal32_t value)
{
        ResetRounding = TRUE;

        writeword (ptr, value.bits);
}

/* Read a REAL64 from memory. */
fpreal64_t real64 (uint32_t ptr)
{
        fpreal64_t x;
        uint32_t lobits, hibits;

        ResetRounding = TRUE;

        lobits = word (ptr);
        hibits = word (ptr + 4);

        x.bits = ((uint64_t)(hibits) << 32) | lobits;
        return x;
}

/* Write a REAL64 to memory. */
void writereal64 (uint32_t ptr, fpreal64_t value)
{
        ResetRounding = TRUE;

        writeword (ptr,     value.bits & 0xffffffff);
        writeword (ptr + 4, (value.bits >> 32) & 0xffffffff);
}

/* Add an executing instruction to the profile list. */
static uint32_t previnstruction = 0x400;

void add_profile (uint32_t instruction)
{
        if (instruction > 0x3ff)
        {
                printf ("-E-EMU414: Error - profile invalid instruction! (%u)", instruction);
                return;
        }
        instrprof[instruction]++;
        if (0x400 != previnstruction)
                combinedprof[previnstruction][instruction]++;
        previnstruction = instruction;
}

char* profmnemonic(uint32_t i, char *buf)
{
        if (i < 0x100)
                sprintf (buf, "  %02X  %-12s", i, mnemonic (i, MostNeg, MostNeg, 1));
	else if (i < 0x300)
                sprintf (buf, "%04X  %-12s", i - 0x100, mnemonic (0xF0, i - 0x100, MostNeg, 1));
        else
                sprintf (buf, "S%03X  %-12s", i - 0x300, mnemonic (0xF0, 0xAB, i - 0x300, 1));
        return buf;
}

void print_profile (void)
{
        int i, j;
	extern FILE *ProfileFile;
        char buf1[64], buf2[64];

        fprintf (ProfileFile, "-----Instruction--------Freq------\n");
	for (i = 0; i < 0x400; i++)
	{
                /* Skip empty counters. */
                if (0 == instrprof[i])
                        continue;

                fprintf (ProfileFile, "%s     %u\n", profmnemonic (i, buf1), instrprof[i]);
        }
	for (i = 0; i < 0x400; i++)
	{
                for (j = 0; j < 0x400; j++)
                {
                        /* Skip empty counters. */
                        if (0 == combinedprof[i][j])
                                continue;

                        fprintf (ProfileFile, "%s%s     %u\n", profmnemonic (i, buf1), profmnemonic (j, buf2), combinedprof[i][j]);
                }
        }
}
