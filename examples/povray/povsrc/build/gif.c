/****************************************************************************
*                   gif.c
*
*  Gif-format file reader.
*
*  NOTE:  Portions of this module were written by Steve Bennett and are used
*         here with his permission.
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

/*
   The following routines were borrowed freely from FRACTINT, and represent
   a generalized GIF file decoder.  This seems the best, most universal format
   for reading in Bitmapped images.  GIF is a Copyright of Compuserve, Inc.
   Swiped and converted to entirely "C" coded routines by AAC for the most
   in future portability!
*/

#include "frame.h"
#include "povproto.h"

static IMAGE *Current_Image;  
static int Bitmap_Line;
static FILE *Bit_File;
unsigned char *decoderline  /*  [2049] */ ;  /* write-line routines use this */

static IMAGE_COLOUR *gif_colour_map;
static int colourmap_size;

int out_line (pixels, linelen)
unsigned char *pixels;
int linelen;
{
   register int x;
   register unsigned char *line;

   line = Current_Image->data.map_lines[Bitmap_Line++];

   for (x = 0; x < linelen; x++) {
      if ((int)(*pixels) > Current_Image->Colour_Map_Size) {
         fprintf (stderr, "Error - GIF Image Map Colour out of range\n");
         exit(1);
      }

      line[x] = *pixels;
      pixels++;
   }

   return (0);
}

#define READ_ERROR -1

int get_byte() /* get byte from file, return the next byte or an error */
{
   register int byte;

   if ((byte = getc(Bit_File)) != EOF)
      return (byte);
   else {
      fprintf (stderr, "Premature End Of File reading GIF image\n");
      exit (1);
   }
   return (0);  /* Keep the compiler happy */
}

   /* Main GIF file decoder.  */

   void Read_Gif_Image(Image, filename)
      IMAGE *Image;
char *filename;
{
   register int i, j, status;
   unsigned finished, planes;
   unsigned char buffer[16];

   status = 0;
   Current_Image = Image;

   if ((Bit_File = Locate_File(filename, READ_FILE_STRING)) == NULL) {
      fprintf (stderr, "Cannot open GIF file %s\n", filename);
      exit(1);
   }

   /* zero out the full write-line */
   if ((decoderline = (unsigned char *) malloc (2049)) == NULL) {
      fprintf (stderr, "Cannot allocate space for GIF decoder line\n");
      fclose (Bit_File);
      exit (1);
   }

   for (i = 0; i < 2049; i++)
      decoderline[i] = (unsigned char) 0;

   /* Get the screen description */
   for (i = 0; i < 13; i++)
      buffer[i] = (unsigned char)get_byte();

   if (strncmp((char *) buffer,"GIF",3) ||          /* use updated GIF specs */
      buffer[3] < '0' || buffer[3] > '9' ||
      buffer[4] < '0' || buffer[4] > '9' ||
      buffer[5] < 'A' || buffer[5] > 'z' ) {

      fprintf (stderr, "Invalid GIF file format: %s\n", filename);
      fclose(Bit_File);
      exit (1);
   }

   planes = ((unsigned)buffer[10] & 0x0F) + 1;
   colourmap_size = (int)(1 << planes);

   if ((gif_colour_map = (IMAGE_COLOUR *)
      malloc (colourmap_size * sizeof (IMAGE_COLOUR))) == NULL) {
      fprintf (stderr, "Cannot allocate GIF Colour Map\n");
      fclose (Bit_File);
      exit (1);
   }

   if ((buffer[10] & 0x80) == 0) {    /* color map (better be!) */
      fprintf (stderr, "Invalid GIF file format: %s\n", filename);
      fclose(Bit_File);
      exit (1);
   }

   for (i = 0; i < colourmap_size ; i++) {
      gif_colour_map[i].Red = (unsigned char)get_byte();
      gif_colour_map[i].Green = (unsigned char)get_byte();
      gif_colour_map[i].Blue = (unsigned char)get_byte();
      gif_colour_map[i].Alpha = 0;
   }

   /* Now display one or more GIF objects */
   finished = FALSE;
   while (!finished) {
      switch (get_byte()) {
      case ';':                /* End of the GIF dataset */
         finished = TRUE;
         status = 0;
         break;

      case '!':                /* GIF Extension Block */
         get_byte();           /* read (and ignore) the ID */
         while ((i = get_byte()) > 0) /* get data len*/
            for (j = 0; j < i; j++)
               get_byte(); /* flush data */
         break;

      case ',': /* Start of image object. get description */
         for (i = 0; i < 9; i++) {
            if ((j = get_byte()) < 0) {	/* EOF test (?) */
               status = -1;
               break;
            }
            buffer[i] = (unsigned char) j;
         }

         if (status < 0) {
            finished = TRUE;
            break;
         }

         Image->iwidth  = buffer[4] | (buffer[5] << 8);
         Image->iheight = buffer[6] | (buffer[7] << 8);
         Image->width = (DBL) Image->iwidth;
         Image->height = (DBL) Image->iheight;

         Bitmap_Line = 0;

         Image->Colour_Map_Size = colourmap_size;
         Image->Colour_Map = gif_colour_map;

         if ((Image->data.map_lines = (unsigned char **)
            malloc(Image->iheight * sizeof (unsigned char *)))==NULL) {
            fprintf (stderr, "Cannot allocate memory for picture\n");
            exit(1);
         }

         for (i = 0 ; i < Image->iheight ; i++) {
            if ((Image->data.map_lines[i] = (unsigned char *) malloc(Image->iwidth))==NULL) {
               fprintf (stderr, "Cannot allocate memory for picture\n");
               exit(1);
            }
         }

         /* Setup the color palette for the image */
         status = decoder ( Image->iwidth); /*put bytes in Buf*/
         /* changed param to int */
         finished = TRUE;
         break;

      default:
         status = -1;
         finished = TRUE;
         break;
      }
   }

   free (decoderline);
   fclose(Bit_File);
}
