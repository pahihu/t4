/*
 *
 * fparithmetic.c
 *
 * Arithmetic support routines.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fenv.h>
#include "arithmetic.h"
#include "fparithmetic.h"

#pragma STDC FENV_ACCESS ON

#ifdef __APPLE__
#define __INT32_MIN__   ((-__INT32_MAX__)-1)
#endif

#ifdef _MSC_VER
#define __INT32_MIN__	INT32_MIN
#define __INT32_MAX__	INT32_MAX
#endif

/* #define FE_T800_EXCEPT  (FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_INEXACT) */
#define FE_T800_EXCEPT  (FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW)

#define REAL64_SIGN     0x8000000000000000ULL
#define REAL64_EXP      0x7FF0000000000000ULL
#define REAL64_FRAC     0x000FFFFFFFFFFFFFULL

#define REAL64_UNDEFINED ((uint64_t)0x7ff2bad2bad2bad2ULL)

#define INT64(x)        ((int64_t)(x))
#define INT32(x)        ((int32_t)(x))

#define REAL32_SIGN     0x80000000UL
#define REAL32_EXP      0x7F800000UL
#define REAL32_FRAC     0x007FFFFFUL

#define REAL32_UNDEFINED ((uint32_t)0x7f82bad2UL)

#define NAN32_DivZeroByZero     ((uint32_t)0x7fc00000UL)
#define NAN32_DivInfByInf       ((uint32_t)0x7fa00000UL)
#define NAN32_MulZeroByInf      ((uint32_t)0x7f900000UL)
#define NAN32_AddOppositeInf    ((uint32_t)0x7f880000UL)
#define NAN32_SubSameInf        ((uint32_t)0x7f880000UL)
#define NAN32_NegativeSqrt      ((uint32_t)0x7f840000UL)
#define NAN32_ConversionNaN     ((uint32_t)0x7f820000UL)
#define NAN32_RemFromInf        ((uint32_t)0x7f804000UL)
#define NAN32_RemByZero         ((uint32_t)0x7f802000UL)

#define NAN64_DivZeroByZero     ((uint64_t)0x7ff8000000000000ULL)
#define NAN64_DivInfByInf       ((uint64_t)0x7ff4000000000000ULL)
#define NAN64_MulZeroByInf      ((uint64_t)0x7ff2000000000000ULL)
#define NAN64_AddOppositeInf    ((uint64_t)0x7ff1000000000000ULL)
#define NAN64_SubSameInf        ((uint64_t)0x7ff1000000000000ULL)
#define NAN64_NegativeSqrt      ((uint64_t)0x7ff0800000000000ULL)
#define NAN64_ConversionNaN     ((uint64_t)0x7ff0400000000000ULL)
#define NAN64_RemFromInf        ((uint64_t)0x7ff0080000000000ULL)
#define NAN64_RemByZero         ((uint64_t)0x7ff0040000000000ULL)

#undef TRUE
#undef FALSE
#define FALSE   0x0000
#define TRUE    0x0001

const char *RMODE = "PMZN";

REAL32 RInf;
REAL32 RMinusInf;
REAL32 undefined_NaN;
REAL32 unstable_NaN;
REAL32 inexact_NaN;
REAL32 RZero;
REAL32 RUndefined;
REAL32 RInt32Min = (REAL32) __INT32_MIN__;
REAL32 RInt32Max = (REAL32) __INT32_MAX__;

REAL64 DRInf;
REAL64 DRMinusInf;
REAL64 Dundefined_NaN;
REAL64 Dunstable_NaN;
REAL64 Dinexact_NaN;
REAL64 DRZero;
REAL64 DRUndefined;
REAL64 DInt32Min = (REAL64) __INT32_MIN__;
REAL64 DInt32Max = (REAL64) __INT32_MAX__;

/*
 * T800 FPU Not-a-numbers.
 */
static REAL32 DivZeroByZero_NaN;
static REAL32 DivInfByInf_NaN;
static REAL32 MulZeroByInf_NaN;
static REAL32 AddOppositeInf_NaN;
static REAL32 SubSameInf_NaN;
static REAL32 NegativeSqrt_NaN;
static REAL32 Conversion_NaN;
static REAL32 RemFromInf_NaN;
static REAL32 RemByZero_NaN;

static REAL64 DDivZeroByZero_NaN;
static REAL64 DDivInfByInf_NaN;
static REAL64 DMulZeroByInf_NaN;
static REAL64 DAddOppositeInf_NaN;
static REAL64 DSubSameInf_NaN;
static REAL64 DNegativeSqrt_NaN;
static REAL64 DConversion_NaN;
static REAL64 DRemFromInf_NaN;
static REAL64 DRemByZero_NaN;

extern int emudebug;
extern int FP_Error;
extern int RoundingMode;
extern void fp_pushdb (REAL64);
extern void fp_pushsn (REAL32);
extern void handler (int);

REAL32 BargSN, AargSN, ResultSN;
REAL64 BargDB, AargDB, ResultDB;
void db_dump (char*, REAL64);
void sn_dump (char*, REAL32);
void sn_setbits (REAL32*, uint32_t);
void db_setbits (REAL64*, uint64_t);

void fp_init (void)
{
        fenv_t fpenv;
        int rc;

        rc = feholdexcept (&fpenv);
        if (rc)
                printf ("-W-EMUFPU: Warning - cannot initialize FP environment!\n");

        fp_setrounding ("fpinit", ROUND_N);

        sn_setbits (&RInf,           PINFINITY32);
        sn_setbits (&RMinusInf,      MINFINITY32);
        sn_setbits (&undefined_NaN,  NAN32_UNDEFINED);
        sn_setbits (&unstable_NaN,   NAN32_UNSTABLE);
        sn_setbits (&inexact_NaN,    NAN32_INEXACT);
        sn_setbits (&RZero,          ZERO32);
        sn_setbits (&RUndefined,     REAL32_UNDEFINED);

        sn_setbits (&DivZeroByZero_NaN,  NAN32_DivZeroByZero);
        sn_setbits (&DivInfByInf_NaN,    NAN32_DivInfByInf);
        sn_setbits (&MulZeroByInf_NaN,   NAN32_MulZeroByInf);
        sn_setbits (&AddOppositeInf_NaN, NAN32_AddOppositeInf);
        sn_setbits (&SubSameInf_NaN,     NAN32_SubSameInf);
        sn_setbits (&NegativeSqrt_NaN,   NAN32_NegativeSqrt);
        sn_setbits (&Conversion_NaN,     NAN32_ConversionNaN);
        sn_setbits (&RemFromInf_NaN,     NAN32_RemFromInf);
        sn_setbits (&RemByZero_NaN,      NAN32_RemByZero);

        db_setbits (&DRInf,          PINFINITY64);
        db_setbits (&DRMinusInf,     MINFINITY64);
        db_setbits (&Dundefined_NaN, NAN64_UNDEFINED);
        db_setbits (&Dunstable_NaN,  NAN64_UNSTABLE);
        db_setbits (&Dinexact_NaN,   NAN64_INEXACT);
        db_setbits (&DRZero,         ZERO64);
        db_setbits (&DRUndefined,    REAL64_UNDEFINED);

        db_setbits (&DDivZeroByZero_NaN,  NAN64_DivZeroByZero);
        db_setbits (&DDivInfByInf_NaN,    NAN64_DivInfByInf);
        db_setbits (&DMulZeroByInf_NaN,   NAN64_MulZeroByInf);
        db_setbits (&DAddOppositeInf_NaN, NAN64_AddOppositeInf);
        db_setbits (&DSubSameInf_NaN,     NAN64_SubSameInf);
        db_setbits (&DNegativeSqrt_NaN,   NAN64_NegativeSqrt);
        db_setbits (&DConversion_NaN,     NAN64_ConversionNaN);
        db_setbits (&DRemFromInf_NaN,     NAN64_RemFromInf);
        db_setbits (&DRemByZero_NaN,      NAN64_RemByZero);
}

void db_setbits (REAL64 *ptr, uint64_t bits)
{
        uint64_t *rawPtr;

        rawPtr  = (uint64_t *) ptr;
        *rawPtr = bits;
}

void sn_setbits (REAL32 *ptr, uint32_t bits)
{
        uint32_t *rawPtr;

        rawPtr  = (uint32_t *) ptr;
        *rawPtr = bits;
}

void fp_setrounding (const char *where, int mode)
{
        int fpu_mode;
        int rc;

        fpu_mode = FE_TONEAREST;
        switch (mode)
        {
                case ROUND_P:
                        fpu_mode = FE_UPWARD;
                        break;
                case ROUND_M:
                        fpu_mode = FE_DOWNWARD;
                        break;
                case ROUND_Z:
                        fpu_mode = FE_TOWARDZERO;
                        break;
                case ROUND_N:
                        fpu_mode = FE_TONEAREST;
                        break;
                default     :
                        printf ("-E-EMUFPU: Error - unknown rounding mode! (%d)\n", mode);
#ifndef FPA_STANDALONE
                        handler (-1);
#endif
        }
        
#ifndef FPA_STANDALONE
        RoundingMode = mode;
        if (emudebug)
                printf ("-I-EMUFPU: RoundingMode set to '%c' (%s).\n", RMODE[mode - 1], where);
#endif

        rc = fesetround (fpu_mode);
        if (rc != 0)
                printf ("-W-EMUFPU: Warning - cannot set rounding mode! (%d)\n", mode);
}

int db_sign (uint64_t fpbits)
{
        return (REAL64_SIGN & fpbits) ? 1 : 0;
}

int fp_signdb(REAL64 fp)
{
        fpreal64_t x;

        x.fp = fp;
        return db_sign (x.bits);
}

int fp_expdb(REAL64 fp)
{
        fpreal64_t x;

        x.fp = fp;
        return (REAL64_EXP & x.bits) >> 52;
}

uint64_t fp_fracdb(REAL64 fp)
{
        fpreal64_t x;

        x.fp = fp;
        return (REAL64_FRAC & x.bits);
}

int fp_infdb(REAL64 fp)
{
        return (2047 == fp_expdb (fp)) && (0LL == fp_fracdb (fp));
}

int fp_nandb(REAL64 fp)
{
        return (2047 == fp_expdb (fp)) && (0LL != fp_fracdb (fp));
}

int fp_notfinitedb(REAL64 fp)
{
        return (2047 == fp_expdb (fp));
}

int fp_zerodb(REAL64 fp)
{
        fpreal64_t r64;

        r64.fp = fp;
        /* Minus zero is the same as plus zero. */
        return ((r64.bits & ~REAL64_SIGN) == ZERO64);
}

int sn_sign (uint32_t fpbits)
{
        return (REAL32_SIGN & fpbits) ? 1 : 0;
}

int fp_signsn(REAL32 fp)
{
        fpreal32_t x;

        x.fp = fp;
        return sn_sign (x.bits);
}

int fp_expsn(REAL32 fp)
{
        fpreal32_t x;

        x.fp = fp;
        return (REAL32_EXP & x.bits) >> 23;
}

uint32_t fp_fracsn(REAL32 fp)
{
        fpreal32_t x;

        x.fp = fp;
        return (REAL32_FRAC & x.bits);
}

int fp_infsn(REAL32 fp)
{
        return (255 == fp_expsn (fp)) && (0L == fp_fracsn (fp));
}

int fp_nansn(REAL32 fp)
{
        return (255 == fp_expsn (fp)) && (0L != fp_fracsn (fp));
}

int fp_notfinitesn(REAL32 fp)
{
        return (255 == fp_expsn (fp));
}

int fp_zerosn(REAL32 fp)
{
        fpreal32_t r32;

        r32.fp = fp;
        /* Minus zero is the same as plus zero. */
        return ((r32.bits & ~REAL32_SIGN) == ZERO32);
}

void db_dump (char *msg, REAL64 fp)
{
        fpreal64_t x;

        x.fp = fp;
        printf ("%s: fp = %lf (#%016llX, %d,%04d,%014llX)\n",
                msg, fp, x.bits,
                fp_signdb(fp) ? 1 : 0,
                fp_expdb(fp),
                fp_fracdb(fp));
}

void sn_dump (char *msg, REAL32 fp)
{
        fpreal32_t x;

        x.fp = fp;
        printf ("%s: fp = %f (#%08X, %d,%03d,%06X)\n",
                msg, fp, x.bits,
                fp_signsn(fp) ? 1 : 0,
                fp_expsn(fp),
                fp_fracsn(fp));
}

/* Clear native FPU exceptions. */
void fp_clrexcept (void)
{
        int rc;

        rc = feclearexcept (FE_T800_EXCEPT);
        if (rc)
                printf ("-W-EMUFPU: Warning - cannot clear native FPU exceptions!\n");
}

/* Check native FPU exceptions. */
void fp_chkexcept (char *msg)
{
        int exc;

        exc = fetestexcept (FE_T800_EXCEPT);
        if (exc)
        {
                printf ("-W-EMUFPU: Warning - FPU exception flags are set! (%s)\n", msg);
        }
}

#define setclear(x)        (x ? "Set" : "Clear")

/* Translate native FPU exception to FP_Error. */
void translate_except (int excp)
{
#if 0
        printf ("-W-EMUFPU: FPExceptFlag   = %d\n", excp);
        printf ("-W-EMUFPU:   Invalid      %s\n", setclear (excp & FE_INVALID));
        printf ("-W-EMUFPU:   DivideByZero %s\n", setclear (excp & FE_DIVBYZERO));
        printf ("-W-EMUFPU:   Overflow     %s\n", setclear (excp & FE_OVERFLOW));
        printf ("-W-EMUFPU:   Underflow    %s\n", setclear (excp & FE_UNDERFLOW));
        printf ("-W-EMUFPU:   Inexact      %s\n", setclear (excp & FE_INEXACT));
#endif

        if (excp & FE_INVALID)
                FP_Error = TRUE;
        else if (excp & FE_DIVBYZERO)
                FP_Error = TRUE;
        else if (excp & FE_OVERFLOW)
                FP_Error = TRUE;
}

REAL64 db_check_except (REAL64 result)
{
        int excp;

        excp = fetestexcept (FE_T800_EXCEPT);
        if (0 == excp)
                return result;

#if 0
        printf  ("-W-EMUFPU: Native FPU exception!\n");
        printf  ("-W-EMUFPU: Operation arguments.\n");
        db_dump ("-W-EMUFPU:   Barg", BargDB);
        db_dump ("-W-EMUFPU:   Aarg", AargDB);
        db_dump ("-W-EMUFPU: Result", ResultDB);
#endif

        translate_except (excp);
/*
        if (excp & FE_INEXACT)
                result = Real64InexactNaN;
*/
        fp_clrexcept ();

        return result;
}

REAL32 sn_check_except (REAL32 result)
{
        int excp;

        excp = fetestexcept (FE_T800_EXCEPT);
        if (0 == excp)
                return result;

#if 0
        printf  ("-W-EMUFPU: Native FPU exception!\n");
        printf  ("-W-EMUFPU: Operation arguments.\n");
        sn_dump ("-W-EMUFPU:   Barg", BargSN);
        sn_dump ("-W-EMUFPU:   Aarg", AargSN);
        sn_dump ("-W-EMUFPU: Result", ResultSN);
#endif

        translate_except (excp);
/*
        if (excp & FE_INEXACT)
                result = Real32InexactNaN;
*/
        fp_clrexcept ();

        return result;
}

REAL64 db_add(REAL64, REAL64);
REAL64 db_sub(REAL64, REAL64);
REAL64 db_mul(REAL64, REAL64);
REAL64 db_div(REAL64, REAL64);

/* Do a binary REAL64 operation, return REAL64 result. */
REAL64 db_binary (REAL64 fb, REAL64 fa, REAL64 (*opr)(REAL64, REAL64))
{
        REAL64 result;
        uint64_t fracb, fraca;

        fp_chkexcept ("Enter db_binary ()");

#ifndef NDEBUG
        BargDB = fb; AargDB = fa; ResultDB = DRUndefined;
#endif

        /* Set FP_Error on NaN. */
        if (fp_nandb (fb) && fp_nandb (fb))
        {
                FP_Error = TRUE;
                fracb = fp_fracdb (fb);
                fraca = fp_fracdb (fa);
                if (fracb == fraca)
                        return fb;
                return (fracb > fraca) ? fb : fa;
        }
        else if (fp_nandb (fb))
        {
                FP_Error = TRUE;
                return fb;
        }
        else if (fp_nandb (fa))
        {
                FP_Error = TRUE;
                return fa;
        }

        if (opr == db_add)
        {
                if (fp_infdb (fb) && fp_infdb (fa))
                {
                        FP_Error = TRUE;
                        if (fp_signdb (fb) != fp_signdb (fa))
                                return DAddOppositeInf_NaN;
                }
        }
        else if (opr == db_sub)
        {
                if (fp_infdb (fb) && fp_infdb (fa))
                {
                        FP_Error = TRUE;
                        if (fp_signdb (fb) == fp_signdb (fa))
                                return DSubSameInf_NaN;
                }
        }
        else if (opr == db_mul)
        {
                if ((fp_zerodb (fb) && fp_infdb (fa)) ||
                    (fp_infdb (fb)  && fp_zerodb (fa)))
                {
                        FP_Error = TRUE;
                        return DMulZeroByInf_NaN;
                }
        }

        if (fp_infdb (fa) || fp_infdb (fb))
                FP_Error = TRUE;

        result = opr(fb, fa);

#ifndef NDEBUG
        ResultDB = result;
#endif

        result = db_check_except (result);
        return result;
}

/* Do a binary operation on two REAL64 numbers, return the result flag. */
int db_binary2word (REAL64 fb, REAL64 fa, int (*opr)(REAL64, REAL64))
{
        int result;

#ifndef NDEBUG
        BargDB = fb; AargDB = fa; ResultDB = DRUndefined;
#endif

        if (fp_notfinitedb (fb) || fp_notfinitedb (fa))
        {
                FP_Error = TRUE;
        }

        result = opr(fb, fa);

#ifndef NDEBUG
        ResultDB = result;
#endif

        db_check_except (0.0);
        return result;
}

/* Do an unary REAL64 operation, return REAL64 result. */
REAL64 db_unary (REAL64 fa, REAL64 (*opr)(REAL64))
{
        REAL64 result;

#ifndef NDEBUG
        AargDB = fa; ResultDB = DRUndefined;
#endif
        if (fp_nandb (fa))
        {
                FP_Error = TRUE;
                return fa;
        }

        if (fp_infdb (fa))
                FP_Error = TRUE;

        result = opr(fa);

#ifndef NDEBUG
        ResultDB = result;
#endif

        result = db_check_except (result);
        return result;
}

REAL32 sn_add(REAL32, REAL32);
REAL32 sn_sub(REAL32, REAL32);
REAL32 sn_mul(REAL32, REAL32);
REAL32 sn_div(REAL32, REAL32);

/* Do a binary REAL64 operation, return REAL64 result. */
REAL32 sn_binary (REAL32 fb, REAL32 fa, REAL32 (*opr)(REAL32, REAL32))
{
        REAL32 result;
        uint32_t fracb, fraca;

        fp_chkexcept ("Enter sn_binary ()");

#ifndef NDEBUG
        BargSN = fb; AargSN = fa; ResultSN = RUndefined;
#endif

        /* Set FP_Error on NaN. */
        if (fp_nansn (fb) && fp_nansn (fa))
        {
                FP_Error = TRUE;
                fracb = fp_fracsn (fb);
                fraca = fp_fracsn (fa);
                if (fracb == fraca)
                        return fb;
                return (fracb > fraca) ? fb : fa;
        }
        else if (fp_nansn (fb))
        {
                FP_Error = TRUE;
                return fb;
        }
        else if (fp_nansn (fa))
        {
                FP_Error = TRUE;
                return fa;
        }

        if (opr == sn_add)
        {
                if (fp_infsn (fb) && fp_infsn (fa))
                {
                        FP_Error = TRUE;
                        if (fp_signsn (fb) != fp_signsn (fa))
                                return AddOppositeInf_NaN;
                }
        }
        else if (opr == sn_sub)
        {
                if (fp_infsn (fb) && fp_infsn (fa))
                {
                        FP_Error = TRUE;
                        if (fp_signsn (fb) == fp_signsn (fa))
                                return SubSameInf_NaN;
                }
        }
        else if (opr == sn_mul)
        {
                if ((fp_zerosn (fb) && fp_infsn (fa)) ||
                    (fp_infsn (fb)  && fp_zerosn (fa)))
                {
                        FP_Error = TRUE;
                        return MulZeroByInf_NaN;
                }
        }

        if (fp_infsn (fb) || fp_infsn (fa))
                FP_Error = TRUE;

        result = opr(fb, fa);

#ifndef NDEBUG
        ResultSN = result;
#endif

        result = sn_check_except (result);
        return result;
}

/* Do a binary operation on two REAL32 numbers, return the result flag. */
int sn_binary2word (REAL32 fb, REAL32 fa, int (*opr)(REAL32, REAL32))
{
        int result;

#ifndef NDEBUG
        BargSN = fb; AargSN = fa; ResultSN = RUndefined;
#endif

        if (fp_notfinitesn (fb) || fp_notfinitesn (fa))
        {
                FP_Error = TRUE;
        }

        result = opr(fb, fa);

#ifndef NDEBUG
        ResultSN = result;
#endif

        sn_check_except (0.0F);
        return result;
}

/* Do an unary REAL32 operation, return REAL32 result. */
REAL32 sn_unary (REAL32 fa, REAL32 (*opr)(REAL32))
{
        REAL32 result;

#ifndef NDEBUG
        AargSN = fa; ResultSN = RUndefined;
#endif

        if (fp_nandb (fa))
        {
                FP_Error = TRUE;
                return fa;
        }

        if (fp_infdb (fa))
                FP_Error = TRUE;

        result = opr(fa);

#ifndef NDEBUG
        ResultSN = result;
#endif

        result = sn_check_except (result);
        return result;
}


REAL64 DQuotRem (REAL64 X, REAL64 Y, long *N)
{
        REAL64 rem;

        rem = remainder (X, Y);
        *N  = (X - rem) / Y;
        fp_clrexcept ();

        return rem;
}

REAL32 RQuotRem (REAL32 X, REAL32 Y, long *N)
{
        REAL32 rem;

        rem = remainder (X, Y);
        *N  = (X - rem) / Y;
        fp_clrexcept ();

        return rem;
}

/* 
 * REAL64 basic operations.
 */
REAL64 db_add(REAL64 fb, REAL64 fa)      { return fb + fa; }
REAL64 db_sub(REAL64 fb, REAL64 fa)      { return fb - fa; }
REAL64 db_mul(REAL64 fb, REAL64 fa)      { return fb * fa; }
REAL64 db_div(REAL64 fb, REAL64 fa)
{ 
        REAL64 result;
        fpreal64_t r64;

        if (fp_zerodb (fb) && fp_zerodb (fa))
        {
                FP_Error = TRUE;
                return DDivZeroByZero_NaN;
        }
        else if (fp_infdb (fb) && fp_infdb (fa))
        {
                FP_Error = TRUE;
                return DDivInfByInf_NaN;
        }

        result = fb / fa;

        if (fp_signdb (fb) == fp_signdb (fa))
        {
                r64.fp    = result;
                r64.bits &= ~REAL64_SIGN;
                result    = r64.fp;
        }
        else
        {
                r64.fp    = result;
                r64.bits |= REAL64_SIGN;
                result    = r64.fp;
        }

        return result;
}

REAL64 db_mulby2 (REAL64 fa)             { return ldexp (fa,  1); }
REAL64 db_divby2 (REAL64 fa)             { return ldexp (fa, -1); }
REAL64 db_expinc32 (REAL64 fa)           { return ldexp (fa,  32); }
REAL64 db_expdec32 (REAL64 fa)           { return ldexp (fa, -32); }
REAL64 db_sqrt (REAL64 fa)               { return sqrt (fa); }
REAL64 db_remfirst(REAL64 fb, REAL64 fa) { return fmod (fb, fa); }
int    db_gt (REAL64 fb, REAL64 fa)      { return fb  > fa; }
int    db_eq (REAL64 fb, REAL64 fa)      { return fb == fa; }


/* 
 * REAL32 basic operations.
 */
REAL32 sn_add (REAL32 fb, REAL32 fa)     { return fb + fa; }
REAL32 sn_sub (REAL32 fb, REAL32 fa)     { return fb - fa; }
REAL32 sn_mul (REAL32 fb, REAL32 fa)     { return fb * fa; }
REAL32 sn_div (REAL32 fb, REAL32 fa)
{ 
        REAL32 result;
        fpreal32_t r32;

        if (fp_zerosn (fb) && fp_zerosn (fa))
        {
                FP_Error = TRUE;
                return DivZeroByZero_NaN;
        }
        else if (fp_infsn (fb) && fp_infsn (fa))
        {
                FP_Error = TRUE;
                return DivInfByInf_NaN;
        }

        result = fb / fa;

        if (fp_signsn (fb) == fp_signsn (fa))
        {
                r32.fp    = result;
                r32.bits &= ~REAL32_SIGN;
                result    = r32.fp;
        }
        else
        {
                r32.fp    = result;
                r32.bits |= REAL32_SIGN;
                result    = r32.fp;
        }

        return result;
}
REAL32 sn_mulby2 (REAL32 fa)             { return ldexpf (fa,   1); }
REAL32 sn_divby2 (REAL32 fa)             { return ldexpf (fa,  -1); }
REAL32 sn_expinc32 (REAL32 fa)           { return ldexpf (fa,  32); }
REAL32 sn_expdec32 (REAL32 fa)           { return ldexpf (fa, -32); }
REAL32 sn_sqrt (REAL32 fa)               { return  sqrtf (fa); }
REAL32 sn_remfirst (REAL32 fb, REAL32 fa){ return fmodf (fb, fa); }
int    sn_gt (REAL32 fb, REAL32 fa)      { return fb  > fa; }
int    sn_eq (REAL32 fb, REAL32 fa)      { return fb == fa; }


/*
 * Checked REAL64 operations.
 */
REAL64 fp_adddb (REAL64 fb, REAL64 fa)   { return db_binary (fb, fa, db_add); }
REAL64 fp_subdb (REAL64 fb, REAL64 fa)   { return db_binary (fb, fa, db_sub); }
REAL64 fp_muldb (REAL64 fb, REAL64 fa)   { return db_binary (fb, fa, db_mul); }
REAL64 fp_divdb (REAL64 fb, REAL64 fa)   { return db_binary (fb, fa, db_div); }
REAL64 fp_mulby2db (REAL64 fa)           { return db_unary (fa, db_mulby2); }
REAL64 fp_divby2db (REAL64 fa)           { return db_unary (fa, db_divby2); }
REAL64 fp_expinc32db (REAL64 fa)         { return db_unary (fa, db_expinc32); }
REAL64 fp_expdec32db (REAL64 fa)         { return db_unary (fa, db_expdec32); }
REAL64 fp_absdb (REAL64 fa)
{
        fpreal64_t x;
        REAL64 result;

        if (fp_nandb (fa))
        {
                x.fp = fa;
                if (db_sign (x.bits))
                        x.bits ^= REAL64_SIGN;
                FP_Error = TRUE;
                return x.fp;
        }
        else if (fp_infdb (fa))
                FP_Error = TRUE;

        result = fabs (fa);
        fp_clrexcept ();

        return result;
}
REAL64 fp_sqrtfirstdb (REAL64 fa)
{
        if (fp_notfinitedb (fa))
                FP_Error = TRUE;
        if (fa < 0.0)
                return DNegativeSqrt_NaN;

        return fa;
}
REAL64 fp_sqrtlastdb (REAL64 fa)         { return db_unary (fa, db_sqrt); }
REAL64 fp_remfirstdb (REAL64 fb, REAL64 fa)
{
        REAL64 result;
        long N;

        if (fp_infdb (fb))
                return DRemFromInf_NaN;
        if (fp_zerodb (fa))
                return DRemByZero_NaN;
#if 0
        result = db_binary (fb, fa, db_remfirst);
        fp_pushdb (fp_divdb (fp_subdb (fb, result), fa));
#else
        result = DQuotRem (fb, fa, &N);
#ifndef FPA_STANDALONE
        fp_pushdb ((REAL64) N);
#endif
#endif

        return result;
}
int    fp_gtdb (REAL64 fb, REAL64 fa)
{
        fpreal64_t rb, ra;

        /* ACWG pp.158: checks (Inf U NaN) membership, db_binary2word (fb, fa, db_gt) */
        /* ACWG pp.106 B.2.7: binary comparison on total ordering */
        rb.fp = fb;
        ra.fp = fa;
        return INT64(rb.bits) > INT64(ra.bits);
}
int    fp_eqdb (REAL64 fb, REAL64 fa)
{
        fpreal64_t rb, ra;

        /* ACWG pp.158: checks (Inf U NaN) membership, db_binary2word (fb, fa, db_eq) */
        /* ACWG pp.106 B.2.7: binary comparison on total ordering */
        rb.fp = fb;
        ra.fp = fa;
        return rb.bits == ra.bits;
}
int    fp_ordereddb (REAL64 fb, REAL64 fa)
{
        if (fp_nandb (fb) || fp_nandb (fa))
                return FALSE;
        return TRUE;
}
REAL32  fp_r64tor32 (REAL64 fp)
{
        REAL32     result;

#ifndef NDEBUG
        AargDB = fp; ResultSN = RUndefined;
#endif

        if (fp_notfinitedb (fp))
                FP_Error = TRUE;

        result = (REAL32) fp;

#ifndef NDEBUG
        ResultSN = result;
#endif

        result = sn_check_except (result);
        if (fp_nandb (fp) && fp_infsn (result))
                result = Conversion_NaN;

        return result;
}
REAL64 fp_intdb (REAL64 fp)
{
        REAL64 result;

#ifndef NDEBUG
        AargDB = fp; ResultDB = DRUndefined;
#endif

        if (fp_notfinitedb (fp))
                FP_Error = TRUE;

        result = rint (fp);

#ifndef NDEBUG
        ResultDB = result;
#endif

        result = db_check_except (result);
        return result;
}
int fp_chki32db (REAL64 fp)
{
        long result;

#ifndef NDEBUG
        AargDB = fp; ResultDB = DRUndefined;
#endif

        if (fp_notfinitedb (fp))
        {
                FP_Error = TRUE;
                return FALSE;
        }

        result = lrint (fp);

#ifndef NDEBUG
        ResultDB = result;
#endif

        db_check_except (0.0);
        if (FP_Error)
                return FALSE;

        return (__INT32_MIN__ <= result) && (result <= __INT32_MAX__);
}
int fp_chki64db (REAL64 fp)
{
        long long result;

#ifndef NDEBUG
        AargDB = fp; ResultDB = DRUndefined;
#endif

        if (fp_notfinitedb (fp))
        {
                FP_Error = TRUE;
                return FALSE;
        }

        result = llrintf (fp);

#ifndef NDEBUG
        ResultDB = result;
#endif

        db_check_except (0.0);
        if (FP_Error)
                return FALSE;
        return TRUE;
}
REAL64 fp_rtoi32db (REAL64 fp)
{
        fp_chki32db (fp);
        return fp_intdb (fp);
}
REAL32 fp_norounddb (REAL64 fp)
{
        fpreal32_t r32;
        int exp;
        int sign;
        uint64_t frac;

        sign = fp_signdb (fp);
        exp  = fp_expdb (fp);
        frac = fp_fracdb (fp);

        exp -= 1023;
        if ((exp < -126) || (exp > 127))
        {
                exp = 128;
                frac = 0;
        }
        exp = (exp + 127) & 0xff;
        r32.bits = (sign ? REAL32_SIGN : 0) + 
                   (REAL32_EXP  & (exp  << 23)) +
                   (REAL32_FRAC & (frac >> 29));
        return r32.fp;
}
uint32_t fp_stnli32db (REAL64 fp)
{
        union {
                int32_t i;
                uint32_t u;
        } result;

        fp = trunc (fp);
        result.i = (int32_t) fp;
        fp_clrexcept ();

        return result.u;
}



/*
 * Checked REAL32 operations.
 */
REAL32 fp_addsn (REAL32 fb, REAL32 fa)   { return sn_binary (fb, fa, sn_add); }
REAL32 fp_subsn (REAL32 fb, REAL32 fa)   { return sn_binary (fb, fa, sn_sub); }
REAL32 fp_mulsn (REAL32 fb, REAL32 fa)   { return sn_binary (fb, fa, sn_mul); }
REAL32 fp_divsn (REAL32 fb, REAL32 fa)   { return sn_binary (fb, fa, sn_div); }
REAL32 fp_mulby2sn (REAL32 fa)           { return sn_unary (fa, sn_mulby2); }
REAL32 fp_divby2sn (REAL32 fa)           { return sn_unary (fa, sn_divby2); }
REAL32 fp_expinc32sn (REAL32 fa)         { return sn_unary (fa, sn_expinc32); }
REAL32 fp_expdec32sn (REAL32 fa)         { return sn_unary (fa, sn_expdec32); }
REAL32 fp_abssn (REAL32 fa)
{
        fpreal32_t x;
        REAL32 result;

        if (fp_nansn (fa))
        {
                x.fp = fa;
                if (sn_sign (x.bits))
                        x.bits ^= REAL32_SIGN;
                FP_Error = TRUE;
                return x.fp;
        }
        else if (fp_infsn (fa))
                FP_Error = TRUE;

        result =  fabsf (fa);
        fp_clrexcept ();

        return result;
}
REAL32 fp_sqrtfirstsn (REAL32 fa)
{
        if (fp_notfinitesn (fa))
                FP_Error = TRUE;
        if (fa < 0.0)
                return NegativeSqrt_NaN;

        return fa;
}
REAL32 fp_sqrtlastsn (REAL32 fa)         { return sn_unary (fa, sn_sqrt); }
REAL32 fp_remfirstsn (REAL32 fb, REAL32 fa)
{
        REAL32 result;
        long N;

        if (fp_infsn (fb))
                return RemFromInf_NaN;
        if (fp_zerosn (fa))
                return RemByZero_NaN;

#if 0
        result = sn_binary (fb, fa, sn_remfirst);
        fp_pushsn (fp_divsn (fp_subsn (fb, result), fa));
#else
        result = RQuotRem (fb, fa, &N);
#ifndef FPA_STANDALONE
        fp_pushsn ((REAL32) N);
#endif
#endif

        return result;
}
int    fp_gtsn (REAL32 fb, REAL32 fa)
{
        fpreal32_t rb, ra;

        /* ACWG pp.158: checks (Inf U NaN) membership, sn_binary2word (fb, fa, sn_gt) */
        /* ACWG pp.106 B.2.7: binary comparison on total ordering */
        rb.fp = fb;
        ra.fp = fa;
        return INT32(rb.bits) > INT32(ra.bits);
}
int    fp_eqsn (REAL32 fb, REAL32 fa)
{
        fpreal32_t rb, ra;

        /* ACWG pp.158: checks (Inf U NaN) membership, sn_binary2word (fb, fa, sn_eq) */
        /* ACWG pp.106 B.2.7: binary comparison on total ordering */
        rb.fp = fb;
        ra.fp = fa;
        return rb.bits == ra.bits;
}
int    fp_orderedsn (REAL32 fb, REAL32 fa)
{
        if (fp_nansn (fb) || fp_nansn (fa))
                return FALSE;
        return TRUE;
}
REAL64  fp_r32tor64 (REAL32 fp)
{
        REAL64 result;

#ifndef NDEBUG
        AargDB = fp; ResultDB = DRUndefined;
#endif

        if (fp_notfinitesn (fp))
                FP_Error = TRUE;

        result = (REAL64) fp;

#ifndef NDEBUG
        ResultDB = result;
#endif

        result = db_check_except (result);
        return result;
}
REAL32 fp_intsn (REAL32 fp)
{
        REAL32 result;

#ifndef NDEBUG
        AargSN = fp; ResultSN = RUndefined;
#endif

        if (fp_notfinitesn (fp))
                FP_Error = TRUE;

        result = rintf (fp);

#ifndef NDEBUG
        ResultSN = result;
#endif

        result = sn_check_except (result);
        return result;
}
int fp_chki32sn (REAL32 fp)
{
        long result;

#ifndef NDEBUG
        AargSN = fp; ResultSN = RUndefined;
#endif

        if (fp_notfinitesn (fp))
        {
                FP_Error = TRUE;
                return FALSE;
        }

        result = lrintf (fp);

#ifndef NDEBUG
        ResultSN = result;
#endif

        sn_check_except (0.0F);
        if (FP_Error)
                return FALSE;
        return (__INT32_MIN__ <= result) && (result <= __INT32_MAX__);
}
int fp_chki64sn (REAL32 fp)
{
        long long result;

#ifndef NDEBUG
        AargSN = fp; ResultSN = RUndefined;
#endif

        if (fp_notfinitesn (fp))
        {
                FP_Error = TRUE;
                return FALSE;
        }
        result = llrintf (fp);

#ifndef NDEBUG
        ResultSN = result;
#endif

        sn_check_except (0.0F);
        if (FP_Error)
                return FALSE;
        return TRUE;
}
REAL32 fp_rtoi32sn (REAL32 fp)
{
        fp_chki32sn (fp);
        return fp_intsn (fp);
}
uint32_t fp_stnli32sn (REAL32 fp)
{
        union {
                int32_t i;
                uint32_t u;
        } result;

        fp = truncf (fp);
        result.i = (int32_t) fp;
        fp_clrexcept ();

        return result.u;
}
