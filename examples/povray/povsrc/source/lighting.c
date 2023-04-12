/****************************************************************************
*                   lighting.c
*
*  This module calculates lighting properties like ambient, diffuse, specular,
*  reflection, refraction, etc.
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
#include "frame.h"
#include "vector.h"
#include "povproto.h"

extern int Trace_Level;
extern FRAME Frame;
extern unsigned int Options;
extern int Quality;
extern int Shadow_Test_Flag;
extern long Shadow_Ray_Tests, Shadow_Rays_Succeeded;
extern long Reflected_Rays_Traced, Refracted_Rays_Traced;
extern long Transmitted_Rays_Traced;



#define COORDINATE_LIMIT 1.0e17

/* "Small_Tolerance" is just too tight for higher order polynomial equations.
   this value should probably be a variable of some sort, but for now just
   use a reasonably small value.  If people render real small objects real 
   close to each other then there may be some shading problems.  Otherwise
   having SHADOW_TOLERANCE as large as this won't affect images. */
#define SHADOW_TOLERANCE 0.05

static void do_light PARAMS((LIGHT_SHAPE *Light_Source, DBL *Light_Source_Depth,
RAY *Light_Source_Ray, VECTOR *Intersection_Point, COLOUR *Light_Colour));
static int do_blocking PARAMS((INTERSECTION *Local_Intersection, 
COLOUR *Light_Colour, PRIOQ *Local_Queue));
static void do_phong PARAMS((TEXTURE *Texture, RAY *Light_Source_Ray,
VECTOR Eye, VECTOR *Surface_Normal, COLOUR *Colour, COLOUR *Light_Colour,
COLOUR *Surface_Colour));
static void do_specular PARAMS((TEXTURE *Texture, RAY *Light_Source_Ray,
VECTOR REye, VECTOR *Surface_Normal, COLOUR *Colour, COLOUR *Light_Colour,
COLOUR *Surface_Colour));
static void do_diffuse PARAMS((TEXTURE *Texture, RAY *Light_Source_Ray,
VECTOR *Surface_Normal, COLOUR *Colour, COLOUR *Light_Colour,
COLOUR *Surface_Colour, DBL Attenuation));




void Colour_At (Colour, Texture, Intersection_Point)
COLOUR *Colour;
TEXTURE *Texture;
VECTOR *Intersection_Point;
{
   register DBL x, y, z;
   VECTOR Transformed_Point;


   if ((Intersection_Point->x > COORDINATE_LIMIT) ||
      (Intersection_Point->y > COORDINATE_LIMIT) ||
      (Intersection_Point->z > COORDINATE_LIMIT) ||
      (Intersection_Point->x < -COORDINATE_LIMIT) ||
      (Intersection_Point->y < -COORDINATE_LIMIT) ||
      (Intersection_Point->z < -COORDINATE_LIMIT))
   {
      Make_Vector (&Transformed_Point, 0.0, 0.0, 0.0);
   }
   else
      if (Texture->Texture_Transformation) {
         MInverseTransformVector (&Transformed_Point,
            Intersection_Point,
            Texture->Texture_Transformation);
      }
      else {
         Transformed_Point = *Intersection_Point;
      }

      x = Transformed_Point.x;
   y = Transformed_Point.y;
   z = Transformed_Point.z;

   switch (Texture->Texture_Number) {
   case NO_TEXTURE:
      /* No colouring texture has been specified - make it black. */
      Make_Colour (Colour, 0.0, 0.0, 0.0);
      Colour -> Alpha  = 0.0;
      break;

   case COLOUR_TEXTURE:
      Colour -> Red += Texture->Colour1->Red;
      Colour -> Green += Texture->Colour1->Green;
      Colour -> Blue += Texture->Colour1->Blue;
      Colour -> Alpha += Texture->Colour1->Alpha;
      break;

   case BOZO_TEXTURE: 
      bozo (x, y, z, Texture, Colour);
      break;

   case MARBLE_TEXTURE:
      marble (x, y, z, Texture, Colour);
      break;

   case WOOD_TEXTURE:
      wood (x, y, z, Texture, Colour);
      break;

   case BRICK_TEXTURE:
      brick (x, y, z, Texture, Colour);
      break;

   case CHECKER_TEXTURE:
      checker (x, y, z, Texture, Colour);
      break;

   case CHECKER_TEXTURE_TEXTURE:
      checker_texture (x, y, z, Texture, Colour);
      break;

   case SPOTTED_TEXTURE:
      spotted (x, y, z, Texture, Colour);
      break;

   case AGATE_TEXTURE:
      agate (x, y, z, Texture, Colour);
      break;

   case GRANITE_TEXTURE:
      granite (x, y, z, Texture, Colour);
      break;

   case GRADIENT_TEXTURE:
      gradient (x, y, z, Texture, Colour);
      break;

   case IMAGEMAP_TEXTURE:
      image_map (x, y, z, Texture, Colour);
      break;

   case ONION_TEXTURE:
      onion (x, y, z, Texture, Colour);
      break;

   case LEOPARD_TEXTURE:
      leopard (x, y, z, Texture, Colour);
      break;

   case PAINTED1_TEXTURE:
      painted1 (x, y, z, Texture, Colour);
      break;

   case PAINTED2_TEXTURE:
      painted2 (x, y, z, Texture, Colour);
      break;

   case PAINTED3_TEXTURE:
      painted3 (x, y, z, Texture, Colour);
      break;
   }
}


void Perturb_Normal(New_Normal, Texture, Intersection_Point, Surface_Normal)
VECTOR *New_Normal, *Intersection_Point, *Surface_Normal;
TEXTURE *Texture;
{
   VECTOR Transformed_Point;
   register DBL x, y, z;

   if (Texture->Bump_Number == NO_BUMPS) {
      *New_Normal = *Surface_Normal;
      return;
   }

   if (Texture->Texture_Transformation)
      MInverseTransformVector (&Transformed_Point,
         Intersection_Point,
         Texture->Texture_Transformation);
   else
      Transformed_Point = *Intersection_Point;

   x = Transformed_Point.x;
   y = Transformed_Point.y;
   z = Transformed_Point.z;

   switch (Texture->Bump_Number) {

   case WAVES: 
      waves (x, y, z, Texture, New_Normal);
      break;

   case RIPPLES: 
      ripples (x, y, z, Texture, New_Normal);
      break;

   case WRINKLES: 
      wrinkles (x, y, z, Texture, New_Normal);
      break;

   case BUMPS: 
      bumps (x, y, z, Texture, New_Normal);
      break;

   case DENTS: 
      dents (x, y, z, Texture, New_Normal);
      break; 

   case BUMPY1: 
      bumpy1 (x, y, z, Texture, New_Normal);
      break;

   case BUMPY2: 
      bumpy2 (x, y, z, Texture, New_Normal);
      break;

   case BUMPY3: 
      bumpy3 (x, y, z, Texture, New_Normal);
      break;

   case BUMPMAP: 
      bump_map (x, y, z, Texture, New_Normal);
      break;

   }
   return;
}

void Ambient (Texture, Surface_Colour, Colour, Attenuation)
TEXTURE *Texture;
COLOUR *Surface_Colour;
COLOUR *Colour;
DBL    Attenuation;
{
   if (Texture -> Object_Ambient == 0.0)
      return;

   Colour->Red += Surface_Colour->Red * Texture->Object_Ambient * Attenuation;
   Colour->Green += Surface_Colour->Green * Texture->Object_Ambient * Attenuation;
   Colour->Blue += Surface_Colour->Blue * Texture->Object_Ambient * Attenuation;
   return;
}


void Diffuse (Texture, Intersection_Point, Eye, Surface_Normal, Surface_Colour, Colour, Attenuation)
TEXTURE *Texture;
VECTOR *Intersection_Point, *Surface_Normal;
COLOUR *Surface_Colour;
COLOUR *Colour;
RAY    *Eye;
DBL    Attenuation;
{
   DBL Light_Source_Depth;
   RAY Light_Source_Ray;
   LIGHT_SHAPE *Light_Source;
   OBJECT *Blocking_Object;
   int Intersection_Found;
   INTERSECTION *Local_Intersection;
   VECTOR REye;
   COLOUR Light_Colour;
   PRIOQ *Local_Queue;

   if ((Texture -> Object_Diffuse == 0.0) &&
      (Texture -> Object_Specular == 0.0) &&
      (Texture -> Object_Phong == 0.0))
      return;

   if (Texture -> Object_Specular != 0.0)
   {
      REye.x = -Eye->Direction.x;
      REye.y = -Eye->Direction.y;
      REye.z = -Eye->Direction.z;
   }

   Local_Queue = pq_new (128);

   for (Light_Source = Frame.Light_Sources ; 
      	Light_Source != NULL;
      	Light_Source = Light_Source -> Next_Light_Source)
   {
      Intersection_Found = FALSE;

      do_light(Light_Source, &Light_Source_Depth, &Light_Source_Ray, Intersection_Point, &Light_Colour);

      /* What objects does this ray intersect? */
      if (Quality > 3) {
         Shadow_Test_Flag = TRUE;
         for (Blocking_Object = Frame.Objects ; 
            	      Blocking_Object != NULL ;
            	      Blocking_Object = Blocking_Object -> Next_Object) {

            Shadow_Ray_Tests++;
            for (All_Intersections (Blocking_Object, &Light_Source_Ray, Local_Queue) ;
               		 (Local_Intersection = pq_get_highest(Local_Queue)) != NULL ;
               		 pq_delete_highest(Local_Queue)) {

               if ((Local_Intersection -> Depth < Light_Source_Depth-Small_Tolerance)
                  && (Local_Intersection -> Depth > SHADOW_TOLERANCE))

                  /* Does the object not cast a shadow? */
                  if (!Local_Intersection -> Object -> No_Shadow_Flag)
                     if (do_blocking(Local_Intersection, &Light_Colour, Local_Queue)) {
                        Intersection_Found = TRUE;
                        break;
                     }
            }
            if (Intersection_Found)
               break;
         }
         Shadow_Test_Flag = FALSE;
      }

      /* If light source was not blocked by any intervening object, then
	   calculate it's contribution to the object's overall illumination */

      if (!Intersection_Found) {
         if (Texture->Object_Phong >0.0) /* Phong Hilite */
            do_phong(Texture,&Light_Source_Ray,Eye->Direction,Surface_Normal,Colour,&Light_Colour, Surface_Colour);

         if (Texture->Object_Specular >0.0) /* Specular Hilite */
            do_specular(Texture,&Light_Source_Ray,REye,Surface_Normal,Colour,&Light_Colour, Surface_Colour);

         if (Texture->Object_Diffuse > 0.0) /* Normal Diffuse Illum. */
            do_diffuse(Texture,&Light_Source_Ray,Surface_Normal,Colour,&Light_Colour,Surface_Colour, Attenuation);
      }
   }
   pq_free (Local_Queue);
   return;
}


static void do_light(Light_Source, Light_Source_Depth, Light_Source_Ray, Intersection_Point, Light_Colour)
LIGHT_SHAPE *Light_Source;
DBL *Light_Source_Depth;
RAY *Light_Source_Ray;
VECTOR *Intersection_Point;
COLOUR *Light_Colour;
{
   DBL Attenuation = 1.0;

   /* Get the light source colour. */
   if (Light_Source->Shape_Colour == NULL) {
      Make_Colour (Light_Colour, 1.0, 1.0, 1.0);
   }
   else
      *Light_Colour = *Light_Source->Shape_Colour;

   Light_Source_Ray->Initial = *Intersection_Point;
   Light_Source_Ray->Quadric_Constants_Cached = FALSE;

   VSub (Light_Source_Ray->Direction,
      Light_Source->Center,
      *Intersection_Point);

   VLength (*Light_Source_Depth, Light_Source_Ray->Direction);

   VScale (Light_Source_Ray->Direction, Light_Source_Ray->Direction,
      1.0/(*Light_Source_Depth));

   Attenuation = Attenuate_Light(Light_Source, Light_Source_Ray);

   /* Now scale the color by the attenuation */
   Light_Colour->Red   *= Attenuation;
   Light_Colour->Green *= Attenuation;
   Light_Colour->Blue  *= Attenuation;


   return;
}

static int do_blocking(Local_Intersection, Light_Colour, Local_Queue)
INTERSECTION *Local_Intersection;
COLOUR *Light_Colour;
PRIOQ *Local_Queue;
{
   Shadow_Rays_Succeeded++;

   Determine_Surface_Colour (Local_Intersection, Light_Colour, NULL, TRUE);

   if ((Light_Colour->Red < 0.01) && (Light_Colour->Green < 0.01)
      && (Light_Colour->Blue < 0.01)) {

      while (pq_get_highest(Local_Queue))
         pq_delete_highest(Local_Queue);
      return(TRUE);
   }
   return(FALSE);
}

static void do_phong(Texture, Light_Source_Ray, Eye, Surface_Normal, Colour, Light_Colour, Surface_Colour)
TEXTURE *Texture;
RAY *Light_Source_Ray;
VECTOR *Surface_Normal, Eye;
COLOUR *Colour, *Light_Colour, *Surface_Colour;
{
   DBL Cos_Angle_Of_Incidence, Normal_Length, Intensity;
   VECTOR Local_Normal, Normal_Projection, Reflect_Direction;

   VDot(Cos_Angle_Of_Incidence, Eye, *Surface_Normal);

   if (Cos_Angle_Of_Incidence < 0.0)
   {
      Local_Normal = *Surface_Normal;
      Cos_Angle_Of_Incidence = -Cos_Angle_Of_Incidence;
   }
   else
   {
      VScale (Local_Normal, *Surface_Normal, -1.0);
   }

   VScale (Normal_Projection, Local_Normal, Cos_Angle_Of_Incidence);
   VScale (Normal_Projection, Normal_Projection, 2.0);
   VAdd (Reflect_Direction, Eye, Normal_Projection);

   VDot (Cos_Angle_Of_Incidence, Reflect_Direction, Light_Source_Ray->Direction);
   VLength (Normal_Length, Light_Source_Ray->Direction);

   if (Normal_Length == 0.0)
      Cos_Angle_Of_Incidence = 0.0;
   else Cos_Angle_Of_Incidence /= Normal_Length;

      if (Cos_Angle_Of_Incidence < 0.0)
         Cos_Angle_Of_Incidence = 0;

   if (Texture -> Object_PhongSize != 1.0)
      Intensity = pow(Cos_Angle_Of_Incidence, Texture->Object_PhongSize);
   else
      Intensity = Cos_Angle_Of_Incidence;

   Intensity *= Texture -> Object_Phong;

   if (Texture->Metallic_Flag) {
      Colour->Red+=Intensity*(Surface_Colour->Red);
      Colour->Green+=Intensity*(Surface_Colour->Green);
      Colour->Blue+=Intensity*(Surface_Colour->Blue);
   }
   else {
      Colour->Red+=Intensity*(Light_Colour->Red);
      Colour->Green+=Intensity*(Light_Colour->Green);
      Colour->Blue+=Intensity*(Light_Colour->Blue);
   }
   return;
}

   static void do_specular(Texture, Light_Source_Ray, REye, Surface_Normal, Colour, Light_Colour, Surface_Colour)
      TEXTURE *Texture;
RAY *Light_Source_Ray;
VECTOR *Surface_Normal, REye;
COLOUR *Colour, *Light_Colour, *Surface_Colour;
{
   DBL Cos_Angle_Of_Incidence, Normal_Length, Intensity, Halfway_Length, Roughness;
   VECTOR Halfway;

   VHalf (Halfway, REye, Light_Source_Ray->Direction);
   VLength (Normal_Length, *Surface_Normal);
   VLength (Halfway_Length, Halfway);
   VDot (Cos_Angle_Of_Incidence, Halfway, *Surface_Normal);

   if (Normal_Length == 0.0 || Halfway_Length == 0.0)
      Cos_Angle_Of_Incidence = 0.0;
   else Cos_Angle_Of_Incidence /= (Normal_Length * Halfway_Length);

      if (Cos_Angle_Of_Incidence < 0.0)
         Cos_Angle_Of_Incidence = 0.0;

   Roughness = 1.0 / Texture->Object_Roughness;

   if (Roughness != 1.0)
      Intensity = pow(Cos_Angle_Of_Incidence, Roughness);
   else
      Intensity = Cos_Angle_Of_Incidence;
   Intensity *= Texture->Object_Specular;
   if (Texture->Metallic_Flag) {
      Colour->Red+=Intensity*(Surface_Colour->Red);
      Colour->Green+=Intensity*(Surface_Colour->Green);
      Colour->Blue+=Intensity*(Surface_Colour->Blue);
   }
   else {
      Colour->Red+=Intensity*(Light_Colour->Red);
      Colour->Green+=Intensity*(Light_Colour->Green);
      Colour->Blue+=Intensity*(Light_Colour->Blue);
   }
   return;
}

   static void do_diffuse(Texture, Light_Source_Ray, Surface_Normal, Colour, Light_Colour, Surface_Colour, Attenuation)
      TEXTURE *Texture;
RAY *Light_Source_Ray;
VECTOR *Surface_Normal;
COLOUR *Colour, *Light_Colour, *Surface_Colour;
DBL Attenuation;
{
   DBL Cos_Angle_Of_Incidence, Intensity, RandomNumber;

   VDot (Cos_Angle_Of_Incidence, *Surface_Normal, Light_Source_Ray->Direction);
   if (Cos_Angle_Of_Incidence < 0.0)
      Cos_Angle_Of_Incidence = -Cos_Angle_Of_Incidence;

   if (Texture -> Object_Brilliance != 1.0)
      Intensity = pow(Cos_Angle_Of_Incidence, Texture->Object_Brilliance);
   else
      Intensity = Cos_Angle_Of_Incidence;

   Intensity *= Texture -> Object_Diffuse * Attenuation;

   RandomNumber = (rand()&0x7FFF)/(DBL) 0x7FFF;

   Intensity -= RandomNumber * Texture->Texture_Randomness;

   Colour->Red += Intensity * (Surface_Colour->Red) * (Light_Colour->Red);
   Colour->Green += Intensity * (Surface_Colour->Green) * (Light_Colour->Green);
   Colour->Blue += Intensity * (Surface_Colour->Blue) * (Light_Colour->Blue);
   return;
}

void Reflect (Texture, Intersection_Point, Ray, Surface_Normal, Colour)
TEXTURE *Texture;
VECTOR *Intersection_Point;
RAY *Ray;
VECTOR *Surface_Normal;
COLOUR *Colour;
{
   RAY New_Ray;
   COLOUR Temp_Colour;
   VECTOR Local_Normal;
   VECTOR Normal_Projection;
   VECTOR Surface_Offset;
   register DBL Normal_Component;

   if (Texture -> Object_Reflection != 0.0)
      {
      Reflected_Rays_Traced++;
      VDot (Normal_Component, Ray -> Direction, *Surface_Normal);
      if (Normal_Component < 0.0) {
         Local_Normal = *Surface_Normal;
         Normal_Component *= -1.0;
      }
      else
         VScale (Local_Normal, *Surface_Normal, -1.0);

      VScale (Normal_Projection, Local_Normal, Normal_Component);
      VScale (Normal_Projection, Normal_Projection, 2.0);
      VAdd (New_Ray.Direction, Ray -> Direction, Normal_Projection);
      New_Ray.Initial = *Intersection_Point;

      /* ARE 08/25/91 */

      VScale(Surface_Offset, New_Ray.Direction, 2.0 * Small_Tolerance); 
      VAdd(New_Ray.Initial, New_Ray.Initial, Surface_Offset);           

      Copy_Ray_Containers (&New_Ray, Ray);
      Trace_Level++;
      Make_Colour (&Temp_Colour, 0.0, 0.0, 0.0);
      New_Ray.Quadric_Constants_Cached = FALSE;
      Trace (&New_Ray, &Temp_Colour);
      Trace_Level--;

      Colour -> Red += Temp_Colour.Red 
      * Texture -> Object_Reflection;
      Colour -> Green += Temp_Colour.Green
      * Texture -> Object_Reflection;
      Colour -> Blue += Temp_Colour.Blue
      * Texture -> Object_Reflection;

   }
}

void Refract (Texture, Intersection_Point, Ray, Surface_Normal, Colour)
TEXTURE *Texture;
VECTOR *Intersection_Point;
RAY *Ray;
VECTOR *Surface_Normal;
COLOUR *Colour;
{
   RAY New_Ray;
   COLOUR Temp_Colour;
   VECTOR Local_Normal;
   VECTOR Ray_Direction;
   register DBL Normal_Component, Temp_IOR;
   DBL temp, ior;
   int inside;

   if (Surface_Normal == NULL) {
      New_Ray.Initial = *Intersection_Point;
      New_Ray.Direction = Ray->Direction;

      Copy_Ray_Containers (&New_Ray, Ray);
      Trace_Level++;
      Transmitted_Rays_Traced++;
      Make_Colour (&Temp_Colour, 0.0, 0.0, 0.0);
      New_Ray.Quadric_Constants_Cached = FALSE;
      Trace (&New_Ray, &Temp_Colour);
      Trace_Level--;
      (Colour -> Red) += Temp_Colour.Red;
      (Colour -> Green) += Temp_Colour.Green;
      (Colour -> Blue) += Temp_Colour.Blue;
   }
      else {
         Refracted_Rays_Traced++;
         VDot (Normal_Component, Ray -> Direction, *Surface_Normal);
         if (Normal_Component <= 0.0)
            {
            Local_Normal.x = Surface_Normal -> x;
            Local_Normal.y = Surface_Normal -> y;
            Local_Normal.z = Surface_Normal -> z;
            Normal_Component *= -1.0;
            inside = FALSE;
         }
         else
         {
            VScale (Local_Normal, *Surface_Normal, -1.0);
            inside = TRUE;
         }


         Copy_Ray_Containers (&New_Ray, Ray);

         if (Ray -> Containing_Index == -1)
            {
            /* The ray is entering from the atmosphere */
            Ray_Enter (&New_Ray, Texture);
            ior = (Frame.Atmosphere_IOR)/(Texture -> Object_Index_Of_Refraction);
         }
         else
         {
            /* The ray is currently inside an object */
            if (New_Ray.Containing_Textures [New_Ray.Containing_Index] == Texture) 
               /*         if (inside) */
            {
               /* The ray is leaving the current object */
               Ray_Exit (&New_Ray);
               if (New_Ray.Containing_Index == -1)
                  /* The ray is leaving into the atmosphere */
               Temp_IOR = Frame.Atmosphere_IOR;
               else
                  /* The ray is leaving into another object */
                  Temp_IOR = New_Ray.Containing_IORs [New_Ray.Containing_Index];

               ior =  (Texture -> Object_Index_Of_Refraction)/Temp_IOR;
            }
               else
               {
                  /* The ray is entering a new object */
                  Temp_IOR = New_Ray.Containing_IORs [New_Ray.Containing_Index];
                  Ray_Enter (&New_Ray, Texture);

                  ior =  Temp_IOR / (Texture -> Object_Index_Of_Refraction);
               }
            }

            temp = 1.0 + ior * ior * (Normal_Component * Normal_Component - 1.0);
            if (temp < 0.0) {
               /* Total internal reflection - not yet implemented.
	 Reflect (Texture, Intersection_Point, Ray, Surface_Normal, Color);
	 */
               return;
            }

            temp = ior*Normal_Component - sqrt(temp);
            VScale (Local_Normal, Local_Normal, temp);
            VScale (Ray_Direction, Ray->Direction, ior);
            VAdd (New_Ray.Direction, Local_Normal, Ray_Direction);
            VNormalize (New_Ray.Direction, New_Ray.Direction);

            New_Ray.Initial = *Intersection_Point;
            Trace_Level++;
            Make_Colour (&Temp_Colour, 0.0, 0.0, 0.0);
            New_Ray.Quadric_Constants_Cached = FALSE;

            Trace (&New_Ray, &Temp_Colour);
            Trace_Level--;

            (Colour -> Red) += (Temp_Colour.Red)
               * (Texture -> Object_Refraction);
            (Colour -> Green) += (Temp_Colour.Green)
               * (Texture -> Object_Refraction);
            (Colour -> Blue) += (Temp_Colour.Blue)
               * (Texture -> Object_Refraction);
         }
      }

      void Fog (Distance, Fog_Colour, Fog_Distance, Colour)
         DBL Distance, Fog_Distance;
      COLOUR *Fog_Colour, *Colour;
      {
         DBL Fog_Factor, Fog_Factor_Inverse;

         Fog_Factor = exp(-1.0 * Distance/Fog_Distance);
         Fog_Factor_Inverse = 1.0 - Fog_Factor;
         Colour->Red = Colour->Red*Fog_Factor + Fog_Colour->Red*Fog_Factor_Inverse;
         Colour->Green = Colour->Green*Fog_Factor + Fog_Colour->Green*Fog_Factor_Inverse;
         Colour->Blue = Colour->Blue*Fog_Factor + Fog_Colour->Blue*Fog_Factor_Inverse;
      }



      void Compute_Reflected_Colour (Ray, Texture, Ray_Intersection, Surface_Colour, Filter_Colour, Colour)
         RAY *Ray;
      TEXTURE *Texture;
      INTERSECTION *Ray_Intersection;
      COLOUR *Surface_Colour;
      COLOUR *Filter_Colour;
      COLOUR *Colour;
      {
         VECTOR Surface_Normal;
         DBL Normal_Direction, Attenuation;
         COLOUR Emitted_Colour;

         /* This variable keeps track of how much colour comes from the surface
      of the object and how much is transmited through. */

         Make_Colour (&Emitted_Colour, 0.0, 0.0, 0.0);

         if (Texture == NULL)
            Texture = Ray_Intersection->Object->Object_Texture;

         if (Quality <= 1) {
            Surface_Colour->Alpha = 0.0;

            Colour->Red   += Surface_Colour->Red * Filter_Colour->Alpha;
            Colour->Green += Surface_Colour->Green * Filter_Colour->Alpha;
            Colour->Blue  += Surface_Colour->Blue * Filter_Colour->Alpha;
            return;
         }

         Normal (&Surface_Normal, (OBJECT *) Ray_Intersection -> Shape,
         &Ray_Intersection -> Point);

         if (Quality >= 8) {
            Perturb_Normal (&Surface_Normal, Texture,
            &Ray_Intersection->Point, &Surface_Normal);
         }

         /* If the surface normal points away, flip its direction. */
         VDot (Normal_Direction, Surface_Normal, Ray->Direction);
         if (Normal_Direction > 0.0) {
            VScale (Surface_Normal, Surface_Normal, -1.0);
         }

         Attenuation = Filter_Colour->Alpha * (1.0 - Surface_Colour->Alpha);


         Ambient (Texture, Surface_Colour, &Emitted_Colour, Attenuation);
         Diffuse (Texture, &Ray_Intersection -> Point, Ray,
         &Surface_Normal, Surface_Colour, &Emitted_Colour, Attenuation);
         Colour->Red   += Emitted_Colour.Red;
         Colour->Green += Emitted_Colour.Green;
         Colour->Blue  += Emitted_Colour.Blue;
         if (Quality >= 8)
            Reflect (Texture, &Ray_Intersection -> Point, Ray,
         &Surface_Normal, Colour); 
      }

      void Determine_Surface_Colour (Ray_Intersection, Colour, Ray, Shadow_Ray)
         INTERSECTION *Ray_Intersection;
      COLOUR *Colour;
      RAY *Ray;
      int Shadow_Ray;
      {
         COLOUR Surface_Colour, Refracted_Colour, Filter_Colour;
         TEXTURE *Temp_Texture, *Texture;
         VECTOR Surface_Normal;
         DBL Normal_Direction;
         int surface;

         if (!Shadow_Ray)
            Make_Colour (Colour, 0.0, 0.0, 0.0);

         if (Options & DEBUGGING)
            if (Ray_Intersection->Shape->Shape_Colour)
            printf ("Depth: %f Object %d Colour %f %f %f ", Ray_Intersection->Depth,
         Ray_Intersection->Shape->Type,
         Ray_Intersection->Shape->Shape_Colour->Red,
         Ray_Intersection->Shape->Shape_Colour->Green,
         Ray_Intersection->Shape->Shape_Colour->Blue);
         else
            printf ("Depth: %f Object %d Colour NIL ", Ray_Intersection->Depth,
            Ray_Intersection->Shape->Type);


         Make_Colour (&Surface_Colour, 0.0, 0.0, 0.0);

         /* Is there a texture in the shape?  If not, use the one in the object. */
         if ((Texture = Ray_Intersection->Shape->Shape_Texture) == NULL) {
            Texture = Ray_Intersection->Object->Object_Texture;      
         }
         /* Check to see if this object/shape has a material_map texture, if so */
         /* then change the texture pointer to point to the mapped texture - CdW 7/91 */
         if(Texture->Texture_Number == MATERIAL_MAP_TEXTURE)  
            Texture = material_map(&Ray_Intersection->Point, Texture);



         /* If this is just a shadow ray and we're rendering low quality, then return */

         if (Shadow_Ray && (Quality <= 5))
            return;

         Make_Colour (&Filter_Colour, 1.0, 1.0, 1.0);
         Filter_Colour.Alpha = 1.0;

         /* Now, we perform the lighting calculations. */
         for ( surface = 1 , Temp_Texture = Texture;
            	(Temp_Texture != NULL) && (Filter_Colour.Alpha > 0.01) ;
            	surface++, Temp_Texture = Temp_Texture->Next_Texture) {

            Make_Colour (&Surface_Colour, 0.0, 0.0, 0.0);
            if (Quality <= 5) {
               if (Ray_Intersection->Shape->Shape_Colour != NULL)
                  Surface_Colour = *Ray_Intersection->Shape->Shape_Colour;
            else if (Ray_Intersection->Object->Object_Colour != NULL)
               Surface_Colour = *Ray_Intersection->Object->Object_Colour;
            else {
               Make_Colour (&Surface_Colour, 0.5, 0.5, 0.5);
            }
         }
         else
            Colour_At (&Surface_Colour,
            Temp_Texture,
            &Ray_Intersection -> Point);
         /* We don't need to compute the lighting characteristics for shadow rays. */
         if (!Shadow_Ray) {
            Compute_Reflected_Colour (Ray,
            Temp_Texture,
            Ray_Intersection,
            &Surface_Colour,
            &Filter_Colour,
            Colour);

         }

         if (Options & DEBUGGING) {
            printf ("Surface %d\n", surface);
            printf ("   Surf: %6.4f %6.4f %6.4f %6.4f\n",
            Surface_Colour.Red, Surface_Colour.Green, Surface_Colour.Blue, Surface_Colour.Alpha);
            printf ("   Filter_Colour:   %6.4f %6.4f %6.4f %6.4f  Final Colour: %6.4f %6.4f %6.4f %6.4f  \n",
            Filter_Colour.Red, Filter_Colour.Green, Filter_Colour.Blue, Filter_Colour.Alpha,
            Colour->Red, Colour->Green, Colour->Blue, Colour->Alpha);
         }

         Filter_Colour.Red   *= Surface_Colour.Red;
         Filter_Colour.Green *= Surface_Colour.Green;
         Filter_Colour.Blue  *= Surface_Colour.Blue;

         Filter_Colour.Alpha *= Surface_Colour.Alpha;
      }

      /* For shadow rays, we have the filter colour now - time to return */
      if (Shadow_Ray) {

         if (Filter_Colour.Alpha < 0.01) {
            Make_Colour (Colour, 0.0, 0.0, 0.0);
            return;
         }

         if (Texture->Object_Refraction > 0.0) {
            Colour->Red *= Filter_Colour.Red * Texture->Object_Refraction * Filter_Colour.Alpha;
            Colour->Green *= Filter_Colour.Green * Texture->Object_Refraction * Filter_Colour.Alpha;
            Colour->Blue *= Filter_Colour.Blue * Texture->Object_Refraction * Filter_Colour.Alpha;
         }
         else {
            Colour->Red *= Filter_Colour.Red * Filter_Colour.Alpha;
            Colour->Green *= Filter_Colour.Green * Filter_Colour.Alpha;
            Colour->Blue *= Filter_Colour.Blue * Filter_Colour.Alpha;
         }
         return;
      }

         if ((Filter_Colour.Alpha > 0.01) && (Quality > 5)) {
            Make_Colour (&Refracted_Colour, 0.0, 0.0, 0.0);

            if (Texture->Object_Refraction > 0.0) {
               Normal (&Surface_Normal, (OBJECT *) Ray_Intersection -> Shape,
               &Ray_Intersection -> Point);

               if (Quality > 7) {
                  Perturb_Normal (&Surface_Normal, Texture,
                  &Ray_Intersection->Point, &Surface_Normal);
               }

            /* If the surface normal points away, flip its direction. */
            VDot (Normal_Direction, Surface_Normal, Ray->Direction);
            if (Normal_Direction > 0.0) {
               VScale (Surface_Normal, Surface_Normal, -1.0);
            }

            Refract (Texture, &Ray_Intersection -> Point, Ray,
            &Surface_Normal, &Refracted_Colour);
         }
            else
               Refract (Texture, &Ray_Intersection->Point, Ray,
               NULL, &Refracted_Colour);

            Colour->Red += Filter_Colour.Red * Refracted_Colour.Red * Filter_Colour.Alpha;
            Colour->Green += Filter_Colour.Green * Refracted_Colour.Green * Filter_Colour.Alpha;
            Colour->Blue += Filter_Colour.Blue * Refracted_Colour.Blue * Filter_Colour.Alpha;

            if (Texture->Object_Refraction > 0.0 &&
            Texture->Object_Transmit > 0.0) {
               Make_Colour (&Refracted_Colour, 0.0, 0.0, 0.0);
               Refract (Texture, &Ray_Intersection->Point, Ray,
               NULL, &Refracted_Colour);
               Colour->Red += Filter_Colour.Red * Refracted_Colour.Red * Filter_Colour.Alpha;
               Colour->Green += Filter_Colour.Green * Refracted_Colour.Green * Filter_Colour.Alpha;
               Colour->Blue += Filter_Colour.Blue * Refracted_Colour.Blue * Filter_Colour.Alpha;
            }
         }

         if (Frame.Fog_Distance != 0.0)  {
            Fog (Ray_Intersection->Depth, &Frame.Fog_Colour, Frame.Fog_Distance,
            Colour);
         }
      }
