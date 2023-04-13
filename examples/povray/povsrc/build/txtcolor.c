/****************************************************************************
*                txtcolor.c
*
*  This module implements solid texturing functions that modify the color
*  transparency of an object's surface.
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
   Further Ideas Garnered from "The RenderMan Companion" (Addison Wesley).
*/

#include "frame.h"
#include "vector.h"
#include "povproto.h"
#include "texture.h"


void agate (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   register DBL noise, hue;
   COLOUR New_Colour;

   noise = cycloidal(1.3 * Turbulence(x, y, z, Texture->Octaves) + 1.1 * z) + 1;
   noise *= 0.5;
   noise = pow(noise, 0.77);

   if (Options & DEBUGGING)
      printf ("agate %g %g %g noise %g\n", x, y, z, noise);

   if (Texture -> Colour_Map != NULL)
   {
      Compute_Colour (&New_Colour, Texture->Colour_Map, noise);
      colour -> Red += New_Colour.Red;
      colour -> Green += New_Colour.Green;
      colour -> Blue += New_Colour.Blue;
      colour -> Alpha += New_Colour.Alpha;
      return;
   }

   hue = 1.0 - noise;

   if (noise < 0.5)
   {
      colour -> Red += (1.0 - (noise / 10));
      colour -> Green += (1.0 - (noise / 5));
      colour -> Blue += hue;
   }
   else if (noise < 0.6)
   {
      colour -> Red += 0.9;
      colour -> Green += 0.7;
      colour -> Blue += hue;
   }
   else
   {
      colour -> Red += (0.6 + hue);
      colour -> Green += (0.3 + hue);
      colour -> Blue += hue;
   }
   return;
}


void bozo (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   register DBL noise, turb;
   COLOUR New_Colour;
   VECTOR BozoTurbulence;


   if (Options & DEBUGGING)
      printf ("bozo %g %g %g ", x, y, z);

   if ((turb = Texture->Turbulence) != 0.0)
   {
      DTurbulence (&BozoTurbulence, x, y, z, Texture->Octaves);
      x += BozoTurbulence.x * turb;
      y += BozoTurbulence.y * turb;
      z += BozoTurbulence.z * turb;
   }

   noise = Noise (x, y, z);

   if (Options & DEBUGGING)
      printf ("noise %g\n", noise);

   if (Texture -> Colour_Map != NULL) {
      Compute_Colour (&New_Colour, Texture->Colour_Map, noise);
      colour -> Red += New_Colour.Red;
      colour -> Green += New_Colour.Green;
      colour -> Blue += New_Colour.Blue;
      colour -> Alpha += New_Colour.Alpha;
      return ;
   }

   if (noise < 0.4) {
      colour -> Red += 1.0;
      colour -> Green += 1.0;
      colour -> Blue += 1.0;
      return ;
   }

   if (noise < 0.6) {
      colour -> Green += 1.0;
      return ;
   }

   if (noise < 0.8) {
      colour -> Blue += 1.0;
      return ;
   }

   colour -> Red += 1.0;
   return ;
}


void brick (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   DBL xr, yr, zr;
   /*   int brkindx; */

   /*   brkindx = (int)(FLOOR(x) + FLOOR(y) + FLOOR(z));  */

   /*   if (brkindx & 2) {                                */
   xr = fabs(fmod(x, 1.0));
   yr = fabs(fmod(y, 1.0));
   zr = fabs(fmod(z, 1.0));
   /*     }
   else {
     xr = fabs(fmod(x+0, 1.0));
     yr = fabs(fmod(y+0, 1.0));
     zr = fabs(fmod(z+0, 1.0));
   }
*/
   *colour = *Texture -> Colour2;

   if (Options & DEBUGGING)
      printf ("brick %g %g %g\n", x, y, z);

   if (xr > 0 && xr < Texture -> Mortar) {
      *colour = *Texture -> Colour1;
      return;
   }
   if (yr > 0 && yr < Texture -> Mortar) {
      *colour = *Texture -> Colour1;
      return;
   }
   if (zr > 0 && zr < Texture -> Mortar)
      *colour = *Texture -> Colour1;

   return;

}



void checker (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   int brkindx;

   x += Small_Tolerance;  /* add a small offset to x, y, z, axes to prevent noise */
   y += Small_Tolerance;
   z += Small_Tolerance;

   /* AAC: was just x + z */
   /* AAC: Small_Tolerance added to get around Microsoft C (int) bug */
   brkindx = (int)(FLOOR(x) + FLOOR(y) + FLOOR(z)); 

   if (Options & DEBUGGING)
      printf ("checker %g %g %g\n", x, y, z);

   if (brkindx & 1)
      *colour = *Texture -> Colour1;
   else
      *colour = *Texture -> Colour2;
   return;
}



void checker_texture (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   int brkindx;
   VECTOR Point;

   x += Small_Tolerance;  /* add a small offset to x, y, z, axes to prevent noise */
   y += Small_Tolerance;
   z += Small_Tolerance;

   brkindx = (int)(FLOOR(x) + FLOOR(y) + FLOOR(z));

   if (Options & DEBUGGING)
      printf ("checker_texture %g %g %g\n", x, y, z);

   Make_Vector (&Point, x, y, z);

   if (brkindx & 1)
      Colour_At (colour, ((TEXTURE *) Texture -> Colour1), &Point);
   else
      Colour_At (colour, ((TEXTURE *) Texture -> Colour2), &Point);
   return;
}

/*
   Color Gradient Texture - gradient based on the fractional values of x, y or
   z, based on whether or not the given directional vector is a 1.0 or a 0.0.
   Note - ONLY works with colour maps, preferably one that is circular - i.e.
   the last defined colour (value 1.001) is the same as the first colour (with
   a value of 0.0) in the map.  The basic concept of this is from DBW Render,
   but Dave Wecker's only supports simple Y axis gradients.
*/

void gradient (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   COLOUR New_Colour;
   DBL value = 0.0, turb;
   VECTOR GradTurbulence;

   if ((turb = Texture->Turbulence) != 0.0)
   {
      DTurbulence (&GradTurbulence, x, y, z, Texture->Octaves);
      x += GradTurbulence.x * turb;
      y += GradTurbulence.y * turb;
      z += GradTurbulence.z * turb;
   }

   if (Texture -> Colour_Map == NULL)
      return;
   if (Texture -> Texture_Gradient.x != 0.0)
   {
      x = FABS(x);
      value += x - FLOOR(x);	/* obtain fractional X component */
   }
   if (Texture -> Texture_Gradient.y != 0.0)
   {
      y = FABS(y);
      value += y - FLOOR(y);	/* obtain fractional Y component */
   }
   if (Texture -> Texture_Gradient.z != 0.0)
   {
      z = FABS(z);
      value += z - FLOOR(z);	/* obtain fractional Z component */
   }
   value = ((value > 1.0) ? fmod(value, 1.0) : value); /* clamp to 1.0 */

   if (Options & DEBUGGING)
      printf ("gradient %g %g %g value %g\n", x, y, z, value);

   Compute_Colour(&New_Colour, Texture->Colour_Map, value);
   colour -> Red += New_Colour.Red;
   colour -> Green += New_Colour.Green;
   colour -> Blue += New_Colour.Blue;
   colour -> Alpha += New_Colour.Alpha;
   return;
}


/*
   Granite - kind of a union of the "spotted" and the "dented" textures,
   using a 1/f fractal noise function for color values.  Typically used
   w/ small scaling values.  Should work with colour maps for pink granite...
*/


void granite (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   register int i;
   register DBL temp, noise = 0.0, freq = 1.0;
   COLOUR New_Colour;

   for (i = 0; i < 6 ; freq *= 2.0, i++)
   {
      temp = 0.5 - Noise (x * 4 * freq, y * 4 * freq, z * 4 * freq);
      temp = FABS(temp);
      noise += temp / freq;
   }

   if (Options & DEBUGGING)
      printf ("granite %g %g %g noise %g\n", x, y, z, noise);

   if (Texture -> Colour_Map != NULL)
   {
      Compute_Colour (&New_Colour, Texture->Colour_Map, noise);
      colour -> Red += New_Colour.Red;
      colour -> Green += New_Colour.Green;
      colour -> Blue += New_Colour.Blue;
      colour -> Alpha += New_Colour.Alpha;
      return;
   }

   colour -> Red += noise;                  /* "white (1.0) * noise" */
   colour -> Green += noise;
   colour -> Blue += noise;

   return;
}


void marble (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   register DBL noise, hue;
   COLOUR New_Colour;

   noise = Triangle_Wave(x + Turbulence(x, y, z, Texture->Octaves) * Texture -> Turbulence);

   if (Options & DEBUGGING)
      printf ("marble %g %g %g noise %g \n", x, y, z, noise);

   if (Texture -> Colour_Map != NULL)
   {
      Compute_Colour (&New_Colour, Texture->Colour_Map, noise);
      colour -> Red += New_Colour.Red;
      colour -> Green += New_Colour.Green;
      colour -> Blue += New_Colour.Blue;
      colour -> Alpha += New_Colour.Alpha;
      return ;
   }

   if (noise < 0.0)
   {
      colour -> Red += 0.9;
      colour -> Green += 0.8;
      colour -> Blue += 0.8;
   }
   else if (noise < 0.9)
   {
      colour -> Red += 0.9;
      hue = 0.8 - noise * 0.8;
      colour -> Green += hue;
      colour -> Blue += hue;
   }
   return ;
}

/*	
   With a little reflectivity and brilliance, can look like organ pipe
   metal.   With tiny scaling values can look like masonry or concrete.
   Works with color maps.
*/

void spotted (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   register DBL noise;
   COLOUR New_Colour;

   noise = Noise (x, y, z);

   if (Options & DEBUGGING)
      printf ("spotted %g %g %g\n", x, y, z);

   if (Texture -> Colour_Map != NULL)
   {
      Compute_Colour (&New_Colour, Texture->Colour_Map, noise);
      colour -> Red += New_Colour.Red;
      colour -> Green += New_Colour.Green;
      colour -> Blue += New_Colour.Blue;
      colour -> Alpha += New_Colour.Alpha;
      return;
   }

   colour -> Red += noise;             /* "white (1.0) * noise" */
   colour -> Green += noise;
   colour -> Blue += noise;

   return;
}





void wood (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   register DBL noise, length;
   VECTOR WoodTurbulence;
   VECTOR point;
   COLOUR New_Colour;

   DTurbulence (&WoodTurbulence, x, y, z, Texture->Octaves);

   if (Options & DEBUGGING)
      printf ("wood %g %g %g", x, y, z);

   point.x = cycloidal((x + WoodTurbulence.x)
      * Texture -> Turbulence);
   point.y = cycloidal((y + WoodTurbulence.y)
      * Texture -> Turbulence);
   point.z = 0.0;

   point.x += x;
   point.y += y;

   /*  point.z += z;       Deleted per David Buck --  BP 7/91 */

   VLength (length, point);

   noise = Triangle_Wave(length);

   if (Options & DEBUGGING)
      printf ("noise %g\n", noise);

   if (Texture -> Colour_Map != NULL) {
      Compute_Colour (&New_Colour, Texture->Colour_Map, noise);
      colour -> Red += New_Colour.Red;
      colour -> Green += New_Colour.Green;
      colour -> Blue += New_Colour.Blue;
      colour -> Alpha += New_Colour.Alpha;
      return ;
   }

   if (noise > 0.6) {
      colour -> Red += 0.4;
      colour -> Green += 0.133;
      colour -> Blue += 0.066;
   }
   else {
      colour -> Red += 0.666;
      colour -> Green += 0.312;
      colour -> Blue += 0.2;
   }
   return ;
}

   /* Two new textures by Scott Taylor LEOPARD & ONION */

   void leopard (x, y, z, Texture, colour)      /* SWT 7/18/91 */
      DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   /* The variable noise is not used as noise in this function */
   register DBL noise, turb, temp1,temp2,temp3;
   COLOUR New_Colour;
   VECTOR LeopardTurbulence;


   if (Options & DEBUGGING)
      printf ("leopard %g %g %g ", x, y, z);

   if ((turb = Texture->Turbulence) != 0.0)
   {
      DTurbulence (&LeopardTurbulence, x, y, z, Texture->Octaves);
      x += LeopardTurbulence.x * turb;
      y += LeopardTurbulence.y * turb;
      z += LeopardTurbulence.z * turb;
   }
   /* This form didn't work with Zortech 386 compiler */
   /* noise = Sqr((sin(x)+sin(y)+sin(z))/3); */
   /* So we break it down. */
   temp1 = sin(x);
   temp2 = sin(y);
   temp3 = sin(z);
   noise = Sqr((temp1+temp2+temp3)/3);

   if (Options & DEBUGGING)
      printf ("temp123 %g %g %g  ", temp1, temp2, temp3);


   if (Options & DEBUGGING)
      printf ("noise %g\n", noise);

   if (Texture -> Colour_Map != NULL) {
      Compute_Colour (&New_Colour, Texture->Colour_Map, noise);
      colour -> Red += New_Colour.Red;
      colour -> Green += New_Colour.Green;
      colour -> Blue += New_Colour.Blue;
      colour -> Alpha += New_Colour.Alpha;
      return;
   }

   colour -> Red += noise;
   colour -> Green += noise;
   colour -> Blue += noise;
   return;
}

void onion (x, y, z, Texture, colour)      /* SWT 7/18/91 */
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   /* The variable noise is not used as noise in this function */
   register DBL noise, turb;
   COLOUR New_Colour;
   VECTOR OnionTurbulence;


   if (Options & DEBUGGING)
      printf ("onion %g %g %g ", x, y, z);

   if ((turb = Texture->Turbulence) != 0.0)
   {
      DTurbulence (&OnionTurbulence, x, y, z, Texture->Octaves);
      x += OnionTurbulence.x * turb;
      y += OnionTurbulence.y * turb;
      z += OnionTurbulence.z * turb;
   }

   /* This ramp goes 0-1,1-0,0-1,1-0...
   noise = (fmod(SQRT(Sqr(x)+Sqr(y)+Sqr(z)),2.0)-1.0);
   if (noise<0.0) {noise = 0.0-noise;}
   */

   /* This ramp goes 0-1,0-1,0-1,0-1... */
   noise = (fmod(SQRT(Sqr(x)+Sqr(y)+Sqr(z)),1.0));

   if (Options & DEBUGGING)
      printf ("noise %g\n", noise);

   if (Texture -> Colour_Map != NULL) {
      Compute_Colour (&New_Colour, Texture->Colour_Map, noise);
      colour -> Red += New_Colour.Red;
      colour -> Green += New_Colour.Green;
      colour -> Blue += New_Colour.Blue;
      colour -> Alpha += New_Colour.Alpha;
      return;
   }

   colour -> Red += noise;
   colour -> Green += noise;
   colour -> Blue += noise;
   return;
}
