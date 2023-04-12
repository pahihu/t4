/****************************************************************************
*                objects.c
*
*  This module implements the methods for objects and composite objects.
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

extern RAY *VP_Ray;
extern long Bounding_Region_Tests, Bounding_Region_Tests_Succeeded;
extern long Clipping_Region_Tests, Clipping_Region_Tests_Succeeded;
extern unsigned int Options;

METHODS Composite_Methods =
{ Object_Intersect, All_Composite_Intersections,
   Inside_Composite_Object, NULL,
   Copy_Composite_Object,
   Translate_Composite_Object, Rotate_Composite_Object,
   Scale_Composite_Object, Invert_Composite_Object};

METHODS Basic_Object_Methods =
{ Object_Intersect, All_Object_Intersections,
   Inside_Basic_Object, NULL,
   Copy_Basic_Object,
   Translate_Basic_Object, Rotate_Basic_Object,
   Scale_Basic_Object, Invert_Basic_Object};


INTERSECTION *Object_Intersect (Object, Ray)
OBJECT *Object;
RAY *Ray;
{
   INTERSECTION *Local_Intersection, *Queue_Element;
   PRIOQ *Depth_Queue;

   Depth_Queue = pq_new (128);

   if ((All_Intersections (Object, Ray, Depth_Queue))
      && ((Queue_Element = pq_get_highest (Depth_Queue)) != NULL))
   {
      if ((Local_Intersection = (INTERSECTION *) malloc(sizeof(INTERSECTION)))
         == NULL) {
         printf("Cannot allocate memory for local intersection\n");
         exit(1);
      }
      Local_Intersection->Point = Queue_Element->Point;
      Local_Intersection->Shape = Queue_Element->Shape;
      Local_Intersection->Depth = Queue_Element->Depth;
      Local_Intersection->Object = Queue_Element->Object;
      pq_free (Depth_Queue);
      return (Local_Intersection);
   }
   else
   {
      pq_free (Depth_Queue);
      return (NULL);
   }
}


int All_Composite_Intersections (Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;
PRIOQ *Depth_Queue;
{
   register int Intersection_Found, Any_Intersection_Found;
   SHAPE *Bounding_Shape;
   SHAPE *Clipping_Shape;
   INTERSECTION *Local_Intersection;
   OBJECT *Local_Object;
   PRIOQ *Local_Depth_Queue;

   for (Bounding_Shape = ((COMPOSITE *) Object) -> Bounding_Shapes ;
              Bounding_Shape != NULL ;
              Bounding_Shape = Bounding_Shape -> Next_Object) {

      Bounding_Region_Tests++;
      COOPERATE
      if ((Local_Intersection = Intersection ((OBJECT *) Bounding_Shape, Ray)) != NULL)
         free (Local_Intersection);
      else
         if (!Inside (&Ray -> Initial, (OBJECT *) Bounding_Shape))
            return (FALSE);
      Bounding_Region_Tests_Succeeded++;
   }

   Local_Depth_Queue = pq_new (128);
   Any_Intersection_Found = FALSE;

   for (Local_Object = ((COMPOSITE *) Object) -> Objects ;
              Local_Object != NULL ;
              Local_Object = Local_Object -> Next_Object)

      All_Intersections (Local_Object, Ray, Local_Depth_Queue);

   for (Local_Intersection = pq_get_highest (Local_Depth_Queue);
      	Local_Intersection != NULL ;
      	pq_delete_highest (Local_Depth_Queue),
      	Local_Intersection = pq_get_highest (Local_Depth_Queue)) {

      Intersection_Found = TRUE;

      for (Clipping_Shape = Object -> Clipping_Shapes ;
         	        Clipping_Shape != NULL ;
         	        Clipping_Shape = Clipping_Shape -> Next_Object) {
         Clipping_Region_Tests++;
         if (!Inside (&Local_Intersection->Point, (OBJECT *) Clipping_Shape)) {
            Intersection_Found = FALSE;
            break;
         }
         Clipping_Region_Tests_Succeeded++;
      }

      if (Intersection_Found) {
         pq_add (Depth_Queue, Local_Intersection);
         Any_Intersection_Found = TRUE;
      }
   }
   pq_free (Local_Depth_Queue);
   return (Any_Intersection_Found);
}


int All_Object_Intersections (Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;
PRIOQ *Depth_Queue;
{
   int Intersection_Found, Any_Intersection_Found;
   INTERSECTION *Local_Intersection;
   SHAPE *Bounding_Shape;
   SHAPE *Clipping_Shape;
   PRIOQ *Local_Depth_Queue;

   for (Bounding_Shape = Object -> Bounding_Shapes ;
              Bounding_Shape != NULL ;
              Bounding_Shape = Bounding_Shape -> Next_Object) {

      Bounding_Region_Tests++;
      COOPERATE
      if ((Local_Intersection = Intersection ((OBJECT *) Bounding_Shape, Ray)) != NULL)
         free (Local_Intersection);
      else
         if (!Inside (&Ray -> Initial, (OBJECT *) Bounding_Shape))
            return (FALSE);
      Bounding_Region_Tests_Succeeded++;
   }

   Local_Depth_Queue = pq_new (128);
   Any_Intersection_Found = FALSE;
   All_Intersections ((OBJECT *)Object->Shape, Ray, Local_Depth_Queue);

   for (Local_Intersection = pq_get_highest (Local_Depth_Queue);
         	Local_Intersection != NULL ;
      	  pq_delete_highest (Local_Depth_Queue),
      	  Local_Intersection = pq_get_highest (Local_Depth_Queue)) {

      Intersection_Found = TRUE;

      for (Clipping_Shape = Object -> Clipping_Shapes ;
         	   Clipping_Shape != NULL ;
         	   Clipping_Shape = Clipping_Shape -> Next_Object) {

         Clipping_Region_Tests++;
         if (Options & DEBUGGING) {
            printf("Test (%.4lf, %.4lf, %.4lf)\n",
               Local_Intersection->Point.x,
               Local_Intersection->Point.y,
               Local_Intersection->Point.z);
         }
         if (!Inside (&Local_Intersection->Point, (OBJECT *) Clipping_Shape)) {
            if (Options & DEBUGGING)
               printf("not ok\n");
            Intersection_Found = FALSE;
            break;
         }
         Clipping_Region_Tests_Succeeded++; 
      }

      if (Intersection_Found) {
         if (Options & DEBUGGING)
            printf("ok\n");
         pq_add (Depth_Queue, Local_Intersection);
         Any_Intersection_Found = TRUE;

      }
   }
   pq_free (Local_Depth_Queue);
   return (Any_Intersection_Found);
}


int Inside_Basic_Object (Test_Point, Object)
VECTOR *Test_Point;
OBJECT *Object;
{
   SHAPE *Bounding_Shape;
   SHAPE *Clipping_Shape;

   for (Bounding_Shape = Object -> Bounding_Shapes ;
              Bounding_Shape != NULL ;
              Bounding_Shape = Bounding_Shape -> Next_Object)

      if (!Inside (Test_Point, (OBJECT *) Bounding_Shape))
         return (FALSE);

   for (Clipping_Shape = Object -> Clipping_Shapes ;
               Clipping_Shape != NULL ;
               Clipping_Shape = Clipping_Shape -> Next_Object)

      if (!Inside (Test_Point, (OBJECT *) Clipping_Shape))
         return (FALSE);

   if (Inside (Test_Point, (OBJECT *) Object -> Shape))
      return (TRUE);
   return (FALSE);
}

int Inside_Composite_Object (Test_Point, Object)
VECTOR *Test_Point;
OBJECT *Object;
{
   SHAPE *Bounding_Shape;
   SHAPE *Clipping_Shape;
   OBJECT *Local_Object;

   for (Bounding_Shape = ((COMPOSITE *) Object) -> Bounding_Shapes ;
              Bounding_Shape != NULL ;
              Bounding_Shape = Bounding_Shape -> Next_Object)

      if (!Inside (Test_Point, (OBJECT *) Bounding_Shape))
         return (FALSE);

   for (Clipping_Shape = ((COMPOSITE *) Object) -> Clipping_Shapes ;
              Clipping_Shape != NULL ;
              Clipping_Shape = Clipping_Shape -> Next_Object)

      if (!Inside (Test_Point, (OBJECT *) Clipping_Shape))
         return (FALSE);

   for (Local_Object = ((COMPOSITE *) Object) -> Objects ;
              Local_Object != NULL ;
              Local_Object = Local_Object -> Next_Object)

      if (Inside (Test_Point, Local_Object))
         return (TRUE);

   return (FALSE);
}

void *Copy_Basic_Object (Object)
OBJECT *Object;
{
   SHAPE *Local_Shape, *Copied_Shape;
   OBJECT *New_Object;

   New_Object = Get_Object();
   *New_Object = *Object;
   New_Object -> Next_Object = NULL;
   New_Object -> Bounding_Shapes = NULL;
   New_Object -> Clipping_Shapes = NULL;
   for (Local_Shape = Object -> Bounding_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object) {

      Copied_Shape = (SHAPE *) Copy((OBJECT *) Local_Shape);
      Link ((OBJECT *) Copied_Shape,
         (OBJECT **) &(Copied_Shape -> Next_Object),
         (OBJECT **) &(New_Object -> Bounding_Shapes));

      if ((Copied_Shape->Type == CSG_UNION_TYPE)
         || (Copied_Shape->Type == CSG_INTERSECTION_TYPE)
         || (Copied_Shape->Type == CSG_DIFFERENCE_TYPE))
         Set_CSG_Parents ((CSG_SHAPE *) Copied_Shape, New_Object);
   }

   for (Local_Shape = Object -> Clipping_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object) {

      Copied_Shape = (SHAPE *) Copy((OBJECT *) Local_Shape);
      Link ((OBJECT *) Copied_Shape,
         (OBJECT **) &(Copied_Shape -> Next_Object),
         (OBJECT **) &(New_Object -> Clipping_Shapes));

      if ((Copied_Shape->Type == CSG_UNION_TYPE)
         || (Copied_Shape->Type == CSG_INTERSECTION_TYPE)
         || (Copied_Shape->Type == CSG_DIFFERENCE_TYPE))
         Set_CSG_Parents ((CSG_SHAPE *) Copied_Shape, New_Object);
   }

   New_Object -> Shape = (SHAPE *) Copy((OBJECT *) Object -> Shape);
   if ((New_Object->Shape->Type == CSG_UNION_TYPE)
      || (New_Object->Shape->Type == CSG_INTERSECTION_TYPE)
      || (New_Object->Shape->Type == CSG_DIFFERENCE_TYPE))
      Set_CSG_Parents ((CSG_SHAPE *) New_Object->Shape, New_Object);
   else
      New_Object->Shape->Parent_Object = New_Object;


   if (New_Object->Object_Texture != NULL)
      New_Object->Object_Texture = Copy_Texture (New_Object->Object_Texture);

   return ((void *)New_Object);
}

void *Copy_Composite_Object (Object)
OBJECT *Object;
{
   COMPOSITE *New_Object;
   SHAPE *Local_Shape;
   OBJECT *Local_Object, *Copied_Object;

   New_Object = Get_Composite_Object();
   *New_Object = *((COMPOSITE *) Object);
   New_Object -> Next_Object = NULL;
   New_Object -> Objects = NULL;
   for (Local_Object = ((COMPOSITE *) Object) -> Objects;
              Local_Object != NULL ;
              Local_Object = Local_Object -> Next_Object) {

      Copied_Object = (OBJECT *) Copy(Local_Object);
      Link (Copied_Object,
         &(Copied_Object -> Next_Object),
         &(New_Object -> Objects));
   }

   New_Object -> Bounding_Shapes = NULL;
   for (Local_Shape = ((COMPOSITE *) Object) -> Bounding_Shapes;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object) {

      Copied_Object = (OBJECT *) Copy((OBJECT *) Local_Shape);
      Link (Copied_Object,
         &(Copied_Object -> Next_Object),
         (OBJECT **) &(New_Object -> Bounding_Shapes));
   }
   New_Object -> Clipping_Shapes = NULL;
   for (Local_Shape = ((COMPOSITE *) Object) -> Clipping_Shapes;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object) {

      Copied_Object = (OBJECT *) Copy((OBJECT *) Local_Shape);
      Link (Copied_Object,
         &(Copied_Object -> Next_Object),
         (OBJECT **) &(New_Object -> Clipping_Shapes));
   }
   return ((void *)New_Object);
}

void Translate_Basic_Object (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   SHAPE *Local_Shape;

   for (Local_Shape = Object -> Bounding_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Translate ((OBJECT *) Local_Shape, Vector);

   for (Local_Shape = Object -> Clipping_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Translate ((OBJECT *) Local_Shape, Vector);

   Translate ((OBJECT *) Object -> Shape, Vector);

   Translate_Texture (&Object->Object_Texture, Vector);
}

void Rotate_Basic_Object (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   SHAPE *Local_Shape;
   TRANSFORMATION Transformation;

   for (Local_Shape = Object -> Bounding_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Rotate ((OBJECT *) Local_Shape, Vector);

   for (Local_Shape = Object -> Clipping_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Rotate ((OBJECT *) Local_Shape, Vector);

   Rotate ((OBJECT *) Object -> Shape, Vector);
   Get_Rotation_Transformation (&Transformation, Vector);

   Rotate_Texture (&Object->Object_Texture, Vector);
}

void Scale_Basic_Object (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   SHAPE *Local_Shape;

   for (Local_Shape = Object -> Bounding_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Scale ((OBJECT *) Local_Shape, Vector);

   for (Local_Shape = Object -> Clipping_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Scale ((OBJECT *) Local_Shape, Vector);

   Scale ((OBJECT *) Object -> Shape, Vector);

   Scale_Texture (&Object->Object_Texture, Vector);
}

void Translate_Composite_Object (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   OBJECT *Local_Object;
   SHAPE *Local_Shape;

   for (Local_Object = ((COMPOSITE *) Object) -> Objects;
              Local_Object != NULL ;
              Local_Object = Local_Object -> Next_Object)

      Translate (Local_Object, Vector);   

   for (Local_Shape = ((COMPOSITE *) Object) -> Bounding_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Translate ((OBJECT *) Local_Shape, Vector);

   for (Local_Shape = ((COMPOSITE *) Object) -> Clipping_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Translate ((OBJECT *) Local_Shape, Vector);
}

void Rotate_Composite_Object (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   OBJECT *Local_Object;
   SHAPE *Local_Shape;

   for (Local_Object = ((COMPOSITE *) Object) -> Objects;
              Local_Object != NULL ;
              Local_Object = Local_Object -> Next_Object)

      Rotate (Local_Object, Vector);   

   for (Local_Shape = ((COMPOSITE *) Object) -> Bounding_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Rotate ((OBJECT *) Local_Shape, Vector);

   for (Local_Shape = ((COMPOSITE *) Object) -> Clipping_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Rotate ((OBJECT *) Local_Shape, Vector);
}

void Scale_Composite_Object (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   OBJECT *Local_Object;
   SHAPE *Local_Shape;

   for (Local_Object = ((COMPOSITE *) Object) -> Objects;
              Local_Object != NULL ;
              Local_Object = Local_Object -> Next_Object)

      Scale (Local_Object, Vector);   

   for (Local_Shape = ((COMPOSITE *) Object) -> Bounding_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Scale ((OBJECT *) Local_Shape, Vector);

   for (Local_Shape = ((COMPOSITE *) Object) -> Clipping_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Scale ((OBJECT *) Local_Shape, Vector);
}


void Invert_Basic_Object (Object)
OBJECT *Object;
{
   SHAPE *Local_Shape;

   for (Local_Shape = Object -> Bounding_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Invert ((OBJECT *) Local_Shape);

   for (Local_Shape = Object -> Clipping_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Invert ((OBJECT *) Local_Shape);
   Invert ((OBJECT *) Object -> Shape);
}

void Invert_Composite_Object (Object)
OBJECT *Object;
{
   OBJECT *Local_Object;
   SHAPE *Local_Shape;

   for (Local_Object = ((COMPOSITE *)Object) -> Objects;
              Local_Object != NULL ;
              Local_Object = Local_Object -> Next_Object)

      Invert (Local_Object);   

   for (Local_Shape = ((COMPOSITE *) Object) -> Bounding_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Invert ((OBJECT *) Local_Shape);

   for (Local_Shape = ((COMPOSITE *) Object) -> Clipping_Shapes ;
              Local_Shape != NULL ;
              Local_Shape = Local_Shape -> Next_Object)

      Invert ((OBJECT *) Local_Shape);
}
