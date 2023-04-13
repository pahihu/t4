/****************************************************************************
*                boxes.c
*
*  This module implements the box primitive.
*
*  This file was written by Alexander Enzmann.	He wrote the code for
*  boxes and generously provided us these enhancements.
*
*  from Persistence of Vision Raytracer 
*  Copyright 1991 Persistence of Vision Team
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

METHODS Box_Methods =
{ Object_Intersect, All_Box_Intersections,
   Inside_Box, Box_Normal,
   Copy_Box,
   Translate_Box, Rotate_Box,
   Scale_Box, Invert_Box};

extern BOX *Get_Box_Shape();

extern RAY *VP_Ray;
extern long Ray_Box_Tests, Ray_Box_Tests_Succeeded;

#define close(x, y) (fabs(x-y) < EPSILON ? 1 : 0)

int All_Box_Intersections (Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;
PRIOQ *Depth_Queue;
{
   DBL Depth1, Depth2;
   VECTOR Intersection_Point;
   INTERSECTION Local_Element;
   register int Intersection_Found;
   BOX *Shape = (BOX *) Object;

   Intersection_Found = FALSE;
   if (Intersect_Boxx (Ray, Shape, &Depth1, &Depth2))
   {
      Local_Element.Depth = Depth1;
      Local_Element.Object = Shape -> Parent_Object;
      VScale (Intersection_Point, Ray -> Direction, Depth1);
      VAdd (Intersection_Point, Intersection_Point, Ray -> Initial);
      Local_Element.Point = Intersection_Point;
      Local_Element.Shape = (SHAPE *)Shape;
      pq_add (Depth_Queue, &Local_Element);
      Intersection_Found = TRUE;

      if (Depth2 != Depth1)
      {
         Local_Element.Depth = Depth2;
         Local_Element.Object = Shape -> Parent_Object;
         VScale (Intersection_Point, Ray -> Direction, Depth2);
         VAdd (Intersection_Point, Intersection_Point, Ray -> Initial);
         Local_Element.Point = Intersection_Point;
         Local_Element.Shape = (SHAPE *) Shape;
         pq_add (Depth_Queue, &Local_Element);
         Intersection_Found = TRUE;
      }
   }
   return (Intersection_Found);
}

int Intersect_Boxx (Ray, box, Depth1, Depth2)
RAY *Ray;
BOX *box;
DBL *Depth1, *Depth2;
{
   DBL t, tmin, tmax;
   VECTOR P, D;

   Ray_Box_Tests++;

   /* Transform the point into the boxes space */
   if (box->Transform != NULL) {
      MInverseTransformVector(&P, &Ray->Initial, box->Transform);
      MInvTransVector(&D, &Ray->Direction, box->Transform);
   }
   else {
      P.x = Ray->Initial.x;
      P.y = Ray->Initial.y;
      P.z = Ray->Initial.z;
      D.x = Ray->Direction.x;
      D.y = Ray->Direction.y;
      D.z = Ray->Direction.z;
   }

      tmin = 0.0;
   tmax = HUGE_VAL;

   /* Sides first */
   if (D.x < -EPSILON) {
      t = (box->bounds[0].x - P.x) / D.x;
      if (t < tmin)
         return 0;
      if (t <= tmax)
         tmax = t;
      t = (box->bounds[1].x - P.x) / D.x;
      if (t >= tmin) {
         if (t > tmax)
            return 0;
         tmin = t;
      }
   }
   else if (D.x > EPSILON) {
      t = (box->bounds[1].x - P.x) / D.x;
      if (t < tmin)
         return 0;
      if (t <= tmax)
         tmax = t;
      t = (box->bounds[0].x - P.x) / D.x;
      if (t >= tmin) {
         if (t > tmax)
            return 0;
         tmin = t;
      }
   }
   else if (P.x < box->bounds[0].x || P.x > box->bounds[1].x)
      return 0;

   /* Check Top/Bottom */
   if (D.y < -EPSILON) {
      t = (box->bounds[0].y - P.y) / D.y;
      if (t < tmin)
         return 0;
      if (t <= tmax)
         tmax = t;
      t = (box->bounds[1].y - P.y) / D.y;
      if (t >= tmin) {
         if (t > tmax)
            return 0;
         tmin = t;
      }
   }
   else if (D.y > EPSILON) {
      t = (box->bounds[1].y - P.y) / D.y;
      if (t < tmin)
         return 0;
      if (t <= tmax)
         tmax = t;
      t = (box->bounds[0].y - P.y) / D.y;
      if (t >= tmin) {
         if (t > tmax)
            return 0;
         tmin = t;
      }
   }
   else if (P.y < box->bounds[0].y || P.y > box->bounds[1].y)
      return 0;

   /* Now front/back */
   if (D.z < -EPSILON) {
      t = (box->bounds[0].z - P.z) / D.z;
      if (t < tmin)
         return 0;
      if (t <= tmax)
         tmax = t;
      t = (box->bounds[1].z - P.z) / D.z;
      if (t >= tmin) {
         if (t > tmax)
            return 0;
         tmin = t;
      }
   }
   else if (D.z > EPSILON) {
      t = (box->bounds[1].z - P.z) / D.z;
      if (t < tmin)
         return 0;
      if (t <= tmax)
         tmax = t;
      t = (box->bounds[0].z - P.z) / D.z;
      if (t >= tmin) {
         if (t > tmax)
            return 0;
         tmin = t;
      }
   }
   else if (P.z < box->bounds[0].z || P.z > box->bounds[1].z)
      return 0;

   *Depth1 = tmin;
   *Depth2 = tmax;

   /* printf("Box intersects: %g, %g\n", *Depth1, *Depth2); */
   if ((*Depth1 < Small_Tolerance) || (*Depth1 > Max_Distance))
      if ((*Depth2 < Small_Tolerance) || (*Depth2 > Max_Distance))
         return (FALSE);
      else
         *Depth1 = *Depth2;
   else
      if ((*Depth2 < Small_Tolerance) || (*Depth2 > Max_Distance))
         *Depth2 = *Depth1;

   Ray_Box_Tests_Succeeded++;
   return (TRUE);
}

int Inside_Box (Test_Point, Object)
VECTOR *Test_Point;
OBJECT *Object;
{
   VECTOR New_Point;
   BOX *box = (BOX *) Object;

   /* Transform the point into the boxes space */
   if (box->Transform != NULL)
      MInverseTransformVector(&New_Point, Test_Point, box->Transform);
   else
      New_Point = *Test_Point;

   /* Test to see if we are inside the box */
   if (New_Point.x < box->bounds[0].x || New_Point.x > box->bounds[1].x)
      return ((int) box->Inverted);
   if (New_Point.y < box->bounds[0].y || New_Point.y > box->bounds[1].y)
      return ((int) box->Inverted);
   if (New_Point.z < box->bounds[0].z || New_Point.z > box->bounds[1].z)
      return ((int)box->Inverted);
   /* Inside the box */
   return 1-box->Inverted;
}

void Box_Normal (Result, Object, Intersection_Point)
OBJECT *Object;
VECTOR *Result, *Intersection_Point;
{
   VECTOR New_Point;
   BOX *box = (BOX *) Object;

   /* Transform the point into the boxes space */
   if (box->Transform != NULL)
      MInverseTransformVector(&New_Point, Intersection_Point, box->Transform);
   else {
      New_Point.x = Intersection_Point->x;
      New_Point.y = Intersection_Point->y;
      New_Point.z = Intersection_Point->z;
   }

      Result->x = 0.0; Result->y = 0.0; Result->z = 0.0;
   if (close(New_Point.x, box->bounds[1].x))
      Result->x = 1.0;
   else if (close(New_Point.x, box->bounds[0].x))
      Result->x = -1.0;
   else if (close(New_Point.y, box->bounds[1].y))
      Result->y = 1.0;
   else if (close(New_Point.y, box->bounds[0].y))
      Result->y = -1.0;
   else if (close(New_Point.z, box->bounds[1].z))
      Result->z = 1.0;
   else if (close(New_Point.z, box->bounds[0].z))
      Result->z = -1.0;
   else {
      /* Bad result, should we do something with it? */
         Result->x = 1.0;
   }

   /* Transform the point into the boxes space */
   if (box->Transform != NULL) {
      MTransNormal(Result, Result, box->Transform);
      VNormalize(*Result, *Result);
   }
}

void *Copy_Box (Object)
OBJECT *Object;
{
   BOX *New_Shape;
   TRANSFORMATION *Tr;

   New_Shape = Get_Box_Shape ();
   *New_Shape = *((BOX *) Object);
   New_Shape -> Next_Object = NULL;

   /* Copy any associated transformation */
   if (New_Shape->Transform != NULL) {
      Tr = Get_Transformation();
      memcpy(Tr, New_Shape->Transform, sizeof(TRANSFORMATION));
      New_Shape->Transform = Tr;
   }

   if (New_Shape->Shape_Texture != NULL)
      New_Shape->Shape_Texture = Copy_Texture (New_Shape->Shape_Texture);

   return (New_Shape);
}

void Translate_Box (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transform;
   BOX *box = (BOX *)Object;
   if (box->Transform == NULL)
      box->Transform = Get_Transformation();
   Get_Translation_Transformation(&Transform, Vector);
   Compose_Transformations(box->Transform, &Transform);

   Translate_Texture (&((BOX *) Object)->Shape_Texture, Vector);
}

void Rotate_Box (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transform;
   BOX *box = (BOX *)Object;
   if (box->Transform == NULL)
      box->Transform = Get_Transformation();
   Get_Rotation_Transformation(&Transform, Vector);
   Compose_Transformations(box->Transform, &Transform);

   Rotate_Texture (&((BOX *) Object)->Shape_Texture, Vector);
}

void Scale_Box (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transform;
   BOX *box = (BOX *)Object;
   if (box->Transform == NULL)
      box->Transform = Get_Transformation();
   Get_Scaling_Transformation(&Transform, Vector);
   Compose_Transformations(box->Transform, &Transform);

   Scale_Texture (&((BOX *) Object)->Shape_Texture, Vector);
}


void Invert_Box (Object)
OBJECT *Object;
{
   ((BOX *) Object)->Inverted = 1 - ((BOX *)Object)->Inverted;
}
