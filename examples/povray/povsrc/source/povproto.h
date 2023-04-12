/****************************************************************************
*                   povproto.h
*
*  This module defines the prototypes for all system-independent functions.
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


/* Prototypes for functions defined in povray.c */
void usage PARAMS((void));
void init_vars PARAMS((void));
void close_all PARAMS((void));
void get_defaults PARAMS((void));
void read_options PARAMS((char *file_name));
void parse_option PARAMS((char *Option_String));
void parse_file_name PARAMS((char *File_Name));
void Print_Options PARAMS((void));
void print_stats PARAMS((void));
void print_credits PARAMS((void));
FILE *Locate_File PARAMS((char *filename, char *mode));

/* Prototypes for functions defined in render.c */
void Read_Rendered_Part PARAMS((void));
void Create_Ray PARAMS((RAY *ray, int width, int height, DBL x, DBL y));
void Supersample PARAMS((COLOUR *result, int x, int y, int Width,int Height));
void Start_Tracing PARAMS((void));
void Trace PARAMS((RAY *Ray, COLOUR *Colour));
void Initialize_Renderer PARAMS((void));

/* Prototypes for functions defined in tokenize.c */
void Initialize_Tokenizer PARAMS((char *Input_File_Name));
void Terminate_Tokenizer PARAMS((void));
void Tokenize PARAMS((char *name));
int Process_Token PARAMS((void));
int Skip_Spaces PARAMS((DATA_FILE *Data_File));
int Parse_Comments PARAMS((DATA_FILE *Data_File));
int Parse_C_Comments PARAMS((DATA_FILE *Data_File));

void Begin_String PARAMS((void));
void Stuff_Character PARAMS((int c, DATA_FILE *Data_File));
void End_String PARAMS((DATA_FILE *Data_File));
int Read_Float PARAMS((DATA_FILE *Data_File));
void Parse_String PARAMS((DATA_FILE *Data_File));
int Read_Symbol PARAMS((DATA_FILE *Data_File));
int Find_Reserved PARAMS((void));
int Find_Symbol PARAMS((void));
void Write_Token PARAMS((int Token_Id, DATA_FILE *Data_File));
void Token_Error PARAMS((DATA_FILE *Data_File, char *str));
void Get_Token PARAMS((void));
void Unget_Token PARAMS((void));

/* Prototypes for functions defined in parse.c */
void Parse PARAMS((FRAME *Frame_Ptr));
void Token_Init PARAMS((void));
void Frame_Init PARAMS((void));
COMPOSITE *Get_Composite_Object PARAMS((void));
LIGHT_SHAPE *Get_Light_Source_Shape PARAMS((void));
SPHERE *Get_Sphere_Shape PARAMS((void));
QUADRIC *Get_Quadric_Shape PARAMS((void));
POLY *Get_Poly_Shape PARAMS((int));
BOX *Get_Box_Shape PARAMS((void));
BLOB *Get_Blob_Shape PARAMS((void));
BICUBIC_PATCH *Get_Bicubic_Patch_Shape PARAMS((void));
HEIGHT_FIELD *Get_Height_Field_Shape PARAMS((void));
PLANE *Get_Plane_Shape PARAMS((void));
TRIANGLE *Get_Triangle_Shape PARAMS((void));
SMOOTH_TRIANGLE *Get_Smooth_Triangle_Shape PARAMS((void));
CSG_SHAPE *Get_CSG_Shape PARAMS((void));
CSG_SHAPE *Get_CSG_Union PARAMS((void));
CSG_SHAPE *Get_CSG_Intersection PARAMS((void));
OBJECT *Get_Object PARAMS((void));
TEXTURE *Get_Texture PARAMS((void));
VIEWPOINT *Get_Viewpoint PARAMS((void));
COLOUR *Get_Colour PARAMS((void));
VECTOR *Get_Vector PARAMS((void));
DBL *Get_Float PARAMS((void));
TRANSFORMATION *Get_Transformation PARAMS((void));
DBL Parse_Float PARAMS((void));
void Parse_Vector PARAMS((VECTOR *Given_Vector));
void Parse_Coeffs PARAMS((int order, DBL *Given_Coeffs));
void Parse_Colour PARAMS((COLOUR *Given_Colour));
COLOUR_MAP *Parse_Colour_Map PARAMS((void));
TEXTURE *Copy_Texture PARAMS((TEXTURE *Texture));
TEXTURE *Parse_Texture PARAMS((void));
SHAPE *Parse_Sphere PARAMS((void));
SHAPE *Parse_Light_Source PARAMS((void));

SHAPE *Parse_Plane PARAMS((void));
SHAPE *Parse_Triangle PARAMS((void));
SHAPE *Parse_Smooth_Triangle PARAMS((void));
SHAPE *Parse_Quadric PARAMS((void));
SHAPE *Parse_Poly PARAMS((int));
SHAPE *Parse_Box PARAMS((void));
SHAPE *Parse_Blob PARAMS((void));
SHAPE *Parse_Bicubic_Patch PARAMS((void));
SHAPE *Parse_Height_Field PARAMS((void));
CSG_SHAPE *Parse_CSG PARAMS((int type, OBJECT *Parent_Object));
SHAPE *Parse_Shape PARAMS((OBJECT *Object));
OBJECT *Parse_Object PARAMS((void));
OBJECT *Parse_Composite PARAMS((void));
void Parse_Fog PARAMS((void));
void Parse_Frame PARAMS((void));
void Parse_Viewpoint PARAMS((VIEWPOINT *Given_Vp));
void Parse_Declare PARAMS((void));
void Init_Viewpoint PARAMS((VIEWPOINT *vp));
void Link PARAMS((OBJECT *New_Object,OBJECT **Field,OBJECT **Old_Object_List));
CONSTANT Find_Constant PARAMS((void));
char *Get_Token_String PARAMS((TOKEN Token_Id));
void Parse_Error PARAMS((TOKEN Token_Id));
void Type_Error PARAMS((void));
void Undeclared PARAMS((void));
void Error PARAMS((char *str));

/* Prototypes for functions defined in objects.c */
INTERSECTION *Object_Intersect PARAMS((OBJECT *Object, RAY *Ray));
int All_Composite_Intersections PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue));
int All_Object_Intersections PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue));
int Inside_Basic_Object PARAMS((VECTOR *point, OBJECT *Object)); 
int Inside_Composite_Object PARAMS((VECTOR *point, OBJECT *Object));
void *Copy_Basic_Object PARAMS((OBJECT *Object));
void *Copy_Composite_Object PARAMS((OBJECT *Object));
void Translate_Basic_Object PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Basic_Object PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Basic_Object PARAMS((OBJECT *Object, VECTOR *Vector));
void Translate_Composite_Object PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Composite_Object PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Composite_Object PARAMS((OBJECT *Object, VECTOR *Vector));
void Invert_Basic_Object PARAMS((OBJECT *Object));
void Invert_Composite_Object PARAMS((OBJECT *Object));

/* Prototypes for functions defined in spheres.c */
int All_Sphere_Intersections PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue));
int Intersect_Sphere PARAMS((RAY *Ray, SPHERE *Sphere, DBL *Depth1, DBL *Depth2));
int Inside_Sphere PARAMS((VECTOR *point, OBJECT *Object));
void Sphere_Normal PARAMS((VECTOR *Result, OBJECT *Object, VECTOR *Intersection_Point));
void *Copy_Sphere PARAMS((OBJECT *Object));
void Translate_Sphere PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Sphere PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Sphere PARAMS((OBJECT *Object, VECTOR *Vector));
void Invert_Sphere PARAMS((OBJECT *Object));

/* Prototypes for functions defined in point.c */
int All_Point_Intersections PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue));
int Inside_Point PARAMS((VECTOR *Test_Point, OBJECT *Object));
void *Copy_Point PARAMS((OBJECT *Object));
void Translate_Point PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Point PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Point PARAMS((OBJECT *Object, VECTOR *Vector));
void Invert_Point PARAMS((OBJECT *Object));
DBL Attenuate_Light PARAMS((LIGHT_SHAPE *Light_Source, RAY *Light_Source_Ray));

/* Prototypes for functions defined in quadrics.c */
int All_Quadric_Intersections PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue));
int Intersect_Quadric PARAMS((RAY *Ray, QUADRIC *Shape, DBL *Depth1, DBL *Depth2));
int Inside_Quadric PARAMS((VECTOR *point, OBJECT *Object));
void Quadric_Normal PARAMS((VECTOR *Result, OBJECT *Object, VECTOR *Intersection_Point));
void *Copy_Quadric PARAMS((OBJECT *Object));
void Transform_Quadric PARAMS((QUADRIC *Shape, TRANSFORMATION *Transformation));
void Quadric_To_Matrix PARAMS((QUADRIC *Quadric, MATRIX *Matrix));
void Matrix_To_Quadric PARAMS((MATRIX *Matrix, QUADRIC *Quadric));
void Translate_Quadric PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Quadric PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Quadric PARAMS((OBJECT *Object, VECTOR *Vector));
void Invert_Quadric PARAMS((OBJECT *Object));

/* Prototypes for functions defined in poly.c */
int All_Poly_Intersections PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue));
int Inside_Poly PARAMS((VECTOR *point, OBJECT *Object));
void Poly_Normal PARAMS((VECTOR *Result, OBJECT *Object, VECTOR *Intersection_Point));
void *Copy_Poly PARAMS((OBJECT *Object));
void Translate_Poly PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Poly PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Poly PARAMS((OBJECT *Object, VECTOR *Vector));
void Invert_Poly PARAMS((OBJECT *Object));

/* Prototypes for functions defined in bezier.c */
void Precompute_Patch_Values PARAMS((BICUBIC_PATCH *Shape));
int All_Bicubic_Patch_Intersections PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue));
int Inside_Bicubic_Patch PARAMS((VECTOR *point, OBJECT *Object));
void Bicubic_Patch_Normal PARAMS((VECTOR *Result, OBJECT *Object, VECTOR *Intersection_Point));
void *Copy_Bicubic_Patch PARAMS((OBJECT *Object));
void Translate_Bicubic_Patch PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Bicubic_Patch PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Bicubic_Patch PARAMS((OBJECT *Object, VECTOR *Vector));
void Invert_Bicubic_Patch PARAMS((OBJECT *Object));

/* Prototypes for functions defined in boxes.c */
int All_Box_Intersections PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue));
int Intersect_Boxx PARAMS((RAY *Ray, BOX *box, DBL *Depth1, DBL *Depth2));
int Inside_Box PARAMS((VECTOR *point, OBJECT *Object));
void Box_Normal PARAMS((VECTOR *Result, OBJECT *Object, VECTOR *Intersection_Point));
void *Copy_Box PARAMS((OBJECT *Object));
void Translate_Box PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Box PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Box PARAMS((OBJECT *Object, VECTOR *Vector));
void Invert_Box PARAMS((OBJECT *Object));

/* Prototypes for functions defined in blob.c */
void MakeBlob PARAMS((OBJECT *obj, DBL threshold, blobstackptr bloblist,
int npoints, int sflag));
int All_Blob_Intersections PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue));
int Inside_Blob PARAMS((VECTOR *point, OBJECT *Object));
void Blob_Normal PARAMS((VECTOR *Result, OBJECT *Object, VECTOR *Intersection_Point));
void *Copy_Blob PARAMS((OBJECT *Object));
void Translate_Blob PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Blob PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Blob PARAMS((OBJECT *Object, VECTOR *Vector));
void Invert_Blob PARAMS((OBJECT *Object));

/* Prototypes for functions defined in hfield.c */
void Find_Hf_Min_Max PARAMS((HEIGHT_FIELD *H_Field, IMAGE *Image, int Image_Type));
int Intersect_Pixel PARAMS((int x,int z,RAY *Ray,HEIGHT_FIELD *H_Field,DBL height1,DBL height2));
int Intersect_Sub_Block PARAMS((HF_BLOCK *Block, RAY *Ray, HEIGHT_FIELD *H_Field,
VECTOR *start, VECTOR *end));
int Intersect_Hf_Node PARAMS((RAY *Ray, HEIGHT_FIELD *H_Field, VECTOR *start, VECTOR *end));
int All_HeightFld_Intersections PARAMS((OBJECT *Object,RAY *Ray,PRIOQ *Depth_Queue));
int Intersect_HeightFld PARAMS((RAY *Ray,HEIGHT_FIELD *H_Field,DBL *Depth));
int Inside_HeightFld PARAMS((VECTOR *Test_Point,OBJECT *Object));
void HeightFld_Normal PARAMS((VECTOR *Result,OBJECT *Object,VECTOR *Intersection_Point));
void *Copy_HeightFld PARAMS((OBJECT *Object));
void Translate_HeightFld PARAMS((OBJECT *Object,VECTOR *Vector));
void Rotate_HeightFld PARAMS((OBJECT *Object,VECTOR *Vector));
void Scale_HeightFld PARAMS((OBJECT *Object,VECTOR *Vector));
void Invert_HeightFld PARAMS((OBJECT *Object));

/* Prototypes for functions defined in triangle.c */
void Find_Triangle_Dominant_Axis PARAMS((TRIANGLE *Triangle));
int Compute_Triangle  PARAMS((TRIANGLE *Triangle));
void Compute_Smooth_Triangle  PARAMS((SMOOTH_TRIANGLE *Triangle));
int All_Triangle_Intersections  PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue));
int Intersect_Triangle  PARAMS((RAY *Ray, TRIANGLE *Triangle, DBL *Depth));
int Inside_Triangle  PARAMS((VECTOR *point, OBJECT *Object));
void Triangle_Normal  PARAMS((VECTOR *Result, OBJECT *Object, VECTOR *Intersection_Point));
void *Copy_Triangle  PARAMS((OBJECT *Object));
void Translate_Triangle  PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Triangle  PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Triangle  PARAMS((OBJECT *Object, VECTOR *Vector));
void Invert_Triangle  PARAMS((OBJECT *Object));
void Smooth_Triangle_Normal  PARAMS((VECTOR *Result, OBJECT *Object, VECTOR *Intersection_Point));
void *Copy_Smooth_Triangle PARAMS((OBJECT *Object));
void Translate_Smooth_Triangle  PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Smooth_Triangle  PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Smooth_Triangle  PARAMS((OBJECT *Object, VECTOR *Vector));
void Invert_Smooth_Triangle  PARAMS((OBJECT *Object));

/* Prototypes for functions defined in vect.c */
int solve_quadratic PARAMS((DBL *x, DBL *y));
int solve_cubic PARAMS((DBL *x, DBL *y));
int solve_quartic PARAMS((DBL *x, DBL *y));
int polysolve PARAMS((int order, DBL *Coeffs, DBL *roots));

/* Prototypes for functions defined in lighting.c */
void Colour_At PARAMS((COLOUR *Colour, TEXTURE *Texture, VECTOR *Intersection_Point));
void Perturb_Normal PARAMS((VECTOR *New_Normal, TEXTURE *Texture, VECTOR *Intersection_Point, VECTOR *Surface_Normal));
void Ambient PARAMS((TEXTURE *Texture, COLOUR *Surface_Colour, COLOUR *Colour, DBL Attenuation));
void Diffuse PARAMS((TEXTURE *Texture, VECTOR *Intersection_Point, RAY *Eye, VECTOR *Surface_Normal, COLOUR *Surface_Colour, COLOUR *Colour,DBL Attenuation));
void Reflect PARAMS((TEXTURE *Texture, VECTOR *Intersection_Point, RAY *Ray, VECTOR *Surface_Normal, COLOUR *Colour));
void Refract PARAMS((TEXTURE *Texture, VECTOR *Intersection_Point, RAY *Ray, VECTOR *Surface_Normal, COLOUR *Colour));
void Fog PARAMS((DBL Distance, COLOUR *Fog_Colour, DBL Fog_Distance, COLOUR *Colour));
void Compute_Reflected_Colour PARAMS ((RAY *Ray, TEXTURE *Texture, INTERSECTION *Ray_Intersection, COLOUR *Surface_Colour, COLOUR *Filter_Colour,COLOUR *Colour));
void Determine_Surface_Colour PARAMS ((INTERSECTION *Ray_Intersection, COLOUR *Colour, RAY *Ray, int Shadow_Ray));

/* Prototypes for functions defined in prioq.c */
void pq_init PARAMS((void));
PRIOQ *pq_alloc PARAMS((void));
void pq_free PARAMS((PRIOQ *pq));
PRIOQ *pq_new PARAMS((int index_size));
void pq_balance PARAMS((PRIOQ *q, unsigned int entry_pos1));
void pq_add PARAMS((PRIOQ *q, INTERSECTION *queue_entry));
INTERSECTION *pq_get_highest PARAMS((PRIOQ *q));
int pq_is_empty PARAMS((PRIOQ *q));
void pq_delete_highest PARAMS((PRIOQ *q));

/* Prototypes for functions defined in texture.c */
void Compute_Colour PARAMS((COLOUR *Colour,COLOUR_MAP *Colour_Map,DBL value));
void Initialize_Noise PARAMS((void));
void InitTextureTable PARAMS((void));
void InitRTable PARAMS((void));
int R PARAMS((VECTOR *v));
int Crc16 PARAMS((char *buf, int count));
void setup_lattice PARAMS((DBL *x, DBL *y, DBL *z, long *ix, long *iy, long *iz, long *jx, long *jy, long *jz, DBL *sx, DBL *sy, DBL *sz, DBL *tx, DBL *ty, DBL *tz));
DBL Noise PARAMS((DBL x, DBL y, DBL z));
void DNoise PARAMS((VECTOR *result, DBL x, DBL y, DBL z));
DBL cycloidal PARAMS((DBL value));
DBL Triangle_Wave PARAMS((DBL value));
DBL Turbulence PARAMS((DBL x, DBL y, DBL z, int octaves));
void DTurbulence PARAMS((VECTOR *result, DBL x, DBL y, DBL z, int octaves));
void Translate_Texture PARAMS((TEXTURE **Texture_Ptr, VECTOR *Vector));
void Rotate_Texture PARAMS((TEXTURE **Texture_Ptr, VECTOR *Vector));
void Scale_Texture PARAMS((TEXTURE **Texture_Ptr, VECTOR *Vector));


/* Prototypes for functions defined in txtcolor.c */
void agate PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));
void bozo PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));
void brick PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));
void checker PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));
void checker_texture PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));
void gradient PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));
void granite PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));
void marble PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));
void spotted PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));
void wood PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));
/* Two new textures from Scott Taylor ONION & LEOPARD 7/18/91*/
void leopard PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));   /* SWT 7/18/91 */
void onion PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));

/* Prototypes for functions defined in txtbump.c */
void bumps PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, VECTOR *normal));
void dents PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, VECTOR *normal));
void ripples PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, VECTOR *Vector));
void waves PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, VECTOR *Vector));
void wrinkles PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, VECTOR *normal));


/* Prototypes for functions defined in txttest.c */
void painted1 PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour)); /* CdW 7/2/91 */
void painted2 PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour)); /* CdW 7/2/91 */
void painted3 PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour)); /* CdW 7/2/91 */
void bumpy1 PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, VECTOR *normal)); /* CdW 7/2/91*/
void bumpy2 PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, VECTOR *normal)); /* CdW 7/2/91*/
void bumpy3 PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, VECTOR *normal)); /* CdW 7/2/91*/

/* Prototypes for functions defined in txtmap.c */
int map PARAMS((DBL x,DBL y,DBL z,TEXTURE *Texture,IMAGE *Image,DBL *xcoor, DBL *ycoor));
void image_map PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, COLOUR *colour));
TEXTURE *material_map PARAMS((VECTOR *Intersection_Point, TEXTURE *Texture));
void bump_map PARAMS((DBL x, DBL y, DBL z, TEXTURE *Texture, VECTOR *normal));/* CdW 7/8/91*/


/* Prototypes for functions defined in csg.c */
int All_CSG_Union_Intersections PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue));
int All_CSG_Intersect_Intersections PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue)); 
int Inside_CSG_Union PARAMS((VECTOR *point, OBJECT *Object));
int Inside_CSG_Intersection PARAMS((VECTOR *point, OBJECT *Object));
void *Copy_CSG PARAMS((OBJECT *Object));
void Translate_CSG PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_CSG PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_CSG PARAMS((OBJECT *Object, VECTOR *Vector));
void Invert_CSG PARAMS((OBJECT *Object)); 
void Set_CSG_Parents PARAMS((CSG_SHAPE *, OBJECT *));

/* Prototypes for functions defined in colour.c */
DBL Colour_Distance PARAMS((COLOUR *colour1, COLOUR *colour2));
void Add_Colour PARAMS((COLOUR *result, COLOUR *colour1, COLOUR *colour2));
void Scale_Colour PARAMS((COLOUR *result, COLOUR *colour, DBL factor));
void Clip_Colour PARAMS((COLOUR *result, COLOUR *colour)); 

/* Prototypes for functions defined in viewpnt.c */
void *Copy_Viewpoint PARAMS((OBJECT *Object));
void Translate_Viewpoint PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Viewpoint PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Viewpoint PARAMS((OBJECT *Object, VECTOR *Vector));

/* Prototypes for functions defined in ray.c */
void Make_Ray PARAMS((RAY *r));
void Initialize_Ray_Containers PARAMS((RAY *Ray));
void Copy_Ray_Containers PARAMS((RAY *Dest_Ray, RAY *Source_Ray));
void Ray_Enter PARAMS((RAY *ray, TEXTURE *texture));
void Ray_Exit PARAMS((RAY *ray));

/* Prototypes for functions defined in planes.c */
int All_Plane_Intersections PARAMS((OBJECT *Object, RAY *Ray, PRIOQ *Depth_Queue));
int Intersect_Plane PARAMS((RAY *Ray, PLANE *Plane, DBL *Depth));
int Inside_Plane PARAMS((VECTOR *point, OBJECT *Object));
void Plane_Normal PARAMS((VECTOR *Result, OBJECT *Object, VECTOR *Intersection_Point));
void *Copy_Plane PARAMS((OBJECT *Object));
void Translate_Plane PARAMS((OBJECT *Object, VECTOR *Vector));
void Rotate_Plane PARAMS((OBJECT *Object, VECTOR *Vector));
void Scale_Plane PARAMS((OBJECT *Object, VECTOR *Vector));
void Invert_Plane PARAMS((OBJECT *Object));

/* Prototypes for functions defined in iff.c */
void iff_error PARAMS((void));
int read_byte PARAMS((FILE *f));
int read_word PARAMS((FILE *f));
long read_long PARAMS((FILE *f));
void Read_Chunk_Header PARAMS((FILE *f, CHUNK_HEADER *dest)); 
void Read_Iff_Image PARAMS((IMAGE *Image, char *filename));

/* Prototypes for functions defined in gif.c */
int out_line PARAMS((unsigned char *pixels, int linelen));
int get_byte PARAMS((void));
void Read_Gif_Image PARAMS((IMAGE *Image, char *filename));

/* Prototypes for functions defined in gifdecod.c */
void cleanup_gif_decoder PARAMS((void));
WORD init_exp PARAMS((int i_size));   /* changed param to int to avoid
					 problems with 32bit int ANSI
					 compilers. */
WORD get_next_code PARAMS((void));
WORD decoder PARAMS((int i_linewidth)); /* same as above */

/* Prototypes for machine specific functions defined in "computer".c (ibm.c amiga.c unix.c etc.)*/
void display_finished PARAMS((void));
void display_init PARAMS((int width, int height));
void display_close PARAMS((void));
void display_plot PARAMS((int x, int y, unsigned char Red, unsigned char Green, unsigned char Blue));

/* Prototypes for functions defined in matrices.c */
void MZero PARAMS((MATRIX *result));
void MIdentity PARAMS((MATRIX *result));
void MTimes PARAMS((MATRIX *result, MATRIX *matrix1, MATRIX *matrix2));
void MAdd PARAMS((MATRIX *result, MATRIX *matrix1, MATRIX *matrix2));
void MSub PARAMS((MATRIX *result, MATRIX *matrix1, MATRIX *matrix2));
void MScale PARAMS((MATRIX *result, MATRIX *matrix1, DBL amount));
void MTranspose PARAMS((MATRIX *result, MATRIX *matrix1));
void MTransformVector PARAMS((VECTOR *result, VECTOR *vector, TRANSFORMATION *transformation));
void MInverseTransformVector PARAMS((VECTOR *result, VECTOR *vector, TRANSFORMATION *transformation));
void MTransVector PARAMS((VECTOR *result, VECTOR *vector, TRANSFORMATION *transformation));
void MInvTransVector PARAMS((VECTOR *result, VECTOR *vector, TRANSFORMATION *transformation));
void MTransNormal PARAMS((VECTOR *result, VECTOR *vector, TRANSFORMATION *transformation));
void Get_Scaling_Transformation PARAMS((TRANSFORMATION *result, VECTOR *vector));
void Get_Inversion_Transformation PARAMS((TRANSFORMATION *result));
void Get_Translation_Transformation PARAMS((TRANSFORMATION *transformation, VECTOR *vector));
void Get_Rotation_Transformation PARAMS((TRANSFORMATION *transformation, VECTOR *vector));
void Get_Look_At_Transformation PARAMS((TRANSFORMATION *transformation, VECTOR *Look_At, VECTOR *Up, VECTOR *Right));
void Compose_Transformations PARAMS((TRANSFORMATION *Original_Transformation, TRANSFORMATION *New_Transformation));

/* Prototypes for functions defined in dump.c */
FILE_HANDLE *Get_Dump_File_Handle PARAMS((void));
char *Default_Dump_File_Name PARAMS((void));
int Open_Dump_File PARAMS((FILE_HANDLE *handle, char *name,
int *width, int *height, int buffer_size, int mode));
void Write_Dump_Line PARAMS((FILE_HANDLE *handle, COLOUR *line_data, int line_number));
int Read_Dump_Line PARAMS((FILE_HANDLE *handle, COLOUR *line_data, int *line_number));
int Read_Dump_Int_Line PARAMS((FILE_HANDLE *handle, IMAGE_LINE *line_data, int *line_number));
void Read_Dump_Image PARAMS((IMAGE *Image, char *filename));
void Close_Dump_File PARAMS((FILE_HANDLE *handle));

/* Prototypes for functions defined in targa.c */
FILE_HANDLE *Get_Targa_File_Handle PARAMS((void));
char *Default_Targa_File_Name PARAMS((void));
int Open_Targa_File PARAMS((FILE_HANDLE *handle, char *name,
int *width, int *height, int buffer_size, int mode));
void Write_Targa_Line PARAMS((FILE_HANDLE *handle, COLOUR *line_data, int line_number));
int Read_Targa_Line PARAMS((FILE_HANDLE *handle, COLOUR *line_data, int *line_number));
void Read_Targa_Image PARAMS((IMAGE *Image, char *filename));
void Close_Targa_File PARAMS((FILE_HANDLE *handle));
int Read_Targa_Int_Line PARAMS((FILE_HANDLE *handle,IMAGE_LINE *line_data));

/* Prototypes for functions defined in Raw.c */
FILE_HANDLE *Get_Raw_File_Handle PARAMS((void));
char *Default_Raw_File_Name PARAMS((void));
int Open_Raw_File PARAMS((FILE_HANDLE *handle, char *name,
int *width, int *height, int buffer_size, int mode));
void Write_Raw_Line PARAMS((FILE_HANDLE *handle, COLOUR *line_data, int line_number));
int Read_Raw_Line PARAMS((FILE_HANDLE *handle, COLOUR *line_data, int *line_number));
void Close_Raw_File PARAMS((FILE_HANDLE *handle));
