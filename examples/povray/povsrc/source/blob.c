/****************************************************************************
*                blob.c
*
*  This module contains the code for the blob shape.
*
*  This file was written by Alexander Enzmann.	He wrote the code for
*  blobs and generously provided us these enhancements.
*
*  from Persistence of Vision Raytracer 
*       Copyright 1992 Persistence of Vision Team
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

METHODS Blob_Methods =
{ Object_Intersect, All_Blob_Intersections,
   Inside_Blob, Blob_Normal,
   Copy_Blob,
   Translate_Blob, Rotate_Blob,
   Scale_Blob, Invert_Blob};

extern BLOB *Get_Blob_Shape();

extern RAY *VP_Ray;
extern long Ray_Blob_Tests, Ray_Blob_Tests_Succeeded;

static int determine_influences PARAMS((VECTOR *P, VECTOR *D, BLOB *blob, DBL mindist));
static DBL calculate_field_value PARAMS ((OBJECT *obj, VECTOR *Pos));
static int validate_hit PARAMS((BLOB *blob, VECTOR *P));

#define COEFF_LIMIT 1.0e-20
#define INSIDE_TOLERANCE 1.0e-6

/* Starting with the density function: (1-r^2)^2, we have a field
   that varies in strength from 1 at r = 0 to 0 at r = 1.  By
   substituting r/rad for r, we can adjust the range of influence
   of a particular component.  By multiplication by coeff, we can
   adjust the amount of total contribution, giving the formula:
      coeff * (1 - (r/rad)^2)^2
   This varies in strength from coeff at r = 0, to 0 at r = rad. */
void
MakeBlob(obj, threshold, bloblist, npoints, sflag)
OBJECT *obj;
DBL threshold;
blobstackptr bloblist;
int npoints;
int sflag;
{
   BLOB *blob = (BLOB *)obj;
   int i;
   DBL rad, coeff;
   blobstackptr temp;

   if (npoints < 1) {
      printf("Need at least one component in a blob\n");
      exit(1);
   }
   blob->threshold = threshold;
   blob->list = (Blob_Element *)malloc(npoints*sizeof(Blob_Element));
   if (blob->list == NULL) {
      printf("Failed to allocate blob data\n");
      exit(1);
   }
   blob->count = npoints;
   blob->Sturm_Flag = sflag;

   /* Initialize the blob data */
   for(i=0;i<npoints;i++) {
      temp = bloblist;
      if (fabs(temp->elem.coeffs[2]) < EPSILON ||
         temp->elem.radius2 < EPSILON) {
         perror("Degenerate blob element\n");
      }
      /* Store blob specific information */
      rad = temp->elem.radius2;
      rad *= rad;
      coeff = temp->elem.coeffs[2];
      blob->list[i].radius2   = rad;
      blob->list[i].coeffs[2] = coeff;
      blob->list[i].coeffs[1] = -(2.0 * coeff) / rad;
      blob->list[i].coeffs[0] = coeff / (rad * rad);
      blob->list[i].pos.x = temp->elem.pos.x;
      blob->list[i].pos.y = temp->elem.pos.y;
      blob->list[i].pos.z = temp->elem.pos.z;

      bloblist = bloblist->next;
      free(temp);
   }

   /*  Allocate memory for intersection intervals */
   npoints *= 2;
   blob->intervals = (Blob_Interval *)malloc(npoints*sizeof(Blob_Interval));
   if (blob->intervals == NULL) {
      printf("Failed to allocate blob data\n");
      exit(1);
   }
}
/* COMMMENTED OUT
void Set_Blob_Solver PARAMS((OBJECT *obj, int Sturm_Flag));
void BlobDelete PARAMS((OBJECT *obj));

void
Set_Blob_Solver(obj, Sturm_Flag)
   OBJECT *obj;
   int Sturm_Flag;
{
   BLOB *blob = (BLOB *)obj;
   blob->Sturm_Flag = Sturm_Flag;
}

void
BlobDelete(obj)
   OBJECT *obj;
{
   BLOB *blob = (BLOB *)obj;
   free(blob->list);
   free(blob->intervals);
   free(blob);
}
*/

/* Make a sorted list of points along the ray that the various blob
   components start and stop adding their influence.  It would take
   a very complex blob (with many components along the current ray)
   to warrant the overhead of using a faster sort technique. */
static int
determine_influences(P, D, blob, mindist)
VECTOR *P, *D;
BLOB *blob;
DBL mindist;
{
   int i, j, k, cnt;
   DBL b, t, t0, t1, disc;
   VECTOR V;
   Blob_Interval *intervals = blob->intervals;

   cnt = 0;
   for (i=0;i<blob->count;i++) {
      /* Use standard sphere intersection routine
         to determine where the ray hits the volume
         of influence of each component of the blob. */
      VSub(V, blob->list[i].pos, *P);
      VDot(b, V, *D);
      VDot(t, V, V);
      disc = b * b - t + blob->list[i].radius2;
      if (disc < EPSILON)
         continue;
      disc = sqrt(disc);
      t1 = b + disc;
      if (t1 < mindist) t1 = 0.0;
      t0 = b - disc;
      if (t0 < mindist) t0 = 0.0;
      if (t1 == t0) continue;
      else if (t1 < t0) {
         disc = t0;
         t0 = t1;
         t1 = disc;
      }

      /* Store the points of intersection of this
         blob with the ray.  Keep track of: whether
         this is the start or end point of the hit,
         which component was pierced by the ray,
         and the point along the ray that the
         hit occured at. */
      for (k=0;k<cnt && t0 > intervals[k].bound;k++);
      if (k<cnt) {
         /* This hit point is smaller than one that
            already exists - bump the rest and insert
            it here */
         for (j=cnt;j>k;j--)
            memcpy(&intervals[j], &intervals[j-1],
               sizeof(Blob_Interval));
         intervals[k].type  = 0;
         intervals[k].index = i;
         intervals[k].bound = t0;
         cnt++;
         for (k=k+1;k<cnt && t1 > intervals[k].bound;k++);
         if (k<cnt) {
            for (j=cnt;j>k;j--)
               memcpy(&intervals[j], &intervals[j-1],
                  sizeof(Blob_Interval));
            intervals[k].type  = 1;
            intervals[k].index = i;
            intervals[k].bound = t1;
         }
         else {
            intervals[cnt].type  = 1;
            intervals[cnt].index = i;
            intervals[cnt].bound = t1;
         }
         cnt++;
      }
      else {
         /* Just plop the start and end points at
            the end of the list */
         intervals[cnt].type  = 0;
         intervals[cnt].index = i;
         intervals[cnt].bound = t0;
         cnt++;
         intervals[cnt].type  = 1;
         intervals[cnt].index = i;
         intervals[cnt].bound = t1;
         cnt++;
      }
   }
   return cnt;
}

   /* Calculate the field value of a blob - the position vector
   "Pos" must already have been transformed into blob space. */
   static DBL
   calculate_field_value(obj, Pos)
      OBJECT *obj;
VECTOR *Pos;
{
   int i;
   DBL len, density;
   VECTOR V;
   Blob_Element *ptr;
   BLOB *blob = (BLOB *)obj;

   density = 0.0;
   for (i=0,ptr = &(blob->list[0]);i<blob->count;i++,ptr++) {
      VSub(V, ptr->pos, *Pos);
      VDot(len, V, V);
      if (len < ptr->radius2) {
         /* Inside the radius of influence of this
            component, add it's contribution */
         density += len * (len * ptr->coeffs[0] +
            ptr->coeffs[1]) +
         ptr->coeffs[2];
      }
   }
   return density;
}

/* See if the hit in question really is a hit. */
static int
validate_hit(blob, P)
BLOB *blob;
VECTOR *P;
{
   int i;
   Blob_Element *temp;
   DBL val, dist;
   VECTOR V, N;

   N.x = 0.0; N.y = 0.0; N.z = 0.0;
   temp = &(blob->list[0]);
   for(i=0;i<blob->count;i++,temp++) {
      VSub(V, *P, temp->pos);
      VDot(dist, V, V);
      if (dist <= temp->radius2) {
         val = -2.0 * (2.0 * temp->coeffs[0] * dist +
            temp->coeffs[1]);
         N.x += val * V.x;
         N.y += val * V.y;
         N.z += val * V.z;
      }
   }
   VDot(val, N, N);
   if (val < EPSILON)
      return 0;
   else
      return 1;
}

/* Generate intervals of influence of each component.  After these
   are made, determine their aggregate effect on the ray.  As the
   individual intervals are checked, a quartic is generated
   that represents the density at a particular point on the ray.

   After making the substitutions in MakeBlob, there is a formula
   for each component that has the form:
   
      c0 * r^4 + c1 * r^2 + c2.
   
   In order to determine the influence on the ray of all of the
   individual components, we start by determining the distance
   from any point on the ray to the specified point.  This can
   be found using the pythagorean theorem, using C as the center
   of this component, P as the start of the ray, and D as the
   direction of travel of the ray:

      r^2 = (t * D + P - C) . (t * D + P - C)

   we insert this equation for each appearance of r^2 in the
   components' formula, giving:

      r^2 = D.D t^2 + 2 t D . (P - C) + (P - C) . (P - C)

   Since the direction vector has been normalized, D.D = 1.
   Using the substitutions:

      t0 = (P - C) . (P - C),
      t1 = D . (P - C)

   We can write the formula as:

      r^2 = t0 + 2 t t1 + t^2

   Taking r^2 and substituting into the formula for this component
   of the blob we get the formula:

      density = c0 * (r^2)^2 + c1 * r^2 + c2,

   or:

      density = c0 * (t0 + 2 t t1 + t^2)^2 +
                c1 * (t0 + 2 t t1 + t^2) +
                c2

   Expanding terms and collecting with respect to "t" gives:
      t^4 * c0 +
      t^3 * 4 c0 t1 +
      t^2 * (c1 + 2 * c0 t0 + 4 c0 t1^2)
      t   * 2 (c1 t1 + 2 c0 t0 t1) +
            c2 + c1*t0 + c0*t0^2

   This formula can now be solved for "t" by any of the quartic
   root solvers that are available.
*/
int All_Blob_Intersections(Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;
PRIOQ *Depth_Queue;
{
   BLOB *blob = (BLOB *) Object;
   INTERSECTION Local_Element;
   DBL dist, len, *tcoeffs, coeffs[5], roots[4];
   int i, j, cnt;
   VECTOR P, D, V;
   int root_count, in_flag;
   Blob_Element *element;
   DBL t0, t1, c0, c1, c2;
   VECTOR Intersection_Point, dv;
   Blob_Interval *intervals = blob->intervals;
   int Intersection_Found = FALSE;

   Ray_Blob_Tests++;

   /* Transform the ray into the blob space */
   if (blob->Transform != NULL) {
      MInverseTransformVector(&P, &Ray->Initial, blob->Transform);
      MInvTransVector(&D, &Ray->Direction, blob->Transform);
   }
   else {
      P.x = Ray->Initial.x;
      P.y = Ray->Initial.y;
      P.z = Ray->Initial.z;
      D.x = Ray->Direction.x;
      D.y = Ray->Direction.y;
      D.z = Ray->Direction.z;
   }

      len = sqrt(D.x * D.x + D.y * D.y + D.z * D.z);
   if (len == 0.0)
      return 0;
   else {
      D.x /= len;
      D.y /= len;
      D.z /= len;
   }

      /* Figure out the intervals along the ray where each
      component of the blob has an effect. */
      if ((cnt = determine_influences(&P, &D, blob, 0.01)) == 0)
         /* Ray doesn't hit the sphere of influence of any of
         its component elements */
         return 0;

   /* Clear out the coefficients */
   for (i=0;i<4;i++) coeffs[i] = 0.0;
   coeffs[4] = -blob->threshold;

   /* Step through the list of influence points, adding the
      influence of each blob component as it appears */
   for (i=0,in_flag=0;i<cnt;i++) {
      if (intervals[i].type == 0) {
         /* Something is just starting to influence the ray,
            so calculate its coefficients and add them
         into the pot. */
         in_flag++;
         element = blob->list + intervals[i].index;

         VSub(V, P, element->pos);
         c0 = element->coeffs[0];
         c1 = element->coeffs[1];
         c2 = element->coeffs[2];
         VDot(t0, V, V);
         VDot(t1, V, D);
         tcoeffs = &(element->tcoeffs[0]);

         tcoeffs[0] = c0;
         tcoeffs[1] = 4.0 * c0 * t1;
         tcoeffs[2] = 2.0 * c0 * (2.0 * t1 * t1 + t0) + c1;
         tcoeffs[3] = 2.0 * t1 * (2.0 * c0 * t0 + c1);
         tcoeffs[4] = c0 * t0 * t0 + c1 * t0 + c2;

         for (j=0;j<5;j++) coeffs[j] += tcoeffs[j];
      }
      else {
         /* We are losing the influence of a component, so
            subtract off its coefficients */
         tcoeffs = &(blob->list[intervals[i].index].tcoeffs[0]);
         for (j=0;j<5;j++) coeffs[j] -= tcoeffs[j];
         if (--in_flag == 0)
            /* None of the components are currently affecting
               the ray - skip ahead. */
            continue;
      }

      /* Figure out which root solver to use */
      if (blob->Sturm_Flag == 0)
         /* Use Ferrari's method */
         root_count = solve_quartic(coeffs, &roots[0]);
      else
         /* Sturm sequences */
         if (fabs(coeffs[0]) < COEFF_LIMIT)
            if (fabs(coeffs[1]) < COEFF_LIMIT)
               root_count = solve_quadratic(&coeffs[2], &roots[0]);
            else
               root_count = polysolve(3, &coeffs[1], &roots[0]);
         else
            root_count = polysolve(4, coeffs, &roots[0]);

      /* See if any of the roots are valid */
      for(j=0;j<root_count;j++) {
         dist = roots[j];
         /* First see if the root is in the interval of influence of
            the currently active components of the blob */
         if ((dist >= intervals[i].bound) &&
            (dist <= intervals[i+1].bound)) {
            VScale(Intersection_Point, D, dist);
            VAdd(Intersection_Point, Intersection_Point, P);
            if (1 || validate_hit(blob, &Intersection_Point)) {
               /* Only add this hit if it really is near the surface, we can
		  get fooled by numerical inaccuracies */
               /* Transform the point into world space */
               if (blob->Transform != NULL)
                  MTransformVector(&Intersection_Point, &Intersection_Point,
                     blob->Transform);
               VSub(dv, Intersection_Point, Ray->Initial);
               VLength(len, dv);
               Local_Element.Depth = len;
               Local_Element.Object = blob->Parent_Object;
               Local_Element.Point = Intersection_Point;
               Local_Element.Shape = (SHAPE *)blob;
               pq_add (Depth_Queue, &Local_Element);
               Intersection_Found = TRUE;
            }
         }
      }
   }
   if(Intersection_Found)
      Ray_Blob_Tests_Succeeded++;
   return Intersection_Found;
}

/* Calculate the density at this point, then compare to
   the threshold to see if we are in or out of the blob */
int Inside_Blob (Test_Point, Object)
VECTOR *Test_Point;
OBJECT *Object;
{
   VECTOR New_Point;
   BLOB *blob = (BLOB *) Object;

   /* Transform the point into blob space */
   if (blob->Transform != NULL)
      MInverseTransformVector(&New_Point, Test_Point, blob->Transform);
   else
      New_Point = *Test_Point;

   if (calculate_field_value(Object, &New_Point) >
      blob->threshold-INSIDE_TOLERANCE)
      return ((int) 1-blob->Inverted);
   else
      return ((int) blob->Inverted);
}

void Blob_Normal (Result, Object, Intersection_Point)
OBJECT *Object;
VECTOR *Result, *Intersection_Point;
{
   VECTOR New_Point, V;
   int i;
   DBL dist, val;
   BLOB *blob = (BLOB *) Object;
   Blob_Element *temp;

   /* Transform the point into the blobs space */
   if (blob->Transform != NULL)
      MInverseTransformVector(&New_Point, Intersection_Point, blob->Transform);
   else {
      New_Point.x = Intersection_Point->x;
      New_Point.y = Intersection_Point->y;
      New_Point.z = Intersection_Point->z;
   }

      Result->x = 0.0; Result->y = 0.0; Result->z = 0.0;

   /* For each component that contributes to this point, add
      its bit to the normal */
   temp = &(blob->list[0]);
   for(i=0;i<blob->count;i++,temp++) {
      V.x = New_Point.x - temp->pos.x;
      V.y = New_Point.y - temp->pos.y;
      V.z = New_Point.z - temp->pos.z;
      dist = (V.x * V.x + V.y * V.y + V.z * V.z);

      if (dist <= temp->radius2) {
         val = -2.0 * (2.0 * temp->coeffs[0] * dist +
            temp->coeffs[1]);
         Result->x += val * V.x;
         Result->y += val * V.y;
         Result->z += val * V.z;
      }
   }
   val = (Result->x * Result->x + Result->y * Result->y +
      Result->z * Result->z);
   if (val < EPSILON) {
      Result->x = 1.0;
      Result->y = 0.0;
      Result->z = 0.0;
   }
   else {
      val = 1.0 / sqrt(val);
      Result->x *= val;
      Result->y *= val;
      Result->z *= val;
   }

      /* Transform back to world space */
      if (blob->Transform != NULL)
         MTransNormal(Result, Result, blob->Transform);
   VNormalize(*Result, *Result);
}

void *Copy_Blob (Object)
OBJECT *Object;
{
   BLOB *blob;
   BLOB *Old_Shape = (BLOB *)Object;
   TRANSFORMATION *Tr;

   blob = Get_Blob_Shape();
   memcpy(blob, Old_Shape, sizeof(BLOB));
   blob -> Next_Object = NULL;

   /* Allocate space and copy the blob specific data */
   blob->list = (Blob_Element *)malloc(Old_Shape->count * sizeof(Blob_Element));
   if (blob->list == NULL) {
      printf("Failed to allocate blob data\n");
      exit(1);
   }
   memcpy(blob->list, Old_Shape->list, Old_Shape->count * sizeof(Blob_Element));
   blob->intervals = (Blob_Interval *)malloc(2 * blob->count * sizeof(Blob_Interval));
   if (blob->intervals == NULL) {
      printf("Failed to allocate blob data\n");
      exit(1);
   }

   /* Copy any associated transformation */
   if (blob->Transform != NULL) {
      Tr = Get_Transformation();
      memcpy(Tr, blob->Transform, sizeof(TRANSFORMATION));
      blob->Transform = Tr;
   }

   /* Copy any associated texture */
   if (blob->Shape_Texture != NULL)
      blob->Shape_Texture = Copy_Texture (blob->Shape_Texture);

   return (blob);
}

void Translate_Blob (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transform;
   BLOB *blob = (BLOB *)Object;
   if (blob->Transform == NULL)
      blob->Transform = Get_Transformation();
   Get_Translation_Transformation(&Transform, Vector);
   Compose_Transformations(blob->Transform, &Transform);

   Translate_Texture (&((BLOB *) Object)->Shape_Texture, Vector);
}

void Rotate_Blob (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transform;
   BLOB *blob = (BLOB *)Object;
   if (blob->Transform == NULL)
      blob->Transform = Get_Transformation();
   Get_Rotation_Transformation(&Transform, Vector);
   Compose_Transformations(blob->Transform, &Transform);

   Rotate_Texture (&((BLOB *) Object)->Shape_Texture, Vector);
}

void Scale_Blob (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transform;
   BLOB *blob = (BLOB *)Object;
   if (blob->Transform == NULL)
      blob->Transform = Get_Transformation();
   Get_Scaling_Transformation(&Transform, Vector);
   Compose_Transformations(blob->Transform, &Transform);

   Scale_Texture (&((BLOB *) Object)->Shape_Texture, Vector);
}

void Invert_Blob (Object)
OBJECT *Object;
{
   ((BLOB *) Object)->Inverted = 1 - ((BLOB *)Object)->Inverted;
}
