#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <fenv.h>
#pragma STDC FENV_ACCESS ON

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

int main(int argc, char*argv[])
{
        double fa, fb;
        long b;
        int rc, fpexc;


        fa = atof (argv[1]);
        b = atol (argv[2]);
        if (b == 0) fb = FP_NAN; else fb = (double) b;
        rc = fa < fb;
        printf ("rc = %d\n", rc);
        fpexc = fetestexcept (FE_ALL_EXCEPT);
        decode_except (fpexc);
        return 0;
}
