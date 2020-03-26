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


int main(int argc, char*argv[])
{
        fpreal64_t fa, fb, result;
        long b;

        // fb.bits = 0xc1e0000000000001ULL;
        // fa.bits = 0x41dfffffffffffffULL;
        fb.bits = 0x41e0000000000000ULL;
        fa.bits = 0x41e0000000000001ULL;

        fp_init ();
        FP_Error = FALSE;
        db_dump ("fb", fb);
        fp_chki32db (fb);
        printf ("FP_Error = %d\n", FP_Error);

        FP_Error = FALSE;
        db_dump ("fa", fa);
        fp_chki32db (fa);
        printf ("FP_Error = %d\n", FP_Error);
        return 0;
}
