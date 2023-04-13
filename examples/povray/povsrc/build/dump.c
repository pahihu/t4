/****************************************************************************
*                   dump.c
*
*  This module contains the code to read and write the dump file format.
*  The format is as follows:
*
*  (header:)
*    wwww hhhh       - Width, Height (16 bits, LSB first)
*
*  (each scanline:)
*    llll            - Line number (16 bits, LSB first)
*    rr rr rr ...    - Red data for line (8 bits per pixel,
*                       left to right, 0-255 (255=bright, 0=dark))
*    gg gg gg ...    - Green data for line (8 bits per pixel,
*                       left to right, 0-255 (255=bright, 0=dark))
*    bb bb bb ...    - Blue data for line (8 bits per pixel,
*                       left to right, 0-255 (255=bright, 0=dark))
*
*
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
#include "povproto.h"

FILE_HANDLE *Get_Dump_File_Handle()
{
   FILE_HANDLE *handle;

   if ((handle = (FILE_HANDLE *) malloc(sizeof(FILE_HANDLE))) == NULL) {
      fprintf (stderr, "Cannot allocate memory for output file handle\n");
      return(NULL);
   }

   handle->Default_File_Name_p = Default_Dump_File_Name;
   handle->Open_File_p = Open_Dump_File;
   handle->Write_Line_p = Write_Dump_Line;
   handle->Read_Line_p = Read_Dump_Line;
   handle->Read_Image_p = Read_Dump_Image;
   handle->Close_File_p = Close_Dump_File;
   return (handle);
}

char *Default_Dump_File_Name()
{
   return ("data.dis");
}

int Open_Dump_File (handle, name, width, height, buffer_size, mode)
FILE_HANDLE *handle;
char *name;
int *width;
int *height;
int buffer_size;
int mode;
{
   int data1, data2;

   handle->mode = mode;
   handle->filename = name;

   switch (mode) {
   case READ_MODE:
      if ((handle->file = fopen (name, READ_FILE_STRING)) == NULL) {
         return(0);
      }

      if (buffer_size != 0) {
         if ((handle->buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (handle->file, handle->buffer, _IOFBF, buffer_size);
      }

      if (((data1 = getc(handle->file)) == EOF)
         || ((data2 = getc(handle->file)) == EOF))
         return(0);

      *width  = data2 * 256 + data1;

      if (((data1 = getc(handle->file)) == EOF)
         || ((data2 = getc(handle->file)) == EOF))
         return(0);

      *height = data2 * 256 + data1;
      handle->width = *width;
      handle->height = *height;
      handle->buffer_size = buffer_size;
      break;

   case WRITE_MODE:
      if ((handle->file = fopen (name, WRITE_FILE_STRING)) == NULL)
         return(0);

      if (buffer_size != 0) {
         if ((handle->buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (handle->file, handle->buffer, _IOFBF, buffer_size);
      }

      putc(*width % 256, handle->file);  /* write to either type of file */
      putc(*width / 256, handle->file);
      putc(*height % 256, handle->file);
      putc(*height / 256, handle->file);

      handle->width = *width;
      handle->height = *height;
      handle->buffer_size = buffer_size;

      break;

   case APPEND_MODE:
      if ((handle->file = fopen (name, APPEND_FILE_STRING)) == NULL)
         return(0);

      if (buffer_size != 0) {
         if ((handle->buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (handle->file, handle->buffer, _IOFBF, buffer_size);
      }

      handle->buffer_size = buffer_size;
      break;
   }
   return(1);
}

void Write_Dump_Line (handle, line_data, line_number)
FILE_HANDLE *handle;
COLOUR *line_data;
int line_number;
{
   register int x;

   putc(line_number % 256, handle->file);
   putc(line_number / 256, handle->file);

   for (x = 0 ; x < handle->width ; x++)
      putc((int) floor (line_data[x].Red * 255.0), handle->file);

   for (x = 0 ; x < handle->width ; x++)
      putc((int) floor (line_data[x].Green * 255.0), handle->file);

   for (x = 0 ; x < handle->width ; x++)
      putc((int) floor (line_data[x].Blue * 255.0), handle->file);

   if (handle->buffer_size == 0) {
      fflush(handle->file);                       /* close and reopen file for */
      handle->file = freopen(handle->filename, APPEND_FILE_STRING,
         handle->file);                /* integrity in case we crash*/
   }
}

int Read_Dump_Line (handle, line_data, line_number)
FILE_HANDLE *handle;
COLOUR *line_data;
int *line_number;
{
   int data, i, c;

   if ((c = getc(handle->file)) == EOF) {
      return (0);
   }

   *line_number = c;

   if ((c = getc(handle->file)) == EOF)
      return (-1);

   *line_number += c*256;

   for (i = 0 ; i < handle->width ; i++) {
      if ((data = getc(handle->file)) == EOF)
         return(-1);

      line_data[i].Red = (DBL) data / 255.0;
   }

   for (i = 0 ; i < handle->width ; i++) {
      if ((data = getc(handle->file)) == EOF)
         return(-1);

      line_data[i].Green = (DBL) data / 255.0;
   }

   for (i = 0 ; i < handle->width ; i++) {
      if ((data = getc(handle->file)) == EOF)
         return(-1);

      line_data[i].Blue = (DBL) data / 255.0;
   }

   return (1);
}

int Read_Dump_Int_Line (handle, line_data, line_number)
FILE_HANDLE *handle;
IMAGE_LINE *line_data;
int *line_number;
{
   int data, i, c;

   if ((c = getc(handle->file)) == EOF) {
      return (0);
   }

   *line_number = c;

   if ((c = getc(handle->file)) == EOF)
      return (-1);

   *line_number += c*256;

   if (((line_data->red = (unsigned char *) malloc(handle->width))==NULL) ||
      ((line_data->green = (unsigned char *) malloc(handle->width))==NULL) ||
      ((line_data->blue = (unsigned char *) malloc(handle->width))==NULL)) {
      fprintf (stderr, "Cannot allocate memory for picture: %s\n", handle->filename);
      close_all();
      exit(1);
   }

   for (i = 0 ; i < handle->width ; i++) {
      line_data->red[i] = 0;
      line_data->green[i] = 0;
      line_data->blue[i] = 0;
   }

   for (i = 0 ; i < handle->width ; i++) {
      if ((data = getc(handle->file)) == EOF)
         return(-1);

      line_data->red[i] = (unsigned char) data;
   }

   for (i = 0 ; i < handle->width ; i++) {
      if ((data = getc(handle->file)) == EOF)
         return(-1);

      line_data->green[i] = (unsigned char) data;
   }

   for (i = 0 ; i < handle->width ; i++) {
      if ((data = getc(handle->file)) == EOF)
         return(-1);

      line_data->blue[i] = (unsigned char) data;
   }

   return (1);
}

void Close_Dump_File (handle)
FILE_HANDLE *handle;
{
   if(handle->file)
      fclose (handle->file);
   if (handle->buffer_size != 0)
      free (handle->buffer);
}

void Read_Dump_Image(Image, name)
IMAGE *Image;
char *name;
{
   int rc, row, data1, data2;
   struct Image_Line line;
   FILE_HANDLE handle;

   if ((handle.file = Locate_File (name, READ_FILE_STRING)) == NULL) {
      fprintf (stderr, "Cannot open dump file %s\n", name);
      close_all();
      exit(1);
   }

   if (((data1 = getc(handle.file)) == EOF)
      || ((data2 = getc(handle.file)) == EOF)) {

      fprintf (stderr, "Cannot open dump file %s\n", name);
      close_all();
      exit(1);
   }

   Image->iwidth  = data2 * 256 + data1;
   handle.width = Image->iwidth;

   if (((data1 = getc(handle.file)) == EOF)
      || ((data2 = getc(handle.file)) == EOF)) {

      fprintf (stderr, "Cannot open dump file %s\n", name);
      close_all();
      exit(1);
   }

   Image->iheight = data2 * 256 + data1;
   handle.height = Image->iheight;

   Image->width = (DBL)Image->iwidth;
   Image->height = (DBL)Image->iheight;

   Image->Colour_Map_Size = 0;
   Image->Colour_Map = NULL;

   if ((Image->data.rgb_lines = (struct Image_Line *)
      malloc(Image->iheight * sizeof (struct Image_Line))) == NULL) {
      fprintf (stderr, "Cannot allocate memory for picture: %s\n", name);
      exit(1);
   }

   while ((rc = Read_Dump_Int_Line(&handle, &line, &row)) == 1)
      Image->data.rgb_lines[row] = line;

   fclose (handle.file);

   if (rc == 0)
      return;
   else {
      close_all();
      exit(1);
   }
}
