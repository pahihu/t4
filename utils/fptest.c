#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fparithmetic.h"

#include <fenv.h>
#pragma STDC FENV_ACCESS ON

#define onoff(x)        (x ? "on" : "off")

uint32_t FP_Error;

#define FALSE   0
#define TRUE    1

extern void db_dump(char*, fpreal64_t);
extern void sn_dump(char*, fpreal32_t);

#define __INT32_MIN__   (-__INT32_MAX__-1)
#define __INT64_MIN__   (-__INT64_MAX__-1)


int main(int argc, char*argv[])
{
        fpreal32_t ra, rb, rresult;
        fpreal64_t da, db, dresult;
        long b;
        double  one =  1.0, tiny = 1.0e-300, mone = -1.0;

        // rb.bits = 0xc1e0000000000001ULL;
        // ra.bits = 0x41dfffffffffffffULL;
        rb.bits = 0xc0490fdbUL;
        ra.bits = 0xbfb504f3UL;
        rb.fp = __INT32_MAX__;
        ra.fp = __INT32_MIN__;
        rb.fp = __INT64_MAX__;
        ra.fp = __INT64_MIN__;
/*
        rb.bits = 0x4f000001UL;
        ra.bits = 0xcf000001UL;
        rb.bits = 0x50d55531UL;
        ra.bits = 0xd0d55531UL;
*/

        rb.bits = 0x00800000UL;
/*
        // db.bits = 0x3ff0000000000000ULL;
        // da.bits = 0x4000000000000000ULL;
        db.fp = one;
        da.fp = tiny;

        fp_init ();
        FP_Error = FALSE;
        db_dump ("db", db);
        db_dump ("da", da);

        fp_setrounding ("fptest", ROUND_N);
        dresult.fp = db.fp - tiny;
        db_dump ("N one-tiny", dresult);

        fp_setrounding ("fptest", ROUND_M);
        dresult.fp = db.fp - tiny;
        db_dump ("M one-tiny", dresult);

        fp_setrounding ("fptest", ROUND_P);
        dresult.fp = db.fp - tiny;
        db_dump ("P one-tiny", dresult);

        fp_setrounding ("fptest", ROUND_Z);
        dresult.fp = db.fp - tiny;
        db_dump ("Z one-tiny", dresult);

        db.fp = mone;

        fp_setrounding ("fptest", ROUND_N);
        dresult.fp = db.fp + tiny;
        db_dump ("N mone+tiny", dresult);

        fp_setrounding ("fptest", ROUND_M);
        dresult.fp = db.fp + tiny;
        db_dump ("M mone+tiny", dresult);

        fp_setrounding ("fptest", ROUND_P);
        dresult.fp = db.fp + tiny;
        db_dump ("P mone+tiny", dresult);

        fp_setrounding ("fptest", ROUND_Z);
        dresult.fp = db.fp + tiny;
        db_dump ("Z mone+tiny", dresult);
        printf ("FP_Error = %d\n", FP_Error);
        return 0;
*/

        fp_init ();
        FP_Error = FALSE;
        db.bits = 0x0010000000000001;

        db_dump ("db", db);
        fp_setrounding ("fptest", ROUND_M);
        dresult.fp = sqrt (db.fp);
        db_dump ("dresult", dresult);

        dresult.fp = t4_fpsqrt64 (db.fp);
        db_dump ("dresult", dresult);

        printf ("FP_Error = %d\n", FP_Error);
        return 0;

        FP_Error = FALSE;
        sn_dump ("ra", ra);
        rresult.fp = truncf (ra.fp);
        sn_dump ("rresult", rresult);
        printf ("FP_Error = %d\n", FP_Error);
        return 0;
}
