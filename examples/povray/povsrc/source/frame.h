/****************************************************************************
*                   frame.h
*
*  This header file is included by all C modules in POV-Ray. It defines all
*  globally-accessible types and constants.
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

/* Generic header for all modules */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "config.h"


#ifndef READ_ENV_VAR_BEFORE 
#define READ_ENV_VAR_BEFORE 
#endif
#ifndef READ_ENV_VAR_AFTER
#define READ_ENV_VAR_AFTER if ((Option_String_Ptr = getenv("POVRAYOPT")) != NULL) read_options(Option_String_Ptr);   
#endif

#ifndef CONFIG_MATH
#define CONFIG_MATH
#endif

#ifndef EPSILON
#define EPSILON 1.0e-10
#endif

#ifndef FILE_NAME_LENGTH
#define FILE_NAME_LENGTH 150
#endif

#ifndef HUGE_VAL
#define HUGE_VAL 1.0e+17
#endif

#ifndef DBL_FORMAT_STRING
#define DBL_FORMAT_STRING "%lf"
#endif

#ifndef DEFAULT_OUTPUT_FORMAT
#define DEFAULT_OUTPUT_FORMAT   'd'
#endif

#ifndef RED_RAW_FILE_EXTENSION
#define RED_RAW_FILE_EXTENSION ".red"
#endif

#ifndef GREEN_RAW_FILE_EXTENSION
#define GREEN_RAW_FILE_EXTENSION ".grn"
#endif

#ifndef BLUE_RAW_FILE_EXTENSION
#define BLUE_RAW_FILE_EXTENSION ".blu"
#endif

#ifndef FILENAME_SEPARATOR
#define FILENAME_SEPARATOR "/"
#endif

/* 0==yes 1==no 2==opt */
#ifndef CASE_SENSITIVE_DEFAULT
#define CASE_SENSITIVE_DEFAULT 0 
#endif

#ifndef READ_FILE_STRING
#define READ_FILE_STRING "rb"
#endif

#ifndef WRITE_FILE_STRING
#define WRITE_FILE_STRING "wb"
#endif

#ifndef APPEND_FILE_STRING
#define APPEND_FILE_STRING "ab"
#endif

#ifndef NORMAL
#define NORMAL '0'
#endif

#ifndef GREY
#define GREY   'G'
#endif

#ifndef START_TIME
#define START_TIME time(&tstart);     
#endif

#ifndef STOP_TIME
#define STOP_TIME  time(&tstop);
#endif

#ifndef TIME_ELAPSED
#define TIME_ELAPSED difftime (tstop, tstart);
#endif

#ifndef STARTUP_POVRAY
#define STARTUP_POVRAY
#endif

#ifndef PRINT_OTHER_CREDITS
#define PRINT_OTHER_CREDITS
#endif

#ifndef TEST_ABORT
#define TEST_ABORT
#endif

#ifndef FINISH_POVRAY
#define FINISH_POVRAY
#endif

#ifndef COOPERATE
#define COOPERATE 
#endif

#ifndef DBL
#define DBL double
#endif

#ifndef ACOS
#define ACOS acos
#endif

#ifndef SQRT
#define SQRT sqrt
#endif

#ifndef POW
#define POW pow
#endif

#ifndef COS
#define COS cos
#endif

#ifndef SIN
#define SIN sin
#endif

#ifndef labs
#define labs(x) (long) ((x<0)?-x:x)
#endif

#ifndef max
#define max(x,y) ((x<y)?y:x)
#endif

#ifndef STRLN
#define STRLN(x) x
#endif

#ifndef PARAMS
#define PARAMS(x) x
#endif

#ifndef ANSIFUNC
#define ANSIFUNC 1
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef IFF_SWITCH_CAST
#define IFF_SWITCH_CAST (int)
#endif

#ifndef PRINT_CREDITS
#define PRINT_CREDITS print_credits();
#endif

#ifndef PRINT_STATS
#define PRINT_STATS print_stats();
#endif

#ifndef MAX_CONSTANTS
#define MAX_CONSTANTS 1000
#endif

#ifndef WAIT_FOR_KEYPRESS
#define WAIT_FOR_KEYPRESS
#endif

/* If compiler version is undefined, then make it 'u' for unknown */
#ifndef COMPILER_VER
#define COMPILER_VER ".u"
#endif


/* These values determine the minumum and maximum distances
   that qualify as ray-object intersections */
#define Small_Tolerance 0.001
#define Max_Distance 1.0e7

typedef struct q_entry INTERSECTION;
typedef struct Vector_Struct VECTOR;
typedef DBL MATRIX [4][4];
typedef struct Colour_Struct COLOUR;
typedef struct Colour_Map_Entry COLOUR_MAP_ENTRY;
typedef struct Colour_Map_Struct COLOUR_MAP;
typedef struct Transformation_Struct TRANSFORMATION;
typedef struct Image_Struct IMAGE;
typedef struct Texture_Struct TEXTURE;
typedef struct Method_Struct METHODS;
typedef struct Viewpoint_Struct VIEWPOINT;
typedef struct Object_Shape SHAPE;
typedef struct Object_Struct OBJECT;
typedef struct Sphere_Shape SPHERE;
typedef struct Light_Source_Shape LIGHT_SHAPE;
typedef struct Quadric_Shape QUADRIC;
typedef struct Poly_Shape POLY;
typedef struct Bicubic_Patch_Shape BICUBIC_PATCH;
typedef struct Triangle_Shape TRIANGLE;
typedef struct Smooth_Triangle_Shape SMOOTH_TRIANGLE;
typedef struct Plane_Shape PLANE;
typedef struct Box_Shape BOX;
typedef struct Blob_Shape BLOB;
typedef struct CSG_Type CSG_SHAPE;
typedef struct Composite_Object_Struct COMPOSITE;
typedef struct Ray_Struct RAY;
typedef struct Frame_Struct FRAME;
typedef struct prioq_struct PRIOQ;
typedef int TOKEN;
typedef int CONSTANT;
typedef struct Chunk_Header_Struct CHUNK_HEADER;
typedef struct Data_File_Struct DATA_FILE;
typedef struct complex_block complex;
typedef struct Height_Field_Shape HEIGHT_FIELD;
typedef short WORD;


struct Vector_Struct
{
   DBL x, y, z;
};


struct Colour_Struct
{
   DBL Red, Green, Blue, Alpha;
};


struct Colour_Map_Entry
{
   DBL start, end;
   COLOUR Start_Colour, End_Colour;
};


struct Colour_Map_Struct
{
   int Number_Of_Entries;
   COLOUR_MAP_ENTRY *Colour_Map_Entries;
   int Transparency_Flag;
};


struct Transformation_Struct
{
   MATRIX matrix;
   MATRIX inverse;
};


/* Types for reading IFF files. */
typedef struct {
   unsigned short Red, Green, Blue, Alpha;
} IMAGE_COLOUR;

struct Image_Line
{
   unsigned char *red, *green, *blue;
};

typedef struct Image_Line IMAGE_LINE;

struct Image_Struct
{
   DBL width, height;
   int iwidth, iheight;
   int Map_Type;
   int Interpolation_Type;
   short Once_Flag;
   short Use_Colour_Flag;
   VECTOR Image_Gradient;
   short Colour_Map_Size;
   IMAGE_COLOUR *Colour_Map;
   union {
      IMAGE_LINE *rgb_lines;
      unsigned char **map_lines;
   }    data;  
};

/* Image/Bump Map projection methods */
#define PLANAR_MAP      0
#define SPHERICAL_MAP   1
#define CYLINDRICAL_MAP 2
#define PARABOLIC_MAP   3
#define HYPERBOLIC_MAP  4
#define TORUS_MAP       5
#define PIRIFORM_MAP    6
#define OLD_MAP         7

/* Bit map interpolation types */
#define NO_INTERPOLATION 0
#define NEAREST_NEIGHBOR 1
#define BILINEAR         2
#define CUBIC_SPLINE     3
#define NORMALIZED_DIST  4 


/* Coloration texture list */
#define NO_TEXTURE               0
#define COLOUR_TEXTURE           1
#define BOZO_TEXTURE             2
#define MARBLE_TEXTURE           3
#define WOOD_TEXTURE             4
#define CHECKER_TEXTURE          5
#define CHECKER_TEXTURE_TEXTURE  6
#define SPOTTED_TEXTURE          7
#define AGATE_TEXTURE            8
#define GRANITE_TEXTURE          9
#define GRADIENT_TEXTURE        10
#define IMAGEMAP_TEXTURE        11
#define PAINTED1_TEXTURE        12 
#define PAINTED2_TEXTURE        13 
#define PAINTED3_TEXTURE        14 
#define ONION_TEXTURE           15 
#define LEOPARD_TEXTURE         16 
#define BRICK_TEXTURE           17 /* RHA 2/92 for brick */
#define MATERIAL_MAP_TEXTURE    99 /* Not really colored, but... CdW */

/* Normal perturbation (bumpy) texture list  */
#define NO_BUMPS   0
#define WAVES      1
#define RIPPLES    2
#define WRINKLES   3
#define BUMPS      4
#define DENTS      5
#define BUMPY1     6
#define BUMPY2     7
#define BUMPY3     8
#define BUMPMAP    9

struct Texture_Struct
{
   TEXTURE *Next_Texture;
   TEXTURE *Next_Material;
   int Number_Of_Materials;
   DBL Object_Reflection;
   DBL Object_Ambient;
   DBL Object_Diffuse, Object_Brilliance;
   DBL Object_Index_Of_Refraction;
   DBL Object_Refraction, Object_Transmit;
   DBL Object_Specular, Object_Roughness;
   DBL Object_Phong, Object_PhongSize;
   DBL Bump_Amount;
   DBL Texture_Randomness;
   DBL Frequency;
   DBL Phase;
   int Texture_Number;
   int Bump_Number;
   int Texture_Index;
   TRANSFORMATION *Texture_Transformation;
   COLOUR *Colour1;
   COLOUR *Colour2;
   DBL Turbulence;
   VECTOR Texture_Gradient;
   COLOUR_MAP *Colour_Map;
   IMAGE *Image;
   IMAGE *Bump_Image;
   IMAGE *Material_Image;
   short Metallic_Flag, Once_Flag, Constant_Flag;
   int Octaves; /* dmf, 1/92 for turb */
   DBL Mortar;  /* rha, 2/92 for brick */
};

/* Object/shape types */
#define SPHERE_TYPE            0
#define TRIANGLE_TYPE          1
#define SMOOTH_TRIANGLE_TYPE   2
#define PLANE_TYPE             3
#define QUADRIC_TYPE           4
#define POLY_TYPE              5
#define BICUBIC_PATCH_TYPE     6
#define COMPOSITE_TYPE         7
#define OBJECT_TYPE            8
#define CSG_UNION_TYPE         9
#define CSG_INTERSECTION_TYPE 10
#define CSG_DIFFERENCE_TYPE   11
#define VIEWPOINT_TYPE        12
#define HEIGHT_FIELD_TYPE     13
#define POINT_LIGHT_TYPE      14
#define SPOT_LIGHT_TYPE       15
#define BOX_TYPE              16
#define BLOB_TYPE             17

struct Object_Struct
{
   METHODS *Methods;
   int Type;
   struct Object_Struct *Next_Object;
   /*   struct Object_Struct *Next_Light_Source; */
   SHAPE *Bounding_Shapes;
   SHAPE *Clipping_Shapes;
   SHAPE *Shape;
   char No_Shadow_Flag;
   COLOUR *Object_Colour;
   TEXTURE *Object_Texture;
};


typedef INTERSECTION *(*INTERSECTION_METHOD)PARAMS((OBJECT *, RAY *));
typedef int (*ALL_INTERSECTIONS_METHOD)PARAMS((OBJECT *, RAY *, PRIOQ *));
typedef int (*INSIDE_METHOD)PARAMS((VECTOR *, OBJECT *));
typedef void (*NORMAL_METHOD)PARAMS((VECTOR *, OBJECT *, VECTOR *));
typedef void *(*COPY_METHOD)PARAMS((OBJECT *));
typedef void (*TRANSLATE_METHOD)PARAMS((OBJECT *, VECTOR *));
typedef void (*ROTATE_METHOD)PARAMS((OBJECT *, VECTOR *));
typedef void (*SCALE_METHOD)PARAMS((OBJECT *, VECTOR *));
typedef void (*INVERT_METHOD)PARAMS((OBJECT *));

struct Method_Struct
{
   INTERSECTION_METHOD Intersection_Method;
   ALL_INTERSECTIONS_METHOD All_Intersections_Method;
   INSIDE_METHOD Inside_Method;
   NORMAL_METHOD Normal_Method;
   COPY_METHOD Copy_Method;
   TRANSLATE_METHOD Translate_Method;
   ROTATE_METHOD Rotate_Method;
   SCALE_METHOD Scale_Method;
   INVERT_METHOD Invert_Method;
};


#define All_Intersections(x,y,z) ((*((x)->Methods->All_Intersections_Method)) (x,y,z))
#define Intersection(x,y) ((INTERSECTION *) ((*((x)->Methods->Intersection_Method)) (x,y)))
#define Inside(x,y) ((*((y)->Methods->Inside_Method)) (x,y))
#define Normal(x,y,z) ((*((y)->Methods->Normal_Method)) (x,y,z))
#define Copy(x) ((*((x)->Methods->Copy_Method)) (x))
#define Translate(x,y) ((*((x)->Methods->Translate_Method)) (x,y))
#define Scale(x,y) ((*((x)->Methods->Scale_Method)) (x,y))
#define Rotate(x,y) ((*((x)->Methods->Rotate_Method)) (x,y))
#define Invert(x) ((*((x)->Methods->Invert_Method)) (x))

struct Viewpoint_Struct
{
   METHODS *Methods;
   int Type;
   VECTOR Location;
   VECTOR Direction;
   VECTOR Up;
   VECTOR Right;
   VECTOR Sky;
};


struct Object_Shape
{
   METHODS *Methods;
   int Type;
   struct Object_Shape *Next_Object;
   void *Parent_Object;
   TEXTURE *Shape_Texture;
   COLOUR *Shape_Colour;
};


struct Sphere_Shape
{
   METHODS *Methods;
   int Type;
   SHAPE *Next_Object;
   OBJECT *Parent_Object;
   TEXTURE *Shape_Texture;
   COLOUR *Shape_Colour;
   VECTOR  Center;
   DBL     Radius;
   DBL     Radius_Squared;
   DBL     Inverse_Radius;
   VECTOR  VPOtoC;
   DBL     VPOCSquared;
   short   VPinside, VPCached, Inverted;
};

struct Box_Shape
{
   METHODS *Methods;
   int Type;
   SHAPE *Next_Object;
   OBJECT *Parent_Object;
   TEXTURE *Shape_Texture;
   COLOUR *Shape_Colour;
   TRANSFORMATION *Transform;
   VECTOR bounds[2];
   short Inverted;
};

/* Blob types */
typedef struct {
   VECTOR pos;
   DBL radius2;
   DBL coeffs[3];
   DBL tcoeffs[5];
} Blob_Element;

typedef struct blob_list_struct *blobstackptr;
struct blob_list_struct {
   Blob_Element elem;
   blobstackptr next;
};

typedef struct {
   int type, index;
   DBL bound;
} Blob_Interval;

struct Blob_Shape
{
   METHODS *Methods;
   int Type;
   SHAPE *Next_Object;
   OBJECT *Parent_Object;
   TEXTURE *Shape_Texture;
   COLOUR *Shape_Colour;
   TRANSFORMATION *Transform;
   short Inverted;
   int count;
   DBL threshold;
   Blob_Element *list;
   Blob_Interval *intervals;
   int Sturm_Flag;
};

struct Light_Source_Shape
{
   METHODS *Methods;
   int Type;
   SHAPE *Next_Object;
   OBJECT *Parent_Object;
   TEXTURE *Shape_Texture;
   COLOUR *Shape_Colour;
   VECTOR  Center, Points_At;
   LIGHT_SHAPE *Next_Light_Source;
   short Inverted;
   DBL Coeff, Radius, Falloff;
};

struct Quadric_Shape
{
   METHODS *Methods;
   int Type;
   SHAPE *Next_Object;
   OBJECT *Parent_Object;
   TEXTURE *Shape_Texture;
   COLOUR *Shape_Colour;
   VECTOR  Object_2_Terms;
   VECTOR  Object_Mixed_Terms;
   VECTOR  Object_Terms;
   DBL Object_Constant;
   DBL Object_VP_Constant;
   int Constant_Cached;
   int Non_Zero_Square_Term;
};


#define GIF 0   /* These are the types of image maps which can be used as a */
#define POT 1   /* height field. */
#define TGA 2

typedef struct {
   DBL min_y, max_y;
} HF_BLOCK;

struct Height_Field_Shape
{
   METHODS *Methods;
   int Type;
   SHAPE *Next_Object;
   OBJECT *Parent_Object;
   TEXTURE *Shape_Texture;
   COLOUR *Shape_Colour;
   TRANSFORMATION *Transformation;
   BOX *bounding_box;
   DBL Block_Size;
   DBL Inv_Blk_Size;
   HF_BLOCK **Block;
   float **Map;
};

#define MAX_ORDER 7

struct Poly_Shape
{
   METHODS *Methods;
   int Type;
   SHAPE *Next_Object;
   OBJECT *Parent_Object;
   TEXTURE *Shape_Texture;
   COLOUR *Shape_Colour;
   TRANSFORMATION *Transform;
   short Inverted;
   int Order, Sturm_Flag;
   DBL *Coeffs;
};


typedef struct Bezier_Node_Struct BEZIER_NODE;
typedef struct Bezier_Child_Struct BEZIER_CHILDREN;
typedef struct Bezier_Vertices_Struct BEZIER_VERTICES;


struct Bezier_Child_Struct
{
   BEZIER_NODE *Children[4];
};


struct Bezier_Vertices_Struct
{
   VECTOR Vertices[4];
};


struct Bezier_Node_Struct
{
   int Node_Type;      /* Is this an interior node, or a leaf */
   VECTOR Center;      /* Center of sphere bounding the (sub)patch */
   DBL Radius_Squared; /* Radius of bounding sphere (squared) */
   int Count;          /* # of subpatches associated with this node */
   void *Data_Ptr;     /* Either pointer to vertices or pointer to children */
};


#define BEZIER_INTERIOR_NODE 0
#define BEZIER_LEAF_NODE 1
#define MAX_BICUBIC_INTERSECTIONS 32


struct Bicubic_Patch_Shape
{
   METHODS *Methods;
   int Type;
   SHAPE *Next_Object;
   OBJECT *Parent_Object;
   TEXTURE *Shape_Texture;
   COLOUR *Shape_Colour;
   int Patch_Type, U_Steps, V_Steps;
   VECTOR Control_Points[4][4];
   VECTOR Bounding_Sphere_Center;
   DBL Bounding_Sphere_Radius;
   DBL Flatness_Value;
   int Intersection_Count;
   VECTOR Normal_Vector[MAX_BICUBIC_INTERSECTIONS];
   VECTOR Intersection_Point[MAX_BICUBIC_INTERSECTIONS];
   VECTOR **Interpolated_Grid, **Interpolated_Normals, **Smooth_Normals;
   DBL **Interpolated_D;
   BEZIER_NODE *Node_Tree;
};


#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2


struct Triangle_Shape
{
   METHODS *Methods;
   int Type;
   SHAPE *Next_Object;
   OBJECT *Parent_Object;
   TEXTURE *Shape_Texture;
   COLOUR *Shape_Colour;
   VECTOR  Normal_Vector;
   DBL     Distance;
   DBL     VPNormDotOrigin;
unsigned int  VPCached:1;
unsigned int  Dominant_Axis:2;
unsigned int  Inverted:1;
unsigned int  vAxis:2;  /* used only for smooth triangles */
   VECTOR  P1, P2, P3;
   short int Degenerate_Flag;
};


struct Smooth_Triangle_Shape
{
   METHODS *Methods;
   int Type;
   SHAPE *Next_Object;
   OBJECT *Parent_Object;
   TEXTURE *Shape_Texture;
   COLOUR *Shape_Colour;
   VECTOR  Normal_Vector;
   DBL     Distance;
   DBL     VPNormDotOrigin;
unsigned int  VPCached:1;
unsigned int  Dominant_Axis:2;
unsigned int  Inverted:1;
unsigned int  vAxis:2;         /* used only for smooth triangles */
   VECTOR  P1, P2, P3;
   short int Degenerate_Flag;  /* do not move this field */
   VECTOR  N1, N2, N3, Perp;
   DBL  BaseDelta;
};



struct Plane_Shape
{
   METHODS *Methods;
   int Type;
   SHAPE *Next_Object;
   OBJECT *Parent_Object;
   TEXTURE *Shape_Texture;
   COLOUR *Shape_Colour;
   VECTOR  Normal_Vector;
   DBL     Distance;
   DBL     VPNormDotOrigin;
   int     VPCached;
};


struct CSG_Type
{
   METHODS *Methods;
   int Type;
   SHAPE *Next_Object;
   OBJECT *Parent_Object;
   SHAPE *Shapes;
};


struct Composite_Object_Struct
{
   METHODS *Methods;
   int Type;
   OBJECT *Next_Object;
   /*   OBJECT *Next_Light_Source;*/
   SHAPE *Bounding_Shapes;
   SHAPE *Clipping_Shapes;
   OBJECT *Objects;
};


#define MAX_CONTAINING_OBJECTS 10

struct Ray_Struct
{
   VECTOR Initial;               /*  Xo  Yo  Zo  */
   VECTOR Direction;             /*  Xv  Yv  Zv  */
   VECTOR Initial_2;             /*  Xo^2  Yo^2  Zo^2  */
   VECTOR Direction_2;           /*  Xv^2  Yv^2  Zv^2  */
   VECTOR Initial_Direction;     /*  XoXv  YoYv  ZoZv  */
   VECTOR Mixed_Initial_Initial; /*  XoYo  XoZo  YoZo  */
   VECTOR Mixed_Dir_Dir;         /*  XvYv  XvZv  YvZv  */
   VECTOR Mixed_Init_Dir;        /*  XoYv+XvYo  XoZv+XvZo  YoZv+YvZo  */
   int Containing_Index;
   TEXTURE *Containing_Textures [MAX_CONTAINING_OBJECTS];
   DBL Containing_IORs [MAX_CONTAINING_OBJECTS];
   int Quadric_Constants_Cached;
};


struct Frame_Struct
{
   VIEWPOINT View_Point;
   int Screen_Height, Screen_Width;
   LIGHT_SHAPE *Light_Sources;
   OBJECT *Objects;
   DBL Atmosphere_IOR, Antialias_Threshold;
   DBL Fog_Distance;
   COLOUR Fog_Colour;
};


#define DISPLAY 1
#define VERBOSE 2
#define DISKWRITE 4
#define PROMPTEXIT 8
#define ANTIALIAS 16
#define DEBUGGING 32
#define RGBSEPARATE 64
#define EXITENABLE 128
#define CONTINUE_TRACE 256
#define VERBOSE_FILE 512

#define Make_Colour(c,r,g,b) { (c)->Red=(r);(c)->Green=(g);(c)->Blue=(b); (c)->Alpha=0.0; }

#define Make_Vector(v,a,b,c) { (v)->x=(a);(v)->y=(b);(v)->z=(c); }

/* Definitions for PRIOQ structure */

struct q_entry
{
   DBL Depth;
   OBJECT *Object;
   VECTOR Point;
   SHAPE *Shape;
};

struct prioq_struct
{
   struct prioq_struct *next_pq;
   struct q_entry *queue;
   unsigned int current_entry, queue_size;
};


/* Token Definitions for Parser */
/* This list must have the same number of tokens as list in tokenize.c */
#define AGATE_TOKEN                  0
#define ALL_TOKEN                    1
#define ALPHA_TOKEN                  2
#define AMBIENT_TOKEN                3
#define AMPERSAND_TOKEN              4
#define AT_TOKEN                     5
#define BACK_QUOTE_TOKEN             6
#define BACK_SLASH_TOKEN             7
#define BAR_TOKEN                    8
#define BICUBIC_PATCH_TOKEN          9
#define BLUE_TOKEN                  10
#define BRILLIANCE_TOKEN            11
#define BOZO_TOKEN                  12
#define BOUNDED_TOKEN               13
#define BUMPS_TOKEN                 14
#define CHECKER_TOKEN               15
#define CHECKER_TEXTURE_TOKEN       16
#define CLIPPED_TOKEN               17
#define COLON_TOKEN                 18
#define COLOR_TOKEN                 19
#define COLOUR_TOKEN                20
#define COLOR_MAP_TOKEN             21
#define COLOUR_MAP_TOKEN            22
#define COMMA_TOKEN                 23
#define COMPOSITE_TOKEN             24
#define CONCENTRATION_TOKEN         25
#define CUBIC_TOKEN                 26
#define DASH_TOKEN                  27
#define DECLARE_TOKEN               28
#define DENTS_TOKEN                 29
#define DIFFERENCE_TOKEN            30
#define DIFFUSE_TOKEN               31
#define DIRECTION_TOKEN             32
#define DOLLAR_TOKEN                33
#define DUMP_TOKEN                  34
#define EQUALS_TOKEN                35
#define EXCLAMATION_TOKEN           36
#define FLOAT_TOKEN                 37
#define FOG_TOKEN                   38
#define FREQUENCY_TOKEN             39
#define GIF_TOKEN                   40
#define GRADIENT_TOKEN              41
#define GRANITE_TOKEN               42
#define GREEN_TOKEN                 43
#define HASH_TOKEN                  44
#define HAT_TOKEN                   45
#define IDENTIFIER_TOKEN            46
#define IFF_TOKEN                   47
#define IMAGEMAP_TOKEN              48
#define INCLUDE_TOKEN               49
#define INTERSECTION_TOKEN          50
#define INVERSE_TOKEN               51
#define IOR_TOKEN                   52
#define LEFT_ANGLE_TOKEN            53
#define LEFT_CURLY_TOKEN            54
#define LEFT_SQUARE_TOKEN           55
#define LIGHT_SOURCE_TOKEN          56
#define LOCATION_TOKEN              57
#define LOOK_AT_TOKEN               58
#define MARBLE_TOKEN                59
#define METALLIC_TOKEN              60
#define OBJECT_TOKEN                61
#define ONCE_TOKEN                  62
#define PERCENT_TOKEN               63
#define PHASE_TOKEN                 64
#define PHONG_TOKEN                 65
#define PHONGSIZE_TOKEN             66
#define PLANE_TOKEN                 67
#define PLUS_TOKEN                  68
#define POINTS_TOKEN                69
#define POINT_AT_TOKEN              70
#define POLYGON_TOKEN               71
#define POLY_TOKEN                  72
#define QUADRIC_TOKEN               73
#define QUARTIC_TOKEN               74 
#define QUESTION_TOKEN              75
#define RAW_TOKEN                   76
#define RED_TOKEN                   77
#define REFLECTION_TOKEN            78 
#define REFRACTION_TOKEN            79
#define REVOLVE_TOKEN               80
#define RIGHT_TOKEN                 81
#define RIGHT_ANGLE_TOKEN           82
#define RIGHT_PAREN_TOKEN           83
#define RIGHT_SQUARE_TOKEN          84
#define RIPPLES_TOKEN               85
#define ROTATE_TOKEN                86
#define ROUGHNESS_TOKEN             87
#define SCALE_TOKEN                 88
#define SEMI_COLON_TOKEN            89
#define SHAPE_TOKEN                 90
#define SINGLE_QUOTE_TOKEN          91
#define SIZE_TOKEN                  92
#define SKY_TOKEN                   93
#define SLASH_TOKEN                 94
#define SMOOTH_TRIANGLE_TOKEN       95
#define SPECULAR_TOKEN              96
#define SPHERE_TOKEN                97
#define SPOTLIGHT_TOKEN             98
#define SPOTTED_TOKEN               99
#define STAR_TOKEN                  100
#define STRING_TOKEN                101
#define STURM_TOKEN                 102
#define TEXTURE_TOKEN               103
#define TILDE_TOKEN                 104
#define TILE2_TOKEN                 105
#define TRANSLATE_TOKEN             106
#define TRANSMIT_TOKEN              107
#define TRIANGLE_TOKEN              108
#define TURBULENCE_TOKEN            109
#define UNION_TOKEN                 110
#define UP_TOKEN                    111
#define VIEW_POINT_TOKEN            112
#define WAVES_TOKEN                 113
#define WOOD_TOKEN                  114
#define WRINKLES_TOKEN              115 
#define PAINTED1_TOKEN              116 
#define PAINTED2_TOKEN              117
#define PAINTED3_TOKEN              118
#define BUMPY1_TOKEN                119
#define BUMPY2_TOKEN                120
#define BUMPY3_TOKEN                121
#define BUMPMAP_TOKEN               122
#define BUMPSIZE_TOKEN              123
#define MATERIAL_MAP_TOKEN          124 
#define ONION_TOKEN                 125
#define LEOPARD_TOKEN               126
#define DUMMY_TOKEN                 127 /* Dummy token to pad list because some colour_tokens are used twice */
#define INTERPOLATE_TOKEN           128
#define HEIGHT_FIELD_TOKEN          129
#define POT_TOKEN                   130 
#define WATER_LEVEL_TOKEN           131
#define USE_COLOUR_TOKEN            132
#define USE_INDEX_TOKEN             133
#define MAPTYPE_TOKEN               134
#define RIGHT_CURLY_TOKEN           135
#define LEFT_PAREN_TOKEN            136
#define TGA_TOKEN                   137 /* ARE 11/91 for tga mapp/heights */
#define CENTER_TOKEN                138 /* ARE 11/91 for spotlight */
#define FALLOFF_TOKEN               139 /* ARE 11/91 for spotlight */
#define TIGHTNESS_TOKEN             140 /* ARE 11/91 for spotlight */
#define RADIUS_TOKEN                141 /* ARE 11/91 for spotlight */
#define NO_SHADOW_TOKEN             142 /* CEY 12/91 for shadowless objects */
#define END_OF_FILE_TOKEN           143
#define MAX_TRACE_LEVEL_TOKEN       144
#define DEFAULT_TOKEN               145
#define BOX_TOKEN                   146 /* ARE 1/92 for boxes */
#define BLOB_TOKEN                  147 /* ARE 1/92 for blobs */
#define THRESHOLD_TOKEN             148 /* ARE 1/92 for blobs */
#define COMPONENT_TOKEN             149 /* ARE 1/92 for blobs */
#define OCTAVES_TOKEN               150 /* DMF 2/92 for turb  */
#define BRICK_TOKEN                 151 /* RHA 2/92 for brick */
#define MORTAR_TOKEN                152 /* RHA 2/92 for brick */
#define LOST_TOKEN                  153
#define LAST_TOKEN                  154


struct Reserved_Word_Struct
{                                
   TOKEN Token_Number;
   char *Token_Name;
};

/* Here's where you dump the information on the current token (fm. PARSE.C) */

struct Token_Struct
{
   TOKEN Token_Id;
   int Token_Line_No;
   char *Token_String;
   DBL Token_Float;
   int Identifier_Number;
   int Unget_Token, End_Of_File;
   char *Filename;
};

/* Types of constants allowed in DECLARE statement (fm. PARSE.C) */

#define OBJECT_CONSTANT            0
#define VIEW_POINT_CONSTANT        1
#define VECTOR_CONSTANT            2
#define FLOAT_CONSTANT             3
#define COLOUR_CONSTANT            4
#define QUADRIC_CONSTANT           5
#define POLY_CONSTANT              6
#define BICUBIC_PATCH_CONSTANT     7
#define SPHERE_CONSTANT            8
#define PLANE_CONSTANT             9
#define TRIANGLE_CONSTANT         10
#define SMOOTH_TRIANGLE_CONSTANT  11  
#define CSG_INTERSECTION_CONSTANT 12   
#define CSG_UNION_CONSTANT        13
#define CSG_DIFFERENCE_CONSTANT   14
#define COMPOSITE_CONSTANT        15
#define TEXTURE_CONSTANT          16
#define HEIGHT_FIELD_CONSTANT     17
#define BOX_CONSTANT              18
#define BLOB_CONSTANT             19
#define LIGHT_SOURCE_CONSTANT     20

struct Constant_Struct
{
   int Identifier_Number;
   CONSTANT Constant_Type;
   char *Constant_Data;
};

struct Chunk_Header_Struct {
   long name;
   long size;
};

struct Data_File_Struct {
   FILE *File;
   char *Filename;
   int Line_Number;
};

struct complex_block {
   DBL r, c;
};

#define READ_MODE 0
#define WRITE_MODE 1
#define APPEND_MODE 2

struct file_handle_struct {
   char *filename;
   int  mode;
   int width, height;
   int buffer_size;
   char *buffer;
   FILE *file;
   char *(*Default_File_Name_p) PARAMS((void));
   int  (*Open_File_p) PARAMS((struct file_handle_struct *handle,
      char *name, int *width, int *height, int buffer_size,
      int mode));
   void (*Write_Line_p) PARAMS((struct file_handle_struct *handle,
      COLOUR *line_data, int line_number));
   int  (*Read_Line_p) PARAMS((struct file_handle_struct *handle,
      COLOUR *line_data, int *line_number));
   void (*Read_Image_p) PARAMS((IMAGE *Image, char *filename));
   void (*Close_File_p) PARAMS((struct file_handle_struct *handle));
};

typedef struct file_handle_struct FILE_HANDLE;

#define Default_File_Name(h) ((*((h)->Default_File_Name_p)) ())
#define Open_File(h,n,wd,ht,sz,m) ((*((h)->Open_File_p)) (h,n,wd,ht,sz,m))
#define Write_Line(h,l,n) ((*((h)->Write_Line_p)) (h, l, n))
#define Read_Line(h,l,n) ((*((h)->Read_Line_p)) (h, l, n))
#define Read_Image(h,i) ((*((h)->Read_Image_p)) (h, i))
#define Close_File(h) ((*((h)->Close_File_p)) (h))
