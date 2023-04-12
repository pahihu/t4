/****************************************************************************
*                tokenize.c
*
*  This module implements the first part of a two part parser for the scene
*  description files.  This phase changes the input file into tokens.
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

#include <ctype.h>
#include "frame.h"
#include "povproto.h"

/* This module tokenizes the input file and sends the tokens created
to the parser (the second stage).  Tokens sent to the parser contain a
token ID, the line number of the token, and if necessary, some data for
the token.  */

#define MAX_STRING_INDEX 41
char String[MAX_STRING_INDEX];
int String_Index;
extern char Library_Path[];
extern int Stop_Flag;
static int pov_stricmp PARAMS ((char *s1,char *s2));

/* Here are the reserved words.  If you need to add new words, be sure
to declare them in frame.h */

struct Reserved_Word_Struct Reserved_Words [LAST_TOKEN] = {
   AGATE_TOKEN, "agate",
   ALL_TOKEN, "all",
   ALPHA_TOKEN, "alpha",
   AMBIENT_TOKEN, "ambient",
   AMPERSAND_TOKEN, "&",
   AT_TOKEN, "@",
   BACK_QUOTE_TOKEN, "`",
   BACK_SLASH_TOKEN, "\\",
   BAR_TOKEN, "|",
   BICUBIC_PATCH_TOKEN, "bicubic_patch",
   BLUE_TOKEN, "blue",
   BRICK_TOKEN, "brick",
   BRILLIANCE_TOKEN, "brilliance",
   BLOB_TOKEN, "blob",
   BOX_TOKEN, "box",
   BOZO_TOKEN, "bozo",
   BOUNDED_TOKEN, "bounded_by",
   BUMPS_TOKEN, "bumps",
   BUMPSIZE_TOKEN, "bump_size",
   BUMPMAP_TOKEN, "bump_map",
   BUMPY1_TOKEN, "bumpy1",
   BUMPY2_TOKEN, "bumpy2",
   BUMPY3_TOKEN, "bumpy3",  
   CENTER_TOKEN, "center",
   CHECKER_TOKEN, "checker",
   CHECKER_TEXTURE_TOKEN, "tiles",
   CLIPPED_TOKEN, "clipped_by",
   COLON_TOKEN, ":",
   COLOUR_TOKEN, "color",
   COLOUR_TOKEN, "colour",
   COLOUR_MAP_TOKEN, "color_map",
   COLOUR_MAP_TOKEN, "colour_map",
   COMMA_TOKEN, ",",
   COMPONENT_TOKEN, "component",
   COMPOSITE_TOKEN, "composite",
   CONCENTRATION_TOKEN, "concentration",
   CUBIC_TOKEN, "cubic",
   DASH_TOKEN, "-",
   DECLARE_TOKEN, "declare",
   DEFAULT_TOKEN, "default",
   DENTS_TOKEN, "dents",
   DIFFERENCE_TOKEN, "difference",
   DIFFUSE_TOKEN, "diffuse",
   DIRECTION_TOKEN, "direction",
   DOLLAR_TOKEN, "$",
   DUMP_TOKEN, "dump",
   END_OF_FILE_TOKEN, "End of File",
   EQUALS_TOKEN, "=",
   EXCLAMATION_TOKEN, "!",
   FALLOFF_TOKEN, "falloff",
   FLOAT_TOKEN, "float",
   FOG_TOKEN, "fog",
   FREQUENCY_TOKEN, "frequency",
   GIF_TOKEN, "gif",
   GRANITE_TOKEN, "granite",
   GRADIENT_TOKEN, "gradient",
   GREEN_TOKEN, "green",
   HASH_TOKEN, "#",
   HAT_TOKEN, "^",
   HEIGHT_FIELD_TOKEN, "height_field",
   IDENTIFIER_TOKEN, "identifier",
   IFF_TOKEN, "iff",
   IMAGEMAP_TOKEN, "image_map",
   INCLUDE_TOKEN, "include",
   INTERPOLATE_TOKEN, "interpolate",
   INTERSECTION_TOKEN, "intersection",
   INVERSE_TOKEN, "inverse",
   IOR_TOKEN, "ior",
   LEFT_ANGLE_TOKEN, "<",
   LEFT_CURLY_TOKEN, "{",
   LEFT_PAREN_TOKEN, "(",
   LEFT_SQUARE_TOKEN, "[",
   LEOPARD_TOKEN, "leopard",
   LIGHT_SOURCE_TOKEN, "light_source",
   LOCATION_TOKEN, "location",
   LOOK_AT_TOKEN, "look_at",
   MARBLE_TOKEN, "marble",
   MATERIAL_MAP_TOKEN, "material_map",
   MAPTYPE_TOKEN, "map_type",
   MAX_TRACE_LEVEL_TOKEN, "max_trace_level",
   METALLIC_TOKEN, "metallic",
   MORTAR_TOKEN, "mortar",
   NO_SHADOW_TOKEN, "no_shadow",
   OBJECT_TOKEN, "object",
   OCTAVES_TOKEN, "octaves",
   ONCE_TOKEN, "once",
   ONION_TOKEN, "onion",
   PAINTED1_TOKEN, "painted1",
   PAINTED2_TOKEN, "painted2",
   PAINTED3_TOKEN, "painted3",
   PERCENT_TOKEN, "%",
   PHASE_TOKEN, "phase",
   PHONG_TOKEN, "phong",
   PHONGSIZE_TOKEN, "phong_size",
   PLANE_TOKEN, "plane",
   PLUS_TOKEN, "+",
   POINTS_TOKEN, "points",
   POINT_AT_TOKEN, "point_at",
   POLY_TOKEN, "poly",
   POLYGON_TOKEN, "polygon",
   POT_TOKEN, "pot",
   QUADRIC_TOKEN, "quadric",
   QUARTIC_TOKEN, "quartic",
   QUESTION_TOKEN, "?",
   RADIUS_TOKEN, "radius",
   RAW_TOKEN, "raw",
   RED_TOKEN, "red",
   REFLECTION_TOKEN, "reflection",
   REFRACTION_TOKEN, "refraction",
   REVOLVE_TOKEN, "revolve",
   RIGHT_TOKEN, "right",
   RIGHT_CURLY_TOKEN, "}",
   RIGHT_ANGLE_TOKEN, ">",
   RIGHT_PAREN_TOKEN, ")",
   RIGHT_SQUARE_TOKEN, "]",
   RIPPLES_TOKEN, "ripples",
   ROTATE_TOKEN, "rotate",
   ROUGHNESS_TOKEN, "roughness",
   SCALE_TOKEN, "scale",
   SEMI_COLON_TOKEN, ";",
   SHAPE_TOKEN, "shape",
   SKY_TOKEN, "sky",
   SINGLE_QUOTE_TOKEN, "'",
   SIZE_TOKEN, "size",
   SLASH_TOKEN, "/",
   SMOOTH_TRIANGLE_TOKEN, "smooth_triangle",
   SPECULAR_TOKEN, "specular",
   SPHERE_TOKEN, "sphere",
   SPOTLIGHT_TOKEN, "spotlight",
   SPOTTED_TOKEN, "spotted",
   STAR_TOKEN, "*",
   STRING_TOKEN, "string",
   STURM_TOKEN, "sturm",
   TEXTURE_TOKEN, "texture",
   TGA_TOKEN, "tga",
   TIGHTNESS_TOKEN, "tightness",
   TILDE_TOKEN, "~",
   TILE2_TOKEN, "tile2",
   THRESHOLD_TOKEN, "threshold",
   TRANSLATE_TOKEN, "translate",
   TRANSMIT_TOKEN, "* disabled *", /* Transmit disabled 2/92 */
   TRIANGLE_TOKEN, "triangle",
   TURBULENCE_TOKEN, "turbulence",
   UNION_TOKEN, "union",
   UP_TOKEN, "up",
   USE_COLOUR_TOKEN,"use_color",
   USE_COLOUR_TOKEN,"use_colour",
   USE_INDEX_TOKEN,"use_index",
   VIEW_POINT_TOKEN, "camera",
   WATER_LEVEL_TOKEN, "water_level",
   WAVES_TOKEN, "waves",
   WOOD_TOKEN, "wood",
   WRINKLES_TOKEN, "wrinkles", 
   LAST_TOKEN, "orville" 
};

/* Make a table for user-defined symbols.  500 symbols should be more
than enough. */

/* Now defined in POVRAY.c */
/* #define MAX_SYMBOLS 500 */
extern int Max_Symbols;

int token_count = 0, line_count = 0; /* moved here to allow reinitialization */

char **Symbol_Table;
int Number_Of_Symbols;
extern int Case_Sensitive_Flag; /* defined & init in povray.c */

#define MAX_INCLUDE_FILES 10

DATA_FILE Include_Files[MAX_INCLUDE_FILES];
int Include_File_Index;
DATA_FILE *Data_File;

struct Token_Struct Token;

#define CALL(x) { if (!(x)) return (FALSE); }

void Initialize_Tokenizer(filename)
char *filename;
{
   Symbol_Table = NULL;
   Data_File = NULL;

   Include_File_Index = 0;
   Data_File = &Include_Files[0];

   Data_File->File = Locate_File (filename, "r");
   if (Data_File->File == NULL) {
      fprintf (stderr, "Cannot open input file\n");
      exit(1);
   }

   Data_File->Filename = malloc(strlen(filename) + 1);
   strcpy (Data_File->Filename, filename);
   Data_File->Line_Number = 0;


   if ((Symbol_Table = (char **) malloc (Max_Symbols * sizeof (char *))) == NULL) {
      fprintf(stderr, "Out of Memory. Cannot allocate space for symbol table\n");
      exit(1);
   }

   Token.End_Of_File = FALSE;
   Number_Of_Symbols = 0;
}


void Terminate_Tokenizer()
{
   int i;

   if (Symbol_Table != NULL) {
      for (i = 1 ; i < Number_Of_Symbols ; i++)
         free(Symbol_Table[i]);

      free (Symbol_Table);
   }

   if (Data_File != NULL) {
      fclose (Data_File->File);
      free (Data_File->Filename);
   }
}

/* The main tokenizing routine.  Set up the files and continue parsing
until the end of file */

/* This function performs most of the work involved in tokenizing.  It
   reads the first character of the token and decides which function to
   call to tokenize the rest.  For simple tokens, it simply writes them
   out to the token buffer.  */

/* Read a token from the input file and store it in the Token variable.
If the token is an INCLUDE token, then set the include file name and
read another token. */

void Get_Token ()
{
   register int c,c2;
   COOPERATE
   if(Stop_Flag){
      close_all();
      exit(1);
   }
   if (Token.Unget_Token)
   {
      Token.Unget_Token = FALSE;
      return;
   }

   if (Token.End_Of_File)
      return;

   Token.Token_Id = END_OF_FILE_TOKEN;

   while (Token.Token_Id == END_OF_FILE_TOKEN) {

      Skip_Spaces (Data_File);

      c = getc(Data_File->File);
      if (c == EOF) {
         if (Include_File_Index == 0) {
            Token.Token_Id = END_OF_FILE_TOKEN;
            Token.End_Of_File = TRUE;
            /*putchar ('\n');*/
            fprintf (stderr,"\n");
            return;
         }

         fclose(Data_File->File); /* added to fix open file buildup JLN 12/91 */

         Data_File = &Include_Files[--Include_File_Index];
         continue;
      }

      String[0] = '\0';
      String_Index  = 0;

      switch (c)
      {
      case '\n': Data_File->Line_Number++;
         break;

      case '{' : Write_Token (LEFT_CURLY_TOKEN, Data_File);
         /* Parse_Comments(Data_File); */
         break;

      case '}' : Write_Token (RIGHT_CURLY_TOKEN, Data_File);
         break;                                    

      case '@' : Write_Token (AT_TOKEN, Data_File);
         break;

      case '&' : Write_Token (AMPERSAND_TOKEN, Data_File);
         break;

      case '`' : Write_Token (BACK_QUOTE_TOKEN, Data_File);
         break;

      case '\\': Write_Token (BACK_SLASH_TOKEN, Data_File);
         break;

      case '|' : Write_Token (BAR_TOKEN, Data_File);
         break;

      case ':' : Write_Token (COLON_TOKEN, Data_File);
         break;

      case ',' : Write_Token (COMMA_TOKEN, Data_File);
         break;

      case '-' : Write_Token (DASH_TOKEN, Data_File);
         break;

      case '$' : Write_Token (DOLLAR_TOKEN, Data_File);
         break;

      case '=' : Write_Token (EQUALS_TOKEN, Data_File);
         break;

      case '!' : Write_Token (EXCLAMATION_TOKEN, Data_File);
         break;

      case '#' : /* Parser doesn't use it, so let's ignore it */
         /* Write_Token (HASH_TOKEN, Data_File); */
         break;

      case '^' : Write_Token (HAT_TOKEN, Data_File);
         break;

      case '<' : Write_Token (LEFT_ANGLE_TOKEN, Data_File);
         break;

      case '(' : Write_Token (LEFT_PAREN_TOKEN, Data_File);
         break;

      case '[' : Write_Token (LEFT_SQUARE_TOKEN, Data_File);
         break;

      case '%' :  Write_Token (PERCENT_TOKEN, Data_File);
         break;

      case '+' : Write_Token (PLUS_TOKEN, Data_File);
         break;

      case '?' : Write_Token (QUESTION_TOKEN, Data_File);
         break;

      case '>' : Write_Token (RIGHT_ANGLE_TOKEN, Data_File);
         break;

      case ')' : Write_Token (RIGHT_PAREN_TOKEN, Data_File);
         break;

      case ']' : Write_Token (RIGHT_SQUARE_TOKEN, Data_File);
         break;

      case ';' : /* Parser doesn't use it, so let's ignore it */
         /* Write_Token (SEMI_COLON_TOKEN, Data_File); */
         break;

      case '\'': Write_Token (SINGLE_QUOTE_TOKEN, Data_File);
         break;

         /* enable C++ style commenting */
      case '/' : 	c2 = getc(Data_File->File);
         if(c2 != (int) '/' && c2 != (int) '*'){
            ungetc(c2,Data_File->File);
            Write_Token (SLASH_TOKEN, Data_File);
            break;
         }          
         if(c2 == (int)'*'){
            Parse_C_Comments(Data_File);
            break;
         }           
         while(c2 != (int)'\n'){
            c2=getc(Data_File->File);
            if(c2==EOF){
               ungetc(c2,Data_File->File);
               break;
            }
         }
         Data_File->Line_Number++;
         break;

      case '*' : Write_Token (STAR_TOKEN, Data_File);
         break;

      case '~' : Write_Token (TILDE_TOKEN, Data_File);
         break;

      case '"' : Parse_String (Data_File);
         break;

      case '0':   case '1':   case '2':   case '3':   case '4':   case '5':
      case '6':   case '7':   case '8':   case '9':   case '.':
         ungetc (c, Data_File->File);
         if (Read_Float (Data_File) != TRUE)
            return;
         break;

      case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
      case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
      case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
      case 'v': case 'w': case 'x': case 'y': case 'z':
      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
      case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
      case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
      case 'V': case 'W': case 'X': case 'Y': case 'Z': case '_':
         ungetc (c, Data_File->File);
         if (Read_Symbol (Data_File) != TRUE)
            return;
         break;
      case '\t':
      case '\r':
      case '\032':   /* Control Z - EOF on many systems */
      case '\0': break;

      default:
         fprintf (stderr, "Error in %s line %d\n", Data_File->Filename, Data_File->Line_Number+1);
         fprintf (stderr, "Illegal character in input file, value is %02x\n", c);
         break;
      }
      if (Token.Token_Id == INCLUDE_TOKEN)
      {
         if (Skip_Spaces (Data_File) != TRUE)
            Token_Error (Data_File, "Expecting a string after INCLUDE\n");

         if ((c = getc(Data_File->File)) != '"')
            Token_Error (Data_File, "Expecting a string after INCLUDE\n");

         Parse_String(Data_File);
         Include_File_Index++;
         if (Include_File_Index > MAX_INCLUDE_FILES)
            Token_Error (Data_File, "Too many nested include files\n");

         Data_File = &Include_Files[Include_File_Index];
         Data_File->Line_Number = 0;

         Data_File->Filename = malloc(strlen(Token.Token_String) + 1);
         if (Data_File->Filename == NULL) {
            fprintf (stderr, "Out of memory opening include file: %s\n",
               Token.Token_String);
            exit (1);
         }

         strcpy (Data_File->Filename, Token.Token_String);

         if ((Data_File->File = Locate_File (Token.Token_String, "r")) == NULL) {
            fprintf (stderr, "Cannot open include file: %s\n", Token.Token_String);
            exit(1);
         }
         Token.Token_Id = END_OF_FILE_TOKEN;
      }

   }

   token_count++;
   if (token_count > 1000) {
      token_count = 0;
      fprintf(stderr,".");
      fflush(stderr);
      line_count++;
      if (line_count > 78) {
         line_count = 0;
         fprintf (stderr,"\n");
      }
   }
   return;
}


/* Mark that the token has been put back into the input stream.  The next
call to Get_Token will return the last-read token instead of reading a
new one from the file. */

void Unget_Token ()
{
   Token.Unget_Token = TRUE;
}

/* Skip over spaces in the input file */

int Skip_Spaces (Data_File)
DATA_FILE *Data_File;
{
   register int c;

   while (TRUE)
   {
      c = getc(Data_File->File);
      if (c == EOF)
         return (FALSE);

      if (!(isspace(c) || c == 0x0A))
         break;

      if (c == '\n')
         Data_File->Line_Number++;
   }

   ungetc (c, Data_File->File);
   return (TRUE);
}
/* The comments on comments are outdated and incorrect */
/* Comments start with an open brace ({) and end with a close brace (}).
   The open brace has been read already.  Continue reading until a close
   brace is encountered. Be sure to count the lines while you're at it.
   Incidently, nested comments are supported (in case you do such esoteric
   things) */

int Parse_Comments (Data_File)
DATA_FILE *Data_File;
{
   register int c;
   int End_Of_Comment;

   End_Of_Comment = FALSE;
   while (!End_Of_Comment)
   {
      c = getc (Data_File->File);
      if (c == EOF)
      {
         Token_Error (Data_File, "No closing comment found");
         return (FALSE);
      }

      if (c == (int) '\n')
         Data_File->Line_Number++;

      if (c == (int) '{')
         CALL (Parse_Comments(Data_File))
else
   End_Of_Comment = (c == (int) '}');
}

return (TRUE);
}

/* C style comments with asterik and slash - CdW 8/91 */

int Parse_C_Comments (Data_File)
DATA_FILE *Data_File;
{
register int c,c2;
int End_Of_Comment;

End_Of_Comment = FALSE;
while (!End_Of_Comment)
{
c = getc (Data_File->File);
if (c == EOF)
{
   Token_Error (Data_File, "No */ closing comment found");
   return (FALSE);
}

if (c == (int) '\n')
Data_File->Line_Number++;

if (c == (int) '*'){
   c2= getc(Data_File->File);
   if(c2 != (int) '/')
      ungetc(c2,Data_File->File);
   else
      End_Of_Comment = TRUE;
}
/* Check for and handle nested comments */
if (c == (int) '/'){
   c2= getc(Data_File->File);
   if(c2 != (int) '*')
      ungetc(c2,Data_File->File);
   else 
      Parse_C_Comments(Data_File);
}
}
return (TRUE);
}

/* The following routines make it easier to handle strings.  They stuff
   characters into a string buffer one at a time making all the proper
   range checks.  Call Begin_String to start, Stuff_Character to put
   characters in, and End_String to finish.  The String variable contains
   the final string. */

void Begin_String()
{
String_Index = 0;
}

void Stuff_Character (c, Data_File)
int c;
DATA_FILE *Data_File;
{
if (String_Index < MAX_STRING_INDEX)
{
String [String_Index++] = (char) c;
if (String_Index >= MAX_STRING_INDEX)
{
   Token_Error (Data_File, "String too long");
   String [String_Index-1] = '\0';
}
}
}

void End_String (Data_File)
DATA_FILE *Data_File;
{
Stuff_Character ((int) '\0', Data_File);
}

/* Read a float from the input file and tokenize it as one token. The phase
   variable is 0 for the first character, 1 for all subsequent characters
   up to the decimal point, 2 for all characters after the decimal
   point, 3 for the E+/- and 4 for the exponent.  This helps to insure
   that the number is formatted properly. E format added 9/91 CEY */

int Read_Float(Data_File)
DATA_FILE *Data_File;
{
register int c, Finished, Phase;

Finished = FALSE;
Phase = 0;

Begin_String();
while (!Finished)
{
c = getc(Data_File->File);
if (c == EOF)
{
   Token_Error (Data_File, "Unexpected end of file");
   return (FALSE);
}

switch (Phase)
{
case 0: if (isdigit(c))
      Stuff_Character(c, Data_File);
   else
      if (c == '.') {
         Stuff_Character('0', Data_File);
         ungetc (c, Data_File->File);
      }
      else
         Token_Error (Data_File, "Error in decimal number");
   Phase = 1;
   break;

case 1: if (isdigit(c))
      Stuff_Character(c, Data_File);
   else if (c == (int) '.')
   {Stuff_Character(c, Data_File); Phase = 2;
   }
   else
      if ((c == 'e') || (c == 'E'))
      {Stuff_Character(c, Data_File); Phase = 3;
      }
      else Finished = TRUE;
   break;

   case 2: if (isdigit(c))
         Stuff_Character(c, Data_File);
      else if ((c == 'e') || (c == 'E'))
      {Stuff_Character(c, Data_File); Phase = 3;
      }
      else Finished = TRUE;
   break;

   case 3: if (isdigit(c) || (c == '+') || (c == '-'))
      {Stuff_Character(c, Data_File); Phase = 4;
      }
      else Finished = TRUE;
   break;

   case 4: if (isdigit(c))
         Stuff_Character(c, Data_File);
      else Finished = TRUE;
   break;
}
}

ungetc (c, Data_File->File);
End_String(Data_File);

Write_Token (FLOAT_TOKEN, Data_File);
if (sscanf (String, DBL_FORMAT_STRING, &Token.Token_Float) == 0)
return (FALSE);

return (TRUE);
}

/* Parse a string from the input file into a token. */
void Parse_String (Data_File)
DATA_FILE *Data_File;
{
register int c;

Begin_String();
while (TRUE)
{
c = getc(Data_File->File);
if (c == EOF)
Token_Error (Data_File, "No end quote for string");

if (c != (int) '"')
Stuff_Character (c, Data_File);
else
   break;
}
End_String(Data_File);

Write_Token (STRING_TOKEN, Data_File);
Token.Token_String = String;
}

/* Read in a symbol from the input file.  Check to see if it is a reserved
   word.  If it is, write out the appropriate token.  Otherwise, write the
   symbol out to the Symbol file and write out an IDENTIFIER token. An
   Identifier token is a token whose token number is greater than the
   highest reserved word. */

int Read_Symbol (Data_File)
DATA_FILE *Data_File;
{
register int c, Symbol_Id;

Begin_String();
while (TRUE)
{
c = getc(Data_File->File);
if (c == EOF)
{
   Token_Error (Data_File, "Unexpected end of file");
   return (FALSE);
}

if (isalpha(c) || isdigit(c) || c == (int) '_')
Stuff_Character (c, Data_File);
else
{
   ungetc (c, Data_File->File);
   break;
}
}
End_String(Data_File);

/* Ignore the symbol if it was meant for the tokenizer (-2) */  
if ((Symbol_Id = Find_Reserved()) != -1 && Symbol_Id != -2)
Write_Token (Symbol_Id, Data_File);
else
{
/* Ignore the symbol if it was meant for the tokenizer (-2) */  
if (Symbol_Id==-2)
return(TRUE);

if ((Symbol_Id = Find_Symbol()) == -1) {
   if (++Number_Of_Symbols < Max_Symbols) {
      if ((Symbol_Table[Number_Of_Symbols] = malloc (strlen(String)+1)) == NULL)
         Token_Error (Data_File, "Out of memory. Cannot allocate space for identifier");

      strcpy (Symbol_Table[Number_Of_Symbols], String);
      Symbol_Id = Number_Of_Symbols;
   }
   else {
      fprintf (stderr, "\nToo many symbols. Use +ms### option to raise Max_Symbols.\n");
      exit(1);
   }
}

   Write_Token (LAST_TOKEN + Symbol_Id, Data_File);
}

return (TRUE);
}

/* Return the index the token in the reserved words table or -1 if it
   isn't there. */

int Find_Reserved ()
{
register int i;

if (pov_stricmp ("case_sensitive_yes", &(String[0])) == 0){
Case_Sensitive_Flag = 0;
return (-2);
}
else if (pov_stricmp ("case_sensitive_no", &(String[0])) == 0){
Case_Sensitive_Flag = 1;
return (-2);
}
/* The optional case sensitive option only checks keywords unsensitive */
/* Symbols can be upper/lower, but not be the same as a keyword */  
else if (pov_stricmp ("case_sensitive_opt", &(String[0])) == 0){
Case_Sensitive_Flag = 2;
return (-2);
}

for (i = 0 ; i < LAST_TOKEN ; i++)
if(Case_Sensitive_Flag==0){
   if (strcmp (Reserved_Words[i].Token_Name, &(String[0])) == 0)
      return (Reserved_Words[i].Token_Number);
}
else{ 
if (pov_stricmp (Reserved_Words[i].Token_Name, &(String[0])) == 0)
return (Reserved_Words[i].Token_Number);
} 

return (-1);
}

/* Check to see if a symbol already exists with this name.  If so, return
    its symbol ID. */

int Find_Symbol ()
{
register int i;

for (i = 1 ; i <= Number_Of_Symbols ; i++)
if(Case_Sensitive_Flag == 0 || Case_Sensitive_Flag == 2)
if (strcmp (Symbol_Table[i], String) == 0)
return (i);
else if (Case_Sensitive_Flag == 1)
if (pov_stricmp (Symbol_Table[i], String) == 0)
return (i);

return (-1);
}

/* Write a token out to the token file */
void Write_Token (Token_Id, Data_File)
TOKEN Token_Id;
DATA_FILE *Data_File;
{
Token.Token_Id = Token_Id;
Token.Token_Line_No = Data_File->Line_Number;
Token.Filename = Data_File->Filename;
Token.Token_String = String;

if (Token.Token_Id > LAST_TOKEN)
{
Token.Identifier_Number = (int) Token.Token_Id - (int) LAST_TOKEN;
Token.Token_Id = IDENTIFIER_TOKEN;
}
}

/* Report an error */
void Token_Error (Data_File, str)
DATA_FILE *Data_File;
char *str;
{
fprintf(stderr, "Error in %s line %d\n", Data_File->Filename, Data_File->Line_Number);
fputs(str, stderr);
fputs("\n\n", stderr);
exit(1);
}
/* Since the stricmp function isn't available on all systems, we've
   provided a simplified version of it here. */

static int pov_stricmp (s1, s2)
char *s1, *s2;
{
char c1, c2;

while ((*s1 != '\0') && (*s2 != '\0')) {
c1 = *s1++;
c2 = *s2++;
if (islower(c1))
c1 = (char) toupper(c1);
if (islower(c2))
c2 = (char) toupper(c2);
if (c1 < c2)
return (-1);
if (c1 > c2)
return (1);
}

if (*s1 == '\0')
if (*s2 == '\0')
return (0);
else
return (-1);
else
return (1);
}
