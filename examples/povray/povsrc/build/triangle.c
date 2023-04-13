/****************************************************************************
*                triangle.c
*
*  This module implements primitives for triangles and smooth triangles.
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

METHODS Triangle_Methods = {
   Object_Intersect, All_Triangle_Intersections,
   Inside_Triangle, Triangle_Normal,
   Copy_Triangle,
   Translate_Triangle, Rotate_Triangle,
   Scale_Triangle, Invert_Triangle};

METHODS Smooth_Triangle_Methods = {
   Object_Intersect, All_Triangle_Intersections,
   Inside_Triangle, Smooth_Triangle_Normal,
   Copy_Smooth_Triangle,
   Translate_Smooth_Triangle, Rotate_Smooth_Triangle,
   Scale_Smooth_Triangle, Invert_Smooth_Triangle};

extern TRIANGLE *Get_Triangle_Shape();

extern RAY *VP_Ray;
extern long Ray_Triangle_Tests, Ray_Triangle_Tests_Succeeded;

#define max3(x,y,z) ((x>y)?((x>z)?1:3):((y>z)?2:3))

void Find_Triangle_Dominant_Axis(Triangle)
TRIANGLE *Triangle;
{
   DBL x, y, z;

   x = fabs(Triangle->Normal_Vector.x);
   y = fabs (Triangle->Normal_Vector.y);
   z = fabs (Triangle->Normal_Vector.z);
   switch (max3(x, y, z)) {
   case 1: 
      Triangle->Dominant_Axis = X_AXIS;
      break;
   case 2: 
      Triangle->Dominant_Axis = Y_AXIS;
      break;
   case 3: 
      Triangle->Dominant_Axis = Z_AXIS;
      break;
   }
}

void Compute_Smooth_Triangle (Triangle)
SMOOTH_TRIANGLE *Triangle;
{
   VECTOR P3MinusP2, VTemp1, VTemp2;
   DBL x, y, z, uDenominator, Proj;

   VSub (P3MinusP2, Triangle->P3, Triangle->P2);
   x = fabs (P3MinusP2.x);
   y = fabs (P3MinusP2.y);
   z = fabs (P3MinusP2.z);

   switch (max3 (x, y, z)) {
   case 1:  
      Triangle->vAxis = X_AXIS;
      Triangle->BaseDelta = P3MinusP2.x;
      break;

   case 2:  
      Triangle->vAxis = Y_AXIS;
      Triangle->BaseDelta = P3MinusP2.y;
      break;

   case 3:  
      Triangle->vAxis = Z_AXIS;
      Triangle->BaseDelta = P3MinusP2.z;
      break;
   }      

   VSub (VTemp1, Triangle->P2, Triangle->P3);
   VNormalize (VTemp1, VTemp1);
   VSub (VTemp2, Triangle->P1, Triangle->P3);
   VDot (Proj, VTemp2, VTemp1);
   VScale (VTemp1, VTemp1, Proj);
   VSub (Triangle->Perp, VTemp1, VTemp2);
   VNormalize (Triangle->Perp, Triangle->Perp);
   VDot (uDenominator, VTemp2, Triangle->Perp);
   uDenominator = -1.0 / uDenominator;
   VScale (Triangle->Perp, Triangle->Perp, uDenominator);
}

int Compute_Triangle (Triangle)
TRIANGLE *Triangle;
{
   VECTOR V1, V2, Temp;
   DBL Length;

   VSub (V1, Triangle->P1, Triangle->P2);
   VSub (V2, Triangle->P3, Triangle->P2);
   VCross (Triangle->Normal_Vector, V1, V2);
   VLength (Length, Triangle->Normal_Vector);
   /* Set up a flag so we can ignore degenerate triangles */
   if (Length < 1.0e-9){
      Triangle->Degenerate_Flag = TRUE;
      return (0);
   }

   /* Normalize the normal vector. */
   VScale (Triangle->Normal_Vector, Triangle->Normal_Vector, 1.0/Length);

   VDot (Triangle->Distance, Triangle->Normal_Vector, Triangle->P1);
   Triangle->Distance *= -1.0;
   Find_Triangle_Dominant_Axis(Triangle);

   switch (Triangle->Dominant_Axis) {
   case X_AXIS:
      if ((Triangle->P2.y - Triangle->P3.y)*(Triangle->P2.z - Triangle->P1.z) <
         (Triangle->P2.z - Triangle->P3.z)*(Triangle->P2.y - Triangle->P1.y)) {

         Temp = Triangle->P2;
         Triangle->P2 = Triangle->P1;
         Triangle->P1 = Temp;
         if (Triangle->Type == SMOOTH_TRIANGLE_TYPE) {
            Temp = ((SMOOTH_TRIANGLE *) Triangle)->N2;
            ((SMOOTH_TRIANGLE *) Triangle)->N2 = ((SMOOTH_TRIANGLE *) Triangle)->N1;
            ((SMOOTH_TRIANGLE *) Triangle)->N1 = Temp;
         }
      }
      break;

   case Y_AXIS:
      if ((Triangle->P2.x - Triangle->P3.x)*(Triangle->P2.z - Triangle->P1.z) <
         (Triangle->P2.z - Triangle->P3.z)*(Triangle->P2.x - Triangle->P1.x)) {

         Temp = Triangle->P2;
         Triangle->P2 = Triangle->P1;
         Triangle->P1 = Temp;
         if (Triangle->Type == SMOOTH_TRIANGLE_TYPE) {
            Temp = ((SMOOTH_TRIANGLE *) Triangle)->N2;
            ((SMOOTH_TRIANGLE *) Triangle)->N2 = ((SMOOTH_TRIANGLE *) Triangle)->N1;
            ((SMOOTH_TRIANGLE *) Triangle)->N1 = Temp;
         }
      }
      break;

   case Z_AXIS:
      if ((Triangle->P2.x - Triangle->P3.x)*(Triangle->P2.y - Triangle->P1.y) <
         (Triangle->P2.y - Triangle->P3.y)*(Triangle->P2.x - Triangle->P1.x)) {

         Temp = Triangle->P2;
         Triangle->P2 = Triangle->P1;
         Triangle->P1 = Temp;
         if (Triangle->Type == SMOOTH_TRIANGLE_TYPE) {
            Temp = ((SMOOTH_TRIANGLE *) Triangle)->N2;
            ((SMOOTH_TRIANGLE *) Triangle)->N2 = ((SMOOTH_TRIANGLE *) Triangle)->N1;
            ((SMOOTH_TRIANGLE *) Triangle)->N1 = Temp;
         }
      }
      break;
   }

   if (Triangle->Type == SMOOTH_TRIANGLE_TYPE)
      Compute_Smooth_Triangle((SMOOTH_TRIANGLE *) Triangle);
   return (1);
}

int All_Triangle_Intersections (Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;
PRIOQ *Depth_Queue;
{
   TRIANGLE *Shape = (TRIANGLE *) Object;
   DBL Depth;
   VECTOR Intersection_Point;
   INTERSECTION Local_Element;

   if(Shape->Degenerate_Flag)
      return(FALSE);

   if (Intersect_Triangle (Ray, Shape, &Depth))
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

int Intersect_Triangle (Ray, Triangle, Depth)
RAY *Ray;
TRIANGLE *Triangle;
DBL *Depth;
{
   DBL NormalDotOrigin, NormalDotDirection;
   DBL s, t;

   Ray_Triangle_Tests++;
   if(Triangle->Degenerate_Flag)
      return(FALSE);

   if (Ray == VP_Ray) {
      if (!Triangle->VPCached) {
         VDot (Triangle->VPNormDotOrigin, Triangle->Normal_Vector, Ray->Initial);
         Triangle->VPNormDotOrigin += Triangle->Distance;
         Triangle->VPNormDotOrigin *= -1.0;
         Triangle->VPCached = TRUE;
      }

      VDot (NormalDotDirection, Triangle->Normal_Vector, Ray->Direction);
      if ((NormalDotDirection < Small_Tolerance) &&
         (NormalDotDirection > -Small_Tolerance))
         return (FALSE);

      *Depth = Triangle->VPNormDotOrigin / NormalDotDirection;
   }
   else {
      VDot (NormalDotOrigin, Triangle->Normal_Vector, Ray->Initial);
      NormalDotOrigin += Triangle->Distance;
      NormalDotOrigin *= -1.0;

         VDot (NormalDotDirection, Triangle->Normal_Vector, Ray->Direction);
      if ((NormalDotDirection < Small_Tolerance) &&
         (NormalDotDirection > -Small_Tolerance))
         return (FALSE);

      *Depth = NormalDotOrigin / NormalDotDirection;
   }

   if ((*Depth < Small_Tolerance) || (*Depth > Max_Distance))
      return (FALSE);

   switch (Triangle->Dominant_Axis) {
   case X_AXIS:
      s = Ray->Initial.y + *Depth * Ray->Direction.y;
      t = Ray->Initial.z + *Depth * Ray->Direction.z;

      if ((Triangle->P2.y - s)*(Triangle->P2.z - Triangle->P1.z) <
         (Triangle->P2.z - t)*(Triangle->P2.y - Triangle->P1.y))
         if ((int) Triangle->Inverted) {
            Ray_Triangle_Tests_Succeeded++;
            return (TRUE);
         }
         else
            return (FALSE);

      if ((Triangle->P3.y - s)*(Triangle->P3.z - Triangle->P2.z) <
         (Triangle->P3.z - t)*(Triangle->P3.y - Triangle->P2.y))
         if ((int) Triangle->Inverted) {
            Ray_Triangle_Tests_Succeeded++;
            return (TRUE);
         }
         else
            return (FALSE);

      if ((Triangle->P1.y - s)*(Triangle->P1.z - Triangle->P3.z) <
         (Triangle->P1.z - t)*(Triangle->P1.y - Triangle->P3.y))
         if ((int) Triangle->Inverted) {
            Ray_Triangle_Tests_Succeeded++;
            return (TRUE);
         }
         else
            return (FALSE);

      if (!(int) Triangle->Inverted) {
         Ray_Triangle_Tests_Succeeded++;
         return (TRUE);
      }
      else
         return (FALSE);

   case Y_AXIS:
      s = Ray->Initial.x + *Depth * Ray->Direction.x;
      t = Ray->Initial.z + *Depth * Ray->Direction.z;

      if ((Triangle->P2.x - s)*(Triangle->P2.z - Triangle->P1.z) <
         (Triangle->P2.z - t)*(Triangle->P2.x - Triangle->P1.x))
         if ((int) Triangle->Inverted) {
            Ray_Triangle_Tests_Succeeded++;
            return (TRUE);
         }
         else
            return (FALSE);

      if ((Triangle->P3.x - s)*(Triangle->P3.z - Triangle->P2.z) <
         (Triangle->P3.z - t)*(Triangle->P3.x - Triangle->P2.x))
         if ((int) Triangle->Inverted) {
            Ray_Triangle_Tests_Succeeded++;
            return (TRUE);
         }
         else
            return (FALSE);

      if ((Triangle->P1.x - s)*(Triangle->P1.z - Triangle->P3.z) <
         (Triangle->P1.z - t)*(Triangle->P1.x - Triangle->P3.x))
         if ((int) Triangle->Inverted) {
            Ray_Triangle_Tests_Succeeded++;
            return (TRUE);
         }
         else
            return (FALSE);

      if (!(int) Triangle->Inverted) {
         Ray_Triangle_Tests_Succeeded++;
         return (TRUE);
      }
      else
         return (FALSE);

   case Z_AXIS:
      s = Ray->Initial.x + *Depth * Ray->Direction.x;
      t = Ray->Initial.y + *Depth * Ray->Direction.y;

      if ((Triangle->P2.x - s)*(Triangle->P2.y - Triangle->P1.y) <
         (Triangle->P2.y - t)*(Triangle->P2.x - Triangle->P1.x))
         if ((int) Triangle->Inverted) {
            Ray_Triangle_Tests_Succeeded++;
            return (TRUE);
         }
         else
            return (FALSE);

      if ((Triangle->P3.x - s)*(Triangle->P3.y - Triangle->P2.y) <
         (Triangle->P3.y - t)*(Triangle->P3.x - Triangle->P2.x))
         if ((int) Triangle->Inverted) {
            Ray_Triangle_Tests_Succeeded++;
            return (TRUE);
         }
         else
            return (FALSE);

      if ((Triangle->P1.x - s)*(Triangle->P1.y - Triangle->P3.y) <
         (Triangle->P1.y - t)*(Triangle->P1.x - Triangle->P3.x))
         if ((int) Triangle->Inverted) {
            Ray_Triangle_Tests_Succeeded++;
            return (TRUE);
         }
         else
            return (FALSE);

      if (!(int) Triangle->Inverted) {
         Ray_Triangle_Tests_Succeeded++;
         return (TRUE);
      }
      else
         return (FALSE);

   }
   return (FALSE);
}

int Inside_Triangle (Test_Point, Object)
VECTOR *Test_Point;
OBJECT *Object;
{
   return (FALSE);
}

void Triangle_Normal (Result, Object, Intersection_Point)
OBJECT *Object;
VECTOR *Result, *Intersection_Point;
{
   TRIANGLE *Triangle = (TRIANGLE *) Object;

   *Result = Triangle->Normal_Vector;
}

void *Copy_Triangle (Object)
OBJECT *Object;
{
   TRIANGLE *New_Shape;

   New_Shape = Get_Triangle_Shape ();
   *New_Shape = * ((TRIANGLE *)Object);
   New_Shape -> Next_Object = NULL;

   if (New_Shape->Shape_Texture != NULL)
      New_Shape->Shape_Texture = Copy_Texture (New_Shape->Shape_Texture);

   return (New_Shape);
}

void Translate_Triangle (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRIANGLE *Triangle = (TRIANGLE *) Object;
   VECTOR Translation;

   VEvaluate (Translation, Triangle->Normal_Vector, *Vector);
   Triangle->Distance -= Translation.x + Translation.y + Translation.z;
   VAdd (Triangle->P1, Triangle->P1, *Vector)
      VAdd (Triangle->P2, Triangle->P2, *Vector)
         VAdd (Triangle->P3, Triangle->P3, *Vector)

            Translate_Texture (&((TRIANGLE *) Object)->Shape_Texture, Vector);
}

void Rotate_Triangle (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transformation;
   TRIANGLE *Triangle = (TRIANGLE *) Object;

   Get_Rotation_Transformation (&Transformation, Vector);
   MTransformVector (&Triangle->Normal_Vector,
      &Triangle->Normal_Vector, &Transformation);
   MTransformVector (&Triangle->P1, &Triangle->P1, &Transformation);
   MTransformVector (&Triangle->P2, &Triangle->P2, &Transformation);
   MTransformVector (&Triangle->P3, &Triangle->P3, &Transformation);
   Compute_Triangle (Triangle);

   Rotate_Texture (&((TRIANGLE *) Object)->Shape_Texture, Vector);
}

void Scale_Triangle (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRIANGLE *Triangle = (TRIANGLE *) Object;
   DBL Length;

   Triangle->Normal_Vector.x = Triangle->Normal_Vector.x / Vector->x;
   Triangle->Normal_Vector.y = Triangle->Normal_Vector.y / Vector->y;
   Triangle->Normal_Vector.z = Triangle->Normal_Vector.z / Vector->z;

   VLength(Length, Triangle->Normal_Vector);
   VScale (Triangle->Normal_Vector, Triangle->Normal_Vector, 1.0 / Length);
   Triangle->Distance /= Length;

   VEvaluate (Triangle->P1, Triangle->P1, *Vector);
   VEvaluate (Triangle->P2, Triangle->P2, *Vector);
   VEvaluate (Triangle->P3, Triangle->P3, *Vector);

   Scale_Texture (&((TRIANGLE *) Object)->Shape_Texture, Vector);
}


void Invert_Triangle (Object)
OBJECT *Object;
{
   TRIANGLE *Triangle = (TRIANGLE  *) Object;

   Triangle->Inverted ^= TRUE;
}

/* Calculate the Phong-interpolated vector within the triangle
   at the given intersection point. The math for this is a bit
   bizarre:

    -         P1
    |        /|\ \
    |       / |Perp\
    |      /  V  \   \
    |     /   |    \   \
  u |    /____|_____PI___\
    |   /     |       \    \
    -  P2-----|--------|----P3
              Pbase    PIntersect
        |-------------------|
                       v

   Triangle->Perp is a unit vector from P1 to Pbase. We calculate

   u = (PI - P1) DOT Perp / ((P3 - P1) DOT Perp).

   We then calculate where the line from P1 to PI intersects the line P2 to P3:
   PIntersect = (PI - P1)/u.

   We really only need one coordinate of PIntersect.  We then calculate v as:

      v = PIntersect.x / (P3.x - P2.x)
 or   v = PIntersect.y / (P3.y - P2.y)
 or   v = PIntersect.z / (P3.z - P2.z)

   depending on which calculation will give us the best answers.

   Once we have u and v, we can perform the normal interpolation as:

     NTemp1 = N1 + u(N2 - N1);
     NTemp2 = N1 + u(N3 - N1);
     Result = normalize (NTemp1 + v(NTemp2 - NTemp1))

   As always, any values which are constant for the triangle are cached
   in the triangle.
*/

void Smooth_Triangle_Normal (Result, Object, Intersection_Point)
OBJECT *Object;
VECTOR *Result, *Intersection_Point;
{
   SMOOTH_TRIANGLE *Triangle = (SMOOTH_TRIANGLE *) Object;
   VECTOR PIMinusP1, NTemp1, NTemp2;
   DBL u = 0.0, v = 0.0;

   VSub (PIMinusP1, *Intersection_Point, Triangle->P1);
   VDot (u, PIMinusP1, Triangle->Perp);
   if (u < 1.0e-9) {
      *Result = Triangle->N1;
      return;
   }

   /* BaseDelta contains P3.x-P2.x,  P3.y-P2.y, or P3.z-P2.z depending on the
      value of vAxis. */

   switch (Triangle->vAxis) {
   case X_AXIS:  
      v = (PIMinusP1.x/u + Triangle->P1.x - Triangle->P2.x) / Triangle->BaseDelta;
      break;

   case Y_AXIS:  
      v = (PIMinusP1.y/u + Triangle->P1.y - Triangle->P2.y) / Triangle->BaseDelta;
      break;

   case Z_AXIS:  
      v = (PIMinusP1.z/u + Triangle->P1.z - Triangle->P2.z)/ Triangle->BaseDelta;
      break;
   }

   VSub (NTemp1, Triangle->N2, Triangle->N1);
   VScale (NTemp1, NTemp1, u);
   VAdd (NTemp1, NTemp1, Triangle->N1);
   VSub (NTemp2, Triangle->N3, Triangle->N1);
   VScale (NTemp2, NTemp2, u);
   VAdd (NTemp2, NTemp2, Triangle->N1);
   VSub (*Result, NTemp2, NTemp1);
   VScale (*Result, *Result, v);
   VAdd (*Result, *Result, NTemp1); 
   VNormalize (*Result, *Result);
}

void *Copy_Smooth_Triangle (Object)
OBJECT *Object;
{
   SMOOTH_TRIANGLE *New_Shape;

   New_Shape = Get_Smooth_Triangle_Shape ();
   *New_Shape = * ((SMOOTH_TRIANGLE *)Object);
   New_Shape -> Next_Object = NULL;

   if (New_Shape->Shape_Texture != NULL)
      New_Shape->Shape_Texture = Copy_Texture (New_Shape->Shape_Texture);

   return (New_Shape);
}

void Rotate_Smooth_Triangle (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transformation;
   SMOOTH_TRIANGLE *Triangle = (SMOOTH_TRIANGLE *) Object;

   Get_Rotation_Transformation (&Transformation, Vector);
   MTransformVector (&Triangle->Normal_Vector,
      &Triangle->Normal_Vector, &Transformation);
   MTransformVector (&Triangle->P1, &Triangle->P1, &Transformation);
   MTransformVector (&Triangle->P2, &Triangle->P2, &Transformation);
   MTransformVector (&Triangle->P3, &Triangle->P3, &Transformation);
   MTransformVector (&Triangle->N1, &Triangle->N1, &Transformation);
   MTransformVector (&Triangle->N2, &Triangle->N2, &Transformation);
   MTransformVector (&Triangle->N3, &Triangle->N3, &Transformation);
   Compute_Triangle ((TRIANGLE *) Triangle);

   Rotate_Texture (&((TRIANGLE *) Object)->Shape_Texture, Vector);
}

void Translate_Smooth_Triangle (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   SMOOTH_TRIANGLE *Triangle = (SMOOTH_TRIANGLE *) Object;
   VECTOR Translation;

   VEvaluate (Translation, Triangle->Normal_Vector, *Vector);
   Triangle->Distance -= Translation.x + Translation.y + Translation.z;
   VAdd (Triangle->P1, Triangle->P1, *Vector)
      VAdd (Triangle->P2, Triangle->P2, *Vector)
         VAdd (Triangle->P3, Triangle->P3, *Vector)
            Compute_Triangle ((TRIANGLE *) Triangle);

   Translate_Texture (&((TRIANGLE *) Object)->Shape_Texture, Vector);
}

void Scale_Smooth_Triangle (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   SMOOTH_TRIANGLE *Triangle = (SMOOTH_TRIANGLE *) Object;
   DBL Length;

   Triangle->Normal_Vector.x = Triangle->Normal_Vector.x / Vector->x;
   Triangle->Normal_Vector.y = Triangle->Normal_Vector.y / Vector->y;
   Triangle->Normal_Vector.z = Triangle->Normal_Vector.z / Vector->z;

   VLength(Length, Triangle->Normal_Vector);
   VScale (Triangle->Normal_Vector, Triangle->Normal_Vector, 1.0 / Length);
   Triangle->Distance /= Length;

   VEvaluate (Triangle->P1, Triangle->P1, *Vector);
   VEvaluate (Triangle->P2, Triangle->P2, *Vector);
   VEvaluate (Triangle->P3, Triangle->P3, *Vector);
   Compute_Triangle ((TRIANGLE *) Triangle);

   Scale_Texture (&((SMOOTH_TRIANGLE *) Object)->Shape_Texture, Vector);
}


void Invert_Smooth_Triangle (Object)
OBJECT *Object;
{
   SMOOTH_TRIANGLE *Triangle = (SMOOTH_TRIANGLE  *) Object;

   Triangle->Inverted ^= TRUE;
}
