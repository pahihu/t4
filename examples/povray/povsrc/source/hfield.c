/****************************************************************************
*                   hfield.c
*
*	This file implements the height field shape primitive.  The shape is
*	implemented as a collection of triangles which are calculated as
*	needed.  The basic intersection routine first computes the rays
*	intersection with the box marking the limits of the shape, then
*	follows the line from one intersection point to the other, testing the
*	two triangles which form the pixel for an intersection with the ray at
*	each step.
*		height field added by Doug Muir
*		with lots of advice and support from David Buck 
*			and Drew Wells.
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

#define sign(x) (((x) > 0.0) ? 1: (((x) == 0) ? 0: (-1)))

#ifndef min_value
#define min_value(x,y) ((x) > (y) ? (y) : (x))
#endif
#ifndef max_value
#define max_value(x,y) ((x) < (y) ? (y) : (x))
#endif

METHODS Height_Field_Methods = {
   Object_Intersect, All_HeightFld_Intersections,
   Inside_HeightFld, HeightFld_Normal,
   Copy_HeightFld,
   Translate_HeightFld, Rotate_HeightFld,
   Scale_HeightFld, Invert_HeightFld};

extern HEIGHT_FIELD *Get_Height_Field_Shape();

extern long Ray_Ht_Field_Tests, Ray_Ht_Field_Tests_Succeeded;
extern long Ray_Ht_Field_Box_Tests, Ray_HField_Box_Tests_Succeeded;
extern int Options;

int isdx, isdz, X_Dom;
DBL Gdx, Gdy, Gdz;
DBL Myx, Mxz, Mzx, Myz;
INTERSECTION *Hf_Intersection;
PRIOQ *Hf_Queue;
RAY *RRay;

#define Get_Height(x, z, H_Field) ((DBL)(H_Field)->Map[(z)][(x)])

int Intersect_Pixel(x, z, Ray, H_Field, height1, height2)
int x;
int z;
RAY *Ray;
HEIGHT_FIELD *H_Field;
DBL height1;
DBL height2;
{
   VECTOR T1V1,T1V2,T1V3,T2V1,T2V2,T2V3,Local_Normal;
   DBL pos1,pos2,dot,depth1,depth2,s,t,y1,y2,y3,y4;
   DBL max_height, min_height;

   depth1 = HUGE_VAL;
   depth2 = HUGE_VAL;

   y1 = Get_Height(x,z,H_Field);
   y2 = Get_Height(x+1,z,H_Field);
   y3 = Get_Height(x,z+1,H_Field);
   y4 = Get_Height(x+1,z+1,H_Field);

   Make_Vector(&T1V1,(DBL)x,y1,(DBL)z);
   Make_Vector(&T1V2,1.0,y2-y1,0.0);
   Make_Vector(&T1V3,0.0,y3-y1,1.0);
   Make_Vector(&T2V1,(DBL)(x+1),y4,(DBL)(z+1));
   Make_Vector(&T2V2,-1.0,y3-y4,0.0);
   Make_Vector(&T2V3,0.0,y2-y4,-1.0);

   /*
	 * first, we check to see if it is even possible for the ray to
	 * intersect the triangle.
	 */

   if((max_value(y1,max_value(y2,y3)) >= height1) && 
      (min_value(y1,min_value(y2,y3)) <= height2))
   {
      VCross(Local_Normal,T1V3,T1V2);
      VDot(dot,Local_Normal,Ray->Direction);

      if((dot > EPSILON) || (dot < -EPSILON))
      {
         VDot(pos1,Local_Normal,T1V1);

         VDot(pos2,Local_Normal,Ray->Initial);

         pos1 -= pos2;

         depth1 = pos1 / dot;

         if((depth1 > Small_Tolerance) && (depth1 < Max_Distance))
         {
            s = Ray->Initial.x+(depth1*Ray->Direction.x)-(DBL)x;
            t = Ray->Initial.z+(depth1*Ray->Direction.z)-(DBL)z;

            if((s<-EPSILON) || (t<-EPSILON) || ((s+t)>1.0+EPSILON))
               depth1 = HUGE_VAL;
         }
         else
            depth1 = HUGE_VAL;
      }
   }

   /*
	 * first, we check to see if it is even possible for the ray to
	 * intersect the triangle.
	   Rewritten to get around Code Builder FP stack problem.
	   Original code:
              if((max_value(y4,max_value(y2,y3)) >= height1) && 
	      (min_value(y4,min_value(y2,y3)) <= height2))            */

   max_height = max_value(y4,max_value(y2,y3));
   min_height = min_value(y4,min_value(y2,y3));
   if((max_height >= height1) && (min_height <= height2))
   {
      VCross(Local_Normal,T2V3,T2V2);
      VDot(dot,Local_Normal,Ray->Direction);

      if((dot > EPSILON) || (dot < -EPSILON))
      {
         VDot(pos1,Local_Normal,T2V1);

         VDot(pos2,Local_Normal,Ray->Initial);
         pos1 -= pos2;

         depth2 = pos1 / dot;

         if((depth2 > Small_Tolerance) && (depth2 < Max_Distance))
         {
            s = Ray->Initial.x+(depth2*Ray->Direction.x)-(DBL)x;
            t = Ray->Initial.z+(depth2*Ray->Direction.z)-(DBL)z;

            if((s>1.0+EPSILON) || (t>1.0+EPSILON) || ((s+t)<1.0-EPSILON))
               depth2 = HUGE_VAL;
         }
         else
            depth2 = HUGE_VAL;
      }
   }

   if((depth1 >= Max_Distance) && (depth2 >= Max_Distance))
      return(FALSE);

   if(depth2 < depth1) {
      Hf_Intersection->Depth = depth2;
      Hf_Intersection->Object = H_Field -> Parent_Object;
      VScale (T1V1, RRay -> Direction, depth2);
      VAdd (T1V1, T1V1, RRay -> Initial);
      Hf_Intersection->Point = T1V1;
      Hf_Intersection->Shape = (SHAPE *)H_Field;
      pq_add (Hf_Queue, Hf_Intersection);
   }
   else {
      Hf_Intersection->Depth = depth1;
      Hf_Intersection->Object = H_Field -> Parent_Object;
      VScale (T1V1, RRay -> Direction, depth1);
      VAdd (T1V1, T1V1, RRay -> Initial);
      Hf_Intersection->Point = T1V1;
      Hf_Intersection->Shape = (SHAPE *)H_Field;
      pq_add (Hf_Queue, Hf_Intersection);
   }
   Ray_Ht_Field_Tests_Succeeded++;
   return(TRUE);
}

   int Intersect_Sub_Block(Block, Ray, H_Field, start, end)
      HF_BLOCK *Block;
RAY *Ray;
HEIGHT_FIELD *H_Field;
VECTOR *start, *end;
{
   DBL y1, y2;
   DBL sx, sy, sz, ex, ey, ez, f;
   int ix, iz, length, i, x_size, z_size;

   if(min_value(start->y,end->y) > Block->max_y)
      return(FALSE);

   if(max_value(start->y,end->y) < Block->min_y)
      return(FALSE);

   sx = start->x;
   sy = start->y;
   sz = start->z;
   ex = end->x;
   ey = end->y;
   ez = end->z;

   if(X_Dom)
   {
      if(isdx >= 0) {
         f = floor(sx) - sx;
         sx = floor(sx);
         sy += Myx * f;
         sz += Mzx * f;
         ex = floor(ex);
         ix = (int)sx;
      }
      else {
         f = ceil(sx) - sx;
         sx = ceil(sx);
         sy += Myx * f;
         sz += Mzx * f;
         ex = ceil(ex);
         ix = (int)sx - 1;
      }

         length = 0.5 + fabs(ex - sx);

      if(isdz >= 0) {
         f = sz - ceil(sz);
         iz = (int)floor(sz);	
      }
      else {
         f = floor(sz) - sz;
         iz = (int)ceil(sz) - 1;
      }

         if(Gdy >= 0.0) {
            y1 = sy;
            y2 = sy + Gdy;
         }
         else {
            y1 = sy + Gdy;
            y2 = sy;
         }

         for(i=0;i<=length;i++) {
            if(Intersect_Pixel(ix,iz,Ray,H_Field,y1,y2))
               return(TRUE);
            f += Gdz;
            if(f>=0.0) {
               iz += isdz;
               if(Intersect_Pixel(ix,iz,Ray,H_Field,y1,y2))
                  return(TRUE);
               f -= 1.0;
            }
            ix += isdx;
            y1 += Gdy;
            y2 += Gdy;
         }
   }
   else {

         if(isdz >= 0) {
            f = floor(sz) - sz;
            sz = floor(sz);
            sy += Myz * f;
            sx += Mxz * f;
            ez = floor(ez);
            iz = (int)sz;
         }
         else {
            f = ceil(sz) - sz;
            sz = ceil(sz);
            sy += Myz * f;
            sx += Mxz * f;
            ez = ceil(ez);
            iz = (int)sz - 1;
         }            	

         length = 0.5 + fabs(ez - sz);

      if(isdx >= 0) {
         f = sx - ceil(sx);
         ix = (int)floor(sx);
      }
      else {
         f = floor(sx) - sx;
         ix = (int)ceil(sx) - 1;
      }

         if(Gdy >= 0.0) {
            y1 = sy;
            y2 = sy + Gdy;
         }
         else {
            y1 = sy + Gdy;
            y2 = sy;
         }

         for(i=0;i<=length;i++) {
            if(Intersect_Pixel(ix,iz,Ray,H_Field,y1,y2))
               return(TRUE);
            f += Gdx;
            if(f>=0.0) {
               ix += isdx;
               if(Intersect_Pixel(ix,iz,Ray,H_Field,y1,y2))
                  return(TRUE);
               f -= 1.0;
            }
            iz += isdz;
            y1 += Gdy;
            y2 += Gdy;
         }
   }
   return (FALSE);
}

int Intersect_Hf_Node(Ray, H_Field, start, end)
RAY *Ray;
HEIGHT_FIELD *H_Field;
VECTOR *start, *end;
{
   VECTOR *curr, *next, *temp, temp1, temp2;
   DBL sx, sy, sz, ex, ey, ez, x, y, z;
   DBL tnear, tfar, t, Block_Size, Inv_Blk_Size;
   int ix, iz, x_size, z_size, length, i;

   x = sx = start->x;
   y = sy = start->y;
   z = sz = start->z;
   ex = end->x;
   ey = end->y;
   ez = end->z;

   Block_Size = H_Field->Block_Size;
   Inv_Blk_Size = H_Field->Inv_Blk_Size;

   x_size = abs((int)(ex*Inv_Blk_Size) - (int)(sx*Inv_Blk_Size));
   z_size = abs((int)(ez*Inv_Blk_Size) - (int)(sz*Inv_Blk_Size));
   length = x_size + z_size;

   curr = &temp1;
   next = &temp2;
   Make_Vector(curr, x, y, z);
   t = 0.0;

   if(X_Dom)
   {
      if(isdx >= 0) {
         ix = (int)floor(sx*Inv_Blk_Size);
         tnear = Block_Size*(ix+1) - sx;

         if (isdz >= 0) {
            iz = (int)floor(sz*Inv_Blk_Size);
            tfar = Gdx * (Block_Size*(iz+1) - sz);
         }
         else {
            iz = (int)ceil(sz*Inv_Blk_Size) - 1;
            tfar = Gdx * (sz - Block_Size*(iz));
         }         		
         for (i = 0; i < length; i++) {
            if(tnear < tfar) {
               t = tnear;
               x = sx + t;
               y = sy + Myx * t;
               z = sz + Mzx * t;
               Make_Vector(next, x, y, z);
               if(Intersect_Sub_Block(&(H_Field->Block[ix][iz]),Ray,H_Field,curr,next))
                  return(TRUE);
               temp = curr;
               curr = next;
               next = temp;
               ix++;
               tnear = Block_Size*(ix+1) - sx;
            }
            else {
               t = tfar;
               x = sx + t;
               y = sy + Myx * t;
               z = sz + Mzx * t;
               Make_Vector(next, x, y, z);
               if(Intersect_Sub_Block(&(H_Field->Block[ix][iz]),Ray,H_Field,curr,next))
                  return(TRUE);
               temp = curr;
               curr = next;
               next = temp;
               iz += isdz;
               if (isdz >= 0) 
                  tfar = Gdx * (Block_Size*(iz+1) - sz);
               else 
                  tfar = Gdx * (sz - Block_Size*(iz));
            }
         }
      }
      else {				
         ix = (int)ceil(sx*Inv_Blk_Size) - 1;
         tnear = sx - Block_Size*(ix);

            if (isdz >= 0) {
               iz = (int)floor(sz*Inv_Blk_Size);
               tfar = Gdx * (Block_Size*(iz+1) - sz);
            }
            else {
               iz = (int)ceil(sz*Inv_Blk_Size) - 1;
               tfar = Gdx * (sz - Block_Size*(iz));
            }

            for (i = 0; i < length; i++) {
               if(tnear < tfar) {
                  t = tnear;
                  x = sx - t;
                  y = sy - Myx * t;
                  z = sz - Mzx * t;
                  Make_Vector(next, x, y, z);
                  if(Intersect_Sub_Block(&(H_Field->Block[ix][iz]),Ray,H_Field,curr,next))
                     return(TRUE);
                  temp = curr;
                  curr = next;
                  next = temp;
                  ix--;
                  tnear = sx - Block_Size*(ix);
               }
               else {
                  t = tfar;
                  x = sx - t;
                  y = sy - Myx * t;
                  z = sz - Mzx * t;
                  Make_Vector(next, x, y, z);
                  if(Intersect_Sub_Block(&(H_Field->Block[ix][iz]),Ray,H_Field,curr,next))
                     return(TRUE);
                  temp = curr;
                  curr = next;
                  next = temp;
                  iz += isdz;
                  if (isdz >= 0) 
                     tfar = Gdx * (Block_Size*(iz+1) - sz);
                  else 
                     tfar = Gdx * (sz - Block_Size*(iz));
               }
            }
      }      			
   }
   else {
      if(isdz >= 0) {
         iz = (int)floor(sz*Inv_Blk_Size);
         tnear = Block_Size*(iz+1) - sz;

         if (isdx >= 0) {
            ix = (int)floor(sx*Inv_Blk_Size);	
            tfar = Gdz * (Block_Size*(ix+1) - sx);
         }
         else {
            ix = (int)ceil(sx*Inv_Blk_Size) - 1;
            tfar = Gdz * (sx - Block_Size*(ix));
         }
         for (i = 0; i < length; i++) {
            if(tnear < tfar) {
               t = tnear;
               z = sz + t;
               y = sy + Myz * t;
               x = sx + Mxz * t;
               Make_Vector(next, x, y, z);
               if(Intersect_Sub_Block(&(H_Field->Block[ix][iz]),Ray,H_Field,curr,next))
                  return(TRUE);
               temp = curr;
               curr = next;
               next = temp;
               iz++;
               tnear = Block_Size*(iz+1) - sz;
            }
            else {
               t = tfar;
               z = sz + t;
               y = sy + Myz * t;
               x = sx + Mxz * t;
               Make_Vector(next, x, y, z);
               if(Intersect_Sub_Block(&(H_Field->Block[ix][iz]),Ray,H_Field,curr,next))
                  return(TRUE);
               temp = curr;
               curr = next;
               next = temp;
               ix += isdx;
               if (isdx >= 0) 
                  tfar = Gdz * (Block_Size*(ix+1) - sx);
               else 
                  tfar = Gdz * (sx - Block_Size*(ix));
            }
         }
      }
      else {				
         iz = (int)ceil(sz*Inv_Blk_Size) - 1;
         tnear = sz - Block_Size*(iz);

            if (isdx >= 0) {
               ix = (int)floor(sx*Inv_Blk_Size);
               tfar = Gdz * (Block_Size*(ix+1) - sx);
            }
            else {
               ix = (int)ceil(sx*Inv_Blk_Size) - 1;
               tfar = Gdz * (sx - Block_Size*(ix));
            }            			
         for (i = 0; i < length; i++) {
            if(tnear < tfar) {
               t = tnear;
               z = sz - t;
               y = sy - Myz * t;
               x = sx - Mxz * t;
               Make_Vector(next, x, y, z);
               if(Intersect_Sub_Block(&(H_Field->Block[ix][iz]),Ray,H_Field,curr,next))
                  return(TRUE);
               temp = curr;
               curr = next;
               next = temp;
               iz--;
               tnear = sz - Block_Size*iz;
            }
            else {
               t = tfar;
               z = sz - t;
               y = sy - Myz * t;
               x = sx - Mxz * t;
               Make_Vector(next, x, y, z);
               if(Intersect_Sub_Block(&(H_Field->Block[ix][iz]),Ray,H_Field,curr,next))
                  return(TRUE);
               temp = curr;
               curr = next;
               next = temp;
               ix += isdx;
               if (isdx >= 0) 
                  tfar = Gdz * (Block_Size*(ix+1) - sx);
               else 
                  tfar = Gdz * (sx - Block_Size*(ix));
            }
         }
      }      			   	
   }
   Make_Vector(next,ex,ey,ez);
   if(isdx >= 0)
      ix = (int)floor(ex*Inv_Blk_Size);
   else
      ix = (int)ceil(ex*Inv_Blk_Size) - 1;
   if(isdz >= 0)
      iz = (int)floor(ez*Inv_Blk_Size);
   else
      iz = (int)ceil(ez*Inv_Blk_Size) - 1;
   if(Intersect_Sub_Block(&(H_Field->Block[ix][iz]),Ray,H_Field,curr,next))
      return(TRUE);
   return (FALSE);
}

void Find_Hf_Min_Max(H_Field, Image, Image_Type)
HEIGHT_FIELD *H_Field;
IMAGE *Image;
int Image_Type;
{
   int n, i, i2, j, j2, x, z, w, h, max_x, max_z, temp1, temp2;
   DBL size;
   DBL temp_y, Block_Size, Inv_Blk_Size;

   max_x = Image->iwidth;
   if(Image_Type == POT) max_x = max_x/2;
   max_z = Image->iheight;

   size = (DBL)max_value(max_x, max_z);
   H_Field->Block_Size  = Block_Size = ceil(sqrt(size+1.0));
   H_Field->Inv_Blk_Size = Inv_Blk_Size = 1.0/Block_Size;
   n = (int)Block_Size;

   w = (int)ceil((Image->width+1.0)*Inv_Blk_Size);
   h = (int)ceil((Image->height+1.0)*Inv_Blk_Size);

   H_Field->Map = (float **)calloc(max_z+1, sizeof(float *));
   if (H_Field->Map == NULL)
      fprintf(stderr,"Cannot allocate memory for height field\n");

   H_Field->Block = (HF_BLOCK **)calloc(w,sizeof(HF_BLOCK *));
   if(H_Field->Block == NULL)
      fprintf(stderr, "Cannot allocate memory for height field buffer\n");
   for(i=0; i<w; i++) {
      H_Field->Block[i] = (HF_BLOCK *)calloc(h,sizeof(HF_BLOCK));
      if (H_Field->Block[i] == NULL)
         fprintf(stderr, "Cannot allocate memory for height field buffer line\n");
      for(j=0; j<h; j++) {
         H_Field->Block[i][j].min_y = 65536.0;
         H_Field->Block[i][j].max_y = 0.0;
      }
   }

   H_Field->Map[0] = (float *)calloc(max_x+1,sizeof(float));
   if (H_Field->Map[0] == NULL)
      fprintf(stderr,"Cannot allocate memory for height field\n");

   for(j=0; j < h; j++){
      for(j2=0;(j2 <= n) && (j*n+j2 <= max_z);j2++){
         z = j*n+j2;
         if(j2!=0){
            H_Field->Map[z] = (float *)calloc(max_x+1,sizeof(float));
            if (H_Field->Map[z] == NULL)
               fprintf(stderr, "Cannot allocate memory for height field\n");
         }
         for(i=0; i < w; i++){
            for(i2=0;(i2 <= n)&&(i*n+i2 <= max_x);i2++){
               x = i*n+i2;
               if((x > 1) && (x < max_x - 1) && (z > 1) && (z < max_z - 1)) {
                  switch(Image_Type) {
                  case GIF:
                     temp1 = Image->data.map_lines[max_z - z - 1][x];
                     temp_y = (DBL)(temp1);
                     break;
                  case POT:
                     temp1 = Image->data.map_lines[max_z - z - 1][x];
                     temp2 = Image->data.map_lines[max_z - z - 1][x + max_x];
                     temp_y = (DBL)((DBL)temp1 + (DBL)temp2/256.0);
                     break;
                  case TGA: 
                     temp1 = Image->data.rgb_lines[max_z - z - 1].red[x];
                     temp2 = Image->data.rgb_lines[max_z - z - 1].green[x];
                     temp_y = (DBL)((DBL)temp1 + (DBL)temp2/256.0);
                     break;
                  }
                  if (temp_y <= H_Field->bounding_box->bounds[0].y)
                     H_Field->Map[z][x] = -10000.0;
                  else
                     H_Field->Map[z][x] = (float)temp_y;
               }
               else {
                  temp_y = -10000.0;
                  H_Field->Map[z][x] = (float)temp_y;
               }

                  if(temp_y < H_Field->bounding_box->bounds[0].y)
                     temp_y = H_Field->bounding_box->bounds[0].y;
               if(temp_y < H_Field->Block[i][j].min_y)
                  H_Field->Block[i][j].min_y = temp_y;
               if(temp_y > H_Field->Block[i][j].max_y)
                  H_Field->Block[i][j].max_y = temp_y;
            }
         }
         if((z >= 0) && (z < max_z) && (j2!=n)) {
            switch (Image_Type) {
            case GIF: 
               free(Image->data.map_lines[max_z - z - 1]); break;
            case POT: 
               free(Image->data.map_lines[max_z - z - 1]); break;
            case TGA:
               free(Image->data.rgb_lines[max_z - z - 1].blue);
               free(Image->data.rgb_lines[max_z - z - 1].green);
               free(Image->data.rgb_lines[max_z - z - 1].red);
               break;
            }
         }
      }
   }
}

int All_HeightFld_Intersections(Object, Ray, Depth_Queue)
OBJECT *Object;
RAY *Ray;	
PRIOQ *Depth_Queue;
{
   VECTOR Temp1, Temp2, Test;
   RAY Temp_Ray;
   DBL depth1, depth2;
   int ret_val = FALSE;
   HEIGHT_FIELD *H_Field = (HEIGHT_FIELD *) Object;
   INTERSECTION Local_Element;

   Ray_Ht_Field_Tests++;

   MInverseTransformVector(&(Temp_Ray.Initial),&(Ray->Initial),H_Field->Transformation);
   MInvTransVector(&(Temp_Ray.Direction),&(Ray->Direction),H_Field->Transformation);

   if(!Intersect_Boxx(&Temp_Ray,H_Field->bounding_box,&depth1,&depth2))
      return(FALSE);     

   /*	if(	fabs(depth1 - depth2) < Small_Tolerance) { Try EPSILON if next line doesn't work */

   if( depth1 == depth2) {
      depth1 = 0.0;
      VScale(Temp1,Temp_Ray.Direction,depth1);
      VAdd(Temp1,Temp1,Temp_Ray.Initial);
      VScale(Temp2,Temp_Ray.Direction,depth2);
      VAdd(Temp2,Temp2,Temp_Ray.Initial);
   }
   else {
      VScale(Temp1,Temp_Ray.Direction,depth1);
      VAdd(Temp1,Temp1,Temp_Ray.Initial);
      VScale(Temp2,Temp_Ray.Direction,depth2);
      VAdd(Temp2,Temp2,Temp_Ray.Initial);		
   }

      if(fabs(Temp_Ray.Direction.x) > EPSILON) {
         Mzx = Temp_Ray.Direction.z/Temp_Ray.Direction.x;
         Myx = Temp_Ray.Direction.y/Temp_Ray.Direction.x;
      }
      else {
         Mzx = Temp_Ray.Direction.z/EPSILON;
         Myx = Temp_Ray.Direction.y/EPSILON;
      }
   if(fabs(Temp_Ray.Direction.z) > EPSILON) {
      Mxz = Temp_Ray.Direction.x/Temp_Ray.Direction.z;
      Myz = Temp_Ray.Direction.y/Temp_Ray.Direction.z;
   }
   else {
      Mxz = Temp_Ray.Direction.x/EPSILON;
      Myz = Temp_Ray.Direction.y/EPSILON;
   }

      Hf_Queue = Depth_Queue;
   Hf_Intersection = &Local_Element;
   RRay = Ray;

   isdx = sign(Temp_Ray.Direction.x);
   isdz = sign(Temp_Ray.Direction.z);

   X_Dom = FALSE;
   if(fabs(Temp_Ray.Direction.x) >= fabs(Temp_Ray.Direction.z))
      X_Dom = TRUE;

   Gdx = fabs(Mxz);
   Gdz = fabs(Mzx);
   if(X_Dom) {
      Gdy = Myx * (DBL)isdx;
   }
   else {
      Gdy = Myz * (DBL)isdz;
   }

      if(Intersect_Hf_Node(&Temp_Ray, H_Field, &Temp1, &Temp2))
         ret_val = TRUE;
   return(ret_val);
}

int Inside_HeightFld (Test_Point, Object)
VECTOR *Test_Point;
OBJECT *Object;
{
   HEIGHT_FIELD *H_Field = (HEIGHT_FIELD *) Object;
   int px, pz, dot1, dot2;
   DBL x,z,y1,y2,y3;
   VECTOR Local_Origin, Temp1, Temp2, Local_Normal, Test;

   MInverseTransformVector(&Test, Test_Point, H_Field->Transformation);

   px = (int)Test.x;
   pz = (int)Test.z;
   x = Test.x - (DBL)px;
   z = Test.z - (DBL)pz;

   if((x+z)<1.0)
   {
      y1 = Get_Height(px,pz,H_Field);
      y2 = Get_Height(px+1,pz,H_Field);
      y3 = Get_Height(px,pz+1,H_Field);
      Make_Vector(&Local_Origin,(DBL)px,y1,(DBL)pz);
      Temp1.x = 1.0;
      Temp1.z = 0.0;
      Temp1.y = y2 - y1;
      Temp2.x = 0.0;
      Temp2.z = 1.0;
      Temp2.y = y3 - y1;
   }
   else
   {
      px = ceil(Test.x);
      pz = ceil(Test.z);
      y1 = Get_Height(px,pz,H_Field);
      y2 = Get_Height(px-1,pz,H_Field);
      y3 = Get_Height(px,pz-1,H_Field);
      Make_Vector(&Local_Origin,(DBL)px,y1,(DBL)pz);
      Temp1.x = -1.0;
      Temp1.z = 0.0;
      Temp1.y = y2 - y1;
      Temp2.x = 0.0;
      Temp2.z = -1.0;
      Temp2.y = y3 - y1;
   }
   VCross(Local_Normal,Temp2,Temp1);
   if(Local_Normal.y < 0.0) {
      VScale(Local_Normal,Local_Normal,-1.0);
   }
   VDot(dot1,Test,Local_Normal);
   VDot(dot2,Local_Origin,Local_Normal);
   if((dot1 < dot2) && (Test.y > (H_Field->bounding_box->bounds[0].y)+1.0))
      return(TRUE);
   return(FALSE);
}

void HeightFld_Normal (Result, Object, Intersection_Point)
OBJECT *Object;
VECTOR *Result, *Intersection_Point;
{
   HEIGHT_FIELD *H_Field = (HEIGHT_FIELD *) Object;
   int px,pz;
   DBL x,z,y1,y2,y3;
   VECTOR Local_Origin, Temp1, Temp2;

   MInverseTransformVector(&Local_Origin, Intersection_Point, H_Field->Transformation);

   px = (int)Local_Origin.x;
   pz = (int)Local_Origin.z;
   x = Local_Origin.x - (DBL)px;
   z = Local_Origin.z - (DBL)pz;

   if((x+z)<=1)
   {
      y1 = Get_Height(px,pz,H_Field);
      y2 = Get_Height(px+1,pz,H_Field);
      y3 = Get_Height(px,pz+1,H_Field);
      Temp1.x = 1.0;
      Temp1.z = 0.0;
      Temp1.y = y2 - y1;
      Temp2.x = 0.0;
      Temp2.z = 1.0;
      Temp2.y = y3 - y1;
   }
   else
   {
      y1 = Get_Height(px+1,pz+1,H_Field);
      y2 = Get_Height(px,pz+1,H_Field);
      y3 = Get_Height(px+1,pz,H_Field);
      Temp1.x = -1.0;
      Temp1.z = 0.0;
      Temp1.y = y2 - y1;
      Temp2.x = 0.0;
      Temp2.z = -1.0;
      Temp2.y = y3 - y1;
   }

   MTransVector(&Temp1,&Temp1,H_Field->Transformation);
   MTransVector(&Temp2,&Temp2,H_Field->Transformation);
   VCross(*Result, Temp2, Temp1);
   VNormalize(*Result,*Result);
   return;
}

void *Copy_HeightFld (Object)
OBJECT *Object;
{
   HEIGHT_FIELD *New_Shape;

   New_Shape = Get_Height_Field_Shape();
   *New_Shape = * ((HEIGHT_FIELD *)Object);
   New_Shape -> Next_Object = NULL;

   if (New_Shape->Shape_Texture != NULL)
      New_Shape->Shape_Texture = Copy_Texture (New_Shape->Shape_Texture);

   return (New_Shape);
}

void Translate_HeightFld (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   HEIGHT_FIELD *H_Field = (HEIGHT_FIELD *) Object;
   TRANSFORMATION Transformation;

   if (! H_Field -> Transformation)
      H_Field -> Transformation = Get_Transformation ();
   Get_Translation_Transformation(&Transformation,Vector);
   Compose_Transformations(H_Field->Transformation,&Transformation);

   Translate_Texture (&((HEIGHT_FIELD *) Object)->Shape_Texture, Vector);
}

void Rotate_HeightFld (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   TRANSFORMATION Transformation;
   HEIGHT_FIELD *H_Field = (HEIGHT_FIELD *)Object;

   if (! H_Field -> Transformation)
      H_Field -> Transformation = Get_Transformation ();
   Get_Rotation_Transformation(&Transformation,Vector);
   Compose_Transformations(H_Field->Transformation,&Transformation);

   Rotate_Texture (&((HEIGHT_FIELD *) Object)->Shape_Texture, Vector);
}

void Scale_HeightFld (Object, Vector)
OBJECT *Object;
VECTOR *Vector;
{
   HEIGHT_FIELD *H_Field = (HEIGHT_FIELD *) Object;
   TRANSFORMATION Transformation;

   if (! H_Field -> Transformation)
      H_Field -> Transformation = Get_Transformation ();
   Get_Scaling_Transformation(&Transformation,Vector);
   Compose_Transformations(H_Field->Transformation,&Transformation);

   Scale_Texture (&((HEIGHT_FIELD *) Object)->Shape_Texture, Vector);
}

void Invert_HeightFld (Object)
OBJECT *Object;
{
   return;
}
