/****************************************************************************
*                   ibm.c
*
*  This module implements the IBM-PC specific routines for POV-Ray.
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

/* Original IBM VGA "colour" output routines for MS/DOS by Aaron A. Collins.

   This will deliver approximate colorings using HSV values for the selection.
   The palette map is divided into 4 parts - upper and lower half generated
   with full and half "value" (intensity), respectively.  These halves are
   further halved by full and half saturation values of each range (pastels).
   There are three constant colors, black, white, and grey.  They are used
   when the saturation is low enough that the hue becomes undefined, and which
   one is selected is based on a simple range map of "value".  Usage of the
   palette is accomplished by converting the requested color RGB into an HSV
   value.  If the saturation is too low (< .25) then black, white or grey is
   selected.  If there is enough saturation to consider looking at the hue,
   then the hue range of 1-63 is scaled into one of the 4 palette quadrants
   based on its "value" and "saturation" characteristics.

   Further SVGA, MVGA mods by Aaron A. Collins:
   SVGA, MVGA assembler routines originally by John Bridges.
   VESA assembler routines from FRACTINT, by The Stone Soup Group
   AT&T VDC600 SVGA mods to DKB Trace 2.01 module IBM.C by John Gooding

   This file now represents the collective wisdom of the VGAKIT34 package,
   with support for all of the SVGA types known to mankind.  Seriously,
   VGAKIT34 is an excellent technical reference for IBM programmers wishing
   to do any sort of SVGA video access, and it encompasses nearly all of the
   SVGA adapters in use today.  It was written by John Bridges, a.k.a.
   CIS:73307,606, GENIE:J.BRIDGES.  It was originally coded in IBM 80x86
   assembler, and since DKBTrace is a completely "C"-based package, I have
   converted John's assembler routines all into "C".  These may be a tad bit
   slower, but they should be compatible across a wide variety of 80x86/(S)VGA
   machines.  Note if you have a regular cheapo VGA card like myself, included
   is "MODE13x" or MVGA (modified VGA) mode (some call it "tweaked", but I
   call it "Simulated SVGA"), which gives 360x480 on any reasonably register-
   compatible plain vanilla VGA card.  This mode gives a good simulated 640 by
   480 screen resolution.  I did not implement all the neat hi-res modes of
   all the various SVGA adapters, if you select a trace size bigger than the
   program and/or card can handle (most likely 640x480), it is dynamically
   scaled to fit the available resolution, so you'll be able to see a rough
   approximation of an 800x600 trace even on any el-cheapo VGA card at 320x200
   resolution.  The VESA VGA mode was freely adapted from FRACTINT, whose GIF
   reading routines we are already using in DKBTrace.  I hope my conversion
   of it works properly.

   There is still a reported problem with the EVEREX autodetect returning
   TRIDENT.  In fact EVEREX uses a TRIDENT chip set, but apparently there
   is some difference in operation.  There are cryptic diagnostic messages
   such as T0000, etc. printed as a result of the autodetection routines
   to help track down why the error is happening.  If you are experiencing
   problems with EVEREX or TRIDENT, make note of the letter-4 digit code you
   are given.  There is now an autodetect for VDC600 that I hope will work
   universally.  A similar problem as the EVEREX exists, in that the VDC600
   is detected as a PARADISE because it uses the PARADISE chip set.  I am now
   looking for what I believe to be the model number in the BIOS ROM of the
   VDC600 to differentiate between the two.  I hope this works  with all
   VDC600's, as I only had one example to work from.  Please send all bug
   reports to Aaron Collins at the "You Can Call Me RAY" BBS, the number is
   in the authors.txt document.


   POV-Ray Enhancements:

   (S)VGA B&W 64 Greyscale code courtesy of Grant Hess 6/21/91

   16/24-bit HGWS/TIGA/TRUECOLOR code courtesy of Jay S. Curtis 11/15/90

   VGA "332" palette routines courtesy of Alexander Enzmann 7/20/91

   Additional support for VESA and Tseng Labs 256 and 32768 color modes by
   Charles Marslett  (CIS: 75300,1636).  Resolution is selected to be the
   lowest possible and still fit the image entirely on the screen. 4/28/91

   ATI VGA Wonder XL Support for 32,768 color mode and all other SVGA modes
   by Randy Antler.  Also image centering, outline box, and forces SVGA mode
   for small images.  Minor bug fixes and cleanups as well. 9/11/91

   Combined IBMPRO.C and IBM.C into 1 file. 
   B.Pulver 10/20/91

   Corrected defects in the centering and high color support for VESA modes
   (particularly necessary for the newer 24-bit color cards and S3 based
   Windows accellerator cards) -- Charles Marslett, 12/8/91 to 4/25/92.

*/

#define DITHERED        /* Defined to test Doug Muir's dithering code */

#include <dos.h>        /* MS-DOS specific - for int86() REGS struct, etc. */
#include <stdarg.h>
#include <time.h>
#include "frame.h"
#include "povproto.h"


#ifndef _INTELC32_
#ifndef __386__
#ifndef GCCDOS
  #include "tiga.h"       /* HGWS/TIGA function library include file */
  #include "tigadefs.h"   /* HGWS/TIGA function library include file */
  #include "extend.h"     /* HGWS/TIGA function library include file */
#endif
#endif
#endif

#ifdef __TURBOC__
  extern unsigned _stklen = 12288; /* fairly large stack for HEAVY recursion */
  #define CLD asm cld
  #define _asm asm
  #define _far far
#else
#ifdef __WATCOMC__
  #define CLD clear_direction()
  unsigned short __8087cw = IC_AFFINE | RC_NEAR | PC_64  | 0x007F;
  void clear_direction(void);
  #pragma aux clear_direction = 0xFC parm [] modify nomemory;
#ifdef __386__
  #define ax eax
  #define bx ebx
  #define cx ecx
  #define dx edx
  #define di edi
  #define int86 int386
  #define int86x int386x
#endif
#else

#ifdef _INTELC32_
  #pragma interrupt (int10)
  #pragma noansi
  #include <stk.h>
  #define CLD cld()

  void int10();
  void (*prev_int10)();

  void cld (void)
  {
    unsigned int flags;
    flags = _getflags();
    flags = flags & (!_FLAG_DIRECTION);
    _setflags (flags);
  }
#else
#ifdef __ZTC__
  #include <math.h>
  #include <int.h>

  unsigned _stack = 12032;

  #define CLD
  #define min(x,y) ((x) < (y)? (x): (y))

  #ifdef DOS386
    extern unsigned short _x386_zero_base_selector;
    #define _disable()
    #define _enable()
    #define _cdecl
  #else
  #ifdef DOS16RM
    void *D16SegAbsolute(long absadr, short size);
    void D16SegCancel(void *segptr);
    #define _disable()
    #define _enable()
    unsigned char *display_base;
  #else
    #define _disable() int_off()
    #define _enable()  int_on()
  #endif
  #endif

#else
#ifdef GCCDOS
  #include <errno.h>
  #include <pc.h>
  #include <ctype.h>

  unsigned char *find_go32(int *go32_offset);

  #define _cdecl
  #define _far
  #define CLD
#else
  #define CLD _asm cld
#endif
#endif
#endif
#endif
#endif

#ifdef __WATCOMC__
  int matherr(struct exception *);
#else
#ifdef GCCDOS
  int matherr(struct libm_exception *);
#else
  int _cdecl matherr(struct exception *);
#endif
#endif


#ifdef DITHERED
signed char r_err[2][1026];             /* Allow up to 1024x??? resolutions */
signed char g_err[2][1026];
signed char b_err[2][1026];
#endif

/* The supported VGA adapter types 1 - 9, A - Z.  0 is for auto-detect. */

#define BASIC_VGA       1               /* 1 - Tested: AAC */
#define MODE13x         2               /* 2 - Tested: AAC */
#define TSENG3          3               /* 3 - Tested: William Minus */
#define TSENG4          4               /* 4 - Tested: William Minus */
#define VDC600          5               /* 5 - Tested: John Gooding */
#define OAKTECH         6               /* 6 - Untested */
#define VIDEO7          7               /* 7 - Untested */
#define CIRRUS          8               /* 8 - Tested: AAC */
#define PARADISE        9               /* 9 - Tested: John Degner */
#define AHEADA          17              /* A - Untested */
#define AHEADB          18              /* B - Untested */
#define CHIPSTECH       19              /* C - Untested */
#define ATIVGA          20              /* D - Tested: William Earl */
#define EVEREX          21              /* E - Tested: A+B problem - Larry Minton */
#define TRIDENT         22              /* F - Tested: A problem - Alexander Enzmann */
#define VESA            23              /* G - Tested: Charles Marslett/AAC */
#define ATIXL           24              /* H - Tested: Randy Antler */

#define TRUECOLOR       31              /* #O or higher isn't an (S)VGA */
#define TIGA_16         32              /* O - Tested: Jay S. Curtis */
#define TIGA_24         33              /* P - Tested: Jay S. Curtis */
#define TRUE_640        34              /* Q - 640 x 480 truecolor Tested: Jay S. Curtis */
#define TRUE_800        35              /* R - 800 x 600 truecolor Tested: Jay S. Curtis */
#define TRUE_1024       36              /* S - 1024 x 768 truecolor Tested: Jay S. Curtis */


#define MISCOUT         0x3c2           /* VGA chip msic output reg. addr */
#define SEQUENCER       0x3c4           /* VGA chip sequencer register addr */
#define CRTC            0x3d4           /* VGA chip crt controller reg addr */


char *vga_names[] = 
    {
    "",                                 /* '0' is autodetect */
    "Standard VGA",                     /* '1'-'9' */
    "Simulated SVGA",
    "Tseng Labs 3000 SVGA",
    "Tseng Labs 4000 SVGA",
    "AT&T VDC600 SVGA",
    "Oak Technologies SVGA",
    "Video 7 SVGA",
    "Video 7 Vega (Cirrus) VGA",
    "Paradise SVGA",
    "",                                 /* misc ASCII */
    "",
    "",
    "", /* reserved */
    "",
    "",
    "",
    "Ahead Systems Ver. A SVGA",        /* 'A' - 'H' */
    "Ahead Systems Ver. B SVGA",
    "Chips & Technologies SVGA",
    "ATI SVGA",
    "Everex SVGA",
    "Trident SVGA",
    "VESA Standard SVGA",
    "ATI VGA Wonder XL 32K Color SVGA",
    "",
    "",
    "", /* spare SVGAs */
    "",
    "",
    "",
    "Hercules GWS/TIGA 16-bit", /* 'O' on is reserved for non-SVGA cards */
    "Hercules GWS/TIGA 24-bit",
    "TrueColor 640 x 480",
    "TrueColor 800 x 600",
    "TrueColor 1024 x 768"
    };


unsigned int vptbl[] =          /* CRTC register values for MODE13x */
    {
    0x6b00,     /* horz total */
    0x5901,     /* horz displayed */
    0x5a02,     /* start horz blanking */
    0x8e03U,    /* end horz blanking */
    0x5e04,     /* start h sync */
    0x8a05U,    /* end h sync */
    0x0d06,     /* vertical total */
    0x3e07,     /* overflow */
    0x4009,     /* cell height */
    0xea10U,    /* v sync start */
    0xac11U,    /* v sync end and protect cr0-cr7 */
    0xdf12U,    /* vertical displayed */
    0x2d13,     /* offset */
    0x0014,     /* turn off dword mode */
    0xe715U,    /* v blank start */
    0x0616,     /* v blank end */
    0xe317U     /* turn on byte mode */
};

int screen_height, screen_width;
int svga_width = 640;           /* width of a scan line on the screen */
int svga_height = 480;          /* number of scan lines on the screen */
int svga_yincr = 640;           /* width of a scan line in video memory */
int lastx, lasty, lastline;     /* Pixel / Line Caches */
unsigned int x_off = 0;         /* Offsets for image centering option */
unsigned int y_off = 0;
int whichvga = BASIC_VGA;       /* BASIC_VGA mode by default */
int vga_512K = FALSE;           /* Flag for whether or not >= 512K VGA mem */
int bpp = 8;                    /* Number of bits per pixel */
unsigned char cur_page = 255;   /* SVGA current page (bank) number */
unsigned int map_code = 0;      /* default map code is 0 */
unsigned long gran = 65536L;    /* SVGA granule size (64K by default) */
#ifdef _INTELC32_
unsigned char *answer;          /* answer area for VESA BIOS calls */
#else
#ifdef DOS16RM
unsigned char *answer;          /* answer area for VESA BIOS calls */
static char save[260];
unsigned long tl;
#else
#ifdef GCCDOS
volatile unsigned char *answer; /* answer area for VESA BIOS calls */
int di_val;
#else
#ifdef __WATCOMC__
#ifdef __386__
unsigned int vesabuffer;        /* save area for the DOS segment pointer */
unsigned char *answer;          /* answer area for VESA BIOS calls */
#else
unsigned char answer[257];      /* answer area for VESA BIOS calls */
#endif
#else
unsigned char answer[257];      /* answer area for VESA BIOS calls */
#endif
#endif
#endif
#endif
int vesamode;                   /* cur. VESA BIOS supported mode */

int dither_matrix[4][4]=
	{{0,8,2,10},            /* HGWS/TIGA dither matrix */
	 {12,4,14,6},           /* this may not be needed? */
	 {3,11,1,9},
	 {15,7,13,5}
	};
char hpt[3];                    /* HWGS/TIGA information passing area */
char _far *hptr=hpt;            /* HWGS/TIGA information passing area ptr */
#ifndef _INTELC32_
#ifndef __386__
#ifndef GCCDOS
CONFIG config;
#endif
#endif
#endif
DBL Height_Adjust = 1.0;        /* fudge factors for SVGA scaling */
DBL Width_Adjust = 1.0;

/* VESA VBE standard mode numbers */

struct {
   unsigned short modenum;
   unsigned short width;
   unsigned short height;
   unsigned short bpp;
   unsigned short gsize;
   } modelist[] = {{0x100, 640, 400, 8, 0}, {0x101, 640, 480, 8, 0},
		   {0x103, 800, 600, 8, 0}, {0x105, 1024, 768, 8, 0},
		   {0x107, 1280, 1024, 8, 0}, {0x10D, 320, 200, 16, 5},
		   {0x10E, 320, 200, 16, 6}, {0x10F, 320, 200, 24, 8},
		   {0x110, 640, 480, 16, 5}, {0x111, 640, 480, 16, 6},
		   {0x112, 640, 480, 24, 8}, {0x113, 800, 600, 16, 5},
		   {0x114, 800, 600, 16, 6}, {0x115, 800, 600, 24, 8},
		   {0x116, 1024, 768, 16, 5}, {0x117, 1024, 768, 16, 6},
		   {0x118, 1024, 768, 24, 8}, {0x119, 1280, 1024, 16, 5},
		   {0x11A, 1280, 1024, 16, 6}, {0x11B, 1280, 1024, 24, 8}};

unsigned int bitmask[] = {0x0000, 0x0001, 0x0003, 0x0007, 0x000F, 0x001F,
      0x003F, 0x007F, 0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF,
      0x7FFF, 0xFFFF};

int r_shift, g_shift, b_shift;
unsigned int r_mask, g_mask, b_mask;


extern unsigned int Options;
extern char DisplayFormat, PaletteOption, Color_Bits;


int AutodetectVGA(void);
int cirrus(void);
int chkbank(unsigned int, unsigned int);
void palette_init(void);
void newbank(void);
void set_palette_register(unsigned, unsigned, unsigned, unsigned);
void hsv_to_rgb(DBL, DBL, DBL, unsigned *, unsigned *, unsigned *);
void rgb_to_hsv(unsigned, unsigned, unsigned, DBL *, DBL *, DBL *);
void atiplot(int, int, int, int, int);
void atibank(int);
void box(int, int, int, int);
int _cdecl box_fprintf( FILE *stream, char *format,...);

#ifdef DOS386
void get_real_buf(short *real_buf, unsigned char _far **ptr);

static short real_buf[2];
static unsigned char _far *real_ptr;
#endif

void display_init(width, height) /* Set video to requested or best mode */
int width, height;
   {
   union REGS inr, outr;
   struct SREGS segs;
   unsigned char _far *fp;
#ifdef DOS386
   unsigned int tmp_word;
#else
   unsigned short _far *mp;
#endif
   unsigned int u, granule, vesamodes[64];
/* unsigned int tmp_word, tmp_word1; */
   int i, maxvesamode;
   time_t l, lt;
   int show_display_type = FALSE;

   if (DisplayFormat == '?'){  
      DisplayFormat = '0';
      show_display_type = TRUE;
      }
#ifdef __WATCOMC__
#ifdef __386__
   inr.x.ax = 0x4800;
   inr.x.bx = 17;
   int86(0x21, &inr, &outr);
   vesabuffer = outr.x.ax;
   answer = MK_FP(outr.x.ax, 0);
#endif
#endif
#ifdef DOS16RM
   display_base = (unsigned char *)D16SegAbsolute(0xA0000, 0);
   answer = (unsigned char *)D16SegAbsolute(0x00580, 0x110);
   for (u=0; u < 257; u++)
      save[u] = answer[u];
#endif
#ifdef GCCDOS
   if (DisplayFormat == '0' || DisplayFormat == VESA + '0')
      answer = find_go32(&di_val);
   else
      answer = NULL;
#endif
#ifdef _INTELC32_
   prev_int10 = _dos_getvect ( 0x10 );     /* Save previous handler pointer */
   _dos_setvect ( 0x80000010, int10 );     /* Install user Int 10h handler */
   if (_dos_allocmem(17, &answer) != 0) { 
      printf ("Memory allocation failure for VESA detect.\n");
      exit(0);
      }
#endif
#ifdef DOS386
   get_real_buf(real_buf, &real_ptr);
#endif

   lastline = -1;               /* make sure we display the 1st line... */
   screen_height = height;      /* requested screen height and width */
   screen_width = width;

#ifdef DITHERED
   for (i=0; i < screen_width+2; i++) {
      r_err[0][i] = 0;
      r_err[1][i] = 0;
      g_err[0][i] = 0;
      g_err[1][i] = 0;
      b_err[0][i] = 0;
      b_err[1][i] = 0;
      }
#endif
    
   if (DisplayFormat != '0')    /* if not 0, some display type specified */
	whichvga = (int)(DisplayFormat - '0');  /* de-ASCII-fy selection */
   else {
	whichvga = AutodetectVGA();
	lt = l = time(&l);
	if (show_display_type) {   /* If display format is ? */
	    printf("Display detected: (%c) %s Adapter", whichvga + '0', vga_names[whichvga]);
	    if (whichvga < TRUECOLOR)   /*  if not a True-Color graphics card */
		printf(", with %s 512K RAM\n", vga_512K ? ">=" : "<");
	    else
		printf("\nPausing...\n");
	    while (time(&l) < lt + 5)   /* display detected VGA type for 5 secs */
	       ;
	    }
	if (!vga_512K)                  /* not enough RAM for 640 x 480? */
	    whichvga = MODE13x;         /* then try for next best mode... */
	}

   if (whichvga < TRUECOLOR) /* If an (S)VGA (not True-Color graphics card) */
	{
	if (whichvga == CIRRUS) /* Register Compatible VGA? */
	    whichvga = MODE13x; /* MODE13x if > 320x200, else... */

	if (screen_height <= 200 && screen_width <= 320 && PaletteOption != HICOLOR)
	    whichvga = BASIC_VGA;

	if (whichvga == TSENG4 && PaletteOption == HICOLOR)
	    {
	    inr.x.ax = 0x10F1;          /* check and see if it's true... */
#ifdef DOS386
	    int86_real(0x10, &inr, &inr);
#else
	    int86(0x10, &inr, &inr);
#endif
	    if ((int) inr.h.al != 0x10 || (int)inr.h.bl == 0)
		{
		printf("Error - High Color Palette Option Unavailable\n");
		exit(1);
		}
	    }
	}

   switch (whichvga)
	{
	case MODE13x:
	    inr.x.ax = 0x0013;   /* setup to VGA 360x480x256 (mode 13X) */
	    svga_width = 360;           /* Fake 640 mode actually is 360 */
	    svga_yincr = 90;
	    break;
	case VDC600:
	    inr.x.ax = 0x005E;   /* setup to VGA 640x400x256 (mode 5EH) */
	    svga_height = 400;   /* This is the only SVGA card w/400 Lines */
	    break;
	case OAKTECH:
	    inr.x.ax = 0x0053;   /* setup to VGA 640x480x256 most SVGAs */
	    break;
	case AHEADA:
	case AHEADB:
	    inr.x.ax = 0x0061;
	    break;
	case EVEREX:
	    inr.x.ax = 0x0070;   /* BIOS Mode 0x16 for EV-678? */
	    inr.h.bl = 0x30;
	    break;
	case ATIVGA:
	    if (screen_width <= 1024 && screen_height <= 768)
		{
		inr.x.ax = 0x0064;
		svga_width = 1024;
		svga_height = 768;
		svga_yincr = 1024;
		}
	    if (screen_width <= 800 && screen_height <= 600)
		{
		inr.x.ax = 0x0063;
		svga_width = 800;
		svga_height = 600;
		svga_yincr = 800;
		}
	    if (screen_width <= 640 && screen_height <= 480)
		{
		inr.x.ax = 0x0062;
		svga_width = 640;
		svga_height = 480;
		svga_yincr = 640;
		}
	    break;
	case ATIXL:
	    inr.x.ax = 0x0072;
	    svga_width = 640;
	    svga_height = 480;
	    svga_yincr = 640;
	    break;
	case TRIDENT:
	    inr.x.ax = 0x005d;
	    break;
	case VIDEO7:
	    inr.x.ax = 0x6f05;
	    inr.h.bl = 0x67;
	    break;
	case CHIPSTECH:
	    if (screen_height <= 400)
	    {
	       inr.x.ax = 0x0078;
	       svga_height = 400;
	    }
	    else
		inr.x.ax = 0x0079;
	    break;
	case PARADISE:
	    inr.x.ax = 0x005f;
	    break;
	case TSENG3:
	case TSENG4:
	    if (screen_height <= 200 && screen_width <= 320)
	    {
		 inr.x.ax = 0x0013;  /* setup to VGA 320x200 for 32K mode  */
		 svga_width = 320;   /* allow scaling to run at 320x200 */
		 svga_height = 200;
		 svga_yincr = 320;
	    }
	    else if (screen_height <= 350 && screen_width <= 640)
	    {
		 inr.x.ax = 0x002D;
		 svga_height = 350;
	    }
	    else if (screen_height <= 400 && screen_width <= 640 && whichvga == TSENG4)
	    {
		 inr.x.ax = 0x0078;
		 svga_height = 400;
	    }
	    else if (screen_height <= 480 && screen_width <= 640)
	    {
		 inr.x.ax = 0x002E;
	    }
	    else        /* 800 by 600 and beyond */
	    {
		 inr.x.ax = 0x0030;
		 svga_width = 800;
		 svga_height = 600;
		 svga_yincr = 800;
	    }
	    if ((screen_height > 600 || screen_width > 800) && whichvga == TSENG4)
	    {
		if (PaletteOption == HICOLOR)
		{
		    inr.x.ax = 0x0030; /* Limit to 800x600 in HiColor mode */
		    svga_width = 800;
		    svga_height = 600;
		    svga_yincr = 800;
		}
		else
		{
		    inr.x.ax = 0x0038;
		    svga_width = 1024;
		    svga_height = 768;
		    svga_yincr = 1024;
		}
	    }
	    break;
	case VESA:
	    inr.x.ax = 0x4F00;      /* Call function 0 to get VGA info */
#ifdef _INTELC32_
	    for (u=0; u<256; u++)
		answer[u] = 0;
	    inr.w.edi = (unsigned)answer;  
	    int86(0x10, &inr, &outr);         /* Get thru int 10h inr outr */
#else
#ifdef DOS386
	    for (u=0; u<256; u++)
		real_ptr[u] = 0;
	    inr.x.di = real_buf[0];         /* deposit results here */
	    segread(&segs);                 /* get our DS, etc. */
	    segs.es = real_buf[1];          /* get segment of answer */
	    int86x_real(0x10, &inr, &outr, &segs);
	    for(u=0; u<sizeof(answer); u++)
		answer[u] = real_ptr[u];
#else
#ifdef GCCDOS
	    for (u=0; u<256; u++)
		answer[u] = 0;
	    inr.x.di = di_val;              /* deposit results here */
	    int86x(0x10, &inr, &outr, &segs);
#else
#ifdef DOS16RM
	    for (u=0; u<256; u++)
		answer[u] = 0;
	    segread(&segs);                 /* get our DS, etc. */
	    inr.x.di = 0x580;               /* deposit results here */
	    segs.es = 0;                    /* get segment of answer */
	    int86x(0x10, &inr, &outr, &segs);
#else
	    for (u=0; u<256; u++)
		answer[u] = 0;
	    inr.x.di = FP_OFF(answer);      /* deposit results here */
	    segread(&segs);                 /* get our DS, etc. */
	    segs.es = FP_SEG(answer);       /* get segment of answer */
	    int86x(0x10, &inr, &outr, &segs);
#endif
#endif
#endif
#endif
	    if ( outr.x.ax != 0x004F || answer[0] != 'V' ||
		    answer[1] != 'E' || answer[2] != 'S' ||
		    answer[3] != 'A' )
		{                      /* if response unsuccessful */
		printf("Error - VESA BIOS Extensions Not Available: code=%04X<%c%c%c%c>\n",
			outr.x.ax, answer[0],answer[1],answer[2],answer[3]);
#ifdef DOS16RM
		for (u=0; u < 257; u++)
		    answer[u] = save[u];
#endif
#ifdef _INTELC32_
		_dos_setvect(0x10, prev_int10);/* Restore previous handler */
		_dos_freemem((unsigned) answer);/* Free DOS buffer */
#endif
		exit(1);
		}
#ifdef _INTELC32_
	    mp = (char *)(*(unsigned short *)(answer + 14) +
			 (((long)*(unsigned short *)(answer + 16)) << 4));
	    for (i = 0; i < 64 && mp[i] != 0xFFFF; i++) /* for all modes available */
		vesamodes[i] = mp[i];           /* copy to local mode list */
#else
#ifdef DOS386
	    fp = MK_FP(_x386_zero_base_selector,
		*(unsigned short *)(answer+14)  /* get mode tbl. pointer   */
		+ (*(unsigned short *)(answer+16) << 4));
	    for (u=i=0; i < 64 && (tmp_word=fp[u]|(fp[u+1]<<8)) != 0xFFFF; i++, u += 2)/* for all modes available */
		vesamodes[i] = tmp_word;        /* copy to local mode list */
#else
#ifdef DOS16RM
	    tl = *(unsigned short *)(answer + 14) +
		  (((long)*(unsigned short *)(answer + 16)) << 4);
	    mp = (unsigned short *)D16SegAbsolute(tl, 0);
	    for (i = 0; i < 64 && mp[i] != 0xFFFF; i++) /* for all modes available */
		vesamodes[i] = mp[i];           /* copy to local mode list */
	    D16SegCancel((void *)mp);
#else
	    mp = (unsigned short *)MK_FP(answer[16]+(answer[17]<<8),
		       answer[14]+(answer[15]<<8)); /* get mode tbl. pointer */
	    for (i = 0; i < 64 && mp[i] != 0xFFFF; i++)   /* for all modes available */
		vesamodes[i] = mp[i];           /* copy to local mode list */
#endif
#endif
#endif
	    maxvesamode = i;                    /* note end of list */
	    bpp = (PaletteOption == HICOLOR)? 16: (PaletteOption == FULLCOLOR)? 32: 8;

	    svga_width = 0;
	    svga_height = 0;
	    vesamode = 0;
	    for (i = 0; i < maxvesamode; i++) {
		inr.x.ax = 0x4F01;      /* VESA BIOS fetch attributes call */
		inr.x.bx = inr.x.cx = vesamodes[i]; /* get attrs for mode */
#ifdef DOS386
		inr.x.di = real_buf[0];         /* deposit results here */
		segread(&segs);                 /* get our DS, etc. */
		segs.es = real_buf[1];          /* get segment of answer */
		int86x_real(0x10, &inr, &outr, &segs);
		for(u=0; u<sizeof(answer); u++)
		    answer[u] = real_ptr[u];
#else
#ifdef GCCDOS
		inr.x.di = di_val;              /* deposit results here */
		int86x(0x10, &inr, &outr, &segs);/* BIOS fetch attrib call */
#else
#ifdef DOS16RM
		segread(&segs);                 /* get our DS, etc. */
		inr.x.di = 0x580;               /* deposit results here */
		segs.es = 0;
		int86x(0x10, &inr, &outr, &segs);/* BIOS fetch attrib call */
#else
		inr.x.di = FP_OFF(answer);      /* deposit attribs here */
		segread(&segs);                 /* get our DS, etc. */
		segs.es = FP_SEG(answer);       /* get segment of answer */
		int86x(0x10, &inr, &outr, &segs);/* BIOS fetch attrib call */
#endif
#endif
#endif
		if ((answer[0] & 0x11) != 0x11)
		    continue;
		if ((answer[0] & 0x02) == 0) {
		    for (u=0; u < sizeof(modelist)/sizeof(modelist[0]); u++)
			if (inr.x.cx == modelist[u].modenum) {
			    *(unsigned short *)(answer+18) = modelist[u].width;
			    *(unsigned short *)(answer+20) = modelist[u].height;
			    answer[25] = modelist[u].bpp;
			    answer[36] = 0;
			    if (answer[25] == 16) {
				answer[31] = 5;
				answer[32] = modelist[u].gsize + 5;
				answer[33] = modelist[u].gsize;
				answer[34] = 5;
				answer[35] = 5;
				}
			    else if (answer[25] == 24 || answer[25] == 32) {
				answer[31] = modelist[u].gsize;
				answer[32] = modelist[u].gsize * 2;
				answer[33] = modelist[u].gsize;
				answer[34] = modelist[u].gsize;
				answer[35] = modelist[u].gsize;
				}
			    answer[0] |= 0x02;
			    break;
			    }
		    }
		else if (bpp == 16 && answer[31] == 0 && answer[33] == 0 &&
			answer[35] == 0) {
		    answer[31] = 5;     /* VESA 1.1 Programming guideline */
		    answer[32] = 10;    /* default high color is 5-5-5 */
		    answer[33] = 5;
		    answer[34] = 5;
		    answer[35] = 5;
		    }

		if ((answer[0] & 0x02) && (answer[25] == bpp ||
			(bpp == 32 && answer[25] == 24))) {
			if ((svga_height < screen_height || svga_width < screen_width) &&
			    *(unsigned short *)(answer+18) >= svga_width &&
			    *(unsigned short *)(answer+20) >= svga_height)
			      vesamode = vesamodes[i];
		    else if (svga_height >= *(unsigned short *)(answer+18) &&
			    svga_width >= *(unsigned short *)(answer+20) &&
			    *(unsigned short *)(answer+18) >= screen_width &&
			    *(unsigned short *)(answer+20) >= screen_height)
			      vesamode = vesamodes[i];

			if (vesamode == vesamodes[i]) {
			if ((answer[2] & 0x05) == 0x05)
			    map_code = 0x0000;          /* Select A page */
			else if ((answer[3] & 0x05) == 0x05)
			    map_code = 0x0001;          /* Select B page */
			bpp = answer[25];               /* Handle 24 or 32 bpp */
				svga_width = *(unsigned short *)(answer+18);
				svga_height = *(unsigned short *)(answer+20);
				r_shift = answer[32] + answer[31] - 8;
				g_shift = answer[34] + answer[33] - 8;
				b_shift = answer[36] + answer[35] - 8;
				r_mask = 0xFF - bitmask[8 - answer[31]];
				g_mask = 0xFF - bitmask[8 - answer[33]];
				b_mask = 0xFF - bitmask[8 - answer[35]];
				b_shift = - b_shift;
				if (bpp > 8 && (b_shift < 0 || r_shift < 0 || g_shift < 0)) {
					printf("Adapter not supported currently in high color modes!\n");
					printf("Shifts required are RED:%d, GREEN:%d, BLUE:%d\n",
						r_shift, g_shift, b_shift);
					printf("Bit masks: RED:%d, GREEN:%d, BLUE:%d\n",
						r_mask, g_mask, b_mask);
					printf("Field positions: RED:%d, GREEN:%d, BLUE:%d\n",
						answer[36], answer[34], answer[32]);
					printf("Field widths: RED:%d, GREEN:%d, BLUE:%d\n",
						answer[35], answer[33], answer[31]);
					exit(1);
					}
				}
			}
		}

	    if (vesamode == 0)
		goto no_valid_mode;

	    inr.x.ax = 0x4F01;  /* VESA BIOS fetch attributes call */
	    inr.x.bx = inr.x.cx = vesamode;     /* get attrs for mode */
#ifdef GCCDOS
	    inr.x.di = di_val;                  /* deposit results here */
	    int86x(0x10, &inr, &outr, &segs);
#else
#ifdef DOS386
	    inr.x.di = real_buf[0];             /* deposit results here */
	    segread(&segs);                     /* get our DS, etc. */
	    segs.es = real_buf[1];              /* get segment of answer */
	    int86x_real(0x10, &inr, &outr, &segs);
	    for(u=0; u<sizeof(answer); u++)
		answer[u] = real_ptr[u];
#else
#ifdef DOS16RM
	    segread(&segs);                     /* get our DS, etc. */
	    inr.x.di = 0x580;                   /* deposit results here */
	    segs.es = 0;
	    int86x(0x10, &inr, &outr, &segs);
#else
	    inr.x.di = FP_OFF(answer);  /* deposit attribs here */
	    segread(&segs);                     /* get our DS, etc. */
	    segs.es = FP_SEG(answer);   /* get segment of answer */
	    int86x(0x10, &inr, &outr, &segs);/* BIOS fetch attrib call */
#endif
#endif
#endif
	    if (outr.x.ax != 0x004F)
		goto no_valid_mode;
	    svga_yincr = *(unsigned short *)(answer+16);
	    granule = *(unsigned short *)(answer + 4); /* "granule" size */
	    if (granule < 1)
		granule = 1;
	    gran = 1024L * granule;
	    inr.x.ax = 0x4F02;  /* VESA BIOS initialize video mode call */

	    if (svga_yincr == svga_width && PaletteOption == HICOLOR)
		if (svga_width == 640 && svga_height == 200) {
		    svga_width = 320;
		    svga_yincr = 1024;
		}
		else {
		    svga_yincr = svga_width * 2;
		}
	    break;

#if !defined(DOS16RM) && !defined(DOS386) && !defined(_INTELC32_)
#if !defined(__386__) && !defined(GCCDOS)

	case TIGA_16:
	case TIGA_24:
	case TRUE_640:
	case TRUE_800:
	case TRUE_1024:
	    if ((int)(DisplayFormat - '0') == whichvga) /* not autodetected */
		if (tiga_set(CD_OPEN) < 0) /* Attempt to open the TIGA CD */
		    {
		    printf("Error - TIGA CD Not Installed.");
		    goto TIGA_error;
		    }
	    if (!set_videomode(TIGA, INIT_GLOBALS | CLR_SCREEN))
		{
		printf("Error #1 Initializing TIGA.");
		goto TIGA_error;
		}
	    if (install_primitives() < 0) /* load if not already loaded */
		{
		if (!set_videomode(TIGA, INIT)) /*reinit if due to full heap*/
		    {
		    printf("Error #2 Initializing TIGA.");
		    goto TIGA_error;
		    }
		else if (install_primitives() < 0)
		    {
		    printf("Error Installing TIGA Primitives.");
TIGA_error:
#ifdef DOS16RM
		    for (u=0; u < 257; u++)
			answer[u] = save[u];
#endif
#ifdef _INTELC32_
		    _dos_setvect(0x10, prev_int10);/* Restore previous handler */
		    _dos_freemem((unsigned) answer);/* Free DOS buffer */
#endif
		    exit(1);
		    }
		switch (whichvga)
		    {
		    case TIGA_16:       /* initialize demo globals */
			 get_config(&config);
			 if ((config.mode.disp_vres == 640) && (config.mode.disp_psize == 16))
			 {
			    Color_Bits = 5;
			    svga_width = 640;
			    svga_height = 480;
			    svga_yincr = 640;
			    set_config(1,0);        /* 16 bit (was 2,0 ???? - AAC) */
			    break;
			 }
			 else whichvga = TIGA_24;
		    case TIGA_24:
			get_config(&config);
			if ((config.mode.disp_vres == 480) && (config.mode.disp_hres == 512))
			{
			    svga_width = 512;   /* NTSC standard television resolution */
			    svga_height = 480;
			    svga_yincr = 512;
			    set_config(4,0);       /* 24 bit */
			    break;
			}
			else whichvga = TRUE_640;
		    case TRUE_640:
			get_config(&config);
			if ((config.mode.disp_hres == 640) && (config.mode.disp_psize == 32))
			{
			    svga_width = 640;
			    svga_height = 480;
			    svga_yincr = 640;
			    set_config(0,0);       /* 24 bit */
			    break;
			}
			else whichvga = TRUE_800;
		   case TRUE_800:
			get_config(&config);
			if ((config.mode.disp_vres == 600) && (config.mode.disp_hres == 800))
			{
			    svga_width = 800;
			    svga_height = 600;
			    svga_yincr = 800;
			    set_config(2,0);       /* 24 bit */
			    break;
			}
			else whichvga = TRUE_1024;
		   case TRUE_1024:
			svga_width = 1024;
			svga_height = 768;
			svga_yincr = 1024;
			set_config(4,0);
		   }
		}
#endif
#endif
	    break;

	default:                /* BASIC_VGA */
no_valid_mode:
	    inr.x.ax = 0x0013;  /* setup to VGA 320x200x256 (mode 13H) */
	    svga_width = 320;   /* allow scaling to run at 320x200 */
	    svga_height = 200;
	    svga_yincr = 320;
	    break;
	}
#ifdef DEBUG_VIDEO
    if (whichvga >= TRUECOLOR)  /* if True-Color then it has no palette */
		goto exit_point;

    lt = l = time(&l);
    printf("Display detected: (%c) %s Adapter", whichvga + '0', vga_names[whichvga]);
    if (whichvga < TRUECOLOR)   /*  if not a True-Color graphics card */
	printf(", with %s 512K RAM", vga_512K ? ">=" : "<");
    printf("\n   ax=%04X, bx=%04X, cx=%04X\n", inr.x.ax, inr.x.bx, inr.x.cx);
    printf("   %dx%d, yincr=%d\n", svga_width, svga_height, svga_yincr);
    printf("   shifts(R,G,B)=%d,%d,%d\n", r_shift, g_shift, -b_shift);
    printf("   masks(R,G,B) =%d,%d,%d\n", r_mask, g_mask, b_mask);
    while (time(&l) < lt + 5)   /* display detected VGA type for 5 secs */
	;
#endif

#ifdef DOS386
    int86_real(0x10, &inr, &outr); /* do the BIOS video mode sel. call */
#else
    int86(0x10, &inr, &outr);   /* do the BIOS video mode sel. call */
#endif

    if (whichvga == MODE13x)    /* Tweak VGA registers to get higher res! */
	{
	outpw(SEQUENCER, 0x0604);   /* disable chain 4 */
	outpw(SEQUENCER, 0x0f02);   /* allow writes to all planes */
#ifdef DOS386
	fp = MK_FP(_x386_zero_base_selector, 0xA0000);
#else
#ifdef DOS16RM
	fp = display_base;
#else
	fp = MK_FP(0xA000, 0);
#endif
#endif
	for (u = 0; u < 43200; u++) /* clear the whole screen */
	    {
	    fp = (unsigned char *)MK_FP(0xA000, u);
	    *fp = 0;                /* set all bytes to 0 */
	    }
	outpw(SEQUENCER, 0x0100);   /* synchronous reset */
	outp(MISCOUT, 0xE7);        /* use 28 mhz dot clock */
	outpw(SEQUENCER, 0x0300);   /* restart sequencer */
	outp(CRTC, 0x11);           /* ctrl register 11, please */
	outp(CRTC+1, inp(CRTC+1) & 0x7f); /* write-prot cr0-7 */

	for (i = 0; i < 17; i++)    /* write CRTC register array */
	    outpw(CRTC, vptbl[i]);
	}

    if (PaletteOption == HICOLOR && whichvga == TSENG4) {
	if ((int)inr.h.al == 0x78)      /* if it was mode 78 */
	   inr.x.bx = 0x2F;             /* make it 2F... */
	else
	   inr.x.bx = (unsigned int)inr.h.al;
	inr.x.ax = 0x10F0;
	int86(0x10, &inr, &inr); /* Go from 256 color to 32K color mode */
	r_mask = g_mask = b_mask = 0xF8;
	r_shift = 7;
	g_shift = 2;
	b_shift = 3;
	svga_yincr <<= 1;
	}

    if (whichvga != ATIXL && PaletteOption != HICOLOR && PaletteOption != FULLCOLOR)
	palette_init();  /* if we get here it has a normal 256 color palette DAC */

    if (whichvga == CHIPSTECH) {        /* (not sure why this is necessary) */
	outpw(0x46E8, 0x001E);          /* put chip in setup mode */
	outpw(0x103, 0x0080);           /* enable extended registers */
	outpw(0x46E8, 0x000E);          /* take chip out of setup mode */
	outp(0x3D6, 0x10);
	}

exit_point:
    if (whichvga == VESA && vesamode == 0x13) {
	svga_height = 200;
	svga_width = 320;
/*        svga_yincr = 320; */
	}
    if (screen_height <= svga_height)
	y_off = (svga_height - screen_height)/2;
    else
    Height_Adjust = (DBL)screen_height / (DBL)svga_height;
    if (screen_width <= svga_width)
	x_off = (svga_width - screen_width)/2;
    else
    Width_Adjust = (DBL)screen_width / (DBL)svga_width;

    box(0, 0, width-1, height-1);

#if defined(DOS16RM)
    for (u=0; u < 257; u++)
	answer[u] = save[u];
#endif
#ifdef _INTELC32_
    _dos_setvect(0x10, prev_int10);/* Restore previous handler */
    _dos_freemem((unsigned) answer);/* Free DOS buffer */
#endif
    return;
}


int AutodetectVGA()           /* Autodetect (S)VGA Adapter Type */
    {
    unsigned char _far *biosptr;
    unsigned char tmp_byte;
    unsigned int tmp_word;
    unsigned int retval;
    union REGS inr, outr;
    struct SREGS segs;

#if !defined(GCCDOS) && !defined(__386__) && !defined(_INTELC32_)
#if !defined(__ZTC__)
    if (tiga_set(CD_OPEN) >= 0) /* Try initializing HGWS/TIGA Adapter */
	{
	get_config(&config);
	vga_512K = TRUE;        /* All True-Color Adapters are > 512K! */
	if ((config.mode.disp_vres == 640) && (config.mode.disp_psize == 16) == TRUE)
	    return(TIGA_16);
	if ((config.mode.disp_vres == 480) && (config.mode.disp_hres == 512) == TRUE)
	    return(TIGA_24);
	if ((config.mode.disp_hres == 640) && (config.mode.disp_psize == 32) == TRUE)
	    return(TRUE_640);
	if ((config.mode.disp_vres == 600) && (config.mode.disp_hres == 800) == TRUE)
	    return(TRUE_800);
	return(TRUE_1024);
	}
#endif
#endif

    inr.x.ax = 0x4F00;                  /* Test for VESA Adapter */
#ifdef GCCDOS
    inr.x.di = di_val;                  /* deposit results here */
    int86x(0x10, &inr, &outr, &segs);
#else
#ifdef DOS386
    inr.x.di = real_buf[0];             /* deposit results here */
    segread(&segs);                     /* get our DS, etc. */
    segs.es = real_buf[1];              /* get segment of answer */
    int86x_real(0x10, &inr, &outr, &segs);
    for(tmp_word=0; tmp_word<sizeof(answer); tmp_word++)
	answer[tmp_word] = real_ptr[tmp_word];
#else
#ifdef DOS16RM
    inr.x.di = 0x580;                   /* deposit results here */
    segs.es = 0;
    int86x(0x10, &inr, &outr, &segs);
#else
    segread(&segs);                     /* get our DS, etc. */
    inr.x.di = FP_OFF(answer);      /* deposit results here */
    segread(&segs);                 /* get our DS, etc. */
    segs.es = FP_SEG(answer);       /* get segment of answer */
    int86x(0x10, &inr, &outr, &segs); /* BIOS adapter identify call */
#endif
#endif
#endif

    if (outr.x.ax == 0x004F && answer[0] == 'V' && answer[1] == 'E' &&
	    answer[2] == 'S' && answer[3] == 'A') {
	vga_512K = TRUE;            /* assume all VESA's have >= 512K */
	return (VESA);
	}

#ifdef DOS386
    biosptr = MK_FP(_x386_zero_base_selector, 0x000C0040);
#else
#ifdef DOS16RM
    biosptr = (unsigned char *)D16SegAbsolute(0xC0040, 16);
#else
    biosptr = (unsigned char *)MK_FP(0xC000, 0x0040); /* Test for ATI Wonder */
#endif
#endif

    if (*biosptr == '3' && *(biosptr + 1) == '1')
	{
	_disable();                     /* Disable system interrupts */
	outp(0x1CE, 0xBB);
	if (inp(0x1CD) & 0x20)
	    vga_512K = TRUE;
	_enable();                      /* Re-enable system interrupts */
#ifdef DOS16RM
	D16SegCancel((void *)biosptr);
#endif
	return (ATIVGA);
	}
    inr.x.ax = 0x7000;                  /* Test for Everex &| Trident */
    inr.x.bx = 0;

    CLD;

    int86(0x10, &inr, &outr);
    if (outr.h.al == 0x70)
	{
	if (outr.h.ch & 0xC0)
	    vga_512K = TRUE;
	outr.x.dx &= 0xFFF0;
	if (outr.x.dx == 0x6780)
	    {
	    printf("\nT6780\n");
	    return (TRIDENT);
	    }
	if (outr.x.dx == 0x2360)
	    {
	    printf("\nT2360\n");
	    return (TRIDENT);
	    }
	if (outr.x.dx == 0x6730)        /* EVGA? (No BIOS Page Fn.) */
	    {
	    printf("\nE6730\n");
	    return (EVEREX);
	    }
	printf("\nE0000\n");
	return (EVEREX);        /* Newer board with fetchable bankswitch */
	}
    outp(0x3C4, 0x0B);                  /* Test for Trident */
    tmp_byte = (unsigned char) inp(0x3C5);
    if ((tmp_byte > 1) && (tmp_byte < 0x10))
	{
	vga_512K = TRUE;
	printf("\nT0000\n");
	return (TRIDENT);
	}
    if (cirrus())                       /* Test Video7 Vega VGA (Cirrus) */
	return (CIRRUS);
    inr.x.ax = 0x6F00;                  /* Test for Video7 SVGA */
    inr.x.bx = 0;                       /* note - Vega VGA (Cirrus) will */

    CLD;

    int86(0x10, &inr, &outr);           /* pass this test - test Cirrus 1st */
    if (outr.h.bh == 'V' && outr.h.bl == '7')
	{
	inr.x.ax = 0x6F07;

	CLD;

	int86(0x10, &inr, &outr);
	if ((outr.h.ah & 0x7F) > 1)
	    vga_512K = TRUE;
	return (VIDEO7);
	}
    outp(0x3CE, 9);                     /* Test for Paradise */
    if (!inp(0x3CF))
	{
	outpw(0x3CE, 0x050F);           /* Turn off write protect on regs */
	if (chkbank(0,1))               /* if bank 0 and 1 same not para. */
	    {                           /* FALSE == banks same... (C) */
	    if (chkbank(0, 64))         /* if bank 0 and 64 same only 256K */
		vga_512K = TRUE;

	    retval = PARADISE;
#ifdef DOS16RM
	    biosptr = (unsigned char *)D16SegAbsolute(0xC0039, 16);
	    if ((*biosptr == '1') && (*biosptr+1 == '6')) /* p/n 003116 */
		retval = VDC600;        /* a real Paradise is p/n 003145 */
	    D16SegCancel((void *)biosptr);
#else
	    biosptr = (unsigned char *)MK_FP(0xC000, 0x0039);/* Test for AT&T VDC600 */
	    if ((*biosptr == '1') && (*biosptr+1 == '6')) /* p/n 003116 */
		retval = VDC600;        /* a real Paradise is p/n 003145 */
#endif
	    return (retval);
	    }
	}
    inr.x.ax = 0x5F00;                  /* Test for Chips & Tech */
    inr.x.bx = 0;

    CLD;

    int86(0x10, &inr, &outr);
    if (outr.h.al == 0x5F)
	{
	if (outr.h.bh >= 1)
	    vga_512K = TRUE;
	return (CHIPSTECH);
	}
    outp(0x3D4, 0x33);                  /* Test for Tseng 4000 or 3000 Chip */
    tmp_word = (unsigned int) inp(0x3D5) << 8;
    outpw(0x3D4, 0x0A33);
    outp(0x3D4, 0x33);
    retval = BASIC_VGA;
    if ((inp(0x3D5) & 0x0F) == 0x0A)
	{
	outpw(0x3D4, 0x0533);
	outp(0x3D4, 0x33);
	if ((inp(0x3D5) & 0x0F) == 0x05)
	    {
	    retval = TSENG4;
	    outpw(0x3D4, tmp_word | 0x33);
	    outp(0x3D4, 0x37);
	    if ((inp(0x3D5) & 0x0A) == 0x0A)
		vga_512K = TRUE;
	    outp(0x3BF, 0x03);          /* Enable access to extended regs */
	    outp(0x3D8, 0xA0);
	    outp(0x3D8, 0x29);          /* Enable mapping register access */
	    outp(0x3D8, 0xA0);
	    }
	}
    tmp_byte = (unsigned char) inp(0x3CD);      /* save bank switch reg */
    outp(0x3CD, 0xAA);                          /* test register w/ 0xAA */
    if (inp(0x3CD) == 0xAA)
	{
	outp(0x3CD, 0x55);                      /* test register w/ 0x55 */
	if (inp(0x3CD) == 0x55)
	    {
	    outp(0x3CD, tmp_byte);              /* restore bank switch reg */
	    if (retval != TSENG4)             /* yep, it's a Tseng... */
		retval = TSENG3;
	    vga_512K = TRUE;
	    return (retval);
	    }
	}
    outpw(0x3CE, 0x200F);               /* Test for Ahead A or B chipsets */
    tmp_byte = (unsigned char) inp(0x3CF);
    if (tmp_byte == 0x21)
	{
	vga_512K = TRUE;                /* Assume all Ahead's have 512K... */
	return (AHEADB);
	}
    if (tmp_byte == 0x20)
	{
	vga_512K = TRUE;
	return (AHEADA);
	}
    if ((inp(0x3DE) & 0xE0) == 0x60)    /* Test for Oak Tech OTI-067 */
	{
	outp(0x3DE, 0x0D);
	if (inp(0x3DF) & 0x80)
	    vga_512K = TRUE;
	return(OAKTECH);
	}
    return (BASIC_VGA);                 /* Return 1 if Unknown/BASIC_VGA */
    }

int cirrus()                    /* Test for presence of Cirrus VGA Chip */
    {
    unsigned char tmp_byte;
    unsigned int crc_word, tmp_word;
    int retcode = FALSE;

    outp(0x3D4, 0x0C);  /* assume 3Dx addressing, scrn A start addr hi */
    crc_word = (unsigned int) inp(0x3D5) << 8;  /* save the crc */
    outp(0x3D5, 0);                             /* clear the crc */
    outp(0x3D4, 0x1F);                          /* Eagle ID register */
    tmp_byte = (unsigned char) inp(0x3D5);      /* nybble swap "register" */
    tmp_word = (((tmp_byte & 0x0F) << 4) | ((tmp_byte & 0xf0) >> 4)) << 8;
    outpw(0x3C4, tmp_word | 0x06);              /* disable extensions */
    if (!inp(0x3C5))
	{
	tmp_word = (unsigned int) tmp_byte << 8;
	outpw(0x3C4, tmp_word | 0x06);          /* re-enable extensions */
	if (inp(0x3C5) == 1)
	    retcode = TRUE;
	}
    outpw(0x3D5, crc_word | 0x0c);              /* restore the crc */
    return (retcode);
    }

int chkbank(bank0, bank1)       /* Paradise SVGA specific stuff */
   unsigned int bank0, bank1;   /* returns TRUE if banks are different RAM */
   {
   static unsigned int value = 0x1234;
   unsigned int _far *fp;
   unsigned int temp;
   unsigned int oldval0, oldval1;

#ifdef DOS386                           /* point out into display RAM */
   fp = MK_FP(_x386_zero_base_selector, 0xB8000);
#else
   fp = (unsigned int *)MK_FP(0xB800, 0); /* point out into display RAM */
#endif

   outp(0x3CE, 9);
   outp(0x3CF, bank0);  /* save prior video data and write test values */
   oldval0 = *fp;
   *fp ^= value;
   if (*fp != (oldval0 ^ value)) {
      *fp = oldval0;
      return FALSE;             /* No RAM there at all -- can't be 512K */
      }

   outp(0x3CE, 9);
   outp(0x3CF, bank1);  /* save prior video data and write test values */
   oldval1 = *fp;
   *fp ^= value;
   if (*fp != (oldval1 ^ value)) {
      *fp = oldval1;
      outp(0x3CE, 9);
      outp(0x3CF, bank0);
      *fp = oldval0;
      return FALSE;             /* No RAM there at all -- can't be 512K */
      }
   if (*fp != oldval0) {
      *fp = oldval1;
      outp(0x3CE, 9);
      outp(0x3CF, bank0);
      *fp = oldval0;
      return TRUE;              /* pages cannot be the same RAM */
      }

   outp(0x3CE, 9);
   outp(0x3CF, bank0);
   temp = *fp;
   outp(0x3CE, 9);
   outp(0x3CF, bank1);
   *fp = oldval1;
   outp(0x3CE, 9);
   outp(0x3CF, bank0);
   *fp = oldval0;

   if (temp == oldval0)
      return FALSE;             /* pages are the same RAM */
   else
      return TRUE;              /* independent values, so not same RAM */
   }

/* BP ? */   
void atiplot(x, y, r, g, b)     /* ATI VGA Wonder XL 32K color display plot */
int x, y, r, g, b;
{
   char plane=0;
   unsigned offset;
   unsigned long address;
   unsigned short _far *fp;

   r/=8;
   g/=8;
   b/=8;
   address = y * 1280L + x * 2L;
   offset = (unsigned)(address % 65536L);
   plane = (char)(address / 65536L);
   _disable();
   atibank(plane);

#ifdef DOS386
   fp = MK_FP(_x386_zero_base_selector, 0xA0000 + offset);
#else
#ifdef DOS16RM
   fp = (unsigned short *)(display_base + offset);
#else
   fp = (unsigned short *)MK_FP(0xA000, offset);
#endif
#endif

   *fp = (unsigned short)(r * 1024 + g * 32 + b);
   _enable();
   return;
}

void atibank(b)         /* ATI VGA Wonder XL bank switch routine */
int b;
{
   unsigned tmp_word;
   outp(0x1CE, 0xB2);
   tmp_word = (unsigned int)((b << 1) | (inp(0x1CF) & 0xE1));
   outpw(0x1CE, (tmp_word << 8) | 0x00B2);
   return;
}


void palette_init()             /* Fill VGA 256 color palette with colors! */
    {
    register unsigned m;
    unsigned r, g, b;
    register DBL hue, sat, val;

    if (PaletteOption == GREY)  /* B/W Video Mod */
    {
	for (m = 1; m < 64; m++)     /* for the 1st 64 colors... */
	    set_palette_register (m, m, m, m); /* set m to rgb value */
	for (m = 64; m < 256; m++)     /* for the remaining, at full value */
	    set_palette_register (m, 63, 63, 63);
	return;
    }
    if (PaletteOption == P_332) /* 332 Video Mod */
    {
		for (r=0;r<8;r++)
			for (g=0;g<8;g++)
				for (b=0;b<4;b++) {
					m = (r * 32 + g * 4 + b);
					set_palette_register(m, r * 9, g * 9, b * 21);
				}
		return;
	}
    set_palette_register(0, 0, 0, 0);   /* make palette register 0 black */

    set_palette_register(64, 63, 63, 63); /* make palette register 64 white */

    set_palette_register(128, 31, 31, 31); /* make register 128 dark grey */

    set_palette_register(192, 48, 48, 48); /* make register 192 lite grey */

    for (m = 1; m < 64; m++)     /* for the 1st 64 colors... */
	{
	sat = 0.5;      /* start with the saturation and intensity low */
	val = 0.5;
	hue = 360.0 * ((DBL)(m)) / 64.0;   /* normalize to 360 */
	hsv_to_rgb (hue, sat, val, &r, &g, &b);
	set_palette_register (m, r, g, b); /* set m to rgb value */

	sat = 1.0;      /* high saturation and half intensity (shades) */
	val = 0.50;
	hue = 360.0 * ((DBL)(m)) / 64.0;   /* normalize to 360 */
	hsv_to_rgb (hue, sat, val, &r, &g, &b);
	set_palette_register (m + 64, r, g, b);  /* set m + 64 */

	sat = 0.5;      /* half saturation and high intensity (pastels) */
	val = 1.0;

	hue = 360.0 * ((DBL)(m)) / 64.0;   /* normalize to 360 */
	hsv_to_rgb (hue, sat, val, &r, &g, &b);
	set_palette_register (m + 128, r, g, b); /* set m + 128 */

	sat = 1.0;            /* normal full HSV set at full intensity */
	val = 1.0;
   
	hue = 360.0 * ((DBL)(m)) / 64.0;   /* normalize to 360 */
	hsv_to_rgb (hue, sat, val, &r, &g, &b);
	set_palette_register (m + 192, r, g, b); /* set m + 192 */
	}
    return;
    }


void display_finished ()
    {
    if (Options & PROMPTEXIT)
	{
	printf ("\007\007");    /* long beep */
	while(!kbhit())         /* wait for key hit */
	   ;
	if (!getch())           /* get another if ext. scancode */
	   getch();
	}
    }


void display_close()   /* setup to Text 80x25 (mode 3) */
    {
    union REGS regs;
#ifdef __386__
    struct SREGS segs;

    regs.x.ax = 0x0003;
    int86(0x10, &regs, &regs);
    regs.x.ax = 0x4900;
    segs.es = vesabuffer;
    segs.ds = 0;
    int86x(0x21, &regs, &regs, &segs);
#else
#if defined(_INTELC32_) || defined(GCCDOS) || defined(DOS16RM) || defined(DOS386)
    regs.x.ax = 0x0003;
    int86(0x10, &regs, &regs);
#else
    if (whichvga < TRUECOLOR) {
	regs.x.ax = 0x0003;
	int86(0x10, &regs, &regs);
	}
    else
	set_videomode(PREVIOUS, INIT);
#endif
#endif

#ifdef DOS16RM
    D16SegCancel(display_base);
#endif
    return;
    }


  /* plot a single RGB pixel */
#ifdef ANSIFUNC
void display_plot (                     /* plot a single RGB pixel */
   int x, int y,
   unsigned char Red,
   unsigned char Green,
   unsigned char Blue)
#else
void display_plot (x, y, Red, Green, Blue)   /* plot a single RGB pixel */
   int x, y;
   unsigned Red, Green, Blue;
#endif
   {
   union REGS inr;
   register unsigned char color, svga_page;
   unsigned char _far *fp;
   unsigned int svga_word;
   unsigned long svga_loc;
   DBL h, s, v, fx, fy;
#ifdef DITHERED
   int i, r, g, b, re, ge, be, ri, gi, bi;
#endif

   if (x == 0)                  /* first pixel on this line? */
	{
#ifdef DITHERED
	for(i=0;i<screen_width+2;i++)
	    {
	    r_err[(y+1)&1][i+x_off] = 0;
	    g_err[(y+1)&1][i+x_off] = 0;
	    b_err[(y+1)&1][i+x_off] = 0;
	    }
#endif          
	lastx = -1;             /* reset cache, make sure we do the 1st one */
	lasty = lastline;       /* set last line do to prior line */
	}

    y += y_off;
    x += x_off;
   
   /* Scaling factors precomputed per RHA in Height_Adjust or Width_Adjust */
	
   if (screen_height > svga_height)     /* auto-scale Y */
	{
	fy = (DBL)y / Height_Adjust;
	y = (int)fy;            /* scale y to svga_height */
	if (y <= lasty)         /* discard if repeated line */
	    return;
	lastline = y;           /* save current working line */
	}

   if (screen_width > svga_width)               /* auto-scale X */
	{
	fx = (DBL)x / Width_Adjust;
	x = (int)fx;            /* scale x to svga_width */
	if (x <= lastx)         /* discard if repeated pixel */
	    return;
	lastx = x;              /* save most recent pixel done */
	}

   if (whichvga == ATIXL)
	{
	atiplot(x, y, Red, Green, Blue);
	return;
	}
	
#if !defined(_INTELC32_) && !defined(__386__) && !defined(GCCDOS)
#if !defined(__ZTC__)
   if (whichvga == TIGA_16)
	{
	set_bcolor(0);
	set_fcolor(((unsigned)Red << 10) + (Green << 5) + Blue);
	draw_point(x, y);
	return;
	}

   if (whichvga == TIGA_24 || whichvga == TRUE_640 || whichvga == TRUE_640 || whichvga == TRUE_1024)
	{
	hpt[0] = Green;
	hpt[1] = Red;
	hpt[2] = Blue;
	host2gsp(hptr, ((whichvga == TIGA_24) ? 0x4000L : 0x8000L) * y + ((long) x << 5), 3, 0);
	return;
	}
#endif
#endif

   if (PaletteOption == FULLCOLOR)
	{
	svga_loc = ((unsigned long)svga_yincr) * y + x * bpp/8;
	svga_page = (unsigned char)(svga_loc/gran);
	svga_loc %= gran;
	if (svga_page != cur_page)
	    {
	    cur_page = svga_page;
	    if (whichvga != VESA)
		outp(0x3CD, (unsigned char)svga_page);
	    else
		{
		inr.x.bx = map_code;            /* map code = 0 or 1 */
		inr.x.ax = 0x4F05;
		inr.x.dx = (unsigned int)svga_page;
#ifdef DOS386
		int86_real(0x10, &inr, &inr);
#else
		int86(0x10, &inr, &inr);
#endif
		}
	    }

#ifdef DOS386
	fp = MK_FP(_x386_zero_base_selector, 0xA0000 + (unsigned int)svga_loc);
#else
#ifdef DOS16RM
	fp = display_base + (unsigned int)svga_loc;
#else
	fp = (unsigned char *)MK_FP(0xA000, (unsigned int)svga_loc);
#endif
#endif
	*fp++ = Blue;
	*fp++ = Green;
	*fp++ = Red;
	return;
	}
   if (PaletteOption == HICOLOR)
	{
	svga_loc = ((unsigned long)svga_yincr) * y + (x << 1);
	svga_page = (unsigned char)(svga_loc/gran);
	svga_loc %= gran;
	if (svga_page != cur_page)
	    {
	    cur_page = svga_page;
	    if (whichvga != VESA)
		outp(0x3CD, (unsigned char)svga_page);
	    else
		{
		inr.x.bx = map_code;            /* map code = 0 or 1 */
		inr.x.ax = 0x4F05;
		inr.x.dx = (unsigned int)svga_page;
#ifdef DOS386
		int86_real(0x10, &inr, &inr);
#else
		int86(0x10, &inr, &inr);
#endif
		}
	    }

#ifdef DOS386
	fp = MK_FP(_x386_zero_base_selector, 0xA0000 + (unsigned int)svga_loc);
#else
#ifdef DOS16RM
	fp = display_base + (unsigned int)svga_loc;
#else
	fp = (unsigned char *)MK_FP(0xA000, (unsigned int)svga_loc);
#endif
#endif

#ifdef DITHERED
	r = (int)Red + (int)r_err[y&1][x+1];
	g = (int)Green + (int)g_err[y&1][x+1];
	b = (int)Blue + (int)b_err[y&1][x+1];

	ri = (r+3) >> 3;
	gi = (g+3) >> 3;
	bi = (b+3) >> 3;

	if(ri > 31) ri = 31;
	if(gi > 31) gi = 31;
	if(bi > 31) bi = 31;
	if(ri < 0) ri = 0;
	if(gi < 0) gi = 0;
	if(bi < 0) bi = 0;

	re = r - ((255*ri)/31);
	ge = g - ((255*gi)/31);
	be = b - ((255*bi)/31);
	r_err[y&1][x+2] += (signed char)((7*re)/16);
	r_err[y&1][x] += (signed char)(re/16);
	r_err[(y+1)&1][x+1] += (signed char)((5*re)/16);
	r_err[(y+1)&1][x+2] += (signed char)((3*re)/16);
	g_err[y&1][x+2] += (signed char)((7*ge)/16);
	g_err[y&1][x] += (signed char)(ge/16);
	g_err[(y+1)&1][x+1] += (signed char)((5*ge)/16);
	g_err[(y+1)&1][x+2] += (signed char)((3*ge)/16);
	b_err[y&1][x+2] += (signed char)((7*be)/16);
	b_err[y&1][x] += (signed char)(be/16);
	b_err[(y+1)&1][x+1] += (signed char)((5*be)/16);
	b_err[(y+1)&1][x+2] += (signed char)((3*be)/16);

	*(unsigned short _far *)fp = (unsigned short)  /* stash 16-bit pixel */
		(((unsigned long)(ri << 10)) |
		((unsigned long)(gi << 5)) |
		((unsigned long)(bi)));
	return;
#else
	*(unsigned short _far *)fp = (unsigned short)  /* stash 16-bit pixel */
		(((unsigned long)(Red & r_mask) << r_shift) |
		((unsigned long)(Green & g_mask) << g_shift) |
		((unsigned long)(Blue & b_mask) >> b_shift));
	return;
#endif
	}

   if (PaletteOption == GREY)           /* RGB are already set the same, so */
      color = (unsigned char)(Green >> 2);  /* really, any color will do... */
   else if (PaletteOption == P_332) {
#ifdef DITHERED
      r = (int)Red + (int)r_err[y&1][x+1];
      g = (int)Green + (int)g_err[y&1][x+1];
      b = (int)Blue + (int)b_err[y&1][x+1];

      ri = (r+18)/36;
      gi = (g+18)/36;
      bi = (b+42)/84;

      if (ri > 7)
		  ri=7;
      if (gi > 7)
		  gi=7;
      if (bi > 3)
		  bi=3;
      if (ri < 0)
		  ri=0;
      if (gi < 0)
		  gi=0;
      if (bi < 0)
		  bi=0;
	
      color = (unsigned char)(((ri<<5) + (gi<<2) + bi));

      re = r - ((255 * ri) / 7);
      ge = g - ((255 * gi) / 7);
      be = b - ((255 * bi) / 3);

      r_err[y&1][x+2] += (signed char)((7*re)/16);
      r_err[y&1][x] += (signed char)(re/16);
      r_err[(y+1)&1][x+1] += (signed char)((5*re)/16);
      r_err[(y+1)&1][x+2] += (signed char)((3*re)/16);
      g_err[y&1][x+2] += (signed char)((7*ge)/16);
      g_err[y&1][x] += (signed char)(ge/16);
      g_err[(y+1)&1][x+1] += (signed char)((5*ge)/16);
      g_err[(y+1)&1][x+2] += (signed char)((3*ge)/16);
      b_err[y&1][x+2] += (signed char)((7*be)/16);
      b_err[y&1][x] += (signed char)(be/16);
      b_err[(y+1)&1][x+1] += (signed char)((5*be)/16);
      b_err[(y+1)&1][x+2] += (signed char)((3*be)/16);
#else
      color = ((Red & 0xE0) | ((Green & 0xE0) >> 3) | ((Blue & 0xC0) >> 6));
#endif
      }
   else  /* Translate RGB value to nearest of 256 palette Colors (by HSV) */
   {
      rgb_to_hsv((unsigned)Red,(unsigned)Green,(unsigned)Blue, &h, &s, &v);
      if (s < 0.20)   /* black or white if no saturation of color... */
      {
	 if (v < 0.25)
	    color = 0;        /* black */
	 else if (v > 0.8)
	    color = 64;       /* white */
	 else if (v > 0.5)
	    color = 192;      /* lite grey */
	 else
	    color = 128;      /* dark grey */
      }
      else
      {
	 color = (unsigned char) (64.0 * ((DBL)(h)) / 360.0);

	 if (!color)
	    color = 1;        /* avoid black, white or grey */
      
	 if (color > 63)
	    color = 63;       /* avoid same */

	 if (v > 0.50)
	    color |= 0x80;    /* colors 128-255 for high inten. */

	 if (s > 0.50)        /* more than half saturated? */
	    color |= 0x40;    /* color range 64-128 or 192-255 */
      }
   }
   switch (whichvga)    /* decide on (S)VGA bank switching scheme to use */
	{
	   case BASIC_VGA:      /* none */
		svga_loc = svga_yincr * y + x;
		break;

	   case MODE13x:        /* faked */
		svga_word = 1 << (x & 3);       /* form bit plane mask */
		svga_word = (svga_word << 8) | 2;
		outpw(SEQUENCER, svga_word);    /* tweak the sequencer */
		svga_loc = svga_yincr * y + (x >> 2);
		break;

	   default:             /* actual bank switch for all SVGA cards */
		svga_loc=((unsigned long)svga_yincr) * y + x;
		svga_page=(unsigned char)(svga_loc/gran);
		svga_loc %= gran;
		if (cur_page != svga_page)      /* if not in correct bank */
		{
		    cur_page = svga_page;       /* set new working bank */
		    if (whichvga != VESA)
		    {
			_disable();
		    newbank();
			_enable();
		}
		    else
			newbank();
		}
		break;
	}

#ifdef DOS386
   fp = MK_FP(_x386_zero_base_selector, 0xA0000 + (unsigned int)svga_loc);
#else
#ifdef DOS16RM
   fp = display_base + (unsigned int)svga_loc;
#else
   fp = (unsigned char *)MK_FP(0xA000, (unsigned int)svga_loc);
#endif
#endif
   *fp = color;         /* write normalized pixel color val to bitplane */

   return;
   }

void newbank()          /* Perform SVGA bank switch on demand - Voila! */
{
    register unsigned char tmp_byte, tmp_byte1;
    register unsigned int tmp_word;
    union REGS regs;
    static unsigned char xlateT3[] = {0x40, 0x49, 0x52, 0x5B, 0x64, 0x6D,
	    0x76, 0x7F};
    static unsigned char xlateT4[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
	    0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};


    switch (whichvga)
	{
	case VDC600:                    /* AT&T VDC 600 */
	    tmp_byte = (unsigned char) (cur_page << 4); /* was >> 12... */
	    outpw(0x03CE,0x050F);
	    outp(0x03CE,0x09);
	    outp(0x03CF, tmp_byte);
	    break;
	case OAKTECH:                   /* Oak Technology OTI-067 */
	    tmp_byte = (unsigned char)(cur_page & 0x0F);
	    outp(0x3DF, (tmp_byte << 4) | tmp_byte);
	    break;
	case AHEADA:                    /* Ahead Systems Ver A */
	    outpw(0x3CE, 0x200F);       /* enable extended registers */
	    tmp_byte = (unsigned char)(inp(0x3CC) & 0xDF);  /* bit 0 */
	    if (cur_page & 1)
		    tmp_byte |= 0x20;
	    outp(0x3C2, tmp_byte);
	    outp(0x3CF, 0);                /* bits 1, 2, 3 */
	    tmp_word = (unsigned int)((cur_page >> 1 )|(inp(0x3D0) & 0xF8));
	    outpw(0x3CF, tmp_word << 8);
	    break;
	case AHEADB:                    /* Ahead Systems Ver B */
	    outpw(0x3CE, 0x200F);       /* enable extended registers */
	    tmp_word = (unsigned int)((cur_page << 4) | cur_page);
	    outpw(0x3CF, (tmp_word << 8) | 0x000D);
	    break;
	case EVEREX:                    /* Everex SVGA's */
	    outp(0x3C4, 8);
	    if (cur_page & 1)
		 tmp_word = (unsigned int)(inp(0x3C5) | 0x80);
	    else tmp_word = (unsigned int)(inp(0x3C5) & 0x7F);
	    outpw(0x3C4, (tmp_word << 8) | 0x0008);
	    tmp_byte = (unsigned char)(inp(0x3CC) & 0xDF);
	    if (!(cur_page & 2))
		 tmp_byte |= 0x20;
	    outp(0x3C2, tmp_byte);
	    break;
	case ATIVGA:                    /* ATI VGA Wonder (and the XL) */
	    outp(0x1CE, 0xB2);
	    tmp_word = (unsigned int)((cur_page << 1) | (inp(0x1CF) & 0xE1));
	    outpw(0x1CE, (tmp_word << 8) | 0x00B2);
	    break;
	case TRIDENT:
	    outp(0x3CE, 6);             /* set page size to 64K */
	    tmp_word = (unsigned int)(inp(0x3CF) | 4) << 8;
	    outpw(0x3CE, tmp_word | 0x0006);
	    outp(0x3C4, 0x0b);          /* switch to BPS mode */
	    inp(0x3C5);                 /* dummy read?? */
	    tmp_word = (unsigned int)(cur_page ^ 2) << 8;
	    outpw(0x3C4, tmp_word | 0x000E);
	    break;
	case VIDEO7:                    /* Video-7 VRAM, FastRAM SVGA cards */
	    tmp_byte1 = tmp_byte = (unsigned char)(cur_page & 0x0F);
	    outpw(0x3C4, 0xEA06);
	    tmp_word = (unsigned int)(tmp_byte & 1) << 8;
	    outpw(0x3C4, tmp_word | 0x00F9);
	    tmp_byte &= 0x0C;
	    tmp_word = (unsigned int)((tmp_byte >> 2) | tmp_byte) << 8;
	    outpw(0x3C4, tmp_word | 0x00F6);
	    tmp_word |= (inp(0x3C5) & 0xF0) << 8;
	    outpw(0x3C4, tmp_word | 0x00F6);
	    tmp_byte = (unsigned char)((tmp_byte1 << 4) & 0x20);
	    outp(0x3C2, (inp(0x3CC) & 0xDF) | tmp_byte);
	    break;
	case CHIPSTECH:                 /* Chips & Technology VGA Chip Set */
	    outp(0x3D7, cur_page << 2); /* this is all that's necessary?? */
	    break;
	case PARADISE:                  /* Paradise, Professional, Plus */
	    outpw(0x3CE, 0x050F);       /* turn off VGA reg. write protect */
	    tmp_word = (unsigned int)(cur_page << 4) << 8;
	    outpw(0x3CE, tmp_word | 0x0009);
	    break;
	case TSENG3:                    /* Tseng 3000 - Orchid, STB, etc. */
	    outp(0x3CD, xlateT3[cur_page & 0x07]);
	    break;
	case TSENG4:                    /* Tseng 4000 - Orchid PD+, etc. */
	    outp(0x3CD, xlateT4[cur_page & 0x0F]);
	    break;
	case VESA:                      /* VESA standard mode bank switch */
	    regs.x.ax = 0x4F05;         /* VESA BIOS bank switch call */
	    regs.x.dx = cur_page;
	    regs.x.bx = map_code;       /* Map code 0 or 1? */
#ifdef DOS386
	    int86_real(0x10, &regs, &regs);
#else
	    int86(0x10, &regs, &regs);  /* Do the video BIOS interrupt */
#endif
	}
    return;
}

void set_palette_register (Val, Red, Green, Blue)
   unsigned Val;
   unsigned Red, Green, Blue;
   {
   union REGS Regs;

   Regs.x.ax = 0x1010;              /* Set one palette register function */
   Regs.x.bx = Val;                 /* the palette register to set (color#)*/
   Regs.h.dh = (char)(Red & 0x3f);  /* set the gun values (6 bits ea.) */
   Regs.h.ch = (char)(Green & 0x3f);
   Regs.h.cl = (char)(Blue & 0x3f);
   int86(0x10, &Regs, &Regs);       /* Do the video interrupt */
   }

/* Conversion from Hue, Saturation, Value to Red, Green, and Blue and back */
/* From "Computer Graphics", Donald Hearn & M. Pauline Baker, p. 304 */

void hsv_to_rgb(hue, s, v, r, g, b)
   DBL hue, s, v;               /* hue (0.0-360.0) s and v from 0.0-1.0) */
   unsigned *r, *g, *b;         /* values from 0 to 63 */
   {
   register DBL i, f, p1, p2, p3;
   register DBL xh;
   register DBL nr, ng, nb;     /* rgb values of 0.0 - 1.0 */

   if (hue == 360.0)
      hue = 0.0;                /* (THIS LOOKS BACKWARDS BUT OK) */

   xh = hue / 60.0;             /* convert hue to be in 0,6     */
   i = floor(xh);               /* i = greatest integer <= h    */
   f = xh - i;                  /* f = fractional part of h     */
   p1 = v * (1 - s);
   p2 = v * (1 - (s * f));
   p3 = v * (1 - (s * (1 - f)));

   switch ((int) i)
      {
      case 0:
	 nr = v;
	 ng = p3;
	 nb = p1;
	 break;
      case 1:
	 nr = p2;
	 ng = v;
	 nb = p1;
	 break;
      case 2:
	 nr = p1;
	 ng = v;
	 nb = p3;
	 break;
      case 3:
	 nr = p1;
	 ng = p2;
	 nb = v;
	 break;
      case 4:
	 nr = p3;
	 ng = p1;
	 nb = v;
	 break;
      case 5:
	 nr = v;
	 ng = p1;
	 nb = p2;
	 break;
      default:
	 nr = ng = nb = 0;
	}

   *r = (unsigned)(nr * 63.0); /* Normalize the values to 63 */
   *g = (unsigned)(ng * 63.0);
   *b = (unsigned)(nb * 63.0);
   
   return;
   }


void rgb_to_hsv(r, g, b, h, s, v)
   unsigned r, g, b;
   DBL *h, *s, *v;
   {
   register DBL m, r1, g1, b1;
   register DBL nr, ng, nb;             /* rgb values of 0.0 - 1.0 */
   register DBL nh = 0.0, ns, nv;       /* hsv local values */

   nr = (DBL) r / 255.0;
   ng = (DBL) g / 255.0;
   nb = (DBL) b / 255.0;

   nv = max (nr, max (ng, nb));
   m = min (nr, min (ng, nb));

   if (nv != 0.0)                /* if no value, it's black! */
      ns = (nv - m) / nv;
   else
      ns = 0.0;                  /* black = no colour saturation */

   if (ns == 0.0)                /* hue undefined if no saturation */
   {
      *h = 0.0;                  /* return black level (?) */
      *s = 0.0;
      *v = nv;
      return;
   }

   r1 = (nv - nr) / (nv - m);    /* distance of color from red   */
   g1 = (nv - ng) / (nv - m);    /* distance of color from green */
   b1 = (nv - nb) / (nv - m);    /* distance of color from blue  */

   if (nv == nr)
   {
      if (m == ng)
	 nh = 5. + b1;
      else
	 nh = 1. - g1;
   } 

   if (nv == ng)
      {
      if (m == nb)
	 nh = 1. + r1;
      else
	 nh = 3. - b1;
      }

   if (nv == nb)
      {
      if (m == nr)
	 nh = 3. + g1;
      else
	 nh = 5. - r1;
      }

   *h = nh * 60.0;      /* return h converted to degrees */
   *s = ns;
   *v = nv;
   return;
   }

void box(x1, y1, x2, y2)  /* RHA's draw a thin white box funct. */
int x1, y1, x2, y2;
{
   int i;
   for (i = x1+1; i < x2; i++)
   {
	display_plot(i, y1, 255, 255, 255);
	display_plot(i, y2, 255, 255, 255);
   }
   for (i = y1; i <= y2; i++)
   {
	display_plot(x1, i, 255, 255, 255);
	display_plot(x2, i, 255, 255, 255);
   }
   return;
}

#if !__STDC__

/* ANSI Standard psuedo-random number generator */

static unsigned long int next = 1;

int rand()
   {
   next = next * 1103515245L + 12345L;
   return ((int) (next / 0x10000L) & 0x7FFF);
   }

void srand(seed)
   unsigned int seed;
   {
   next = seed;
   }

#endif


/* Math Error exception struct format:
	int type;               - exception type - see below
	char _far *name;        - name of function where error occured
	long double arg1;       - first argument to function
	long double arg2;       - second argument (if any) to function
	long double retval;     - value to be returned by function
*/

#ifdef __WATCOMC__
#define EDOM    7       /* MSC is 33 */
#define ERANGE  8       /* MSC is 34 */

int matherr(e)
   struct exception *e;
#else
#ifdef GCCDOS
int matherr(e)
   struct libm_exception *e;
#else
int _cdecl matherr(e)
   struct exception *e;
#endif
#endif
   {
   if (Options & DEBUGGING) {
      /* Since we are just making pictures, not keeping nuclear power under
	 control - it really isn't important if there is a minor math problem.
	 This routine traps and ignores them.  Note: the most common one is
	 a DOMAIN error coming out of "acos". */
      switch (e->type) {
	 case DOMAIN   : printf("DOMAIN error in '%s'\n", e->name); break;
	 case SING     : printf("SING   error in '%s'\n", e->name); break;
	 case OVERFLOW : printf("OVERFLOW error in '%s'\n", e->name); break;
	 case UNDERFLOW: printf("UNDERFLOW error in '%s'\n", e->name); break;
	 case TLOSS    : printf("TLOSS error in '%s'\n", e->name); break;
	 case PLOSS    : printf("PLOSS error in '%s'\n", e->name); break;
#ifdef EDOM
	 case EDOM     : printf("EDOM error in '%s'\n", e->name); break;
#endif
#ifdef ERANGE
	 case ERANGE   : printf("ERANGE error in '%s'\n", e->name); break;
#endif
	 default       : printf("Unknown math error in '%s'\n",e->name);break;
	 }
      }
   return (1);  /* Indicate the math error was corrected... */
   }

#ifdef GCCDOS
unsigned char *find_go32(go32_offset)
   int *go32_offset;
   {
   static unsigned char *memptr = (unsigned char *)0xE0000000;
   static unsigned char buf[0x1000];

   union REGS reg;
   unsigned char *p;

   reg.x.si = (int)buf;
   reg.h.ah = 0x47;
   int86(0x21, &reg, &reg);

   for (p = &memptr[0xA0000-1-strlen((char *)buf)]; p != memptr; p--)
      if (buf[0] == *p && strcmp((char *)buf, (char *)p) == 0)
	 break;

   if (p == memptr) {
      printf("Unable to locate the GO32 communication buffer, fatal error!\n");
      exit(1);
      }

   reg.h.ah = 0x1A;
   reg.x.dx = (int)buf;
   int86(0x21, &reg, &reg);

   reg.h.ah = 0x4E;
   reg.x.dx = (int)"*.*";
   int86(0x21, &reg, &reg);
   *go32_offset = (reg.x.dx & 0xFFFF) - 43;

   return(p);
   }
#endif

#ifdef _INTELC32_

void int10 ( void )         /* User Int 10h Protected-Mode Interrupt Handler */
   {
   _XSTACK *ebp;            /* Stack frame passed to real-mode handler */
   _XSTACK *ebx;            /* Stack frame returned to interrupted process */

   ebp = (_XSTACK *) _get_stk_frame(); /* Get stack frame address */
   if ((ebp->eax & 0x0000FFFE) != 0x4F00) { /* Check for function = 4F00/1h */
      (*prev_int10)();             /* Call previous handler? */
      return;
      }

   if ((ebp->edi & 0xFFF00000) != 0) {
      ebx = (_XSTACK *) _get_ebx();/* Get return stack frame address */
      ebx->eax = 0x0000104F;       /* Return code of 0x10=Extended mem err */
      ebp->flg |= _FLAG_CARRY;     /* Set carry flag to indicate an error */
      ebp->opts |= _STK_NOINT;     /* Bypass real-mode handler */
      return;
      }

   /* Put the segment value from the physical address in the ES.  */
   ebp->es = (short)((ebp->edi & 0x000FFFF0) >> 4);
   ebp->edi &= 0x0000000F;         /* Offset value from physical address */
   ebp->opts = 0;                  /* Pass to real-mode handler */
   return;
   }

#endif

void print_ibm_credits()
  {
  fprintf (stderr," \n");
  
  fprintf (stderr,"ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»\n");
  box_fprintf (stderr,"  Persistence of Vision Raytracer Ver 1.0%s",COMPILER_VER);
  fprintf (stderr,"ºÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄº\n");
  fprintf (stderr,"º  Copyright (c) 1992 POV-Team                                           º\n");
  fprintf (stderr,"º  POV-Ray is based on DKBTrace 2.12 by David K. Buck & Aaron A. Collins.º\n");
  fprintf (stderr,"ºÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄº\n");
  fprintf (stderr,"º  Contributing Authors: (Alphabetically)                                º\n");
  fprintf (stderr,"ºÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄº\n");
  fprintf (stderr,"º  Steve A. Bennett   David K. Buck      Aaron A. Collins                º\n");
  fprintf (stderr,"º  Alexander Enzmann  Dan Farmer         Girish T. Hagan                 º\n");
  fprintf (stderr,"º  Douglas Muir       Bill Pulver        Robert Skinner                  º\n");
  fprintf (stderr,"º  Scott Taylor       Drew Wells         Chris Young                     º\n");
  fprintf (stderr,"º  -Other contributors listed in the documentation.-                     º\n");
  fprintf (stderr,"ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼\n");
  
  }  

/* Stats kept by the ray tracer: */
extern long Number_Of_Pixels, Number_Of_Rays, Number_Of_Pixels_Supersampled;
extern long Ray_Sphere_Tests, Ray_Sphere_Tests_Succeeded;
extern long Ray_Box_Tests, Ray_Box_Tests_Succeeded;
extern long Ray_Blob_Tests, Ray_Blob_Tests_Succeeded;
extern long Ray_Plane_Tests, Ray_Plane_Tests_Succeeded;
extern long Ray_Triangle_Tests, Ray_Triangle_Tests_Succeeded;
extern long Ray_Quadric_Tests, Ray_Quadric_Tests_Succeeded;
extern long Ray_Poly_Tests, Ray_Poly_Tests_Succeeded;
extern long Ray_Bicubic_Tests, Ray_Bicubic_Tests_Succeeded;
extern long Ray_Ht_Field_Tests, Ray_Ht_Field_Tests_Succeeded;
extern long Ray_Ht_Field_Box_Tests, Ray_HField_Box_Tests_Succeeded;
extern long Bounding_Region_Tests, Bounding_Region_Tests_Succeeded;
extern long Clipping_Region_Tests, Clipping_Region_Tests_Succeeded;
extern long Calls_To_Noise, Calls_To_DNoise;
extern long Shadow_Ray_Tests, Shadow_Rays_Succeeded;
extern long Reflected_Rays_Traced, Refracted_Rays_Traced;
extern long Transmitted_Rays_Traced;
extern time_t tstart, tstop;
extern DBL    tused;             /* Trace timer variables. - BP */ 
extern FRAME Frame;

extern char Input_File_Name[FILE_NAME_LENGTH], Output_File_Name[FILE_NAME_LENGTH], Stat_File_Name[FILE_NAME_LENGTH];

void print_ibm_stats()
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
   
   fprintf (stat_out,"\n");
   fprintf (stat_out,"ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»\n");
   box_fprintf (stat_out,"  Persistence of Vision Raytracer Ver 1.0%s",COMPILER_VER);
   fprintf (stat_out,"ºÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄº\n");
   box_fprintf (stat_out,"  %s statistics",Input_File_Name);
   if(Pixels_In_Image > Number_Of_Pixels)
   box_fprintf (stat_out,"  Partial Image Rendered");
   fprintf (stat_out,"ºÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄº\n");
   box_fprintf (stat_out," Image Resolution %d pixels wide x %d pixels high",Frame.Screen_Width, Frame.Screen_Height);
   box_fprintf (stat_out," # Rays Calculated     :  %10ld",Number_Of_Rays);
   box_fprintf (stat_out," # Pixels Calculated   :  %10ld", Number_Of_Pixels);
   box_fprintf (stat_out," # Pixels Supersampled :  %10ld",Number_Of_Pixels_Supersampled);
   fprintf (stat_out,"ºÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄº\n");
   box_fprintf (stat_out,"   Ray -¯ Shape Intersection Tests");
   box_fprintf (stat_out,"   Type             Tests    Succeeded   Percentage");
   fprintf (stat_out,"ºÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄº\n");
   if(Ray_Sphere_Tests)
   box_fprintf (stat_out,"  Sphere       %10ld  %10ld  %10.2f", Ray_Sphere_Tests, Ray_Sphere_Tests_Succeeded, ( ((DBL)Ray_Sphere_Tests_Succeeded/(DBL)Ray_Sphere_Tests) *100.0 ) );
   if(Ray_Plane_Tests)
   box_fprintf (stat_out,"  Plane        %10ld  %10ld  %10.2f", Ray_Plane_Tests, Ray_Plane_Tests_Succeeded, ( ((DBL)Ray_Plane_Tests_Succeeded/(DBL)Ray_Plane_Tests) *100.0 ));
   if(Ray_Triangle_Tests)
   box_fprintf (stat_out,"  Triangle     %10ld  %10ld  %10.2f", Ray_Triangle_Tests, Ray_Triangle_Tests_Succeeded, ( ((DBL)Ray_Triangle_Tests_Succeeded/(DBL)Ray_Triangle_Tests) *100.0 ));
   if(Ray_Quadric_Tests)
   box_fprintf (stat_out,"  Quadric      %10ld  %10ld  %10.2f", Ray_Quadric_Tests, Ray_Quadric_Tests_Succeeded, ( ((DBL)Ray_Quadric_Tests_Succeeded/(DBL)Ray_Quadric_Tests) *100.0 ));
   if(Ray_Blob_Tests)
   box_fprintf (stat_out,"  Blob         %10ld  %10ld  %10.2f", Ray_Blob_Tests, Ray_Blob_Tests_Succeeded, ( ((DBL)Ray_Blob_Tests_Succeeded/(DBL)Ray_Blob_Tests) *100.0 ));
   if(Ray_Box_Tests)
   box_fprintf (stat_out,"  Box          %10ld  %10ld  %10.2f", Ray_Box_Tests, Ray_Box_Tests_Succeeded, ( ((DBL)Ray_Box_Tests_Succeeded/(DBL)Ray_Box_Tests) *100.0 ));
   if(Ray_Poly_Tests)
   box_fprintf (stat_out,"  Quartic\\Poly %10ld  %10ld  %10.2f", Ray_Poly_Tests, Ray_Poly_Tests_Succeeded, ( ((DBL)Ray_Poly_Tests_Succeeded/(DBL)Ray_Poly_Tests) *100.0 ));
   if(Ray_Bicubic_Tests)
   box_fprintf (stat_out,"  Bezier Patch %10ld  %10ld  %10.2f", Ray_Bicubic_Tests, Ray_Bicubic_Tests_Succeeded, ( ((DBL)Ray_Bicubic_Tests_Succeeded/(DBL)Ray_Bicubic_Tests) *100.0 ));
   if(Ray_Ht_Field_Tests)
   box_fprintf (stat_out,"  Height Fld   %10ld  %10ld  %10.2f", Ray_Ht_Field_Tests, Ray_Ht_Field_Tests_Succeeded, ( ((DBL)Ray_Ht_Field_Tests_Succeeded/(DBL)Ray_Ht_Field_Tests) *100.0 ));
   if(Ray_Ht_Field_Box_Tests)
   box_fprintf (stat_out,"  Hght Fld Box %10ld  %10ld  %10.2f", Ray_Ht_Field_Box_Tests, Ray_HField_Box_Tests_Succeeded, ( ((DBL)Ray_HField_Box_Tests_Succeeded/(DBL)Ray_Ht_Field_Box_Tests) *100.0 ));
   if(Bounding_Region_Tests)
   box_fprintf (stat_out,"  Bounds       %10ld  %10ld  %10.2f", Bounding_Region_Tests, Bounding_Region_Tests_Succeeded, ( ((DBL)Bounding_Region_Tests_Succeeded/(DBL)Bounding_Region_Tests) *100.0 ));
   if(Clipping_Region_Tests)
   box_fprintf (stat_out,"  Clips        %10ld  %10ld  %10.2f", Clipping_Region_Tests, Clipping_Region_Tests_Succeeded, ( ((DBL)Clipping_Region_Tests_Succeeded/(DBL)Clipping_Region_Tests) *100.0 ));
   fprintf (stat_out,"ºÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄº\n");
   if(Calls_To_Noise)
   box_fprintf (stat_out,  "  Calls to Noise Routine  :  %10ld", Calls_To_Noise);
   if(Calls_To_DNoise)
   box_fprintf (stat_out,  "  Calls to DNoise Routine :  %10ld", Calls_To_DNoise);
   fprintf (stat_out,"ºÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄº\n");
   if(Shadow_Ray_Tests){
     box_fprintf (stat_out,"  Shadow Ray Tests        :  %10ld",Shadow_Ray_Tests);
     box_fprintf (stat_out,"  Blocking Objects Found  :  %10ld",Shadow_Rays_Succeeded);
     }
   if(Reflected_Rays_Traced)
   box_fprintf (stat_out,  "  Reflected Rays          :  %10ld", Reflected_Rays_Traced);
   if(Refracted_Rays_Traced)                         
   box_fprintf (stat_out,  "  Refracted Rays          :  %10ld", Refracted_Rays_Traced);
   if(Transmitted_Rays_Traced)
   box_fprintf (stat_out,  "  Transmitted Rays        :  %10ld", Transmitted_Rays_Traced);
   
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
     fprintf (stat_out,"ºÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄº\n");
     box_fprintf (stat_out,"  Rendering Time          :   %5d hours %2d minutes %4.2f seconds", hours,min,sec); 
     }
     fprintf (stat_out,"ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼\n");

    if (Options & VERBOSE_FILE)
      fclose(stat_out);
 
}


/* Output a string to stream with vfprintf formatted for ibm stat screen. */
/* String should be <80 chars                 */
int _cdecl box_fprintf( FILE *stream, char *fmt , ... )
{                     
    va_list marker;
    char buffer[160];
    int len,i;
    
    buffer[0] = 'º';
    va_start( marker, fmt );
    vsprintf( buffer+1, fmt, marker );
    va_end( marker );
    len = strlen (buffer);
    for(i=len;i<73;i++)
       buffer[i] = ' ';
    strcpy (buffer+73,"º\n");   
    return( fputs(buffer,stream) );
}
