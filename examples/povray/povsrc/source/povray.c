/****************************************************************************
*                povray.c
*
*  This module contains the entry routine for the raytracer and the code to
*  parse the parameters on the command line.
*
*  from Persistence of Vision Raytracer 
*       Copyright 1992 Persistence of Vision Team
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
#include <time.h>          /* BP */
#include "frame.h"              /* common to ALL modules in this program */
#include "povproto.h"

#define MAX_FILE_NAMES 1
unsigned int Options;
int Quality;
int Case_Sensitive_Flag = CASE_SENSITIVE_DEFAULT;

FILE *bfp;

extern FRAME Frame;

char Input_File_Name[FILE_NAME_LENGTH], Output_File_Name[FILE_NAME_LENGTH], Stat_File_Name[FILE_NAME_LENGTH];

#define MAX_LIBRARIES 10
char *Library_Paths[MAX_LIBRARIES];
int Library_Path_Index;

int Max_Symbols   = 500;

FILE_HANDLE *Output_File_Handle;
int File_Buffer_Size;
static int Number_Of_Files;
static int inflag, outflag; 
DBL VTemp;
DBL Antialias_Threshold;
int First_Line, Last_Line;
int Display_Started = FALSE;
int Shadow_Test_Flag = FALSE;

/* Stats kept by the ray tracer: */
long Number_Of_Pixels, Number_Of_Rays, Number_Of_Pixels_Supersampled;
long Ray_Sphere_Tests, Ray_Sphere_Tests_Succeeded;
long Ray_Box_Tests, Ray_Box_Tests_Succeeded;
long Ray_Blob_Tests, Ray_Blob_Tests_Succeeded;
long Ray_Plane_Tests, Ray_Plane_Tests_Succeeded;
long Ray_Triangle_Tests, Ray_Triangle_Tests_Succeeded;
long Ray_Quadric_Tests, Ray_Quadric_Tests_Succeeded;
long Ray_Poly_Tests, Ray_Poly_Tests_Succeeded;
long Ray_Bicubic_Tests, Ray_Bicubic_Tests_Succeeded;
long Ray_Ht_Field_Tests, Ray_Ht_Field_Tests_Succeeded;
long Ray_Ht_Field_Box_Tests, Ray_HField_Box_Tests_Succeeded;
long Bounding_Region_Tests, Bounding_Region_Tests_Succeeded;
long Clipping_Region_Tests, Clipping_Region_Tests_Succeeded;
long Calls_To_Noise, Calls_To_DNoise;
long Shadow_Ray_Tests, Shadow_Rays_Succeeded;
long Reflected_Rays_Traced, Refracted_Rays_Traced;
long Transmitted_Rays_Traced;
time_t tstart, tstop;
DBL    tused;             /* Trace timer variables. - BP */ 

char DisplayFormat, OutputFormat, VerboseFormat, PaletteOption, Color_Bits;






#ifdef NOCMDLINE        /* a main() by any other name... */
#ifdef ALTMAIN
void alt_main()
#else
void main()
#endif
#else
#ifdef ALTMAIN
void alt_main(argc, argv)
#else
void main(argc, argv)
#endif
int argc;
char **argv;
#endif                  /* ...would be a lot less hassle!! :-) AAC */
{
   register int i;
   FILE *stat_file;

   STARTUP_POVRAY

   PRINT_CREDITS

   PRINT_OTHER_CREDITS

   /* Parse the command line parameters */
#ifndef NOCMDLINE
   if (argc == 1)
      usage();
#endif

   init_vars();

   Output_File_Name[0]='\0';

   Library_Paths[0] = NULL;
   Library_Path_Index = 0;

   /* Read the default parameters from povray.def */
   get_defaults();

#ifndef NOCMDLINE
   for (i = 1 ; i < argc ; i++ )
      if ((*argv[i] == '+') || (*argv[i] == '-'))
         parse_option(argv[i]);
      else
         parse_file_name(argv[i]);
#endif

   if (Last_Line == -1)
      Last_Line = Frame.Screen_Height;

   if (Options & DISKWRITE) {
      switch (OutputFormat) {
      case '\0':
      case 'd':
      case 'D':
         if ((Output_File_Handle = Get_Dump_File_Handle()) == NULL) {
            close_all();
            exit(1);
         }
         break;
         /*
         case 'i':
         case 'I':
                   if ((Output_File_Handle = Get_Iff_File_Handle()) == NULL) {
                      close_all();
                      exit(1);
                      }
                   break;

*/
      case 'r':
      case 'R':
         if ((Output_File_Handle = Get_Raw_File_Handle()) == NULL) {
            close_all();
            exit(1);
         }
         break;

      case 't':
      case 'T':
         if ((Output_File_Handle = Get_Targa_File_Handle()) == NULL) {
            close_all();
            exit(1);
         }
         break;
      default:
         fprintf (stderr, "Unrecognized output file format %c\n", OutputFormat);
         exit(1);
      }
      if (Output_File_Name[0] == '\0')
         strcpy (Output_File_Name, Default_File_Name (Output_File_Handle));
   }

   Print_Options();

   Initialize_Tokenizer(Input_File_Name);
   fprintf (stderr,"Parsing...");
   if (Options & VERBOSE_FILE){
      stat_file = fopen(Stat_File_Name,"w+t");
      fprintf (stat_file, "Parsing...\n");
      fclose(stat_file);
   }
   Parse (&Frame);
   Terminate_Tokenizer();
   /* fprintf (stderr,"\n"); */

   if (Options & DISPLAY)
   {
      printf ("Displaying...\n");
      display_init(Frame.Screen_Width, Frame.Screen_Height);
      Display_Started = TRUE;
   }

   /* Get things ready for ray tracing */
   if (Options & DISKWRITE)
      if (Options & CONTINUE_TRACE) {
         if (Open_File (Output_File_Handle, Output_File_Name,
            &Frame.Screen_Width, &Frame.Screen_Height, File_Buffer_Size,
            READ_MODE) != 1) {
            fprintf (stderr, "Error opening continue trace output file\n");
            fprintf (stderr, "Opening new output file %s.\n",Output_File_Name);
            Options &= ~CONTINUE_TRACE; /* Turn off continue trace */

            if (Open_File (Output_File_Handle, Output_File_Name,
               &Frame.Screen_Width, &Frame.Screen_Height, File_Buffer_Size,
               WRITE_MODE) != 1) {
               fprintf (stderr, "Error opening output file\n");
               close_all();
               exit(1);
            }
         }

         Initialize_Renderer();
         if (Options & CONTINUE_TRACE) 
            Read_Rendered_Part();
      }
      else {
         if (Open_File (Output_File_Handle, Output_File_Name,
            &Frame.Screen_Width, &Frame.Screen_Height, File_Buffer_Size,
            WRITE_MODE) != 1) {
            fprintf (stderr, "Error opening output file\n");
            close_all();
            exit(1);
         }

         Initialize_Renderer();
      }
   else
      Initialize_Renderer();

   pq_init();
   Initialize_Noise();

   START_TIME  /* Store start time for trace. Timer macro in CONFIG.H */

   /* Ok, go for it - trace the picture */
   if ((Options & VERBOSE) && (VerboseFormat !='1'))
      printf ("Rendering...\n");
   else if ((Options & VERBOSE) && (VerboseFormat=='1'))
      fprintf (stderr,"POV-Ray rendering %s to %s :\n",Input_File_Name,Output_File_Name);
   if (Options & VERBOSE_FILE){
      stat_file = fopen(Stat_File_Name,"w+t");
      fprintf (stat_file,"Parsed ok. Now rendering %s to %s :\n",Input_File_Name,Output_File_Name);
      fclose(stat_file);
   }

   CONFIG_MATH               /* Macro for setting up any special FP options */
   Start_Tracing ();

   if (Options & VERBOSE && VerboseFormat=='1')
      fprintf (stderr,"\n");

   /* Record the time so well spent... */
   STOP_TIME                  /* Get trace done time. */
   tused = TIME_ELAPSED       /* Calc. elapsed time. Define TIME_ELAPSED as */
   /* 0 in your specific CONFIG.H if unsupported */

   /* Clean up and leave */
   display_finished();

   close_all ();

   PRINT_STATS

   if (Options & VERBOSE_FILE){
      stat_file = fopen(Stat_File_Name,"a+t");
      fprintf (stat_file,"Done Tracing\n");
      fclose(stat_file);
   }

   FINISH_POVRAY
}

/* Print out usage error message */

void usage ()
{
   WAIT_FOR_KEYPRESS		
   fprintf (stdout,"\nUsage:");
   fprintf (stdout,"\n   povray  [+/-] Option1 [+/-] Option2 ...");
   fprintf (stdout,"\n");
   fprintf (stdout,"\n Options: ");
   fprintf (stdout,"\n    dxy = display in format x, using palette option y");
   fprintf (stdout,"\n    vx  = verbose in format x");
   /*fprintf (stdout,"\n    @filename  = verbose to file name -- see docs");*/
   fprintf (stdout,"\n    p  = pause before exit");
   fprintf (stdout,"\n    x  = enable early exit by key hit");
   fprintf (stdout,"\n    fx = write output file in format x");
   fprintf (stdout,"\n         ft - Uncompressed Targa-24  fd - DKB/QRT Dump  fr - 3 Raw Files");
   fprintf (stdout,"\n    a  = perform antialiasing");
   fprintf (stdout,"\n    c  = continue aborted trace");
   fprintf (stdout,"\n    qx = image quality 0=rough, 9=full");
   fprintf (stdout,"\n    l<pathname> = library path prefix");
   fprintf (stdout,"\n    wxxx = width of the screen");
   fprintf (stdout,"\n    hxxx = height of the screen");
   fprintf (stdout,"\n    sxxx = start at line number xxx");
   fprintf (stdout,"\n    exxx = end at line number xxx");
   fprintf (stdout,"\n    bxxx = Use xxx kilobytes for output file buffer space");
   fprintf (stdout,"\n    i<filename> = input file name");
   fprintf (stdout,"\n    o<filename> = output file name");
   fprintf (stdout,"\n  Ex: +l\\povray\\include +iscene.pov +oscene.tga +w320 +h200 +d -v +x");
   fprintf (stdout,"\n  Ex: +iscene.pov +oscene.tga +w160 +h200 +v -d +x");
   fprintf (stdout,"\n");
   exit(1);
}

void init_vars()
{
   Output_File_Handle = NULL;
   File_Buffer_Size = 0;
   Options = 0;
   Quality = 9;
   Number_Of_Files = 0;
   First_Line = 0;
   Last_Line = -1;
   Color_Bits = 8;

   Number_Of_Pixels = 0L;
   Number_Of_Rays = 0L;
   Number_Of_Pixels_Supersampled = 0L;
   Ray_Ht_Field_Tests = 0L;
   Ray_Ht_Field_Tests_Succeeded = 0L;
   Ray_Ht_Field_Box_Tests = 0L;
   Ray_HField_Box_Tests_Succeeded = 0L;
   Ray_Bicubic_Tests = 0L;
   Ray_Bicubic_Tests_Succeeded = 0L;
   Ray_Blob_Tests = 0L;
   Ray_Blob_Tests_Succeeded = 0L;
   Ray_Box_Tests = 0L;
   Ray_Box_Tests_Succeeded = 0L;
   Ray_Sphere_Tests = 0L;
   Ray_Sphere_Tests_Succeeded = 0L;
   Ray_Plane_Tests = 0L;
   Ray_Plane_Tests_Succeeded = 0L;
   Ray_Triangle_Tests = 0L;
   Ray_Triangle_Tests_Succeeded = 0L;
   Ray_Quadric_Tests = 0L;
   Ray_Quadric_Tests_Succeeded = 0L;
   Ray_Poly_Tests = 0L;
   Ray_Poly_Tests_Succeeded = 0L;
   Bounding_Region_Tests = 0L;
   Bounding_Region_Tests_Succeeded = 0L;
   Clipping_Region_Tests = 0L;
   Clipping_Region_Tests_Succeeded = 0L;
   Calls_To_Noise = 0L;
   Calls_To_DNoise = 0L;
   Shadow_Ray_Tests = 0L;
   Shadow_Rays_Succeeded = 0L;
   Reflected_Rays_Traced = 0L;
   Refracted_Rays_Traced = 0L;
   Transmitted_Rays_Traced = 0L;

   Frame.Screen_Height = 100;
   Frame.Screen_Width = 100;

   Antialias_Threshold = 0.3;
   strcpy (Input_File_Name, "object.dat");
   return;
}

/* Close all the stuff that has been opened. */
void close_all ()
{
   if ((Options & DISPLAY) && Display_Started)
      display_close();

   if (Output_File_Handle)
      Close_File (Output_File_Handle);
}

/* Read the default parameters from povray.def */
void get_defaults()
{
   FILE *defaults_file;
   char Option_String[256], *Option_String_Ptr;
   /* READ_ENV_VAR_? should be defined in config.h */
   /* Only one READ_ENV_VAR_? should ever be defined. */
   /* This allows some machines to read environment variable before */
   /* reading povray.def and others to do it after depending on the */
   /* operating system. IBM-PC is before. Default is after if not */
   /* defined in config.h. CDW 2/92 */
   /* Set Diskwrite as default */
   Options |= DISKWRITE;
   OutputFormat = DEFAULT_OUTPUT_FORMAT;

   READ_ENV_VAR_BEFORE
   if ((defaults_file = Locate_File("povray.def", "r")) != NULL) {
      while (fgets(Option_String, 256, defaults_file) != NULL)
         read_options(Option_String);
      fclose (defaults_file);
   }
   READ_ENV_VAR_AFTER
}

void read_options (Option_Line)
char *Option_Line;
{
   register int c, String_Index, Option_Started;
   short Option_Line_Index = 0;
   char Option_String[80];

   String_Index = 0;
   Option_Started = FALSE;
   while ((c = Option_Line[Option_Line_Index++]) != '\0')
   {
      if (Option_Started)
         if (isspace(c))
         {
            Option_String[String_Index] = '\0';
            parse_option (Option_String);
            Option_Started = FALSE;
            String_Index = 0;
         }
         else
            Option_String[String_Index++] = (char) c;

      else /* Option_Started */
         if ((c == (int) '-') || (c == (int) '+'))
         {
            String_Index = 0;
            Option_String[String_Index++] = (char) c;
            Option_Started = TRUE;
         }
         else
            if (!isspace(c))
            {
               fprintf (stderr, "\nBad default file format.  Offending char: (%c), val: %d.\n", (char) c, c);
               exit (1);
            }
   }

   if (Option_Started)
   {
      Option_String[String_Index] = '\0';
      parse_option (Option_String);
   }
}

/* parse the command line parameters */
void parse_option (Option_String)
char *Option_String;
{
   register int Add_Option;
   unsigned int Option_Number;
   DBL threshold;

   inflag = outflag = FALSE;   /* if these flags aren't immediately used, reset them on next -/+ option! */
   if (*(Option_String++) == '-')
      Add_Option = FALSE;
   else
      Add_Option = TRUE;

   switch (*Option_String)
   {
   case 'B':
   case 'b':  
      sscanf (&Option_String[1], "%d", &File_Buffer_Size);
      File_Buffer_Size *= 1024;
      if (File_Buffer_Size < BUFSIZ)
         File_Buffer_Size = BUFSIZ;
      Option_Number = 0;
      break;

   case 'C':
   case 'c':  
      Option_Number = CONTINUE_TRACE;
      break;

   case 'D':
   case 'd':  
      Option_Number = DISPLAY;
      DisplayFormat = '0';
      PaletteOption = '3';
      if (Option_String[1] != '\0')
         DisplayFormat = (char)toupper(Option_String[1]);

      if (Option_String[1] != '\0' && Option_String[2] != '\0')
         PaletteOption = (char)toupper(Option_String[2]);
      break;

   case '@':  
      Option_Number = VERBOSE_FILE;
      if(Option_String[1] == '\0')
         strcpy(Stat_File_Name, "POVSTAT.OUT");
      else
         strncpy (Stat_File_Name, &Option_String[1], FILE_NAME_LENGTH);
      break;
   case 'V':
   case 'v':  
      Option_Number = VERBOSE;
      VerboseFormat = (char)toupper(Option_String[1]);
      if (VerboseFormat == '\0')
         VerboseFormat = '1';
      break;

   case 'W':
   case 'w':  
      sscanf (&Option_String[1], "%d", &Frame.Screen_Width);
      Option_Number = 0;
      break;

   case 'H':
   case 'h':  
      sscanf (&Option_String[1], "%d", &Frame.Screen_Height);
      Option_Number = 0;
      break;

   case 'F':
   case 'f':  
      Option_Number = DISKWRITE;
      if (isupper(Option_String[1]))
         OutputFormat = (char)tolower(Option_String[1]);
      else
         OutputFormat = Option_String[1];

      /* Default the output format to the default in the config file */
      if (OutputFormat == '\0')
         OutputFormat = DEFAULT_OUTPUT_FORMAT;
      break;

   case 'P':
   case 'p':  
      Option_Number = PROMPTEXIT;
      break;

   case 'I':
   case 'i':  
      if (Option_String[1] == '\0')
         inflag = TRUE;
      else 
         strncpy (Input_File_Name, &Option_String[1], FILE_NAME_LENGTH);
      Option_Number = 0;
      break;

   case 'O':
   case 'o':  
      if (Option_String[1] == '\0')
         outflag = TRUE;
      else
         strncpy (Output_File_Name, &Option_String[1], FILE_NAME_LENGTH);
      Option_Number = 0;
      break;

   case 'A':
   case 'a':  
      Option_Number = ANTIALIAS;
      if (sscanf (&Option_String[1], DBL_FORMAT_STRING, &threshold) != EOF)
         Antialias_Threshold = threshold;
      break;

   case 'X':
   case 'x':  
      Option_Number = EXITENABLE;
      break;


   case 'L':
   case 'l':  
      if (Library_Path_Index >= MAX_LIBRARIES) {
         fprintf (stderr, "Too many library directories specified\n");
         exit(1);
      }
      Library_Paths [Library_Path_Index] = malloc (strlen(Option_String));
      if (Library_Paths [Library_Path_Index] == NULL) {
         fprintf (stderr, "Out of memory. Cannot allocate memory for library pathname\n");
         exit(1);
      }
      strcpy (Library_Paths [Library_Path_Index], &Option_String[1]);
      Library_Path_Index++;
      Option_Number = 0;
      break;
   case 'T':
   case 't':  
      switch (toupper(Option_String[1])){
      case 'Y':
         Case_Sensitive_Flag = 0;
         break;
      case 'N':
         Case_Sensitive_Flag = 1;
         break;
      case 'O':
         Case_Sensitive_Flag = 2;
         break;
      default:
         Case_Sensitive_Flag = 2;
         break;
      }        
      Option_Number = 0;
      break;

   case 'S':
   case 's':  
      sscanf (&Option_String[1], "%d", &First_Line);
      Option_Number = 0;
      break;

   case 'E':
   case 'e':  
      sscanf (&Option_String[1], "%d", &Last_Line);
      Option_Number = 0;
      break;

   case 'M': /* Switch used so other max values can be inserted easily */
   case 'm':  
      switch (Option_String[1]){
      case 's': /* Max Symbols */
      case 'S':
         sscanf (&Option_String[2], "%d", &Max_Symbols);
         Option_Number = 0;
         break;
      default:
         break;
      }
      break;

   case 'Q':
   case 'q':  
      sscanf (&Option_String[1], "%d", &Quality);
      Option_Number = 0;
      break;

      /* Turn on debugging print statements. */
   case 'Z':
   case 'z':  
      Option_Number = DEBUGGING;
      break;

   default:   
      fprintf (stderr, "\nInvalid option: %s\n\n", --Option_String);
      Option_Number = 0;
   }

   if (Option_Number != 0)
      if (Add_Option)
         Options |= Option_Number;
      else Options &= ~Option_Number;
}

   void Print_Options()
   {
      int i;

      fprintf (stdout,"\nPOV-Ray	Options in effect: ");

      if (Options & CONTINUE_TRACE)
         fprintf (stdout,"+c ");

      if (Options & DISPLAY)
         fprintf (stdout,"+d%c%c ", DisplayFormat, PaletteOption);

      if (Options & VERBOSE)
         fprintf (stdout,"+v%c ", VerboseFormat);

      if (Options & VERBOSE_FILE)
         fprintf (stdout,"+@%s ", Stat_File_Name);

      if (Options & DISKWRITE)
         fprintf (stdout,"+f%c ", OutputFormat);

      if (Options & PROMPTEXIT)
         fprintf (stdout,"+p ");

      if (Options & EXITENABLE)
         fprintf (stdout,"+x ");

      if (Options & ANTIALIAS)
         fprintf (stdout,"+a%f ", Antialias_Threshold);

      if (Options & DEBUGGING)
         fprintf (stdout,"+z ");

      if (File_Buffer_Size != 0)
         fprintf (stdout,"-b%d ", File_Buffer_Size/1024);

      fprintf (stdout,"-q%d -w%d -h%d -s%d -e%d\n-i%s ",
         Quality, Frame.Screen_Width, Frame.Screen_Height,
         First_Line, Last_Line, Input_File_Name);

      if (Options & DISKWRITE)
         fprintf (stdout,"-o%s ", Output_File_Name);

      for (i = 0 ; i < Library_Path_Index ; i++)
         fprintf (stdout,"-l%s ", Library_Paths[i]);

      fprintf (stdout,"\n");
   }

void parse_file_name (File_Name)
char *File_Name;
{
   FILE *defaults_file;
   char Option_String[256];

   if (inflag)   /* file names may now be separated by spaces from cmdline option */
   {
      strncpy (Input_File_Name, File_Name, FILE_NAME_LENGTH);
      inflag = FALSE;
      return;
   }

   if (outflag)  /* file names may now be separated by spaces from cmdline option */
   {
      strncpy (Output_File_Name, File_Name, FILE_NAME_LENGTH);
      outflag = FALSE;
      return;
   }


   if (++Number_Of_Files > MAX_FILE_NAMES)
   {
      fprintf (stderr, "\nOnly %d option file names are allowed in a command line.", 
         MAX_FILE_NAMES);
      exit(1);
   }

   if ((defaults_file = Locate_File(File_Name, "r")) != NULL) {
      while (fgets(Option_String, 256, defaults_file) != NULL)
         read_options(Option_String);
      fclose (defaults_file);
   }
   else
      printf("\nError opening option file %s.",File_Name);  
}

void print_stats()
{
   int hours,min;
   DBL sec;
   FILE *stat_out;
   long Pixels_In_Image;

   if (Options & VERBOSE_FILE)
      stat_out = fopen(Stat_File_Name,"w+t");
   else
      stat_out = stdout;  

   Pixels_In_Image = (long)Frame.Screen_Width * (long)Frame.Screen_Height;


   fprintf (stat_out,"\n%s statistics\n",Input_File_Name);
   if(Pixels_In_Image > Number_Of_Pixels)
      fprintf (stat_out,"  Partial Image Rendered");

   fprintf (stat_out,"--------------------------------------\n");
   fprintf (stat_out,"Resolution %d x %d\n",Frame.Screen_Width, Frame.Screen_Height);
   fprintf (stat_out,"# Rays:  %10ld    # Pixels:  %10ld  # Pixels supersampled: %10ld\n",
      Number_Of_Rays, Number_Of_Pixels, Number_Of_Pixels_Supersampled);

   fprintf (stat_out,"  Ray->Shape Intersection Tests:\n");
   fprintf (stat_out,"   Type             Tests    Succeeded   Percentage\n");
   fprintf (stat_out,"  -----------------------------------------------------------\n");
   if(Ray_Sphere_Tests)
      fprintf (stat_out,"  Sphere       %10ld  %10ld  %10.2f\n", Ray_Sphere_Tests, Ray_Sphere_Tests_Succeeded, ( ((DBL)Ray_Sphere_Tests_Succeeded/(DBL)Ray_Sphere_Tests) *100.0 ) );
   if(Ray_Plane_Tests)
      fprintf (stat_out,"  Plane        %10ld  %10ld  %10.2f\n", Ray_Plane_Tests, Ray_Plane_Tests_Succeeded, ( ((DBL)Ray_Plane_Tests_Succeeded/(DBL)Ray_Plane_Tests) *100.0 ));
   if(Ray_Triangle_Tests)
      fprintf (stat_out,"  Triangle     %10ld  %10ld  %10.2f\n", Ray_Triangle_Tests, Ray_Triangle_Tests_Succeeded, ( ((DBL)Ray_Triangle_Tests_Succeeded/(DBL)Ray_Triangle_Tests) *100.0 ));
   if(Ray_Quadric_Tests)
      fprintf (stat_out,"  Quadric      %10ld  %10ld  %10.2f\n", Ray_Quadric_Tests, Ray_Quadric_Tests_Succeeded, ( ((DBL)Ray_Quadric_Tests_Succeeded/(DBL)Ray_Quadric_Tests) *100.0 ));
   if(Ray_Blob_Tests)
      fprintf (stat_out,"  Blob         %10ld  %10ld  %10.2f\n", Ray_Blob_Tests, Ray_Blob_Tests_Succeeded, ( ((DBL)Ray_Blob_Tests_Succeeded/(DBL)Ray_Blob_Tests) *100.0 ));
   if(Ray_Box_Tests)
      fprintf (stat_out,"  Box          %10ld  %10ld  %10.2f\n", Ray_Box_Tests, Ray_Box_Tests_Succeeded, ( ((DBL)Ray_Box_Tests_Succeeded/(DBL)Ray_Box_Tests) *100.0 ));
   if(Ray_Poly_Tests)
      fprintf (stat_out,"  Quartic\\Poly %10ld  %10ld  %10.2f\n", Ray_Poly_Tests, Ray_Poly_Tests_Succeeded, ( ((DBL)Ray_Poly_Tests_Succeeded/(DBL)Ray_Poly_Tests) *100.0 ));
   if(Ray_Bicubic_Tests)
      fprintf (stat_out,"  Bezier Patch %10ld  %10ld  %10.2f\n", Ray_Bicubic_Tests, Ray_Bicubic_Tests_Succeeded, ( ((DBL)Ray_Bicubic_Tests_Succeeded/(DBL)Ray_Bicubic_Tests) *100.0 ));
   if(Ray_Ht_Field_Tests)
      fprintf (stat_out,"  Height Fld   %10ld  %10ld  %10.2f\n", Ray_Ht_Field_Tests, Ray_Ht_Field_Tests_Succeeded, ( ((DBL)Ray_Ht_Field_Tests_Succeeded/(DBL)Ray_Ht_Field_Tests) *100.0 ));
   if(Ray_Ht_Field_Box_Tests)
      fprintf (stat_out,"  Hght Fld Box %10ld  %10ld  %10.2f\n", Ray_Ht_Field_Box_Tests, Ray_HField_Box_Tests_Succeeded, ( ((DBL)Ray_HField_Box_Tests_Succeeded/(DBL)Ray_Ht_Field_Box_Tests) *100.0 ));
   if(Bounding_Region_Tests)
      fprintf (stat_out,"  Bounds       %10ld  %10ld  %10.2f\n", Bounding_Region_Tests, Bounding_Region_Tests_Succeeded, ( ((DBL)Bounding_Region_Tests_Succeeded/(DBL)Bounding_Region_Tests) *100.0 ));
   if(Clipping_Region_Tests)
      fprintf (stat_out,"  Clips        %10ld  %10ld  %10.2f\n", Clipping_Region_Tests, Clipping_Region_Tests_Succeeded, ( ((DBL)Clipping_Region_Tests_Succeeded/(DBL)Clipping_Region_Tests) *100.0 ));

   if(Calls_To_Noise) 

      fprintf (stat_out,"  Calls to Noise:   %10ld\n", Calls_To_Noise);
   if(Calls_To_DNoise)
      fprintf (stat_out,"  Calls to DNoise:  %10ld\n", Calls_To_DNoise);
   if(Shadow_Ray_Tests)
      fprintf (stat_out,"  Shadow Ray Tests: %10ld     Blocking Objects Found:  %10ld\n",
         Shadow_Ray_Tests, Shadow_Rays_Succeeded);
   if(Reflected_Rays_Traced)
      fprintf (stat_out,"  Reflected Rays:   %10ld\n", Reflected_Rays_Traced);
   if(Refracted_Rays_Traced)
      fprintf (stat_out,"  Refracted Rays:   %10ld\n", Refracted_Rays_Traced);
   if(Transmitted_Rays_Traced)
      fprintf (stat_out,"  Transmitted Rays: %10ld\n", Transmitted_Rays_Traced);

   if(tused==0) {
      STOP_TIME                  /* Get trace done time. */
      tused = TIME_ELAPSED       /* Calc. elapsed time. Define TIME_ELAPSED as */
      /* 0 in your specific CONFIG.H if unsupported */
   }
   if (tused != 0){
      /* Convert seconds to hours, min & sec. CdW */
      hours = (int) tused/3600;
      min = (int) (tused - hours*3600)/60;
      sec = tused - (DBL) (hours*3600 + min*60);
      fprintf (stat_out,"  Time For Trace:   %2d hours %2d minutes %4.2f seconds\n", hours,min,sec); 
   }
   if (Options & VERBOSE_FILE)
      fclose(stat_out);

}

/* Find a file in the search path. */

FILE *Locate_File (filename, mode)
char *filename, *mode;
{
   FILE *f;
   int i;
   char pathname[FILE_NAME_LENGTH];

   /* Check the current directory first. */
   if ((f = fopen (filename, mode)) != NULL)
      return (f);

   for (i = 0 ; i < Library_Path_Index ; i++) {
      strcpy (pathname, Library_Paths[i]);
      if (FILENAME_SEPARATOR != NULL)
         strcat (pathname, FILENAME_SEPARATOR);
      strcat (pathname, filename);
      if ((f = fopen (pathname, mode)) != NULL)
         return (f);
   }

   return (NULL);
}
void print_credits()
{
   fprintf (stderr,"\n");
   fprintf (stderr,"  Persistence of Vision Raytracer Ver 1.0%s\n",COMPILER_VER);
   fprintf (stderr,"  Copyright 1992 POV-Team\n");
   fprintf (stderr,"  ----------------------------------------------------------------------\n");
   fprintf (stderr,"  POV-Ray is based on DKBTrace 2.12 by David K. Buck & Aaron A. Collins.\n");
   fprintf (stderr,"  \n");
   fprintf (stderr,"  Contributing Authors: (Alphabetically)\n");
   fprintf (stderr,"  \n");
   fprintf (stderr,"  Steve A. Bennett   David K. Buck      Aaron A. Collins\n");
   fprintf (stderr,"  Alexander Enzmann  Dan Farmer         Girish T. Hagan\n");
   fprintf (stderr,"  Douglas Muir       Bill Pulver        Robert Skinner\n");
   fprintf (stderr,"  Scott Taylor       Drew Wells         Chris Young\n");
   fprintf (stderr,"  ----------------------------------------------------------------------\n");
   fprintf (stderr,"  Other contributors listed in the documentation.\n");
}
