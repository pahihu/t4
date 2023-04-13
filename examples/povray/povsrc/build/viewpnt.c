/****************************************************************************
*                viewpnt.c
*
*  This module implements methods for managing the viewpoint.
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

METHODS Viewpoint_Methods =
{ NULL, NULL, NULL, NULL, Copy_Viewpoint,
   Translate_Viewpoint, Rotate_Viewpoint,
   Scale_Viewpoint, NULL};

void *Copy_Viewpoint (Object)
OBJECT *Object;
{
   VIEWPOINT *Viewpoint = (VIEWPOINT *) Object;
   VIEWPOINT *New_Viewpoint;

   New_Viewpoint = Get_Viewpoint();

   New_Viewpoint -> Location = Viewpoint -> Location;
   New_Viewpoint -> Direction = Viewpoint -> Direction;
   New_Viewpoint -> Right = Viewpoint -> Right;
   New_Viewpoint -> Up = Viewpoint -> Up;
   return (New_Viewpoint);
}

void Translate_Viewpoint (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   VAdd (((VIEWPOINT *) Object) -> Location, 
      ((VIEWPOINT *) Object) -> Location,
      *Vector);
}

void Rotate_Viewpoint (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transformation;
   VIEWPOINT *Viewpoint = (VIEWPOINT *) Object;

   Get_Rotation_Transformation(&Transformation, Vector);
   MTransformVector (&(Viewpoint -> Location),
      &(Viewpoint -> Location), &Transformation);

   MTransformVector (&(Viewpoint -> Direction),
      &(Viewpoint -> Direction), &Transformation);

   MTransformVector (&(Viewpoint -> Up),
      &(Viewpoint -> Up), &Transformation);

   MTransformVector (&(Viewpoint -> Right),
      &(Viewpoint -> Right), &Transformation);
}

void Scale_Viewpoint (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transformation;
   VIEWPOINT *Viewpoint = (VIEWPOINT *) Object;

   Get_Scaling_Transformation(&Transformation, Vector);
   MTransformVector (&(Viewpoint -> Location),
      &(Viewpoint -> Location), &Transformation);

   MTransformVector (&(Viewpoint -> Direction),
      &(Viewpoint -> Direction), &Transformation);

   MTransformVector (&(Viewpoint -> Up),
      &(Viewpoint -> Up), &Transformation);

   MTransformVector (&(Viewpoint -> Right),
      &(Viewpoint -> Right), &Transformation);
}
