# IBM Makefile for Persistence of Vision Raytracer 
# This file is released to the public domain.
# For use with the ZORTECH C/C++ protected mode compiler
# Note for the IBM Version:
# Uses system environment variable LIB for the linker's .LIB file path.
# (Example:  Set LIB=C:\LIB)  if you change this the two .LNK files will also
# need to be changed as well.  The system environment variable CMODEL also
# may be defined for the memory model of the compiler.  PV-Ray requires
# the "Large" memory model.  (Example Set CMODEL=l)  If you don't want to
# use the DOS environment variables, uncomment the following two lines:
#
#
# MAKE Macros and Such...
#

CMODEL  =x
LIB     =d:\zt\lib
CC      =ztc -c
OBJ     =obj
MACHINE_OBJ = ibm.$(OBJ)

LINKER  =blinkx /CO/CH/NOE/MAP

# Uncomment for 80268 (ZPM DOS extender) instruction set usage
# Normally, you'll use this:
#
CFLAGS  =-m$(CMODEL) -o+all -bx -f -a2


.c.obj :
	$(CC) $(CFLAGS) $*.c


# The option:
#
# /Oaxz
#
# Is purported by Microsoft to produce the fastest possible code.  In fact it
# will break the RGB->HSV->RGB routines in IBM.C, if using the 8087 emulator.
# So, for IBM.C we are using:
#
# /Ogiltaz
#
# This optimization string works for IBM.C and should then have worked for all
# files, but, somehow, it BREAKS the compiler! (Internal compiler error C1001
# on TEXTURE.C!!)  Oh, well...
# Try using /Od to disable optimization if you're having problems.
#

O1 = povray.$(OBJ) render.$(OBJ) tokenize.$(OBJ) parse.$(OBJ)
O2 = objects.$(OBJ) spheres.$(OBJ) quadrics.$(OBJ) lighting.$(OBJ)
O3 = prioq.$(OBJ) texture.$(OBJ) matrices.$(OBJ) csg.$(OBJ)
O4 = hfield.$(OBJ) txtcolor.$(OBJ) txtbump.$(OBJ) txtmap.$(OBJ)
O5 = txttest.$(OBJ) colour.$(OBJ) viewpnt.$(OBJ) ray.$(OBJ)
O6 = planes.$(OBJ) iff.$(OBJ) gif.$(OBJ) gifdecod.$(OBJ)
O7 = boxes.$(OBJ) point.$(OBJ) blob.$(OBJ)
O8 = triangle.$(OBJ) raw.$(OBJ) dump.$(OBJ) targa.$(OBJ) poly.$(OBJ)
O9 = bezier.$(OBJ) vect.$(OBJ) $(MACHINE_OBJ) ztc386.$(OBJ)

PVOBJS = $(O1) $(O2) $(O3) $(O4) $(O5) $(O6) $(O7) $(O8) $(O9)

# POV-Ray - Specific Dependencies

povray.exe : $(PVOBJS)
	$(LINKER)  @ibmzor3.lnk

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

quadrics.$(OBJ) : quadrics.c povproto.h frame.h vector.h config.h

poly.$(OBJ) : poly.c povproto.h frame.h vector.h config.h

bezier.$(OBJ) : bezier.c povproto.h frame.h vector.h config.h

vect.$(OBJ) : vect.c povproto.h frame.h config.h

blob.$(OBJ) : blob.c povproto.h frame.h config.h

boxes.$(OBJ) : boxes.c povproto.h frame.h config.h

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

ztc386.$(OBJ) : ztc386.asm
	masm -ml ztc386;
