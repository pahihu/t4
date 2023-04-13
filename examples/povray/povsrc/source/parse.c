/****************************************************************************
*                parse.c
*
*  This module implements a parser for the scene description files.
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



/* This file implements a simple recursive-descent parser for reading the
input file.  */

static void Link_Shapes PARAMS((LIGHT_SHAPE *New_Object,LIGHT_SHAPE **Field,LIGHT_SHAPE **Old_Object_List));
static void Post_Process_Object PARAMS((OBJECT *Object));
static void Post_Process_Shape PARAMS((SHAPE *Shape));



extern DBL Max_Trace_Level;
extern char VerboseFormat;
extern unsigned int Options;
extern char Stat_File_Name[FILE_NAME_LENGTH];


FRAME *Parsing_Frame_Ptr;

extern METHODS Composite_Methods;
extern METHODS Basic_Object_Methods;
extern METHODS Sphere_Methods;
extern METHODS Point_Methods;
extern METHODS Quadric_Methods;
extern METHODS Poly_Methods;
extern METHODS Box_Methods;
extern METHODS Blob_Methods;
extern METHODS Bicubic_Patch_Methods;
extern METHODS Viewpoint_Methods;
extern METHODS Plane_Methods;
extern METHODS Triangle_Methods;
extern METHODS Smooth_Triangle_Methods;
extern METHODS CSG_Union_Methods;
extern METHODS CSG_Intersection_Methods;
extern METHODS Height_Field_Methods;

extern struct Reserved_Word_Struct Reserved_Words [];
extern DBL Antialias_Threshold;

extern int term_counts[MAX_ORDER+1];
extern struct Token_Struct Token;

COLOUR_MAP_ENTRY *Construction_Map = NULL; /* moved here to allow reinitialization */

struct Constant_Struct Constants[MAX_CONSTANTS];
int Number_Of_Constants;
TEXTURE *Default_Texture;
int Degenerate_Triangles;

/* Here we create our own little language for doing the parsing.  It
makes the code easier to read. */

#define EXPECT { int Exit_Flag; Exit_Flag = FALSE; \
while (!Exit_Flag) {Get_Token();  switch (Token.Token_Id) {
#define CASE(x) case x:
#define CASE2(x, y) case x: case y:
#define CASE3(x, y, z) case x: case y: case z:
#define CASE4(w, x, y, z) case w: case x: case y: case z:
#define CASE5(v, w, x, y, z) case v: case w: case x: case y: case z:
#define CASE6(u, v, w, x, y, z) case u: case v: case w: case x: case y: case z:
#define END_CASE break;
#define EXIT Exit_Flag = TRUE;
#define OTHERWISE default:
#define END_EXPECT } } }
#define GET(x) Get_Token(); if (Token.Token_Id != x) Parse_Error (x)
#define UNGET Unget_Token();

      /*

char *Coeff_terms[35] = {
   "x^4", "x^3*y", "x^3*z", "x^3", "x^2*y^2", "x^2*y*z", "x^2*y", "x^2*z^2",
   "x^2*z", "x^2", "x*y^3", "x*y^2*z", "x*y^2", "x*y*z^2", "x*y*z", "x*y",
   "x*z^3", "x*z^2", "x*z", "x", "y^4", "y^3*z", "y^3", "y^2*z^2", "y^2*z",
   "y^2", "y*z^3", "y*z^2", "y*z", "y", "z^4", "z^3", "z^2", "z", ""
   };

void show_quartic(Coeffs)
DBL *Coeffs;
{
   int i,j;
   for (i=0,j=0;i<35;i++)
      if (Coeffs[i] != 0.0) {
	 if (j) printf(" + ");
	 printf("%.5Lg %s", Coeffs[i], Coeff_terms[i]);
	 j = 1;
	 }
}

*/

      /* Parse the file into the given frame. */
      void Parse (Frame_Ptr)
         FRAME *Frame_Ptr;
      {
         OBJECT *Object;
         Parsing_Frame_Ptr = Frame_Ptr;

         Degenerate_Triangles = FALSE;
         Token_Init ();
         Frame_Init ();
         Parse_Frame ();
         for (Object = Parsing_Frame_Ptr->Objects;
            	Object != NULL;
            	Object = Object->Next_Object)
            Post_Process_Object (Object);
         if (Degenerate_Triangles) {
            fprintf (stderr, "Degenerate triangles were found and are being ignored.\n");
            /* exit(1); Let's ignore degen tri instead of blowing up. CdW */
         }
      }

      void Token_Init ()
      {
         Number_Of_Constants = 0;
         /*
  Constants = (struct Constant_Struct *)malloc(
	  sizeof(struct Constant_Struct) *MAX_CONSTANTS);
  */
      }


      /* Set up the fields in the frame to default values. */
      void Frame_Init ()
      {
         Default_Texture = Get_Texture();
         Init_Viewpoint(&(Parsing_Frame_Ptr -> View_Point));
         Parsing_Frame_Ptr -> Light_Sources = NULL;
         Parsing_Frame_Ptr -> Objects = NULL;
         Parsing_Frame_Ptr -> Atmosphere_IOR = 1.0;
         Parsing_Frame_Ptr -> Antialias_Threshold = Antialias_Threshold;
         Parsing_Frame_Ptr -> Fog_Distance = 0.0;
         Make_Colour (&(Parsing_Frame_Ptr->Fog_Colour), 0.0, 0.0, 0.0);
      }

      /* Allocate and initialize a composite object. */
      COMPOSITE *Get_Composite_Object()
      {
         COMPOSITE *New_Composite;

         if ((New_Composite = (COMPOSITE *) malloc (sizeof (COMPOSITE)))
            == NULL)
            Error ("Out of memory. Cannot allocate object");

         New_Composite -> Objects = NULL;
         New_Composite -> Next_Object = NULL;
         /*  New_Composite -> Next_Light_Source = NULL;*/
         New_Composite -> Bounding_Shapes = NULL;
         New_Composite -> Clipping_Shapes = NULL;
         New_Composite -> Type = COMPOSITE_TYPE;
         New_Composite -> Methods = &Composite_Methods;
         return (New_Composite);
      }

      /* Allocate and initialize a sphere. */
      SPHERE *Get_Sphere_Shape()
      {
         SPHERE *New_Shape;

         if ((New_Shape = (SPHERE *) malloc (sizeof (SPHERE))) == NULL)
            Error ("Out of memory. Cannot allocate shape");

         Make_Vector (&(New_Shape -> Center), 0.0, 0.0, 0.0);
         New_Shape->Radius = 1.0;
         New_Shape->Radius_Squared = 1.0;
         New_Shape->Inverse_Radius = 1.0;
         New_Shape -> Type = SPHERE_TYPE;
         New_Shape -> Next_Object = NULL;
         New_Shape -> Methods = &Sphere_Methods;
         New_Shape -> VPCached = FALSE;
         New_Shape -> Inverted = FALSE;
         New_Shape -> Shape_Texture = NULL;
         New_Shape -> Shape_Colour = NULL;
         return (New_Shape);
      }

      /* Allocate and initialize a light source. */
      /* A point light source has no shape, but we'll treat it like it does */

      LIGHT_SHAPE *Get_Light_Source_Shape()
      {
         LIGHT_SHAPE *New_Shape;

         if ((New_Shape = (LIGHT_SHAPE *) malloc (sizeof (LIGHT_SHAPE))) == NULL)
            Error ("Out of memory. Cannot allocate shape");
         Make_Vector (&(New_Shape -> Center), 0.0, 0.0, 0.0);
         Make_Vector (&(New_Shape -> Points_At), 0.0, 0.0, 1.0);
         New_Shape -> Type = POINT_LIGHT_TYPE;
         New_Shape -> Methods = &Point_Methods;
         New_Shape -> Next_Object = NULL;
         New_Shape -> Inverted = FALSE; /* needed so CSG routines don't blow up */
         New_Shape -> Shape_Texture = NULL; /* always NULL */
         New_Shape -> Shape_Colour = Get_Colour();  /* becomes light colour */
         Make_Colour(New_Shape->Shape_Colour, 1.0, 1.0, 1.0);
         New_Shape->Shape_Colour->Alpha = 0.0;
         New_Shape -> Coeff   = 10.0;
         New_Shape -> Radius  = 0.35;
         New_Shape -> Falloff = 0.35;
         return (New_Shape);
      }

      /* Allocate and initialize a quadric surface. */
      QUADRIC *Get_Quadric_Shape()
      {
         QUADRIC *New_Shape;

         if ((New_Shape = (QUADRIC *) malloc (sizeof (QUADRIC))) == NULL)
            Error ("Out of memory. Cannot allocate shape");

         Make_Vector (&(New_Shape -> Object_2_Terms), 1.0, 1.0, 1.0);
         Make_Vector (&(New_Shape -> Object_Mixed_Terms), 0.0, 0.0, 0.0);
         Make_Vector (&(New_Shape -> Object_Terms), 0.0, 0.0, 0.0);
         New_Shape -> Object_Constant = 1.0;
         New_Shape -> Object_VP_Constant = HUGE_VAL;
         New_Shape -> Constant_Cached = FALSE;
         New_Shape -> Non_Zero_Square_Term = FALSE;
         New_Shape -> Type = QUADRIC_TYPE;
         New_Shape -> Next_Object = NULL;
         New_Shape -> Methods = &Quadric_Methods;
         New_Shape -> Shape_Texture = NULL;
         New_Shape -> Shape_Colour = NULL;
         return (New_Shape);
      }
      /* Allocate and initialize a polynomial surface. */
      POLY *Get_Poly_Shape(order)
         int order;
      {
         POLY *New_Shape;
         int i;

         if ((New_Shape = (POLY *) malloc (sizeof (POLY))) == NULL)
            Error ("Out of memory. Cannot allocate shape");

         New_Shape->Type = POLY_TYPE;
         New_Shape->Next_Object = NULL;
         New_Shape->Methods = &Poly_Methods;
         New_Shape->Shape_Texture = NULL;
         New_Shape->Shape_Colour = NULL;
         New_Shape->Transform = NULL;
         New_Shape->Inverted = 0;
         New_Shape->Order = order;
         New_Shape->Sturm_Flag = 0;
         New_Shape->Coeffs = (DBL *)malloc(term_counts[order] * sizeof(DBL));
         if (New_Shape->Coeffs == NULL)
            Error("Out of memory. Cannot allocate coefficients for POLY");
         for (i=0;i<term_counts[order];i++)
            New_Shape->Coeffs[i] = 0.0;
         return (New_Shape);
      }

      /* Allocate and initialize a box. */
      BOX *Get_Box_Shape()
      {
         BOX *New_Shape;

         if ((New_Shape = (BOX *) malloc (sizeof (BOX))) == NULL)
            Error ("Out of memory. Cannot allocate shape");

         Make_Vector(&(New_Shape->bounds[0]), -1.0, -1.0, -1.0);
         Make_Vector(&(New_Shape->bounds[1]),  1.0,  1.0,  1.0);
         New_Shape -> Transform = NULL;
         New_Shape -> Type = BOX_TYPE;
         New_Shape -> Next_Object = NULL;
         New_Shape -> Methods = &Box_Methods;
         New_Shape -> Inverted = FALSE;
         New_Shape -> Shape_Texture = NULL;
         New_Shape -> Shape_Colour = NULL;
         return (New_Shape);
      }

      /* Allocate a blob. */
      BLOB *Get_Blob_Shape()
      {
         BLOB *New_Shape;

         if ((New_Shape = (BLOB *) malloc (sizeof (BLOB))) == NULL)
            Error ("Out of memory. Cannot allocate shape");

         New_Shape -> Transform = NULL;
         New_Shape -> Type = BLOB_TYPE;
         New_Shape -> Next_Object = NULL;
         New_Shape -> Methods = &Blob_Methods;
         New_Shape -> Inverted = FALSE;
         New_Shape -> Shape_Texture = NULL;
         New_Shape -> Shape_Colour = NULL;
         return (New_Shape);
      }

      /* Allocate and initialize a bicubic patch surface. */
      BICUBIC_PATCH *Get_Bicubic_Patch_Shape()
      {
         BICUBIC_PATCH *New_Shape;

         if ((New_Shape = (BICUBIC_PATCH *) malloc (sizeof (BICUBIC_PATCH))) == NULL)
            Error ("Out of memory. Cannot allocate shape");

         New_Shape->Type = BICUBIC_PATCH_TYPE;
         New_Shape->Next_Object = NULL;
         New_Shape->Methods = &Bicubic_Patch_Methods;
         New_Shape->Shape_Texture = NULL;
         New_Shape->Shape_Colour = NULL;
         New_Shape->U_Steps = 0;
         New_Shape->V_Steps = 0;
         New_Shape->Intersection_Count = 0;
         New_Shape->Interpolated_Grid = (VECTOR **)NULL;
         New_Shape->Interpolated_Normals = (VECTOR **)NULL;
         New_Shape->Smooth_Normals = (VECTOR **)NULL;
         New_Shape->Interpolated_D = (DBL **)NULL;
         return (New_Shape);
      }       

      /* Allocate and intialize a Height Field */
      HEIGHT_FIELD *Get_Height_Field_Shape()
      {
         HEIGHT_FIELD *New_Shape;

         if((New_Shape = (HEIGHT_FIELD *) malloc (sizeof(HEIGHT_FIELD))) == NULL)
            Error ("Out of memory. Cannot allocate shape");
         New_Shape -> bounding_box = Get_Box_Shape();
         New_Shape -> Map = NULL;
         New_Shape -> Transformation = Get_Transformation();
         New_Shape -> Type = HEIGHT_FIELD_TYPE;
         New_Shape -> Next_Object = NULL;
         New_Shape -> Methods = &Height_Field_Methods;
         New_Shape -> Shape_Texture = NULL;
         New_Shape -> Shape_Colour = NULL;
         return(New_Shape);
      }


      /* Allocate and initialize a plane. */
      PLANE *Get_Plane_Shape()
      {
         PLANE *New_Shape;

         if ((New_Shape = (PLANE *) malloc (sizeof (PLANE))) == NULL)
            Error ("Out of memory. Cannot allocate shape");

         Make_Vector (&(New_Shape -> Normal_Vector), 0.0, 1.0, 0.0);
         New_Shape->Distance = 0.0;
         New_Shape -> Type = PLANE_TYPE;
         New_Shape -> Next_Object = NULL;
         New_Shape -> Methods = &Plane_Methods;
         New_Shape -> VPCached = 0;
         New_Shape -> Shape_Texture = NULL;
         New_Shape -> Shape_Colour = NULL;
         return (New_Shape);
      }

      /* Allocate and initialize a triangle. */
      TRIANGLE *Get_Triangle_Shape()
      {
         TRIANGLE *New_Shape;

         if ((New_Shape = (TRIANGLE *) malloc (sizeof (TRIANGLE))) == NULL)
            Error ("Out of memory. Cannot allocate shape");

         Make_Vector (&(New_Shape -> Normal_Vector), 0.0, 1.0, 0.0);
         Make_Vector (&(New_Shape -> P1), 0.0, 0.0, 0.0);
         Make_Vector (&(New_Shape -> P2), 1.0, 0.0, 0.0);
         Make_Vector (&(New_Shape -> P3), 0.0, 1.0, 0.0);
         New_Shape->Distance = 0.0;
         New_Shape->Inverted = FALSE;
         New_Shape -> Type = TRIANGLE_TYPE;
         New_Shape -> Next_Object = NULL;
         New_Shape -> Methods = &Triangle_Methods;
         New_Shape -> VPCached = FALSE;
         New_Shape -> Shape_Texture = NULL;
         New_Shape -> Shape_Colour = NULL;
         New_Shape -> Degenerate_Flag = FALSE;
         return (New_Shape);
      }

      /* Allocate and initialize a smooth triangle. */
      SMOOTH_TRIANGLE *Get_Smooth_Triangle_Shape()
      {
         SMOOTH_TRIANGLE *New_Shape;

         if ((New_Shape = (SMOOTH_TRIANGLE *) malloc (sizeof (SMOOTH_TRIANGLE))) == NULL)
            Error ("Out of memory. Cannot allocate shape");

         Make_Vector (&(New_Shape -> Normal_Vector), 0.0, 1.0, 0.0);
         Make_Vector (&(New_Shape -> P1), 0.0, 0.0, 0.0);
         Make_Vector (&(New_Shape -> P2), 1.0, 0.0, 0.0);
         Make_Vector (&(New_Shape -> P3), 0.0, 1.0, 0.0);
         Make_Vector (&(New_Shape -> N1), 0.0, 1.0, 0.0);
         Make_Vector (&(New_Shape -> N2), 0.0, 1.0, 0.0);
         Make_Vector (&(New_Shape -> N3), 0.0, 1.0, 0.0);
         New_Shape->Distance = 0.0;
         New_Shape -> Type = SMOOTH_TRIANGLE_TYPE;
         New_Shape->Inverted = FALSE;
         New_Shape -> Next_Object = NULL;
         New_Shape -> Methods = &Smooth_Triangle_Methods;
         New_Shape -> VPCached = 0;
         New_Shape -> Shape_Texture = NULL;
         New_Shape -> Shape_Colour = NULL;
         New_Shape -> Degenerate_Flag = FALSE;
         return (New_Shape);
      }

      CSG_SHAPE *Get_CSG_Shape()
      {
         CSG_SHAPE *New_Shape;

         if ((New_Shape = (CSG_SHAPE *) malloc (sizeof (CSG_SHAPE))) == NULL)
            Error ("Out of memory. Cannot allocate shape");

         New_Shape -> Parent_Object = NULL;
         New_Shape -> Next_Object = NULL;
         New_Shape -> Shapes = NULL;
         return (New_Shape);
      }

      CSG_SHAPE *Get_CSG_Union()
      {
         CSG_SHAPE *New_Shape;

         New_Shape = Get_CSG_Shape();
         New_Shape -> Methods = &CSG_Union_Methods;
         New_Shape -> Type = CSG_UNION_TYPE;
         return (New_Shape);
      }

      CSG_SHAPE *Get_CSG_Intersection()
      {
         CSG_SHAPE *New_Shape;

         New_Shape = Get_CSG_Shape();
         New_Shape -> Methods = &CSG_Intersection_Methods;
         New_Shape -> Type = CSG_INTERSECTION_TYPE;
         return (New_Shape);
      }

      OBJECT *Get_Object ()
      {
         OBJECT *New_Object;

         if ((New_Object = (OBJECT *) malloc (sizeof (OBJECT))) == NULL)
            Error ("Out of memory. Cannot allocate object");

         New_Object -> Next_Object = NULL;
         /*  New_Object -> Next_Light_Source = NULL;*/
         New_Object -> Shape = NULL;
         New_Object -> Bounding_Shapes = NULL;
         New_Object -> Clipping_Shapes = NULL;
         New_Object -> Object_Texture = Default_Texture;

         New_Object->Object_Colour = NULL;

         New_Object -> No_Shadow_Flag = FALSE;
         New_Object -> Type = OBJECT_TYPE;
         New_Object -> Methods = &Basic_Object_Methods;
         return (New_Object);
      }

      TEXTURE *Get_Texture ()
      {
         TEXTURE *New_Texture;

         if ((New_Texture = (TEXTURE *) malloc (sizeof (TEXTURE))) == NULL)
            Error ("Out of memory. Cannot allocate object");

         New_Texture -> Next_Texture = NULL;
         New_Texture -> Next_Material = NULL;
         New_Texture -> Number_Of_Materials = 0;
         New_Texture -> Object_Reflection = 0.0;
         New_Texture -> Object_Ambient = 0.1;
         New_Texture -> Object_Diffuse = 0.6;
         New_Texture -> Object_Brilliance = 1.0;
         New_Texture -> Object_Specular = 0.0;
         New_Texture -> Object_Roughness = 0.05;
         New_Texture -> Object_Phong = 0.0;
         New_Texture -> Object_PhongSize = 40;

         New_Texture -> Texture_Randomness= 0.0;
         New_Texture -> Bump_Amount = 0.0;
         New_Texture -> Phase = 0.0;
         New_Texture -> Frequency = 1.0;
         New_Texture -> Texture_Number = NO_TEXTURE;
         New_Texture -> Texture_Transformation = NULL;
         New_Texture -> Bump_Number = NO_BUMPS;
         New_Texture -> Turbulence = 0.0;
         New_Texture -> Colour_Map = NULL;
         New_Texture -> Once_Flag = FALSE;
         New_Texture -> Metallic_Flag = FALSE;
         New_Texture -> Octaves = 6;         /* dmf, for turbulence functs */
         New_Texture -> Mortar = 0.2;        /* rha, for brick texture */

         New_Texture -> Constant_Flag = TRUE;
         New_Texture -> Colour1 = NULL;
         New_Texture -> Colour2 = NULL;
         Make_Vector (&New_Texture->Texture_Gradient, 0.0, 0.0, 0.0);

         New_Texture -> Object_Index_Of_Refraction = 1.0;
         New_Texture -> Object_Transmit = 0.0;
         New_Texture -> Object_Refraction = 0.0;
         return (New_Texture);
      }


      VIEWPOINT *Get_Viewpoint ()
      {
         VIEWPOINT *New_Viewpoint;

         if ((New_Viewpoint = (VIEWPOINT *)malloc (sizeof (VIEWPOINT)))
            == NULL)
            Error ("Out of memory. Cannot allocate viewpoint");

         Init_Viewpoint (New_Viewpoint);
         return (New_Viewpoint);
      }

      COLOUR *Get_Colour ()
      {
         COLOUR *New_Colour;

         if ((New_Colour = (COLOUR *) malloc (sizeof (COLOUR))) == NULL)
            Error ("Out of memory. Cannot allocate colour");

         Make_Colour (New_Colour, 0.0, 0.0, 0.0);
         return (New_Colour);
      }

      VECTOR *Get_Vector ()
      {
         VECTOR *New_Vector;

         if ((New_Vector = (VECTOR *) malloc (sizeof (VECTOR))) == NULL)
            Error ("Out of memory. Cannot allocate vector");

         New_Vector -> x = 0.0;
         New_Vector -> y = 0.0;
         New_Vector -> z = 0.0;
         return (New_Vector);
      }

      DBL *Get_Float ()
      {
         DBL *New_Float;

         if ((New_Float = (DBL *) malloc (sizeof (DBL))) == NULL)
            Error ("Out of memory. Cannot allocate float");

         *New_Float = 0.0;
         return (New_Float);
      }

      TRANSFORMATION *Get_Transformation()
      {
         TRANSFORMATION *New_Transformation;

         if ((New_Transformation =
            (TRANSFORMATION *) malloc (sizeof (TRANSFORMATION))) == NULL)
            Error ("Out of memory. Cannot allocate transformation");

         MIdentity ((MATRIX *) &(New_Transformation -> matrix[0][0]));
         MIdentity ((MATRIX *) &(New_Transformation -> inverse[0][0]));
         return (New_Transformation);
      }

      /* Parse a float.  Doesn't handle exponentiation. */
      DBL Parse_Float ()
      {
         DBL Local_Float = 0.0;
         CONSTANT Constant_Id;
         register int Negative, Sign_Parsed;

         Negative = FALSE;
         Sign_Parsed = FALSE;

         EXPECT
         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == FLOAT_CONSTANT)
               {
                  Local_Float = *((DBL *) Constants[(int)Constant_Id].Constant_Data);
                  if (Negative)
                     Local_Float *= -1.0;
               }
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         CASE (PLUS_TOKEN)
            if (Sign_Parsed)
               Parse_Error (FLOAT_TOKEN);
         Sign_Parsed = TRUE;
         END_CASE

         CASE (DASH_TOKEN)
            if (Sign_Parsed)
               Parse_Error (FLOAT_TOKEN);
         Negative = TRUE;
         Sign_Parsed = TRUE;
         END_CASE

         CASE (FLOAT_TOKEN)
            Local_Float = Token.Token_Float;
         if (Negative)
            Local_Float *= -1.0;
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (FLOAT_TOKEN);
         END_CASE
         END_EXPECT

         return (Local_Float);
      }

      void Parse_Vector (Given_Vector)
         VECTOR *Given_Vector;
      {
         CONSTANT Constant_Id;

         EXPECT
         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == VECTOR_CONSTANT)
                  *Given_Vector = *((VECTOR *) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         CASE (LEFT_ANGLE_TOKEN)
            (Given_Vector -> x) = Parse_Float();
         (Given_Vector -> y) = Parse_Float();
         (Given_Vector -> z) = Parse_Float();
         GET (RIGHT_ANGLE_TOKEN);
         EXIT
         END_CASE

         OTHERWISE 
         Parse_Error (LEFT_ANGLE_TOKEN);
         END_CASE
         END_EXPECT
      }

      void Parse_Coeffs(order, Given_Coeffs)
         int order;
      DBL *Given_Coeffs;
      {
         int i;

         EXPECT
         CASE (LEFT_ANGLE_TOKEN)
            for (i = 0; i < term_counts[order]; i++)
               Given_Coeffs[i] = Parse_Float();
         GET (RIGHT_ANGLE_TOKEN);
         EXIT
         END_CASE

         OTHERWISE 
         Parse_Error (LEFT_ANGLE_TOKEN);
         END_CASE
         END_EXPECT
      }

      void Parse_Colour (Given_Colour)
         COLOUR *Given_Colour;
      {
         CONSTANT Constant_Id;
         Make_Colour (Given_Colour, 0.0, 0.0, 0.0);
         EXPECT
         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == COLOUR_CONSTANT)
                  *Given_Colour = *((COLOUR *) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         END_CASE

         CASE (RED_TOKEN)
            (Given_Colour -> Red) = Parse_Float();
         END_CASE

         CASE (GREEN_TOKEN)
            (Given_Colour -> Green) = Parse_Float();
         END_CASE

         CASE (BLUE_TOKEN)
            (Given_Colour -> Blue) = Parse_Float();
         END_CASE

         CASE (ALPHA_TOKEN)
            (Given_Colour -> Alpha) = Parse_Float();
         END_CASE

         OTHERWISE
         UNGET
         EXIT
         END_CASE
         END_EXPECT
      }

      COLOUR_MAP *Parse_Colour_Map ()
      {
#define MAX_ENTRIES 20
         COLOUR_MAP *New_Colour_Map;


         register int i, j;

         if ((New_Colour_Map = (COLOUR_MAP *) malloc (sizeof (COLOUR_MAP))) == NULL)
            Error ("Not enough memory for colour map.");


         if (Construction_Map == NULL)
            if ((Construction_Map = (COLOUR_MAP_ENTRY *)
               malloc(MAX_ENTRIES * sizeof (COLOUR_MAP_ENTRY))) == NULL)
               Error ("Not enough memory for colour map.");

         i = 0;
         New_Colour_Map->Transparency_Flag = FALSE;
         GET(LEFT_CURLY_TOKEN);
         EXPECT
         CASE (LEFT_SQUARE_TOKEN)
            Construction_Map [i].start = Parse_Float();
         Construction_Map [i].end = Parse_Float();

         GET (COLOUR_TOKEN);
         Parse_Colour (&(Construction_Map[i].Start_Colour));
         if (Construction_Map[i].Start_Colour.Alpha != 0.0)
            New_Colour_Map->Transparency_Flag = TRUE;

         GET (COLOUR_TOKEN);
         Parse_Colour (&(Construction_Map[i].End_Colour));
         if (Construction_Map[i].End_Colour.Alpha != 0.0)
            New_Colour_Map->Transparency_Flag = TRUE;

         i++;
         if (i > MAX_ENTRIES)
            Error ("Colour_Map too long.");
         GET (RIGHT_SQUARE_TOKEN);
         END_CASE

         CASE (RIGHT_CURLY_TOKEN)
            New_Colour_Map -> Number_Of_Entries = i;

         if ((New_Colour_Map -> Colour_Map_Entries = (COLOUR_MAP_ENTRY *)
            malloc(sizeof(COLOUR_MAP_ENTRY) * i)) == NULL)
            Error ("Not enough memory for colour map.");

         for (j = 0 ; j < i ; j++)
            New_Colour_Map->Colour_Map_Entries[j] = Construction_Map[j];

         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         return (New_Colour_Map);
      }

      TEXTURE *Copy_Texture (Texture)
         TEXTURE *Texture;
      {
         TEXTURE *New_Texture, *Local_Texture, *First_Texture, *Previous_Texture;

         Previous_Texture = First_Texture = NULL;

         for (Local_Texture = Texture ; Local_Texture != NULL ; Local_Texture = Local_Texture->Next_Texture)  {
            New_Texture = Get_Texture();
            *New_Texture = *Local_Texture;

            if (First_Texture == NULL)
               First_Texture = New_Texture;

            if (Previous_Texture != NULL)
               Previous_Texture->Next_Texture = New_Texture;

            if (New_Texture->Texture_Transformation) {
               if ((New_Texture->Texture_Transformation = (TRANSFORMATION *) malloc (sizeof (TRANSFORMATION))) == NULL)
                  Error("Out of memory. Cannot allocate texture transformation");
               *New_Texture->Texture_Transformation = *Local_Texture->Texture_Transformation;
            }
            New_Texture->Constant_Flag = FALSE;
            Previous_Texture = New_Texture;
         }            
         return (First_Texture);
      }

      TEXTURE *Parse_Texture ()
      {
         VECTOR Local_Vector;
         TRANSFORMATION Local_Transformation;
         CONSTANT Constant_Id;
         TEXTURE *Texture, *Local_Texture, *First_Texture;
         TEXTURE *temp_texture;
         int reg;

         Texture = Default_Texture;

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == TEXTURE_CONSTANT) {
                  Texture = ((TEXTURE *) Constants[(int)Constant_Id].Constant_Data);
               }
               else
                  Type_Error ();
            else
               Undeclared ();
         END_CASE

         CASE (FLOAT_TOKEN)
            UNGET
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture(Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Randomness = Parse_Float();
         END_CASE

         CASE (ONCE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture(Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture->Once_Flag = TRUE;
         END_CASE

         CASE (TURBULENCE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture(Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Turbulence = Parse_Float();
         END_CASE

         CASE (OCTAVES_TOKEN)                  /* dmf 02/05 for turb */
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture(Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture->Octaves = (int) Parse_Float();
         if(Texture->Octaves < 1)
            Texture->Octaves = 6;
         if(Texture->Octaves > 10)  /* Avoid DOMAIN errors */
            Texture->Octaves = 10;
         END_CASE

         CASE (BOZO_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = BOZO_TEXTURE;
         END_CASE

         CASE (MORTAR_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture->Mortar = Parse_Float();
         if (Texture->Mortar < 0)
            Texture->Mortar = 0.2;
         END_CASE

         CASE (BRICK_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = BRICK_TEXTURE;
         EXPECT
         CASE (COLOUR_TOKEN)
            Texture->Colour1 = Get_Colour();
         Texture->Colour2 = Get_Colour();
         Parse_Colour (Texture -> Colour1);
         GET (COLOUR_TOKEN);
         Parse_Colour (Texture -> Colour2);
         END_CASE

         OTHERWISE
         UNGET
         EXIT
         END_CASE
         END_EXPECT
         END_CASE

         CASE (CHECKER_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = CHECKER_TEXTURE;
         EXPECT
         CASE (COLOUR_TOKEN)
            Texture->Colour1 = Get_Colour();
         Texture->Colour2 = Get_Colour();
         Parse_Colour (Texture -> Colour1);
         GET (COLOUR_TOKEN);
         Parse_Colour (Texture -> Colour2);
         END_CASE

         OTHERWISE
         UNGET
         EXIT
         END_CASE
         END_EXPECT
         END_CASE

         CASE (CHECKER_TEXTURE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = CHECKER_TEXTURE_TEXTURE;

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         {
            for (temp_texture = Local_Texture ;
               		    temp_texture->Next_Texture != NULL ;
               		    temp_texture = temp_texture->Next_Texture)
            {}

            temp_texture->Next_Texture = (TEXTURE *) Texture->Colour1;
            Texture->Colour1 = (COLOUR *) Local_Texture;
         }
         END_CASE
         OTHERWISE
         UNGET
         EXIT
         END_CASE
         END_EXPECT

         GET (TILE2_TOKEN);
         EXPECT
         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         {
            for (temp_texture = Local_Texture ;
               		    temp_texture->Next_Texture != NULL ;
               		    temp_texture = temp_texture->Next_Texture)
            {}

            temp_texture->Next_Texture = (TEXTURE *) Texture->Colour2;
            Texture->Colour2 = (COLOUR *) Local_Texture;
         }
         END_CASE
         OTHERWISE
         UNGET
         EXIT
         END_CASE
         END_EXPECT
         GET (RIGHT_CURLY_TOKEN);
         END_CASE

         CASE (MARBLE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = MARBLE_TEXTURE;
         END_CASE

         CASE (WOOD_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = WOOD_TEXTURE;
         END_CASE

         CASE (SPOTTED_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = SPOTTED_TEXTURE;
         END_CASE

         CASE (AGATE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = AGATE_TEXTURE;
         END_CASE

         CASE (GRANITE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = GRANITE_TEXTURE;
         END_CASE

         CASE (GRADIENT_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = GRADIENT_TEXTURE;
         Parse_Vector (&(Texture -> Texture_Gradient));
         END_CASE

         CASE (AMBIENT_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         (Texture -> Object_Ambient) = Parse_Float ();
         END_CASE

         CASE (BRILLIANCE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         (Texture -> Object_Brilliance) = Parse_Float ();
         END_CASE

         CASE (ROUGHNESS_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         (Texture -> Object_Roughness) = Parse_Float ();
         /* No training wheels */
         /* if (Texture -> Object_Roughness > 1.0)
	    Texture -> Object_Roughness = 1.0;
	 if (Texture -> Object_Roughness < 0.001)
	    Texture -> Object_Roughness = 0.001; */
         END_CASE

         CASE (PHONGSIZE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         (Texture -> Object_PhongSize) = Parse_Float ();
         /* No training wheels */
         /*if (Texture -> Object_PhongSize < 1.0)
	    Texture -> Object_PhongSize = 1.0;
	 if (Texture -> Object_PhongSize > 100)
	    Texture -> Object_PhongSize = 100; */
         END_CASE

         CASE (DIFFUSE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         (Texture -> Object_Diffuse) = Parse_Float ();
         END_CASE

         CASE (SPECULAR_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         (Texture -> Object_Specular) = Parse_Float ();
         END_CASE

         CASE (PHONG_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         (Texture -> Object_Phong) = Parse_Float ();
         END_CASE

         CASE (METALLIC_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Metallic_Flag = TRUE;
         END_CASE

         CASE (IOR_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         (Texture -> Object_Index_Of_Refraction) = Parse_Float ();
         END_CASE

         CASE (REFRACTION_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         (Texture -> Object_Refraction) = Parse_Float ();
         END_CASE

         CASE (TRANSMIT_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         (Texture -> Object_Transmit) = Parse_Float ();
         END_CASE

         CASE (REFLECTION_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         (Texture -> Object_Reflection) = Parse_Float ();
         END_CASE

         CASE (IMAGEMAP_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture->Texture_Number = IMAGEMAP_TEXTURE;
         if ((Texture->Image = (IMAGE *)malloc(sizeof(IMAGE))) == NULL)
            Error("Out of memory. Cannot allocate imagemap texture");
         Make_Vector (&Texture->Image->Image_Gradient, 1.0, -1.0, 0.0);
         Texture->Image->Map_Type = PLANAR_MAP;  
         Texture->Image->Interpolation_Type = NO_INTERPOLATION;
         Texture->Image->Once_Flag = FALSE;
         Texture->Image->Use_Colour_Flag= TRUE;

         GET(LEFT_CURLY_TOKEN);   

         EXPECT
         CASE3 (DASH_TOKEN, PLUS_TOKEN, FLOAT_TOKEN)
            UNGET
            (Texture->Image->Map_Type) = (int) Parse_Float ();
         END_CASE

         CASE (LEFT_ANGLE_TOKEN)
            UNGET
            Parse_Vector (&(Texture -> Image->Image_Gradient));
         END_CASE

         CASE (IFF_TOKEN)
            GET (STRING_TOKEN);
         Read_Iff_Image(Texture->Image, Token.Token_String);
         EXIT
         END_CASE

         CASE (GIF_TOKEN)
            GET (STRING_TOKEN);
         Read_Gif_Image(Texture->Image, Token.Token_String);
         EXIT
         END_CASE

         CASE (TGA_TOKEN)
            GET (STRING_TOKEN);
         Read_Targa_Image(Texture->Image, Token.Token_String);
         EXIT
         END_CASE

         CASE (DUMP_TOKEN)
            GET (STRING_TOKEN);
         Read_Dump_Image(Texture->Image, Token.Token_String);
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (GIF_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (ONCE_TOKEN)
            Texture->Image->Once_Flag=TRUE;
         END_CASE

         CASE (INTERPOLATE_TOKEN)
            Texture->Image->Interpolation_Type = (int)Parse_Float();
         END_CASE

         CASE (MAPTYPE_TOKEN)
            (Texture->Image->Map_Type) = (int) Parse_Float ();
         END_CASE

         CASE (USE_COLOUR_TOKEN)
            Texture ->Image->Use_Colour_Flag = TRUE;
         END_CASE  

         CASE (USE_INDEX_TOKEN)
            Texture ->Image->Use_Colour_Flag = FALSE;
         END_CASE  

         CASE (ALPHA_TOKEN)
            EXPECT
            CASE (FLOAT_TOKEN)
               reg = (int)(Token.Token_Float + 0.01);
         if (Texture->Image->Colour_Map == NULL)
            Error ("Can't apply ALPHA to a non colour-mapped image\n");

         if ((reg < 0) || (reg >= Texture->Image->Colour_Map_Size))
            Error ("ALPHA colour register value out of range.\n");

         Texture->Image->Colour_Map[reg].Alpha = (unsigned short) (255.0 * Parse_Float());
         EXIT
         END_CASE

         CASE (ALL_TOKEN)
         {
            DBL alpha;
            alpha = Parse_Float();

            for (reg = 0 ; reg < Texture->Image->Colour_Map_Size ; reg++)
               Texture->Image->Colour_Map[reg].Alpha = (unsigned short) (alpha *255.0);
            EXIT
         }

         END_CASE
         END_EXPECT
         END_CASE

         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            OTHERWISE
            Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT
         END_CASE

         CASE (WAVES_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Bump_Number = WAVES;
         Texture -> Bump_Amount = Parse_Float ();
         EXPECT
         CASE (PHASE_TOKEN)
            Texture -> Phase = Parse_Float();
         EXIT
         END_CASE

         OTHERWISE
         UNGET
         EXIT
         END_CASE
         END_EXPECT
         END_CASE

         CASE (FREQUENCY_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Frequency = Parse_Float();
         END_CASE

         CASE (PHASE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Phase = Parse_Float();
         END_CASE

         CASE (RIPPLES_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Bump_Number = RIPPLES;
         Texture -> Bump_Amount = Parse_Float ();
         END_CASE

         CASE (WRINKLES_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Bump_Number = WRINKLES;
         Texture -> Bump_Amount = Parse_Float ();
         END_CASE

         CASE (BUMPS_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Bump_Number = BUMPS;
         Texture -> Bump_Amount = Parse_Float ();
         END_CASE

         CASE (DENTS_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Bump_Number = DENTS;
         Texture -> Bump_Amount = Parse_Float ();
         END_CASE

         CASE (TRANSLATE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Parse_Vector (&Local_Vector);
         Translate_Texture (&Texture, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Parse_Vector (&Local_Vector);
         Rotate_Texture (&Texture, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Parse_Vector (&Local_Vector);
         Scale_Texture (&Texture, &Local_Vector);
         END_CASE

         CASE (COLOUR_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture->Colour1 = Get_Colour();
         Parse_Colour (Texture -> Colour1);
         Texture -> Texture_Number = COLOUR_TEXTURE;
         END_CASE

         CASE (COLOUR_MAP_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Colour_Map = Parse_Colour_Map();
         END_CASE

         CASE (ONION_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = ONION_TEXTURE;
         END_CASE

         CASE (LEOPARD_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = LEOPARD_TEXTURE;
         END_CASE



         /* New Texture Parsing - Cdw */
         CASE (PAINTED1_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = PAINTED1_TEXTURE;
         END_CASE

         CASE (PAINTED2_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = PAINTED2_TEXTURE;
         END_CASE

         CASE (PAINTED3_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = PAINTED3_TEXTURE;
         END_CASE

         CASE (BUMPY1_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Bump_Number = BUMPY1;
         Texture -> Bump_Amount = Parse_Float ();
         END_CASE

         CASE (BUMPY2_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Bump_Number = BUMPY2;
         Texture -> Bump_Amount = Parse_Float ();
         END_CASE

         CASE (BUMPY3_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Bump_Number = BUMPY3;
         Texture -> Bump_Amount = Parse_Float ();
         END_CASE


         CASE (BUMPMAP_TOKEN)
            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Bump_Number = BUMPMAP;
         if ((Texture->Bump_Image = (IMAGE *)malloc(sizeof(IMAGE))) == NULL)
            Error("Out of memory. Cannot allocate bumpmap texture");
         Make_Vector (&Texture->Bump_Image->Image_Gradient, 1.0, -1.0, 0.0);
         Texture->Bump_Image->Map_Type = PLANAR_MAP;  
         Texture->Bump_Image->Interpolation_Type = NO_INTERPOLATION;
         Texture->Bump_Image->Once_Flag = FALSE;
         Texture->Bump_Image->Use_Colour_Flag = TRUE;

         GET(LEFT_CURLY_TOKEN);    

         EXPECT
         CASE3 (DASH_TOKEN, PLUS_TOKEN, FLOAT_TOKEN)
            UNGET
            (Texture->Bump_Image->Map_Type) = (int) Parse_Float ();
         END_CASE            

         CASE (LEFT_ANGLE_TOKEN)
            UNGET
            Parse_Vector (&(Texture ->Bump_Image->Image_Gradient));
         END_CASE

         CASE (IFF_TOKEN)
            GET (STRING_TOKEN);
         Read_Iff_Image(Texture->Bump_Image, Token.Token_String);
         EXIT
         END_CASE

         CASE (GIF_TOKEN)
            GET (STRING_TOKEN);
         Read_Gif_Image(Texture->Bump_Image, Token.Token_String);
         EXIT
         END_CASE

         CASE (TGA_TOKEN)
            GET (STRING_TOKEN);
         Read_Targa_Image(Texture->Bump_Image, Token.Token_String);
         EXIT
         END_CASE

         CASE (DUMP_TOKEN)
            GET (STRING_TOKEN);
         Read_Dump_Image(Texture->Bump_Image, Token.Token_String);
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (GIF_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (ONCE_TOKEN)
            Texture->Bump_Image->Once_Flag=TRUE;
         END_CASE          

         CASE (MAPTYPE_TOKEN)
            (Texture->Bump_Image->Map_Type) = (int) Parse_Float ();
         END_CASE

         CASE (INTERPOLATE_TOKEN)
            Texture->Bump_Image->Interpolation_Type = (int)Parse_Float();
         END_CASE

         CASE (BUMPSIZE_TOKEN)
            Texture -> Bump_Amount = Parse_Float ();
         END_CASE  

         CASE (USE_COLOUR_TOKEN)
            Texture ->Bump_Image->Use_Colour_Flag = TRUE;
         END_CASE  
         CASE (USE_INDEX_TOKEN)
            Texture ->Bump_Image->Use_Colour_Flag = FALSE;
         END_CASE  

         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            OTHERWISE
            Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT
         END_CASE


         CASE (MATERIAL_MAP_TOKEN)

            if (Texture->Constant_Flag) {
               Texture = Copy_Texture (Texture);
               Texture->Constant_Flag = FALSE;
            }
         Texture -> Texture_Number = MATERIAL_MAP_TEXTURE;
         if ((Texture->Material_Image=(IMAGE *)malloc(sizeof(IMAGE))) == NULL)
            Error("Out of memory. Cannot allocate material map texture");
         Make_Vector (&Texture->Texture_Gradient, 1.0, -1.0, 0.0);
         Texture->Material_Image->Map_Type = PLANAR_MAP;  
         Texture->Material_Image->Interpolation_Type = NO_INTERPOLATION;
         Texture->Material_Image->Once_Flag = FALSE;
         Texture->Material_Image->Use_Colour_Flag = FALSE; 

         GET(LEFT_CURLY_TOKEN);    

         EXPECT
         CASE3 (DASH_TOKEN, PLUS_TOKEN, FLOAT_TOKEN)
            UNGET
            (Texture->Image->Map_Type) = (int) Parse_Float ();
         END_CASE

         CASE (LEFT_ANGLE_TOKEN)
            UNGET
            Parse_Vector (&(Texture ->Material_Image->Image_Gradient));
         END_CASE

         CASE (IFF_TOKEN)
            GET (STRING_TOKEN);
         Read_Iff_Image(Texture->Material_Image, Token.Token_String);
         EXIT
         END_CASE

         CASE (GIF_TOKEN)
            GET (STRING_TOKEN);
         Read_Gif_Image(Texture->Material_Image, Token.Token_String);
         EXIT
         END_CASE

         CASE (TGA_TOKEN)
            GET (STRING_TOKEN);
         Read_Targa_Image(Texture->Material_Image, Token.Token_String);
         EXIT
         END_CASE

         CASE (DUMP_TOKEN)
            GET (STRING_TOKEN);
         Read_Dump_Image(Texture->Material_Image, Token.Token_String);
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (GIF_TOKEN);
         END_CASE
         END_EXPECT

         /* remember where the First_Texture is */
         First_Texture = Texture; 

         EXPECT

         CASE (MAPTYPE_TOKEN)
            (Texture->Material_Image->Map_Type) = (int) Parse_Float ();
         END_CASE

         CASE (INTERPOLATE_TOKEN)
            Texture->Material_Image->Interpolation_Type=(int)Parse_Float();
         END_CASE

         CASE (ONCE_TOKEN)
            Texture->Material_Image->Once_Flag=TRUE;
         END_CASE          


         CASE (TEXTURE_TOKEN) {
            Texture->Next_Material = Parse_Texture (); 
            First_Texture->Number_Of_Materials++;
            Texture = Texture->Next_Material;
         }

         END_CASE

         CASE (RIGHT_CURLY_TOKEN){
            Texture->Next_Material = NULL; 
            Texture = First_Texture; 
            EXIT
         }
         END_CASE  

         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT
         END_CASE

         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            OTHERWISE
            Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT
         return (Texture);
      }

      SHAPE *Parse_Light_Source ()
      {
         LIGHT_SHAPE *Local_Shape;
         VECTOR Local_Vector;
         CONSTANT Constant_Id;


         GET (LEFT_CURLY_TOKEN);

         EXPECT
         CASE (LEFT_ANGLE_TOKEN)
            UNGET
            Local_Shape = Get_Light_Source_Shape();
         Parse_Vector(&(Local_Shape->Center));
         Local_Shape->Shape_Colour = Get_Colour();
         Make_Colour(Local_Shape->Shape_Colour, 1.0, 1.0, 1.0);
         Local_Shape->Shape_Colour->Alpha = 0.0;
         GET (COLOUR_TOKEN);
         Parse_Colour (Local_Shape->Shape_Colour);
         EXIT
         END_CASE


         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == LIGHT_SOURCE_CONSTANT)
                  Local_Shape = (LIGHT_SHAPE *)Copy((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
               else 
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (LEFT_ANGLE_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (TRANSLATE_TOKEN)
               Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         /* Point that the spot is pointed at */
         CASE (POINT_AT_TOKEN)
            Parse_Vector(&(Local_Shape -> Points_At));
         END_CASE

         CASE (TIGHTNESS_TOKEN)
            Local_Shape -> Coeff = Parse_Float();
         END_CASE

         CASE (RADIUS_TOKEN)
            Local_Shape -> Radius = cos(Parse_Float() * M_PI / 180.0);
         END_CASE

         CASE (COLOUR_TOKEN)
            Parse_Colour (Local_Shape->Shape_Colour);
         END_CASE


         CASE (FALLOFF_TOKEN)
            Local_Shape -> Falloff = cos(Parse_Float() * M_PI / 180.0);
         END_CASE

         CASE (SPOTLIGHT_TOKEN)
            Local_Shape -> Type = SPOT_LIGHT_TYPE;
         END_CASE


         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE

         END_EXPECT
         /*  Link_Shapes (Local_Shape, &(Local_Shape -> Next_Light_Source),
	       &(Parsing_Frame_Ptr -> Light_Sources));
*/
         return ((SHAPE *) Local_Shape);
      }




      SHAPE *Parse_Sphere ()
      {
         SPHERE *Local_Shape;
         CONSTANT Constant_Id;
         VECTOR Local_Vector;
         TEXTURE *Local_Texture;
         TEXTURE *temp_texture;

         Local_Shape = NULL;

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (LEFT_ANGLE_TOKEN)
            UNGET
            Local_Shape = Get_Sphere_Shape();
         Parse_Vector(&(Local_Shape -> Center));
         Local_Shape -> Radius = Parse_Float();
         Local_Shape -> Radius_Squared = Local_Shape -> Radius * Local_Shape -> Radius;
         Local_Shape -> Inverse_Radius = 1.0 / Local_Shape -> Radius;
         EXIT
         END_CASE

         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == SPHERE_CONSTANT)
                  Local_Shape = (SPHERE *)Copy((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (LEFT_ANGLE_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (TRANSLATE_TOKEN)
               Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert ((OBJECT *) Local_Shape);
         END_CASE

         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         {
            for (temp_texture = Local_Texture ;
               	     temp_texture->Next_Texture != NULL ;
               	     temp_texture = temp_texture->Next_Texture)
            {}

            temp_texture->Next_Texture = Local_Shape->Shape_Texture;
            Local_Shape->Shape_Texture = Local_Texture;
         }
         END_CASE

         CASE (COLOUR_TOKEN)
            Local_Shape->Shape_Colour = Get_Colour();
         Parse_Colour (Local_Shape->Shape_Colour);
         END_CASE



         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         return ((SHAPE *) Local_Shape);
      }

      SHAPE *Parse_Plane ()
      {
         PLANE *Local_Shape;
         CONSTANT Constant_Id;
         VECTOR Local_Vector;
         TEXTURE *Local_Texture;
         TEXTURE *temp_texture;

         Local_Shape = NULL;

         GET(LEFT_CURLY_TOKEN);


         EXPECT
         CASE (LEFT_ANGLE_TOKEN)
            UNGET
            Local_Shape = Get_Plane_Shape();
         Parse_Vector(&(Local_Shape -> Normal_Vector));
         Local_Shape->Distance = Parse_Float();
         Local_Shape->Distance *= -1.0;
         EXIT
         END_CASE

         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == PLANE_CONSTANT)
                  Local_Shape = (PLANE *)Copy((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (LEFT_ANGLE_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (TRANSLATE_TOKEN)
               Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert ((OBJECT *) Local_Shape);
         END_CASE

         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         {
            for (temp_texture = Local_Texture ;
               	     temp_texture->Next_Texture != NULL ;
               	     temp_texture = temp_texture->Next_Texture)
            {}

            temp_texture->Next_Texture = Local_Shape->Shape_Texture;
            Local_Shape->Shape_Texture = Local_Texture;
         }
         END_CASE

         CASE (COLOUR_TOKEN)
            Local_Shape->Shape_Colour = Get_Colour();
         Parse_Colour (Local_Shape->Shape_Colour);
         END_CASE



         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         return ((SHAPE *) Local_Shape);
      }

      SHAPE *Parse_Height_Field ()
      {
         HEIGHT_FIELD *Local_Shape;
         CONSTANT Constant_Id;
         VECTOR Local_Vector;
         TEXTURE *Local_Texture;
         IMAGE *Image;
         int Image_Type;

         Local_Shape = NULL;

         GET(LEFT_CURLY_TOKEN);

         EXPECT  /* This should be modified to include other image types - CdW */
         CASE (GIF_TOKEN)
            Image_Type = GIF;
         Local_Shape = Get_Height_Field_Shape();
         if((Image = (IMAGE *) malloc(sizeof(IMAGE))) == NULL)
            Error("Out of memory. Cannot allocate space for Height Field (1st message).");
         GET(STRING_TOKEN); 
         Read_Gif_Image(Image,Token.Token_String);
         Local_Shape -> bounding_box -> bounds[0].x = 1.0;
         Local_Shape -> bounding_box -> bounds[0].y = 0.0;
         Local_Shape -> bounding_box -> bounds[0].z = 1.0;
         Local_Shape -> bounding_box -> bounds[1].x = Image -> width - 2.0;
         Local_Shape -> bounding_box -> bounds[1].y = 256.0;
         Local_Shape -> bounding_box -> bounds[1].z = Image -> height - 2.0;
         Make_Vector(&Local_Vector,1.0/(Image->width),1.0/256.0,1.0/(Image->height));
         Get_Scaling_Transformation(Local_Shape->Transformation,&Local_Vector);
         EXIT
         END_CASE

         CASE (POT_TOKEN)
            Image_Type = POT;
         Local_Shape = Get_Height_Field_Shape();
         if((Image = (IMAGE *) malloc(sizeof(IMAGE))) == NULL)
            Error("Out of memory. Cannot allocate space for Height Field (1st message).");
         GET(STRING_TOKEN); 
         Read_Gif_Image(Image,Token.Token_String);
         Local_Shape -> bounding_box -> bounds[0].x = 1.0;
         Local_Shape -> bounding_box -> bounds[0].y = 0.0;
         Local_Shape -> bounding_box -> bounds[0].z = 1.0;
         Local_Shape -> bounding_box -> bounds[1].x = Image -> width/2.0 - 2.0;
         Local_Shape -> bounding_box -> bounds[1].y = 256.0;
         Local_Shape -> bounding_box -> bounds[1].z = Image -> height - 2.0;
         Make_Vector(&Local_Vector,2.0/Image->width,1.0/256.0,1.0/Image->height);
         Get_Scaling_Transformation(Local_Shape->Transformation,&Local_Vector);
         EXIT
         END_CASE

         CASE (TGA_TOKEN)
            Image_Type = TGA;
         Local_Shape = Get_Height_Field_Shape();
         if((Image = (IMAGE *) malloc(sizeof(IMAGE))) == NULL)
            Error("Cannot allocate space for Height Field (1st message).");
         GET(STRING_TOKEN); 
         Read_Targa_Image(Image,Token.Token_String);
         Local_Shape -> bounding_box -> bounds[0].x = 1.0;
         Local_Shape -> bounding_box -> bounds[0].y = 0.0;
         Local_Shape -> bounding_box -> bounds[0].z = 1.0;
         Local_Shape -> bounding_box -> bounds[1].x = Image -> width - 2.0;
         Local_Shape -> bounding_box -> bounds[1].y = 256.0;
         Local_Shape -> bounding_box -> bounds[1].z = Image -> height - 2.0;
         Make_Vector(&Local_Vector,1.0/Image->width,1.0/256.0,1.0/Image->height);
         Get_Scaling_Transformation(Local_Shape->Transformation,&Local_Vector);  
         EXIT
         END_CASE

         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == HEIGHT_FIELD_CONSTANT)
                  Local_Shape = (HEIGHT_FIELD *)Copy((OBJECT *)Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (GIF_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (WATER_LEVEL_TOKEN)
               Local_Shape -> bounding_box -> bounds[0].y = Parse_Float(); 
         END_CASE

         CASE (TRANSLATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert ((OBJECT *) Local_Shape);
         END_CASE

         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         {
            TEXTURE *temp_texture;

            for (temp_texture = Local_Texture ;
               	     temp_texture->Next_Texture != NULL ;
               	     temp_texture = temp_texture->Next_Texture)
            {}

            temp_texture->Next_Texture = Local_Shape->Shape_Texture;
            Local_Shape->Shape_Texture = Local_Texture;
         }
         END_CASE

         CASE (COLOUR_TOKEN)
            Local_Shape->Shape_Colour = Get_Colour();
         Parse_Colour (Local_Shape->Shape_Colour);
         END_CASE


         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         Find_Hf_Min_Max(Local_Shape, Image, Image_Type);
         return ((SHAPE *) Local_Shape);
      }        


      SHAPE *Parse_Triangle ()
      {
         TRIANGLE *Local_Shape;
         CONSTANT Constant_Id;
         VECTOR Local_Vector;
         TEXTURE *Local_Texture;
         TEXTURE *temp_texture;

         Local_Shape = NULL;

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (LEFT_ANGLE_TOKEN)
            UNGET
            Local_Shape = Get_Triangle_Shape();
         Parse_Vector (&Local_Shape->P1);
         Parse_Vector (&Local_Shape->P2);
         Parse_Vector (&Local_Shape->P3);
         if (!Compute_Triangle (Local_Shape)) {
            fprintf (stderr, "Degenerate triangle on line %d.  Please remove.\n",
               Token.Token_Line_No);
            Degenerate_Triangles = TRUE;
         }
         EXIT
         END_CASE

         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == TRIANGLE_CONSTANT)
                  Local_Shape = (TRIANGLE *)Copy((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (LEFT_ANGLE_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (TRANSLATE_TOKEN)
               Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert ((OBJECT *) Local_Shape);
         END_CASE

         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         {
            for (temp_texture = Local_Texture ;
               	     temp_texture->Next_Texture != NULL ;
               	     temp_texture = temp_texture->Next_Texture)
            {}

            temp_texture->Next_Texture = Local_Shape->Shape_Texture;
            Local_Shape->Shape_Texture = Local_Texture;
         }
         END_CASE

         CASE (COLOUR_TOKEN)
            Local_Shape->Shape_Colour = Get_Colour();
         Parse_Colour (Local_Shape->Shape_Colour);
         END_CASE

         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         return ((SHAPE *) Local_Shape);
      }

      SHAPE *Parse_Smooth_Triangle ()
      {
         SMOOTH_TRIANGLE *Local_Shape;
         CONSTANT Constant_Id;
         VECTOR Local_Vector;
         TEXTURE *Local_Texture;
         TEXTURE *temp_texture;

         Local_Shape = NULL;

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (LEFT_ANGLE_TOKEN)
            UNGET
            Local_Shape = (SMOOTH_TRIANGLE *) Get_Smooth_Triangle_Shape();
         Parse_Vector (&Local_Shape->P1);
         Parse_Vector (&Local_Shape->N1);
         VNormalize (Local_Shape->N1, Local_Shape->N1)
            Parse_Vector (&Local_Shape->P2);
         Parse_Vector (&Local_Shape->N2);
         VNormalize (Local_Shape->N2, Local_Shape->N2)
            Parse_Vector (&Local_Shape->P3);
         Parse_Vector (&Local_Shape->N3);
         VNormalize (Local_Shape->N3, Local_Shape->N3)
            if (!Compute_Triangle ((TRIANGLE *) Local_Shape)) {
               fprintf (stderr, "Degenerate triangle on line %d.  Please remove.\n",
                  Token.Token_Line_No);
               Degenerate_Triangles = TRUE;
            }
         EXIT
         END_CASE

         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == SMOOTH_TRIANGLE_CONSTANT)
                  Local_Shape = (SMOOTH_TRIANGLE *)Copy((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (LEFT_ANGLE_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (TRANSLATE_TOKEN)
               Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert ((OBJECT *) Local_Shape);
         END_CASE

         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         {
            for (temp_texture = Local_Texture ;
               	     temp_texture->Next_Texture != NULL ;
               	     temp_texture = temp_texture->Next_Texture)
            {}

            temp_texture->Next_Texture = Local_Shape->Shape_Texture;
            Local_Shape->Shape_Texture = Local_Texture;
         }
         END_CASE

         CASE (COLOUR_TOKEN)
            Local_Shape->Shape_Colour = Get_Colour();
         Parse_Colour (Local_Shape->Shape_Colour);
         END_CASE

         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         return ((SHAPE *) Local_Shape);
      }

      SHAPE *Parse_Quadric ()
      {
         QUADRIC *Local_Shape;
         VECTOR Local_Vector;
         CONSTANT Constant_Id;
         TEXTURE *Local_Texture;
         TEXTURE *temp_texture;

         Local_Shape = NULL;

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (LEFT_ANGLE_TOKEN)
            UNGET
            Local_Shape = Get_Quadric_Shape();
         Parse_Vector(&(Local_Shape -> Object_2_Terms));
         Parse_Vector(&(Local_Shape -> Object_Mixed_Terms));
         Parse_Vector(&(Local_Shape -> Object_Terms));
         (Local_Shape -> Object_Constant) = Parse_Float();
         Local_Shape -> Non_Zero_Square_Term = 
         !((Local_Shape -> Object_2_Terms.x == 0.0)
            && (Local_Shape -> Object_2_Terms.y == 0.0)
            && (Local_Shape -> Object_2_Terms.z == 0.0)
            && (Local_Shape -> Object_Mixed_Terms.x == 0.0)
            && (Local_Shape -> Object_Mixed_Terms.y == 0.0)
            && (Local_Shape -> Object_Mixed_Terms.z == 0.0));
         EXIT
         END_CASE

         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == QUADRIC_CONSTANT)
                  Local_Shape = (QUADRIC *)Copy((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (LEFT_ANGLE_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (TRANSLATE_TOKEN)
               Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert ((OBJECT *) Local_Shape);
         END_CASE

         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         {
            for (temp_texture = Local_Texture ;
               	     temp_texture->Next_Texture != NULL ;
               	     temp_texture = temp_texture->Next_Texture)
            {}

            temp_texture->Next_Texture = Local_Shape->Shape_Texture;
            Local_Shape->Shape_Texture = Local_Texture;
         }
         END_CASE

         CASE (COLOUR_TOKEN)
            Local_Shape->Shape_Colour = Get_Colour();
         Parse_Colour (Local_Shape->Shape_Colour);
         END_CASE

         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         return ((SHAPE *) Local_Shape);
      }

      SHAPE *Parse_Poly (known_order)
         int known_order;
      {
         POLY *Local_Shape;
         VECTOR Local_Vector;
         CONSTANT Constant_Id;
         int order;
         TEXTURE *Local_Texture;

         if (known_order > 0)
            Local_Shape = Get_Poly_Shape(known_order);
         else
            Local_Shape = NULL;

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE3 (DASH_TOKEN, PLUS_TOKEN, FLOAT_TOKEN)
            UNGET
            if (Local_Shape != NULL)
               Error("The order of a polynomial may not be specified twice");
         order = (int)Parse_Float();
         if (order < 2 || order > MAX_ORDER)
            Error("Order of Poly is out of range");
         Local_Shape = Get_Poly_Shape(order);
         END_CASE

         CASE (LEFT_ANGLE_TOKEN)
            UNGET
            if (Local_Shape == NULL)
               printf("Need the order of the Poly");
         Parse_Coeffs(Local_Shape->Order, &(Local_Shape->Coeffs[0]));
         EXIT
         END_CASE

         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == POLY_CONSTANT)
                  Local_Shape = (POLY *)Copy((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (LEFT_ANGLE_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (STURM_TOKEN)
               Local_Shape->Sturm_Flag = 1;
         END_CASE

         CASE (TRANSLATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert ((OBJECT *) Local_Shape);
         END_CASE

         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         Link ((OBJECT *) Local_Texture,
            (OBJECT **) &Local_Texture->Next_Texture,
            (OBJECT **) &Local_Shape->Shape_Texture);
         END_CASE

         CASE (COLOUR_TOKEN)
            Local_Shape->Shape_Colour = Get_Colour();
         Parse_Colour (Local_Shape->Shape_Colour);
         END_CASE

         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         return ((SHAPE *) Local_Shape);
      }

      SHAPE *Parse_Bicubic_Patch ()
      {
         BICUBIC_PATCH *Local_Shape;
         VECTOR Local_Vector;
         CONSTANT Constant_Id;
         TEXTURE *Local_Texture;
         int i, j;

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE3 (DASH_TOKEN, PLUS_TOKEN, FLOAT_TOKEN)
            UNGET
            Local_Shape = Get_Bicubic_Patch_Shape();
         Local_Shape->Patch_Type = (int)Parse_Float();
         if (Local_Shape->Patch_Type == 2 ||
            Local_Shape->Patch_Type == 3)
            Local_Shape->Flatness_Value = Parse_Float();
         else
            Local_Shape->Flatness_Value = 0.1;
         Local_Shape->U_Steps = (int)Parse_Float();
         Local_Shape->V_Steps = (int)Parse_Float();
         for (i=0;i<4;i++) for (j=0;j<4;j++)
            Parse_Vector(&(Local_Shape -> Control_Points[i][j]));
         Precompute_Patch_Values(Local_Shape); /* interpolated mesh coords */
         EXIT
         END_CASE

         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type==BICUBIC_PATCH_CONSTANT)
                  Local_Shape = (BICUBIC_PATCH *) Copy((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (LEFT_ANGLE_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (TRANSLATE_TOKEN)
               Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert ((OBJECT *) Local_Shape);
         END_CASE

         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         Link ((OBJECT *) Local_Texture,
            (OBJECT **) &Local_Texture->Next_Texture,
            (OBJECT **) &Local_Shape->Shape_Texture);
         END_CASE

         CASE (COLOUR_TOKEN)
            Local_Shape->Shape_Colour = Get_Colour();
         Parse_Colour (Local_Shape->Shape_Colour);
         END_CASE



         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         return ((SHAPE *) Local_Shape);
      }

      SHAPE *Parse_Box ()
      {
         BOX *Local_Shape;
         CONSTANT Constant_Id;
         VECTOR Local_Vector;
         TEXTURE *Local_Texture;
         TEXTURE *temp_texture;

         Local_Shape = NULL;
         EXPECT
         CASE (LEFT_CURLY_TOKEN)
            EXIT
            END_CASE
            OTHERWISE
            Parse_Error (LEFT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (LEFT_ANGLE_TOKEN)
            UNGET
            Local_Shape = Get_Box_Shape();
         Parse_Vector(&(Local_Shape->bounds[0]));
         Parse_Vector(&(Local_Shape->bounds[1]));
         EXIT
         END_CASE

         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == BOX_CONSTANT)
                  Local_Shape = (BOX *)Copy((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (LEFT_ANGLE_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (TRANSLATE_TOKEN)
               Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert ((OBJECT *) Local_Shape);
         END_CASE

         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         {
            for (temp_texture = Local_Texture ;
               	     temp_texture->Next_Texture != NULL ;
               	     temp_texture = temp_texture->Next_Texture)
            {}

            temp_texture->Next_Texture = Local_Shape->Shape_Texture;
            Local_Shape->Shape_Texture = Local_Texture;
         }
         END_CASE

         CASE (COLOUR_TOKEN)
            Local_Shape->Shape_Colour = Get_Colour();
         Parse_Colour (Local_Shape->Shape_Colour);
         END_CASE

         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         return ((SHAPE *) Local_Shape);
      }

      SHAPE *Parse_Blob ()
      {
         BLOB *Local_Shape;
         CONSTANT Constant_Id;
         VECTOR Local_Vector;
         TEXTURE *Local_Texture;
         TEXTURE *temp_texture;
         DBL threshold;
         int npoints;
         blobstackptr blob_components, blob_component;

         Local_Shape = NULL;
         EXPECT
         CASE (LEFT_CURLY_TOKEN)
            EXIT
            END_CASE
            OTHERWISE
            Parse_Error (LEFT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE2 (THRESHOLD_TOKEN, COMPONENT_TOKEN)
            UNGET
            Local_Shape = Get_Blob_Shape();
         blob_components = NULL;
         npoints = 0;
         threshold = 1.0;

         /* Here is where we get the blob coefficients */
         EXPECT
         CASE (THRESHOLD_TOKEN)
            threshold = Parse_Float();
         END_CASE

         CASE (COMPONENT_TOKEN)
            blob_component = (blobstackptr) malloc(sizeof(struct
               blob_list_struct));
         if (blob_component == NULL)
            Error("Out of Memory! Cannot allocate blob component");
         blob_component->elem.coeffs[2] = Parse_Float();
         blob_component->elem.radius2   = Parse_Float();
         Parse_Vector(&blob_component->elem.pos);
         blob_component->next = blob_components;
         blob_components = blob_component;
         npoints++;
         END_CASE

         OTHERWISE
         UNGET
         EXIT
         END_CASE
         END_EXPECT

         /* Finally, process the information */
         MakeBlob((OBJECT *)Local_Shape, threshold, blob_components, npoints, 0);
         EXIT
         END_CASE

         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == BLOB_CONSTANT)
                  Local_Shape = (BLOB *)Copy((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (FLOAT_TOKEN);
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (STURM_TOKEN)
               Local_Shape->Sturm_Flag = 1;
         END_CASE

         CASE (TRANSLATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Local_Shape, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert ((OBJECT *) Local_Shape);
         END_CASE

         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         {
            for (temp_texture = Local_Texture ;
               	     temp_texture->Next_Texture != NULL ;
               	     temp_texture = temp_texture->Next_Texture)
            {}

            temp_texture->Next_Texture = Local_Shape->Shape_Texture;
            Local_Shape->Shape_Texture = Local_Texture;
         }
         END_CASE

         CASE (COLOUR_TOKEN)
            Local_Shape->Shape_Colour = Get_Colour();
         Parse_Colour (Local_Shape->Shape_Colour);
         END_CASE

         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         return ((SHAPE *) Local_Shape);
      }


      CSG_SHAPE *Parse_CSG (type, Parent_Object)
         int type;
      OBJECT *Parent_Object;
      {
         CSG_SHAPE *Container = NULL;
         SHAPE *Local_Shape;
         VECTOR Local_Vector;
         CONSTANT Constant_Id;
         int First_Shape_Parsed = FALSE;

         if (type == CSG_UNION_TYPE)
            Container = Get_CSG_Union ();

         else if ((type == CSG_INTERSECTION_TYPE) || (type == CSG_DIFFERENCE_TYPE))
            Container = Get_CSG_Intersection ();

         Container -> Parent_Object = Parent_Object;

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if ((Constants[(int)Constant_Id].Constant_Type == CSG_INTERSECTION_CONSTANT)
                  || (Constants[(int)Constant_Id].Constant_Type == CSG_UNION_CONSTANT)
                  || (Constants[(int)Constant_Id].Constant_Type == CSG_DIFFERENCE_CONSTANT)) {
                  free (Container);
                  Container = (CSG_SHAPE *) Copy ((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
                  Set_CSG_Parents(Container, Parent_Object);
               }
               else
                  Type_Error ();
            else
               Undeclared ();
         END_CASE          

         CASE (LIGHT_SOURCE_TOKEN)
            Local_Shape = Parse_Light_Source ();
         Local_Shape -> Parent_Object = Parent_Object;
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (SPHERE_TOKEN)
            Local_Shape = Parse_Sphere ();
         Local_Shape -> Parent_Object = Parent_Object;
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (PLANE_TOKEN)
            Local_Shape = Parse_Plane ();
         Local_Shape -> Parent_Object = Parent_Object;
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (TRIANGLE_TOKEN)
            Local_Shape = Parse_Triangle ();
         Local_Shape -> Parent_Object = Parent_Object;
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (SMOOTH_TRIANGLE_TOKEN)
            Local_Shape = Parse_Smooth_Triangle ();
         Local_Shape -> Parent_Object = Parent_Object;
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (QUADRIC_TOKEN)
            Local_Shape = Parse_Quadric ();
         Local_Shape -> Parent_Object = Parent_Object;
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (HEIGHT_FIELD_TOKEN)
            Local_Shape = Parse_Height_Field ();
         Local_Shape -> Parent_Object = Parent_Object;
         if((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape-> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE


         CASE (CUBIC_TOKEN)
            Local_Shape = Parse_Poly (3);
         Local_Shape -> Parent_Object = Parent_Object;
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (QUARTIC_TOKEN)
            Local_Shape = Parse_Poly (4);
         Local_Shape -> Parent_Object = Parent_Object;
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (POLY_TOKEN)
            Local_Shape = Parse_Poly (0);
         Local_Shape -> Parent_Object = Parent_Object;
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (BOX_TOKEN)
            Local_Shape = Parse_Box ();
         Local_Shape -> Parent_Object = Parent_Object;
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (BLOB_TOKEN)
            Local_Shape = Parse_Blob ();
         Local_Shape -> Parent_Object = Parent_Object;
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (BICUBIC_PATCH_TOKEN)
            Local_Shape = Parse_Bicubic_Patch ();
         Local_Shape -> Parent_Object = Parent_Object;
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (UNION_TOKEN)
            Local_Shape = (SHAPE *) Parse_CSG (CSG_UNION_TYPE, Parent_Object);
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (INTERSECTION_TOKEN)
            Local_Shape = (SHAPE *) Parse_CSG (CSG_INTERSECTION_TYPE, Parent_Object);
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         CASE (DIFFERENCE_TOKEN)
            Local_Shape = (SHAPE *) Parse_CSG (CSG_DIFFERENCE_TYPE, Parent_Object);
         if ((type == CSG_DIFFERENCE_TYPE) && First_Shape_Parsed)
            Invert ((OBJECT *) Local_Shape);
         First_Shape_Parsed = TRUE;
         Link((OBJECT *) Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Container -> Shapes));
         END_CASE

         OTHERWISE
         UNGET
         EXIT
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (TRANSLATE_TOKEN)
               Parse_Vector (&Local_Vector);
         Translate((OBJECT *) Container, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Container, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Container, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert ((OBJECT *) Container);
         END_CASE

         OTHERWISE
         if (type == CSG_UNION_TYPE)
            Parse_Error (RIGHT_CURLY_TOKEN);
         else if (type == CSG_INTERSECTION_TYPE)
            Parse_Error (RIGHT_CURLY_TOKEN);
         else
            Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         return ((CSG_SHAPE *) Container);
      }

      SHAPE *Parse_Shape (Object)
         OBJECT *Object;
      {
         SHAPE *Local_Shape = NULL;

         EXPECT
         CASE (LIGHT_SOURCE_TOKEN)
            Local_Shape = Parse_Light_Source ();
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE


         CASE (SPHERE_TOKEN)
            Local_Shape = Parse_Sphere ();
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE

         CASE (PLANE_TOKEN)
            Local_Shape = Parse_Plane ();
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE

         CASE (TRIANGLE_TOKEN)
            Local_Shape = Parse_Triangle ();
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE

         CASE (SMOOTH_TRIANGLE_TOKEN)
            Local_Shape = Parse_Smooth_Triangle ();
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE

         CASE (QUADRIC_TOKEN)
            Local_Shape = Parse_Quadric ();
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE

         CASE (HEIGHT_FIELD_TOKEN)
            Local_Shape = Parse_Height_Field ();
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE

         CASE (CUBIC_TOKEN)
            Local_Shape = Parse_Poly (3);
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE

         CASE (QUARTIC_TOKEN)
            Local_Shape = Parse_Poly (4);
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE

         CASE (POLY_TOKEN)
            Local_Shape = Parse_Poly (0);
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE

         CASE (BOX_TOKEN)
            Local_Shape = Parse_Box ();
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE

         CASE (BLOB_TOKEN)
            Local_Shape = Parse_Blob ();
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE

         CASE (BICUBIC_PATCH_TOKEN)
            Local_Shape = Parse_Bicubic_Patch ();
         Local_Shape -> Parent_Object = Object;
         EXIT
         END_CASE

         CASE (UNION_TOKEN)
            Local_Shape = (SHAPE *) Parse_CSG (CSG_UNION_TYPE, Object);
         EXIT
         END_CASE

         CASE (INTERSECTION_TOKEN)
            Local_Shape = (SHAPE *) Parse_CSG (CSG_INTERSECTION_TYPE, Object);
         EXIT
         END_CASE

         CASE (DIFFERENCE_TOKEN)
            Local_Shape = (SHAPE *) Parse_CSG (CSG_DIFFERENCE_TYPE, Object);
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (QUADRIC_TOKEN);
         END_CASE
         END_EXPECT
         return (Local_Shape);
      }

      OBJECT *Parse_Object ()
      {
         OBJECT *Object;
         SHAPE *Local_Shape;
         VECTOR Local_Vector;
         CONSTANT Constant_Id;
         TEXTURE *Local_Texture;
         TEXTURE *temp_texture;

         Object = NULL;

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == OBJECT_CONSTANT)
                  Object = (OBJECT *) Copy((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         EXIT
         END_CASE

         CASE6 (SPHERE_TOKEN, QUADRIC_TOKEN, QUARTIC_TOKEN, UNION_TOKEN,
            INTERSECTION_TOKEN, DIFFERENCE_TOKEN)
            CASE6 (TRIANGLE_TOKEN, SMOOTH_TRIANGLE_TOKEN, PLANE_TOKEN, CUBIC_TOKEN,
               POLY_TOKEN, BICUBIC_PATCH_TOKEN)
               CASE4  (HEIGHT_FIELD_TOKEN, LIGHT_SOURCE_TOKEN, BOX_TOKEN, BLOB_TOKEN)
                  UNGET
                  if (Object == NULL)
                     Object = Get_Object();

         Local_Shape = Parse_Shape(Object);
         Link((OBJECT *)Local_Shape, (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Object -> Shape));
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (SHAPE_TOKEN);
         EXIT
         END_CASE
         END_EXPECT

         EXPECT
         CASE (BOUNDED_TOKEN)

            GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            OTHERWISE
            UNGET
            Local_Shape = Parse_Shape(Object);
         Link((OBJECT *) Local_Shape,
            (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Object -> Bounding_Shapes));
         END_CASE
         END_EXPECT       
         END_CASE

         CASE (CLIPPED_TOKEN)

            GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            OTHERWISE
            UNGET
            Local_Shape = Parse_Shape(Object);
         Link((OBJECT *) Local_Shape,
            (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Object -> Clipping_Shapes));
         END_CASE
         END_EXPECT       
         END_CASE

         CASE (COLOUR_TOKEN)
            Object->Object_Colour = Get_Colour();
         Parse_Colour (Object -> Object_Colour);
         END_CASE

         CASE (TEXTURE_TOKEN)
            Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         if (Object->Object_Texture == Default_Texture)
            Object->Object_Texture = Local_Texture;
         else
         {
            for (temp_texture = Local_Texture ;
               	     temp_texture->Next_Texture != NULL ;
               	     temp_texture = temp_texture->Next_Texture)
            {}

            temp_texture->Next_Texture = Object->Object_Texture;
            Object->Object_Texture = Local_Texture;
         }
         END_CASE

         CASE (NO_SHADOW_TOKEN)
            Object -> No_Shadow_Flag = TRUE;
         END_CASE

         CASE (LIGHT_SOURCE_TOKEN)
            Error("Light source must be defined using new syntax");
         END_CASE

         CASE (TRANSLATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Translate (Object, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate (Object, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale (Object, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert (Object);
         END_CASE

         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            OTHERWISE
            Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE

         END_EXPECT

         return (Object);
      }

      OBJECT *Parse_Composite ()
      {
         COMPOSITE *Local_Composite;
         OBJECT *Local_Object;
         SHAPE *Local_Shape;
         CONSTANT Constant_Id;
         VECTOR Local_Vector;

         Local_Composite = NULL;

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == COMPOSITE_CONSTANT)
                  Local_Composite = (COMPOSITE *)Copy((OBJECT *) Constants[(int)Constant_Id].Constant_Data);
               else          
                  Type_Error ();
            else
               Undeclared ();
         END_CASE

         CASE (COMPOSITE_TOKEN)
            if (Local_Composite == NULL)       
               Local_Composite = Get_Composite_Object();

         Local_Object = Parse_Composite();
         Link((OBJECT *) Local_Object,(OBJECT **) &(Local_Object -> Next_Object),
            (OBJECT **) &(Local_Composite -> Objects));
         END_CASE

         CASE (OBJECT_TOKEN)
            if (Local_Composite == NULL)       
               Local_Composite = Get_Composite_Object();

         Local_Object = Parse_Object();
         Link(Local_Object, &(Local_Object -> Next_Object),
            &(Local_Composite -> Objects));
         END_CASE

         CASE (RIGHT_CURLY_TOKEN)
            UNGET
            if (Local_Composite == NULL)       
               Local_Composite = Get_Composite_Object();

         EXIT
         END_CASE

         OTHERWISE
         UNGET
         EXIT
         END_CASE
         END_EXPECT

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            CASE (BOUNDED_TOKEN)

               GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            OTHERWISE
            UNGET
            Local_Shape = Parse_Shape((OBJECT *) Local_Composite);
         Link((OBJECT *) Local_Shape,
            (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Local_Composite -> Bounding_Shapes));
         END_CASE
         END_EXPECT       
         END_CASE

         CASE (CLIPPED_TOKEN)

            GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            OTHERWISE
            UNGET
            Local_Shape = Parse_Shape((OBJECT *) Local_Composite);
         Link((OBJECT *) Local_Shape,
            (OBJECT **) &(Local_Shape -> Next_Object),
            (OBJECT **) &(Local_Composite -> Clipping_Shapes));
         END_CASE
         END_EXPECT       
         END_CASE

         CASE (TRANSLATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Local_Composite, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Local_Composite, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Local_Composite, &Local_Vector);
         END_CASE

         CASE (INVERSE_TOKEN)
            Invert ((OBJECT *) Local_Composite);
         END_CASE

         OTHERWISE
         Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT

         return ((OBJECT *) Local_Composite);
      }

      void Parse_Fog ()
      {   

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (COLOUR_TOKEN)
            Parse_Colour (&Parsing_Frame_Ptr->Fog_Colour);
         END_CASE

         CASE (FLOAT_TOKEN)
            Parsing_Frame_Ptr->Fog_Distance = Token.Token_Float;
         END_CASE

         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            OTHERWISE
            Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT
      }

      /*
   }
*/

      void Parse_Frame ()
      {
         OBJECT *Local_Object;

         EXPECT
         CASE (FOG_TOKEN)
            Parse_Fog();
         END_CASE


         CASE (DEFAULT_TOKEN)
            GET(LEFT_CURLY_TOKEN);
         EXPECT    
         CASE (TEXTURE_TOKEN)
            Default_Texture->Constant_Flag = FALSE;
         Default_Texture = Parse_Texture ();
         Default_Texture->Constant_Flag = TRUE;
         END_CASE        
         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            OTHERWISE
            Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT      
         END_CASE


         CASE (MAX_TRACE_LEVEL_TOKEN)
            Max_Trace_Level = Parse_Float ();
         END_CASE        

         CASE (OBJECT_TOKEN)
            Local_Object = Parse_Object();
         Link(Local_Object, &(Local_Object -> Next_Object),
            &(Parsing_Frame_Ptr -> Objects));

         /* light sources are now linked in object parsing */
         /* if (Local_Object -> Light_Source_Flag)
	 Link(Local_Object, &(Local_Object -> Next_Light_Source),
	       &(Parsing_Frame_Ptr -> Light_Sources)); */
         END_CASE

         CASE (COMPOSITE_TOKEN)
            Local_Object = Parse_Composite();
         Link(Local_Object, &(Local_Object -> Next_Object),
            &(Parsing_Frame_Ptr -> Objects));

         /*      Add_Composite_Light_Source ((COMPOSITE *)Local_Object);*/
         END_CASE

         CASE (VIEW_POINT_TOKEN)
            Parse_Viewpoint(&(Parsing_Frame_Ptr -> View_Point));
         END_CASE

         CASE (DECLARE_TOKEN)
            Parse_Declare ();
         END_CASE

         CASE (END_OF_FILE_TOKEN)
            EXIT
            END_CASE

            OTHERWISE 
            Parse_Error (OBJECT_TOKEN);
         END_CASE
         END_EXPECT
      }

      void Parse_Viewpoint (Given_Vp)
         VIEWPOINT *Given_Vp;
      {
         CONSTANT Constant_Id;
         VECTOR Local_Vector, Temp_Vector;
         DBL Direction_Length, Up_Length, Right_Length, Handedness;

         Init_Viewpoint (Given_Vp);

         GET(LEFT_CURLY_TOKEN);

         EXPECT
         CASE (IDENTIFIER_TOKEN)
            if ((Constant_Id = Find_Constant()) != -1)
               if (Constants[(int)Constant_Id].Constant_Type == VIEW_POINT_CONSTANT)
                  *Given_Vp = 
                  *((VIEWPOINT*) Constants[(int)Constant_Id].Constant_Data);
               else
                  Type_Error ();
            else
               Undeclared ();
         END_CASE

         CASE (LOCATION_TOKEN)
            Parse_Vector(&(Given_Vp -> Location));
         END_CASE

         CASE (DIRECTION_TOKEN)
            Parse_Vector(&(Given_Vp -> Direction));
         END_CASE

         CASE (UP_TOKEN)
            Parse_Vector(&(Given_Vp -> Up));
         END_CASE

         CASE (RIGHT_TOKEN)
            Parse_Vector(&(Given_Vp -> Right));
         END_CASE

         CASE (SKY_TOKEN)
            Parse_Vector(&(Given_Vp -> Sky));
         END_CASE

         CASE (LOOK_AT_TOKEN)
            VLength (Direction_Length, Given_Vp->Direction);
         VLength (Up_Length, Given_Vp->Up);
         VLength (Right_Length, Given_Vp->Right);
         VCross (Temp_Vector, Given_Vp->Direction, Given_Vp->Up);
         VDot (Handedness, Temp_Vector, Given_Vp->Right);
         Parse_Vector(&Given_Vp->Direction);

         VSub (Given_Vp->Direction, Given_Vp->Direction, Given_Vp->Location);
         VNormalize (Given_Vp->Direction, Given_Vp->Direction);
         VCross(Given_Vp->Right, Given_Vp->Direction, Given_Vp->Sky);
         VNormalize (Given_Vp->Right, Given_Vp->Right);
         VCross (Given_Vp->Up, Given_Vp->Right, Given_Vp->Direction);
         VScale (Given_Vp->Direction, Given_Vp->Direction, Direction_Length);
         if (Handedness >= 0.0) {
            VScale (Given_Vp->Right, Given_Vp->Right, Right_Length);
         }
         else {
            VScale (Given_Vp->Right, Given_Vp->Right, -Right_Length);
         }

            VScale (Given_Vp->Up, Given_Vp->Up, Up_Length);       
         END_CASE

         CASE (TRANSLATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Translate ((OBJECT *) Given_Vp, &Local_Vector);
         END_CASE

         CASE (ROTATE_TOKEN)
            Parse_Vector (&Local_Vector);
         Rotate ((OBJECT *) Given_Vp, &Local_Vector);
         END_CASE

         CASE (SCALE_TOKEN)
            Parse_Vector (&Local_Vector);
         Scale ((OBJECT *) Given_Vp, &Local_Vector);
         END_CASE

         CASE (RIGHT_CURLY_TOKEN)
            EXIT
            END_CASE

            OTHERWISE
            Parse_Error (RIGHT_CURLY_TOKEN);
         END_CASE
         END_EXPECT
      }

      void Parse_Declare ()
      {
         CONSTANT Constant_Id;
         TEXTURE *Local_Texture;
         TEXTURE *temp_texture;
         struct Constant_Struct *Constant_Ptr;

         GET (IDENTIFIER_TOKEN);
         if ((Constant_Id = Find_Constant()) == -1)
            if (++Number_Of_Constants >= MAX_CONSTANTS)
               Error ("Too many constants \"declared\"");
            else
               Constant_Id = Number_Of_Constants;

         Constant_Ptr = &(Constants[(int)Constant_Id]);
         GET (EQUALS_TOKEN);

         EXPECT
         CASE (OBJECT_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Object();
         Constant_Ptr -> Constant_Type = OBJECT_CONSTANT;
         EXIT
         END_CASE

         CASE (SPHERE_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Sphere ();
         Constant_Ptr -> Constant_Type = SPHERE_CONSTANT;
         EXIT
         END_CASE

         CASE (PLANE_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Plane ();
         Constant_Ptr -> Constant_Type = PLANE_CONSTANT;
         EXIT
         END_CASE

         CASE (TRIANGLE_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Triangle ();
         Constant_Ptr -> Constant_Type = TRIANGLE_CONSTANT;
         EXIT
         END_CASE

         CASE (SMOOTH_TRIANGLE_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Smooth_Triangle ();
         Constant_Ptr -> Constant_Type = SMOOTH_TRIANGLE_CONSTANT;
         EXIT
         END_CASE

         CASE (QUADRIC_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Quadric ();
         Constant_Ptr -> Constant_Type = QUADRIC_CONSTANT;
         EXIT
         END_CASE

         CASE (CUBIC_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Poly (3);
         Constant_Ptr -> Constant_Type = POLY_CONSTANT;
         EXIT
         END_CASE

         CASE (QUARTIC_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Poly (4);
         Constant_Ptr -> Constant_Type = POLY_CONSTANT;
         EXIT
         END_CASE

         CASE (HEIGHT_FIELD_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Height_Field ();
         Constant_Ptr -> Constant_Type = HEIGHT_FIELD_CONSTANT;
         EXIT
         END_CASE

         CASE (POLY_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Poly (0);
         Constant_Ptr -> Constant_Type = POLY_CONSTANT;
         EXIT
         END_CASE

         CASE (BOX_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Box ();
         Constant_Ptr -> Constant_Type = BOX_CONSTANT;
         EXIT
         END_CASE

         CASE (BLOB_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Blob ();
         Constant_Ptr -> Constant_Type = BLOB_CONSTANT;
         EXIT
         END_CASE

         CASE (BICUBIC_PATCH_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Bicubic_Patch ();
         Constant_Ptr -> Constant_Type = BICUBIC_PATCH_CONSTANT;
         EXIT
         END_CASE

         CASE (INTERSECTION_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_CSG(CSG_INTERSECTION_TYPE, NULL);
         Constant_Ptr -> Constant_Type = CSG_INTERSECTION_CONSTANT;
         EXIT
         END_CASE

         CASE (UNION_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_CSG(CSG_UNION_TYPE, NULL);
         Constant_Ptr -> Constant_Type = CSG_UNION_CONSTANT;
         EXIT
         END_CASE

         CASE (DIFFERENCE_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_CSG(CSG_DIFFERENCE_TYPE, NULL);
         Constant_Ptr -> Constant_Type = CSG_DIFFERENCE_CONSTANT;
         EXIT
         END_CASE

         CASE (COMPOSITE_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Composite();
         Constant_Ptr -> Constant_Type = COMPOSITE_CONSTANT;
         EXIT
         END_CASE

         CASE (TEXTURE_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Local_Texture = NULL;
         Constant_Ptr -> Constant_Data = (char *) Local_Texture;
         Constant_Ptr -> Constant_Type = TEXTURE_CONSTANT;
         UNGET
         EXPECT
         CASE (TEXTURE_TOKEN)
            Local_Texture = Default_Texture;
         Local_Texture = Parse_Texture ();
         if (Local_Texture->Constant_Flag)
            Local_Texture = Copy_Texture(Local_Texture);

         Local_Texture -> Constant_Flag = TRUE;

         {
            for (temp_texture = Local_Texture ;
               		 temp_texture->Next_Texture != NULL ;
               		 temp_texture = temp_texture->Next_Texture)
            {}

            temp_texture->Next_Texture = (TEXTURE *) Constant_Ptr->Constant_Data;
            Constant_Ptr->Constant_Data = (char *) Local_Texture;
         }
         END_CASE

         OTHERWISE
         UNGET
         EXIT
         END_CASE
         END_EXPECT
         EXIT
         END_CASE

         CASE (VIEW_POINT_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Get_Viewpoint();
         Constant_Ptr -> Constant_Type = VIEW_POINT_CONSTANT;
         Parse_Viewpoint((VIEWPOINT *) Constant_Ptr -> Constant_Data);
         EXIT
         END_CASE

         CASE (COLOUR_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Get_Colour();
         Constant_Ptr -> Constant_Type = COLOUR_CONSTANT;
         Parse_Colour ((COLOUR *) Constant_Ptr -> Constant_Data);
         EXIT
         END_CASE

         CASE (LIGHT_SOURCE_TOKEN)
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Parse_Light_Source ();
         Constant_Ptr -> Constant_Type = LIGHT_SOURCE_CONSTANT;
         EXIT
         END_CASE


         CASE (LEFT_ANGLE_TOKEN)
            UNGET
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Get_Vector();
         Constant_Ptr -> Constant_Type = VECTOR_CONSTANT;
         Parse_Vector((VECTOR *) Constant_Ptr -> Constant_Data);
         EXIT
         END_CASE

         CASE3 (DASH_TOKEN, PLUS_TOKEN, FLOAT_TOKEN)
            UNGET
            Constant_Ptr -> Identifier_Number = Token.Identifier_Number;
         Constant_Ptr -> Constant_Data = (char *) Get_Float();
         Constant_Ptr -> Constant_Type = FLOAT_CONSTANT;
         *((DBL *) Constant_Ptr -> Constant_Data) = Parse_Float();
         EXIT
         END_CASE

         OTHERWISE
         Parse_Error (OBJECT_TOKEN);
         END_CASE
         END_EXPECT
      }

      void Init_Viewpoint (vp)
         VIEWPOINT *vp;
      {
         vp -> Methods = (void *) &Viewpoint_Methods;
         vp -> Type = VIEWPOINT_TYPE;
         Make_Vector (&vp->Location, 0.0, 0.0, 0.0);
         Make_Vector (&vp->Direction, 0.0, 0.0, 1.0);
         Make_Vector (&vp->Up, 0.0, 1.0, 0.0);
         Make_Vector (&vp->Right, 1.33, 0.0, 0.0);
         Make_Vector (&vp->Sky, 0.0, 1.0, 0.0);
      }

      void Link (New_Object, Field, Old_Object_List)
         OBJECT *New_Object, **Field, **Old_Object_List;
      {
         *Field = *Old_Object_List;
         *Old_Object_List = New_Object;
      }
      static void Link_Shapes (New_Object, Field, Old_Object_List)
         LIGHT_SHAPE *New_Object, **Field, **Old_Object_List;
      {
         *Field = *Old_Object_List;
         *Old_Object_List = New_Object;
      }


      CONSTANT Find_Constant()
      {
         register int i;

         for (i = 1 ; i <= Number_Of_Constants ; i++)
            if (Constants [i].Identifier_Number == Token.Identifier_Number)
               return (i);

         return (-1);
      }


      char *Get_Token_String (Token_Id)
         TOKEN Token_Id;
      {
         register int i;

         for (i = 0 ; i < LAST_TOKEN ; i++)
            if (Reserved_Words[i].Token_Number == Token_Id)
               return (Reserved_Words[i].Token_Name);
         return ("");
      }

      void Parse_Error (Token_Id)
         TOKEN Token_Id;
      {
         char *expected, *found;
         FILE *stat_file;
         fprintf (stderr, "Error in file %s line %d\n", Token.Filename,
            Token.Token_Line_No+1);
         expected = Get_Token_String (Token_Id);
         found = Get_Token_String (Token.Token_Id);
         fprintf (stderr, "%s expected but %s found instead\n", expected, found);
         if (Options & VERBOSE_FILE){
            stat_file = fopen(Stat_File_Name,"w+t");
            fprintf (stat_file, "%s expected but %s found instead\n", expected, found);
            fclose(stat_file);
         }

         exit(1);
      }

      void Type_Error ()
      {
         FILE *stat_file;
         fprintf (stderr, "Error in file %s line %d\n", Token.Filename,
            Token.Token_Line_No+1);
         fprintf (stderr, "Identifier %s is the wrong type\n", Token.Token_String);
         if (Options & VERBOSE_FILE){
            stat_file = fopen(Stat_File_Name,"w+t");
            fprintf (stat_file, "Error in file %s line %d\n", Token.Filename,
               Token.Token_Line_No+1);
            fprintf (stat_file, "Identifier %s is the wrong type\n", Token.Token_String);

            fclose(stat_file);
         }

         exit (1);
      }

      void Undeclared ()
      {
         FILE *stat_file;
         fprintf (stderr, "Error in file %s line %d\n", Token.Filename,
            Token.Token_Line_No+1);
         fprintf (stderr, "Undeclared identifier %s\n", Token.Token_String);
         if (Options & VERBOSE_FILE){
            stat_file = fopen(Stat_File_Name,"w+t");
            fprintf (stat_file, "Error in file %s line %d\n", Token.Filename,
               Token.Token_Line_No+1);
            fprintf (stat_file, "Undeclared identifier %s\n", Token.Token_String);
            fclose(stat_file);
         }

         exit (1);
      }

      void Error (str)
         char *str;
      {
         FILE *stat_file;
         fprintf (stderr, "Error in file %s line %d\n", Token.Filename,
            Token.Token_Line_No+1);
         fprintf (stderr,"%s\n",str);

         if (Options & VERBOSE_FILE){
            stat_file = fopen(Stat_File_Name,"w+t");
            fprintf (stat_file, "Error in file %s line %d\n", Token.Filename,
               Token.Token_Line_No+1);
            fprintf (stat_file,"%s\n",str);
            fclose(stat_file);
         }
         exit (1);
      }

      static void Post_Process_Object (Object)
         OBJECT *Object;
      {
         OBJECT *Temp;

         if (Object->Type == COMPOSITE_TYPE)
            for (Temp = ((COMPOSITE *)Object) -> Objects;
               	  Temp != NULL ; 
               	  Temp = Temp->Next_Object) 
               Post_Process_Object (Temp);
         else
            Post_Process_Shape (Object->Shape);
      }

      static void Post_Process_Shape (Shape)
         SHAPE *Shape;
      {
         SHAPE *Temp_Shape;

         if ((Shape->Type == CSG_UNION_TYPE) 
            || (Shape->Type == CSG_INTERSECTION_TYPE)
            || (Shape->Type == CSG_DIFFERENCE_TYPE))
            for (Temp_Shape = ((CSG_SHAPE *)Shape)->Shapes;
               	  Temp_Shape != NULL ; 
               	  Temp_Shape = Temp_Shape->Next_Object)
               Post_Process_Shape(Temp_Shape);
         else
            if ((Shape->Type == POINT_LIGHT_TYPE)
               ||(Shape->Type == SPOT_LIGHT_TYPE))
               Link_Shapes ((LIGHT_SHAPE *)Shape, 
                  &(((LIGHT_SHAPE *)Shape)->Next_Light_Source),
                  &(Parsing_Frame_Ptr -> Light_Sources));
      }
