/****************************************************************************
*                vect.c
*
*  This file was written by Alexander Enzmann.  He wrote the code for
*  4th-6th order shapes and generously provided us these enhancements.
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
#include "povproto.h"
#include "vector.h"

#ifdef ABS
#undef ABS
#endif

#ifdef MAX
#undef MAX
#endif

#undef EPSILON
#define EPSILON 1.0e-10
#define COEFF_LIMIT 1.0e-20

/*                  WARNING     WARNING    WARNING

   The following three constants have been defined so that quartic equations
   will properly render on fpu/compiler combinations that only have 64 bit
   IEEE precision.  Do not arbitrarily change any of these values.

   If you have a machine with a proper fpu/compiler combo - like a Mac with
   a 68881, then use the native floating format (96 bits) and tune the
   values for a little less tolerance: something like: factor1 = 1.0e15,
   factor2 = -1.0e-7, factor3 = 1.0e-10.

   The meaning of the three constants are:
      factor1 - the magnitude of difference between coefficients in a
                quartic equation at which the Sturmian root solver will
		kick in.  The Sturm solver is quite a bit slower than
		the algebraic solver, so the bigger this is, the faster
		the root solving will go.  The algebraic solver is less
		accurate so the bigger this is, the more likely you will
		get bad roots.

      factor2 - Tolerance value that defines how close the quartic equation
		is to being a square of a quadratic.  The closer this can
		get to zero before roots disappear, the less the chance
		you will get spurious roots.

      factor3 - Similar to factor2 at a later stage of the algebraic solver.
*/
#define FUDGE_FACTOR1 1.0e11
#define FUDGE_FACTOR2 -1.0e-5
#define FUDGE_FACTOR3 1.0e-7

#define ABS(x) ((x) < 0.0 ? (0.0 - x) : (x))
#define MAX(x,y) (x<y?y:x)
#define TWO_PI 6.283185207179586476925286766560
#define TWO_PI_3  2.0943951023931954923084
#define TWO_PI_43 4.1887902047863909846168
#define MAX_ITERATIONS 50
#define MAXPOW 32

typedef struct p {
   int ord;
   DBL coef[MAX_ORDER+1];
} polynomial;

static int modp PARAMS((polynomial *u, polynomial *v, polynomial *r));
int regula_falsa PARAMS((int order, DBL *coef, DBL a, DBL b, DBL *val));
void sbisect PARAMS((int np, polynomial *sseq, DBL min, DBL max, int atmin, int atmax, DBL *roots));
int numchanges PARAMS((int np, polynomial *sseq, DBL a));
DBL polyeval PARAMS((DBL x, int n, DBL *Coeffs));
int buildsturm PARAMS((int ord, polynomial *sseq));
int visible_roots PARAMS((int np, polynomial *sseq, int *atneg, int *atpos));
static int difficult_coeffs PARAMS((int n, DBL *x));

extern int Shadow_Test_Flag;

/*
 * modp
 *
 *   calculates the modulus of u(x) / v(x) leaving it in r, it
 *  returns 0 if r(x) is a constant.
 *  note: this function assumes the leading coefficient of v 
 *   is 1 or -1
 */
static int modp(u, v, r)
polynomial   *u, *v, *r;
{
   int    i, k, j;
   for (i=0;i<u->ord;i++)
      r[i] = u[i];

   if (v->coef[v->ord] < 0.0) {
      for (k = u->ord - v->ord - 1; k >= 0; k -= 2)
         r->coef[k] = -r->coef[k];
      for (k = u->ord - v->ord; k >= 0; k--)
         for (j = v->ord + k - 1; j >= k; j--)
            r->coef[j] = -r->coef[j] - r->coef[v->ord + k] * v->coef[j - k];
   }
   else {
      for (k = u->ord - v->ord; k >= 0; k--)
            for (j = v->ord + k - 1; j >= k; j--)
               r->coef[j] -= r->coef[v->ord + k] * v->coef[j - k];
   }

   k = v->ord - 1;
   while (k >= 0 && fabs(r->coef[k]) < COEFF_LIMIT) {
      r->coef[k] = 0.0;
      k--;
   }
   r->ord = (k < 0) ? 0 : k;
   return(r->ord);
}

/* Build the sturmian sequence for a polynomial */
int buildsturm(ord, sseq)
int      ord;
polynomial   *sseq;
{
   int      i;
   DBL   f, *fp, *fc;
   polynomial   *sp;

   sseq[0].ord = ord;
   sseq[1].ord = ord - 1;

   /* calculate the derivative and normalize the leading coefficient. */
   f = fabs(sseq[0].coef[ord] * ord);
   fp = sseq[1].coef;
   fc = sseq[0].coef + 1;
   for (i = 1; i <= ord; i++)
      *fp++ = *fc++ * i / f;

   /* construct the rest of the Sturm sequence */
   for (sp = sseq + 2;modp(sp - 2, sp - 1, sp); sp++) {
      /* reverse the sign and normalize */
      f = -fabs(sp->coef[sp->ord]);
      for (fp = &sp->coef[sp->ord]; fp >= sp->coef; fp--)
         *fp /= f;
   }
   sp->coef[0] = -sp->coef[0];   /* reverse the sign */
   return(sp - sseq);
}

/* Find out how many visible intersections there are */
int visible_roots(np, sseq, atzer, atpos)
int np;
polynomial *sseq;
int *atzer, *atpos;
{
   int atposinf, atzero;
   polynomial *s;
   DBL f, lf;

   atposinf = atzero = 0;
   /* changes at positve infinity */
   lf = sseq[0].coef[sseq[0].ord];
   for (s = sseq + 1; s <= sseq + np; s++) {
      f = s->coef[s->ord];
      if (lf == 0.0 || lf * f < 0)
         atposinf++;
      lf = f;
   }

   /* Changes at zero */
   lf = sseq[0].coef[0];
   for (s = sseq+1; s <= sseq + np; s++) {
      f = s->coef[0];
      if (lf == 0.0 || lf * f < 0)
         atzero++;
      lf = f;
   }

   *atzer = atzero;
   *atpos = atposinf;
   return(atzero - atposinf);
}

/*
 * numchanges
 *
 *   return the number of sign changes in the Sturm sequence in
 * sseq at the value a.
 */
int numchanges(np, sseq, a)
int      np;
polynomial   *sseq;
DBL   a;

{
   int      changes;
   DBL   f, lf;
   polynomial   *s;
   changes = 0;
   lf = polyeval(a, sseq[0].ord, sseq[0].coef);
   for (s = sseq + 1; s <= sseq + np; s++) {
      f = polyeval(a, s->ord, s->coef);
      if (lf == 0.0 || lf * f < 0)
         changes++;
      lf = f;
   }
   return(changes);
}

/*
 * sbisect
 *
 *   uses a bisection based on the sturm sequence for the polynomial
 * described in sseq to isolate intervals in which roots occur,
 * the roots are returned in the roots array in order of magnitude.

Note: This routine has one severe bug: When the interval containing the
      root [min, max] has a root at one of its endpoints, as well as one
      within the interval, the root at the endpoint will be returned rather
      than the one inside.

 */
void sbisect(np, sseq, min_value, max_value, atmin, atmax, roots)
int      np;
polynomial   *sseq;
DBL   min_value, max_value;
int      atmin, atmax;
DBL   *roots;
{
   DBL  mid;
   int  n1, n2, its, atmid, nroot;

   if ((nroot = atmin - atmax) == 1) {
      /* first try using regula-falsa to find the root.  */
      if (regula_falsa(sseq->ord, sseq->coef, min_value, max_value, roots))
         return;
      else {
         /* That failed, so now find it by bisection */
            for (its = 0; its < MAX_ITERATIONS; its++) {
               mid = (min_value + max_value) / 2;
               atmid = numchanges(np, sseq, mid);
               if (fabs(mid) > EPSILON) {
                  if (fabs((max_value - min_value) / mid) < EPSILON) {
                     roots[0] = mid;
                     return;
                  }
               }
               else if (fabs(max_value - min_value) < EPSILON) {
                  roots[0] = mid;
                  return;
               }
               else if ((atmin - atmid) == 0)
                  min_value = mid;
               else
                  max_value = mid;
            }
         /* Bisection took too long - just return what we got */
         roots[0] = mid;
         return;
      }
   }

   /* There is more than one root in the interval.
      Bisect to find new intervals */
   for (its = 0; its < MAX_ITERATIONS; its++) {
      mid = (min_value + max_value) / 2;
      atmid = numchanges(np, sseq, mid);
      n1 = atmin - atmid;
      n2 = atmid - atmax;
      if (n1 != 0 && n2 != 0) {
         sbisect(np, sseq, min_value, mid, atmin, atmid, roots);
         sbisect(np, sseq, mid, max_value, atmid, atmax, &roots[n1]);
         return;
      }
      if (n1 == 0)
         min_value = mid;
      else
         max_value = mid;
   }

   /* Took too long to bisect.  Just return what we got. */
   for (n1 = atmax; n1 < atmin; n1++)
      roots[n1 - atmax] = mid;
}

DBL polyeval(x, n, Coeffs)
DBL x, *Coeffs;
int n;
{
   register int i;
   DBL val;
   val = Coeffs[n];
   for (i=n-1;i>=0;i--) val = val * x + Coeffs[i];
   return val;
}

/* Close in on a root by using regula-falsa */
int regula_falsa(order, coef, a, b, val)
int order;
DBL *coef;
DBL a, b, *val;
{
   int its;
   DBL fa, fb, x, lx, fx, lfx;

   fa = polyeval(a, order, coef);
   fb = polyeval(b, order, coef);

   if (fa * fb > 0.0)
      return 0;

   if (fabs(fa) < COEFF_LIMIT) {
      *val = a;
      return 1;
   }

   if (fabs(fb) < COEFF_LIMIT) {
      *val = b;
      return 1;
   }

   lfx = fa;
   lx = a;

   for (its = 0; its < MAX_ITERATIONS; its++) {
      x = (fb * a - fa * b) / (fb - fa);
      fx = polyeval(x, order, coef);

      if (fabs(x) > EPSILON) {
         if (fabs(fx / x) < EPSILON) {
            *val = x;
            return 1;
         }
      }
      else if (fabs(fx) < EPSILON) {
         *val = x;
         return 1;
      }

      if (fa < 0)
         if (fx < 0) {
            a = x;
            fa = fx;
            if ((lfx * fx) > 0)
               fb /= 2;
         }
         else {
            b = x;
            fb = fx;
            if ((lfx * fx) > 0)
               fa /= 2;
         }
      else if (fx < 0) {
         b = x;
         fb = fx;
         if ((lfx * fx) > 0)
            fa /= 2;
      }
      else {
         a = x;
         fa = fx;
         if ((lfx * fx) > 0)
            fb /= 2;
      }
      if (fabs(b-a) < EPSILON) {
         /* Check for underflow in the domain */
         *val = x;
         return 1;
      }
      lx = x;
      lfx = fx;
   }
   return 0;
}

/*
   Solve the quadratic equation:
      		x[0] * x^2 + x[1] * x + x[2] = 0.

   The value returned by this function is the number of real roots.
   The roots themselves are returned in y[0], y[1].
*/
solve_quadratic(x, y)
DBL *x, *y;
{
   DBL d, t, a, b, c;
   a = x[0];
   b = -x[1];
   c = x[2];
   if (a == 0.0) {
      if (b == 0.0)
         return 0;
      y[0] = c / b;
      return 1;
   }
   d = b * b - 4.0 * a * c;
   if (d < 0.0)
      return 0;
   else if (fabs(d) < COEFF_LIMIT) {
      y[0] = 0.5 * b / a;
      return 1;
   }
   d = sqrt(d);
   t = 2.0 * a;
   y[0] = (b + d) / t;
   y[1] = (b - d) / t;
   return 2;
}

/*
   Solve the cubic equation:

      x[0] * x^3 + x[1] * x^2 + x[2] * x + x[3] = 0.

   The result of this function is an integer that tells how many real
   roots exist.  Determination of how many are distinct is up to the
   process that calls this routine.  The roots that exist are stored
   in (y[0], y[1], y[2]).

   Note: this function relies very heavily on trigonometric functions and
   the square root function.  If an alternative solution is found that does
   not rely on transcendentals this code will be replaced.
*/
int solve_cubic(x, y)
DBL *x, *y;
{
   DBL Q, R, Q3, R2, sQ, d, an, theta;
   DBL A2, a0, a1, a2, a3;
   a0 = x[0];
   if (a0 == 0.0) return solve_quadratic(&x[1], y);
   else if (a0 != 1.0) {
      a1 = x[1] / a0;
      a2 = x[2] / a0;
      a3 = x[3] / a0;
   }
   else {
      a1 = x[1];
      a2 = x[2];
      a3 = x[3];
   }
   A2 = a1 * a1;
   Q = (A2 - 3.0 * a2) / 9.0;
   R = (2.0 * A2 * a1 - 9.0 * a1 * a2 + 27.0 * a3) / 54.0;
   Q3 = Q * Q * Q;
   R2 = R * R;
   d = Q3 - R2;
   an = a1 / 3.0;
   if (d >= 0.0) {
      /* Three real roots. */
      d = R / sqrt(Q3);
      theta = acos(d) / 3.0;
      sQ = -2.0 * sqrt(Q);
      y[0] = sQ * cos(theta) - an;
      y[1] = sQ * cos(theta + TWO_PI_3) - an;
      y[2] = sQ * cos(theta + TWO_PI_43) - an;
      return 3;
   }
   else {
      sQ = pow(sqrt(R2 - Q3) + ABS(R), 1.0 / 3.0);
      if (R < 0)
         y[0] = (sQ + Q / sQ) - an;
      else
         y[0] = -(sQ + Q / sQ) - an;
      return 1;
   }
}

/* Test to see if any coeffs are more than 6 orders of magnitude
   larger than the smallest */
static int
difficult_coeffs(n, x)
int n;
DBL *x;
{
   int i;
   DBL biggest;

   biggest = 0.0;
   for (i=0;i<=n;i++)
      if (fabs(x[i]) > biggest)
         biggest = x[i];

   /* Everything is zero no sense in doing any more */
   if (biggest == 0.0) return 0;

   for (i=0;i<=n;i++)
      if (x[i] != 0.0)
         if (fabs(biggest / x[i]) > FUDGE_FACTOR1)
            return 1;

   return 0;
}

int solve_quartic(x, results)
DBL *x, *results;
{
   DBL cubic[4], roots[3];
   DBL a0, a1, y, d1, x1, t1, t2;
   DBL c0, c1, c2, c3, c4, d2, q1, q2;
   int i;

   /* Figure out the size difference between coefficients */
   if (difficult_coeffs(4, x)) {
      if (fabs(x[0]) < COEFF_LIMIT)
         if (fabs(x[1]) < COEFF_LIMIT)
            return solve_quadratic(&x[2], results);
         else
            return solve_cubic(&x[1], results);
      else
         return polysolve(4, x, results);
   }

   c0 = x[0];
   if (fabs(c0) < COEFF_LIMIT)
      return solve_cubic(&x[1], results);
   else if (fabs(x[4]) < COEFF_LIMIT) {
      return solve_cubic(x, results);
   }
   else if (c0 != 1.0) {
      c1 = x[1] / c0;
      c2 = x[2] / c0;
      c3 = x[3] / c0;
      c4 = x[4] / c0;
   }
   else {
      c1 = x[1];
      c2 = x[2];
      c3 = x[3];
      c4 = x[4];
   }

      /* The first step is to take the original equation:

	 x^4 + b*x^3 + c*x^2 + d*x + e = 0

      and rewrite it as:

	 x^4 + b*x^3 = -c*x^2 - d*x - e,

      adding (b*x/2)^2 + (x^2 + b*x/2)y + y^2/4 to each side gives a
      perfect square on the lhs:

	 (x^2 + b*x/2 + y/2)^2 = (b^2/4 - c + y)x^2 + (b*y/2 - d)x + y^2/4 - e

      By choosing the appropriate value for y, the rhs can be made a perfect
      square also.  This value is found when the rhs is treated as a quadratic
      in x with the discriminant equal to 0.  This will be true when:

	 (b*y/2 - d)^2 - 4.0 * (b^2/4 - c*y)*(y^2/4 - e) = 0, or

	 y^3 - c*y^2 + (b*d - 4*e)*y - b^2*e + 4*c*e - d^2 = 0.

      This is called the resolvent of the quartic equation.  */

      a0 = 4.0 * c4;
   cubic[0] = 1.0;
   cubic[1] = -1.0 * c2;
   cubic[2] = c1 * c3 - a0;
   cubic[3] = a0 * c2 - c1 * c1 * c4 - c3 * c3;
   i = solve_cubic(&cubic[0], &roots[0]);
   if (i > 0)
      y = roots[0];
   else
      return 0;

   /* What we are left with is a quadratic squared on the lhs and a
      linear term on the right.  The linear term has one of two signs,
      take each and add it to the lhs.  The form of the quartic is now:

	 a' = b^2/4 - c + y,    b' = b*y/2 - d, (from rhs quadritic above)

	 (x^2 + b*x/2 + y/2) = +sqrt(a'*(x + 1/2 * b'/a')^2), and
	 (x^2 + b*x/2 + y/2) = -sqrt(a'*(x + 1/2 * b'/a')^2).

      By taking the linear term from each of the right hand sides and
      adding to the appropriate part of the left hand side, two quadratic
      formulas are created.  By solving each of these the four roots of
      the quartic are determined.
   */
   i = 0;
   a0 = c1 / 2.0;
   a1 = y / 2.0;

   t1 = a0 * a0 - c2 + y;
   if (t1 < 0.0) {
      if (t1 > FUDGE_FACTOR2)
         t1 = 0.0;
      else {
         /* First Special case, a' < 0 means all roots are complex. */
            return 0;
      }
   }
   if (t1 < FUDGE_FACTOR3) {
      /* Second special case, the "x" term on the right hand side above
	 has vanished.  In this case:
		(x^2 + b*x/2 + y/2) = +sqrt(y^2/4 - e), and
		(x^2 + b*x/2 + y/2) = -sqrt(y^2/4 - e).  */
      t2 = a1 * a1 - c4;
      if (t2 < 0.0) {
         return 0;
      }
      x1 = 0.0;
      d1 = sqrt(t2);
   }
   else {
      x1 = sqrt(t1);
      d1 = 0.5 * (a0 * y - c3) / x1;
   }
   /* Solve the first quadratic */
      q1 = -a0 - x1;
   q2 = a1 + d1;
   d2 = q1 * q1 - 4.0 * q2;
   if (d2 >= 0.0) {
      d2 = sqrt(d2);
      results[0] = 0.5 * (q1 + d2);
      results[1] = 0.5 * (q1 - d2);
      i = 2;
   }
   /* Solve the second quadratic */
   q1 = q1 + x1 + x1;
   q2 = a1 - d1;
   d2 = q1 * q1 - 4.0 * q2;
   if (d2 >= 0.0) {
      d2 = sqrt(d2);
      results[i++] = 0.5 * (q1 + d2);
      results[i++] = 0.5 * (q1 - d2);
   }
   return i;
}

/* Root solver based on the Sturm sequences for a polynomial. */
int polysolve(order, Coeffs, roots)
int order;
DBL *Coeffs, *roots;
{
   polynomial sseq[MAX_ORDER+1];
   DBL min_value, max_value;
   int i, nroots, nchanges, np, atmin, atmax;

   /* Put the coefficients into the top of the stack. */
   for (i=0;i<=order;i++)
      sseq[0].coef[order-i] = Coeffs[i];

   /* Build the Sturm sequence */
   np = buildsturm(order, &sseq[0]);

   /* Get the total number of visible roots */
   if ((nroots = visible_roots(np, sseq, &atmin, &atmax)) == 0)
      return 0;

   /* Bracket the roots */
   if (Shadow_Test_Flag)
      min_value = 0.05;
   else
      min_value = 0.0;
   max_value = Max_Distance;

   atmin = numchanges(np, sseq, min_value);
   atmax = numchanges(np, sseq, max_value);
   nroots = atmin - atmax;
   if (nroots == 0) return 0;

   /* perform the bisection. */
   sbisect(np, sseq, min_value, max_value, atmin, atmax, roots);

   return nroots;
}

#ifdef TEST     /* This is not used anywhere or tested.  Interesting? */

#define MAX_POLYGON_SIDES 8
#define Crossing_Point(x1, y1, x2, y2) (x1 - y1 * (x2 - x1) / (y2 - y1))

/* See if "Ray" intersects the polygon defined by the coordinate list "vertices". */
int Intersect_Polygon(Ray, vertex_count, vertices, n, d, Depth, Intersect_Point)
RAY *Ray;
int vertex_count;
VECTOR *vertices, *n, *Intersect_Point;
DBL d, *Depth;
{
   DBL s, t, x, y, z;
   int Sign_Holder, Next_Sign, Crossings;
   int i, this_vertex, next_vertex;

   static DBL temp_x[MAX_POLYGON_SIDES],
   temp_y[MAX_POLYGON_SIDES];

   /* Calculate the point of intersection and the depth. */
   VDot(s, Ray->Direction, *n);
   if (s == 0.0)
      return 0;
   VDot(t, Ray->Initial, *n);
   *Depth = 0.0 - (d + t) / s;
   if (*Depth < Small_Tolerance)
      return 0;
   VScale(*Intersect_Point, Ray->Direction, *Depth);
   VAdd(*Intersect_Point, *Intersect_Point, Ray->Initial);

   /* Map the intersection point and the triangle onto a plane. */
   x = ABS(n->x); y = ABS(n->y); z = ABS(n->z);
   if (x>y)
      if (x>z)
         for (i=0;i<vertex_count;i++) {
            temp_x[i] = vertices[i].y - Intersect_Point->y;
            temp_y[i] = vertices[i].z - Intersect_Point->z;
         }
      else
         for (i=0;i<vertex_count;i++) {
            temp_x[i] = vertices[i].x - Intersect_Point->x;
            temp_y[i] = vertices[i].y - Intersect_Point->y;
         }
   else if (y>z)
      for (i=0;i<vertex_count;i++) {
         temp_x[i] = vertices[i].x - Intersect_Point->x;
         temp_y[i] = vertices[i].z - Intersect_Point->z;
      }
   else
      for (i=0;i<vertex_count;i++) {
         temp_x[i] = vertices[i].x - Intersect_Point->x;
         temp_y[i] = vertices[i].y - Intersect_Point->y;
      }

   /* Determine crossing count */
   Crossings = 0;
   if (temp_y[0] < 0) Sign_Holder = -1;
   else Sign_Holder = 1;

      for (i=0;i<vertex_count;i++) {
         /* Start of winding tests, test the segment from v1 to v2 */
         this_vertex = i;
         next_vertex = (i + 1) % vertex_count;
         if (temp_y[next_vertex] < 0) Next_Sign = -1;
         else Next_Sign = 1;
         if (Sign_Holder != Next_Sign) {
            if (temp_x[this_vertex] > 0.0) {
               if (temp_x[next_vertex] > 0.0)
                  Crossings++;
               else if (Crossing_Point(temp_x[this_vertex], temp_y[this_vertex],
                  temp_x[next_vertex], temp_y[next_vertex]) > 0.0)
                  Crossings++;
            }
            else if (temp_x[next_vertex] > 0.0) {
               if (Crossing_Point(temp_x[this_vertex], temp_y[this_vertex],
                  temp_x[next_vertex], temp_y[next_vertex]) > 0.0)
                  Crossings++;
            }
            Sign_Holder = Next_Sign;
         }
      }

   return (Crossings&1); /* Inside only if # of crossings odd. */
}

/* Translate screen coordinates into world coordinates. */
void World_Coordinate (Viewpoint, Ray, width, height, x, y)
VIEWPOINT *Viewpoint;
RAY *Ray;
int width, height;
DBL x, y;
{
   DBL scalex, scaley;
   VECTOR V1, V2;

   /* Convert the X Coordinate to be a DBL from -0.5 to 0.5 */
   scalex = (x - (DBL)width / 2.0) / (DBL)width;
   /* Convert the Y Coordinate to be a DBL from -0.5 to 0.5 */
   scaley = (((DBL)(height - 1) - y) - (DBL)height / 2.0) / (DBL)height;
   /* Compute the direction of the screen point from the viewpoint */
   VScale (V1, Viewpoint->Up, scaley);
   VScale (V2, Viewpoint->Right, scalex);
   VAdd (Ray->Direction, V1, V2);
   VAdd (Ray->Direction, Ray->Direction, Viewpoint->Direction);
   VNormalize (Ray->Direction, Ray->Direction);
}
/* Uncomment these two declarations if your compiler needs them */
/* They give Microsoft C an out of macro expansion space error */
/* void show_univariate_poly PARAMS((char *label, int order,DBL *Coeffs));  */
/* void show_points PARAMS((char *label,int count,DBL *point_list);  */

void show_univariate_poly(label, order, Coeffs)
char *label;
int order;
DBL *Coeffs;
{
   int i;
   printf("%s", label);
   for (i=0;i<=order;i++) {
      printf("%.2lf x^%d", Coeffs[i], order-i);
      if (i==order) printf("\n");
      else printf(" + ");
   }
}

   void show_points(label, count, point_list)
      char *label;
int count;
DBL *point_list;
{
   int i;
   printf("%s", label);
   for (i=0;i<count;i++) {
      printf("%lf", point_list[i]);
      if (i==(count-1)) printf("\n");
      else printf(", ");
   }
}

#endif
