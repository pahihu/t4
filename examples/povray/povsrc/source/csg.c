/****************************************************************************
*                   csg.c
*
*  This module implements routines for constructive solid geometry.
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

METHODS CSG_Union_Methods =
{ Object_Intersect, All_CSG_Union_Intersections,
   Inside_CSG_Union, NULL,
   Copy_CSG,
   Translate_CSG, Rotate_CSG,
   Scale_CSG, Invert_CSG};

METHODS CSG_Intersection_Methods =
{ Object_Intersect, All_CSG_Intersect_Intersections,
   Inside_CSG_Intersection, NULL,
   Copy_CSG,
   Translate_CSG, Rotate_CSG,
   Scale_CSG, Invert_CSG};

extern RAY *VP_Ray;

int All_CSG_Union_Intersections (Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;
PRIOQ *Depth_Queue;
{
   register int Intersection_Found;
   CSG_SHAPE *Shape = (CSG_SHAPE *) Object;
   SHAPE *Local_Shape;

   Intersection_Found = FALSE;
   for (Local_Shape = Shape -> Shapes;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)
      if (All_Intersections ((OBJECT *) Local_Shape, Ray, Depth_Queue))
         Intersection_Found = TRUE;

   return (Intersection_Found);
}

int All_CSG_Intersect_Intersections (Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;
PRIOQ *Depth_Queue;
{
   int Intersection_Found, Any_Intersection_Found;
   CSG_SHAPE *Shape = (CSG_SHAPE *) Object;
   SHAPE *Local_Shape, *Shape2;
   PRIOQ *Local_Depth_Queue;
   INTERSECTION *Local_Intersection;

   Local_Depth_Queue = pq_new (128);

   Any_Intersection_Found = FALSE;

   for (Local_Shape = Shape -> Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object) {

      All_Intersections ((OBJECT *) Local_Shape, Ray, Local_Depth_Queue);

      for (Local_Intersection = pq_get_highest (Local_Depth_Queue);
                    Local_Intersection != NULL ;
                    pq_delete_highest (Local_Depth_Queue),
                    Local_Intersection = pq_get_highest (Local_Depth_Queue)) {

         Intersection_Found = TRUE;

         for (Shape2 = Shape -> Shapes;
                          Shape2 != NULL ;
                          Shape2 = Shape2 -> Next_Object)

            if (Shape2 != Local_Shape)
               if (!Inside (&Local_Intersection->Point, (OBJECT *) Shape2)) {
                  Intersection_Found = FALSE;
                  break;
               }

         if (Intersection_Found) {
            pq_add (Depth_Queue, Local_Intersection);
            Any_Intersection_Found = TRUE;
         }
      }
   }

   pq_free (Local_Depth_Queue);

   return (Any_Intersection_Found);
}

int Inside_CSG_Union (Test_Point, Object)
VECTOR *Test_Point;
OBJECT *Object;
{
   CSG_SHAPE *Shape = (CSG_SHAPE *) Object;
   SHAPE *Local_Shape;

   for (Local_Shape = Shape -> Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      if (Inside (Test_Point, (OBJECT *) Local_Shape))
         return (TRUE);
   return (FALSE);
}

int Inside_CSG_Intersection (Test_Point, Object)
OBJECT *Object;
VECTOR *Test_Point;
{
   SHAPE *Local_Shape;
   CSG_SHAPE *Shape = (CSG_SHAPE *) Object;

   for (Local_Shape = Shape -> Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      if (!Inside (Test_Point, (OBJECT *) Local_Shape))
         return (FALSE);

   return (TRUE);
}

void *Copy_CSG (Object)
OBJECT *Object;
{
   CSG_SHAPE *Shape = (CSG_SHAPE *) Object;
   CSG_SHAPE *New_Shape;
   SHAPE *Local_Shape, *Copied_Shape;

   New_Shape = Get_CSG_Shape ();
   New_Shape->Methods = Shape->Methods;
   New_Shape->Type = Shape->Type;
   New_Shape -> Next_Object = NULL;
   New_Shape -> Shapes = NULL;

   for (Local_Shape = Shape -> Shapes;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object) {

      Copied_Shape = (SHAPE *) Copy ((OBJECT *) Local_Shape);
      Link ((OBJECT *) Copied_Shape,
         (OBJECT **) &(Copied_Shape -> Next_Object),
         (OBJECT **) &(New_Shape -> Shapes));
   }
   return ((void *)New_Shape);
}

void Translate_CSG (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   SHAPE *Local_Shape;

   for (Local_Shape = ((CSG_SHAPE *) Object) -> Shapes;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Translate ((OBJECT *) Local_Shape, Vector);   
}

void Rotate_CSG (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   SHAPE *Local_Shape;

   for (Local_Shape = ((CSG_SHAPE *) Object) -> Shapes;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Rotate ((OBJECT *) Local_Shape, Vector);   
}

void Scale_CSG (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   SHAPE *Local_Shape;

   for (Local_Shape = ((CSG_SHAPE *) Object) -> Shapes;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Scale ((OBJECT *) Local_Shape, Vector);   
}

void Invert_CSG (Object)
OBJECT *Object;
{
   SHAPE *Local_Shape;
   CSG_SHAPE *Csg = (CSG_SHAPE *) Object;

   if (Csg->Type == CSG_INTERSECTION_TYPE) {
      Csg->Type = CSG_UNION_TYPE;
      Csg->Methods = &CSG_Union_Methods;
   }
   else if (Csg->Type == CSG_UNION_TYPE) {
      Csg->Type = CSG_INTERSECTION_TYPE;
      Csg->Methods = &CSG_Intersection_Methods;
   }

   for (Local_Shape = Csg -> Shapes;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Invert ((OBJECT *) Local_Shape);   
}

void Set_CSG_Parents (Shape, Object)
CSG_SHAPE *Shape;
OBJECT *Object;
{
   SHAPE *Local_Shape;

   for (Local_Shape = Shape -> Shapes;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object) {

      Local_Shape->Parent_Object = Object;
      if ((Local_Shape->Type == CSG_UNION_TYPE) ||
         (Local_Shape->Type == CSG_INTERSECTION_TYPE))
         Set_CSG_Parents((CSG_SHAPE *)Local_Shape, Object);
   }
}
