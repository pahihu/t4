/*
 *
 * arithmetic.c
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

#define __INT32_MIN__   ((-__INT32_MAX__)-1)

/* #define FE_T800_EXCEPT  (FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_INEXACT) */
#define FE_T800_EXCEPT  (FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW)

#define REAL64_SIGN     0x8000000000000000LL
#define REAL64_EXP      0x7FF0000000000000LL
#define REAL64_FRAC_MSB 0x0010000000000000LL
#define REAL64_FRAC     0x000FFFFFFFFFFFFFLL

#define NAN64_UNKNOWN   ((uint64_t)0x7ff2bad2bad2bad2LL)

#define INT64(x)        ((int64_t)(x))
#define INT32(x)        ((int32_t)(x))

#define REAL32_SIGN     0x80000000L
#define REAL32_EXP      0x7F800000L
#define REAL32_FRAC     0x007FFFFFL
#define REAL32_FRAC_MSB 0x00800000L

#define NAN32_UNKNOWN   ((uint32_t)0x7f82bad2)

#undef TRUE
#undef FALSE
#define FALSE   0x0000
#define TRUE    0x0001

REAL32 Real32PlusInf;
REAL32 Real32MinusInf;
REAL32 Real32UndefinedNaN;
REAL32 Real32UnstableNaN;
REAL32 Real32InexactNaN;
REAL32 Real32Zero;
REAL32 Real32Undefined_p;

REAL64 Real64PlusInf;
REAL64 Real64MinusInf;
REAL64 Real64UndefinedNaN;
REAL64 Real64UnstableNaN;
REAL64 Real64InexactNaN;
REAL64 Real64Zero;
REAL32 Real32ConversionNaN;
REAL64 Real64Undefined_p;

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
                printf ("-W-EMU414: Warning - cannot initialize FP environment!\n");

        sn_setbits (&Real32PlusInf,      PINFINITY32);
        sn_setbits (&Real32MinusInf,     MINFINITY32);
        sn_setbits (&Real32UndefinedNaN, NAN32_UNDEFINED);
        sn_setbits (&Real32UnstableNaN,  NAN32_UNSTABLE);
        sn_setbits (&Real32InexactNaN,   NAN32_INEXACT);
        sn_setbits (&Real32Zero,         ZERO32);
        sn_setbits (&Real32Undefined_p,  NAN32_UNKNOWN);

        db_setbits (&Real64PlusInf,      PINFINITY64);
        db_setbits (&Real64MinusInf,     MINFINITY64);
        db_setbits (&Real64UndefinedNaN, NAN64_UNDEFINED);
        db_setbits (&Real64UnstableNaN,  NAN64_UNSTABLE);
        db_setbits (&Real64InexactNaN,   NAN64_INEXACT);
        db_setbits (&Real64Zero,         ZERO64);
        db_setbits (&Real64Undefined_p,  NAN64_UNKNOWN);
        sn_setbits (&Real32ConversionNaN, NAN32_CONVERSION64);
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

void fp_setrounding (int mode)
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
                        printf ("-E-EMU414: Error - unknown rounding mode! (%d)\n", mode);
#ifndef FPA_STANDALONE
                        handler (-1);
#endif
        }
        
#ifndef FPA_STANDALONE
        RoundingMode = mode;
#endif
        rc = fesetround (fpu_mode);
        if (rc != 0)
                printf ("-W-EMU414: Warning - cannot set rounding mode! (%d)\n", mode);
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
        return REAL64_FRAC_MSB | (REAL64_FRAC & x.bits);
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
        return REAL32_FRAC_MSB | (REAL32_FRAC & x.bits);
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
                printf ("-W-EMU414: Warning - cannot clear native FPU exceptions!\n");
}

/* Check native FPU exceptions. */
void fp_chkexcept (char *msg)
{
        int exc;

        exc = fetestexcept (FE_T800_EXCEPT);
        if (exc)
        {
                printf ("-W-EMU414: Warning - FPU exception flags are set! (%s)\n", msg);
        }
}

#define setclear(x)        (x ? "Set" : "Clear")

/* Translate native FPU exception to FP_Error. */
void translate_except (int excp)
{
#if 1
        printf ("-W-EMU414: FPExceptFlag   = %d\n", excp);
        printf ("-W-EMU414:   Invalid      %s\n", setclear (excp & FE_INVALID));
        printf ("-W-EMU414:   DivideByZero %s\n", setclear (excp & FE_DIVBYZERO));
        printf ("-W-EMU414:   Overflow     %s\n", setclear (excp & FE_OVERFLOW));
        printf ("-W-EMU414:   Underflow    %s\n", setclear (excp & FE_UNDERFLOW));
        printf ("-W-EMU414:   Inexact      %s\n", setclear (excp & FE_INEXACT));
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
        printf  ("-W-EMU414: Native FPU exception!\n");
        printf  ("-W-EMU414: Operation arguments.\n");
        db_dump ("-W-EMU414:   Barg", BargDB);
        db_dump ("-W-EMU414:   Aarg", AargDB);
        db_dump ("-W-EMU414: Result", ResultDB);
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
        printf  ("-W-EMU414: Native FPU exception!\n");
        printf  ("-W-EMU414: Operation arguments.\n");
        sn_dump ("-W-EMU414:   Barg", BargSN);
        sn_dump ("-W-EMU414:   Aarg", AargSN);
        sn_dump ("-W-EMU414: Result", ResultSN);
#endif

        translate_except (excp);
/*
        if (excp & FE_INEXACT)
                result = Real32InexactNaN;
*/
        fp_clrexcept ();

        return result;
}

/* Do a binary REAL64 operation, return REAL64 result. */
REAL64 db_binary (REAL64 fb, REAL64 fa, REAL64 (*opr)(REAL64, REAL64))
{
        REAL64 result;
        uint64_t fracb, fraca;

        fp_chkexcept ("Enter db_binary ()");

#ifndef NDEBUG
        BargDB = fb; AargDB = fa; ResultDB = Real64Undefined_p;
#endif

        if (fp_nandb (fb) && fp_nandb (fb))
        {
                fracb = fp_fracdb (fb);
                fraca = fp_fracdb (fa);
                if (INT64(fracb) > INT64(fraca))
                        return fb;
                else
                        return fa;
        }
        else if (fp_nandb (fb))
                return fb;
        else if (fp_nandb (fa))
                return fa;

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
        BargDB = fb; AargDB = fa; ResultDB = Real64Undefined_p;
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
        AargDB = fa; ResultDB = Real64Undefined_p;
#endif
        if (fp_nandb (fa))
                return fa;

        if (fp_infdb (fa))
                FP_Error = TRUE;

        result = opr(fa);

#ifndef NDEBUG
        ResultDB = result;
#endif

        result = db_check_except (result);
        return result;
}

/* Do a binary REAL64 operation, return REAL64 result. */
REAL32 sn_binary (REAL32 fb, REAL32 fa, REAL32 (*opr)(REAL32, REAL32))
{
        REAL32 result;
        uint32_t fracb, fraca;

        fp_chkexcept ("Enter sn_binary ()");

#ifndef NDEBUG
        BargSN = fb; AargSN = fa; ResultSN = Real32Undefined_p;
#endif

        if (fp_nansn (fb) && fp_nansn (fa))
        {
                fracb = fp_fracsn (fb);
                fraca = fp_fracsn (fa);
                if (INT32(fracb) > INT32(fraca))
                        return fb;
                else
                        return fa;
        }
        else if (fp_nansn (fb))
                return fb;
        else if (fp_nansn (fa))
                return fa;

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
        BargSN = fb; AargSN = fa; ResultSN = Real32Undefined_p;
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
        AargSN = fa; ResultSN = Real32Undefined_p;
#endif

        if (fp_nandb (fa))
                return fa;

        if (fp_infdb (fa))
                FP_Error = TRUE;

        result = opr(fa);

#ifndef NDEBUG
        ResultSN = result;
#endif

        result = sn_check_except (result);
        return result;
}


/* 
 * REAL64 basic operations.
 */
REAL64 db_add(REAL64 fb, REAL64 fa)      { return fb + fa; }
REAL64 db_sub(REAL64 fb, REAL64 fa)      { return fb - fa; }
REAL64 db_mul(REAL64 fb, REAL64 fa)      { return fb * fa; }
REAL64 db_div(REAL64 fb, REAL64 fa)      { return fb / fa; }
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
REAL32 sn_div (REAL32 fb, REAL32 fa)     { return fb / fa; }
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

        return fa;
}
REAL64 fp_sqrtlastdb (REAL64 fa)         { return db_unary (fa, db_sqrt); }
REAL64 fp_remfirstdb (REAL64 fb, REAL64 fa)
{
        REAL64 result;

        result = db_binary (fb, fa, db_remfirst);
        fp_pushdb (fp_divdb (fp_subdb (fb, result), fa));
        return result;
}
int    fp_gtdb (REAL64 fb, REAL64 fa)    { return db_binary2word (fb, fa, db_gt); }
int    fp_eqdb (REAL64 fb, REAL64 fa)    { return db_binary2word (fb, fa, db_eq); }
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
        AargDB = fp; ResultSN = Real32Undefined_p;
#endif

        if (fp_nandb (fp))
        {
                FP_Error = TRUE;
                return Real32ConversionNaN;
        }

        if (fp_infdb (fp))
                FP_Error = TRUE;

        result = (REAL32) fp;

#ifndef NDEBUG
        ResultSN = result;
#endif

        result = sn_check_except (result);
        return result;
}
REAL64 fp_intdb (REAL64 fp)
{
        REAL64 result;

#ifndef NDEBUG
        AargDB = fp; ResultDB = Real64Undefined_p;
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
        AargDB = fp; ResultDB = Real64Undefined_p;
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

        return (__INT32_MIN__ < result) && (result < __INT32_MAX__);
}
int fp_chki64db (REAL64 fp)
{
        long long result;

#ifndef NDEBUG
        AargDB = fp; ResultDB = Real64Undefined_p;
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

        if (exp > 1024) exp -= 2048;
        if ((exp < -126) || (exp > 127))
        {
                exp = 127;
                frac = 0;
        }
        exp = (exp + 128) & 0xff;;
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

        return fa;
}
REAL32 fp_sqrtlastsn (REAL32 fa)         { return sn_unary (fa, sn_sqrt); }
REAL32 fp_remfirstsn (REAL32 fb, REAL32 fa)
{
        REAL32 result;

        result = sn_binary (fb, fa, sn_remfirst);
        fp_pushsn (fp_divsn (fp_subsn (fb, result), fa));
        return result;
}
int    fp_gtsn (REAL32 fb, REAL32 fa)    { return sn_binary2word (fb, fa, sn_gt); }
int    fp_eqsn (REAL32 fb, REAL32 fa)    { return sn_binary2word (fb, fa, sn_eq); }
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
        AargDB = fp; ResultDB = Real64Undefined_p;
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
        AargSN = fp; ResultSN = Real32Undefined_p;
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
        AargSN = fp; ResultSN = Real32Undefined_p;
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
        return (__INT32_MIN__ < result) && (result < __INT32_MAX__);
}
int fp_chki64sn (REAL32 fp)
{
        long long result;

#ifndef NDEBUG
        AargSN = fp; ResultSN = Real32Undefined_p;
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
