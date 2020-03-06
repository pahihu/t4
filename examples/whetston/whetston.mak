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

whetston.cfb: whetston.pgm /Users/pahi/d7205/libs/hostio.inc whetston.cah 
	$(OCONFIG) whetston.pgm -o whetston.cfb $(OCONFOPT)

whetston.cah: whetston.lah whetston.tah 
	$(LINK) -f whetston.lah -ta -h -o whetston.cah $(LINKOPT)

whet.tau: whet.occ
	$(OCCAM) whet -ta -o whet.tau

whetston.tah: whetston.occ /Users/pahi/d7205/libs/hostio.inc \
               /Users/pahi/d7205/libs/hostio.lib \
               /Users/pahi/d7205/libs/hostio.liu \
               /Users/pahi/d7205/libs/convert.lib whet.tau 
	$(OCCAM) whetston -ta -h -o whetston.tah $(OCCOPT)

