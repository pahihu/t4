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
LINKOPT=
CONFOPT=
OCONFOPT=
COLLECTOPT=
COPT=


##### IMAKEF CUT #####

whetston.btl: whetston.cfb 
	$(COLLECT) whetston.cfb -o whetston.btl $(COLLECTOPT)

whetston.cfb: whetston.pgm whetston.c8h 
	$(OCONFIG) whetston.pgm -o whetston.cfb $(OCONFOPT)

whetston.c8h: whetston.l8h whetston.t8h 
	$(LINK) -t8 -h -f whetston.l8h -o whetston.c8h $(LINKOPT)

whet.t8h: whet.occ
	$(OCCAM) whet -t8 -o whet.t8h

whetston.t8h: whetston.occ whet.t8h 
	$(OCCAM) whetston -t8 -h -o whetston.t8h $(OCCOPT)

