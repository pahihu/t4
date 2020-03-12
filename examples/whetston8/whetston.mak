LIBRARIAN=ilibr
OCCAM=oc
LINK=ilink
CONFIG=icconf
OCONFIG=occonf
COLLECT=icollect
CC=icc
DELETE=rm
LIBOPT=
OCCOPT=
LINKOPT=-i
CONFOPT=
OCONFOPT=-i
COLLECTOPT=
COPT=


##### IMAKEF CUT #####

whetston.btl: whetston.cfb 
	$(COLLECT) whetston.cfb -o whetston.btl $(COLLECTOPT)

whetston.cfb: whetston.pgm /Users/pahi/d7205/libs/hostio.inc whetston.c8h 
	$(OCONFIG) whetston.pgm -o whetston.cfb $(OCONFOPT)

whetston.c8h: whetston.l8h whetston.t8h 
	$(LINK) -t8 -h -f whetston.l8h -o whetston.c8h $(LINKOPT)

whet.t8h: whet.occ
	$(OCCAM) whet -t8 -o whet.t8h

whetston.t8h: whetston.occ /Users/pahi/d7205/libs/hostio.inc \
               /Users/pahi/d7205/libs/hostio.lib \
               /Users/pahi/d7205/libs/hostio.liu \
               /Users/pahi/d7205/libs/convert.lib whet.t8h 
	$(OCCAM) whetston -t8 -h -o whetston.t8h $(OCCOPT)

