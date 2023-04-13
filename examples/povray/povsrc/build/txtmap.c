/****************************************************************************
*                txtmap.c
*
*  This module implements the mapped textures including image map, bump map
*  and material map. 
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
#include "texture.h"


#ifdef _ICC
static int cylindrical_image_map PARAMS((DBL x, DBL y, DBL z, IMAGE *Image, DBL *u, DBL *v));
static int torus_image_map PARAMS((DBL x, DBL y, DBL z, IMAGE *Image, DBL *u, DBL *v));
static int spherical_image_map PARAMS((DBL x, DBL y, DBL z, IMAGE *Image, DBL *u, DBL *v));
static int planar_image_map PARAMS((DBL x, DBL y, DBL z, IMAGE *Image, DBL *u, DBL *v));
static void no_interpolation PARAMS((IMAGE *Image,DBL xcoor,DBL ycoor,COLOUR *colour,int *index));
static DBL bilinear PARAMS((DBL *corners,DBL x,DBL y));
static DBL norm_dist PARAMS((DBL *corners,DBL x,DBL y));
static void Interp PARAMS((IMAGE *Image,DBL xcoor,DBL ycoor,COLOUR *colour,int *index));
static void image_colour_at PARAMS((IMAGE *Image,DBL xcoor,DBL ycoor,COLOUR *colour,int *index));
#else
extern int cylindrical_image_map PARAMS((DBL x, DBL y, DBL z, IMAGE *Image, DBL *u, DBL *v));
extern int torus_image_map PARAMS((DBL x, DBL y, DBL z, IMAGE *Image, DBL *u, DBL *v));
extern int spherical_image_map PARAMS((DBL x, DBL y, DBL z, IMAGE *Image, DBL *u, DBL *v));
extern int planar_image_map PARAMS((DBL x, DBL y, DBL z, IMAGE *Image, DBL *u, DBL *v));
extern void no_interpolation PARAMS((IMAGE *Image,DBL xcoor,DBL ycoor,COLOUR *colour,int *index));
extern DBL bilinear PARAMS((DBL *corners,DBL x,DBL y));
extern DBL norm_dist PARAMS((DBL *corners,DBL x,DBL y));
extern void Interp PARAMS((IMAGE *Image,DBL xcoor,DBL ycoor,COLOUR *colour,int *index));
extern void image_colour_at PARAMS((IMAGE *Image,DBL xcoor,DBL ycoor,COLOUR *colour,int *index));
#endif

/*
   2-D to 3-D Procedural Texture Mapping of a Bitmapped Image onto an Object:
	   
A. Simplistic (planar) method of image projection devised by DKB and AAC:

   1. Transform texture in 3-D space if requested.
   2. Determine local object 2-d coords from 3-d coords by <X Y Z> triple.
   3. Return pixel color value at that position on the 2-d plane of "Image".
   3. Map colour value in Image [0..255] to a more normal colour range [0..1].

B. Specialized shape projection variations by Alexander Enzmann:

   1. Cylindrical mapping
   2. Spherical mapping
   3. Torus mapping
*/

void image_map (x, y, z, Texture, colour)
DBL x, y, z;
TEXTURE *Texture;
COLOUR *colour;
{
   /* determine local object 2-d coords from 3-d coords */
   /* "unwrap" object 2-d coord onto flat 2-d plane */
   /* return pixel color value at that posn on 2-d plane */

   DBL xcoor = 0.0, ycoor = 0.0 ;
   int reg_number;


   if(map (x, y, z, Texture,Texture->Image,&xcoor, &ycoor)){
      Make_Colour(colour,1.0,1.0,1.0);
      colour->Alpha=1.0;
      return;
   }
   image_colour_at(Texture->Image,xcoor, ycoor, colour, &reg_number);
}

/* Very different stuff than the other routines here. This routine takes  */
/* an intersection point and a texture and returns a new texture based on */
/* the index/color of that point in an image/materials map. CdW 7/91      */

TEXTURE *material_map(Intersection_Point,Texture)
VECTOR *Intersection_Point;
TEXTURE *Texture;
{
   VECTOR Transformed_Point;
   register DBL x, y, z;
   DBL xcoor = 0.0, ycoor = 0.0;
   int reg_number = 0;
   COLOUR colour;
   int Material_Number=0;
   TEXTURE *Temp_Tex;
   int numtex;

   Make_Colour(&colour,0.0,0.0,0.0);
   colour.Alpha=0.0; 

   if (Texture->Texture_Transformation)
      MInverseTransformVector (&Transformed_Point,
         Intersection_Point,
         Texture->Texture_Transformation);
   else 
      Transformed_Point = *Intersection_Point;

   x = Transformed_Point.x;
   y = Transformed_Point.y;
   z = Transformed_Point.z;

   /* now we have transformed x, y, z we use image mapping routine */
   /* to determine texture index */
   if(map (x, y, z, Texture, Texture->Material_Image,&xcoor, &ycoor))
      Material_Number=0;
   else{
      image_colour_at(Texture->Material_Image,xcoor, ycoor, &colour, &reg_number);

      if (Texture->Material_Image->Colour_Map == NULL) 
         Material_Number = (int)colour.Red*255;
      else 
         Material_Number = reg_number;
   }    

   /* Now I've got the Material number, I just have to find it in the */  
   /* texture linked list and return it to Determine_Surface_Colour   */
   /* printf("-B-Num Mt#%d Mn#%d\n",Texture->Number_Of_Materials, Material_Number);   */
   if(Material_Number > Texture->Number_Of_Materials)
      Material_Number %= Texture->Number_Of_Materials;
   for(numtex=0, Temp_Tex = Texture->Next_Material;
           Temp_Tex->Next_Material != NULL && numtex<Material_Number; 
           Temp_Tex = Temp_Tex->Next_Material, numtex++)
      ; /* do nothing */

   /* Texture = Temp_Tex;  */

   return(Temp_Tex);  
}


void bump_map (x, y, z, Texture, normal)
DBL x, y, z;
TEXTURE *Texture;
VECTOR *normal;
{
   DBL xcoor = 0.0, ycoor = 0.0;
   int index,index2,index3;
   COLOUR colour, colour2, colour3;
   VECTOR p1,p2,p3;
   VECTOR bump_normal;
   VECTOR xprime, yprime, zprime, Temp;
   DBL Length;
   Make_Colour(&colour,0.0,0.0,0.0);
   colour.Alpha=0.0; 
   Make_Colour(&colour2,0.0,0.0,0.0);
   colour2.Alpha=0.0; 
   Make_Colour(&colour3,0.0,0.0,0.0);
   colour3.Alpha=0.0; 

   /* going to have to change this */
   /* need to know if bump point is off of image for all 3 points */

   if(map (x, y, z, Texture, Texture->Bump_Image,&xcoor, &ycoor)){
      Make_Colour(&colour,1.0,1.0,1.0);
      colour.Alpha=1.0;
      index = 255;
      return;
   }
   else
      image_colour_at(Texture->Bump_Image,xcoor, ycoor, &colour, &index);

   xcoor--;
   ycoor++;
   if (xcoor < 0.0)
      xcoor += (DBL)Texture->Bump_Image->iwidth;
   else if (xcoor >= Texture->Bump_Image->iwidth)
      xcoor -= (DBL)Texture->Bump_Image->iwidth;
   if (ycoor < 0.0)
      ycoor += (DBL)Texture->Bump_Image->iheight;
   else if (ycoor >=(DBL) Texture->Bump_Image->iheight)
      ycoor -= (DBL)Texture->Bump_Image->iheight;
   image_colour_at(Texture->Bump_Image,xcoor, ycoor, &colour2, &index2);

   xcoor +=2.0;
   if (xcoor < 0.0)
      xcoor += (DBL)Texture->Bump_Image->iwidth;
   else if (xcoor >= Texture->Bump_Image->iwidth)
      xcoor -= (DBL)Texture->Bump_Image->iwidth;

   image_colour_at(Texture->Bump_Image,xcoor, ycoor, &colour3, &index3);

   if (Options & DEBUGGING)
      printf ("Bump Map %g %g %g xcoor %lf ycoor %lf\n", x, y, z, xcoor, ycoor);

   if (Texture->Bump_Image->Colour_Map == NULL || 
      Texture->Bump_Image->Use_Colour_Flag ) {
      p1.x = 0; 
      p1.y =
      Texture->Bump_Amount*(0.229*colour.Red+0.587*colour.Green+0.114*colour.Blue);
      p1.z = 0;
      p2.x = 0; 
      p2.y =
      Texture->Bump_Amount*(0.229*colour2.Red+0.587*colour2.Green+0.114*colour2.Blue);
      p2.z = 1;
      p3.x = 1; 
      p3.y =
      Texture->Bump_Amount*(0.229*colour3.Red+0.587*colour3.Green+0.114*colour3.Blue);
      p3.z = 1;
   }
   else {
      p1.x = 0; 
      p1.y = Texture->Bump_Amount * index;
      p1.z = 0;
      p2.x = 0; 
      p2.y = Texture->Bump_Amount * index2;
      p2.z = 1;
      p3.x = 1; 
      p3.y = Texture->Bump_Amount * index3;
      p3.z = 1;
   }
   /* we have points 1,2,3 for a triangle now we need the surface normal for it
*/  
   VSub (xprime, p1, p2);
   VSub (yprime, p3, p2);
   VCross (bump_normal, yprime, xprime);
   VNormalize(bump_normal, bump_normal);

      Make_Vector(&yprime,normal->x,normal->y,normal->z);
   Make_Vector(&Temp,0.0,1.0,0.0);
   VCross (xprime,yprime,Temp);
   VLength(Length,xprime);
   if(Length < 1.0e-9)
   {
      if(fabs(normal->y - 1.0)<Small_Tolerance)
      {
         Make_Vector(&yprime,0.0,1.0,0.0);
         Make_Vector(&xprime,1.0,0.0,0.0);
         Length = 1.0;
      }
      else
      {
         Make_Vector(&yprime,0.0,-1.0,0.0);
         Make_Vector(&xprime,1.0,0.0,0.0);
         Length = 1.0;
      }
   }
   VScale(xprime,xprime,1.0/Length);
   VCross (zprime,xprime,yprime);
   VNormalize(zprime,zprime);
   VScale(xprime,xprime,bump_normal.x);
   VScale(yprime,yprime,bump_normal.y);
   VScale(zprime,zprime,bump_normal.z);
   VAdd(Temp,xprime,yprime);
   VAdd(*normal,Temp,zprime);
   VNormalize(*normal,*normal);

   return;
}





static void image_colour_at(Image, xcoor, ycoor, colour, index)
IMAGE *Image;     
DBL xcoor, ycoor;
COLOUR *colour;
int *index;
{  

   switch(Image->Interpolation_Type){
   case NO_INTERPOLATION:
      no_interpolation(Image,xcoor,ycoor,colour,index);
      break;
   default: 
      Interp(Image,xcoor, ycoor, colour, index);
      break;
   }      
}



/* Map a point (x, y, z) on a cylinder of radius 1, height 1, that has its
   axis of symmetry along the y-axis to the square [0,1]x[0,1]. */
static int cylindrical_image_map (x, y, z, Image, u, v)
DBL x, y, z;
IMAGE *Image;
DBL *u, *v;
{
   DBL len, theta;

   if ((Image->Once_Flag) && ((y < 0.0) || (y > 1.0)))
      return 0;
   *v = fmod (y * Image->height, Image->height);

   /* Make sure this vector is on the unit sphere. */
   len = sqrt(x*x + y*y + z*z);
   if (len == 0.0)
      return 0;
   else {
      x /= len;
      z /= len;
   }      
   /* Determine its angle from the point (1, 0, 0) in the x-z plane. */
      len = sqrt(x*x + z*z);
   if (len == 0.0)
      return 0;
   else {
      if (z == 0.0)
         if (x > 0)
            theta = 0.0;
         else
            theta = M_PI;
      else {
         theta = acos(x / len);
         if (z < 0.0) theta = 2.0 * M_PI - theta;
      }
      theta /= 2.0 * M_PI; /* This will be from 0 to 1 */
   }
   *u = (theta * Image->width);
   return 1;
}

/* Map a point (x, y, z) on a torus  to a 2-d image. */
static int torus_image_map (x, y, z, Image, u, v)
DBL x, y, z;
IMAGE *Image;
DBL *u, *v;
{
   DBL len, phi, theta;
   DBL r0, r1;

   r0 = Image->Image_Gradient.x;
   r1 = Image->Image_Gradient.y;

   /* Determine its angle from the x-axis. */
   len = sqrt(x*x + z*z);
   if (len == 0.0)
      return 0;
   else {
      if (z == 0.0)
         if (x > 0)
            theta = 0.0;
         else
            theta = M_PI;
      else {
         theta = acos(x / len);
         if (z < 0.0) theta = 2.0 * M_PI - theta;
      }
   }
   theta = 0.0 - theta;

   /* Now rotate about the y-axis to get the point (x, y, z) into the x-y plane. */
   x = len - r0;
   len = sqrt(x * x + y * y);
   phi = acos(-x / len);
   if (y > 0.0) phi = 2.0 * M_PI - phi;

   /* Determine the parametric coordinates. */
   theta /= 2.0 * M_PI;
   phi /= 2.0 * M_PI;
   *u = (-theta * Image->width);
   *v = (phi * Image->height);
   return 1;
}

/* Map a point (x, y, z) on a sphere of radius 1 to a 2-d image. (Or is it the
   other way around?) */
static int spherical_image_map (x, y, z, Image, u, v)
DBL x, y, z;
IMAGE *Image;
DBL *u, *v;
{
   DBL len, phi, theta;

   /* Make sure this vector is on the unit sphere. */
   len = sqrt(x*x + y*y + z*z);
   if (len == 0.0)
      return 0;
   else {
      x /= len;
      y /= len;
      z /= len;
   }
   /* Determine its angle from the x-z plane. */
      phi = 0.5 + asin(y) / M_PI; /* This will be from 0 to 1 */

   /* Determine its angle from the point (1, 0, 0) in the x-z plane. */
   len = sqrt(x*x + z*z);
   if (len == 0.0) {
      /* This point is at one of the poles. Any value of xcoord will be ok...*/
      theta = 0;
   }
   else {
      if (z == 0.0)
         if (x > 0)
            theta = 0.0;
         else
            theta = M_PI;
      else {
         theta = acos(x / len);
         if (z < 0.0) theta = 2.0 * M_PI - theta;
      }
      theta /= 2.0 * M_PI; /* This will be from 0 to 1 */
   }
   *u = (theta * Image->width);
   *v = (phi * Image->height);
   return 1;
}

/*
   2-D to 3-D Procedural Texture Mapping of a Bitmapped Image onto an Object:
	   
   Simplistic planar method of object image projection devised by DKB and AAC.

   1. Transform texture in 3-D space if requested.
   2. Determine local object 2-d coords from 3-d coords by <X Y Z> triple.
   3. Return pixel color value at that position on the 2-d plane of "Image".
   3. Map colour value in Image [0..255] to a more normal colour range [0..1].
*/

/* Return 0 if there is no color at this point (i.e. invisible), return 1
   if a good mapping is found. */
static int planar_image_map(x, y, z, Image, u, v)
DBL x, y, z;
IMAGE *Image;
DBL *u, *v;
{  
   if (Image -> Image_Gradient.x != 0.0) {
      if ((Image->Once_Flag) &&
         ((x < 0.0) || (x > 1.0)))
         return 0;
      if (Image -> Image_Gradient.x > 0)
         *u = fmod (x * Image->width, Image->width);
      else
         *v = fmod (x * Image->height, Image->height);
   }
   if (Image -> Image_Gradient.y != 0.0) {
      if ((Image->Once_Flag) &&
         ((y < 0.0) || (y > 1.0)))
         return 0;
      if (Image -> Image_Gradient.y > 0)
         *u = fmod (y * Image->width, Image->width);
      else
         *v = fmod (y * Image->height, Image->height);
   }
   if (Image -> Image_Gradient.z != 0.0) {
      if ((Image->Once_Flag) &&
         ((z < 0.0) || (z > 1.0)))
         return 0;
      if (Image -> Image_Gradient.z > 0)
         *u = fmod (z * Image->width, Image->width);
      else
         *v = fmod (z * Image->height, Image->height);
   }
   return 1;
}
/* Map returns 1 if no color found (invisible) or 0 if color found */ 
int map (x, y, z, Texture, Image, xcoor, ycoor)
DBL x, y, z;
TEXTURE *Texture;
IMAGE *Image;
DBL *xcoor, *ycoor;
{
   /* determine local object 2-d coords from 3-d coords */
   /* "unwrap" object 2-d coord onto flat 2-d plane */
   /* return pixel color value at that posn on 2-d plane */
   DBL turb;
   VECTOR TextureTurbulence;
   
   /* This causes problems so let's do without it for this release
   if ((turb = Texture->Turbulence) != 0.0)
   {
      DTurbulence (&TextureTurbulence, x, y, z, Texture->Octaves);
      x += TextureTurbulence.x * turb;
      y += TextureTurbulence.y * turb;
      z += TextureTurbulence.z * turb;
   }
   */
   
   /* Now determine which mapper to use. */
   switch (Image->Map_Type) {
   case PLANAR_MAP:
      if (!planar_image_map(x, y, z, Image, xcoor, ycoor)) 
         return(1);
      break;
   case SPHERICAL_MAP:
      if (!spherical_image_map(x, y, z,Image, xcoor, ycoor)) 
         return(1);
      break;
   case CYLINDRICAL_MAP:
      if (!cylindrical_image_map(x, y, z,Image, xcoor, ycoor)) 
         return(1);
      break;
   case TORUS_MAP:
      if (!torus_image_map(x, y, z,Image, xcoor, ycoor)) 
         return(1);
      break;
   default:
      if (!planar_image_map(x, y, z,Image, xcoor, ycoor)) 
         return(1);
      break;
   }
   /* Now make sure the point is on the image */
   *ycoor += Small_Tolerance;
   *xcoor += Small_Tolerance;
   /* Compensate for y coordinates on the images being upsidedown */
   *ycoor = (DBL)Image->iheight - *ycoor;

   if (*xcoor < 0.0)
      *xcoor +=(DBL) Image->iwidth;
   else if (*xcoor >=(DBL)Image->iwidth)
      *xcoor -= (DBL)Image->iwidth;

   if (*ycoor < 0.0)
      *ycoor += (DBL)Image->iheight;
   else if (*ycoor >= (DBL)Image->iheight)
      *ycoor -= (DBL)Image->iheight;

   if (Options & DEBUGGING)
      printf ("\nmap %g %g %g xcoor %d ycoor %d ih %d iw %d\n", x, y, z, *xcoor, *ycoor,Image->iheight,Image->iwidth);

   if ((*xcoor >= (DBL)Image->iwidth) ||
      (*ycoor >= (DBL)Image->iheight) ||
      (*xcoor < 0.0) || (*ycoor < 0.0)) {
      printf ("\nPicture index out of range\n");
      close_all();
      exit (1);
   }

   return(0);    
}

static void no_interpolation(Image, xcoor, ycoor, colour, index)
IMAGE *Image;     
DBL xcoor, ycoor;
COLOUR *colour;
int *index;
{  
   struct Image_Line *line;
   int iycoor, ixcoor;
   IMAGE_COLOUR *map_colour;

   if (xcoor < 0.0)
      xcoor += (DBL)Image->iwidth;
   else if (xcoor >= (DBL)Image->iwidth)
      xcoor -= (DBL)Image->iwidth;
   if (ycoor < 0.0)
      ycoor += (DBL)Image->iheight;
   else if (ycoor >=(DBL) Image->iheight)
      ycoor -= (DBL) Image->iheight;

   iycoor=(int)ycoor;
   ixcoor=(int)xcoor;
   if (Image->Colour_Map == NULL) {
      line = &Image->data.rgb_lines[iycoor];
      colour -> Red += (DBL) line->red[ixcoor]/255.0;
      colour -> Green += (DBL) line->green[ixcoor]/255.0;
      colour -> Blue += (DBL) line->blue[ixcoor]/255.0;
      *index = -1;
   }    
   else {   
      *index = Image->data.map_lines[iycoor][ixcoor];
      map_colour = &Image->Colour_Map[*index];
      /*printf ("icat index %d xc %d yc %d  CLR %d %d %d %d\n",*index,ixcoor,iycoor, 
    map_colour->Red,map_colour->Green,map_colour->Blue,map_colour->Alpha ); */
      colour -> Red   += (DBL) map_colour->Red/255.0;
      colour -> Green += (DBL) map_colour->Green/255.0;
      colour -> Blue  += (DBL) map_colour->Blue/255.0;
      colour -> Alpha += (DBL) map_colour->Alpha/255.0;
   }

      if (Options & DEBUGGING)
         printf ("\n no_interpolation index %d xc %d yc %d \n",*index,ixcoor,iycoor);
}


/* Interpolate color and alpha values when mapping */

static void Interp(Image, xcoor, ycoor, colour, index)
IMAGE *Image;     
DBL xcoor, ycoor;
COLOUR *colour;
int *index;
{  
   int iycoor, ixcoor,i;
   int Corners_Index[4];
   DBL Index_Crn[4];
   COLOUR Corner_Colour[4];
   DBL Red_Crn[4];
   DBL Green_Crn[4];
   DBL Blue_Crn[4];
   DBL Alpha_Crn[4];
   DBL val1, val2, val3,val4;

   iycoor=(int)ycoor;
   ixcoor=(int)xcoor;
   for(i=0;i<4;i++){
      Make_Colour(&Corner_Colour[i],0.0,0.0,0.0);
      Corner_Colour[i].Alpha=0.0; 
   }
   /* OK, now that you have the corners, what are you going to do with them? */
   if(Image->Interpolation_Type==BILINEAR){
      no_interpolation(Image,(DBL)ixcoor+1,(DBL)iycoor,&Corner_Colour[0],&Corners_Index[0]);
      no_interpolation(Image,(DBL)ixcoor,(DBL)iycoor,&Corner_Colour[1],&Corners_Index[1]);
      no_interpolation(Image,(DBL)ixcoor+1,(DBL)iycoor-1,&Corner_Colour[2],&Corners_Index[2]);
      no_interpolation(Image,(DBL)ixcoor,(DBL)iycoor-1,&Corner_Colour[3],&Corners_Index[3]);
      for(i=0;i<4;i++){
         Red_Crn[i] = Corner_Colour[i].Red;
         Green_Crn[i] = Corner_Colour[i].Green;
         Blue_Crn[i] = Corner_Colour[i].Blue;
         Alpha_Crn[i] = Corner_Colour[i].Alpha;
         /* printf("Crn %d = %lf %lf %lf\n",i,Red_Crn[i],Blue_Crn[i],Green_Crn[i]); */
      }

      val1 = bilinear(Red_Crn,xcoor,ycoor);
      val2 = bilinear(Green_Crn,xcoor,ycoor);
      val3 = bilinear(Blue_Crn,xcoor,ycoor);
      val4 = bilinear(Alpha_Crn,xcoor,ycoor);
   }
   if(Image->Interpolation_Type==NORMALIZED_DIST){
      no_interpolation(Image,(DBL)ixcoor,(DBL)iycoor-1,&Corner_Colour[0],&Corners_Index[0]);
      no_interpolation(Image,(DBL)ixcoor+1,(DBL)iycoor-1,&Corner_Colour[1],&Corners_Index[1]);
      no_interpolation(Image,(DBL)ixcoor,(DBL)iycoor,&Corner_Colour[2],&Corners_Index[2]);
      no_interpolation(Image,(DBL)ixcoor+1,(DBL)iycoor,&Corner_Colour[3],&Corners_Index[3]);
      for(i=0;i<4;i++){
         Red_Crn[i] = Corner_Colour[i].Red;
         Green_Crn[i] = Corner_Colour[i].Green;
         Blue_Crn[i] = Corner_Colour[i].Blue;
         Alpha_Crn[i] = Corner_Colour[i].Alpha;
         /* printf("Crn %d = %lf %lf %lf\n",i,Red_Crn[i],Blue_Crn[i],Green_Crn[i]); */
      }

      val1 = norm_dist(Red_Crn,xcoor,ycoor);
      val2 = norm_dist(Green_Crn,xcoor,ycoor);
      val3 = norm_dist(Blue_Crn,xcoor,ycoor);
      val4 = norm_dist(Alpha_Crn,xcoor,ycoor);
   }

   colour->Red   += val1;
   colour->Green += val2;
   colour->Blue  += val3;
   colour->Alpha += val4;
   /* printf("Final = %lf %lf %lf\n",val1,val2,val3);  */
   /* use bilinear for index try average later */
   for(i=0;i<4;i++)
      Index_Crn[i] = (DBL) Corners_Index[i];
   if(Image->Interpolation_Type==BILINEAR){
      *index = (int)(bilinear(Index_Crn,xcoor,ycoor)+0.5);
   }
   if(Image->Interpolation_Type==NORMALIZED_DIST){
      *index = (int)(norm_dist(Index_Crn,xcoor,ycoor)+0.5);
   }
}

/* These interpolation techniques are taken from an article by */
/* Girish T. Hagan in the C Programmer's Journal V 9 No. 8 */
/* They were adapted for POV-Ray by CdW */

static DBL bilinear (corners, x,y)
DBL *corners, x,y;   
{
   DBL p,q;
   DBL val = 0.0;

   p = x - (int) x;
   q = y - (int) y;
   if ((p==0.0) && (q==0.0))
      return(*corners); /* upper left */

   val = (p*q* *corners) + (q*(1-p)* *(corners+1)) + 
   (p*(1-q)* *(corners+2)) + ((1-p)*(1-q)* *(corners+3));
   return(val);
}  


#define MAX_PTS 4
#define PYTHAGOREAN_SQ(a,b)  ( (a)*(a) + (b)*(b) )

static DBL norm_dist(corners,x,y)
DBL *corners,x,y;
{
   register int i;

   DBL p,q;
   DBL wts[MAX_PTS];
   DBL sum_inv_wts = 0.0;
   DBL sum_I  = 0.0;

   p = x - (int) x;
   q = y - (int) y;

   if( (p==0.0) && (q==0.0) )
      return(*corners); /* upper left */

   wts[0] = PYTHAGOREAN_SQ(p,q);
   wts[1] = PYTHAGOREAN_SQ(1-p,q);
   wts[2] = PYTHAGOREAN_SQ(p,1-q);
   wts[3] = PYTHAGOREAN_SQ(1-p,1-q);

   for(i=0; i<MAX_PTS; i++){
      sum_inv_wts += 1/wts[i];
      sum_I += *(corners+i)/wts[i];
   }

   return( sum_I /sum_inv_wts );
}
