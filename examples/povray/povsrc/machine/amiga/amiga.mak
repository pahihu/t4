# Makefile for Persistence of Vision Raytracer 
# This file is released to the public domain.
#
#
# MAKE Macros and Such...
#

#***************************************************************
#*
#*                      Amiga Options
#*
#***************************************************************

# Uncomment for Amiga Lattice C for IEEE w/o coprocessor
#CFLAGS	= -cusrft -fi -m0 -b0 -O -v

# Uncomment for Amiga Lattice C with 68881
CFLAGS	= -cusrft -f8 -m2 -b0 -O -v -dM68881

# Flags for debugging
#CFLAGS	= -cusrft -q5w5e -d5

LIBSIEEE	= lib:lcmieee.lib lib:lcnb.lib lib:amiga.lib
LIBS881	= lib:lcm881.lib lib:lcnb.lib lib:amiga.lib

OBJ	= o
MACHINE_OBJ	= amiga.$(OBJ)

POVOBJS = povray.$(OBJ) render.$(OBJ) tokenize.$(OBJ) parse.$(OBJ) \
	  objects.$(OBJ) spheres.$(OBJ) quadrics.$(OBJ) lighting.$(OBJ) \
	  prioq.$(OBJ) texture.$(OBJ) matrices.$(OBJ) csg.$(OBJ)  hfield.$(OBJ)\
	  txtcolor.$(OBJ) txtbump.$(OBJ) txtmap.$(OBJ) txttest.$(OBJ) \
	  colour.$(OBJ) viewpnt.$(OBJ) ray.$(OBJ) planes.$(OBJ) iff.$(OBJ) \
	  gif.$(OBJ) gifdecod.$(OBJ) triangle.$(OBJ) raw.$(OBJ) dump.$(OBJ) \
	  targa.$(OBJ) poly.$(OBJ) bezier.$(OBJ) vect.$(OBJ) \
          blob.$(OBJ) boxes.$(OBJ) point.$(OBJ) $(MACHINE_OBJ)


#  Amiga Linkage...
#
 povray881: $(POVOBJS)
	blink <with <
	from lib:c.o $(POVOBJS) LIB $(LIBS881) to povray881
	<
                                                          


 povrayieee: $(POVOBJS)
	blink <with <
	from lib:c.o $(POVOBJS) LIB $(LIBSIEEE) to povrayieee
	<
