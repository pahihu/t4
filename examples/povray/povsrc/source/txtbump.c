/****************************************************************************
*                txtbump.c
*
*  This module implements solid texturing functions that perturb the surface
*  normal to create a bumpy effect. 
*  
*  from Persistence of Vision Raytracer 
*  Copyright 1992 Persistence of Vision Team
*---------------------------------------------------------------------------
*  Copying, distribution and legal info is in the file povlegal.doc which
*  should be distributed with this file. If povlegal.doc is not available
*  or for more info please contact:
*
*       Drew Wells [POV-Team Leader] 
*       CIS: 73767,1244  Internet: 73767.1244@compuserve.com
*       Phone: (213) 254-4041
* 
* This program is based on the popular DKB raytracer version 2.12.
* DKBTrace was originally written by David K. Buck.
* DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
*
*****************************************************************************/
/*
   Some texture ideas garnered from SIGGRAPH '85 Volume 19 Number 3, 
   "An Image Synthesizer" By Ken Perlin.
   Further Ideas Garnered from "The RenderMan Companion" (Addison Wesley)
*/

#include "frame.h"
#include "vector.h"
#include "povproto.h"
#include "texture.h"

extern unsigned short crctab[256];

void ripples (x, y, z, Texture, normal)
DBL x, y, z;
TEXTURE *Texture;
VECTOR *normal;
{
   register int i;
   VECTOR point;
   register DBL length, scalar, index;

   if (Options & DEBUGGING)
      printf ("ripples %g %g %g", x, y, z);

   for (i = 0 ; i < NUMBER_OF_WAVES ; i++) {
      point.x = x;
      point.y = y;
      point.z = z;
      VSub (point, point, Wave_Sources[i]);
      VDot (length, point, point);
      if (length == 0.0)
         length = 1.0;

      length = sqrt(length);
      index = length*Texture->Frequency
      + Texture -> Phase;
      scalar = cycloidal (index) * Texture -> Bump_Amount;

      if (Options & DEBUGGING)
         printf (" index %g scalar %g length %g\n", index, scalar, length);

      VScale (point, point, scalar/length/(DBL)NUMBER_OF_WAVES);
      VAdd (*normal, *normal, point);
   }
   VNormalize (*normal, *normal);
}

void waves (x, y, z, Texture, normal)
DBL x, y, z;
TEXTURE *Texture;
VECTOR *normal;
{
   register int i;
   VECTOR point;
   register DBL length, scalar, index, sinValue ;

   if (Options & DEBUGGING)
      printf ("waves %g %g %g\n", x, y, z);

   for (i = 0 ; i < NUMBER_OF_WAVES ; i++) {
      point.x = x;
      point.y = y;
      point.z = z;
      VSub (point, point, Wave_Sources[i]);
      VDot (length, point, point);
      if (length == 0.0)
         length = 1.0;

      length = sqrt(length);
      index = (length * Texture -> Frequency * frequency[i])
         + Texture -> Phase;
      sinValue = cycloidal (index);

      scalar =  sinValue * Texture -> Bump_Amount /
      frequency[i];
      VScale (point, point, scalar/length/(DBL)NUMBER_OF_WAVES);
      VAdd (*normal, *normal, point);
   }
   VNormalize (*normal, *normal);
}


void bumps (x, y, z, Texture, normal)
DBL x, y, z;
TEXTURE *Texture;
VECTOR *normal;
{
   VECTOR bump_turb;

   if (Texture -> Bump_Amount == 0.0)
      return;                            /* why are we here?? */

   if (Options & DEBUGGING)
      printf ("bumps %g %g %g\n", x, y, z);

   DNoise (&bump_turb, x, y, z);         /* Get Normal Displacement Val. */
   VScale(bump_turb, bump_turb, Texture->Bump_Amount);
   VAdd (*normal, *normal, bump_turb);   /* displace "normal" */
   VNormalize (*normal, *normal);        /* normalize normal! */
   return;
}

/*
   dents is similar to bumps, but uses noise() to control the amount of
   dnoise() perturbation of the object normal...
*/

void dents (x, y, z, Texture, normal)
DBL x, y, z;
TEXTURE *Texture;
VECTOR *normal;
{
   VECTOR stucco_turb;
   DBL noise;

   if (Texture -> Bump_Amount == 0.0)
      return;                           /* why are we here?? */

   noise = Noise (x, y, z);

   noise =  noise * noise * noise * Texture->Bump_Amount;

   if (Options & DEBUGGING)
      printf ("dents %g %g %g noise %g\n", x, y, z, noise);

   DNoise (&stucco_turb, x, y, z);       /* Get Normal Displacement Val. */

   VScale (stucco_turb, stucco_turb, noise);
   VAdd (*normal, *normal, stucco_turb); /* displace "normal" */
   VNormalize (*normal, *normal);        /* normalize normal! */
   return;
}




/*
   Ideas garnered from the April 89 Byte Graphics Supplement on RenderMan,
   refined from "The RenderMan Companion, by Steve Upstill of Pixar, (C) 1990
   Addison-Wesley.
*/


/*
   wrinkles - This is my implementation of the dented() routine, using
   a surface iterative fractal derived from DTurbulence.  This is a 3-D vers.
   (thanks to DNoise()...) of the usual version using the singular Noise()...
   Seems to look a lot like wrinkles, however... (hmmm)
*/

void wrinkles (x, y, z, Texture, normal)
DBL x, y, z;
TEXTURE *Texture;
VECTOR *normal;
{
   register int i;
   register DBL scale = 1.0;
   VECTOR result, value;

   if (Texture -> Bump_Amount == 0.0)
      return;                                /* why are we here?? */

   if (Options & DEBUGGING)
      printf ("wrinkles %g %g %g\n", x, y, z);

   result.x = 0.0;
   result.y = 0.0;
   result.z = 0.0;

   for (i = 0; i < 10 ; scale *= 2.0, i++)
   {
      DNoise(&value, x * scale, y * scale, z * scale);   /* * scale,*/
      result.x += FABS (value.x / scale);
      result.y += FABS (value.y / scale);
      result.z += FABS (value.z / scale);
   }

   VScale(result, result, Texture->Bump_Amount);
   VAdd (*normal, *normal, result);             /* displace "normal" */
   VNormalize (*normal, *normal);               /* normalize normal! */
   return;
}
