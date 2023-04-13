/* Test file to create a Targa height field for POV-Ray -- Alexander Enzmann */
/* This is a sombrero */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "hftarga.c"

#define M_PI 3.1415926535897932384626

void
main()
{
   float x, y, z;
   float x0, z0;
   float b_const, phase;
   unsigned char r, g, b;

   /* Open the ouput file */
   TargaOpen("hfsomb.tga", 160, 160);

   /* Adjust the characteristics of the surface */
   b_const = 1.0;
   phase = 0.0;

   /* Compute the height of the surface over a square area (80x80) */
   for (x0=-40.0;x0<40.0;x0+=0.5) {
      for (z0=-40.0;z0<40.0;z0+=0.5) {
	 /* Want a total of 4 ripples from the center to the edge */
	 x = x0 / 10.0;
	 z = z0 / 10.0;

	 /* The height field ranges from 0->256, so use an offset of
	    128 for the zero level of the function */
	 y = 128;

	 /* The range of the function is from -1 -> 1, so to make it fit
	    the height field box a scale of 127 is used. */
	 y += 127.0 * cos(2.0 * M_PI * sqrt(x * x + z * z) + phase) *
		      exp(-b_const * sqrt(x * x + z * z));

	 /* Determine the values to write to the Targa file */
	 r = y;
	 g = (y - (float)r) * 256.0;
	 b = 0;
	 TargaWrite(r, g, b);
	 }
      }

   /* Close em up and get outta here */
   TargaClose();
}
