
/*
 *	"DHRYSTONE" Benchmark Program
 *
 *	Version:	C/1
 *	Date:		12/01/84, RESULTS updated 10/22/85
 *	Author:		Reinhold P. Weicker,  CACM Vol 27, No 10, 10/84 pg. 1013
 *			Translated from ADA by Rick Richardson
 *			Every method to preserve ADA-likeness has been used,
 *			at the expense of C-ness.
 *	Compile:	cc -O dry.c -o drynr			: No registers
 *			cc -O -DREG=register dry.c -o dryr	: Registers
 *	Defines:	Defines are provided for old C compiler's
 *			which don't have enums, and can't assign structures.
 *			The time(2) function is library dependant; One is
 *			provided for CI-C86.  Your compiler may be different.
 *			The LOOPS define is initially set for 50000 loops.
 *			If you have a machine with large integers and is
 *			very fast, please change this number to 500000 to
 *			get better accuracy.  Please select the way to
 *			measure the execution time using the TIME define.
 *			For single user machines, time(2) is adequate. For
 *			multi-user machines where you cannot get single-user
 *			access, use the times(2) function.  If you have
 *			neither, use a stopwatch in the dead of night.
 *			Use a "printf" at the point marked "start timer"
 *			to begin your timings. DO NOT use the UNIX "time(1)"
 *			command, as this will measure the total time to
 *			run this program, which will (erroneously) include
 *			the time to malloc(3) storage and to compute the
 *			time it takes to do nothing.
 *	Run:		drynr; dryr
 *
 *	Results:	If you get any new machine/OS results, please send to:
 *			{ihnp4,vax135,..}!houxm!vaximile!rer
 *			and thanks to all that do.  Space prevents listing
 *			the names of those who have provided some of these
 *			results.
 *	Note:		I order the list in increasing performance of the
 *			"with registers" benchmark.  If the compiler doesn't
 *			provide register variables, then the benchmark
 *			is the same for both REG and NOREG.  I'm not going
 *			to list a compiler in a better place because if it
 *			had register variables it might do better. No
 *			register variables is a big loss in my book.
 *	PLEASE:		Send complete information about the machine type,
 *			clock speed, OS and C manufacturer/version.  If
 *			the machine is modified, tell me what was done.
 *			Otherwise, I won't include it in this list.  My
 *			favorite flame on this subject was a machine that
 *			was listed as an IBM PC/XT 8086-9.54Mhz.  That must
 *			have been some kind of co-processor board that ran
 *			the benchmark, not the XT.  Tell me what it was!
 *
 * MACHINE	MICROPROCESSOR	OPERATING	COMPILER	DHRYSTONES/SEC.
 * TYPE				SYSTEM				NO REG	REGS
 * --------------------------	------------	-----------	---------------
 * IBM PC/XT	8088-4.77Mhz	PC/IX		cc		 257	 287
 * Cosmos	68000-8Mhz	UniSoft		cc		 305	 322
 * IBM PC/XT	8088-4.77Mhz	VENIX/86 2.0	cc		 297	 324
 * IBM PC	8088-4.77Mhz	MSDOS 2.0	b16cc 2.0	 310	 340
 * IBM PC	8088-4.77Mhz	MSDOS 2.0	CI-C86 2.20M	 390	 390
 * IBM PC/XT	8088-4.77Mhz	PCDOS 2.1	Lattice 2.15	 403	  -  @
 * PDP-11/34	-		UNIX V7M	cc		 387	 438
 * Onyx C8002	Z8000-4Mhz	IS/1 1.1 (V7)	cc		 476	 511
 * ATT PC6300	8086-8Mhz	MSDOS 2.11	b16cc 2.0	 632	 684
 * IBM PC/AT	80286-6Mhz	PCDOS 3.0	CI-C86 2.1	 666	 684
 * Macintosh	68000-7.8Mhz 2M	Mac Rom		Mac C 32 bit int 694	 704
 * Macintosh	68000-7.7Mhz	-		MegaMax C 2.0	 661	 709
 * NEC PC9801F	8086-8Mhz	PCDOS 2.11	Lattice 2.15	 768	  -  @
 * ATT PC6300	8086-8Mhz	MSDOS 2.11	CI-C86 2.20M	 769	 769
 * ATT 3B2/300	WE32000-?Mhz	UNIX 5.0.2	cc		 735	 806
 * IBM PC/AT	80286-6Mhz	PCDOS 3.0	MS 3.0(large)	 833	 847 LM
 * VAX 11/750	-		Unix 4.2bsd	cc		 862	 877
 * Fast Mac	68000-7.7Mhz	-		MegaMax C 2.0	 839	 904 +
 * Macintosh	68000-7.8Mhz 2M	Mac Rom		Mac C 16 bit int 877	 909 S
 * IRIS-1400	68010-10Mhz	Unix System V	cc		 909	1000
 * IBM PC/AT	80286-6Mhz	VENIX/86 2.1	cc		 961	1000
 * IBM PC/AT	80286-6Mhz	PCDOS 3.0	b16cc 2.0	 943	1063
 * IBM PC/AT	80286-6Mhz	PCDOS 3.0	MS 3.0(small)	1063	1086
 * VAX 11/750	-		VMS		VAX-11 C 2.0	 958	1091
 * ATT PC7300	68010-10Mhz	UNIX 5.2	cc		1041	1111
 * ATT PC6300+	80286-6Mhz	MSDOS 3.1	b16cc 2.0	1111	1219
 * Sun2/120	68010-10Mhz	Sun 4.2BSD	cc		1136	1219
 * IBM PC/AT	80286-6Mhz	PCDOS 3.0	CI-C86 2.20M	1219	1219
 * MASSCOMP 500	68010-10MHz	RTU V3.0	cc (V3.2)	1156	1238
 * Cyb DataMate	68010-12.5Mhz	Uniplus 5.0	Unisoft cc	1162	1250
 * PDP 11/70	-		UNIX 5.2	cc		1162	1250
 * IBM PC/AT	80286-6Mhz	PCDOS 3.1	Lattice 2.15	1250	  -  @
 * IBM PC/AT	80286-7.5Mhz	VENIX/86 2.1	cc		1190	1315 *
 * Sun2/120	68010-10Mhz	Standalone	cc		1219	1315
 * ATT 3B2/400	WE32100-?Mhz	UNIX 5.2	cc		1315	1315
 * HP-110	8086-5.33Mhz	MSDOS 2.11	Aztec-C		1282	1351 ?
 * IBM PC/AT	80286-6Mhz	?		?		1250	1388 ?
 * ATT PC6300+	80286-6Mhz	MSDOS 3.1	CI-C86 2.20M	1428	1428
 * Cyb DataMate	68010-12.5Mhz	Uniplus 5.0	Unisoft cc	1470	1562 S
 * VAX 11/780	-		UNIX 5.2	cc		1515	1562
 * MicroVAX-II	-		-		-		1562	1612
 * ATT 3B20	-		UNIX 5.2	cc		1515	1724
 * HP9000-500	B series CPU	HP-UX 4.02	cc		1724	-
 * IBM PC/STD	80286-8Mhz	?		?		1724	1785
 * Gould PN6005	-		UTX 1.1(4.2BSD)	cc		1675	1964
 * VAX 11/785	-		UNIX 5.2	cc		2083	2083
 * VAX 11/785	-		VMS		VAX-11 C 2.0	2083	2083
 * Pyramid 90x	-		OSx 2.3		cc		2272	2272
 * Pyramid 90x	-		OSx 2.5		cc		3125	3125
 * SUN 3/75	68020-16.67Mhz	SUN 4.2 V3	cc		3333	3571
 * Sun 3/180	68020-16.67Mhz	Sun 4.2		cc		3333	3846
 * MC 5400	68020-16.67MHz	RTU V3.0	cc (V4.0)	3952	4054
 * SUN-3/160C	68020-16.67Mhz	Sun3.0ALPHA1 Un*x		3333	4166
 * Gould PN9080	-		UTX-32 1.1c	cc		-	4629
 * MC 5600/5700	68020-16.67MHz	RTU V3.0	cc (V4.0)	4504	4746 %
 * VAX 8600	-		VMS		VAX-11 C 2.0	7142	7142
 * Amdahl 470 V/8 		?		?		-      15015
 * Amdahl 580	-		UTS 5.0 Rel 1.2	cc Ver. 1.5    23076   23076
 * Amdahl 5860	 		?		?		-      28355
 *
 *   *  15Mhz crystal substituted for original 12Mhz;
 *   +  This Macintosh was upgraded from 128K to 512K in such a way that
 *      the new 384K of memory is not slowed down by video generator accesses.
 *   %  Single processor; MC == MASSCOMP
 *   &  Seattle Telecom STD-286 board
 *   @  vanilla Lattice compiler used with MicroPro standard library
 *   S  Shorts used instead of ints
 *   LM Large Memory Model. (Otherwise, all 80x8x results are small model)
 *   ?  I don't trust results marked with '?'.  These were sent to me with
 *      either incomplete information, or with times that just don't make sense.
 *      If anybody can confirm these figures, please respond.
 *
 **************************************************************************
 *
 *	The following program contains statements of a high-level programming
 *	language (C) in a distribution considered representative:
 *
 *	assignments			53%
 *	control statements		32%
 *	procedure, function calls	15%
 *
 *	100 statements are dynamically executed.  The program is balanced with
 *	respect to the three aspects:
 *		- statement type
 *		- operand type (for simple data types)
 *		- operand access
 *			operand global, local, parameter, or constant.
 *
 *	The combination of these three aspects is balanced only approximately.
 *
 *	The program does not compute anything meaningfull, but it is
 *	syntactically and semantically correct.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

/* Accuracy of timings and human fatigue controlled by next two lines */
/*#define LOOPS	50000		/* Use this for slow or 16 bit machines */
#ifdef _ICC
#define LOOPS	500000		/* Use this for faster machines */
#endif
/*#define LOOPS	5000000		/* Use this for faster machines */
#ifdef __APPLE__
#define LOOPS	100000000	/* Use this for faster machines */
#endif

/* Compiler dependent options */
/*#undef	NOENUM			/* Define if compiler has no enum's */
/*#undef	NOSTRUCTASSIGN		/* Define if compiler can't assign structures */
/* define only one of the next three defines */
/*#define NOTIME		/* Define if no time() function in library */
/*#define TIMES			/* Use times(2) time function */
/*#define TIME			/* Use time(2) time function */
#define CLOCK                   /* Use clock(2) function */
#include <time.h>


/* define the granularity of your times(2) function (when used) */
#define HZ	CLK_TCK		/* on SGI, use this! */
/*#define HZ	60		/* times(2) returns 1/60 second (most) */
/*#define HZ	100		/* times(2) returns 1/100 second (WECo) */



#ifdef	NOSTRUCTASSIGN
#define	structassign(d, s)	memcpy(&(d), &(s), sizeof(d))
#else
#define	structassign(d, s)	d = s
#endif

#ifdef	NOENUM
#define	Ident1	1
#define	Ident2	2
#define	Ident3	3
#define	Ident4	4
#define	Ident5	5
typedef int	Enumeration;
#else
typedef enum	{Ident1, Ident2, Ident3, Ident4, Ident5} Enumeration;
#endif

typedef int	OneToThirty;
typedef int	OneToFifty;
typedef char	CapitalLetter;
typedef char	String30[31];
typedef int	Array1Dim[51];
typedef int	Array2Dim[51][51];

struct	Record
{
	struct Record		*PtrComp;
	Enumeration		Discr;
	Enumeration		EnumComp;
	OneToFifty		IntComp;
	String30		StringComp;
};

typedef struct Record 	RecordType;
typedef RecordType *	RecordPtr;
typedef int		boolean;

/* #define	NULL		0 */
#define	TRUE		1
#define	FALSE		0

#ifndef REG
#define	REG
#endif

extern Enumeration	Func1();
extern boolean		Func2();

#ifdef TIMES
#include <sys/types.h>
#include <sys/times.h>
#endif

void Proc0(void);
void Proc1(RecordPtr);
void Proc2(OneToFifty *IntParIO);
void Proc3(RecordPtr*PtrParOut);
void Proc4(void);
void Proc5(void);
void Proc6(Enumeration EnumParIn, Enumeration*EnumParOut);
void Proc7(OneToFifty IntParI1, OneToFifty IntParI2, OneToFifty *IntParOut);
void Proc8(Array1Dim Array1Par, Array2Dim Array2Par, OneToFifty IntParI1, OneToFifty IntParI2);

int main()
{
	Proc0();
        return 0;
}

/*
 * Package 1
 */
int		IntGlob;
boolean		BoolGlob;
char		Char1Glob;
char		Char2Glob;
Array1Dim	Array1Glob;
Array2Dim	Array2Glob;
RecordPtr	PtrGlb;
RecordPtr	PtrGlbNext;

void Proc0(void)
{
	OneToFifty		IntLoc1;
	REG OneToFifty		IntLoc2;
	OneToFifty		IntLoc3;
	REG char		CharLoc;
	REG char		CharIndex;
	Enumeration	 	EnumLoc;
	String30		String1Loc;
	String30		String2Loc;

        float                   benchsecs;
        float                   KIPS;

#ifdef TIME
	long			time();
	long			starttime;
	long			benchtime;
	long			nulltime;
	register unsigned int	i;
	starttime = time(0);
	for (i = 0; i < LOOPS; ++i);
	nulltime = time(0) - starttime; /* Computes overhead of looping */
#endif
#ifdef TIMES
	time_t			starttime;
	time_t			benchtime;
	time_t			nulltime;
	struct tms		tms;
	register unsigned int	i;
	times(&tms); starttime = tms.tms_utime;
	for (i = 0; i < LOOPS; ++i);
	times(&tms);
	nulltime = tms.tms_utime - starttime; /* Computes overhead of looping */
#endif
#ifdef CLOCK
	clock_t			starttime;
	clock_t			benchtime;
	clock_t			nulltime;
	register unsigned int	i;
	starttime = clock();
	for (i = 0; i < LOOPS; ++i);
	nulltime = clock() - starttime; /* Computes overhead of looping */
#endif

	PtrGlbNext = (RecordPtr) malloc(sizeof(RecordType));
	PtrGlb = (RecordPtr) malloc(sizeof(RecordType));
	PtrGlb->PtrComp = PtrGlbNext;
	PtrGlb->Discr = Ident1;
	PtrGlb->EnumComp = Ident3;
	PtrGlb->IntComp = 40;
	strcpy(PtrGlb->StringComp, "DHRYSTONE PROGRAM, SOME STRING");

/*****************
-- Start Timer --
*****************/
#ifdef TIME
	starttime = time(0);
#endif
#ifdef TIMES
	times(&tms); starttime = tms.tms_utime;
#endif
#ifdef CLOCK
        starttime = clock();
#endif
for (i = 0; i < LOOPS; ++i)
{

	Proc5();
	Proc4();
	IntLoc1 = 2;
	IntLoc2 = 3;
	strcpy(String2Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
	EnumLoc = Ident2;
	BoolGlob = ! Func2(String1Loc, String2Loc);
	while (IntLoc1 < IntLoc2)
	{
		IntLoc3 = 5 * IntLoc1 - IntLoc2;
		Proc7(IntLoc1, IntLoc2, &IntLoc3);
		++IntLoc1;
	}
	Proc8(Array1Glob, Array2Glob, IntLoc1, IntLoc3);
	Proc1(PtrGlb);
	for (CharIndex = 'A'; CharIndex <= Char2Glob; ++CharIndex)
		if (EnumLoc == Func1(CharIndex, 'C'))
			Proc6(Ident1, &EnumLoc);
	IntLoc3 = IntLoc2 * IntLoc1;
	IntLoc2 = IntLoc3 / IntLoc1;
	IntLoc2 = 7 * (IntLoc3 - IntLoc2) - IntLoc1;
	Proc2(&IntLoc1);

/*****************
-- Stop Timer --
*****************/
}
#ifdef TIME
        benchtime = time(0) - starttime - nulltime;
        benchsecs = (float) benchtime;
        KIPS = ((long) LOOPS) / (float) benchtime;
#endif
#ifdef TIMES
	times(&tms);
	benchtime = tms.tms_utime - starttime - nulltime;
        benchsecs = (float) benchtime / HZ;
        KIPS = ((long) LOOPS) * HZ / (float) benchtime;
#endif
#ifdef CLOCK
	benchtime = clock() - starttime - nulltime;
        benchsecs = (float) benchtime / CLOCKS_PER_SEC;
        KIPS = ((long) LOOPS) * CLOCKS_PER_SEC / (float) benchtime;
#endif

printf("Dhrystone time for %ld passes = %.1fs\n", (long) LOOPS, benchsecs);
KIPS = KIPS / 1000.0;
if (KIPS > 1000.0)
printf("This machine benchmarks at %.1fM dhrystones/second\n", KIPS / 1000.0);
else
printf("This machine benchmarks at %.1fK dhrystones/second\n", KIPS);
}

void Proc1(PtrParIn)
REG RecordPtr	PtrParIn;
{
#define	NextRecord	(*(PtrParIn->PtrComp))

	structassign(NextRecord, *PtrGlb);
	PtrParIn->IntComp = 5;
	NextRecord.IntComp = PtrParIn->IntComp;
	NextRecord.PtrComp = PtrParIn->PtrComp;
	Proc3(&NextRecord.PtrComp);
	if (NextRecord.Discr == Ident1)
	{
		NextRecord.IntComp = 6;
		Proc6(PtrParIn->EnumComp, &NextRecord.EnumComp);
		NextRecord.PtrComp = PtrGlb->PtrComp;
		Proc7(NextRecord.IntComp, 10, &NextRecord.IntComp);
	}
	else
		structassign(*PtrParIn, NextRecord);

#undef	NextRecord
}

void Proc2(IntParIO)
OneToFifty	*IntParIO;
{
	REG OneToFifty		IntLoc;
	REG Enumeration		EnumLoc;

	IntLoc = *IntParIO + 10;
	for(;;)
	{
		if (Char1Glob == 'A')
		{
			--IntLoc;
			*IntParIO = IntLoc - IntGlob;
			EnumLoc = Ident1;
		}
		if (EnumLoc == Ident1)
			break;
	}
}

void Proc3(PtrParOut)
RecordPtr	*PtrParOut;
{
	if (PtrGlb != NULL)
		*PtrParOut = PtrGlb->PtrComp;
	else
		IntGlob = 100;
	Proc7(10, IntGlob, &PtrGlb->IntComp);
}

void Proc4(void)
{
	REG boolean	BoolLoc;

	BoolLoc = Char1Glob == 'A';
	BoolLoc |= BoolGlob;
	Char2Glob = 'B';
}

void Proc5(void)
{
	Char1Glob = 'A';
	BoolGlob = FALSE;
}

extern boolean Func3();

void Proc6(EnumParIn, EnumParOut)
REG Enumeration	EnumParIn;
REG Enumeration	*EnumParOut;
{
	*EnumParOut = EnumParIn;
	if (! Func3(EnumParIn) )
		*EnumParOut = Ident4;
	switch (EnumParIn)
	{
	case Ident1:	*EnumParOut = Ident1; break;
	case Ident2:	if (IntGlob > 100) *EnumParOut = Ident1;
			else *EnumParOut = Ident4;
			break;
	case Ident3:	*EnumParOut = Ident2; break;
	case Ident4:	break;
	case Ident5:	*EnumParOut = Ident3;
	}
}

void Proc7(IntParI1, IntParI2, IntParOut)
OneToFifty	IntParI1;
OneToFifty	IntParI2;
OneToFifty	*IntParOut;
{
	REG OneToFifty	IntLoc;

	IntLoc = IntParI1 + 2;
	*IntParOut = IntParI2 + IntLoc;
}

void Proc8(Array1Par, Array2Par, IntParI1, IntParI2)
Array1Dim	Array1Par;
Array2Dim	Array2Par;
OneToFifty	IntParI1;
OneToFifty	IntParI2;
{
	REG OneToFifty	IntLoc;
	REG OneToFifty	IntIndex;

	IntLoc = IntParI1 + 5;
	Array1Par[IntLoc] = IntParI2;
	Array1Par[IntLoc+1] = Array1Par[IntLoc];
	Array1Par[IntLoc+30] = IntLoc;
	for (IntIndex = IntLoc; IntIndex <= (IntLoc+1); ++IntIndex)
		Array2Par[IntLoc][IntIndex] = IntLoc;
	++Array2Par[IntLoc][IntLoc-1];
	Array2Par[IntLoc+20][IntLoc] = Array1Par[IntLoc];
	IntGlob = 5;
}

Enumeration Func1(CharPar1, CharPar2)
CapitalLetter	CharPar1;
CapitalLetter	CharPar2;
{
	REG CapitalLetter	CharLoc1;
	REG CapitalLetter	CharLoc2;

	CharLoc1 = CharPar1;
	CharLoc2 = CharLoc1;
	if (CharLoc2 != CharPar2)
		return (Ident1);
	else
		return (Ident2);
}

boolean Func2(StrParI1, StrParI2)
String30	StrParI1;
String30	StrParI2;
{
	REG OneToThirty		IntLoc;
	REG CapitalLetter	CharLoc;

	IntLoc = 1;
	while (IntLoc <= 1)
		if (Func1(StrParI1[IntLoc], StrParI2[IntLoc+1]) == Ident1)
		{
			CharLoc = 'A';
			++IntLoc;
		}
	if (CharLoc >= 'W' && CharLoc <= 'Z')
		IntLoc = 7;
	if (CharLoc == 'X')
		return(TRUE);
	else
	{
		if (strcmp(StrParI1, StrParI2) > 0)
		{
			IntLoc += 7;
			return (TRUE);
		}
		else
			return (FALSE);
	}
}

boolean Func3(EnumParIn)
REG Enumeration	EnumParIn;
{
	REG Enumeration	EnumLoc;

	EnumLoc = EnumParIn;
	if (EnumLoc == Ident3) return (TRUE);
	return (FALSE);
}

#ifdef	NOSTRUCTASSIGN
memcpy(d, s, l)
register char	*d;
register char	*s;
int	l;
{
	while (l--) *d++ = *s++;
}
#endif

/*
 *	Library function for compilers with no time(2) function in the
 *	library.
 */
#ifdef	NOTIME
long	time(p)
long	*p;
{		/* CI-C86 time function - don't use around midnight */
	long	t;
	struct regval {unsigned int ax,bx,cx,dx,si,di,ds,es; } regs;

	regs.ax = 0x2c00;
	sysint21(&regs, &regs);
	t = ((regs.cx>>8)*60L + (regs.cx & 0xff))*60L + (regs.dx>>8);
	if (p) *p = t;
	return t;
}
#endif


