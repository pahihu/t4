/****************************************************************************
*                xwindows.c
*
*  This module implements X-windows specific routines.
*
*  from Persistence of Vision Raytracer 
*  Copyright 1991 Persistence of Vision Team
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
 * Here are some routines I wrote which implement +d option on unix computers
 * running X-windows. For now, only black and white output is supported. If I
 * get access to a computer with a color monitor, I'll probably add support
 * for colors to my routines.
 * 
 * In future I'll probably add some more dithering methods. I have tested these
 * routines on SUN 3 and SUN 4. I'm using the tvtwm window manager.
 * 
 * If you have some suggestions to my source code or if you change anything,
 * please let me know. I can be reached at the following address: Marek
 * Rzewuski, Avstikkeren 11, 1156 Oslo 11, Norway or marekr@ifi.uio.no on
 * Internet.
 */

/*
 * 91-07-29 Allan H. Wax (Wax.OSBU_South@Xerox.COM) Hacked this module so it
 * can be used with either a colour or B&W screen. Made all variables local
 * to this routine (no need for more globals if no one ever uses them). Fixed
 * bug in refresh routine. Use dither from colorx.c. Use xpov.icon instead of
 * theIcon.
 */

#include <stdio.h>
#include <X11/Xlib.h>		/* some Xlib stuff */
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include "xpov.icn"
#include <math.h>

#include "frame.h"
#include "povproto.h"

#define 	BORDER_WIDTH	2
#define	EV_MASK	(ButtonPressMask | \
		 KeyPressMask	 | \
		 ExposureMask    | \
		 StructureNotifyMask)

static Display *theDisplay;
static int      theScreen;
static int      theDepth;
static unsigned long theBlackPixel;
static unsigned long theWhitePixel;
static XEvent   theEvent;
static Window   theWindow, openWindow();
static GC       theGC;
static unsigned char *bitmap;	/* pointer to our picture bitmap */
static unsigned char *bitmapPos;/* position to the last drawn pixel in our
				 * bitmap */


static char    *display_name = 0;
static int     disp_wide, disp_high;
static XImage  *xi;
static char    *data;
static XGCValues gcvalues;
static long     colorpixels[256];
static Bool     iscolor;
static int 	screen_open = 0;

struct {
    void            (*Init) ();
    void            (*Close) ();
    void            (*Finished) ();
    void            (*Plot) ();
}               display;

/* global POVRay variables */

extern FRAME    Frame;
extern unsigned int Options;
extern char     DisplayFormat;
extern int      First_Line;

/*
 * Sets up a connection to the X server and stores informations about the
 * enviroment
 */

static          initX()
{
    theDisplay = XOpenDisplay(NULL);
    if (theDisplay == NULL) {
	fprintf(stderr, "ERROR: Cannot establish a connection to the X server %s\n",
		XDisplayName(NULL));
	exit(1);
    }
    theScreen = DefaultScreen(theDisplay);
    theDepth = DefaultDepth(theDisplay, theScreen);
    theWhitePixel = WhitePixel(theDisplay, theScreen);
    theBlackPixel = BlackPixel(theDisplay, theScreen);
}

/*
 * This procedure will do the following things: 1) 	Set up attributes
 * desired for the window 2)	Set up an icon to our window. 3) 	Send
 * hints to the window manager. 4) 	Open a window on the display 5)	Tell
 * the X to place the window on the screen 6)	Flush out all the queued up X
 * requests to the X server
 */

static Window   openWindow(x, y, width, height, flag, theNewGC)
int             x, y;
int             width, height;
int             flag;
GC             *theNewGC;
{
    XSetWindowAttributes theWindowAttributes;
    XSizeHints      theSizeHints;
    unsigned long   theWindowMask;
    Window          theNewWindow;
    Pixmap          theIconPixmap;
    XWMHints        theWMHints;


    /*
     * Set up some attributes for the window. Override_redirect tells the
     * window manager to deal width the window or leave it alone
     */

    theWindowAttributes.border_pixel = theBlackPixel;
    theWindowAttributes.background_pixel = theWhitePixel;
    theWindowAttributes.override_redirect = False;
    theWindowMask = CWBackPixel | CWBorderPixel | CWOverrideRedirect;

    /* Now, open out window */

    theNewWindow = XCreateWindow(theDisplay,
				 RootWindow(theDisplay, theScreen),
				 x, y,
				 width, height,
				 BORDER_WIDTH,
				 theDepth,
				 InputOutput,
				 CopyFromParent,
				 theWindowMask,
				 &theWindowAttributes);

    /* Create one iconbitmap */

    theIconPixmap = XCreateBitmapFromData(theDisplay,
					  theNewWindow,
					  xpov_bits,
					  xpov_width,
					  xpov_height);

    /*
     * Now tell the window manager where on screen we should place our
     * window.
     */

    theWMHints.icon_pixmap = theIconPixmap;
    theWMHints.initial_state = NormalState;	/* we don't want an iconized
						 * window when it's created */
    theWMHints.flags = IconPixmapHint | StateHint;

    XSetWMHints(theDisplay, theNewWindow, &theWMHints);

    theSizeHints.flags = PPosition | PSize;
    theSizeHints.x = x;
    theSizeHints.y = y;
    theSizeHints.width = width;
    theSizeHints.height = height;
    /*
     * theSizeHints.min_width	= width; theSizeHints.min_height	=
     * height; theSizeHints.max_width	= width; theSizeHints.max_height =
     * height;
     */

    XSetNormalHints(theDisplay, theNewWindow, &theSizeHints);


    if (createGC(theNewWindow, theNewGC) == 0) {
	XDestroyWindow(theDisplay, theNewWindow);
	return ((Window) 0);
    }

    /* make a name for our window */

    XStoreName(theDisplay, theNewWindow, "POVRay \0");

    /*
     * Now, could we please see the window on the screen? Until now, we have
     * dealed with a window which has been created but noe appeared on the
     * screen. Maping the window places it visibly	on the screen
     */

    XMapWindow(theDisplay, theNewWindow);
    XFlush(theDisplay);
    return (theNewWindow);
}

static          refreshWindow(theExposedWindow)
Window          theExposedWindow;
{
    int             i = 0, x = 0, y = First_Line;
    register unsigned char *dummy = bitmap;
    while (dummy != bitmapPos) {
	/* draw until most current pixel is drawn */
	if (*dummy)
	    XDrawPoint(theDisplay, theWindow, theGC, x, y);
	if (x == Frame.Screen_Width) {
	    x = 0;
	    y++;
	}
	else {
	    dummy++;
	    x++;
	    i++;
	}
    }
    XFlush(theDisplay);
}

/* Creates a new graphics context */

static int      createGC(theNewWindow, theNewGC)
Window          theNewWindow;
GC             *theNewGC;
{
    XGCValues       theGCValues;
    *theNewGC = XCreateGC(theDisplay,
			  theNewWindow,
			  (unsigned long) 0,
			  &theGCValues);

    if (*theNewGC == 0) {
	return (0);		/* error */
    }
    else {			/* set foreground and background defaults for
				 * the new GC */
	XSetForeground(theDisplay,
		       *theNewGC,
		       theBlackPixel);

	XSetBackground(theDisplay,
		       *theNewGC,
		       theWhitePixel);

	return (1);		/* OK */
    }
}

static          initEvents(theWindow)
Window          theWindow;
{
    XSelectInput(theDisplay,
		 theWindow,
		 EV_MASK);
}

void            BWFinished()
{

}


void            BWInit()
{
    int             i;

    /*
     * set some room for a bitmap for our picture. I've got to "remember" the
     * whole picture bacuse of resizing of the window, overlapping etc. Then
     * I've got to refresh the picture. This should be easy to convert to an
     * "color version" in future
     */

    bitmap = (unsigned char *) malloc(sizeof(unsigned char) *
				(Frame.Screen_Width * Frame.Screen_Height));
    bitmapPos = bitmap;
    if (bitmap == NULL)
	printf("ERROR: Can not allocate the buffer..\n");

    for (i = 0; i < (Frame.Screen_Width * Frame.Screen_Height); i++) {
	*bitmapPos++ = 0;
    }
    bitmapPos = bitmap;
    initX();
    theWindow = openWindow(0, 0, Frame.Screen_Width, Frame.Screen_Height, 0, &theGC);
    initEvents(theWindow);
    XFlush(theDisplay);
    XNextEvent(theDisplay, &theEvent);
    XFlush(theDisplay);
}				/* end of display initilazation */

void            BWClose()
{
    sleep(10);			/* an simple delay. 10 seconds. */
    XDestroyWindow(theDisplay, theWindow);
    XFlush(theDisplay);
    XCloseDisplay(theDisplay);
    free(bitmap);
}

void            BWPlot(x, y, Red, Green, Blue)
int             x, y;
unsigned char   Red, Green, Blue;
{
    unsigned char   color;
    long            thePixel;
    /* lets find if there are some events waiting for us */

    while (XPending(theDisplay) > 0) {	/* now deal with the events.. */
	XNextEvent(theDisplay, &theEvent);

	switch (theEvent.type) {
	  case Expose:
	    /* printf("Window is exposed.\n"); */
	    refreshWindow(theEvent.xany.window);
	    break;

	  case MapNotify:
	    /* printf("The window is mapped.\n"); */
	    refreshWindow(theEvent.xany.window);
	    break;

	  case ButtonPress:
	    /* printf("A mouse button was pressed.\n"); */
	    break;

	  case ConfigureNotify:
	    /* printf("The window configuration has been changed\n"); */
	    refreshWindow(theEvent.xany.window);
	    break;
	}
    }
    if (dither(x, y, Red, Green, Blue)) {
	thePixel = BlackPixel(theDisplay, theScreen);
	XDrawPoint(theDisplay, theWindow, theGC, x, y);
	XFlush(theDisplay);
    }
    else {
	thePixel = WhitePixel(theDisplay, theScreen);
    }
    *bitmapPos++ = thePixel;
}

/* so we can get at the dimensions */
extern FRAME    Frame;

/* x11 stuff */

static Bool     evtPred(d, e, a)
Display        *d;
XEvent         *e;
char           *a;
{
    return (True);
}

static void     BuildColormap()
{
    XColor          tmp;
    register unsigned short r, g, b, i;
    Colormap        cmap;

    /*
     * build a colormap that will contain 216, or 6^3 colors. We won't do
     * anything clever like adding extra grays in the leftover space.
     * Remember, this was written in a few hours.
     */
    cmap = DefaultColormap(theDisplay, theScreen);

    i = 0;
    for (r = 0; r < 6; r++) {
	for (g = 0; g < 6; g++) {
	    for (b = 0; b < 6; b++, i++) {
		tmp.red = (r * 36) << 8;
		tmp.green = (g * 36) << 8;
		tmp.blue = (b * 36) << 8;
		tmp.flags = DoRed | DoGreen | DoBlue;
		if (!XAllocColor(theDisplay, cmap, &tmp))
		    goto crap_out;
		colorpixels[i] = tmp.pixel;
	    }
	}
    }
crap_out:			/* couldn't get what we wanted.  Boo-hoo. */
    if (i != 216)
	fprintf(stderr, "warning: could only get %d colors.\n", i);
}

void            ColourFinished()
{
    XEvent         *report;
    /* do nothing.  I suppose we could wait... */
    while (1) {
	XNextEvent(theDisplay, &report);
	XPutImage(theDisplay, theWindow, theGC, xi, 0, 0, 0, 0, Frame.Screen_Width,
		  Frame.Screen_Height);
    }
}

void            display_close()
{
	if(screen_open)
   		 (display.Close) ();
}

void            display_plot(x, y, Red, Green, Blue)
int             x, y;
unsigned char   Red, Green, Blue;
{
    (display.Plot) (x, y, Red, Green, Blue);
}

void            display_finished()
{
	if(screen_open)
    		(display.Finished) ();
}

void            ColourInit()
{
    Pixmap          icon;
    XSizeHints      hints;
    long            size, i;
    XSetWindowAttributes attrib;

    if ((theDisplay = XOpenDisplay(display_name)) == NULL) {
	fprintf(stderr, "Can't open X server %s.\n",
		XDisplayName(display_name));
	exit(1);
    }
    theScreen = DefaultScreen(theDisplay);

    /* screen dimensions */
    disp_wide = DisplayWidth(theDisplay, theScreen);
    disp_high = DisplayHeight(theDisplay, theScreen);

    theWindow = XCreateSimpleWindow(theDisplay, RootWindow(theDisplay, theScreen),
			      (disp_wide - Frame.Screen_Width) / 2,
			      (disp_high - Frame.Screen_Height) / 2,
			      Frame.Screen_Width,
			      Frame.Screen_Height, 2 /* border width */ ,
			  BlackPixel(theDisplay, theScreen), WhitePixel(theDisplay, theScreen));

    /* create a graphics context for drawing */
    gcvalues.function = GXcopy;
    theGC = XCreateGC(theDisplay, theWindow, GCFunction, &gcvalues);

    /* make a cute icon */
    icon = XCreateBitmapFromData(theDisplay, theWindow, xpov_bits, xpov_width,
				 xpov_height);

    /* give a few clues about ourself */
    hints.flags = PPosition | PSize | PMinSize;
    hints.x = (disp_wide - Frame.Screen_Width) / 2;
    hints.y = (disp_high - Frame.Screen_Height) / 2;
    hints.width = Frame.Screen_Width;
    hints.height = Frame.Screen_Height;
    hints.min_width = 32;
    hints.min_height = 32;

    XSetStandardProperties(theDisplay, theWindow, "povray", "povray", icon, NULL, 0,
			   &hints);

    /* only want to know about expose events */
    XSelectInput(theDisplay, theWindow, ExposureMask);
    XMapWindow(theDisplay, theWindow);

    BuildColormap();

    size = Frame.Screen_Width * Frame.Screen_Height;
    data = (char *) XtMalloc(size);
    for (i = 0; i < size; i++)
	data[i] = 0;
    xi = XCreateImage(theDisplay, DefaultVisual(theDisplay, theScreen), 8, ZPixmap, 0, data,
	    Frame.Screen_Width, Frame.Screen_Height, 8, Frame.Screen_Width);
};

void            ColourClose()
{
    sleep(10);
    XDestroyImage(xi);
    XtFree(data);
    XDestroyWindow(theDisplay, theWindow);
    XCloseDisplay(theDisplay);
}

 /* static */ int dither(x, y, Red, Green, Blue)
register unsigned int x, y;
unsigned char   Red, Green, Blue;
{
    /*
     * Perform ordered dithering for halftone screen. For details, see
     * Graphics Gems, Ed. Andrew Glassner, p. 176-178 Floyd-Steinberg dither
     * would look better, but we would need more saved state, making the
     * coding a lot uglier.  Since this is just a display hack and not an
     * image processing package, I don't bother.
     */
    static unsigned int dm[] = {
	0, 192, 48, 240, 12, 204, 60, 252,
	128, 64, 176, 112, 140, 76, 188, 124,
	32, 224, 16, 208, 44, 236, 28, 220,
	160, 96, 144, 80, 172, 108, 156, 92,
	8, 200, 56, 248, 4, 196, 52, 244,
	136, 72, 184, 120, 132, 68, 180, 116,
	40, 232, 24, 216, 36, 228, 20, 212,
	168, 104, 152, 88, 164, 100, 148, 84
    };
    register unsigned long luminence =
    (307 * Red + 599 * Green + 118 * Blue) / 1024;
    return (luminence < dm[(x & 7) + ((y & 7) << 3)]);
}

void            ColourPlot(x, y, Red, Green, Blue)
int             x, y;
unsigned char   Red, Green, Blue;
{
    long            thePixel;
    int             r, g, b;
    XEvent          report;
    int             luminence;

    /* scale rgb to be in our 216 color space */
    r = (Red * 216) / (256 * 36);
    g = (Green * 216) / (256 * 36);
    b = (Blue * 216) / (256 * 36);

    /* find the pixel we got */
    thePixel = colorpixels[(r * 36) + (g * 6) + b];
    XPutPixel(xi, x, y, thePixel);
    /* on each pixel, copy a scanline.  Inefficient, but so what?  See above. */
    XPutImage(theDisplay, theWindow, theGC, xi, 0, y, 0, y, Frame.Screen_Width, 1);

    /* poll for exposure events */
    if (XCheckIfEvent(theDisplay, &report, evtPred, "blah")) {
	XPutImage(theDisplay, theWindow, theGC, xi, 0, 0, 0, 0, Frame.Screen_Width, y + 1);
    }
}

void unix_init_POVRAY PARAMS((void))
{
}

int             matherr(x)
struct exception *x;
{
    switch (x->type) {
      case DOMAIN:
      case OVERFLOW:
	x->retval = 1.0e17;
	break;

      case SING:
      case UNDERFLOW:
	x->retval = 0.0;
	break;

      default:
	break;
    }
    return (1);
}

extern void     BWInit(), BWClose(), BWFinished(), BWPlot();

void            display_init()
{
    Pixmap          icon;
    XSizeHints      hints;
    long            size, i;
    XSetWindowAttributes attrib;

    if ((theDisplay = XOpenDisplay(display_name)) == NULL) {
	fprintf(stderr, "Can't open X server %s.\n",
		XDisplayName(display_name));
	exit(1);
    }
    
    screen_open = 1;
    
    theScreen = DefaultScreen(theDisplay);
    iscolor = (XDisplayCells(theDisplay, theScreen) > 2);

    if (iscolor) {
	display.Init = ColourInit;
	display.Close = ColourClose;
	display.Finished = ColourFinished;
	display.Plot = ColourPlot;
    }
    else {
	display.Init = BWInit;
	display.Close = BWClose;
	display.Finished = BWFinished;
	display.Plot = BWPlot;
    }

    XCloseDisplay(theDisplay);
    (display.Init) ();

}
