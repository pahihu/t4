#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define REAL    double
#define TAN     tan
#define ATN     atan
#define EXP     exp
#define LOG     log
#define SQR     sqrt

int main(int argc, char*argv[])
{
  REAL a, b, c, d, e, f;
  REAL diffa;
  int i;

  printf ("Savage benchmark...\n");

  a = (REAL) 1.0;
  for (i = 1; i <= 2499; i++)
    a = TAN(ATN(EXP(LOG(SQR(a*a))))) + (REAL) 1.0;

  printf ("   a = %.16lf\n", a);
  diffa = (REAL) 2500.0 - a;
  printf ("diff = %.16e\n", diffa);
  return 0;
} 
