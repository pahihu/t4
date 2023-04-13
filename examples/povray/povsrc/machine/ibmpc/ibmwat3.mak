# IBM Makefile for Persistence of Vision Raytracer
# This file is released to the public domain.
# For use with the Watcom C protected mode compiler on the IBM-PC
# Note for the IBM Version:
# Uses system environment variable LIB for the linker's .LIB file path.
# (Example:  Set LIB=C:\LIB)  if you change this the two .LNK files will also
# need to be changed as well.  The system environment variable CMODEL also
# may be defined for the memory model of the compiler.  POV-Ray requires
# the "Large" memory model.  (Example Set CMODEL=l)  If you don't want to
# use the DOS environment variables, uncomment the following two lines:
#
#
# MAKE Macros and Such...
#

CMODEL	=f
#LIB	=c:\lib
CC	=wcc386p
OBJ	=obj
MACHINE_OBJ	= ibm.$(OBJ)

LINKER	=wlink

# Uncomment for 8086/8088 instruction set usage
#
#CFLAGS	=-m$(CMODEL) -fpi -d2 -s -zc

# Uncomment for 80186/80268 (incl. V20!) instruction set usage	[/Gmsr2]
#
CFLAGS	=-m$(CMODEL) -7 -d2 -s -zc

# If you have OS/2 to compile under, fix directory name and add this option
# to CFLAGS above...
#
# /B2 C:\msc\c2l.exe
#

# MS's NMAKE implicit rules for making an .OBJ file from a .C file...
#
# The second command converts debugging info from Watcom's to Turbo Debugger's
#
.c.obj :
	$(CC) $(CFLAGS) -oxat $*.c
#	w2td $*.obj

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
#

OB1 = povray.$(OBJ) render.$(OBJ) tokenize.$(OBJ) parse.$(OBJ)
OB2 = objects.$(OBJ) spheres.$(OBJ) quadrics.$(OBJ) lighting.$(OBJ)
OB3 = texture.$(OBJ) txtcolor.$(OBJ) txtbump.$(OBJ) txtmap.$(OBJ)
OB4 = txttest.$(OBJ) prioq.$(OBJ) matrices.$(OBJ) csg.$(OBJ) hfield.$(OBJ)
OB5 = colour.$(OBJ) viewpnt.$(OBJ) ray.$(OBJ) planes.$(OBJ)
OB6 = point.$(OBJ) boxes.$(OBJ) blob.$(OBJ) iff.$(OBJ)
OB7 = gif.$(OBJ) gifdecod.$(OBJ) triangle.$(OBJ) raw.$(OBJ) dump.$(OBJ)
OB8 = targa.$(OBJ) poly.$(OBJ) bezier.$(OBJ) vect.$(OBJ) $(MACHINE_OBJ)

POVOBJS = $(OB1) $(OB2) $(OB3) $(OB4) $(OB5) $(OB6) $(OB7) $(OB8)

# POV-Ray Specific Dependencies
#
povray.exe : $(POVOBJS)
	$(LINKER) @ibmwat3.lnk

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

point.$(OBJ) : point.c povproto.h frame.h vector.h config.h

boxes.$(OBJ) : boxes.c povproto.h frame.h vector.h config.h

blob.$(OBJ) : blob.c povproto.h frame.h vector.h config.h

quadrics.$(OBJ) : quadrics.c povproto.h frame.h vector.h config.h

hfield.$(OBJ) : hfield.c povproto.h frame.h vector.h config.h

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
