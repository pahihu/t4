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

#include <stdint.h>
#include "fparithmetic.h"

/*
 * processor.h
 *
 * Constants for the processor.
 *
 */

#define MostNeg		0x80000000
#define MostPos		0x7fffffff
#define NotProcess_p	0x80000000
#define Enabling_p	0x80000001
#define Waiting_p	0x80000002
#define Disabling_p	0x80000003
#define TimeSet_p	0x80000001
#define TimeNotSet_p	0x80000002
#define NoneSelected_o	0xffffffff
#define Ready_p		0x80000003

typedef unsigned char  u_char;
typedef unsigned short u_short;

extern int Txxx;
extern uint32_t MemSize;
extern uint32_t MemWordMask;
extern uint32_t MemByteMask;
extern uint32_t MemStart;
extern uint32_t ExtMemStart;

extern uint32_t CLineTagsSize;

extern uint32_t CoreSize;

#define IsT800  (Txxx == 800)
#define IsT414  (Txxx == 414)
#define IsTVS   (Txxx == 1144)

#define HiPriority	0x00000000
#define LoPriority	0x00000001

#define true_t		1
#define false_t		0

#define INT32(x)        ((int32_t)(x))


#define LTO_BOOT       1000
#define LTO_COMM       0
#define LTO_HI         0
#define LTO_POLL       0

/* Processor definitions. */
#define Link0Out 0x80000000 /****/
#define Link1Out 0x80000004 /****/
#define Link2Out 0x80000008 /****/
#define Link3Out 0x8000000C /****/
#define Link0In  0x80000010 /****/
#define Link1In  0x80000014 /****/
#define Link2In  0x80000018 /****/
#define Link3In  0x8000001C /****/

#define IsLinkOut(x) ((Link0Out <= (x)) && ((x) <= Link3Out) && (0 == ((x) & 3)))
#define IsLinkIn(x)  ((Link0In  <= (x)) && ((x) <= Link3In)  && (0 == ((x) & 3)))
#define TheLink(x)   (((x) >> 2) & 3)

#define MAX_CHANNEL_URL 128

#define SCH_LEN         0

#if 0
#define MAX_DATA        255
#define SCH_DATA        1
#define SCHLength(ch)   (*((ch)->schbuf))
#else
#define MAX_DATA        1022
#define SCH_DATA        2
#define SCHLength(ch)   (*(u_short*)(ch)->schbuf)
#endif

#define SCH_SIZE        (MAX_DATA+SCH_DATA)

#ifndef T4CLINEBITS
#define T4CLINEBITS     (4)
#endif

typedef struct _Channel_ {
        uint32_t LinkAddress; /* the address of the Channel */
        uint32_t Address;
        uint32_t Length;
        int  Link;
        int  sock;
        char url[MAX_CHANNEL_URL];
        u_char *schbuf;
        uint64_t IOBytes;
} Channel;

typedef struct _LinkIface_ {
        Channel Out;
        Channel In;
} LinkIface;

extern LinkIface Link[4];

extern int profiling;
extern uint64_t profile[10];

#define PRO_INSTR       0
#define PRO_DCHECK      1
#define PRO_ISERVER     2
#define PRO_STARTP      3
#define PRO_ELAPSEDMS   4
#define PRO_CHANIN      5
#define PRO_CHANOUT     6
#define PRO_ICHIT       7
#define PRO_ICMISS      8

/* Link 0 registers. */
#define Link0OutSource  Link[0].Out.Address
#define Link0OutLength  Link[0].Out.Length
#define Link0InDest     Link[0].In.Address
#define Link0InLength   Link[0].In.Length


/* Function prototypes. */
void mainloop      (void);
void execute       (void);
void operate       (void);
void schedule      (uint32_t);
void UpdateWdescReg (uint32_t);
int  run_process   (void);
int  start_process (void);
void deschedule    (void);
void D_check       (void);
void interrupt     (void);
void insert        (uint32_t);
void purge_timer   (void);
INLINE void update_time   (void);
uint32_t word (uint32_t);
void     writeword     (uint32_t, uint32_t);
u_char byte (uint32_t);
INLINE
void writebyte     (uint32_t, u_char);
fpreal32_t real32 (uint32_t);
void    writereal32 (uint32_t, fpreal32_t);
fpreal64_t real64 (uint32_t);
void    writereal64 (uint32_t, fpreal64_t);
uint32_t   word_int (uint32_t);
void    writeword_int (uint32_t, uint32_t);
u_char byte_int (uint32_t);
void writebyte_int (uint32_t, u_char);
void writebytes_int(uint32_t, u_char*, uint32_t);
void movebytes_int (uint32_t, uint32_t, uint32_t);
u_char* bytes_int (uint32_t, u_char*, uint32_t);
u_char* memrange (uint32_t, u_char*, uint32_t);
void add_profile   (uint32_t);
void print_profile (void);
void open_channel  (uint32_t);
Channel *reset_channel(uint32_t);
void alt_channel   (Channel*);
int channel_ready  (Channel *chan);
void close_channels(void);
int  linkcomms     (char *where, int doBoot, int timeOut);
void init_memory   (void);
void init_processor(void);

