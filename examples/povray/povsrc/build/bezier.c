/****************************************************************************
*                   bezier.c
*
*  This module implements the code for Bezier bicubic patch shapes
*
*  This file was written by Alexander Enzmann.	He wrote the code for
*  bezier bicubic patches and generously provided us these enhancements.
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

#undef EPSILON
#ifndef EPSILON
#define EPSILON 1.0e-10
#endif

METHODS Bicubic_Patch_Methods =
{ Object_Intersect, All_Bicubic_Patch_Intersections,
   Inside_Bicubic_Patch, Bicubic_Patch_Normal, Copy_Bicubic_Patch,
   Translate_Bicubic_Patch, Rotate_Bicubic_Patch,
   Scale_Bicubic_Patch, Invert_Bicubic_Patch};

extern long Ray_Bicubic_Tests, Ray_Bicubic_Tests_Succeeded;
extern unsigned int Options;
extern RAY *VP_Ray;
extern int Shadow_Test_Flag;

int max_depth_reached;

static void bezier_value PARAMS((VECTOR *Result, DBL u, DBL v,
VECTOR (*Control_Points)[4][4]));
static void bezier_partial PARAMS((VECTOR *Result, DBL u, DBL v,
BICUBIC_PATCH *Shape));
static int subpatch_normal PARAMS((VECTOR *v1, VECTOR *v2, VECTOR *v3,
VECTOR *Result, DBL *d));
static int intersect_subpatch PARAMS((int Patch_Type, RAY *Ray,
VECTOR *v1, VECTOR *v2, VECTOR *v3,
VECTOR *n, DBL d,
VECTOR *n1, VECTOR *n2, VECTOR *n3,
DBL *Depth, VECTOR *IP, VECTOR *IP_Norm));
static void find_average PARAMS((int vector_count, VECTOR *vectors,
VECTOR *center, DBL *radius));
static int spherical_bounds_check PARAMS((RAY *Ray, VECTOR *center,
DBL radius));
static int intersect_bicubic_patch0 PARAMS((RAY *Ray, BICUBIC_PATCH *Shape,
DBL *Depths));
static int intersect_bicubic_patch1 PARAMS((RAY *Ray, BICUBIC_PATCH *Shape,
DBL *Depths));
static int intersect_bicubic_patch2 PARAMS((RAY *Ray, BICUBIC_PATCH *Shape,
DBL *Depths));
static int intersect_bicubic_patch3 PARAMS((RAY *Ray, BICUBIC_PATCH *Shape,
DBL *Depths));
static int intersect_bicubic_patch4 PARAMS((RAY *Ray, BICUBIC_PATCH *Shape,
DBL *Depths));
static DBL point_plane_distance PARAMS((VECTOR *, VECTOR *, DBL *));
static DBL determine_subpatch_flatness PARAMS((VECTOR (*)[4][4]));
static int flat_enough PARAMS((BICUBIC_PATCH *, VECTOR (*)[4][4]));
static void bezier_bounding_sphere PARAMS((VECTOR (*)[4][4], VECTOR *,DBL *));
static void bezier_subpatch_intersect PARAMS((RAY *, BICUBIC_PATCH *,
VECTOR (*)[4][4], DBL, DBL, DBL,
int, int *, DBL *, DBL *, DBL *));
static void bezier_split_left_right PARAMS((VECTOR (*)[4][4],VECTOR (*)[4][4],
VECTOR (*)[4][4]));
static void bezier_split_up_down PARAMS((VECTOR (*)[4][4], VECTOR (*)[4][4],
VECTOR (*)[4][4]));
static void bezier_subdivider PARAMS((RAY *, BICUBIC_PATCH *,VECTOR (*)[4][4],
DBL, DBL, DBL, DBL, int, int *, DBL *,
DBL *, DBL *));
static void bezier_tree_deleter PARAMS((BEZIER_NODE *Node));
static BEZIER_NODE *bezier_tree_builder PARAMS((BICUBIC_PATCH *,
VECTOR(*)[4][4], int));
static void bezier_tree_walker PARAMS((RAY *, BICUBIC_PATCH *, BEZIER_NODE *,
int, int *, DBL *));
static BEZIER_NODE *create_new_bezier_node PARAMS((void));
static BEZIER_VERTICES *create_bezier_vertex_block PARAMS((void));
static BEZIER_CHILDREN *create_bezier_child_block PARAMS((void));


static BEZIER_NODE *
create_new_bezier_node()
{
   BEZIER_NODE *Node = (BEZIER_NODE *)malloc(sizeof(BEZIER_NODE));
   if (Node == NULL) {
      printf("Failed to allocate Bezier node\n");
      exit(0);
   }
   Node->Data_Ptr = NULL;
   return Node;
}

static BEZIER_VERTICES *
create_bezier_vertex_block()
{
   BEZIER_VERTICES *Vertices = (BEZIER_VERTICES *)malloc(sizeof(BEZIER_VERTICES));
   if (Vertices == NULL) {
      printf("Failed to allocate Bezier vertices\n");
      exit(0);
   }
   return Vertices;
}

static BEZIER_CHILDREN *
create_bezier_child_block()
{
   BEZIER_CHILDREN *Children = (BEZIER_CHILDREN *)malloc(sizeof(BEZIER_CHILDREN));
   if (Children == NULL) {
      printf("Failed to allocate Bezier children\n");
      exit(0);
   }
   return Children;
}

static BEZIER_NODE *bezier_tree_builder(Object, Patch, depth)
BICUBIC_PATCH *Object;
VECTOR (*Patch)[4][4];
int depth;
{
   VECTOR Lower_Left[4][4], Lower_Right[4][4];
   VECTOR Upper_Left[4][4], Upper_Right[4][4];
   BEZIER_CHILDREN *Children;
   BEZIER_VERTICES *Vertices;
   BEZIER_NODE *Node = create_new_bezier_node();

   if (depth > max_depth_reached) max_depth_reached = depth;

   /* Build the bounding sphere for this subpatch */
   bezier_bounding_sphere(Patch, &(Node->Center), &(Node->Radius_Squared));

   /* If the patch is close to being flat, then just perform a ray-plane
      intersection test. */
   if (flat_enough(Object, Patch)) {
      /* The patch is now flat enough to simply store the corners */
      Node->Node_Type = BEZIER_LEAF_NODE;
      Vertices = create_bezier_vertex_block();
      Vertices->Vertices[0] = (*Patch)[0][0];
      Vertices->Vertices[1] = (*Patch)[0][3];
      Vertices->Vertices[2] = (*Patch)[3][3];
      Vertices->Vertices[3] = (*Patch)[3][0];
      Node->Data_Ptr = (void *)Vertices;
   }
   else if (depth >= Object->U_Steps)
      if (depth >= Object->V_Steps) {
         /* We are at the max recursion depth. Just store corners. */
         Node->Node_Type = BEZIER_LEAF_NODE;
         Vertices = create_bezier_vertex_block();
         Vertices->Vertices[0] = (*Patch)[0][0];
         Vertices->Vertices[1] = (*Patch)[0][3];
         Vertices->Vertices[2] = (*Patch)[3][3];
         Vertices->Vertices[3] = (*Patch)[3][0];
         Node->Data_Ptr = (void *)Vertices;
      }
      else {
         bezier_split_up_down(Patch, (VECTOR (*)[4][4])Lower_Left, (VECTOR (*)[4][4])Upper_Left);
         Node->Node_Type = BEZIER_INTERIOR_NODE;
         Children = create_bezier_child_block();
         Children->Children[0]=bezier_tree_builder(Object,(VECTOR (*)[4][4])Lower_Left,depth+1);
         Children->Children[1]=bezier_tree_builder(Object,(VECTOR (*)[4][4])Upper_Left,depth+1);
         Node->Count = 2;
         Node->Data_Ptr = (void *)Children;
      }
   else if (depth >= Object->V_Steps) {
      bezier_split_left_right(Patch, (VECTOR (*)[4][4])Lower_Left, (VECTOR (*)[4][4])Lower_Right);
      Node->Node_Type = BEZIER_INTERIOR_NODE;
      Children = create_bezier_child_block();
      Children->Children[0] = bezier_tree_builder(Object,(VECTOR (*)[4][4])Lower_Left,depth+1);
      Children->Children[1] = bezier_tree_builder(Object,(VECTOR (*)[4][4])Lower_Right,depth+1);
      Node->Count = 2;
      Node->Data_Ptr = (void *)Children;
   }
   else {
      bezier_split_left_right(Patch, (VECTOR (*)[4][4])Lower_Left, (VECTOR (*)[4][4])Lower_Right);
      bezier_split_up_down((VECTOR (*)[4][4])Lower_Left, (VECTOR (*)[4][4])Lower_Left, (VECTOR (*)[4][4])Upper_Left);
      bezier_split_up_down((VECTOR (*)[4][4])Lower_Right, (VECTOR (*)[4][4])Lower_Right, (VECTOR (*)[4][4])Upper_Right);
      Node->Node_Type = BEZIER_INTERIOR_NODE;
      Children = create_bezier_child_block();
      Children->Children[0] = bezier_tree_builder(Object,(VECTOR (*)[4][4])Lower_Left,depth+1);
      Children->Children[1] = bezier_tree_builder(Object,(VECTOR (*)[4][4])Upper_Left,depth+1);
      Children->Children[2] = bezier_tree_builder(Object,(VECTOR (*)[4][4])Lower_Right,depth+1);
      Children->Children[3] = bezier_tree_builder(Object,(VECTOR (*)[4][4])Upper_Right,depth+1);
      Node->Count = 4;
      Node->Data_Ptr = (void *)Children;
   }
   return Node;
}

   /* Evaluate a single coordinate point (u, v) on a bezier patch. */
   static void bezier_value(Result, u, v, Control_Points)
      VECTOR *Result;
DBL u, v;
VECTOR (*Control_Points)[4][4];
{
   DBL u2, u3, v2, v3, uu1, uu2, uu3, vv1, vv2, vv3;
   DBL t[4][4];
   int i, j;

   u2 = u * u; u3 = u * u2; v2 = v * v; v3 = v * v2;
   uu1 = 1.0 - u; uu2 = uu1 * uu1; uu3 = uu1 * uu2;
   vv1 = 1.0 - v; vv2 = vv1 * vv1; vv3 = vv1 * vv2;
   t[0][0] =		 uu3 *	    vv3;
   t[0][1] = 3.0 *	 uu3 * v  * vv2;
   t[0][2] = 3.0 *	 uu3 * v2 * vv1;
   t[0][3] =		 uu3 * v3;
   t[1][0] = 3.0 *  u  * uu2 *	    vv3;
   t[1][1] = 9.0 *  u  * uu2 * v  * vv2;
   t[1][2] = 9.0 *  u  * uu2 * v2 * vv1;
   t[1][3] = 3.0 *  u  * uu2 * v3;
   t[2][0] = 3.0 *  u2 * uu1 *	    vv3;
   t[2][1] = 9.0 *  u2 * uu1 * v  * vv2;
   t[2][2] = 9.0 *  u2 * uu1 * v2 * vv1;
   t[2][3] = 3.0 *  u2 * uu1 * v3;
   t[3][0] =	    u3 *	    vv3;
   t[3][1] = 3.0 *  u3 *       v  * vv2;
   t[3][2] = 3.0 *  u3 *       v2 * vv1;
   t[3][3] =	    u3 *       v3;

   Result->x = 0.0;
   Result->y = 0.0;
   Result->z = 0.0;
   for (i=0;i<4;i++) for (j=0;j<4;j++) {
      Result->x += t[i][j] * (*Control_Points)[i][j].x;
      Result->y += t[i][j] * (*Control_Points)[i][j].y;
      Result->z += t[i][j] * (*Control_Points)[i][j].z;
   }
}

/* Calculate the normal to a bezier patch for a particular axis, at
   a particular point on the patch.

   The normal at a point of a parametric surface z = f(u, v) is:

      (|[[dy/du, dy/dv],[dz/du, dz/dv]]|,
       |[[dz/du, dz/dv],[dx/du, dx/dv]]|,
       |[[dx/du, dx/dv],[dy/du, dy/dv]]|)

   The normal is undefined where the determinants vanish.
*/
static void bezier_partial(Result, u, v, Shape)
VECTOR *Result;
DBL u, v;
BICUBIC_PATCH *Shape;
{
   VECTOR U, V; /* Partial derivatives with respect to u, and v. */
   DBL u2, u3, v2, v3;
   DBL t[4][4], Temp;
   int i, j;

   u2 = u * u; u3 = u * u2; v2 = v * v; v3 = v * v2;

   /* Calculate the derivative with respect to u */
   t[0][0] = 3.0 *	(v3-3.0*v2+3.0*v-1.0) * (u2-2.0*u+1.0);
   t[0][1] = 9.0 * v  * (v2-2.0*v+1.0) * (u2-2.0*u+1.0);
   t[0][2] = 9.0 * v2 * (v-1.0) * (u2-2.0*u+1.0);
   t[0][3] = 3.0 * v3 * (u2-2.0*u+1.0);
   t[1][0] = 3.0 *	(v3-3.0*v2+3.0*v-1) * (3.0*u2-4.0*u+1);
   t[1][1] = 9.0 * v  * (v2-2.0*v+1.0) * (3.0*u2-4.0*u+1.0);
   t[1][2] = 9.0 * v2 * (v-1.0) * (3.0*u2-4.0*u+1.0);
   t[1][3] = 3.0 * v3 * (3.0*u2-4.0*u+1.0);
   t[2][0] = 3.0 * u  * (v3-3.0*v2+3.0*v-1.0)*(3.0*u-2.0);
   t[2][1] = 9.0 * u  * v  * (v2-2.0*v+1.0)*(3.0*u-2.0);
   t[2][2] = 9.0 * u  * v2 * (v-1.0)*(3.0*u-2.0);
   t[2][3] = 3.0 * u  * v3 * (3.0*u-2.0);
   t[3][0] = 3.0 * u2 * (v3-3.0*v2+3.0*v-1.0);
   t[3][1] = 9.0 * u2 * v  * (v2-2.0*v+1.0);
   t[3][2] = 9.0 * u2 * v2 * (v-1.0);
   t[3][3] = 3.0 * u2 * v3;
   U.x = 0.0; U.y = 0.0; U.z = 0.0;
   for (i=0;i<4;i++) for (j=0;j<4;j++) {
      U.x += t[i][j] * Shape->Control_Points[i][j].x;
      U.y += t[i][j] * Shape->Control_Points[i][j].y;
      U.z += t[i][j] * Shape->Control_Points[i][j].z;
   }
   Temp = U.x * U.x + U.y * U.y + U.z * U.z;
   if (Temp < EPSILON) {
      /* Partial with respect to u is undefined. */
      Result->x = 1.0; Result->y = 0.0; Result->z = 0.0;
      /* *Result = *n; */
      return;
   }
   else {
      Temp = sqrt(Temp);
      VInverseScale(U, U, Temp);
   }
   /* Calculate the derivative with respect to v */
      t[0][0] = 3.0 * (v2-2.0*v+1.0) * (u3-3.0*u2+3.0*u-1.0);
   t[0][1] = 3.0 * (3.0*v2-4.0*v+1.0) * (u3-3.0*u2+3.0*u-1.0);
   t[0][2] = 3.0 * v  * (3.0*v-2.0) * (u3-3.0*u2+3.0*u-1.0);
   t[0][3] = 3.0 * v2 * (u3-3.0*u2+3.0*u-1.0);
   t[1][0] = 9.0 * u  * (v2-2.0*v+1.0) * (u2-2.0*u+1.0);
   t[1][1] = 9.0 * u  * (3.0*v2-4.0*v+1.0) * (u2-2.0*u+1.0);
   t[1][2] = 9.0 * u  * v * (3.0*v-2.0) * (u2-2.0*u+1.0);
   t[1][3] = 9.0 * u  * v2 * (u2-2.0*u+1.0);
   t[2][0] = 9.0 * u2 * (v2-2.0*v+1.0) * (u-1.0);
   t[2][1] = 9.0 * u2 * (3.0*v2-4.0*v+1.0) * (u-1.0);
   t[2][2] = 9.0 * u2 * v * (3.0*v-2.0) * (u-1.0);
   t[2][3] = 9.0 * u2 * v2 * (u-1.0);
   t[3][0] = 3.0 * u3 * (v2-2.0*v+1.0);
   t[3][1] = 3.0 * u3 * (3.0*v2-4.0*v+1.0);
   t[3][2] = 3.0 * u3 * v  * (3.0*v-2.0);
   t[3][3] = 3.0 * u3 * v2;
   V.x = 0.0; V.y = 0.0; V.z = 0.0;
   for (i=0;i<4;i++) for (j=0;j<4;j++) {
      V.x += t[i][j] * Shape->Control_Points[i][j].x;
      V.y += t[i][j] * Shape->Control_Points[i][j].y;
      V.z += t[i][j] * Shape->Control_Points[i][j].z;
   }
   Temp = V.x * V.x + V.y * V.y + V.z * V.z;
   if (Temp < EPSILON) {
      /* Partial with respect to u is undefined. */
      Result->x = 1.0; Result->y = 0.0; Result->z = 0.0;
      return;
   }
   else {
      Temp = sqrt(Temp);
      VInverseScale(V, V, Temp);
   }
   VCross(*Result, U, V);
}

   /* Calculate the normal to a subpatch (triangle) return the vector
   <1.0 0.0 0.0> if the triangle is degenerate. */
   static int subpatch_normal(v1, v2, v3, Result, d)
      VECTOR *v1, *v2, *v3, *Result;
DBL *d;
{
   VECTOR V1, V2;
   DBL Length;

   VSub(V1, *v1, *v2);
   VSub(V2, *v3, *v2);
   VCross(*Result, V1, V2);
   VLength(Length, *Result);
   if (Length < EPSILON) {
      Result->x = 1.0;
      Result->y = 0.0;
      Result->z = 0.0;
      *d = -1.0 * v1->x;
      return 0;
   }
   else {
      VInverseScale(*Result, *Result, Length);
      VDot(*d, *Result, *v1);
      *d = 0.0 - *d;
      return 1;
   }
}

   /* See if Ray intersects the triangular subpatch defined by the three
   points v1, v2, v3 and the normal to the triangle n. If so, Depth will
   be set to the distance along Ray at which the intersection occurs. */
   static int intersect_subpatch(Patch_Type, Ray, v1, v2, v3, n, d, n1, n2, n3,
      Depth, IP, IP_Norm)
      int Patch_Type;
RAY *Ray;
VECTOR *v1, *v2, *v3, *n, *n1, *n2, *n3, *IP, *IP_Norm;
DBL d, *Depth;
{
   VECTOR TempV1, TempV2, Perp;
   DBL s, t, Proj, mu;
   DBL x1, y1, x2, y2, x3, y3;
   DBL x, y, z;
   int Sign_Holder, Next_Sign, Crossings;

   /* Calculate the point of intersection and the depth. */
   VDot(s, Ray->Direction, *n);
   if (s == 0.0)
      return 0;
   VDot(t, Ray->Initial, *n);
   *Depth = 0.0 - (d + t) / s;
   if (*Depth < Small_Tolerance)
      return 0;
   VScale(*IP, Ray->Direction, *Depth);
   VAdd(*IP, *IP, Ray->Initial);

   /* Map the intersection point and the triangle onto a plane. */
   x = fabs(n->x); y = fabs(n->y); z = fabs(n->z);
   if (x>y)
      if (x>z) {
         x1 = v1->y - IP->y; y1 = v1->z - IP->z;
         x2 = v2->y - IP->y; y2 = v2->z - IP->z;
         x3 = v3->y - IP->y; y3 = v3->z - IP->z;
      }
      else {
         x1 = v1->x - IP->x; y1 = v1->y - IP->y;
         x2 = v2->x - IP->x; y2 = v2->y - IP->y;
         x3 = v3->x - IP->x; y3 = v3->y - IP->y;
      }
   else if (y>z) {
      x1 = v1->x - IP->x; y1 = v1->z - IP->z;
      x2 = v2->x - IP->x; y2 = v2->z - IP->z;
      x3 = v3->x - IP->x; y3 = v3->z - IP->z;
   }
   else {
      x1 = v1->x - IP->x; y1 = v1->y - IP->y;
      x2 = v2->x - IP->x; y2 = v2->y - IP->y;
      x3 = v3->x - IP->x; y3 = v3->y - IP->y;
   }

      /* Determine crossing count */
      Crossings = 0;
   if (y1 < 0) Sign_Holder = -1;
   else Sign_Holder = 1;

      /* Start of winding tests, test the segment from v1 to v2 */
      if (y2 < 0) Next_Sign = -1;
      else Next_Sign = 1;
   if (Sign_Holder != Next_Sign) {
      if (x1 > 0.0) {
         if (x2 > 0.0)
            Crossings++;
         else if ((x1 - y1 * (x2 - x1) / (y2 - y1)) > 0.0)
            Crossings++;
      }
      else if (x2 > 0.0) {
         if ((x1 - y1 * (x2 - x1) / (y2 - y1)) > 0.0)
            Crossings++;
      }
      Sign_Holder = Next_Sign;
   }

   /* Test the segment from v2 to v3 */
   if (y3 < 0) Next_Sign = -1;
   else Next_Sign = 1;
   if (Sign_Holder != Next_Sign) {
      if (x2 > 0.0) {
         if (x3 > 0.0)
            Crossings++;
         else if ((x2 - y2 * (x3 - x2) / (y3 - y2)) > 0.0)
            Crossings++;
      }
      else if (x3 > 0.0) {
         if ((x2 - y2 * (x3 - x2) / (y3 - y2)) > 0.0)
            Crossings++;
      }
      Sign_Holder = Next_Sign;
   }

   /* Test the segment from v3 to v1 */
   if (y1 < 0) Next_Sign = -1;
   else Next_Sign = 1;
   if (Sign_Holder != Next_Sign) {
      if (x3 > 0.0) {
         if (x1 > 0.0)
            Crossings++;
         else if ((x3 - y3 * (x1 - x3) / (y1 - y3)) > 0.0)
            Crossings++;
      }
      else if (x1 > 0.0) {
         if ((x3 - y3 * (x1 - x3) / (y1 - y3)) > 0.0)
            Crossings++;
      }
      Sign_Holder = Next_Sign;
   }
   if (Crossings != 1)
      return 0;

   if (Patch_Type == 0 || Patch_Type == 1 || Patch_Type == 2 ||
      Patch_Type == 3) {
      *IP_Norm = *n;
      return 1;
   }
   else if (Patch_Type == 4) {
      VSub(TempV1, *v2, *v3);
      VNormalize(TempV1, TempV1);
      VSub(TempV2, *v1, *v3);
      VDot(Proj, TempV2, TempV1);
      VScale(TempV1, TempV1, Proj);
      VSub(Perp, TempV1, TempV2);
      VNormalize(Perp, Perp);
      VDot(mu, TempV2, Perp);
      mu = -1.0 / mu;
      VScale(Perp, Perp, mu);
      VSub(TempV1, *IP, *v1);
      VDot(s, TempV1, Perp);
      if (s < EPSILON) {
         *IP_Norm = *n1;
         return 1;
      }
      VSub(TempV1, *v3, *v2);
      x = fabs(TempV1.x); y = fabs(TempV1.y); z = fabs(TempV1.z);
      if (x>y)
         if (x>z)
            t = (TempV1.x / s + v1->x - v2->x) / TempV1.x;
         else
            t = (TempV1.z / s + v1->z - v2->z) / TempV1.z;
      else if (y > z)
         t = (TempV1.y / s + v1->y - v2->y) / TempV1.y;
      else
         t = (TempV1.z / s + v1->z - v2->z) / TempV1.z;
      VSub(TempV1, *n2, *n1);
      VScale(TempV1, TempV1, s);
      VAdd(TempV1, TempV1, *n1);
      VSub(TempV2, *n3, *n1);
      VScale(TempV2, TempV2, s);
      VAdd(TempV2, TempV2, *n1);
      VSub(*IP_Norm, TempV2, TempV1);
      VScale(*IP_Norm, *IP_Norm, t);
      VAdd(*IP_Norm, *IP_Norm, TempV1);
      VNormalize(*IP_Norm, *IP_Norm);
      return 1;
   }
   else
      return 0;
}

/* Find a sphere that contains all of the points in the list "vectors" */
static void find_average(vector_count, vectors, center, radius)
int vector_count;
VECTOR *vectors, *center;
DBL *radius;
{
   DBL r0, r1, xc=0, yc=0, zc=0;
   DBL x0, y0, z0;
   int i;
   for (i=0;i<vector_count;i++) {
      xc += vectors[i].x;
      yc += vectors[i].y;
      zc += vectors[i].z;
   }
   xc /= (DBL)vector_count;
   yc /= (DBL)vector_count;
   zc /= (DBL)vector_count;
   r0 = 0.0;
   for (i=0;i<vector_count;i++) {
      x0 = vectors[i].x - xc;
      y0 = vectors[i].y - yc;
      z0 = vectors[i].z - zc;
      r1 = x0 * x0 + y0 * y0 + z0 * z0;
      if (r1 > r0) r0 = r1;
   }
   center->x = xc; center->y = yc; center->z = zc;
   *radius = r0;
}

#define SUBDIVISION_EPSILON 0.001
#define MAX_RECURSION_DEPTH 20

static int spherical_bounds_check(Ray, center, radius)
RAY *Ray;
VECTOR *center;
DBL radius;
{
   DBL x, y, z, dist1, dist2;
   x = center->x - Ray->Initial.x;
   y = center->y - Ray->Initial.y;
   z = center->z - Ray->Initial.z;
   dist1 = x * x + y * y + z * z;
   if (dist1 < radius)
      /* ray starts inside sphere - assume it intersects. */
      return 1;
   else {
      dist2 = x*Ray->Direction.x + y*Ray->Direction.y + z*Ray->Direction.z;
      dist2 = dist2 * dist2;
      if (dist2 > 0 && (dist1 - dist2 < radius))
         return 1;
   }
   return 0;
}

/* Find a sphere that bounds all of the control points of a Bezier patch.
   The values returned are: the center of the bounding sphere, and the
   square of the radius of the bounding sphere. */
static void bezier_bounding_sphere(Patch, center, radius)
VECTOR (*Patch)[4][4], *center;
DBL *radius;
{
   DBL r0, r1, xc=0, yc=0, zc=0;
   DBL x0, y0, z0;
   int i, j;
   for (i=0;i<4;i++) {
      for (j=0;j<4;j++) {
         xc += (*Patch)[i][j].x;
         yc += (*Patch)[i][j].y;
         zc += (*Patch)[i][j].z;
      }
   }
   xc /= 16.0;
   yc /= 16.0;
   zc /= 16.0;
   r0 = 0.0;
   for (i=0;i<4;i++) {
      for (j=0;j<4;j++) {
         x0 = (*Patch)[i][j].x - xc;
         y0 = (*Patch)[i][j].y - yc;
         z0 = (*Patch)[i][j].z - zc;
         r1 = x0 * x0 + y0 * y0 + z0 * z0;
         if (r1 > r0) r0 = r1;
      }
   }
   center->x = xc; center->y = yc; center->z = zc;
   *radius = r0;
}

/* Precompute grid points and normals for a bezier patch */
void
Precompute_Patch_Values(Shape)
BICUBIC_PATCH *Shape;
{
   int i, j;
   DBL d, u, v, delta_u, delta_v;
   VECTOR v0, v1, v2, v3, n;
   VECTOR Control_Points[16];
   VECTOR (*Patch_Ptr)[4][4] = (VECTOR (*)[4][4]) Shape->Control_Points;

   /* Calculate the bounding sphere for the entire patch. */
   for (i=0;i<4;i++) for (j=0;j<4;j++)
      Control_Points[4*i+j] = Shape->Control_Points[i][j];
   find_average(16, &Control_Points[0], &Shape->Bounding_Sphere_Center,
      &Shape->Bounding_Sphere_Radius);
   /* Shape->Node_Tree = NULL; */
   if (Shape->Patch_Type == 0 || Shape->Patch_Type == 2)
      return;
   else if (Shape->Patch_Type == 3) {
      if (Shape->Node_Tree != NULL)
         bezier_tree_deleter(Shape->Node_Tree);
      Shape->Node_Tree = bezier_tree_builder(Shape, Patch_Ptr, 0);
      return;
   }
   delta_u = 1.0 / (DBL)Shape->U_Steps;
   delta_v = 1.0 / (DBL)Shape->V_Steps;
   if (Shape->Interpolated_Grid == NULL) {
      Shape->Interpolated_Grid = (VECTOR **)
         malloc((Shape->U_Steps+1) * sizeof(VECTOR *));
      if (Shape->Interpolated_Grid == NULL)
         Error("Failed to allocate Interpolated_Grid");
      for (i=0;i<=Shape->U_Steps;i++) {
         Shape->Interpolated_Grid[i] = (VECTOR *)
            malloc((Shape->V_Steps+1) * sizeof(VECTOR));
         if (Shape->Interpolated_Grid == NULL)
            Error("Failed to allocate component of Interpolated_Grid");
      }
      Shape->Interpolated_Normals = (VECTOR **)
         malloc((Shape->U_Steps+1) * sizeof(VECTOR *));
      if (Shape->Interpolated_Normals == NULL)
         Error("Failed to allocate Interpolated_Normals");
      for (i=0;i<=Shape->U_Steps;i++) {
         Shape->Interpolated_Normals[i] = (VECTOR *)
            malloc(2*(Shape->V_Steps+1)*sizeof(VECTOR));
         if (Shape->Interpolated_Normals == NULL)
            Error("Failed to allocate component of Interpolated_Normals");
      }

      if (Shape->Patch_Type == 4) {
         Shape->Smooth_Normals = (VECTOR **)
            malloc((Shape->U_Steps+1) * sizeof(VECTOR *));
         if (Shape->Smooth_Normals == NULL)
            Error("Failed to allocate Smooth_Normals");
         for (i=0;i<=Shape->U_Steps;i++) {
            Shape->Smooth_Normals[i] = (VECTOR *)
               malloc((Shape->V_Steps+1)*sizeof(VECTOR));
            if (Shape->Smooth_Normals == NULL)
               Error("Failed to allocate component of Smooth_Normals");
         }
      }

      Shape->Interpolated_D = (DBL **)malloc((Shape->U_Steps+1) * sizeof(DBL *));
      if (Shape->Interpolated_D == NULL)
         Error("Failed to allocate Interpolated_D");
      for (i=0;i<=Shape->U_Steps;i++) {
         Shape->Interpolated_D[i] = (DBL *)
            malloc(2*(Shape->V_Steps+1)*sizeof(DBL));
         if (Shape->Interpolated_D == NULL)
            Error("Failed to allocate component of Interpolated_D");
      }
   }

   /* Calculate the grid values for the given subdivision values. */
   for (i=0;i<=Shape->U_Steps;i++) {
      u = (DBL)i / (DBL)Shape->U_Steps;
      for (j=0;j<Shape->V_Steps;j++) {
         v = (DBL) j / (DBL)Shape->V_Steps;
         bezier_value(&Shape->Interpolated_Grid[i][j], u, v, Patch_Ptr);
      }
   }

   for (i=0;i<Shape->U_Steps;i++) {
      u = (DBL)i / (DBL)Shape->U_Steps;
      for (j=0;j<Shape->V_Steps;j++) {
         v = (DBL)j / (DBL)Shape->V_Steps;

         /* Calculate surface values for the current patch. */
         bezier_value(&v0, u, v, Patch_Ptr);
         bezier_value(&v1, u+delta_u, v, Patch_Ptr);
         bezier_value(&v2, u, v+delta_v, Patch_Ptr);
         bezier_value(&v3, u+delta_u, v+delta_v, Patch_Ptr);

         Shape->Interpolated_Grid[i][j] = v0;
         Shape->Interpolated_Grid[i+1][j] = v1;
         Shape->Interpolated_Grid[i][j+1] = v2;
         Shape->Interpolated_Grid[i+1][j+1] = v3;
         if (Shape->Patch_Type == 1 || Shape->Patch_Type == 4) {
            /* Calculate the normals */
            if (subpatch_normal(&v0, &v2, &v1, &n, &d)) {
               Shape->Interpolated_Normals[i][2*j] = n;
               Shape->Interpolated_D[i][2*j] = d;
            }
            else {
               Shape->Interpolated_Normals[i][2*j].x = 0.0;
               Shape->Interpolated_Normals[i][2*j].y = 0.0;
               Shape->Interpolated_Normals[i][2*j].z = 0.0;
               Shape->Interpolated_D[i][2*j] = 0.0;
            }

               if (subpatch_normal(&v1, &v2, &v3, &n, &d)) {
                  Shape->Interpolated_Normals[i][2*j+1] = n;
                  Shape->Interpolated_D[i][2*j+1] = d;
               }
               else {
                  Shape->Interpolated_Normals[i][2*j+1].x = 0.0;
                  Shape->Interpolated_Normals[i][2*j+1].y = 0.0;
                  Shape->Interpolated_Normals[i][2*j+1].z = 0.0;
                  Shape->Interpolated_D[i][2*j+1] = 0.0;
               }
         }
      }
   }

      if (Shape->Patch_Type == 4) {
         /* Calculate normals at the corners of the subpatches */
         for (i=0;i<=Shape->U_Steps;i++) {
            u = (DBL)i / (DBL)Shape->U_Steps;
            for (j=0;j<=Shape->V_Steps;j++) {
               v = (DBL)j / (DBL)Shape->V_Steps;
               bezier_partial(&Shape->Smooth_Normals[i][j], u, v, Shape);
            }
         }
      }
}

/* Determine the distance from a point to a plane. */
static DBL point_plane_distance(p, n, d)
VECTOR *p, *n;
DBL *d;
{
   DBL temp1, temp2;

   VDot(temp1, *p, *n);
   temp1 += *d;
   VLength(temp2, *n);
   if (fabs(temp2) < EPSILON) return 0;
   temp1 /= temp2;
   return temp1;
}

static void bezier_subpatch_intersect(Ray, Shape, Patch, u0, u1, v0,
recursion_depth, depth_count, Depths, U_Values, V_Values)
RAY *Ray;
BICUBIC_PATCH *Shape;
VECTOR (*Patch)[4][4];
DBL u0, u1, v0;
int recursion_depth, *depth_count;
DBL *Depths, *U_Values, *V_Values;
{
   int tcnt = Shape->Intersection_Count;
   VECTOR vv0, vv1, vv2, vv3, n, ip;
   DBL d, Depth;

   if (tcnt + *depth_count >= MAX_BICUBIC_INTERSECTIONS) return;

   vv0 = (*Patch)[0][0]; vv1 = (*Patch)[0][3];
   vv2 = (*Patch)[3][3]; vv3 = (*Patch)[3][0];

   /* Triangulate this subpatch, then check for intersections in
      the triangles. */
   if (subpatch_normal(&vv0, &vv1, &vv2, &n, &d)) {
      if (intersect_subpatch(Shape->Patch_Type, Ray, &vv0, &vv1, &vv2, &n, d,
         NULL, NULL, NULL, &Depth, &ip, &n)) {
         Shape->Intersection_Point[tcnt + *depth_count] = ip;
         Shape->Normal_Vector[tcnt + *depth_count] = n;
         Depths[*depth_count] = Depth;
         *depth_count += 1;
      }
   }

   if (*depth_count + tcnt >= MAX_BICUBIC_INTERSECTIONS) return;

   if (subpatch_normal(&vv0, &vv2, &vv3, &n, &d)) {
      if (intersect_subpatch(Shape->Patch_Type, Ray, &vv0, &vv2, &vv3, &n, d,
         NULL, NULL, NULL, &Depth, &ip, &n)) {
         Shape->Intersection_Point[tcnt + *depth_count] = ip;
         Shape->Normal_Vector[tcnt + *depth_count] = n;
         Depths[*depth_count] = Depth;
         *depth_count += 1;
      }
   }
}

static void bezier_split_left_right(Patch, Left_Patch, Right_Patch)
VECTOR (*Patch)[4][4], (*Left_Patch)[4][4], (*Right_Patch)[4][4];
{
   VECTOR Temp1[4], Temp2[4], Half;
   int i, j;
   for (i=0;i<4;i++) {
      Temp1[0] = (*Patch)[i][0];
      VHalf(Temp1[1], (*Patch)[i][0], (*Patch)[i][1]);
      VHalf(Half, (*Patch)[i][1], (*Patch)[i][2]);
      VHalf(Temp1[2], Temp1[1], Half);
      VHalf(Temp2[2], (*Patch)[i][2], (*Patch)[i][3]);
      VHalf(Temp2[1], Half, Temp2[2]);
      VHalf(Temp1[3], Temp1[2], Temp2[1]);
      Temp2[0] = Temp1[3];
      Temp2[3] = (*Patch)[i][3];
      for (j=0;j<4;j++) {
         (*Left_Patch)[i][j]  = Temp1[j];
         (*Right_Patch)[i][j] = Temp2[j];
      }
   }
}

static void bezier_split_up_down(Patch, Top_Patch, Bottom_Patch)
VECTOR (*Patch)[4][4], (*Top_Patch)[4][4], (*Bottom_Patch)[4][4];
{
   VECTOR Temp1[4], Temp2[4], Half;
   int i, j;

   for (i=0;i<4;i++) {
      /* Split Left */
      Temp1[0] = (*Patch)[0][i];
      VHalf(Temp1[1], (*Patch)[0][i], (*Patch)[1][i]);
      VHalf(Half, (*Patch)[1][i], (*Patch)[2][i]);
      VHalf(Temp1[2], Temp1[1], Half);
      VHalf(Temp2[2], (*Patch)[2][i], (*Patch)[3][i]);
      VHalf(Temp2[1], Half, Temp2[2]);
      VHalf(Temp1[3], Temp1[2], Temp2[1]);
      Temp2[0] = Temp1[3];
      Temp2[3] = (*Patch)[3][i];
      for (j=0;j<4;j++) {
         (*Bottom_Patch)[j][i] = Temp1[j];
         (*Top_Patch)[j][i]	   = Temp2[j];
      }
   }
}

/* See how close to a plane a subpatch is, the patch must have at least
   three distinct vertices. A negative result from this function indicates
   that a degenerate value of some sort was encountered. */
static DBL determine_subpatch_flatness(Patch)
VECTOR (*Patch)[4][4];
{
   VECTOR vertices[4], n, TempV;
   DBL d, dist, temp1;
   int i, j;

   vertices[0] = (*Patch)[0][0];
   vertices[1] = (*Patch)[0][3];
   VSub(TempV, vertices[0], vertices[1]);
   VLength(temp1, TempV);
   if (fabs(temp1) < EPSILON) {
      /* Degenerate in the V direction for U = 0. This is ok if the other
	 two corners are distinct from the lower left corner - I'm sure there
	 are cases where the corners coincide and the middle has good values,
	 but that is somewhat pathalogical and won't be considered. */
      vertices[1] = (*Patch)[3][3];
      VSub(TempV, vertices[0], vertices[1]);
      VLength(temp1, TempV);
      if (fabs(temp1) < EPSILON) return -1.0;
      vertices[2] = (*Patch)[3][0];
      VSub(TempV, vertices[0], vertices[1]);
      VLength(temp1, TempV);
      if (fabs(temp1) < EPSILON) return -1.0;
      VSub(TempV, vertices[1], vertices[2]);
      VLength(temp1, TempV);
      if (fabs(temp1) < EPSILON) return -1.0;
   }
   else {
      vertices[2] = (*Patch)[3][0];
      VSub(TempV, vertices[0], vertices[1]);
      VLength(temp1, TempV);
      if (fabs(temp1) < EPSILON) {
         vertices[2] = (*Patch)[3][3];
         VSub(TempV, vertices[0], vertices[2]);
         VLength(temp1, TempV);
         if (fabs(temp1) < EPSILON)
            return -1.0;
         VSub(TempV, vertices[1], vertices[2]);
         VLength(temp1, TempV);
         if (fabs(temp1) < EPSILON)
            return -1.0;
      }
      else {
         VSub(TempV, vertices[1], vertices[2]);
         VLength(temp1, TempV);
         if (fabs(temp1) < EPSILON)
            return -1.0;
      }
   }
   /* Now that a good set of candidate points has been found, find the
      plane equations for the patch */
   if (subpatch_normal(&vertices[0], &vertices[1], &vertices[2], &n, &d)) {
      /* Step through all vertices and see what the maximum distance from the
	     plane happens to be. */
      dist = 0.0;
      for (i=0;i<4;i++) {
         for (j=0;j<4;j++) {
            temp1 = fabs(point_plane_distance(&((*Patch)[i][j]), &n, &d));
            if (temp1 > dist)
               dist = temp1;
         }
      }
      return dist;
   }
   else {
      /* printf("Subpatch normal failed in determine_subpatch_flatness\n"); */
         return -1.0;
   }
}

static int flat_enough(Object, Patch)
BICUBIC_PATCH *Object;
VECTOR (*Patch)[4][4];
{
   DBL Dist;

   Dist = determine_subpatch_flatness(Patch);
   if (Dist < 0.0)
      return 0;
   else if (Dist < Object->Flatness_Value)
      return 1;
   else
      return 0;
}

static void bezier_subdivider(Ray, Object, Patch, u0, u1, v0, v1,
recursion_depth, depth_count, Depths, U_Values, V_Values)
RAY *Ray;
BICUBIC_PATCH *Object;
VECTOR (*Patch)[4][4];
DBL u0, u1, v0, v1;
int recursion_depth, *depth_count;
DBL *Depths, *U_Values, *V_Values;
{
   VECTOR Lower_Left[4][4], Lower_Right[4][4];
   VECTOR Upper_Left[4][4], Upper_Right[4][4];
   VECTOR center;
   DBL ut, vt, radius;
   int tcnt = Object->Intersection_Count;

   /* Don't waste time if there are already too many intersections */
   if (tcnt >= MAX_BICUBIC_INTERSECTIONS) return;

   /* Make sure the ray passes through a sphere bounding the control points of
      the patch */
   bezier_bounding_sphere(Patch, &center, &radius);
   if (!spherical_bounds_check(Ray, &center, radius))
      return;

   /* If the patch is close to being flat, then just perform a ray-plane
      intersection test. */
   if (flat_enough(Object, Patch))
      bezier_subpatch_intersect(Ray, Object, Patch, u0, u1, v0,
         recursion_depth+1, depth_count, Depths,
         U_Values, V_Values);

   if (recursion_depth >= Object->U_Steps)
      if (recursion_depth >= Object->V_Steps)
         bezier_subpatch_intersect(Ray, Object, Patch, u0, u1, v0,
            recursion_depth+1, depth_count, Depths,
            U_Values, V_Values);
      else {
         bezier_split_up_down(Patch, (VECTOR (*)[4][4])Lower_Left, (VECTOR (*)[4][4])Upper_Left);
         vt = (v1 - v0) / 2.0;
         bezier_subdivider(Ray, Object, (VECTOR (*)[4][4])Lower_Left, u0, u1, v0, vt,
            recursion_depth+1, depth_count, Depths,
               U_Values, V_Values);
         bezier_subdivider(Ray, Object, (VECTOR (*)[4][4])Upper_Left, u0, u1, vt, v1,
            recursion_depth+1, depth_count, Depths,
            U_Values, V_Values);
      }
   else if (recursion_depth >= Object->V_Steps) {
      bezier_split_left_right(Patch, (VECTOR (*)[4][4])Lower_Left, (VECTOR (*)[4][4])Lower_Right);
      ut = (u1 - u0) / 2.0;
      bezier_subdivider(Ray, Object, (VECTOR (*)[4][4])Lower_Left, u0, ut, v0, v1,
         recursion_depth+1, depth_count, Depths,
         U_Values, V_Values);
      bezier_subdivider(Ray, Object, (VECTOR (*)[4][4])Lower_Right, ut, u1, v0, v1,
         recursion_depth+1, depth_count, Depths,
         U_Values, V_Values);
   }
   else {
      ut = (u1 - u0) / 2.0;
      vt = (v1 - v0) / 2.0;
      bezier_split_left_right(Patch, (VECTOR (*)[4][4])Lower_Left, (VECTOR (*)[4][4])Lower_Right);
      bezier_split_up_down((VECTOR (*)[4][4])Lower_Left, (VECTOR (*)[4][4])Lower_Left, (VECTOR (*)[4][4])Upper_Left);
      bezier_split_up_down((VECTOR (*)[4][4])Lower_Right, (VECTOR (*)[4][4])Lower_Right, (VECTOR (*)[4][4])Upper_Right);
      bezier_subdivider(Ray, Object, (VECTOR (*)[4][4])Lower_Left, u0, ut, v0, vt,
         recursion_depth+1, depth_count, Depths,
            U_Values, V_Values);
      bezier_subdivider(Ray, Object, (VECTOR (*)[4][4])Upper_Left, u0, ut, vt, v1,
         recursion_depth+1, depth_count, Depths,
         U_Values, V_Values);
      bezier_subdivider(Ray, Object, (VECTOR (*)[4][4])Lower_Right, ut, u1, v0, vt,
         recursion_depth+1, depth_count, Depths,
         U_Values, V_Values);
      bezier_subdivider(Ray, Object, (VECTOR (*)[4][4])Upper_Right, ut, u1, vt, v1,
         recursion_depth+1, depth_count, Depths,
         U_Values, V_Values);
   }
}

static void bezier_tree_deleter(Node)
BEZIER_NODE *Node;
{
   BEZIER_CHILDREN *Children;
   int i;

   /* If this is an interior node then continue the descent */
   if (Node->Node_Type == BEZIER_INTERIOR_NODE) {
      Children = (BEZIER_CHILDREN *)Node->Data_Ptr;
      for (i=0;i<Node->Count;i++)
         bezier_tree_deleter(Children->Children[i]);
      free((void *)Children);
   }
   else if (Node->Node_Type == BEZIER_LEAF_NODE) {
      /* Free the memory used for the vertices. */
      free((void *)Node->Data_Ptr);
   }
   /* Free the memory used for the node. */
   free((void *)Node);
}

static void bezier_tree_walker(Ray, Shape, Node, depth, depth_count, Depths)
RAY *Ray;
BICUBIC_PATCH *Shape;
BEZIER_NODE *Node;
int depth, *depth_count;
DBL *Depths;
{
   BEZIER_CHILDREN *Children;
   BEZIER_VERTICES *Vertices;
   VECTOR n, ip, vv0, vv1, vv2, vv3;
   DBL d, Depth;
   int i, tcnt = Shape->Intersection_Count;

   /* Don't waste time if there are already too many intersections */
   if (tcnt >= MAX_BICUBIC_INTERSECTIONS) return;

   /* Make sure the ray passes through a sphere bounding the control points of
      the patch */
   if (!spherical_bounds_check(Ray, &(Node->Center), Node->Radius_Squared))
      return;

   /* If this is an interior node then continue the descent, else
      do a check against the vertices. */
   if (Node->Node_Type == BEZIER_INTERIOR_NODE) {
      Children = (BEZIER_CHILDREN *)Node->Data_Ptr;
      for (i=0;i<Node->Count;i++)
         bezier_tree_walker(Ray, Shape, Children->Children[i],
            depth+1, depth_count, Depths);
   }
   else if (Node->Node_Type == BEZIER_LEAF_NODE) {
      Vertices = (BEZIER_VERTICES *)Node->Data_Ptr;
      vv0 = Vertices->Vertices[0]; vv1 = Vertices->Vertices[1];
      vv2 = Vertices->Vertices[2]; vv3 = Vertices->Vertices[3];

      /* Triangulate this subpatch, then check for intersections in
	 the triangles. */
      if (subpatch_normal(&vv0, &vv1, &vv2, &n, &d)) {
         if (intersect_subpatch(Shape->Patch_Type, Ray, &vv0, &vv1, &vv2, &n, d,
            NULL, NULL, NULL, &Depth, &ip, &n)) {
            Shape->Intersection_Point[tcnt + *depth_count] = ip;
            Shape->Normal_Vector[tcnt + *depth_count] = n;
            Depths[*depth_count] = Depth;
            *depth_count += 1;
         }
      }

      if (*depth_count + tcnt >= MAX_BICUBIC_INTERSECTIONS) return;

      if (subpatch_normal(&vv0, &vv2, &vv3, &n, &d)) {
         if (intersect_subpatch(Shape->Patch_Type, Ray, &vv0, &vv2, &vv3, &n, d,
            NULL, NULL, NULL, &Depth, &ip, &n)) {
            Shape->Intersection_Point[tcnt + *depth_count] = ip;
            Shape->Normal_Vector[tcnt + *depth_count] = n;
            Depths[*depth_count] = Depth;
            *depth_count += 1;
         }
      }
   }
   else {
      printf("Bad Node type at depth %d\n", depth);
   }
}

   /* Intersection of a ray and a bezier patch. */
   /* Note: There is MUCH repeated work being done here. During the subdivision
   process, the values of surface points are not retained from one subpatch
   to the next, even though there are two points in common between one subpatch
   and the one adjacent to it.	An obvious optimization is to retain this
   surface information during the subdivision process.

   A second optimization is to make use of the fact that the surface never
   goes outside the convex hull generated by the control points.  By testing
   the ray against that hull first many unnecessary tests can be avoided.  The
   hull really should be generated at the time the patch is first defined -
   not at run time.
*/
   static int intersect_bicubic_patch0(Ray, Shape, Depths)
      RAY *Ray;
BICUBIC_PATCH *Shape;
DBL *Depths;
{
   int cnt = 0;
   int tcnt = Shape->Intersection_Count;
   int i, j;
   DBL Depth, d, u, v, delta_u, delta_v;
   VECTOR v0, v1, v2, v3, n, ip;
   VECTOR (*Patch_Ptr)[4][4] = (VECTOR (*)[4][4]) Shape->Control_Points;

   if (!spherical_bounds_check(Ray, &(Shape->Bounding_Sphere_Center),
      Shape->Bounding_Sphere_Radius))
      return 0;

   delta_u = 1.0 / (DBL)Shape->U_Steps;
   delta_v = 1.0 / (DBL)Shape->V_Steps;

   /* Calculate the initial point */
   for (i=0;i<Shape->U_Steps;i++) {
      u = (DBL)i / (DBL)Shape->U_Steps;
      for (j=0;j<Shape->V_Steps;j++) {
         v = (DBL)j / (DBL)Shape->V_Steps;

         /* Calculate surface values for the current patch. */
         bezier_value(&v0, u, v, Patch_Ptr);
         bezier_value(&v1, u+delta_u, v, Patch_Ptr);
         bezier_value(&v2, u, v+delta_v, Patch_Ptr);
         bezier_value(&v3, u+delta_u, v+delta_v, Patch_Ptr);

         /* Triangulate this subpatch, then check for intersections in
	    the triangles. */
         if (subpatch_normal(&v0, &v2, &v1, &n, &d)) {
            if (intersect_subpatch(Shape->Patch_Type, Ray, &v0, &v2, &v1, &n, d,
               NULL, NULL, NULL, &Depth, &ip, &n)) {
               Shape->Intersection_Point[tcnt+cnt] = ip;
               Shape->Normal_Vector[tcnt+cnt] = n;
               Depths[cnt] = Depth;
               if (tcnt + ++cnt >= MAX_BICUBIC_INTERSECTIONS)
                  /* Too many intersections. Stop looking for more. */
                  return cnt;
            }
         }
         if (subpatch_normal(&v1, &v2, &v3, &n, &d)) {
            if (intersect_subpatch(Shape->Patch_Type, Ray, &v1, &v2, &v3, &n, d,
               NULL, NULL, NULL, &Depth, &ip, &n)) {
               Shape->Intersection_Point[tcnt+cnt] = ip;
               Shape->Normal_Vector[tcnt+cnt] = n;
               Depths[cnt] = Depth;
               if (tcnt + ++cnt >= MAX_BICUBIC_INTERSECTIONS)
                  /* Too many intersections. Stop looking for more. */
                  return cnt;
            }
         }
      }
   }
   return cnt;
}

static int intersect_bicubic_patch1(Ray, Shape, Depths)
RAY *Ray;
BICUBIC_PATCH *Shape;
DBL *Depths;
{
   int cnt = 0;
   int tcnt = Shape->Intersection_Count;
   int i, j;
   DBL Depth, d, radius;
   VECTOR v[4], n, ip, center;

   if (!spherical_bounds_check(Ray, &(Shape->Bounding_Sphere_Center),
      Shape->Bounding_Sphere_Radius))
      return 0;

   /* Calculate the initial point */
   for (i=0;i<Shape->U_Steps;i++) {
      for (j=0;j<Shape->V_Steps;j++) {

         /* Grab precomputed surface values for the current patch. */
         v[0] = Shape->Interpolated_Grid[i][j];
         v[1] = Shape->Interpolated_Grid[i+1][j];
         v[2] = Shape->Interpolated_Grid[i][j+1];
         v[3] = Shape->Interpolated_Grid[i+1][j+1];

         /* Check the ray against the bounding sphere for this subpatch */
         find_average(4, &v[0], &center, &radius);
         if (!spherical_bounds_check(Ray, &center, radius)) continue;

         n = Shape->Interpolated_Normals[i][2*j];
         if (n.x == 0.0 && n.y == 0.0 && n.z == 0.0) {
            goto l0;
         }
         d = Shape->Interpolated_D[i][2*j];

         /* Check for intersections in this subpatch. */
         if (intersect_subpatch(Shape->Patch_Type, Ray, &v[0], &v[2], &v[1], &n, d,
            NULL, NULL, NULL, &Depth, &ip, &n)) {
            Shape->Intersection_Point[tcnt+cnt] = ip;
            Shape->Normal_Vector[tcnt+cnt] = n;
            Depths[cnt] = Depth;
            if (tcnt + ++cnt >= MAX_BICUBIC_INTERSECTIONS)
               /* Too many intersections. Stop looking for more. */
               return cnt;
         }
l0:
         n = Shape->Interpolated_Normals[i][2*j+1];
         if (n.x == 0.0 && n.y == 0.0 && n.z == 0.0) {
            continue;
         }
         d = Shape->Interpolated_D[i][2*j+1];
         if (intersect_subpatch(Shape->Patch_Type, Ray, &v[1], &v[2], &v[3], &n, d,
            NULL, NULL, NULL, &Depth, &ip, &n)) {
            Shape->Intersection_Point[tcnt+cnt] = ip;
            Shape->Normal_Vector[tcnt+cnt] = n;
            Depths[cnt] = Depth;
            if (tcnt + ++cnt >= MAX_BICUBIC_INTERSECTIONS)
               /* Too many intersections. Stop looking for more. */
               return cnt;
         }
      }
   }
   return cnt;
}

static int intersect_bicubic_patch2(Ray, Shape, Depths)
RAY *Ray;
BICUBIC_PATCH *Shape;
DBL *Depths;
{
   int cnt = 0;
   DBL U_Values[MAX_BICUBIC_INTERSECTIONS];
   DBL V_Values[MAX_BICUBIC_INTERSECTIONS];
   VECTOR (*Patch)[4][4] = (VECTOR (*)[4][4]) Shape->Control_Points;

   bezier_subdivider(Ray, Shape, Patch, 0.0, 1.0, 0.0, 1.0,
      0, &cnt, Depths, &U_Values[0], &V_Values[0]);
   return cnt;
}

static int intersect_bicubic_patch3(Ray, Shape, Depths)
RAY *Ray;
BICUBIC_PATCH *Shape;
DBL *Depths;
{
   int cnt = 0;
   bezier_tree_walker(Ray, Shape, Shape->Node_Tree, 0, &cnt, Depths);
   return cnt;
}

static int intersect_bicubic_patch4(Ray, Shape, Depths)
RAY *Ray;
BICUBIC_PATCH *Shape;
DBL *Depths;
{
   int cnt = 0;
   int tcnt = Shape->Intersection_Count;
   int i, j;
   DBL Depth, d, t;
   VECTOR v0, v1, v2, v3, n;
   VECTOR n0, n1, n2, n3, ip, ip_norm;

   if (!spherical_bounds_check(Ray, &(Shape->Bounding_Sphere_Center),
      Shape->Bounding_Sphere_Radius))
      return 0;

   /* Calculate the initial point */
   for (i=0;i<Shape->U_Steps;i++) {
      for (j=0;j<Shape->V_Steps;j++) {

         /* Grab precomputed surface values for the current patch. */
         v0 = Shape->Interpolated_Grid[i][j];
         v1 = Shape->Interpolated_Grid[i+1][j];
         v2 = Shape->Interpolated_Grid[i][j+1];
         v3 = Shape->Interpolated_Grid[i+1][j+1];

         n0 = Shape->Smooth_Normals[i][j];
         n1 = Shape->Smooth_Normals[i+1][j];
         n2 = Shape->Smooth_Normals[i][j+1];
         n3 = Shape->Smooth_Normals[i+1][j+1];

         n = Shape->Interpolated_Normals[i][2*j];
         if (n.x == 0.0 && n.y == 0.0 && n.z == 0.0) goto l0;
         d = Shape->Interpolated_D[i][2*j];

         /* Make sure the smooth normals point in the same direction as the normal */
         VDot(t, n0, n); if (t < 0) VScale(n0, n0, -1.0);
         VDot(t, n1, n); if (t < 0) VScale(n1, n1, -1.0);
         VDot(t, n2, n); if (t < 0) VScale(n2, n2, -1.0);

         /* Check for intersections in this subpatch. */
         if (intersect_subpatch(Shape->Patch_Type, Ray, &v0, &v2, &v1, &n,
            d, &n0, &n2, &n1, &Depth, &ip, &ip_norm)) {
            Shape->Intersection_Point[tcnt + cnt] = ip;
            Shape->Normal_Vector[tcnt + cnt] = ip_norm;
            Depths[cnt] = Depth;
            if (tcnt + ++cnt >= MAX_BICUBIC_INTERSECTIONS)
               /* Too many intersections. Stop looking for more. */
               return cnt;
         }
l0:
         n = Shape->Interpolated_Normals[i][2*j+1];
         if (n.x == 0.0 && n.y == 0.0 && n.z == 0.0) continue;
         d = Shape->Interpolated_D[i][2*j+1];

         /* Make sure the smooth normals point in the same direction as the normal */
         VDot(t, n1, n); if (t > 0) VScale(n1, n0, -1.0);
         VDot(t, n2, n); if (t > 0) VScale(n2, n1, -1.0);
         VDot(t, n3, n); if (t > 0) VScale(n3, n2, -1.0);

         if (intersect_subpatch(Shape->Patch_Type, Ray, &v1, &v2, &v3, &n,
            d, &n1, &n2, &n3, &Depth, &ip, &ip_norm)) {
            Shape->Intersection_Point[tcnt + cnt] = ip;
            Shape->Normal_Vector[tcnt + cnt] = ip_norm;
            Depths[cnt] = Depth;
            if (tcnt + ++cnt >= MAX_BICUBIC_INTERSECTIONS)
               /* Too many intersections. Stop looking for more. */
               return cnt;
         }
      }
   }
   return cnt;
}

int All_Bicubic_Patch_Intersections(Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;
PRIOQ *Depth_Queue;
{
   BICUBIC_PATCH *Shape = (BICUBIC_PATCH *) Object;
   DBL Depths[MAX_BICUBIC_INTERSECTIONS];
   INTERSECTION Local_Element;
   int cnt, tcnt, i, Intersection_Found;

   Intersection_Found = 0;
   Ray_Bicubic_Tests++;
   if (Ray == VP_Ray)
      Shape->Intersection_Count = 0;
   tcnt = Shape->Intersection_Count;
   if (Shape->Patch_Type == 0)
      cnt = intersect_bicubic_patch0(Ray, Shape, &Depths[0]);
   else if (Shape->Patch_Type == 1)
      cnt = intersect_bicubic_patch1(Ray, Shape, &Depths[0]);
   else if (Shape->Patch_Type == 2)
      cnt = intersect_bicubic_patch2(Ray, Shape, &Depths[0]);
   else if (Shape->Patch_Type == 3)
      cnt = intersect_bicubic_patch3(Ray, Shape, &Depths[0]);
   else if (Shape->Patch_Type == 4)
      cnt = intersect_bicubic_patch4(Ray, Shape, &Depths[0]);
   else {
      Error("Bad patch type\n");
   }
   if (cnt > 0) Ray_Bicubic_Tests_Succeeded++;
   for (i=0;i<cnt;i++) {
      if (!Shadow_Test_Flag)
         Shape->Intersection_Count++;
      Local_Element.Depth = Depths[i];
      Local_Element.Object = Shape->Parent_Object;
      Local_Element.Point = Shape->Intersection_Point[tcnt + i];
      Local_Element.Shape = (SHAPE *)Shape;
      pq_add (Depth_Queue, &Local_Element);
      Intersection_Found = 1;
   }
   return (Intersection_Found);
}

/* A patch is not a solid, so an inside test doesn't make sense. */
int Inside_Bicubic_Patch (Test_Point, Object)
VECTOR *Test_Point;
OBJECT *Object;
{
   return 0;
}

void Bicubic_Patch_Normal (Result, Object, Intersection_Point)
OBJECT *Object;
VECTOR *Result, *Intersection_Point;
{
   BICUBIC_PATCH *Patch = (BICUBIC_PATCH *)Object;
   int i;

   /* If all is going well, the normal was computed at the time the intersection
      was computed.  Look on the list of associated intersection points and normals */
   for (i=0;i<Patch->Intersection_Count;i++)
      if (Intersection_Point->x == Patch->Intersection_Point[i].x &&
         Intersection_Point->y == Patch->Intersection_Point[i].y &&
         Intersection_Point->z == Patch->Intersection_Point[i].z) {
         Result->x = Patch->Normal_Vector[i].x;
         Result->y = Patch->Normal_Vector[i].y;
         Result->z = Patch->Normal_Vector[i].z;
         return;
      }
   if (Options & DEBUGGING) {
      printf("Bicubic patch normal for unknown intersection point\n");
      fflush(stdout);
   }
   Result->x = 1.0;
   Result->y = 0.0;
   Result->z = 0.0;
}

void *Copy_Bicubic_Patch (Object)
OBJECT *Object;
{
   BICUBIC_PATCH *New_Shape;

   New_Shape = Get_Bicubic_Patch_Shape ();
   *New_Shape = * ((BICUBIC_PATCH *)Object);
   New_Shape -> Next_Object = NULL;

   New_Shape->Interpolated_Grid = NULL;
   Precompute_Patch_Values(New_Shape);
   if (New_Shape->Shape_Texture != NULL)
      New_Shape->Shape_Texture = Copy_Texture (New_Shape->Shape_Texture);

   return (void *)(New_Shape);
}

void Translate_Bicubic_Patch (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   BICUBIC_PATCH *Patch = (BICUBIC_PATCH *) Object;
   int i, j;
   for (i=0;i<4;i++) for (j=0;j<4;j++)
      VAdd (Patch->Control_Points[i][j], Patch->Control_Points[i][j], *Vector)
         Precompute_Patch_Values(Patch);
   Translate_Texture (&((BICUBIC_PATCH *) Object)->Shape_Texture, Vector);
}

void Rotate_Bicubic_Patch (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transformation;
   BICUBIC_PATCH *Patch = (BICUBIC_PATCH *) Object;
   int i, j;

   Get_Rotation_Transformation (&Transformation, Vector);
   for (i=0;i<4;i++) for (j=0;j<4;j++)
      MTransformVector(&(Patch->Control_Points[i][j]),
         &(Patch->Control_Points[i][j]),
         &Transformation);
   Precompute_Patch_Values(Patch);
   Rotate_Texture (&((BICUBIC_PATCH *) Object)->Shape_Texture, Vector);
}

void Scale_Bicubic_Patch (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   BICUBIC_PATCH *Patch = (BICUBIC_PATCH *) Object;
   int i, j;
   for (i=0;i<4;i++) for (j=0;j<4;j++)
      VEvaluate(Patch->Control_Points[i][j],
         Patch->Control_Points[i][j], *Vector);
   Precompute_Patch_Values(Patch);
   Scale_Texture (&((BICUBIC_PATCH *) Object)->Shape_Texture, Vector);
}


/* Inversion of a patch really doesn't make sense. */
void Invert_Bicubic_Patch (Object)
OBJECT *Object;
{
   ;
}
