/****************************************************************************
*                point.c
*
*  This module implements the point & spot light source primitive.
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

METHODS Point_Methods =
{ Object_Intersect, All_Point_Intersections,
   Inside_Point, NULL,
   Copy_Point,
   Translate_Point, Rotate_Point,
   Scale_Point, Invert_Point};

static DBL cubic_spline PARAMS(( DBL low,DBL high,DBL pos));
extern LIGHT_SHAPE *Get_Light_Source_Shape();

int All_Point_Intersections (Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;
PRIOQ *Depth_Queue;
{

   return(FALSE);
}


int Inside_Point (Test_Point, Object)
VECTOR *Test_Point;
OBJECT *Object;
{
   return(FALSE);
}


void *Copy_Point (Object)
OBJECT *Object;
{
   LIGHT_SHAPE *New_Shape;

   New_Shape = Get_Light_Source_Shape ();
   *New_Shape = *((LIGHT_SHAPE *) Object);
   New_Shape -> Next_Object = NULL;

   if (New_Shape->Shape_Texture != NULL)
      New_Shape->Shape_Texture = Copy_Texture (New_Shape->Shape_Texture);

   return (New_Shape);
}

void Translate_Point (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   VAdd (((LIGHT_SHAPE *) Object)->Center, ((LIGHT_SHAPE *) Object)->Center, *Vector);
   VAdd (((LIGHT_SHAPE *) Object)->Points_At,
      ((LIGHT_SHAPE *) Object)->Points_At, *Vector);
}

void Rotate_Point (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transformation;
   Get_Rotation_Transformation (&Transformation, Vector);
   MTransformVector (&((LIGHT_SHAPE *) Object)->Center,
      &((LIGHT_SHAPE *) Object)->Center, &Transformation);
   MTransformVector (&((LIGHT_SHAPE *) Object)->Points_At,
      &((LIGHT_SHAPE *) Object)->Points_At, &Transformation);
}

void Scale_Point (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transformation;
   Get_Scaling_Transformation (&Transformation, Vector);
   MTransformVector (&((LIGHT_SHAPE *) Object)->Center,
      &((LIGHT_SHAPE *) Object)->Center, &Transformation);
   MTransformVector (&((LIGHT_SHAPE *) Object)->Points_At,
      &((LIGHT_SHAPE *) Object)->Points_At, &Transformation);
   Scale_Texture (&((LIGHT_SHAPE *) Object)->Shape_Texture, Vector);
}

void Invert_Point (Object)
OBJECT *Object;
{
   ((LIGHT_SHAPE *) Object)->Inverted ^= TRUE;
}


/* Cubic spline that has tangents of slope 0 at x == low and at x == high.
   For a given value "pos" between low and high the spline value is returned */
static DBL cubic_spline(low, high, pos)
DBL low, high, pos;
{
   /* Check to see if the position is within the proper boundaries */
   if (pos < low)
      return 0.0;
   else if (pos > high)
      return 1.0;
   if (high == low)
      return 0.0;

   /* Normalize to the interval 0->1 */
   pos = (pos - low) / (high - low);

   /* See where it is on the cubic curve */
   return (3 - 2 * pos) * pos * pos;
}

DBL Attenuate_Light (Light_Source, Light_Source_Ray)
LIGHT_SHAPE *Light_Source;
RAY *Light_Source_Ray;
{
   DBL Len,costheta;
   DBL Attenuation = 1.0;
   VECTOR Spot_Direction;

   /* If this is a spotlight then attenuate based on the incidence angle */
   if (Light_Source->Type == SPOT_LIGHT_TYPE) {
      VSub(Spot_Direction, Light_Source->Points_At, Light_Source->Center);
      VLength(Len, Spot_Direction);
      if (Len > 0.0) {
         VInverseScale(Spot_Direction, Spot_Direction, Len);
         VDot(costheta, Light_Source_Ray->Direction, Spot_Direction);
         costheta *= -1.0;
         if (costheta > 0.0) {
            Attenuation = pow(costheta, Light_Source->Coeff);
            /* If there is a soft falloff region associated with the light then
	           do an interpolation of values between the hot center and the
	           direction at which light falls to nothing. */
            if (Light_Source->Radius > 0.0)
               Attenuation *= cubic_spline(Light_Source->Falloff,
                  Light_Source->Radius,
                  costheta);
            /* printf("Atten: %lg\n", Attenuation); */
         }
         else
            Attenuation = 0.0;
      }
      else
         Attenuation = 0.0;
   }    
   return(Attenuation);
}    
