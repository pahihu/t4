# Makefile for Persistence of Vision Raytracer 
# This file is released to the public domain.
#
#
# MAKE Macros and Such...
#

#***************************************************************
#*
#*                      UNIX Makefile
#*
#***************************************************************

# The exact options may depend on your compiler.  Feel free to modify
# these as required.

# Added for colorx addition.  You _do_ have $X11 defined by your .cshrc,
# don't you?
X11 = /usr
CFLAGS=		-c -O 
LFLAGS =	-O  $(X11)/lib/libXt.a $(X11)/lib/libXext.a -lX11 \
                -o povray
CC =		cc

OBJ	= o
MACHINE_OBJ	= xwindows.$(OBJ)

# Make's implicit rules for making a .o file from a .c file...
#
.c.o :
	$(CC) $(CFLAGS) $*.c


POVOBJS = povray.$(OBJ) render.$(OBJ) tokenize.$(OBJ) parse.$(OBJ) \
	  objects.$(OBJ) spheres.$(OBJ) quadrics.$(OBJ) lighting.$(OBJ) \
	  prioq.$(OBJ) texture.$(OBJ) matrices.$(OBJ) csg.$(OBJ) \
	  hfield.$(OBJ) txtcolor.$(OBJ) txtbump.$(OBJ) txtmap.$(OBJ) \
	  txttest.$(OBJ) colour.$(OBJ) viewpnt.$(OBJ) ray.$(OBJ) point.$(OBJ)\
	  planes.$(OBJ) iff.$(OBJ) gif.$(OBJ) gifdecod.$(OBJ) blob.$(OBJ)\
	  triangle.$(OBJ) raw.$(OBJ) dump.$(OBJ) targa.$(OBJ) poly.$(OBJ) \
	  bezier.$(OBJ) vect.$(OBJ) boxes.$(OBJ) $(MACHINE_OBJ)


povray:	$(POVOBJS)
	cc $(POVOBJS) -lm $(LFLAGS)

povray.$(OBJ) : povray.c povproto.h frame.h vector.h config.h

xwindows.$(OBJ): xwindows.c xpov.icn frame.h povproto.h config.h
	$(CC) $(CFLAGS) -I$(X11)/include xwindows.c

tokenize.$(OBJ) : tokenize.c povproto.h frame.h config.h

parse.$(OBJ) : parse.c povproto.h frame.h config.h

render.$(OBJ) : render.c povproto.h frame.h vector.h config.h

lighting.$(OBJ) : lighting.c povproto.h frame.h vector.h config.h

prioq.$(OBJ) : prioq.c povproto.h frame.h config.h

texture.$(OBJ) : texture.c povproto.h frame.h vector.h config.h texture.h

txtcolor.$(OBJ) : txtcolor.c povproto.h frame.h vector.h config.h texture.h

txtbump.$(OBJ) : txtbump.c povproto.h frame.h vector.h config.h texture.h

txtmap.$(OBJ) : txtmap.c povproto.h frame.h vector.h config.h texture.h

txttest.$(OBJ) : txttest.c povproto.h frame.h vector.h config.h texture.h

objects.$(OBJ) : objects.c povproto.h frame.h vector.h config.h

spheres.$(OBJ) : spheres.c povproto.h frame.h vector.h config.h

planes.$(OBJ) : planes.c povproto.h frame.h vector.h config.h

poly.$(OBJ) : poly.c povproto.h frame.h vector.h config.h

hfield.$(OBJ) : hfield.c povproto.h frame.h vector.h config.h

bezier.$(OBJ) : bezier.c povproto.h frame.h vector.h config.h

vect.$(OBJ) : vect.c povproto.h frame.h config.h

quadrics.$(OBJ) : quadrics.c povproto.h frame.h vector.h config.h

matrices.$(OBJ) : matrices.c povproto.h frame.h vector.h config.h

csg.$(OBJ) : csg.c povproto.h frame.h vector.h config.h

colour.$(OBJ) : colour.c povproto.h frame.h config.h

viewpnt.$(OBJ) : viewpnt.c povproto.h frame.h vector.h config.h

ray.$(OBJ) : ray.c povproto.h frame.h vector.h config.h

iff.$(OBJ) : iff.c povproto.h frame.h config.h

gif.$(OBJ) : gif.c povproto.h frame.h config.h

gifdecod.$(OBJ) : gifdecod.c povproto.h frame.h config.h

raw.$(OBJ) :	raw.c povproto.h frame.h config.h

triangle.$(OBJ) : triangle.c povproto.h frame.h vector.h config.h

dump.$(OBJ) :	dump.c povproto.h frame.h config.h

boxes.$(OBJ) : boxes.c povproto.h frame.h vector.h config.h

blob.$(OBJ) : blob.c povproto.h frame.h vector.h config.h
