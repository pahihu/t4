# Makefile for Persistence of Vision Raytracer 
# This file is released to the public domain.
# For use with GCC on the IBM-PC
#
# MAKE Macros and Such...
#


#***************************************************************
#*
#*                      IBM Options
#*
#***************************************************************

# Note for the IBM Version:
# Uses system environment variable LIB for the linker's .LIB file path.
# (Example:  Set LIB=C:\LIB)  if you change this the two .LNK files will also
# need to be changed as well.  The system environment variable CMODEL also
# may be defined for the memory model of the compiler.  PV-Ray requires
# the "Large" memory model.  (Example Set CMODEL=l)  If you don't want to
# use the DOS environment variables, uncomment the following two lines:

CMODEL  =
LIB     =

CC      =gcc
OBJ     =o
MACHINE_OBJ     = ibm.$(OBJ)    # gcc.$(OBJ) if your GCC does not support INTs

LINK    =gcc
#LINK   =link /EXEPACK /PACKCODE /FARCALL

LINKFLAGS=

# Uncomment for full optimization
#
CFLAGS  =-O -fomit-frame-pointer -fstrength-reduce -fcombine-regs -finline-functions -Wall -DGCCDOS

# Uncomment for debugging (no optimization)
#CFLAGS =-Wall -DGCC


# Turbo-MAKE's implicit rules for making a .OBJ file from a .C file...
#
.c.$(OBJ) :
	$(CC) $(CFLAGS) -c $<

# Generic "Unix" MAKE's implicit rules for making an .OBJ file from a .C file
#
#*.obj : *.c
# $(CC) $(CFLAGS) $*


#***************************************************************
#*
#*                          Common  Stuff
#*
#***************************************************************

F1 = povray.$(OBJ) colour.$(OBJ) csg.$(OBJ)
F2 = hfield.$(OBJ) lighting.$(OBJ) blob.$(OBJ)
F3 = matrices.$(OBJ) objects.$(OBJ) planes.$(OBJ) poly.$(OBJ) prioq.$(OBJ)
F4 = quadrics.$(OBJ) ray.$(OBJ) render.$(OBJ) spheres.$(OBJ) point.$(OBJ)
F5 = texture.$(OBJ) triangle.$(OBJ) txtbump.$(OBJ) txtcolor.$(OBJ)
F6 = txtmap.$(OBJ) txttest.$(OBJ) vect.$(OBJ) viewpnt.$(OBJ) $(MACHINE_OBJ)
F7 = bezier.$(OBJ) dump.$(OBJ) raw.$(OBJ) targa.$(OBJ) boxes.$(OBJ)
F8 = iff.$(OBJ) gif.$(OBJ) gifdecod.$(OBJ) tokenize.$(OBJ) parse.$(OBJ)

PVOBJS = $(F1) $(F2) $(F3) $(F4) $(F5) $(F6) $(F7) $(F8)

#
# IBM Linkage...
#
povray.exe : $(PVOBJS)
	$(LINK) @ibmgcc.lnk
#       gccbind a.out povray

#
# Specific module/header dependencies for PV-Ray:
#

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

spheres.$(OBJ) : spheres.c povproto.h frame.h vector.h config.h

planes.$(OBJ) : planes.c povproto.h frame.h vector.h config.h

quadrics.$(OBJ) : quadrics.c povproto.h frame.h vector.h config.h

hfield.$(OBJ) : hfield.c povproto.h frame.h vector.h config.h

poly.$(OBJ) : poly.c povproto.h frame.h vector.h config.h

bezier.$(OBJ) : bezier.c povproto.h frame.h vector.h config.h

vect.$(OBJ) : vect.c povproto.h frame.h config.h

boxes.$(OBJ) : boxes.c povproto.h frame.h config.h

blob.$(OBJ) : blob.c povproto.h frame.h config.h

matrices.$(OBJ) : matrices.c povproto.h frame.h vector.h config.h

csg.$(OBJ) : csg.c povproto.h frame.h vector.h config.h

colour.$(OBJ) : colour.c povproto.h frame.h config.h

viewpnt.$(OBJ) : viewpnt.c povproto.h frame.h vector.h config.h

ray.$(OBJ) : ray.c povproto.h frame.h vector.h config.h

iff.$(OBJ) : iff.c povproto.h frame.h config.h

gif.$(OBJ) : gif.c povproto.h frame.h config.h

gifdecod.$(OBJ) : gifdecod.c povproto.h frame.h config.h

raw.$(OBJ) :    raw.c povproto.h frame.h config.h

triangle.$(OBJ) : triangle.c povproto.h frame.h vector.h config.h

ibm.$(OBJ) :    ibm.c povproto.h frame.h config.h
	$(CC) -Wall -DGCCDOS -c ibm.c

ibmclean.$(OBJ) : ibmclean.c povproto.h frame.h config.h
