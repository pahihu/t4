#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <fenv.h>
#pragma STDC FENV_ACCESS ON

#define onoff(x)        (x ? "on" : "off")

uint32_t FP_Error;

#define FALSE   0
#define TRUE    1

int main(int argc, char*argv[])
{
        REAL64 fa, fb, result;
        long b;

        FP_Error = FALSE;
        fb = atof (argv[1]);
        fa = atof (argv[2]);

        fp_dumpdb ("fb", fb);
        fp_dumpdb ("fa", fa);

        result = fp_adddb (fb, fa, fp_addop_db);
        fp_dumpdb ("b + a", result);
        result = fp_subdb (fb, fa, fp_subop_db);
        fp_dumpdb ("b - a", result);
        result = fp_muldb (fb, fa, fp_mulop_db);
        fp_dumpdb ("b * a", result);
        result = fp_divdb (fb, fa, fp_divop_db);
        fp_dumpdb ("b / a", result);

        return 0;
}
