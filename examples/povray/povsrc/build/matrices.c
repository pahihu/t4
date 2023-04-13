/****************************************************************************
*                matrices.c
*
*  This module contains code to manipulate 4x4 matrices.
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

void MZero (result)
MATRIX *result;
{
   /* Initialize the matrix to the following values:
   0.0   0.0   0.0   0.0
   0.0   0.0   0.0   0.0
   0.0   0.0   0.0   0.0
   0.0   0.0   0.0   0.0
*/
   register int i, j;

   for (i = 0 ; i < 4 ; i++)
      for (j = 0 ; j < 4 ; j++)
         (*result)[i][j] = 0.0;
}

void MIdentity (result)
MATRIX *result;
{
   /* Initialize the matrix to the following values:
   1.0   0.0   0.0   0.0
   0.0   1.0   0.0   0.0
   0.0   0.0   1.0   0.0
   0.0   0.0   0.0   1.0
*/
   register int i, j;

   for (i = 0 ; i < 4 ; i++)
      for (j = 0 ; j < 4 ; j++)
         if (i == j)
            (*result)[i][j] = 1.0;
         else
            (*result)[i][j] = 0.0;
}

void MTimes (result, matrix1, matrix2)
MATRIX *result, *matrix1, *matrix2;
{
   register int i, j, k;
   MATRIX temp_matrix;

   for (i = 0 ; i < 4 ; i++)
      for (j = 0 ; j < 4 ; j++) {
         temp_matrix[i][j] = 0.0;
         for (k = 0 ; k < 4 ; k++)
            temp_matrix[i][j] += (*matrix1)[i][k] * (*matrix2)[k][j];
      }

   for (i = 0 ; i < 4 ; i++)
      for (j = 0 ; j < 4 ; j++)
         (*result)[i][j] = temp_matrix[i][j];

   COOPERATE    
}

/*  AAC - These are not used, so they are commented out to save code space...

void MAdd (result, matrix1, matrix2)
   MATRIX *result, *matrix1, *matrix2;
   {
   register int i, j;

   for (i = 0 ; i < 4 ; i++)
      for (j = 0 ; j < 4 ; j++)
         (*result)[i][j] = (*matrix1)[i][j] + (*matrix2)[i][j];
   }

void MSub (result, matrix1, matrix2)
   MATRIX *result, *matrix1, *matrix2;
   {
   register int i, j;

   for (i = 0 ; i < 4 ; i++)
      for (j = 0 ; j < 4 ; j++)
         (*result)[i][j] = (*matrix1)[i][j] - (*matrix2)[i][j];
   }

void MScale (result, matrix1, amount)
MATRIX *result, *matrix1;
DBL amount;
{
   register int i, j;

   for (i = 0 ; i < 4 ; i++)
      for (j = 0 ; j < 4 ; j++)
	 if (amount == 1.0)
	    (*result)[i][j] = (*matrix1)[i][j]; * just copy *
	 else
            (*result)[i][j] = (*matrix1)[i][j] * amount;
   return;
}
... up to here! */

void MTranspose (result, matrix1)
MATRIX *result, *matrix1;
{
   register int i, j;
   MATRIX temp_matrix;

   for (i = 0 ; i < 4 ; i++)
      for (j = 0 ; j < 4 ; j++)
         temp_matrix[i][j] = (*matrix1)[j][i];

   for (i = 0 ; i < 4 ; i++)
      for (j = 0 ; j < 4 ; j++)
         (*result)[i][j] = temp_matrix[i][j];
}


void MTransformVector (result, vector, transformation)
VECTOR *result, *vector;
TRANSFORMATION *transformation;
{
   register int i;
   DBL answer_array[4];
   MATRIX *matrix;

   matrix = (MATRIX *) transformation -> matrix;

   for (i = 0 ; i < 4 ; i++)
      answer_array[i] = vector -> x * (*matrix)[0][i]
      + vector -> y * (*matrix)[1][i]
      + vector -> z * (*matrix)[2][i]
      + (*matrix)[3][i];

   result -> x  = answer_array[0];
   result -> y  = answer_array[1];
   result -> z  = answer_array[2];
}

void MInverseTransformVector (result, vector, transformation)
VECTOR *result, *vector;
TRANSFORMATION *transformation;
{
   register int i;
   DBL answer_array[4];
   MATRIX *matrix;

   matrix = (MATRIX *) transformation -> inverse;

   for (i = 0 ; i < 4 ; i++)
      answer_array[i] = vector -> x * (*matrix)[0][i]
      + vector -> y * (*matrix)[1][i]
      + vector -> z * (*matrix)[2][i]
      + (*matrix)[3][i];

   result -> x  = answer_array[0];
   result -> y  = answer_array[1];
   result -> z  = answer_array[2];
}

void MTransVector (result, vector, transformation)
VECTOR *result, *vector;
TRANSFORMATION *transformation;
{
   register int i;
   DBL answer_array[4];
   MATRIX *matrix;

   matrix = (MATRIX *) transformation -> matrix;

   for (i = 0 ; i < 4 ; i++)
      answer_array[i] = vector -> x * (*matrix)[0][i]
      + vector -> y * (*matrix)[1][i]
      + vector -> z * (*matrix)[2][i];

   result -> x  = answer_array[0];
   result -> y  = answer_array[1];
   result -> z  = answer_array[2];
}

void MInvTransVector (result, vector, transformation)
VECTOR *result, *vector;
TRANSFORMATION *transformation;
{
   register int i;
   DBL answer_array[4];
   MATRIX *matrix;

   matrix = (MATRIX *) transformation -> inverse;

   for (i = 0 ; i < 4 ; i++)
      answer_array[i] = vector -> x * (*matrix)[0][i]
      + vector -> y * (*matrix)[1][i]
      + vector -> z * (*matrix)[2][i];

   result -> x  = answer_array[0];
   result -> y  = answer_array[1];
   result -> z  = answer_array[2];
}

void MTransNormal (result, vector, transformation)
VECTOR *result, *vector;
TRANSFORMATION *transformation;
{
   register int i;
   DBL answer_array[3];
   MATRIX *matrix;

   matrix = (MATRIX *) transformation -> inverse;

   for (i = 0 ; i < 3 ; i++)
      answer_array[i] = vector -> x * (*matrix)[i][0]
      + vector -> y * (*matrix)[i][1]
      + vector -> z * (*matrix)[i][2];

   result -> x  = answer_array[0];
   result -> y  = answer_array[1];
   result -> z  = answer_array[2];
}

void Get_Scaling_Transformation (result, vector)
TRANSFORMATION *result;
VECTOR *vector;
{
   MIdentity ((MATRIX *)result -> matrix);
   (result -> matrix)[0][0] = vector -> x;
   (result -> matrix)[1][1] = vector -> y;
   (result -> matrix)[2][2] = vector -> z;

   MIdentity ((MATRIX *)result -> inverse);
   (result -> inverse)[0][0] = 1.0 / vector -> x;
   (result -> inverse)[1][1]= 1.0 / vector -> y;
   (result -> inverse)[2][2] = 1.0 / vector -> z;
}

/* AAC - This is not used, so it's commented out...

void Get_Inversion_Transformation (result)
   TRANSFORMATION *result;
   {
   MIdentity ((MATRIX *)result -> matrix);
   (result -> matrix)[0][0] = -1.0;
   (result -> matrix)[1][1] = -1.0;
   (result -> matrix)[2][2] = -1.0;
   (result -> matrix)[3][3] = -1.0;


   (result -> inverse)[0][0] = -1.0;
   (result -> inverse)[1][1] = -1.0;
   (result -> inverse)[2][2] = -1.0;
   (result -> inverse)[3][3] = -1.0;
   }
... up to here! */

void Get_Translation_Transformation (transformation, vector)
TRANSFORMATION *transformation;
VECTOR *vector;
{
   MIdentity ((MATRIX *)transformation -> matrix);
   (transformation -> matrix)[3][0] = vector -> x;
   (transformation -> matrix)[3][1] = vector -> y;
   (transformation -> matrix)[3][2] = vector -> z;

   MIdentity ((MATRIX *)transformation -> inverse);
   (transformation -> inverse)[3][0] = 0.0 - vector -> x;
   (transformation -> inverse)[3][1] = 0.0 - vector -> y;
   (transformation -> inverse)[3][2] = 0.0 - vector -> z;
}

void Get_Rotation_Transformation (transformation, vector)
TRANSFORMATION *transformation;
VECTOR *vector;
{
   MATRIX Matrix;
   VECTOR Radian_Vector;
   register DBL cosx, cosy, cosz, sinx, siny, sinz;

   VScale (Radian_Vector, *vector, M_PI/180.0);
   MIdentity ((MATRIX *)transformation -> matrix);
   cosx = cos (Radian_Vector.x);
   sinx = sin (Radian_Vector.x);
   cosy = cos (Radian_Vector.y);
   siny = sin (Radian_Vector.y);
   cosz = cos (Radian_Vector.z);
   sinz = sin (Radian_Vector.z);

   (transformation -> matrix) [1][1] = cosx;
   (transformation -> matrix) [2][2] = cosx;
   (transformation -> matrix) [1][2] = sinx;
   (transformation -> matrix) [2][1] = 0.0 - sinx;
   MTranspose ((MATRIX *)transformation -> inverse, (MATRIX *)transformation -> matrix);

   MIdentity ((MATRIX *)Matrix);
   Matrix [0][0] = cosy;
   Matrix [2][2] = cosy;
   Matrix [0][2] = 0.0 - siny;
   Matrix [2][0] = siny;
   MTimes ((MATRIX *)transformation -> matrix, (MATRIX *)transformation -> matrix, (MATRIX *)Matrix);
   MTranspose ((MATRIX *)Matrix, (MATRIX *)Matrix);
   MTimes ((MATRIX *)transformation -> inverse, (MATRIX *)Matrix, (MATRIX *)transformation -> inverse);

   MIdentity ((MATRIX *)Matrix);
   Matrix [0][0] = cosz;
   Matrix [1][1] = cosz;
   Matrix [0][1] = sinz;
   Matrix [1][0] = 0.0 - sinz;
   MTimes ((MATRIX *)transformation -> matrix, (MATRIX *)transformation -> matrix, (MATRIX *)Matrix);
   MTranspose ((MATRIX *)Matrix, (MATRIX *)Matrix);
   MTimes ((MATRIX *)transformation -> inverse, (MATRIX *)Matrix, (MATRIX *)transformation -> inverse);
}

/* AAC - This is not used so it's commented out...

void Get_Look_At_Transformation (result, Look_At, Up, Right)
   TRANSFORMATION *result;
   VECTOR *Look_At, *Up, *Right;
   {
   MIdentity ((MATRIX *)result -> inverse);
   (result -> matrix)[0][0] = Right->x;
   (result -> matrix)[0][1] = Right->y;
   (result -> matrix)[0][2] = Right->z;
   (result -> matrix)[1][0] = Up->x;
   (result -> matrix)[1][1] = Up->y;
   (result -> matrix)[1][2] = Up->z;
   (result -> matrix)[2][0] = Look_At->x;
   (result -> matrix)[2][1] = Look_At->y;
   (result -> matrix)[2][2] = Look_At->z;

   MIdentity ((MATRIX *)result -> matrix);
   MTranspose ((MATRIX *)result -> matrix, (MATRIX *)result -> inverse);   
   }

... up to here! */

void Compose_Transformations (Original_Transformation, New_Transformation)
TRANSFORMATION *Original_Transformation, *New_Transformation;
{
   MTimes ((MATRIX *)Original_Transformation -> matrix,
      (MATRIX *)Original_Transformation -> matrix,
      (MATRIX *)New_Transformation -> matrix);

   MTimes ((MATRIX *)Original_Transformation -> inverse,
      (MATRIX *)New_Transformation -> inverse,
      (MATRIX *)Original_Transformation -> inverse);
}
