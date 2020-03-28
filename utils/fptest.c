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
        fpreal32_t fa, fb, result;
        long b;

        // fb.bits = 0xc1e0000000000001ULL;
        // fa.bits = 0x41dfffffffffffffULL;
        fb.bits = 0xc0490fdbUL;
        fa.bits = 0xbfb504f3UL;
        fb.fp = __INT32_MAX__;
        fa.fp = __INT32_MIN__;
        fb.fp = __INT64_MAX__;
        fa.fp = __INT64_MIN__;
/*
        fb.bits = 0x4f000001UL;
        fa.bits = 0xcf000001UL;
        fb.bits = 0x50d55531UL;
        fa.bits = 0xd0d55531UL;
*/

        fb.bits = 0x00800000UL;

        fp_init ();
        FP_Error = FALSE;
        sn_dump ("fb", fb);
        result.fp = ldexpf (fb.fp, -1);
        sn_dump ("result", result);
        printf ("FP_Error = %d\n", FP_Error);
        return 0;

        FP_Error = FALSE;
        sn_dump ("fa", fa);
        result.fp = truncf (fa.fp);
        sn_dump ("result", result);
        printf ("FP_Error = %d\n", FP_Error);
        return 0;
}
