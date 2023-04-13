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
OBJ	= o
MACHINE_OBJ	= unix.$(OBJ)
CFLAGS=		-c -O
LFLAGS =	-o povray -O

# Make's implicit rules for making a .o file from a .c file...
#
.c.o :
	$(CC) $(CFLAGS) $*.c


POVOBJS = povray.$(OBJ) render.$(OBJ) tokenize.$(OBJ) parse.$(OBJ) \
	  objects.$(OBJ) spheres.$(OBJ) quadrics.$(OBJ) lighting.$(OBJ) \
	  prioq.$(OBJ) texture.$(OBJ) matrices.$(OBJ) csg.$(OBJ) hfield.$(OBJ)\
	  txtcolor.$(OBJ) txtbump.$(OBJ) txtmap.$(OBJ) txttest.$(OBJ) \
	  colour.$(OBJ) viewpnt.$(OBJ) ray.$(OBJ) planes.$(OBJ) iff.$(OBJ) \
	  gif.$(OBJ) gifdecod.$(OBJ) triangle.$(OBJ) raw.$(OBJ) dump.$(OBJ) \
	  targa.$(OBJ) poly.$(OBJ) bezier.$(OBJ) vect.$(OBJ) \
          blob.$(OBJ) boxes.$(OBJ) point.$(OBJ) $(MACHINE_OBJ)


povray:	$(POVOBJS)
	cc $(LFLAGS) $(POVOBJS) -lm
