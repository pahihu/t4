# IBM Makefile for Persistence of Vision Raytracer
# This file is released to the public domain.
#
# Note for the Zortech C++ 286-DOS-Extender IBM Version:
# Uses Zortech's "Z" memory model and ZPM.EXE DOS extender for
# use with 286 and higher Intel/MSDOS systems.
#
# This makefile is specific to Charles Marslett's MK utility, though
# it can be modified easily for other Microsoft NMAKE or Unix MAKE
# compatible "make"s (mostly, the \ as a continuation character, and
# the << convention for building temporary files are not universal).
#
#
# MAKE Macros and Such...
#

CMODEL  =z
LIB     =d:\zt\lib
CC      =ztc -c
OBJ     =obj
MACHINE_OBJ = ibm.$(OBJ)

LINKER  =blinkx /NOE/NOI/MAP/F/PAC/STACK:12288

# To compile for no FPU
CFLAGS  =-m$(CMODEL) -o+all -bx -a2 -R -2

# To compile for a 287/387
#CFLAGS =-m$(CMODEL) -o+all -bx -f -a2 -R -2

.c.obj :
	$(CC) $(CFLAGS) $*.c


PVOBJS = povray.$(OBJ) render.$(OBJ) tokenize.$(OBJ) parse.$(OBJ) \
	  objects.$(OBJ) spheres.$(OBJ) quadrics.$(OBJ) lighting.$(OBJ) \
	  prioq.$(OBJ) texture.$(OBJ) matrices.$(OBJ) csg.$(OBJ) \
	  hfield.$(OBJ) txtcolor.$(OBJ) txtbump.$(OBJ) txtmap.$(OBJ) \
	  txttest.$(OBJ) colour.$(OBJ) viewpnt.$(OBJ) ray.$(OBJ) \
	  point.$(OBJ) boxes.$(OBJ) blob.$(OBJ) \
	  planes.$(OBJ) iff.$(OBJ) gif.$(OBJ) gifdecod.$(OBJ) \
	  triangle.$(OBJ) raw.$(OBJ) dump.$(OBJ) targa.$(OBJ) poly.$(OBJ) \
	  bezier.$(OBJ) vect.$(OBJ) $(MACHINE_OBJ) ai.lib


# POV-Ray - Specific Dependencies
#
povray.exe : $(PVOBJS)
	$(LINKER) @ibmzor2.lnk

povray.$(OBJ) : povray.c povproto.h frame.h vector.h config.h

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

hfield.$(OBJ) : hfield.c povproto.h frame.h vector.h config.h

spheres.$(OBJ) : spheres.c povproto.h frame.h vector.h config.h

planes.$(OBJ) : planes.c povproto.h frame.h vector.h config.h

point.$(OBJ) : point.c povproto.h frame.h vector.h config.h

boxes.$(OBJ) : boxes.c povproto.h frame.h vector.h config.h

blob.$(OBJ) : blob.c povproto.h frame.h vector.h config.h

quadrics.$(OBJ) : quadrics.c povproto.h frame.h vector.h config.h

poly.$(OBJ) : poly.c povproto.h frame.h vector.h config.h

bezier.$(OBJ) : bezier.c povproto.h frame.h vector.h config.h

vect.$(OBJ) : vect.c povproto.h frame.h config.h

matrices.$(OBJ) : matrices.c povproto.h frame.h vector.h config.h

csg.$(OBJ) : csg.c povproto.h frame.h vector.h config.h

colour.$(OBJ) : colour.c povproto.h frame.h config.h

viewpnt.$(OBJ) : viewpnt.c povproto.h frame.h vector.h config.h

ray.$(OBJ) : ray.c povproto.h frame.h vector.h config.h

iff.$(OBJ) : iff.c povproto.h frame.h config.h

gif.$(OBJ) : gif.c povproto.h frame.h config.h

gifdecod.$(OBJ) : gifdecod.c povproto.h frame.h config.h

raw.$(OBJ) : raw.c povproto.h frame.h config.h

dump.$(OBJ) : dump.c povproto.h frame.h config.h

targa.$(OBJ) : targa.c povproto.h frame.h config.h

triangle.$(OBJ) : triangle.c povproto.h frame.h vector.h config.h

ibm.$(OBJ) : ibm.c povproto.h frame.h config.h
