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
#include "fparithmetic.h"

#pragma STDC FENV_ACCESS ON

#define FE_T800_EXCEPT  (FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW)

#define REAL64_SIGN     0x8000000000000000LL
#define REAL64_EXP      0x7FF0000000000000LL
#define REAL64_FRAC_MSB 0x0010000000000000LL
#define REAL64_FRAC     0x000FFFFFFFFFFFFFLL

#define INT64(x)        ((int64_t)(x))
#define INT32(x)        ((int32_t)(x))

#define REAL32_SIGN     0x80000000L
#define REAL32_EXP      0x7F800000L
#define REAL32_FRAC     0x007FFFFFL
#define REAL32_FRAC_MSB 0x00800000L

#undef TRUE
#undef FALSE
#define FALSE   0x0000
#define TRUE    0x0001

extern int FP_Error;
extern int RoundingMode;
extern void handler (int);

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
                        handler (-1);
        }
        
        RoundingMode = mode;
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
        return (REAL32_EXP & x.bits) >> 52;
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
        printf ("%s: fp = %lf (#%016llX, sign=%d,exp=%d,frac=%llX)\n",
                msg, fp, x.bits,
                fp_signdb(fp) ? 1 : 0,
                fp_expdb(fp),
                fp_fracdb(fp));
        printf ("FP_Error    %s\n", FP_Error ? "Set" : "Clear");
}

void sn_dump (char *msg, REAL32 fp)
{
        fpreal32_t x;

        x.fp = fp;
        printf ("%s: fp = %f (#%08X, sign=%d,exp=%d,frac=%X)\n",
                msg, fp, x.bits,
                fp_signsn(fp) ? 1 : 0,
                fp_expsn(fp),
                fp_fracsn(fp));
        printf ("FP_Error    %s\n", FP_Error ? "Set" : "Clear");
}

#define onoff(x)        (x ? "on" : "off")

void decode_except (int expc)
{
        printf ("fpexcept       = %d\n", expc);
        printf ("FE_INVALID     %s\n", onoff (expc & FE_INVALID));
        printf ("FE_DIVBYZERO   %s\n", onoff (expc & FE_DIVBYZERO));
        printf ("FE_OVERFLOW    %s\n", onoff (expc & FE_OVERFLOW));
        printf ("FE_UNDERFLOW   %s\n", onoff (expc & FE_UNDERFLOW));
        printf ("FE_INEXACT     %s\n", onoff (expc & FE_INEXACT));
}

/* Do a binary REAL64 operation, return REAL64 result. */
REAL64 db_binary (REAL64 fb, REAL64 fa, REAL64 (*opr)(REAL64, REAL64))
{
        REAL64 result;
        uint64_t fracb, fraca;

        int exc;

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

        exc = fetestexcept (FE_T800_EXCEPT);
        if (exc)
        {
                decode_except (exc);
                FP_Error = TRUE;
        }
        return result;
}

/* Do a binary operation on two REAL64 numbers, return the result flag. */
int db_binary2word (REAL64 fb, REAL64 fa, int (*opr)(REAL64, REAL64))
{
        int result;

        int exc;

        if (fp_notfinitedb (fb) || fp_notfinitedb (fa))
        {
                FP_Error = TRUE;
        }

        result = opr(fb, fa);

        exc = fetestexcept (FE_T800_EXCEPT);
        if (exc)
        {
                decode_except (exc);
                FP_Error = TRUE;
        }
        return result;
}

/* Do an unary REAL64 operation, return REAL64 result. */
REAL64 db_unary (REAL64 fa, REAL64 (*opr)(REAL64))
{
        REAL64 result;
        int exc;

        if (fp_nandb (fa))
                return fa;

        if (fp_infdb (fa))
                FP_Error = TRUE;

        result = opr(fa);

        exc = fetestexcept (FE_T800_EXCEPT);
        if (exc)
        {
                decode_except (exc);
                FP_Error = TRUE;
        }
        return result;
}

/* Do a binary REAL64 operation, return REAL64 result. */
REAL32 sn_binary (REAL32 fb, REAL32 fa, REAL32 (*opr)(REAL32, REAL32))
{
        REAL32 result;
        uint32_t fracb, fraca;

        int exc;

        if (fp_nansn (fb) && fp_nansn (fb))
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

        if (fp_infsn (fa) || fp_infsn (fb))
                FP_Error = TRUE;

        result = opr(fb, fa);

        exc = fetestexcept (FE_T800_EXCEPT);
        if (exc)
        {
                decode_except (exc);
                FP_Error = TRUE;
        }
        return result;
}

/* Do a binary operation on two REAL32 numbers, return the result flag. */
int sn_binary2word (REAL32 fb, REAL32 fa, int (*opr)(REAL32, REAL32))
{
        int result;

        int exc;

        if (fp_notfinitesn (fb) || fp_notfinitesn (fa))
        {
                FP_Error = TRUE;
        }

        result = opr(fb, fa);

        exc = fetestexcept (FE_T800_EXCEPT);
        if (exc)
        {
                decode_except (exc);
                FP_Error = TRUE;
        }
        return result;
}

/* Do an unary REAL32 operation, return REAL32 result. */
REAL32 sn_unary (REAL32 fa, REAL32 (*opr)(REAL32))
{
        REAL32 result;
        int exc;

        if (fp_nandb (fa))
                return fa;

        if (fp_infdb (fa))
                FP_Error = TRUE;

        result = opr(fa);

        exc = fetestexcept (FE_T800_EXCEPT);
        if (exc)
        {
                decode_except (exc);
                FP_Error = TRUE;
        }
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
REAL64 db_sqrt (REAL64 fa)               { return  sqrt (fa); }
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
        return fabs (fa);
}
REAL64 fp_sqrtdb (REAL64 fa)             { return db_unary (fa, db_sqrt); }
REAL64 fp_remfirstdb (REAL64 fb, REAL64 fa) { return db_binary (fb, fa, db_remfirst); }
int    fp_gtdb (REAL64 fb, REAL64 fa)    { return db_binary2word (fb, fa, db_gt); }
int    fp_eqdb (REAL64 fb, REAL64 fa)    { return db_binary2word (fb, fa, db_eq); }
int    fp_ordereddb (REAL64 fb, REAL64 fa)
{
        if (fp_nandb (fb) || fp_nandb (fa))
                return FALSE;
        return TRUE;
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
        return fabsf (fa);
}
REAL32 fp_sqrtsn (REAL32 fa)             { return sn_unary (fa, sn_sqrt); }
REAL32 fp_remfirstsn (REAL32 fb, REAL32 fa) { return sn_binary (fb, fa, sn_remfirst); }
int    fp_gtsn (REAL32 fb, REAL32 fa)    { return sn_binary2word (fb, fa, sn_gt); }
int    fp_eqsn (REAL32 fb, REAL32 fa)    { return sn_binary2word (fb, fa, sn_eq); }
int    fp_orderedsn (REAL32 fb, REAL32 fa)
{
        if (fp_nansn (fb) || fp_nansn (fa))
                return FALSE;
        return TRUE;
}

