/****************************************************************************
*                planes.c
*
*  This module implements functions that manipulate planes.
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

METHODS Plane_Methods =
{ Object_Intersect, All_Plane_Intersections,
   Inside_Plane, Plane_Normal,
   Copy_Plane,
   Translate_Plane, Rotate_Plane,
   Scale_Plane, Invert_Plane};

extern PLANE *Get_Plane_Shape();

extern RAY *VP_Ray;
extern long Ray_Plane_Tests, Ray_Plane_Tests_Succeeded;

int All_Plane_Intersections (Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;
PRIOQ *Depth_Queue;
{
   PLANE *Shape = (PLANE *) Object;
   DBL Depth;
   VECTOR Intersection_Point;
   INTERSECTION Local_Element;

   if (Intersect_Plane (Ray, Shape, &Depth))
      if (Depth > Small_Tolerance)
      {
         Local_Element.Depth = Depth;
         Local_Element.Object = Shape -> Parent_Object;
         VScale (Intersection_Point, Ray -> Direction, Depth);
         VAdd (Intersection_Point, Intersection_Point, Ray -> Initial);
         Local_Element.Point = Intersection_Point;
         Local_Element.Shape = (SHAPE *)Shape;
         pq_add (Depth_Queue, &Local_Element);
         return (TRUE);
      }

   return (FALSE);
}

int Intersect_Plane (Ray, Plane, Depth)
RAY *Ray;
PLANE *Plane;
DBL *Depth;
{
   DBL NormalDotOrigin, NormalDotDirection;

   Ray_Plane_Tests++;
   if (Ray == VP_Ray) {
      if (!Plane->VPCached) {
         VDot (Plane->VPNormDotOrigin, Plane->Normal_Vector, Ray->Initial);
         Plane->VPNormDotOrigin += Plane->Distance;
         Plane->VPNormDotOrigin *= -1.0;
         Plane->VPCached = TRUE;
      }

      VDot (NormalDotDirection, Plane->Normal_Vector, Ray->Direction);
      if ((NormalDotDirection < Small_Tolerance) &&
         (NormalDotDirection > -Small_Tolerance))
         return (FALSE);

      *Depth = Plane->VPNormDotOrigin / NormalDotDirection;
      if ((*Depth >= Small_Tolerance) && (*Depth <= Max_Distance)) {
         Ray_Plane_Tests_Succeeded++;
         return (TRUE);
      }
      else
         return (FALSE);
   }
   else {
      VDot (NormalDotOrigin, Plane->Normal_Vector, Ray->Initial);
      NormalDotOrigin += Plane->Distance;
      NormalDotOrigin *= -1.0;

         VDot (NormalDotDirection, Plane->Normal_Vector, Ray->Direction);
      if ((NormalDotDirection < Small_Tolerance) &&
         (NormalDotDirection > -Small_Tolerance))
         return (FALSE);

      *Depth = NormalDotOrigin / NormalDotDirection;
      if ((*Depth >= Small_Tolerance) && (*Depth <= Max_Distance)) {
         Ray_Plane_Tests_Succeeded++;
         return (TRUE);
      }
      else
         return (FALSE);
   }
}

int Inside_Plane (Test_Point, Object)
VECTOR *Test_Point;
OBJECT *Object;
{
   PLANE *Plane = (PLANE *) Object;
   DBL Temp;

   VDot (Temp, *Test_Point, Plane->Normal_Vector);
   return ((Temp + Plane->Distance) <= Small_Tolerance);
}

void Plane_Normal (Result, Object, Intersection_Point)
OBJECT *Object;
VECTOR *Result, *Intersection_Point;
{
   PLANE *Plane = (PLANE *) Object;

   *Result = Plane->Normal_Vector;
}

void *Copy_Plane (Object)
OBJECT *Object;
{
   PLANE *New_Shape;

   New_Shape = Get_Plane_Shape ();
   *New_Shape = * ((PLANE *)Object);
   New_Shape -> Next_Object = NULL;

   if (New_Shape->Shape_Texture != NULL)
      New_Shape->Shape_Texture = Copy_Texture (New_Shape->Shape_Texture);

   return (New_Shape);
}

void Translate_Plane (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   PLANE *Plane = (PLANE *) Object;
   VECTOR Translation;

   VEvaluate (Translation, Plane->Normal_Vector, *Vector);
   Plane->Distance -= Translation.x + Translation.y + Translation.z;

   Translate_Texture (&Plane->Shape_Texture, Vector);
}

void Rotate_Plane (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transformation;

   Get_Rotation_Transformation (&Transformation, Vector);
   MTransformVector (&((PLANE *) Object)->Normal_Vector,
      &((PLANE *) Object)->Normal_Vector, &Transformation);

   Rotate_Texture (&((PLANE *) Object)->Shape_Texture, Vector);
}

void Scale_Plane (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   DBL Length;
   PLANE *Plane = (PLANE  *) Object;

   Plane->Normal_Vector.x = Plane->Normal_Vector.x / Vector->x;
   Plane->Normal_Vector.y = Plane->Normal_Vector.y / Vector->y;
   Plane->Normal_Vector.z = Plane->Normal_Vector.z / Vector->z;

   VLength(Length, Plane->Normal_Vector);
   VScale (Plane->Normal_Vector, Plane->Normal_Vector, 1.0 / Length);
   Plane->Distance /= Length;

   Scale_Texture (&((PLANE *) Object)->Shape_Texture, Vector);
}

void Invert_Plane (Object)
OBJECT *Object;
{
   PLANE *Plane = (PLANE  *) Object;

   VScale (Plane->Normal_Vector, Plane->Normal_Vector, -1.0);
   Plane->Distance *= -1.0;
}
