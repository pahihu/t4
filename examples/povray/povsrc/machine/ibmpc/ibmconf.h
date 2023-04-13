/****************************************************************************
*                   ibmconf.h
*
*  This file contains the IBM-PC specific defines. Rename to config.h when 
*  compiling for IBM-PC.
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
/* Below are several defines needed by the generic parts of POV.  You
*   may redefine them as needed in this config.h file.
*
*   The following constants are needed by various POV modules.  Each has
*   a default (shown below) which is will be defined in frame.h if you don't 
*   define them here.
*
*   #define EPSILON 1.0e-10               - a small value used for POLY
*   #define FILE_NAME_LENGTH 150          - default file name length
*   #define HUGE_VAL 1.0e+17              - a really big number
*   #define DBL_FORMAT_STRING "%lf"       - the string to use for scanfs of doubles
*   #define DEFAULT_OUTPUT_FORMAT  'd'    - the default +f format
*   #define RED_RAW_FILE_EXTENSION ".red" - for +fr, this is the extension for the
*                                           red file
*   #define GREEN_RAW_FILE_EXTENSION ".grn"  - ditto for the green file
*   #define BLUE_RAW_FILE_EXTENSION ".blu"   - ditto again for the blue file
*   #define FILENAME_SEPARATOR "/"        - the character that separates names
*                                           in a path.
*   #define CASE_SENSITIVE_DEFAULT 2     - controls case sensitivity in DAT files
*   #define READ_FILE_STRING "rb"        - special binary (raw, uncooked) 
*   #define WRITE_FILE_STRING "wb"         modes of fopen()
*   #define APPEND_FILE_STRING "ab"
*   #define NORMAL '0'                   - machine-specific PaletteOption settings 
*   #define GREY   'G'
*
*   These routines are required by POV to compute trace times.  The defaults 
*   shown below are defined in most versions of C.  You may redefine them if
*   your compiler doesn't support these.  If time is completely unsupported 
*   define them as 0.
*
*   #define START_TIME time(&tstart);     
*   #define STOP_TIME  time(&tstop);
*   #define TIME_ELAPSED difftime (tstop, tstart);
*
*   Note difftime can be replaced with: #define TIME_ELAPSED (tstop - tstart); 
*   in some cases. 
*
*   These are optional routines that POV calls.  You may define them
*   or if undefined they will be defined as empty in frame.h.
*
*   #define STARTUP_POVRAY             - first executable statement of main
*                                           (useful for initialization)
*   #define PRINT_OTHER_CREDITS           - used for people who extend the machine specific modules
*                                           (put your own printf's here)
*   #define TEST_ABORT                    - code to test for ^C or abort
*                                           (called on each pixel)
*   #define FINISH_POVRAY              - last statement before exiting normally
*   #define COOPERATE                    - called for multi-tasking
*  
*   These items are standard on some C compilers.  If they are not defined
*   on your compiler or are called something different you may define your 
*   own or use the defaults supplied in frame.h as shown below.
*
*   #define DBL double                    - floating point precision
*   #define ACOS acos                     - arc cosine of DBL
*   #define SQRT sqrt                     - square root of DBL
*   #define POW pow                       - x to the y power - x, y DBL
*   #define COS cos                       - cosine of DBL
*   #define SIN sin                       - sine of DBL
*   #define labs(x) (long) ((x<0)?-x:x)   - Returns the absolute value of long integer x
*   #define max (x,y) ((x<y)?y:x)         - greater of x and y
*
*   On some systems "sprintf" returns the string rather than the length
*   of the string.  If the string is returned, define the following:
*   #define STRLN(x) strlen(x)
*
*   #define setvbuf(w,x,y,z)              - some systems don't understand setvbuf.
*                                           If not, just define it away - it's
*                                           not critical to the raytracer.  It
*                                           just buffers disk accesses.
*   This macro controls prototyping and is required by POV.  Choose one
*   of the two options below or if left undefined it will default to ANSI 
*   in frame.h
*
*   #define PARAMS (x) x                  - do ANSI prototyping
*   #define PARAMS (x) ()                 - do K&R non-ANSI prototyping
*
*   These allow command line vs non-command line versions and alternate
*   main routines.  Normally they should be undefined.  Define them 1 if needed.
*   #define ALTMAIN 1
*   #define NOCMDLINE 1
****************************************************/
#ifdef _INTELC32_
#include <i32.h>
#include <float.h>
#endif

#ifndef GCCDOS
#include <conio.h>
#endif

#include <stdarg.h>
#include <stdlib.h>

#ifdef MATH_CO
#define DBL long double
#else
#define DBL double
#endif

#ifdef MATH_CO
#define DBL_FORMAT_STRING "%Lf"
#else
#define DBL_FORMAT_STRING "%lf"
#endif

#define RED_RAW_FILE_EXTENSION   ".r8"  /* PICLAB's "RAW" file format */
#define GREEN_RAW_FILE_EXTENSION ".g8"  /* PICLAB's "RAW" file format */
#define BLUE_RAW_FILE_EXTENSION  ".b8"  /* PICLAB's "RAW" file format */

#define FILENAME_SEPARATOR "\\"         /* weird 'ol IBM's like backslashes */

#if defined(MATH_CO) && !defined(__TURBOC__)
#define ACOS acosl
#define SQRT sqrtl
#define POW powl
#define COS cosl
#define SIN sinl
#define EPSILON 1.0e-15
#else
#define EPSILON 1.0e-5
#endif

#define DEFAULT_OUTPUT_FORMAT   't'

#define TEST_ABORT if (Options & EXITENABLE) if (kbhit()) { Stop_Flag = TRUE; getch(); }

/* defines for machine-specific PaletteOption settings */
#define NORMAL  '0'
#define GREY    'G'
#define HSV     '0'
#define P_332   '3'
#define HICOLOR 'H'             /* 16 bits/pixel "high" color */
#define FULLCOLOR 'T'           /* 24 bits/pixel "true" color */

#ifndef READ_ENV_VAR_BEFORE 
#define READ_ENV_VAR_BEFORE if ((Option_String_Ptr = getenv("POVRAYOPT")) != NULL) read_options(Option_String_Ptr);
#endif
#ifndef READ_ENV_VAR_AFTER
#define READ_ENV_VAR_AFTER     
#endif


/**********************************
*    The defines below have nothing to do with the generic part of POV.
*    They are entirely machine specific.  In general do not include them 
*    when porting to other platforms.
***********************************/

#ifdef _INTELC32_
 #define COMPILER_VER ".ibmicb"
 #define CONFIG_MATH _control87(MCW_EM, MCW_EM);
#endif

#ifdef __MSC__
 #define COMPILER_VER ".ibmmsc"
#endif

#ifdef __ZTC__
 #ifndef __386__
  #define COMPILER_VER ".ibmztc"
 #endif
 #ifdef __386__
  #define COMPILER_VER ".ibmztc386"
 #endif
#endif


#ifdef GCCDOS
void main(int, char **);
#ifdef MK_FP
#undef MK_FP
#endif
#define MK_FP(seg, ofs) ((void *)(0xE0000000 + ((seg)<<4) + ofs))
#ifdef FP_OFF
#undef FP_OFF
#endif
#define FP_OFF(ptr) (unsigned short)(ptr)
#ifdef FP_SEG                   /* note: this produces a warning at >= /W3 */
#undef FP_SEG
#endif
#define FP_SEG(ptr) (unsigned short)(((unsigned long)ptr >> 16) & 0x0FFF)
#ifndef min
#define min(x,y) ((x)<(y)?(x):(y))
#endif
#ifndef max
#define max(x,y) ((x)>(y)?(x):(y))
#endif

#undef  outpw( portid,v )
#undef  outp( portid,v )
#undef  inp( portid )

#define outpw( portid,v )  outportw( portid,v )
#define outp( portid,v )  outportb( portid,v )
#define inp( portid ) inportb( portid )

#define getch() getkey()
#define _enable()
#define _disable()
#else
#ifdef _INTELC32_
#ifdef MK_FP
#undef MK_FP
#endif
#define MK_FP(seg, ofs) ((void _far *)(((seg)<<4) + ofs))
#ifndef far
#define far _far
#endif
#ifdef FP_OFF
#undef FP_OFF
#endif
#define FP_OFF(ptr) (unsigned short)(ptr)
#ifdef FP_SEG
#undef FP_SEG
#endif
#define FP_SEG(ptr) (unsigned short)((unsigned long)ptr >> 16)

#else


#ifdef __BORLANDC__
 #define COMPILER_VER ".ibmbc"
 #define IFF_SWITCH_CAST (long)
#endif

#ifdef __TURBOC__
#ifndef __BORLANDC__
 #define COMPILER_VER ".ibmbtc"
#endif
#endif

#ifdef __WATCOMC__
 #ifndef __386__
  #define COMPILER_VER ".ibmwat"
 #endif
 #ifdef __386__
  #define COMPILER_VER ".ibmwat386"
 #endif
#endif


#ifndef __ZTC__
#ifndef __TURBOC__
#ifdef __WATCOMC__

#ifdef __386__
#ifdef MK_FP
#undef MK_FP
#endif
#define MK_FP(seg,ofs)  ((void *)(((unsigned long)(seg) << 4) | (unsigned)(ofs)))
#ifdef FP_OFF
#undef FP_OFF
#endif
#define FP_OFF(ptr) (((unsigned short)ptr) & 0x0F)
#ifdef FP_SEG
#undef FP_SEG
#endif
#define FP_SEG(ptr) (unsigned int)((unsigned long)ptr >> 4)
#endif

#else

void _cdecl main(int, char **); /* so MSC can use FASTCALL's... */
#define main (_cdecl main)
#ifndef MK_FP
#define MK_FP(seg,ofs)  ((void far *)(((unsigned long)(seg) << 16) | (unsigned)(ofs)))
#endif
#ifdef FP_OFF
#undef FP_OFF
#endif
#define FP_OFF(ptr) (unsigned int)(ptr) /* MSC's version doesn't work!?!? */
#ifdef FP_SEG                   /* note: this produces a warning at >= /W3 */
#undef FP_SEG
#endif
#define FP_SEG(ptr) (unsigned int)((unsigned long)ptr >> 16)
#endif

#else

#define _cdecl  cdecl   /* Turbo-C equivalent function names/keywords */
#define _enable enable
#define _disable disable

#undef  outpw( portid,v )
#undef  outp( portid,v )
#undef  inp( portid )

#define outpw( portid,v )  outport( portid,v )
#define outp( portid,v )  outportb( portid,v )
#define inp( portid ) inportb( portid )
#endif
#endif
#endif
#endif

#ifdef __WATCOMC__
#ifndef COMPILER_VER
#define COMPILER_VER ".ibmwat"
#endif
double sqrt(double x);
#pragma aux sqrt = 0xD9 0xFA parm [8087] value [8087];
double sin(double x);
#pragma aux sin = 0xD9 0xFE parm [8087] value [8087];
double cos(double x);
#pragma aux cos = 0xD9 0xFF parm [8087] value [8087];
double fabs(double x);
#pragma aux fabs = 0xD9 0xE1 parm [8087] value [8087];
#endif

#ifdef GCCDOS
double fmod(double, double);
#define COMPILER_VER ".ibmgcc"
#endif

#define PRINT_CREDITS print_ibm_credits();
#define PRINT_STATS print_ibm_stats();
#define WAIT_FOR_KEYPRESS getch();


void print_ibm_credits (void);
void print_ibm_stats(void);
