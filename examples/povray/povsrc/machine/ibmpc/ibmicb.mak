# Make file for POV-Ray for use with Intel's "Code Builder Kit" compiler
# to produce a 32 bit protected mode version.
# This version will link in the coprocessor emulation module. The EXE
# will use a coprocessor if one is detected however.
#
#
# IMPORTANT: When editing this file do NOT replace the TAB's at the
# begining of the lines with all spaces, it seems to break the Opus 
# "make" utility shipped with Code Builder. 
# (In the HDRS, OBJS & SRCS lists for example.)
#
# B.Pulver --- 6/10/92


CC      = icc         
CFLAGS  =  /d2 /znosignedchar /zfloatsync   
	      
DEST    = .           
		      
EXTHDRS         =
HDRS            = config.h extend.h frame.h povproto.h texture.h tiga.h \
		  tigadefs.h vector.h

LDFLAGS =             
LINKFLAGS =  /e$(PROGRAM) 
# Use /n to generate no copro code with version 1.0x, 1.1 links the
# emulator in by default. Use /F for 1.1 to supress linking of the FP emu.

LDMAP   =           
LIBS    =
	   
OBJS            = bezier.obj blob.obj boxes.obj colour.obj csg.obj dump.obj gif.obj \
		  gifdecod.obj hfield.obj ibm.obj iff.obj lighting.obj \
		  matrices.obj objects.obj parse.obj planes.obj point.obj \
		  poly.obj povray.obj prioq.obj quadrics.obj raw.obj ray.obj \
		  render.obj spheres.obj targa.obj texture.obj tokenize.obj \
		  triangle.obj txtbump.obj txtcolor.obj txtmap.obj \
		  txttest.obj vect.obj viewpnt.obj _get_ebx.obj

PROGRAM = POVRAY

SRCS            = bezier.c blob.c boxes.c colour.c csg.c dump.c gif.c gifdecod.c \
		  hfield.c ibm.c iff.c lighting.c matrices.c objects.c \
		  parse.c planes.c point.c poly.c povray.c prioq.c \
		  quadrics.c raw.c ray.c render.c spheres.c targa.c \
		  texture.c tokenize.c triangle.c txtbump.c txtcolor.c \
		  txtmap.c txttest.c vect.c viewpnt.c


CB_LINK_RESP=$(PROGRAM,B,S/.*/&.LRF)

$(PROGRAM) : $(OBJS)
	echo $(OBJS)       > $(CB_LINK_RESP)
	echo $(LINKFLAGS) >> $(CB_LINK_RESP)
	$(CC) @$(CB_LINK_RESP)
							   



### MKMF:  Do not remove this line!  Automatic dependencies follow.

bezier.obj: config.h frame.h povproto.h vector.h

blob.obj: config.h frame.h povproto.h vector.h

boxes.obj: config.h frame.h povproto.h vector.h

colour.obj: config.h frame.h povproto.h vector.h

csg.obj: config.h frame.h povproto.h vector.h

dump.obj: config.h frame.h povproto.h

gif.obj: config.h frame.h povproto.h

gifdecod.obj: config.h frame.h povproto.h

hfield.obj: config.h frame.h povproto.h vector.h

ibm.obj: config.h extend.h frame.h povproto.h tiga.h tigadefs.h

iff.obj: config.h frame.h povproto.h

lighting.obj: config.h frame.h povproto.h vector.h

matrices.obj: config.h frame.h povproto.h vector.h

objects.obj: config.h frame.h povproto.h vector.h

parse.obj: config.h frame.h povproto.h vector.h

planes.obj: config.h frame.h povproto.h vector.h

point.obj: config.h frame.h povproto.h vector.h

poly.obj: config.h frame.h povproto.h vector.h

povray.obj: config.h frame.h povproto.h 

prioq.obj: config.h frame.h povproto.h

quadrics.obj: config.h frame.h povproto.h vector.h

raw.obj: config.h frame.h povproto.h

ray.obj: config.h frame.h povproto.h vector.h

render.obj: config.h frame.h povproto.h vector.h

spheres.obj: config.h frame.h povproto.h vector.h

targa.obj: config.h frame.h povproto.h

texture.obj: config.h frame.h povproto.h texture.h vector.h

tokenize.obj: config.h frame.h povproto.h

triangle.obj: config.h frame.h povproto.h vector.h

txtbump.obj: config.h frame.h povproto.h texture.h vector.h

txtcolor.obj: config.h frame.h povproto.h texture.h vector.h

txtmap.obj: config.h frame.h povproto.h texture.h vector.h

txttest.obj: config.h frame.h povproto.h texture.h vector.h

vect.obj: config.h frame.h povproto.h vector.h

viewpnt.obj: config.h frame.h povproto.h vector.h
