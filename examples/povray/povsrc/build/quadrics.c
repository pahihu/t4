/****************************************************************************
*                quadrics.c
*
*  This module implements the code for the quadric shape primitive.
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

METHODS Quadric_Methods =
{ Object_Intersect, All_Quadric_Intersections,
   Inside_Quadric, Quadric_Normal,
   Copy_Quadric,
   Translate_Quadric, Rotate_Quadric,
   Scale_Quadric, Invert_Quadric};

extern RAY *VP_Ray;
extern long Ray_Quadric_Tests, Ray_Quadric_Tests_Succeeded;

int All_Quadric_Intersections (Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;
PRIOQ *Depth_Queue;
{
   QUADRIC *Shape = (QUADRIC *) Object;
   DBL Depth1, Depth2;
   VECTOR Intersection_Point;
   INTERSECTION Local_Element;
   register int Intersection_Found;

   Intersection_Found = FALSE;
   if (Intersect_Quadric (Ray, Shape, &Depth1, &Depth2))
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

int Intersect_Quadric (Ray, Shape, Depth1, Depth2)
RAY *Ray;
QUADRIC *Shape;
DBL *Depth1, *Depth2;
{
   register DBL Square_Term, Linear_Term, Constant_Term, Temp_Term;
   register DBL Determinant, Determinant_2, A2, BMinus;

   Ray_Quadric_Tests++;
   if (!Ray->Quadric_Constants_Cached)
      Make_Ray(Ray);

   if (Shape -> Non_Zero_Square_Term)
   {
      VDot (Square_Term, Shape -> Object_2_Terms, Ray -> Direction_2);
      VDot (Temp_Term, Shape -> Object_Mixed_Terms, Ray -> Mixed_Dir_Dir);
      Square_Term += Temp_Term;
   }
   else
      Square_Term = 0.0;

   VDot (Linear_Term, Shape -> Object_2_Terms, Ray -> Initial_Direction);
   Linear_Term *= 2.0;
   VDot (Temp_Term, Shape -> Object_Terms, Ray -> Direction);
   Linear_Term += Temp_Term;
   VDot (Temp_Term, Shape -> Object_Mixed_Terms, Ray -> Mixed_Init_Dir);
   Linear_Term += Temp_Term;

   if (Ray == VP_Ray)
      if (!Shape -> Constant_Cached)
      {
         VDot (Constant_Term, Shape -> Object_2_Terms, Ray -> Initial_2);
         VDot (Temp_Term, Shape -> Object_Terms, Ray -> Initial);
         Constant_Term +=  Temp_Term + Shape -> Object_Constant;
         Shape -> Object_VP_Constant = Constant_Term;
         Shape -> Constant_Cached = TRUE;
      }
      else
         Constant_Term = Shape -> Object_VP_Constant;
   else
   {
      VDot (Constant_Term, Shape -> Object_2_Terms, Ray -> Initial_2);
      VDot (Temp_Term, Shape -> Object_Terms, Ray -> Initial);
      Constant_Term += Temp_Term + Shape -> Object_Constant;
   }

   VDot (Temp_Term, Shape -> Object_Mixed_Terms, 
      Ray -> Mixed_Initial_Initial);
   Constant_Term += Temp_Term;

   if (Square_Term != 0.0)
   {
      /* The equation is quadratic - find its roots */

      Determinant_2 = Linear_Term * Linear_Term - 4.0 * Square_Term * Constant_Term;

      if (Determinant_2 < 0.0)
         return (FALSE);

      Determinant = sqrt (Determinant_2);
      A2 = Square_Term * 2.0;
      BMinus = Linear_Term * -1.0;

      *Depth1 = (BMinus + Determinant) / A2;
      *Depth2 = (BMinus - Determinant) / A2;
   }
   else
   {
      /* There are no quadratic terms.  Solve the linear equation instead. */
      if (Linear_Term == 0.0)
         return (FALSE);

      *Depth1 = Constant_Term * -1.0 / Linear_Term;
      *Depth2 = *Depth1;
   }

   if ((*Depth1 < Small_Tolerance) || (*Depth1 > Max_Distance))
      if ((*Depth2 < Small_Tolerance) || (*Depth2 > Max_Distance))
         return (FALSE);
      else
         *Depth1 = *Depth2;
   else
      if ((*Depth2 < Small_Tolerance) || (*Depth2 > Max_Distance))
         *Depth2 = *Depth1;

   Ray_Quadric_Tests_Succeeded++;
   return (TRUE);
}

int Inside_Quadric (Test_Point, Object)
VECTOR *Test_Point;
OBJECT *Object;
{
   QUADRIC *Shape = (QUADRIC *) Object;
   VECTOR New_Point;
   register DBL Result, Linear_Term, Square_Term;

   VDot (Linear_Term, *Test_Point, Shape -> Object_Terms);
   Result = Linear_Term + Shape -> Object_Constant;
   VSquareTerms (New_Point, *Test_Point);
   VDot (Square_Term, New_Point, Shape -> Object_2_Terms);
   Result += Square_Term;
   Result += Shape -> Object_Mixed_Terms.x * (Test_Point -> x) * (Test_Point -> y)
      + Shape -> Object_Mixed_Terms.y * (Test_Point -> x) * (Test_Point -> z)
         + Shape -> Object_Mixed_Terms.z * (Test_Point -> y) * (Test_Point -> z);

   if (Result < Small_Tolerance)
      return (TRUE);

   return (FALSE);
}

void Quadric_Normal (Result, Object, Intersection_Point)
VECTOR *Result, *Intersection_Point;
OBJECT *Object;
{
   QUADRIC *Intersection_Shape = (QUADRIC *) Object;
   VECTOR Derivative_Linear;
   DBL Len;

   VScale (Derivative_Linear, Intersection_Shape -> Object_2_Terms, 2.0);
   VEvaluate (*Result, Derivative_Linear, *Intersection_Point);
   VAdd (*Result, *Result, Intersection_Shape -> Object_Terms);

   Result -> x += 
   Intersection_Shape->Object_Mixed_Terms.x * Intersection_Point->y +
   Intersection_Shape->Object_Mixed_Terms.y * Intersection_Point->z;


   Result -> y +=
   Intersection_Shape->Object_Mixed_Terms.x * Intersection_Point->x +
   Intersection_Shape->Object_Mixed_Terms.z * Intersection_Point->z;

   Result -> z +=
   Intersection_Shape->Object_Mixed_Terms.y * Intersection_Point->x +
   Intersection_Shape->Object_Mixed_Terms.z * Intersection_Point->y;

   Len = Result->x * Result->x + Result->y * Result->y + Result->z * Result->z;
   Len = sqrt(Len);
   if (Len == 0.0) {
      /* The normal is not defined at this point of the surface.  Set it
         to any arbitrary direction. */
      Result->x = 1.0;
      Result->y = 0.0;
      Result->z = 0.0;
   }
   else {
      Result->x /= Len;		/* normalize the normal */
      Result->y /= Len;
      Result->z /= Len;
   }
}

   void *Copy_Quadric (Object)
      OBJECT *Object;
{
   QUADRIC *New_Shape;

   New_Shape = Get_Quadric_Shape ();
   *New_Shape = *((QUADRIC *) Object);
   New_Shape -> Next_Object = NULL;

   if (New_Shape->Shape_Texture != NULL)
      New_Shape->Shape_Texture = Copy_Texture (New_Shape->Shape_Texture);

   return (New_Shape);
}

void Transform_Quadric (Shape, Transformation)
QUADRIC *Shape;
TRANSFORMATION *Transformation;
{
   MATRIX Quadric_Matrix, Transform_Transposed;

   Quadric_To_Matrix (Shape, (MATRIX *) &Quadric_Matrix[0][0]);
   MTimes ((MATRIX *) &Quadric_Matrix[0][0], (MATRIX *) &(Transformation -> inverse[0][0]), (MATRIX *) &Quadric_Matrix[0][0]);
   MTranspose ((MATRIX *) &Transform_Transposed[0][0], (MATRIX *) &(Transformation -> inverse[0][0]));
   MTimes ((MATRIX *) &Quadric_Matrix[0][0], (MATRIX *) &Quadric_Matrix[0][0], (MATRIX *) &Transform_Transposed[0][0]);
   Matrix_To_Quadric ((MATRIX *) &Quadric_Matrix[0][0], Shape);
}

void Quadric_To_Matrix (Quadric, Matrix)
QUADRIC *Quadric;
MATRIX *Matrix;
{
   MZero (Matrix);
   (*Matrix)[0][0] = Quadric -> Object_2_Terms.x;
   (*Matrix)[1][1] = Quadric -> Object_2_Terms.y;
   (*Matrix)[2][2] = Quadric -> Object_2_Terms.z;
   (*Matrix)[0][1] = Quadric -> Object_Mixed_Terms.x;
   (*Matrix)[0][2] = Quadric -> Object_Mixed_Terms.y;
   (*Matrix)[0][3] = Quadric -> Object_Terms.x;
   (*Matrix)[1][2] = Quadric -> Object_Mixed_Terms.z;
   (*Matrix)[1][3] = Quadric -> Object_Terms.y;
   (*Matrix)[2][3] = Quadric -> Object_Terms.z;
   (*Matrix)[3][3] = Quadric -> Object_Constant;
}

void Matrix_To_Quadric (Matrix, Quadric)
MATRIX *Matrix;
QUADRIC *Quadric;
{
   Quadric -> Object_2_Terms.x = (*Matrix)[0][0];
   Quadric -> Object_2_Terms.y = (*Matrix)[1][1];
   Quadric -> Object_2_Terms.z = (*Matrix)[2][2];
   Quadric -> Object_Mixed_Terms.x = (*Matrix)[0][1] + (*Matrix)[1][0];
   Quadric -> Object_Mixed_Terms.y = (*Matrix)[0][2] + (*Matrix)[2][0];
   Quadric -> Object_Terms.x = (*Matrix)[0][3] + (*Matrix)[3][0];
   Quadric -> Object_Mixed_Terms.z = (*Matrix)[1][2] + (*Matrix)[2][1];
   Quadric -> Object_Terms.y = (*Matrix)[1][3] + (*Matrix)[3][1];
   Quadric -> Object_Terms.z = (*Matrix)[2][3] + (*Matrix)[3][2];
   Quadric -> Object_Constant = (*Matrix)[3][3];
}

void Translate_Quadric (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transformation;

   Get_Translation_Transformation (&Transformation, Vector);
   Transform_Quadric ((QUADRIC *) Object, &Transformation);

   Translate_Texture (&((QUADRIC *) Object)->Shape_Texture, Vector);
}

void Rotate_Quadric (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transformation;

   Get_Rotation_Transformation (&Transformation, Vector);
   Transform_Quadric ((QUADRIC *) Object, &Transformation);

   Rotate_Texture (&((QUADRIC *) Object)->Shape_Texture, Vector);
}

void Scale_Quadric (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transformation;

   Get_Scaling_Transformation (&Transformation, Vector);
   Transform_Quadric ((QUADRIC *) Object, &Transformation);

   Scale_Texture (&((QUADRIC *) Object)->Shape_Texture, Vector);
}

void Invert_Quadric (Object)
OBJECT *Object;
{
   QUADRIC *Shape = (QUADRIC *) Object;

   VScale (Shape -> Object_2_Terms, Shape -> Object_2_Terms, -1.0);
   VScale (Shape -> Object_Mixed_Terms, Shape -> Object_Mixed_Terms, -1.0);
   VScale (Shape -> Object_Terms, Shape -> Object_Terms, -1.0);
   Shape -> Object_Constant *= -1.0;
}
