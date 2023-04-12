/****************************************************************************
*                targa.c
*
*  This module contains the code to read and write the Targa output file
*  format.
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

int Targa_Line_Number = 0;

extern int First_Line;

FILE_HANDLE *Get_Targa_File_Handle()
{
   FILE_HANDLE *handle;

   if ((handle = (FILE_HANDLE *) malloc(sizeof(FILE_HANDLE))) == NULL) {
      fprintf (stderr, "Cannot allocate memory for output file handle\n");
      return(NULL);
   }

   handle->Default_File_Name_p = Default_Targa_File_Name;
   handle->Open_File_p = Open_Targa_File;
   handle->Write_Line_p = Write_Targa_Line;
   handle->Read_Line_p = Read_Targa_Line;
   handle->Read_Image_p = Read_Targa_Image;
   handle->Close_File_p = Close_Targa_File;
   handle->file = NULL;
   handle->buffer_size = 0;
   handle->buffer = NULL;
   return (handle);
}

char *Default_Targa_File_Name()
{
   return ("data.tga");
}

int Open_Targa_File (handle, name, width, height, buffer_size, mode)
FILE_HANDLE *handle;
char *name;
int *width;
int *height;
int buffer_size;
int mode;
{
   int data1, data2, i;

   handle->mode = mode;
   handle->filename = name;

   switch (mode) {
   case READ_MODE:
      if ((handle->file = fopen (name, READ_FILE_STRING)) == NULL)
         return(0);

      if (buffer_size != 0) {
         if ((handle->buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (handle->file, handle->buffer, _IOFBF, buffer_size);
      }

      for (i = 0 ; i < 12 ; i++)
         if (getc(handle->file) == EOF)
            return(0);

      if (((data1 = getc(handle->file)) == EOF)
         || ((data2 = getc(handle->file)) == EOF))
         return(0);

      *width  = data2 * 256 + data1;

      if (((data1 = getc(handle->file)) == EOF)
         || ((data2 = getc(handle->file)) == EOF))
         return(0);

      for (i = 0 ; i < 2 ; i++)
         if (getc(handle->file) == EOF)
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

      for (i = 0; i < 10; i++)       /* 00, 00, 02, then 7 00's... */
         if (i == 2)
            putc(i, handle->file);
         else
            putc(0, handle->file);

      putc(First_Line % 256, handle->file); /* y origin set to "First_Line" */

      putc(First_Line / 256, handle->file);

      putc(*width % 256, handle->file);  /* write width and height */
      putc(*width / 256, handle->file);
      putc(*height % 256, handle->file);
      putc(*height / 256, handle->file);
      putc(24, handle->file);                /* 24 bits/pixel (16 million col */
      putc(32, handle->file);                /* Bitmask, pertinent bit: top-d */

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

      break;
   }

   return(1);
}

void Write_Targa_Line (handle, line_data, line_number)
FILE_HANDLE *handle;
COLOUR *line_data;
int line_number;
{
   register int x;

   for (x = 0; x < handle->width; x++) {
      putc((int) floor (line_data[x].Blue * 255.0), handle->file);
      putc((int) floor (line_data[x].Green * 255.0), handle->file);
      putc((int) floor (line_data[x].Red * 255.0), handle->file);
   }

   if (handle->buffer_size == 0) {
      fflush(handle->file);                       /* close and reopen file for */
      handle->file = freopen(handle->filename, APPEND_FILE_STRING,
         handle->file);                /* integrity in case we crash*/
   }
}

int Read_Targa_Line (handle, line_data, line_number)
FILE_HANDLE *handle;
COLOUR *line_data;
int *line_number;
{
   int x, data;

   for (x = 0; x < handle->width; x++) {

      /* Read the BLUE data byte.  If EOF is reached on the first character read,
      then this line hasn't been rendered yet.  Return 0.  If an EOF occurs
      somewhere within the line, this is an error - return -1. */

      if ((data = getc(handle->file)) == EOF)
         if (x == 0)
            return (0);
         else
            return (-1);

      line_data[x].Blue = (DBL) data / 255.0;

      /* Read the GREEN data byte. */
      if ((data = getc(handle->file)) == EOF)
         return (-1);
      line_data[x].Green = (DBL) data / 255.0;


      /* Read the RED data byte. */
      if ((data = getc(handle->file)) == EOF)
         return (-1);
      line_data[x].Red = (DBL) data / 255.0;
   }

   *line_number = Targa_Line_Number++;
   return(1);
}

void Close_Targa_File (handle)
FILE_HANDLE *handle;
{
   if(handle->file)
      fclose (handle->file);
   if (handle->buffer)
      free (handle->buffer);
}

int Read_Targa_Int_Line(handle, line_data)
FILE_HANDLE *handle;
IMAGE_LINE *line_data;
{
   int x, data;

   if (((line_data->red = (unsigned char *) malloc(handle->width))==NULL) ||
      ((line_data->green = (unsigned char *) malloc(handle->width))==NULL) ||
      ((line_data->blue = (unsigned char *) malloc(handle->width))==NULL)) {
      fprintf(stderr, "Cannot allocate memory for picture: %s\n",
         handle->filename);
      close_all();
      exit(1);
   }

   for (x = 0; x < handle->width; x++) {
      if ((data = getc(handle->file)) == EOF)
         if (x == 0) return (0);
         else return (-1);
      line_data->blue[x] = data;
      if ((data = getc(handle->file)) == EOF) return (-1);
      line_data->green[x] = data;
      if ((data = getc(handle->file)) == EOF) return (-1);
      line_data->red[x] = data;
   }
   return(1);
}

void
Read_Targa_Image(Image, name)
IMAGE *Image;
char *name;
{
   int row;
   FILE_HANDLE handle;

   if ((handle.file = Locate_File(name, READ_FILE_STRING)) == NULL) {
      fprintf (stderr, "Cannot open Targa file %s\n", name);
      close_all();
      exit(1);
   }

   Open_Targa_File(&handle, name, &Image->iwidth, &Image->iheight,
      0, READ_MODE);

   handle.width  = Image->iwidth;
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

   for (row=0;
              row<Image->iheight &&
              Read_Targa_Int_Line(&handle, &Image->data.rgb_lines[row]);
              row++) {
   }
   fclose(handle.file);
}

