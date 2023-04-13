/****************************************************************************
*                raw.c
*
*  This module contains the code to read and write the RAW file format
*  The format is as follows:
*
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

struct Raw_File_Handle_Struct {
   FILE_HANDLE root;
   FILE *Red_File, *Green_File, *Blue_File;
   char *Red_Buffer, *Green_Buffer, *Blue_Buffer;
   int line_number;
};

typedef struct Raw_File_Handle_Struct RAW_FILE_HANDLE;

FILE_HANDLE *Get_Raw_File_Handle()
{
   RAW_FILE_HANDLE *handle;

   if ((handle = (RAW_FILE_HANDLE *) malloc(sizeof(RAW_FILE_HANDLE))) == NULL) {
      fprintf (stderr, "Cannot allocate memory for output file handle\n");
      return(NULL);
   }

   handle->root.Default_File_Name_p = Default_Raw_File_Name;
   handle->root.Open_File_p = Open_Raw_File;
   handle->root.Write_Line_p = Write_Raw_Line;
   handle->root.Read_Line_p = Read_Raw_Line;
   handle->root.Close_File_p = Close_Raw_File;
   return ((FILE_HANDLE *) handle);
}

char *Default_Raw_File_Name()
{
   return ("data");
}

int Open_Raw_File (handle, name, width, height, buffer_size, mode)
FILE_HANDLE *handle;
char *name;
int *width;
int *height;
int buffer_size;
int mode;
{
   RAW_FILE_HANDLE *raw_handle;
   char file_name[256];

   handle->mode = mode;
   handle->filename = name;
   raw_handle = (RAW_FILE_HANDLE *) handle;
   raw_handle->line_number = 0;

   switch (mode) {
   case READ_MODE:
      strcpy (file_name, name);
      strcat (file_name, RED_RAW_FILE_EXTENSION);

      if ((raw_handle->Red_File=fopen(file_name,READ_FILE_STRING)) == NULL)
         return(0);

      strcpy (file_name, name);
      strcat (file_name, GREEN_RAW_FILE_EXTENSION);

      if ((raw_handle->Green_File=fopen(file_name,READ_FILE_STRING))==NULL)
         return(0);

      strcpy (file_name, name);
      strcat (file_name, BLUE_RAW_FILE_EXTENSION);

      if ((raw_handle->Blue_File=fopen(file_name,READ_FILE_STRING))== NULL)
         return(0);

      if (buffer_size != 0) {
         if ((raw_handle->Red_Buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (raw_handle->Red_File, raw_handle->Red_Buffer, _IOFBF, buffer_size);
      }

      if (buffer_size != 0) {
         if ((raw_handle->Green_Buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (raw_handle->Green_File, raw_handle->Green_Buffer, _IOFBF, buffer_size);
      }

      if (buffer_size != 0) {
         if ((raw_handle->Blue_Buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (raw_handle->Blue_File, raw_handle->Blue_Buffer, _IOFBF, buffer_size);
      }

      handle->width = *width;
      handle->height = *height;
      handle->buffer_size = buffer_size;
      break;

   case WRITE_MODE:
      strcpy (file_name, name);
      strcat (file_name, RED_RAW_FILE_EXTENSION);

      if ((raw_handle->Red_File = fopen (file_name, WRITE_FILE_STRING)) == NULL)
         return(0);

      if (buffer_size != 0) {
         if ((raw_handle->Red_Buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (raw_handle->Red_File, raw_handle->Red_Buffer, _IOFBF, buffer_size);
      }

      strcpy (file_name, name);
      strcat (file_name, GREEN_RAW_FILE_EXTENSION);

      if ((raw_handle->Green_File = fopen (file_name, WRITE_FILE_STRING)) == NULL)
         return(0);

      if (buffer_size != 0) {
         if ((raw_handle->Green_Buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (raw_handle->Green_File, raw_handle->Green_Buffer, _IOFBF, buffer_size);
      }

      strcpy (file_name, name);
      strcat (file_name, BLUE_RAW_FILE_EXTENSION);

      if ((raw_handle->Blue_File = fopen (file_name, WRITE_FILE_STRING)) == NULL)
         return(0);

      if (buffer_size != 0) {
         if ((raw_handle->Blue_Buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (raw_handle->Blue_File, raw_handle->Blue_Buffer, _IOFBF, buffer_size);
      }

      handle->width = *width;
      handle->height = *height;
      handle->buffer_size = buffer_size;

      break;

   case APPEND_MODE:
      strcpy (file_name, name);
      strcat (file_name, RED_RAW_FILE_EXTENSION);

      if ((raw_handle->Red_File=fopen(file_name,APPEND_FILE_STRING))==NULL)
         return(0);

      if (buffer_size != 0) {
         if ((raw_handle->Red_Buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (raw_handle->Red_File, raw_handle->Red_Buffer, _IOFBF, buffer_size);
      }

      strcpy (file_name, name);
      strcat (file_name, GREEN_RAW_FILE_EXTENSION);

      if ((raw_handle->Green_File=fopen(file_name,APPEND_FILE_STRING))==NULL)
         return(0);

      if (buffer_size != 0) {
         if ((raw_handle->Green_Buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (raw_handle->Green_File, raw_handle->Green_Buffer, _IOFBF, buffer_size);
      }

      strcpy (file_name, name);
      strcat (file_name, BLUE_RAW_FILE_EXTENSION);

      if ((raw_handle->Blue_File=fopen(file_name,APPEND_FILE_STRING))==NULL)
         return(0);

      if (buffer_size != 0) {
         if ((raw_handle->Blue_Buffer = malloc (buffer_size)) == NULL)
            return(0);

         setvbuf (raw_handle->Blue_File, raw_handle->Blue_Buffer, _IOFBF, buffer_size);
      }

      handle->width = *width;
      handle->height = *height;
      handle->buffer_size = buffer_size;

      break;
   }
   return(1);
}

void Write_Raw_Line (handle, line_data, line_number)
FILE_HANDLE *handle;
COLOUR *line_data;
int line_number;
{
   register int x;
   RAW_FILE_HANDLE *raw_handle;
   char file_name[256];

   raw_handle = (RAW_FILE_HANDLE *) handle;

   for (x = 0 ; x < handle->width ; x++)
      putc((int) floor (line_data[x].Red * 255.0), raw_handle->Red_File);

   for (x = 0 ; x < handle->width ; x++)
      putc((int) floor (line_data[x].Green * 255.0), raw_handle->Green_File);

   for (x = 0 ; x < handle->width ; x++)
      putc((int) floor (line_data[x].Blue * 255.0), raw_handle->Blue_File);

   if (handle->buffer_size == 0) {
      fflush(raw_handle->Red_File);                       /* close and reopen file for */
      strcpy (file_name, handle->filename);
      strcat (file_name, RED_RAW_FILE_EXTENSION);
      raw_handle->Red_File = freopen(file_name, APPEND_FILE_STRING,
         raw_handle->Red_File);                /* integrity in case we crash*/

      fflush(raw_handle->Green_File);                       /* close and reopen file for */
      strcpy (file_name, handle->filename);
      strcat (file_name, GREEN_RAW_FILE_EXTENSION);
      raw_handle->Green_File = freopen(file_name, APPEND_FILE_STRING,
         raw_handle->Green_File);                /* integrity in case we crash*/

      fflush(raw_handle->Blue_File);                       /* close and reopen file for */
      strcpy (file_name, handle->filename);
      strcat (file_name, BLUE_RAW_FILE_EXTENSION);
      raw_handle->Blue_File = freopen(file_name, APPEND_FILE_STRING,
         raw_handle->Blue_File);                /* integrity in case we crash*/
   }
}

int Read_Raw_Line (handle, line_data, line_number)
FILE_HANDLE *handle;
COLOUR *line_data;
int *line_number;
{
   int data, i;
   RAW_FILE_HANDLE *raw_handle;

   raw_handle = (RAW_FILE_HANDLE *) handle;

   for (i = 0 ; i < handle->width ; i++) {
      if ((data = getc(raw_handle->Red_File)) == EOF) {
         if (i == 0)
            return(0);
         else
            return(-1);
      }

      line_data[i].Red = (DBL) data / 255.0;
   }

   for (i = 0 ; i < handle->width ; i++) {
      if ((data = getc(raw_handle->Green_File)) == EOF)
         return(-1);

      line_data[i].Green = (DBL) data / 255.0;
   }

   for (i = 0 ; i < handle->width ; i++) {
      if ((data = getc(raw_handle->Blue_File)) == EOF)
         return(-1);

      line_data[i].Blue = (DBL) data / 255.0;
   }

   *line_number = raw_handle->line_number++;
   return (1);
}

void Close_Raw_File (handle)
FILE_HANDLE *handle;
{
   RAW_FILE_HANDLE *raw_handle;

   raw_handle = (RAW_FILE_HANDLE *) handle;
   if(raw_handle->Red_File)
      fclose (raw_handle->Red_File);
   if(raw_handle->Green_File)
      fclose (raw_handle->Green_File);
   if(raw_handle->Blue_File)
      fclose (raw_handle->Blue_File);

   if (handle->buffer_size != 0) {
      free (raw_handle->Red_Buffer);
      free (raw_handle->Green_Buffer);
      free (raw_handle->Blue_Buffer);
   }
}
