LIBRARIAN=ilibr
OCCAM=oc
LINK=ilink
CONFIG=icconf
OCONFIG=occonf
COLLECT=icollect
CC=icc
DELETE=rm -f
LIBOPT=
OCCOPT=-A -U -NWP -NWU
LINKOPT=
CONFOPT=
OCONFOPT=
COLLECTOPT=
COPT=


##### IMAKEF CUT #####

raymainT8.b8h: raymainT8.c8h 
	$(COLLECT) raymainT8.c8h -t  -o raymainT8.b8h $(COLLECTOPT)

raymainT8.c8h: raymainT8.l8h raymainT8.t8h 
	$(LINK) -f raymainT8.l8h -t8 -h -o raymainT8.c8h $(LINKOPT)

raymainT8.t8h: raymainT8.occ /Users/Pahi/d7205/libs/hostio.inc \
               /Users/Pahi/d7205/libs/hostio.lib \
               /Users/Pahi/d7205/libs/hostio.liu \
               /Users/Pahi/d7205/libs/convert.lib framebufT8.t8h cntlsysT8.t8h \
               raytraceT8.t8h 
	$(OCCAM) raymainT8 -t8 -h -o raymainT8.t8h $(OCCOPT)

framebufT8.t8h: framebufT8.occ /Users/Pahi/d7205/libs/hostio.inc \
                /Users/Pahi/d7205/libs/streamio.inc \
                /Users/Pahi/d7205/libs/hostio.lib \
                /Users/Pahi/d7205/libs/hostio.liu \
                /Users/Pahi/d7205/libs/convert.lib \
                /Users/Pahi/d7205/libs/streamio.lib \
                /Users/Pahi/d7205/libs/streamio.liu \
                /Users/Pahi/d7205/libs/occam8.lib 
	$(OCCAM) framebufT8 -t8 -h -o framebufT8.t8h $(OCCOPT)

cntlsysT8.t8h: cntlsysT8.occ /Users/Pahi/d7205/libs/occam8.lib 
	$(OCCAM) cntlsysT8 -t8 -h -o cntlsysT8.t8h $(OCCOPT)

raytraceT8.t8h: raytraceT8.occ /Users/Pahi/d7205/libs/occam8.lib 
	$(OCCAM) raytraceT8 -t8 -h -o raytraceT8.t8h $(OCCOPT)

clean  : delete
delete :
	-$(DELETE) raymainT8.c8h
	-$(DELETE) raymainT8.t8h
	-$(DELETE) framebufT8.t8h
	-$(DELETE) cntlsysT8.t8h
	-$(DELETE) raytraceT8.t8h
	-$(DELETE) raymainT8.cfb

