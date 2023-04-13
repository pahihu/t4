/****************************************************************************
*                txttest.c
*
*  This module implements "fill-in-the-blank" pre-programmed texture 
*  functions for easy modification and testing. Create new textures here.
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

/* Test new textures in the routines that follow */

/* The painted routines take an x,y,z point on an object and a pointer to the*/
/* object's texture description and return the color at that point           */
/* Similar routines are granite, agate, marble. See txtcolor.c for examples. */ 

void painted1 (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{

   /* Swirled()  */

   VECTOR colour_vector, result;
   register int i;
   register DBL scale = 1.0, temp;
   COLOUR New_Colour;


   if (Options & DEBUGGING)
      printf ("painted1 %g %g %g\n", x, y, z);

   result.x = 0.0;
   result.y = 0.0;
   result.z = 0.0;

   for (i = 0; i < 10 ; scale *= 2.0, i++)
   {
      DNoise(&colour_vector,x,y,z);
      temp = Noise (colour_vector.x * 4 * scale,
         colour_vector.y * 4 * scale,
         colour_vector.z * 4 * scale);
      temp = FABS(temp);
      result.x += temp/scale;
      result.y += temp/scale;
      result.z += temp/scale;
   }

   temp = result.x;
   if (Texture -> Colour_Map != NULL) {
      Compute_Colour (&New_Colour, Texture->Colour_Map, temp);
      colour -> Red += New_Colour.Red;
      colour -> Green += New_Colour.Green;
      colour -> Blue += New_Colour.Blue;
      colour -> Alpha += New_Colour.Alpha;
      return;
   }

   colour -> Red += temp;
   colour -> Green += temp;
   colour -> Blue += temp;
   return;

}

void painted2 (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   int brkindx;
   DBL turb;
   VECTOR TextureTurbulence;
   COLOUR Colour1, Colour2;

   /* You could change the parser to take two colors after PAINTED2,           */
   /* but since the colormap is already parsed it's easier to use it during    */
   /* testing. If the texture works out right you can change the parser later. */
   if (Texture -> Colour_Map != NULL)
   {
      Compute_Colour (&Colour1, Texture->Colour_Map, 0.1);
      Compute_Colour (&Colour2, Texture->Colour_Map, 0.9);
   }
   else
   {
      Make_Colour (&Colour1, 1.0, 1.0, 1.0);
      Colour1.Alpha = 0.0;
      Make_Colour (&Colour2, 0.0, 1.0, 0.0);
      Colour2.Alpha = 0.0;
   }


   if ((turb = Texture->Turbulence) != 0.0)
   {
      DTurbulence (&TextureTurbulence, x, y, z, Texture->Octaves);
      x += TextureTurbulence.x * turb;
      y += TextureTurbulence.y * turb;
      z += TextureTurbulence.z * turb;
   }

   brkindx = (int) FLOOR(x) + (int) FLOOR(z);

   if (Options & DEBUGGING)
      printf ("checker %g %g %g\n", x, y, z);

   if (brkindx & 1){
      colour->Red = Colour1.Red;
      colour->Green = Colour1.Green;
      colour->Blue = Colour1.Blue;
      colour->Alpha = Colour1.Alpha;
   }
   else{
      colour->Red = Colour2.Red;
      colour->Green = Colour2.Green;
      colour->Blue = Colour2.Blue;
      colour->Alpha = Colour2.Alpha;
   }
   return;

   ;
}
void painted3 (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   /* YOUR NAME HERE */
   ;
}

/* The bumpy routines take a point on an object,  a pointer to the          */
/* object's texture description and the surface normal at that point and    */
/* return a peturb surface normal to create the illusion that the surface   */
/* has been displaced.                                                      */
/* Similar routines are ripples, dents, bumps. See txtbump.c for examples.  */ 

void bumpy1 (x, y, z, Texture, normal)
DBL x, y, z;
TEXTURE *Texture;
VECTOR *normal;
{
}

/* Dan Farmer */
/* Same as bumpy1 except use VAdd for both cases of brkindex */
void bumpy2 (x, y, z, Texture, normal)
DBL x, y, z;
TEXTURE *Texture;
VECTOR *normal;
{
}

/* Dan Farmer */
/* Same as bumpy2 except scale AFTER setting brkindex */
void bumpy3 (x, y, z, Texture, normal)
DBL x, y, z;
TEXTURE *Texture;
VECTOR *normal;
{
}
