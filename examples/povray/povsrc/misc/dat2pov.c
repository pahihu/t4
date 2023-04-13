/*
 *
 * cnvdat.c
 * Last Update 6/10/92 - CdW
 * A small program to convert DKBTrace and POV-Ray Beta V0.51 to POV-Ray V1.0
 * Light sources are not converted and must be done by hand.
 * Convert all uppercase keywords to lowercase.
 * Converts INCLUDE to #include.
 * Converts DECLARE to #declare.
 * Should work as a base for more sophisticated conversions.
 *
 */

#include <stdio.h>
#include <string.h>

#define FALSE 0
#define TRUE 1
#define MAX_STRING_INDEX 41
char String[MAX_STRING_INDEX];
char End_Type[MAX_STRING_INDEX];
int String_Index;
char *start_block_words[] = {
 "BICUBIC_PATCH",
 "BOUNDED_BY",
 "BUMPMAP",
 "BUMP_MAP",
 "CHECKER_TEXTURE",
 "CLIPPED_BY",
 "COLOR_MAP",
 "COLOUR_MAP",
 "COMPOSITE",
 "CUBIC",
 "DIFFERENCE",
 "FOG",
 "HEIGHT_FIELD",
 "IMAGEMAP",
 "IMAGE_MAP",
 "INTERSECTION",
 /*"LIGHT_SOURCE",*/
 "MATERIAL_MAP",
 "OBJECT",
 "PLANE",
 "POINTS",
 "POLY",
 "POLYGON",
 "QUADRIC",
 "QUARTIC",
 "SHAPE",
 "SMOOTH_TRIANGLE",
 "SPHERE",
 "TEXTURE",
 "TRIANGLE",
 "UNION",
 "VIEW_POINT",
  NULL
  };

int Read_Write_Symbol (FILE *Data_File);
void Begin_String();
void Stuff_Character (int c);
void End_String ();
void Convert_String();
int num_light = 0;
main()
{ 
 int c;  
 int nest_level = 0; 
 int exit = FALSE;
   
  fprintf(stderr,"DKBTrace\\POV-Ray Ver .5 to POV-Ray Ver 1.0 Converter\n");
  fprintf(stderr,"------------------------------------------------------\n");
  fprintf(stderr,"Usage: dat2pov < filename.dat > filename.pov\n");
  fprintf(stderr,"Light sources must be converted by hand. See dat2pov.doc!\n");
  
  fprintf(stdout,"// Persistence of Vision Raytracer\n");
  while(TRUE)
  {
    c = getc(stdin);
    if(c==EOF){
      fprintf(stderr,"Done Converting. End of File.\n");
      break; 
    }
    /* Handle Comments */
    if(c == '{'){
      fprintf(stdout,"/*");
      exit = FALSE;
      while(!exit) {
        c = getc(stdin);
        switch(c) {
          case '{':
            nest_level++;
            fprintf(stdout,"/*");
            break;
          case '}':
            nest_level--;
            if(nest_level == -1){
              nest_level = 0;
              fprintf(stdout,"*/");
              exit = TRUE;
            }
            else
              fprintf(stdout,"*/");  
            break;
          default:
            putc(c,stdout);
            break;
        }  
      }    
     continue;
    } 
    
    
    /* KLUDGE - Skip over # since we write them later */
    if(c != '#')
     if(isalpha(c) || c == '_' ){
      ungetc(c,stdin);
      if (Read_Write_Symbol(stdin) == FALSE)
        break;
      }
     else
      putc(c,stdout);
  }
}   
   
/* Read in a symbol from the input file.  Check to see if it is a reserved
   word.  If it is, write out the appropriate token.  Otherwise, write the
   symbol out to the Symbol file and write out an IDENTIFIER token. An
   Identifier token is a token whose token number is greater than the
   highest reserved word. */

int Read_Write_Symbol (Data_File)
  FILE *Data_File;
  {
  register int c, Symbol_Id,i;
  
  Begin_String();
  while (TRUE)
    {
    c = getc(Data_File);
    if (c == EOF)
      {
      fprintf(stderr, "Unexpected end of file");
      return (FALSE);
      }

    if (isalpha(c) || isdigit(c) || c == (int) '_' || c == (int) '.')
      Stuff_Character (c);
    else
      {
      ungetc (c, Data_File);
      break;
      }
    }
  End_String(Data_File);
  
  /* If symbol is all uppercase convert it to lower */
  Convert_String();
  i=0;
  while(start_block_words[i]){
    if(!stricmp(start_block_words[i],String)){
      strcat(String," {");
      break;  
    }
    i++;
  }       
  if(!stricmp(String,"colors.dat"))
    strcpy(String,"colors.inc");

  if(!stricmp(String,"shapes.dat"))
    strcpy(String,"shapes.inc");

  if(!stricmp(String,"shapesq.dat"))
    strcpy(String,"shapesq.inc");

  if(!stricmp(String,"textures.dat"))
    strcpy(String,"textures.inc");

  if(!strcmp(String,"include") || !strcmp(String,"declare")) 
    putc('#',stdout);
  
  if(!strcmp(String,"checker_texture {") )
    strcpy(String,"tiles {");    
    
  if(!strcmp(String,"imagemap {") )
    strcpy(String,"image_map {");    
  if(!strcmp(String,"bumpmap {") )
    strcpy(String,"bump_map {");    
  if(!strcmp(String,"phongsize") )
    strcpy(String,"phong_size");    
  if(!strcmp(String,"bumpsize") )
    strcpy(String,"bump_size");    
  if(!strcmp(String,"view_point {") )
    strcpy(String,"camera {");    
  
   
  
  if(!strcmp(String,"CRed") ) 
    strcpy(String,"Red");
  if(!strcmp(String,"CGreen") )
    strcpy(String,"Green");
  if(!strcmp(String,"CBlue") )
    strcpy(String,"Blue");
  if(!strcmp(String,"QSphere"))
    strcpy(String,"Ellipsoid");
  if(!strcmp(String,"Sphere"))
    strcpy(String,"Ellipsoid");
  
  if(!strcmp(String,"X_Disk"))
    strcpy(String,"Disk_X");
  if(!strcmp(String,"Y_Disk"))
    strcpy(String,"Disk_Y");
  if(!strcmp(String,"Z_Disk"))
    strcpy(String,"Disk_Z");
  
  if(!strcmp(String,"Cone_X"))
    strcpy(String,"QCone_X");
  if(!strcmp(String,"Cone_Y"))
    strcpy(String,"QCone_Y");
 if(!strcmp(String,"Cone_Z"))
    strcpy(String,"QCone_Z");
     
  
  if(!strnicmp(String,"end_",4)){
    strcpy(End_Type,String);
    sprintf(String," }");
    /* Use this line to make extra comments available */
     #ifdef XTRA_COMMENTS
     sprintf(String," }/* %s */",End_Type); 
     #endif
    }
  
  if(!stricmp(String,"light_source"))
    fprintf(stderr,"%d Light Sources\n",++num_light);
    
  
  fputs(String,stdout);
   
  return (TRUE);
  }

void Begin_String()
  {
  String_Index = 0;
  }

void Stuff_Character (c)
  int c;
  {
  if (String_Index < MAX_STRING_INDEX)
    {
    String [String_Index++] = (char) c;
    if (String_Index >= MAX_STRING_INDEX)
      {
      String [String_Index-1] = '\0';
      fprintf(stderr, "String too long  %s",String);
      }
    }
  }
void End_String ()
  {
  Stuff_Character ((int) '\0');
  }

/* If symbol is all uppercase convert it to lower */
void Convert_String()
{
  char *tmpstr;    
  
  tmpstr = String;
 
/* check for any lower case*/ 
  while(*tmpstr)
    if(islower(*tmpstr++))
      return;
/* cnv all upper to lower */ 
 tmpstr = String;
  while(*tmpstr){
    *tmpstr = tolower(*tmpstr);
     tmpstr++;
     }
}

  
