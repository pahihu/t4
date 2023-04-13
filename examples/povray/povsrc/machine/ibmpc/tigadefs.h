/*-----------------------------------------------------------------------*/
/*                                 TIGA                                  */
/*              Copyright (c) 1988,89 Texas Instruments Inc.             */
/*-----------------------------------------------------------------------*/
/*  TIGA - Header file                                                   */
/*-----------------------------------------------------------------------*/
/*                                                                       */
/*  file            typedefs.h                                           */
/*                                                                       */
/*  description     This file contains type definitions used in          */ 
/*                  the Applications Interface.                          */
/*                  This file should be included in any Microsoft-C      */
/*                  program accessing data of these types.               */
/*                                                                       */
/*-----------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*  Define integral data types                                              */
/*--------------------------------------------------------------------------*/
typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned long   ulong;
typedef unsigned long   PTR;
typedef uchar far       *HPTR;


/*--------------------------------------------------------------------------*/
/*  CURSOR structure definition                                             */
/*                                                                          */
/*      This structure contains cursor description information.             */
/*--------------------------------------------------------------------------*/
typedef struct 
{
    short   hot_x;      /* offset from top...  */
    short   hot_y;      /* ...left-hand corner */
    ushort  width;      /* array width         */
    ushort  height;     /* array height        */
    ushort  pitch;
    ulong   color;      /* color of cursor     */
    ushort  mask_rop;   /* cursor mask rop     */
    ushort  shape_rop;  /* cursor shape rop    */
    PTR     data;       /* pointer to cursor data in GSP memory */
} CURSOR;


/*--------------------------------------------------------------------------*/
/*  PATTERN structure definition                                            */
/*                                                                          */
/*      This structure contains pattern description information.            */
/*--------------------------------------------------------------------------*/
typedef struct 
{
    ushort  width;      /* width of pattern    */
    ushort  height;     /* height of pattern   */
    ushort  depth;      /* depth (bits/pixel)  */
    PTR     data;       /* address of pattern in GSP memory */
} PATTERN;

	        
/*--------------------------------------------------------------------------*/
/*  PALET structure definition                                              */
/*                                                                          */
/*      This structure contains the rgb and intensity components for a      */
/*      palette entry.                                                      */
/*--------------------------------------------------------------------------*/
typedef struct 
{
    uchar   r;
    uchar   g;
    uchar   b;
    uchar   i;
} PALET;


/*--------------------------------------------------------------------------*/
/*  MONITORINFO structure definition                                        */
/*                                                                          */
/*      This structure contains video timing parameters.                    */
/*--------------------------------------------------------------------------*/
typedef struct
{
    ushort  hesync;
    ushort  heblnk;
    ushort  hsblnk;
    ushort  htotal;
    ushort  vesync;
    ushort  veblnk;
    ushort  vsblnk;
    ushort  vtotal;
    ushort  dpyctl;
    ushort  screen_delay;
    ushort  flags;
} MONITORINFO;


/*--------------------------------------------------------------------------*/
/*  OFFSCREEN structure definitions                                         */
/*                                                                          */
/*      These structures contain definitions of offscreen work areas        */
/*--------------------------------------------------------------------------*/
typedef struct 
{
    PTR     addr;   /* address in gsp space of an offscreen work area */
    ushort  xext;   /* x extension in the current screen pixel size   */
    ushort  yext;   /* y extension using default screen pitch         */
} OFFSCREEN_AREA;


/*--------------------------------------------------------------------------*/
/*  PAGE structure definitions                                              */
/*                                                                          */
/*      These structures contain definitions used to implement multiple     */
/*      display pages.  The information in these structures is used to      */
/*      display a given page.                                               */
/*--------------------------------------------------------------------------*/
typedef struct 
{
    PTR     BaseAddr;   /* Base address of start of page                */
    ushort  DpyStart;   /* Value loaded in Display Start i/o register   */
    short   DummyPad;   /* 16 bits to pad structure to power of 2 size  */
} PAGE;                 


/*--------------------------------------------------------------------------*/
/*  MODEINFO structure definition                                           */
/*                                                                          */
/*      This structure contains information pertaining to the current mode  */ 
/*      of operation.                                                       */
/*--------------------------------------------------------------------------*/
typedef struct
{
    ulong   disp_pitch;         /* display pitch y::x                   */
    ushort  disp_vres;          /* vertical resolution (pixels)         */
    ushort  disp_hres;          /* horizontal resolution (pixels)       */
    short   screen_wide;        /* screen width                         */
    short   screen_high;        /* screen height                        */
    ushort  disp_psize;         /* pixel size                           */
    ulong   pixel_mask;         /* pixel mask                           */
    ushort  palet_gun_depth;    /* # of bits per gun                    */    
    ulong   palet_size;         /* number of palette entries            */
    short   palet_inset;        /* offset from frame start to data      */ 
    ushort  num_pages;          /* number of display pages              */
    short   num_offscrn_areas;  /* number off offscreen work areas      */
    ulong   wksp_addr;          /* address of offscreen workspace       */
    ulong   wksp_pitch;         /* pitch of offscreen workspace         */
} MODEINFO;


/*--------------------------------------------------------------------------*/
/*  CONFIG structure definition                                             */
/*                                                                          */
/*      This structure contains the current hardware operating              */
/*      configuration.                                                      */
/*--------------------------------------------------------------------------*/
typedef struct 
{
    ushort      version_number;
    ulong       comm_buff_size;
    ulong       sys_flags;
    ulong       device_rev;
    ushort      num_modes;
    ushort      current_mode;
    ulong       program_mem_start;
    ulong       program_mem_end;
    ulong       display_mem_start;
    ulong       display_mem_end;
    ulong       stack_size;
    ulong       shared_mem_size;
    HPTR        shared_host_addr;
    PTR         shared_gsp_addr;
    MODEINFO    mode;
} CONFIG;


/*--------------------------------------------------------------------------*/
/*  ENVIRONMENT structure definition                                        */
/*                                                                          */
/*      This structure contains the current values of environment global    */
/*      variables.                                                          */
/*--------------------------------------------------------------------------*/
typedef struct 
{
    ulong   xyorigin;      /* xy drawing origin      */
    ulong   pensize;       /* pensize height::width  */
    PTR     srcbm;         /* source bitmap struct   */
    PTR     dstbm;         /*   dest bitmap struct   */
    ulong   stylemask;     /* styled line patn mask  */
} ENVIRONMENT;


/*----------------------------------------------------------------------*/
/*  MODULE structure definition                                         */
/*                                                                      */
/*  This structure defines the data associated with a function module.  */
/*----------------------------------------------------------------------*/
typedef struct
{
    PTR     ListAddr;
    PTR     MallocAddr;
    ulong   MallocBytes;
    ushort  NumFuncs;
    ushort  Dummy;
} MODULE;


/*----------------------------------------------------------------------*/
/*  FONT structure definition                                           */
/*                                                                      */
/*  This structure defines the data associated with a font              */
/*----------------------------------------------------------------------*/
typedef struct
{
    ushort magic;        /* font type code                               */
    long   length;       /* Length of font in bytes                      */
    char   facename[32];
    short  first;        /* ASCII code of first character                */
    short  last;         /* ASCII code of last character                 */
    short  maxwide;      /* maximum character width                      */
    short  maxkern;      /* maximum character kerning amount             */
    short  charwide;     /* Width of characters (0 if proportional)      */
    short  avgwide;      /* Average width of characters                  */
    short  charhigh;     /* character height                             */
    short  ascent;       /* ascent (how far above base line)             */
    short  descent;      /* descent (how far below base line)            */
    short  leading;      /* leading (row bottom to next row top)         */
    long   rowpitch;     /* bits per row of char patterns                */
    long   oPatnTbl;     /* Bit offset to PatnTbl (from bof)             */
    long   oLocTbl;      /* Bit offset to LocTbl (from bof)              */
    long   oOwTbl;       /* Bit offset to OWTbl (from bof)               */
} FONT;

/*----------------------------------------------------------------------*/
/*  FONTINFO structure definition                                       */
/*----------------------------------------------------------------------*/
typedef struct
{
    char  facename[32];
    short first;        /* ASCII code of first character                */
    short last;         /* ASCII code of last character                 */
    short maxwide;      /* maximum character width                      */
    short avgwide;      /* Average width of characters                  */
    short maxkern;      /* Max character kerning amount                 */
    short charwide;     /* Width of characters (0=proportional)         */
    short charhigh;     /* character height                             */
    short ascent;       /* ascent (how far above base line)             */
    short descent;      /* descent (how far below base line)            */
    short leading;      /* leading (row bottom to next row top)         */
    PTR   fontptr;      /* address of font in gsp memory                */
    short id;           /* id of font (set at install time)             */
} FONTINFO;

