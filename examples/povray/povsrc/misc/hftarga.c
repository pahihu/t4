#include <stdio.h>
#include <stdlib.h>

static FILE *TargaFile;

void
TargaOpen(char *filename, int x, int y)
{
   FILE *file;
   unsigned char tgaheader[18];
   if ((file = fopen(filename, "wb")) == NULL) {
      printf("Failed to open Targa file: %s\n", filename);
      exit(1);
      }
   memset(tgaheader, 0, 18);
   tgaheader[2] = 2;
   tgaheader[12] = (unsigned char)(x & 0xFF);
   tgaheader[13] = (unsigned char)((x >> 8) & 0xFF);
   tgaheader[14] = (unsigned char)(y & 0xFF);
   tgaheader[15] = (unsigned char)((y >> 8) & 0xFF);
   tgaheader[16] = 24;
   tgaheader[17] = 0x20;
   fwrite(tgaheader, 18, 1, file);
   TargaFile = file;
}

void
TargaWrite(r, g, b)
   unsigned char r, g, b;
{
   fputc(b, TargaFile);
   fputc(g, TargaFile);
   fputc(r, TargaFile);
}

void
TargaClose()
{
   fclose(TargaFile);
}
