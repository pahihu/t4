/****************************************************************************
*                texture.c
*
*  This module implements texturing functions such as noise, turbulence and
*  texture transformation functions. The actual texture routines are in the
*  files txtcolor.c, txtbump.c, txtmap.c, etc.
*  The noise function used here is the one described by Ken Perlin in
*  "Hypertexture", SIGGRAPH '89 Conference Proceedings page 253.
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

DBL *sintab;
DBL frequency[NUMBER_OF_WAVES];
VECTOR Wave_Sources[NUMBER_OF_WAVES];
DBL *RTable;
short *hashTable;

unsigned short crctab[256] =
{
   0x0000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
   0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
   0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
   0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
   0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
   0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
   0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
   0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
   0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
   0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
   0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
   0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
   0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
   0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
   0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
   0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
   0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
   0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
   0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
   0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
   0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
   0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
   0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
   0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
   0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
   0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
   0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
   0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
   0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
   0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
   0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
   0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040
};



void Compute_Colour (Colour, Colour_Map, value)
COLOUR *Colour;
COLOUR_MAP *Colour_Map;
DBL value;
{
   register int i;
   COLOUR_MAP_ENTRY *Ent;
   register DBL fraction;

   if (value > 1.0)
      value = 1.0;

   if (value < 0.0)
      value = 0.0;

   for (i = 0, Ent = &(Colour_Map->Colour_Map_Entries[0]) ; i < Colour_Map -> Number_Of_Entries ; i++, Ent++)
      if ((value >= Ent->start) && (value <= Ent->end))
      {
         fraction = (value - Ent->start) / (Ent->end - Ent->start);
         Colour -> Red = Ent->Start_Colour.Red + fraction * (Ent->End_Colour.Red - Ent->Start_Colour.Red);
         Colour -> Green = Ent->Start_Colour.Green + fraction * (Ent->End_Colour.Green - Ent->Start_Colour.Green);
         Colour -> Blue = Ent->Start_Colour.Blue + fraction * (Ent->End_Colour.Blue - Ent->Start_Colour.Blue);
         Colour -> Alpha = Ent->Start_Colour.Alpha + fraction * (Ent->End_Colour.Alpha - Ent->Start_Colour.Alpha);
         return;
      }

   Colour -> Red = 0.0;
   Colour -> Green = 0.0;
   Colour -> Blue = 0.0;
   Colour -> Alpha = 0.0;
   printf ("No colour for value: %g\n", value);
   return;
}

void Initialize_Noise ()
{
   register int i = 0;
   VECTOR point;

   InitRTable();

   if ((sintab = (DBL *)malloc(SINTABSIZE * sizeof(DBL))) == NULL) {
      printf("Cannot allocate memory for sine table\n");
      exit(1);
   }

   for (i = 0 ; i < SINTABSIZE ; i++)
      sintab[i] = sin(i/(DBL)SINTABSIZE * (3.14159265359 * 2.0));

   for (i = 0 ; i < NUMBER_OF_WAVES ; i++)
   {
      DNoise (&point, (DBL) i, 0.0, 0.0);
      VNormalize (Wave_Sources[i], point);
      frequency[i] = (rand() & RNDMASK) / RNDDIVISOR + 0.01;
   }
}

void InitTextureTable()
{
   int i, j, temp;

   srand(0);

   if ((hashTable = (short int *) malloc(4096*sizeof(short int))) == NULL) {
      printf("Cannot allocate memory for hash table\n");
      exit(1);
   }
   for (i = 0; i < 4096; i++)
      hashTable[i] = i;
   for (i = 4095; i >= 0; i--) {
      j = rand() % 4096;
      temp = hashTable[i];
      hashTable[i] = hashTable[j];
      hashTable[j] = temp;
   }
}


/* modified by AAC to work properly with little bitty integers (16 bits) */

void InitRTable()
{
   int i;
   VECTOR rp;

   InitTextureTable();

   if ((RTable = (DBL *)malloc(MAXSIZE * sizeof(DBL))) == NULL) {
      printf("Cannot allocate memory for RTable\n");
      exit(1);
   }

   for (i = 0; i < MAXSIZE; i++)
   {
      rp.x = rp.y = rp.z = (DBL)i;
      RTable[i] = (unsigned int) R(&rp) * REALSCALE - 1.0;
   }
}


int R(v)
VECTOR *v;
{
   v->x *= .12345;
   v->y *= .12345;
   v->z *= .12345;

   return (Crc16((char *) v, sizeof(VECTOR)));
}

/*
 * Note that passing a VECTOR array to Crc16 and interpreting it as
 * an array of chars means that machines with different floating-point
 * representation schemes will evaluate Noise(point) differently.
 */

int Crc16(buf, count)
register char *buf;
register int  count;
{
   register unsigned short crc = 0;

   while (count--)
      crc = (crc >> 8) ^ crctab[ (unsigned char) (crc ^ *buf++) ];

   return ((int) crc);
}


/*
	Robert's Skinner's Perlin-style "Noise" function - modified by AAC
	to ensure uniformly distributed clamped values between 0 and 1.0...
*/

void setup_lattice(x, y, z, ix, iy, iz, jx, jy, jz, sx, sy, sz, tx, ty, tz)
DBL *x, *y, *z, *sx, *sy, *sz, *tx, *ty, *tz;
long *ix, *iy, *iz, *jx, *jy, *jz;
{
   /* ensures the values are positive. */
   *x -= MINX;
   *y -= MINY;
   *z -= MINZ;

   /* its equivalent integer lattice point. */
   *ix = (long)*x; *iy = (long)*y; *iz = (long)*z;
   *jx = *ix + 1; *jy = *iy + 1; *jz = *iz + 1;

   *sx = SCURVE(*x - *ix); *sy = SCURVE(*y - *iy); *sz = SCURVE(*z - *iz);

   /* the complement values of sx,sy,sz */
   *tx = 1.0 - *sx; *ty = 1.0 - *sy; *tz = 1.0 - *sz;
   return;
}


DBL Noise(x, y, z)
DBL x, y, z;
{
   long ix, iy, iz, jx, jy, jz;
   DBL sx, sy, sz, tx, ty, tz;
   DBL sum;
   short m;


   Calls_To_Noise++;

   setup_lattice(&x, &y, &z, &ix, &iy, &iz, &jx, &jy, &jz, &sx, &sy, &sz, &tx, &ty, &tz);

   /*
    *  interpolate!
    */
   m = Hash3d( ix, iy, iz ) & 0xFF;
   sum = INCRSUM(m,(tx*ty*tz),(x-ix),(y-iy),(z-iz));

   m = Hash3d( jx, iy, iz ) & 0xFF;
   sum += INCRSUM(m,(sx*ty*tz),(x-jx),(y-iy),(z-iz));

   m = Hash3d( ix, jy, iz ) & 0xFF;
   sum += INCRSUM(m,(tx*sy*tz),(x-ix),(y-jy),(z-iz));

   m = Hash3d( jx, jy, iz ) & 0xFF;
   sum += INCRSUM(m,(sx*sy*tz),(x-jx),(y-jy),(z-iz));

   m = Hash3d( ix, iy, jz ) & 0xFF;
   sum += INCRSUM(m,(tx*ty*sz),(x-ix),(y-iy),(z-jz));

   m = Hash3d( jx, iy, jz ) & 0xFF;
   sum += INCRSUM(m,(sx*ty*sz),(x-jx),(y-iy),(z-jz));

   m = Hash3d( ix, jy, jz ) & 0xFF;
   sum += INCRSUM(m,(tx*sy*sz),(x-ix),(y-jy),(z-jz));

   m = Hash3d( jx, jy, jz ) & 0xFF;
   sum += INCRSUM(m,(sx*sy*sz),(x-jx),(y-jy),(z-jz));

   sum = sum + 0.5;          /* range at this point -0.5 - 0.5... */

   if (sum < 0.0)
      sum = 0.0;
   if (sum > 1.0)
      sum = 1.0;

   return (sum);
}


/*
       Vector-valued version of "Noise"
*/

void DNoise(result, x, y, z)
VECTOR *result;
DBL x, y, z;
{
   long ix, iy, iz, jx, jy, jz;
   DBL px, py, pz, s;
   DBL sx, sy, sz, tx, ty, tz;
   short m;

   Calls_To_DNoise++;

   setup_lattice(&x, &y, &z, &ix, &iy, &iz, &jx, &jy, &jz, &sx, &sy, &sz, &tx, &ty, &tz);

   /*
    *  interpolate!
    */
   m = Hash3d( ix, iy, iz ) & 0xFF;
   px = x-ix;  py = y-iy;  pz = z-iz;
   s = tx*ty*tz;
   result->x = INCRSUM(m,s,px,py,pz);
   result->y = INCRSUM(m+4,s,px,py,pz);
   result->z = INCRSUM(m+8,s,px,py,pz);

   m = Hash3d( jx, iy, iz ) & 0xFF;
   px = x-jx;
   s = sx*ty*tz;
   result->x += INCRSUM(m,s,px,py,pz);
   result->y += INCRSUM(m+4,s,px,py,pz);
   result->z += INCRSUM(m+8,s,px,py,pz);

   m = Hash3d( jx, jy, iz ) & 0xFF;
   py = y-jy;
   s = sx*sy*tz;
   result->x += INCRSUM(m,s,px,py,pz);
   result->y += INCRSUM(m+4,s,px,py,pz);
   result->z += INCRSUM(m+8,s,px,py,pz);

   m = Hash3d( ix, jy, iz ) & 0xFF;
   px = x-ix;
   s = tx*sy*tz;
   result->x += INCRSUM(m,s,px,py,pz);
   result->y += INCRSUM(m+4,s,px,py,pz);
   result->z += INCRSUM(m+8,s,px,py,pz);

   m = Hash3d( ix, jy, jz ) & 0xFF;
   pz = z-jz;
   s = tx*sy*sz;
   result->x += INCRSUM(m,s,px,py,pz);
   result->y += INCRSUM(m+4,s,px,py,pz);
   result->z += INCRSUM(m+8,s,px,py,pz);

   m = Hash3d( jx, jy, jz ) & 0xFF;
   px = x-jx;
   s = sx*sy*sz;
   result->x += INCRSUM(m,s,px,py,pz);
   result->y += INCRSUM(m+4,s,px,py,pz);
   result->z += INCRSUM(m+8,s,px,py,pz);

   m = Hash3d( jx, iy, jz ) & 0xFF;
   py = y-iy;
   s = sx*ty*sz;
   result->x += INCRSUM(m,s,px,py,pz);
   result->y += INCRSUM(m+4,s,px,py,pz);
   result->z += INCRSUM(m+8,s,px,py,pz);

   m = Hash3d( ix, iy, jz ) & 0xFF;
   px = x-ix;
   s = tx*ty*sz;
   result->x += INCRSUM(m,s,px,py,pz);
   result->y += INCRSUM(m+4,s,px,py,pz);
   result->z += INCRSUM(m+8,s,px,py,pz);
}

DBL Turbulence (x, y, z, octaves)
DBL x, y, z;
int octaves;
{
   int i;                               /* added -dmf */
   register DBL t = 0.0;
   register DBL scale, value;

   for (i=0, scale = 1; i < octaves; i++, scale *= 0.5) {
      value = Noise (x/scale, y/scale, z/scale);
      t += FABS (value) * scale;
   }
   return (t);
}

void DTurbulence (result, x, y, z,octaves)
VECTOR *result;
DBL x, y, z;
int octaves;
{
   int i;                               /* added -dmf */
   register DBL scale;
   VECTOR value;

   result -> x = 0.0;
   result -> y = 0.0;
   result -> z = 0.0;

   value.x = value.y = value.z = 0.0;

   for (i=0, scale = 1; i < octaves ; i++, scale *= 0.5) {
      DNoise(&value, x/scale, y/scale, z/scale);
      result -> x += value.x * scale;
      result -> y += value.y * scale;
      result -> z += value.z * scale;
   }
}

DBL cycloidal (value)
DBL value;
{
   register int indx;

   if (value >= 0.0)
   {
      indx = (int)((value - floor (value)) * SINTABSIZE);
      return (sintab [indx]);
   }
   else
   {
      indx = (int)((0.0 - (value + floor (0.0 - value))) * SINTABSIZE);
      return (0.0 - sintab [indx]);
   }
}


DBL Triangle_Wave (value)
DBL value;
{
   register DBL offset,temp1;

   if (value >= 0.0) offset = value - floor(value);
   else {
      temp1 = -1.0 - floor(fabs(value));
      offset = value - temp1;
   }
   if (offset >= 0.5) return (2.0 * (1.0 - offset));
   else return (2.0 * offset);
}

   void Translate_Texture (Texture_Ptr, Vector)
      TEXTURE **Texture_Ptr;
VECTOR *Vector;
{
   TEXTURE *Texture = *Texture_Ptr;
   TRANSFORMATION Transformation;

   while (Texture != NULL) {
      if (((Texture->Texture_Number != NO_TEXTURE)
         && (Texture->Texture_Number != COLOUR_TEXTURE))
         || (Texture->Bump_Number != NO_BUMPS)) {

         if (Texture->Constant_Flag) {
            Texture = Copy_Texture (Texture);
            *Texture_Ptr = Texture;
            Texture->Constant_Flag = FALSE;
         }

         if (!Texture -> Texture_Transformation)
            Texture -> Texture_Transformation = Get_Transformation ();
         Get_Translation_Transformation (&Transformation, Vector);
         Compose_Transformations (Texture -> Texture_Transformation,
            &Transformation);
         if (Texture->Texture_Number == CHECKER_TEXTURE_TEXTURE) {
            Translate_Texture ((TEXTURE **) &Texture->Colour1, Vector);
            Translate_Texture ((TEXTURE **) &Texture->Colour2, Vector);
         }
      }
      Texture_Ptr = &Texture->Next_Texture;
      Texture = Texture->Next_Texture;
   }
}

void Rotate_Texture (Texture_Ptr, Vector)
TEXTURE **Texture_Ptr;
VECTOR *Vector;
{
   TEXTURE *Texture = *Texture_Ptr;
   TRANSFORMATION Transformation;

   while (Texture != NULL) {
      if (((Texture->Texture_Number != NO_TEXTURE)
         && (Texture->Texture_Number != COLOUR_TEXTURE))
         || (Texture->Bump_Number != NO_BUMPS)) {

         if (Texture->Constant_Flag) {
            Texture = Copy_Texture (Texture);
            *Texture_Ptr = Texture;
            Texture->Constant_Flag = FALSE;
         }

         if (!Texture -> Texture_Transformation)
            Texture -> Texture_Transformation = Get_Transformation ();
         Get_Rotation_Transformation (&Transformation, Vector);
         Compose_Transformations (Texture -> Texture_Transformation,
            &Transformation);
         if (Texture->Texture_Number == CHECKER_TEXTURE_TEXTURE) {
            Rotate_Texture ((TEXTURE **) &Texture->Colour1, Vector);
            Rotate_Texture ((TEXTURE **) &Texture->Colour2, Vector);
         }
      }
      Texture_Ptr = &Texture->Next_Texture;
      Texture = Texture->Next_Texture;
   }
}

void Scale_Texture (Texture_Ptr, Vector)
TEXTURE **Texture_Ptr;
VECTOR *Vector;
{
   TEXTURE *Texture = *Texture_Ptr;
   TRANSFORMATION Transformation;

   while (Texture != NULL) {
      if (((Texture->Texture_Number != NO_TEXTURE)
         && (Texture->Texture_Number != COLOUR_TEXTURE))
         || (Texture->Bump_Number != NO_BUMPS)) {

         if (Texture->Constant_Flag) {
            Texture = Copy_Texture (Texture);
            *Texture_Ptr = Texture;
            Texture->Constant_Flag = FALSE;
         }

         if (!Texture -> Texture_Transformation)
            Texture -> Texture_Transformation = Get_Transformation ();
         Get_Scaling_Transformation (&Transformation, Vector);
         Compose_Transformations (Texture -> Texture_Transformation,
            &Transformation);

         if (Texture->Texture_Number == CHECKER_TEXTURE_TEXTURE) {
            Scale_Texture ((TEXTURE **) &Texture->Colour1, Vector);
            Scale_Texture ((TEXTURE **) &Texture->Colour2, Vector);
         }
      }
      Texture_Ptr = &Texture->Next_Texture;
      Texture = Texture->Next_Texture;
   }
}
