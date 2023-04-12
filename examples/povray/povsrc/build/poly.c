/****************************************************************************
*                poly.c
*
*  This module implements the code for general 3 variable polynomial shapes
*
*  This file was written by Alexander Enzmann.  He wrote the code for
*  4th - 6th order shapes and generously provided us these enhancements.
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

/* Basic form of a quartic equation
   a00*x^4+a01*x^3*y+a02*x^3*z+a03*x^3+a04*x^2*y^2+
   a05*x^2*y*z+a06*x^2*y+a07*x^2*z^2+a08*x^2*z+a09*x^2+
   a10*x*y^3+a11*x*y^2*z+a12*x*y^2+a13*x*y*z^2+a14*x*y*z+
   a15*x*y+a16*x*z^3+a17*x*z^2+a18*x*z+a19*x+a20*y^4+
   a21*y^3*z+a22*y^3+a23*y^2*z^2+a24*y^2*z+a25*y^2+a26*y*z^3+
   a27*y*z^2+a28*y*z+a29*y+a30*z^4+a31*z^3+a32*z^2+a33*z+a34
*/

#define COEFF_LIMIT 1.0e-20

#define LEFT_MARGIN 0
#define RIGHT_MARGIN 72

int binomial[11][12] = {
   { 0, 1,  0,  0,   0,   0,   0,   0,   0, 0,  0,  0},
   { 0, 1,  1,  0,   0,   0,   0,   0,   0, 0,  0,  0},
   { 0, 1,  2,  1,   0,   0,   0,   0,   0, 0,  0,  0},
   { 0, 1,  3,  3,   1,   0,   0,   0,   0, 0,  0,  0},
   { 0, 1,  4,  6,   4,   1,   0,   0,   0, 0,  0,  0},
   { 0, 1,  5, 10,  10,   5,   1,   0,   0, 0,  0,  0},
   { 0, 1,  6, 15,  20,  15,   6,   1,   0, 0,  0,  0},
   { 0, 1,  7, 21,  35,  35,  21,   7,   1, 0,  0,  0},
   { 0, 1,  8, 28,  56,  70,  56,  28,   8, 1,  0,  0},
   { 0, 1,  9, 36,  84, 126, 126,  84,  36, 9,  1,  0},
   { 0, 1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1}};

int factorials[MAX_ORDER+1] = {1, 1, 2, 6, 24, 120, 720, 5040};
int term_counts[MAX_ORDER+1] = {1, 4, 10, 20, 35, 56, 84, 120};

METHODS Poly_Methods =
{ Object_Intersect, All_Poly_Intersections,
   Inside_Poly, Poly_Normal, Copy_Poly,
   Translate_Poly, Rotate_Poly,
   Scale_Poly, Invert_Poly};

extern long Ray_Poly_Tests, Ray_Poly_Tests_Succeeded;

static void transform PARAMS((int order,DBL *Coeffs,MATRIX *q));
static int roll PARAMS((int order,int x,int y,int z));
static void unroll PARAMS((int order,int index,int *x,int *y,int *z,int *w));
/*static void show_poly PARAMS((char *label, int Order, DBL *Coeffs));*/
static int intersect PARAMS((RAY *Ray, int Order, DBL *Coeffs, DBL *Depths));
static int intersect_quartic PARAMS((RAY *Ray, POLY *Shape, DBL *Depths));
static void quartic_normal PARAMS((VECTOR *Result, OBJECT *Object,
VECTOR *Intersection_Point));
static DBL inside PARAMS((VECTOR *point, int Order, DBL *Coeffs));
static void normalp PARAMS((VECTOR *Result, int Order, DBL *Coeffs,
VECTOR *Intersection_Point));
static DBL do_partial_term PARAMS((MATRIX *q, int row, int pwr, int i,
int j, int k, int l));

int All_Poly_Intersections(Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;
PRIOQ *Depth_Queue;
{
   POLY *Shape = (POLY *) Object;
   DBL Depths[MAX_ORDER], len;
   VECTOR Intersection_Point, dv;
   INTERSECTION Local_Element;
   int cnt, i, j, Intersection_Found;
   RAY New_Ray;

   /* Transform the ray into the polynomial's space */
   if (Shape->Transform != NULL) {
      MInverseTransformVector(&New_Ray.Initial, &Ray->Initial,
         Shape->Transform);
      MInvTransVector(&New_Ray.Direction, &Ray->Direction, Shape->Transform);
   }
   else {
      New_Ray.Initial.x = Ray->Initial.x;
      New_Ray.Initial.y = Ray->Initial.y;
      New_Ray.Initial.z = Ray->Initial.z;
      New_Ray.Direction.x = Ray->Direction.x;
      New_Ray.Direction.y = Ray->Direction.y;
      New_Ray.Direction.z = Ray->Direction.z;
   }

      len = sqrt(New_Ray.Direction.x * New_Ray.Direction.x +
         New_Ray.Direction.y * New_Ray.Direction.y +
         New_Ray.Direction.z * New_Ray.Direction.z);
   if (len == 0.0)
      return 0;
   else {
      New_Ray.Direction.x /= len;
      New_Ray.Direction.y /= len;
      New_Ray.Direction.z /= len;
   }

      Intersection_Found = FALSE;
   Ray_Poly_Tests++;
   if (Shape->Order == 4)
      cnt = intersect_quartic(&New_Ray, Shape, Depths);
   else
      cnt = intersect(&New_Ray, Shape->Order, Shape->Coeffs, &Depths[0]);

   if (cnt > 0) Ray_Poly_Tests_Succeeded++;
   for (i=0;i<cnt;i++) {
      if (Depths[i] < 0) goto l0;
      for (j=0;j<i;j++)
         if (Depths[i] == Depths[j]) goto l0;
      VScale(Intersection_Point, New_Ray.Direction, Depths[j]);
      VAdd(Intersection_Point, Intersection_Point, New_Ray.Initial);
      /* Transform the point into world space */
      if (Shape->Transform != NULL)
         MTransformVector(&Intersection_Point, &Intersection_Point,
            Shape->Transform);

      VSub(dv, Intersection_Point, Ray->Initial);
      VLength(len, dv);
      Local_Element.Depth = len;
      Local_Element.Object = Shape->Parent_Object;
      Local_Element.Point = Intersection_Point;
      Local_Element.Shape = (SHAPE *)Shape;
      pq_add (Depth_Queue, &Local_Element);
      Intersection_Found = TRUE;
l0:;
   }
   return (Intersection_Found);
}

/* Given the powers return the index into the polynomial */
static int roll(order, x, y, z)
int order, x, y, z;
{
   int xstart, ystart, zstart;
   xstart = binomial[order-x+2][order-x];
   order = order - x;
   ystart = binomial[order-y+1][order-y];
   order = order - y;
   zstart = binomial[order-z][order-z];
   return xstart+ystart+zstart;
}

/* Given the index into the polynomial, return the powers. */
static void unroll(order, index, x, y, z, w)
int order, index, *x, *y, *z, *w;
{
   int i, torder;
   if (index==0) { *x = order; *y = 0; *z = 0; *w = 0; return; }
   else if (order==1)
      if (index == 1) { *x = 0; *y = 1; *z = 0; *w = 0; return; }
      else { *x = 0; *y = 0; *z = 3 - index; *w = order-(*x+*y+*z); return; }
   else for (i=0;binomial[3+i][i+1]<=index;i++);
   torder = order;
   *x = torder - i; index -= binomial[2+i][i]; torder = i;
   if (index==0) { *y = torder; *z = 0; *w = 0; return; }
   else if (torder==1) { *y = 0; *z = 2-index; *w = order-(*x+*y+*z); return; }
   else for (i=0;binomial[2+i][i+1]<=index;i++);
   *y = torder - i; index -= binomial[1+i][i]; torder = i;
   *z = torder - index;
   *w = order - (*x + *y + *z);
}

   /* REMOVED UNUSED ROUTINE 3/27/92
static void show_poly(label, Order, Coeffs)
   char *label;
   int Order;
   DBL *Coeffs;
{
   int i,j,cnt,col;
   int wp, xp, yp, zp;
   char term[40];
   j = 0;
   printf("%s", label);
   col = strlen(label);


   for (i=0;i<term_counts[Order];i++)
      if (Coeffs[i] != 0.0) {
	 if (j) cnt = STRLN(sprintf(&term[0], " + %.5lg", Coeffs[i]));
	 else cnt = STRLN(sprintf(&term[0], "%.5lg", Coeffs[i]));
	 unroll(Order, i, &xp, &yp, &zp, &wp);
	 if (xp == 1) cnt += STRLN(sprintf(&term[cnt], "*x"));
	 else if (xp > 1) cnt += STRLN(sprintf(&term[cnt], "*x^%d", xp));
	 if (yp == 1) cnt += STRLN(sprintf(&term[cnt], "*y"));
	 else if (yp > 1) cnt += STRLN(sprintf(&term[cnt], "*y^%d", yp));
	 if (zp == 1) cnt += STRLN(sprintf(&term[cnt], "*z"));
	 else if (zp > 1) cnt += STRLN(sprintf(&term[cnt], "*z^%d", zp));
	 j = 1;
	 if (col+cnt>RIGHT_MARGIN) { printf("\n"); col=LEFT_MARGIN; }
	 printf("%s", &term[0]);
	 col += cnt;
	 }
}
*/

/* Intersection of a ray and an arbitrary polynomial function */
static int intersect(Ray, Order, Coeffs, Depths)
RAY *Ray;
int Order;
DBL *Coeffs, *Depths;
{
   MATRIX q;
   DBL *a, t[MAX_ORDER+1];
   int i, j;
   /* Determine the coefficients of t^n, where the line is represented
      as (x,y,z) + (xx,yy,zz)*t.  */
   if ((a = (DBL *)malloc(term_counts[Order] * sizeof(DBL))) == NULL) {
      printf("Cannot allocate memory for coefficients in poly intersect()\n");
      exit(1);
   }
   for (i=0;i<term_counts[Order];i++)
      a[i] = Coeffs[i];
   MZero((MATRIX *)&q[0][0]);
   q[0][0] = Ray->Direction.x; q[3][0] = Ray->Initial.x;
   q[0][1] = Ray->Direction.y; q[3][1] = Ray->Initial.y;
   q[0][2] = Ray->Direction.z; q[3][2] = Ray->Initial.z;
   transform(Order, a, (MATRIX *)&q[0][0]);
   /* The equation is now in terms of one variable.  Use numerical
      techniques to solve the polynomial that represents the intersections. */
   for (i=0;i<=Order;i++) {
      t[i] = a[binomial[3+i][4]-1];
      if (t[i] > -COEFF_LIMIT && t[i] < COEFF_LIMIT)
         t[i] = 0.0;
   }
   free(a);
   for (i=0,j=Order;i<=Order;i++)
      if (t[i] != 0.0)
         break;
      else
         j -= 1;
   if (j > 2)
      return polysolve(j, &t[i], Depths);
   else if (j > 0)
      return solve_quadratic(&t[i], Depths);
   else
      return 0;
}

static DBL inside(point, Order, Coeffs)
VECTOR *point;
int Order;
DBL *Coeffs;
{
   DBL x[MAX_ORDER+1], y[MAX_ORDER+1], z[MAX_ORDER+1], Result;
   int i, k0, k1, k2, k3;
   x[0] = 1.0; y[0] = 1.0; z[0] = 1.0;
   x[1] = point->x; y[1] = point->y; z[1] = point->z;
   for (i=2;i<=MAX_ORDER;i++) {
      x[i] = x[1] * x[i-1]; y[i] = y[1] * y[i-1]; z[i] = z[1] * z[i-1];
   }
   Result = 0.0;
   for (i=0;i<term_counts[Order];i++) {
      unroll(Order, i, &k0, &k1, &k2, &k3);
      Result += Coeffs[i] * x[k0] * y[k1] * z[k2];
   }

   /* The Epsilon fudge factor is so that points really near the
      surface are considered inside the surface */
   return (Result>-EPSILON?(Result<EPSILON?0.0:Result):
   Result);
}

/* Normal to a polynomial */
static void normalp(Result, Order, Coeffs, Intersection_Point)
VECTOR *Result;
int Order;
DBL *Coeffs;
VECTOR *Intersection_Point;
{
   int i, xp, yp, zp, wp;
   DBL *a,x[MAX_ORDER+1],y[MAX_ORDER+1],z[MAX_ORDER+1];
   x[0] = 1.0; y[0] = 1.0; z[0] = 1.0;
   x[1] = Intersection_Point->x;
   y[1] = Intersection_Point->y;
   z[1] = Intersection_Point->z;
   for (i=2;i<=Order;i++) {
      x[i] = Intersection_Point->x * x[i-1];
      y[i] = Intersection_Point->y * y[i-1];
      z[i] = Intersection_Point->z * z[i-1];
   }
   a = Coeffs;
   Result->x = 0.0; Result->y = 0.0; Result->z = 0.0;
   for (i=0;i<term_counts[Order];i++) {
      unroll(Order, i, &xp, &yp, &zp, &wp);
      if (xp >= 1) Result->x += xp * a[i] * x[xp-1] * y[yp] * z[zp];
      if (yp >= 1) Result->y += yp * a[i] * x[xp] * y[yp-1] * z[zp];
      if (zp >= 1) Result->z += zp * a[i] * x[xp] * y[yp] * z[zp-1];
   }
   VTemp = sqrt(Result->x * Result->x +
      Result->y * Result->y +
      Result->z * Result->z);
   if (VTemp > 0.0) {
      Result->x /= VTemp;
      Result->y /= VTemp;
      Result->z /= VTemp;
   }
   else {
      Result->x = 1.0;
      Result->y = 0.0;
      Result->z = 0.0;
   }
}

static DBL
do_partial_term(q, row, pwr, i, j, k, l)
MATRIX *q;
int row, pwr, i, j, k, l;
{
   DBL result;
   int n;

   result = (DBL)(factorials[pwr] /
      (factorials[i]*factorials[j]*factorials[k]*factorials[l]));
   if (i>0) for (n=0;n<i;n++) result *= (*q)[0][row];
   if (j>0) for (n=0;n<j;n++) result *= (*q)[1][row];
   if (k>0) for (n=0;n<k;n++) result *= (*q)[2][row];
   if (l>0) for (n=0;n<l;n++) result *= (*q)[3][row];
   return result;
}

/* Using the transformation matrix q, transform the general polynomial
   equation given by a. */

static void transform(order, Coeffs, q)
int order;
DBL *Coeffs;
MATRIX *q;
{
   int term_index, partial_index;
   int ip, i, i0, i1, i2, i3;
   int jp, j, j0, j1, j2, j3;
   int kp, k, k0, k1, k2, k3;
   int wp;
   DBL *b, partial_term;
   DBL tempx, tempy, tempz;

   for (i=0;i<4;i++) for (j=0;j<4;j++)
      if ((*q)[i][j] > -COEFF_LIMIT && (*q)[i][j] < COEFF_LIMIT)
         (*q)[i][j] = 0.0;
   if ((b = (DBL *)malloc(term_counts[order] * sizeof(DBL))) == NULL) {
      printf("Cannot allocate memory for b in poly transform()\n");
      exit(1);
   }
   for (i=0;i<term_counts[order];i++)
      b[i] = 0.0;
   for (term_index=0;term_index<term_counts[order];term_index++) {
      if (Coeffs[term_index] != 0.0) {
         unroll(order, term_index, &ip, &jp, &kp, &wp);
         /* Step through terms in: (q[0][0]*x+q[0][1]*y+q[0][2]*z+q[0][3])^i */
         for (i=0;i<term_counts[ip];i++) {
            unroll(ip, i, &i0, &i1, &i2, &i3);
            tempx = do_partial_term(q, 0, ip, i0, i1, i2, i3);
            if (tempx != 0.0) {

               /* Step through terms in:
			(q[1][0]*x+q[1][1]*y+q[1][2]*z+q[1][3])^j */
               for (j=0;j<term_counts[jp];j++) {
                  unroll(jp, j, &j0, &j1, &j2, &j3);
                  tempy = do_partial_term(q, 1, jp, j0, j1, j2, j3);
                  if (tempy != 0.0) {

                     /* Step through terms in:
			      (q[2][0]*x+q[2][1]*y+q[2][2]*z+q[2][3])^k */
                     for (k=0;k<term_counts[kp];k++) {
                        unroll(kp, k, &k0, &k1, &k2, &k3);
                        tempz = do_partial_term(q, 2, kp, k0, k1, k2, k3);
                        if (tempz != 0.0) {
                           /* Figure out it's index, and add into result */
                           partial_index=roll(order,i0+j0+k0,i1+j1+k1,i2+j2+k2);
                           partial_term=Coeffs[term_index]*tempx*tempy*tempz;
                           b[partial_index] += partial_term;
                        }
                     }
                  }
               }
            }
         }
      }
   }
   for (i=0;i<term_counts[order];i++)
      if (b[i] > -1.0e-4 && b[i] < 1.0e-4)
         Coeffs[i] = 0.0;
      else
         Coeffs[i] = b[i];
   free(b);
   /*show_poly("New poly: ", order, Coeffs);
printf("\n");
*/
}

/* Intersection of a ray and a quartic */
static int intersect_quartic(Ray, Shape, Depths)
RAY *Ray;
POLY *Shape;
DBL *Depths;
{
   DBL x,y,z,x2,y2,z2,x3,y3,z3,x4,y4,z4;
   DBL xx,yy,zz,xx2,yy2,zz2,xx3,yy3,zz3,xx4,yy4,zz4;
   DBL *a, t[5];
   DBL x_z,x_zz,xx_z,xx_zz,x_y,x_yy,xx_y,xx_yy,y_z,y_zz,yy_z,yy_zz,temp;

   x = Ray->Initial.x;
   y = Ray->Initial.y;
   z = Ray->Initial.z;
   xx = Ray->Direction.x;
   yy = Ray->Direction.y;
   zz = Ray->Direction.z;
   x2 = x * x;  y2 = y * y;  z2 = z * z;
   x3 = x * x2; y3 = y * y2; z3 = z * z2;
   x4 = x * x3; y4 = y * y3; z4 = z * z3;
   xx2 = xx * xx;  yy2 = yy * yy;  zz2 = zz * zz;
   xx3 = xx * xx2; yy3 = yy * yy2; zz3 = zz * zz2;
   xx4 = xx * xx3; yy4 = yy * yy3; zz4 = zz * zz3;
   a = Shape->Coeffs;
   x_z = x * z; x_zz = x * zz; xx_z = xx * z; xx_zz = xx * zz;
   x_y = x * y; x_yy = x * yy; xx_y = xx * y; xx_yy = xx * yy;
   y_z = y * z; y_zz = y * zz; yy_z = yy * z; yy_zz = yy * zz;

   /*
      Determine the coefficients of t^n, where the line is represented
      as (x,y,z) + (xx,yy,zz)*t.
   */
   temp =  a[ 0]*xx4;
   temp += a[ 1]*xx3*yy;
   temp += a[ 2]*xx3*zz;
   temp += a[ 4]*xx2*yy2;
   temp += a[ 5]*xx2*yy_zz;
   temp += a[ 7]*xx2*zz2;
   temp += a[10]*xx*yy3;
   temp += a[11]*xx_zz*yy2;
   temp += a[13]*xx_yy*zz2;
   temp += a[16]*xx*zz3;
   temp += a[20]*yy4;
   temp += a[21]*yy3*zz;
   temp += a[23]*yy2*zz2;
   temp += a[26]*yy*zz3;
   temp += a[30]*zz4;

   t[0] = temp;

   temp = 4*a[ 0]*x*xx3;
   temp += a[ 1]*(3*xx2*x_yy+xx3*y);
   temp += a[ 2]*(3*xx2*x_zz+xx3*z);
   temp += a[ 3]*xx3;
   temp += a[ 4]*(2*x*xx*yy2+2*xx2*y*yy);
   temp += a[ 5]*(xx2*(y_zz+yy_z)+2*x_yy*xx_zz);
   temp += a[ 6]*xx2*yy;
   temp += a[ 7]*(2*x*xx*zz2+2*xx2*z*zz);
   temp += a[ 8]*xx2*zz;
   temp += a[10]*(x*yy3+3*xx_y*yy2);
   temp += a[11]*(xx*(2*y*yy_zz+yy2*z)+x_zz*yy2);
   temp += a[12]*xx*yy2;
   temp += a[13]*(xx*(y*zz2+2*yy_z*zz)+x_yy*zz2);
   temp += a[14]*xx_yy*zz;
   temp += a[16]*(x*zz3+3*xx_z*zz2);
   temp += a[17]*xx*zz2;
   temp += 4*a[20]*y*yy3;
   temp += a[21]*(3*yy2*y_zz+yy3*z);
   temp += a[22]*yy3;
   temp += zz*(2*a[23]*yy*(y_zz+yy_z)+
      a[24]*yy2+
      zz*(a[26]*(y_zz+3*yy_z)+
      a[27]*yy+zz*(4*a[30]*z+a[31])));
   t[1] = temp;

   temp = 6*a[ 0]*x2*xx2;
   temp += 3*a[ 1]*x*xx*(x_yy+xx_y);
   temp += 3*a[ 2]*x*xx*(x_zz+xx_z);
   temp += 3*a[ 3]*x*xx2;
   temp += a[ 4]*(x2*yy2+4*x_yy*xx_y+xx2*y2);
   temp += a[ 5]*(x2*yy_zz+2*x*xx*(y_zz+yy_z)+xx2*y_z);
   temp += a[ 6]*(2*x*xx_yy+xx2*y);
   temp += a[ 7]*(x2*zz2+4*x_zz*xx_z+xx2*z2);
   temp += a[ 8]*(2*x*xx_zz+xx2*z);
   temp += a[ 9]*xx2;
   temp += a[10]*(3*x_y*yy2+3*xx_yy*y2);
   temp += a[11]*(x_yy*(2*y_zz+yy_z)+xx*(y2*zz+2*y*yy_z));
   temp += a[12]*(x*yy2+2*xx_y*yy);
   temp += a[13]*(x_zz*(y_zz+2*yy_z)+xx*(2*y_z*zz+yy*z2));
   temp += a[14]*(x_yy*zz+xx*(y_zz+yy_z));
   temp += a[15]*xx_yy;
   temp += a[16]*(3*x_z*zz2+3*xx_zz*z2);
   temp += a[17]*(x*zz2+2*xx_z*zz);
   temp += a[18]*xx_zz;
   temp += 6*a[20]*y2*yy2;
   temp += 3*a[21]*y*yy*(y_zz+yy_z);
   temp += 3*a[22]*y*yy2;
   temp += a[23]*(y2*zz2+4*y_zz*yy_z+yy2*z2);
   temp += a[24]*(2*y*yy_zz+yy2*z);
   temp += a[25]*yy2;
   temp += zz*(3*a[26]*z*(y_zz+yy_z)+
      a[27]*(y_zz+2*yy_z)+
      a[28]*yy+
      6*a[30]*z2*zz+
      3*a[31]*z*zz+
      a[32]*zz);
   t[2] = temp;

   temp = 4*a[ 0]*x3*xx;
   temp += a[ 1]*x2*(x_yy+3*xx_y);
   temp += a[ 2]*x2*(x_zz+3*xx_z);
   temp += 3*a[ 3]*x2*xx;
   temp += 2*a[ 4]*x_y*(x_yy+xx_y);
   temp += a[ 5]*x*(x*(y_zz+yy_z)+2*xx_y*z);
   temp += a[ 6]*x*(x_yy+2*xx_y);
   temp += 2*a[ 7]*x_z*(x_zz+xx_z);
   temp += a[ 8]*x*(x_zz+2*xx_z);
   temp += 2*a[ 9]*x*xx;
   temp += a[10]*(3*x_yy*y2-xx*y3);
   temp += a[11]*(x_y*(y_zz+2*yy_z)+xx_z*y2);
   temp += a[12]*(2*x_y*yy+xx*y2);
   temp += a[13]*(x_z*(2*y_zz+yy_z)+xx_y*z2);
   temp += a[14]*(x*(y_zz+yy_z)+xx_y*z);
   temp += a[15]*(x_yy+xx_y);
   temp += a[16]*(3*x_zz*z2+xx*z3);
   temp += a[17]*(2*x_z*zz+xx*z2);
   temp += a[18]*(x_zz+xx_z);
   temp += a[19]*xx;
   temp += 4*a[20]*y3*yy;
   temp += a[21]*y2*(y_zz+3*yy_z);
   temp += 3*a[22]*y2*yy;
   temp += 2*a[23]*y_z*(y_zz+yy_z);
   temp += a[24]*y*(y_zz+2*yy_z);
   temp += 2*a[25]*y*yy;
   temp += a[26]*(3*y_zz*z2+yy*z3);
   temp += a[27]*(2*y_z*zz+yy*z2);
   temp += a[28]*(y_zz+yy_z);
   temp += a[29]*yy;
   temp += zz*(4*a[30]*z3+3*a[31]*z2+2*a[32]*z+a[33]);
   t[3] = temp;

   temp = a[ 0]*x4;
   temp += a[ 1]*x3*y;
   temp += a[ 2]*x3*z;
   temp += a[ 3]*x3;
   temp += a[ 4]*x2*y2;
   temp += a[ 5]*x2*y_z;
   temp += a[ 6]*x2*y;
   temp += a[ 7]*x2*z2;
   temp += a[ 8]*x2*z;
   temp += a[ 9]*x2;
   temp += a[10]*x*y3;
   temp += a[11]*x*y2*z;
   temp += a[12]*x*y2;
   temp += a[13]*x_y*z2;
   temp += a[14]*x_y*z;
   temp += a[15]*x_y;
   temp += a[16]*x*z3;
   temp += a[17]*x*z2;
   temp += a[18]*x_z;
   temp += a[19]*x;
   temp += a[20]*y4;
   temp += a[21]*y3*z;
   temp += a[22]*y3;
   temp += a[23]*y2*z2;
   temp += a[24]*y2*z;
   temp += a[25]*y2;
   temp += a[26]*y*z3;
   temp += a[27]*y*z2;
   temp += a[28]*y_z;
   temp += a[29]*y;
   temp += a[30]*z4;
   temp += a[31]*z3;
   temp += a[32]*z2;
   temp += a[33]*z;
   temp += a[34];
   t[4] = temp;

   if (Shape->Sturm_Flag)
      if (t[0] == 0.0)
         if (t[1] == 0.0)
            return solve_quadratic(&t[2], Depths);
         else
            return polysolve(3, &t[1], Depths);
      else
         return polysolve(4, &t[0], Depths);
   else
      return solve_quartic(&t[0], Depths);
}

#ifdef TEST
/* Normal to a quartic */
static void quartic_normal(Result, Object, Intersection_Point)
VECTOR *Result, *Intersection_Point;
OBJECT *Object;
{
   POLY *Shape = (POLY *) Object;
   DBL *a,x,y,z,x2,y2,z2,x3,y3,z3,temp;

   a = Shape->Coeffs;
   x = Intersection_Point->x;
   y = Intersection_Point->y;
   z = Intersection_Point->z;
   x2 = x * x;  y2 = y * y;  z2 = z * z;
   x3 = x * x2; y3 = y * y2; z3 = z * z2;

   temp = a[ 4]*y2+y*(a[ 5]*z+a[ 6])+a[ 7]*z2+a[ 8]*z+a[ 9];
   Result->x = 4*a[ 0]*x3+3*x2*(a[ 1]*y+a[ 2]*z+a[ 3])+
   2*x*(temp)+
   a[10]*y3+y2*(a[11]*z+a[12])+y*(a[13]*z2+a[14]*z+a[15])+
   a[16]*z3+a[17]*z2+a[18]*z+a[19];

   temp = 3*a[10]*y2+2*y*(a[11]*z+a[12])+a[13]*z2+a[14]*z+a[15];
   Result->y = a[ 1]*x3+x2*(2*a[ 4]*y+a[ 5]*z+a[ 6])+
   x*(temp)+
   4*a[20]*y3+3*y2*(a[21]*z+a[22])+2*y*(a[23]*z2+a[24]*z+a[25])+
   a[26]*z3+a[27]*z2+a[28]*z+a[29];

   temp = a[11]*y2+y*(2*a[13]*z+a[14])+3*a[16]*z2+2*a[17]*z+a[18];
   Result->z = a[ 2]*x3+x2*(a[ 5]*y+2*a[ 7]*z+a[ 8])+
   x*(temp)+
   a[21]*y3+y2*(2*a[23]*z+a[24])+y*(3*a[26]*z2+2*a[27]*z+a[28])+
   4*a[30]*z3+3*a[31]*z2+2*a[32]*z+a[33];

   VTemp=Result->x * Result->x + Result->y * Result->y + Result->z * Result->z;
   VTemp=sqrt(VTemp);
   if (VTemp > 0.0) {
      Result->x /= VTemp;
      Result->y /= VTemp;
      Result->z /= VTemp;
   }
   else {
      Result->x = 1.0;
      Result->y = 0.0;
      Result->z = 0.0;
   }
}
#endif

/* Normal to a quartic */
static void quartic_normal(Result, Object, Intersection_Point)
VECTOR *Result, *Intersection_Point;
OBJECT *Object;
{
   POLY *Shape = (POLY *) Object;
   DBL *a,x,y,z,x2,y2,z2,x3,y3,z3;

   a = Shape->Coeffs;
   x = Intersection_Point->x;
   y = Intersection_Point->y;
   z = Intersection_Point->z;
   x2 = x * x;  y2 = y * y;  z2 = z * z;
   x3 = x * x2; y3 = y * y2; z3 = z * z2;

   Result->x = 4*a[ 0]*x3+3*x2*(a[ 1]*y+a[ 2]*z+a[ 3])+
   2*x*(a[ 4]*y2+y*(a[ 5]*z+a[ 6])+a[ 7]*z2+a[ 8]*z+a[ 9])+
   a[10]*y3+y2*(a[11]*z+a[12])+y*(a[13]*z2+a[14]*z+a[15])+
   a[16]*z3+a[17]*z2+a[18]*z+a[19];

   Result->y = a[ 1]*x3+x2*(2*a[ 4]*y+a[ 5]*z+a[ 6])+
   x*(3*a[10]*y2+2*y*(a[11]*z+a[12])+a[13]*z2+a[14]*z+a[15])+
   4*a[20]*y3+3*y2*(a[21]*z+a[22])+2*y*(a[23]*z2+a[24]*z+a[25])+
   a[26]*z3+a[27]*z2+a[28]*z+a[29];

   Result->z = a[ 2]*x3+x2*(a[ 5]*y+2*a[ 7]*z+a[ 8])+
   x*(a[11]*y2+y*(2*a[13]*z+a[14])+3*a[16]*z2+2*a[17]*z+a[18])+
   a[21]*y3+y2*(2*a[23]*z+a[24])+y*(3*a[26]*z2+2*a[27]*z+a[28])+
   4*a[30]*z3+3*a[31]*z2+2*a[32]*z+a[33];
   VTemp = sqrt(Result->x*Result->x+Result->y*Result->y+Result->z*Result->z);
   if (VTemp > 0.0) {
      Result->x /= VTemp;
      Result->y /= VTemp;
      Result->z /= VTemp;
   }
   else {
      Result->x = 1.0;
      Result->y = 0.0;
      Result->z = 0.0;
   }
}

int Inside_Poly (Test_Point, Object)
VECTOR *Test_Point;
OBJECT *Object;
{
   VECTOR New_Point;
   POLY *Shape = (POLY *) Object;
   DBL Result;

   /* Transform the point into polynomial's space */
   if (Shape->Transform != NULL)
      MInverseTransformVector(&New_Point, Test_Point, Shape->Transform);
   else
      New_Point = *Test_Point;

   Result = inside(&New_Point, Shape->Order, Shape->Coeffs);
   if (Result < Small_Tolerance)
      return ((int)(1-Shape->Inverted));
   else
      return ((int)Shape->Inverted);
}

/* Normal to a polynomial */
void Poly_Normal(Result, Object, Intersection_Point)
VECTOR *Result, *Intersection_Point;
OBJECT *Object;
{
   POLY *Shape = (POLY *) Object;
   VECTOR New_Point;

   /* Transform the point into the polynomials space */
   if (Shape->Transform != NULL)
      MInverseTransformVector(&New_Point, Intersection_Point, Shape->Transform);
   else {
      New_Point.x = Intersection_Point->x;
      New_Point.y = Intersection_Point->y;
      New_Point.z = Intersection_Point->z;
   }

      if (Shape->Order == 4)
         quartic_normal(Result, Object, &New_Point);
      else
         normalp(Result, Shape->Order, Shape->Coeffs, &New_Point);

   /* Transform back to world space */
   if (Shape->Transform != NULL)
      MTransNormal(Result, Result, Shape->Transform);
   VNormalize(*Result, *Result);
}

/* Make a copy of a polynomial object */
void *Copy_Poly(Object)
OBJECT *Object;
{
   POLY *Shape = (POLY *)Object;
   POLY *New_Shape = Get_Poly_Shape(Shape->Order);
   int i;

   New_Shape->Shape_Texture = Shape->Shape_Texture;
   New_Shape->Shape_Colour = Shape->Shape_Colour;
   New_Shape->Next_Object = NULL;
   New_Shape->Sturm_Flag = Shape->Sturm_Flag;
   New_Shape->Inverted = Shape->Inverted;

   /* Copy any associated transformation */
   if (Shape->Transform != NULL) {
      New_Shape->Transform = Get_Transformation();
      memcpy(New_Shape->Transform, Shape->Transform, sizeof(TRANSFORMATION));
   }
   for (i=0;i<term_counts[New_Shape->Order];i++)
      New_Shape->Coeffs[i] = Shape->Coeffs[i];

   /* Copy any associated texture */
   if (Shape->Shape_Texture != NULL)
      New_Shape->Shape_Texture = Copy_Texture (Shape->Shape_Texture);

   return (void *)(New_Shape);
}

void Translate_Poly (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transform;
   POLY *Shape = (POLY *)Object;
   if (Shape->Transform == NULL)
      Shape->Transform = Get_Transformation();
   Get_Translation_Transformation(&Transform, Vector);
   Compose_Transformations(Shape->Transform, &Transform);

   Translate_Texture (&Shape->Shape_Texture, Vector);
}

void Rotate_Poly (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transform;
   POLY *Shape = (POLY *)Object;
   if (Shape->Transform == NULL)
      Shape->Transform = Get_Transformation();
   Get_Rotation_Transformation(&Transform, Vector);
   Compose_Transformations(Shape->Transform, &Transform);

   Rotate_Texture (&Shape->Shape_Texture, Vector);
}

void Scale_Poly (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transform;
   POLY *Shape = (POLY *)Object;
   if (Shape->Transform == NULL)
      Shape->Transform = Get_Transformation();
   Get_Scaling_Transformation(&Transform, Vector);
   Compose_Transformations(Shape->Transform, &Transform);

   Scale_Texture (&Shape->Shape_Texture, Vector);
}

void Invert_Poly (Object)
OBJECT *Object;
{
   ((POLY *) Object)->Inverted = 1 - ((POLY *)Object)->Inverted;
}
