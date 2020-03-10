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

extern void db_dump (char *, REAL64);
extern void sn_dump (char *, REAL32);

int main(int argc, char*argv[])
{
        REAL32 fa, fb, result;
        long b;

        FP_Error = FALSE;
        fb = atof (argv[1]);
        fa = atof (argv[2]);

        sn_dump ("fb", fb);
        sn_dump ("fa", fa);

        result = fp_addsn (fb, fa);
        sn_dump ("b + a", result);
        result = fp_subsn (fb, fa);
        sn_dump ("b - a", result);
        result = fp_mulsn (fb, fa);
        sn_dump ("b * a", result);
        result = fp_divsn (fb, fa);
        sn_dump ("b / a", result);

        return 0;
}
