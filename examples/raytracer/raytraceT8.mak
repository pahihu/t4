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

raytraceT8.btl: raytraceT8.cfb 
	$(COLLECT) raytraceT8.cfb -o raytraceT8.btl -p raytraceT8.map $(COLLECTOPT)

raytraceT8.cfb: raytraceT8.pgm /Users/Pahi/d7205/libs/hostio.inc \
                framebufT8.c8h cntlsysT8.c8h raytraceT8.c8h 
	$(OCONFIG) raytraceT8.pgm -o raytraceT8.cfb $(OCONFOPT)

framebufT8.c8h: framebufT8.l8h framebufT8.t8h 
	$(LINK) -f framebufT8.l8h -t8 -h -o framebufT8.c8h $(LINKOPT)

framebufT8.t8h: framebufT8.occ /Users/Pahi/d7205/libs/hostio.inc \
                /Users/Pahi/d7205/libs/streamio.inc \
                /Users/Pahi/d7205/libs/hostio.lib \
                /Users/Pahi/d7205/libs/hostio.liu \
                /Users/Pahi/d7205/libs/convert.lib \
                /Users/Pahi/d7205/libs/streamio.lib \
                /Users/Pahi/d7205/libs/streamio.liu \
                /Users/Pahi/d7205/libs/occam8.lib 
	$(OCCAM) framebufT8 -t8 -h -o framebufT8.t8h $(OCCOPT)

cntlsysT8.c8h: cntlsysT8.l8h cntlsysT8.t8h 
	$(LINK) -f cntlsysT8.l8h -t8 -h -o cntlsysT8.c8h $(LINKOPT)

cntlsysT8.t8h: cntlsysT8.occ /Users/Pahi/d7205/libs/occam8.lib 
	$(OCCAM) cntlsysT8 -t8 -h -o cntlsysT8.t8h $(OCCOPT)

raytraceT8.c8h: raytraceT8.l8h raytraceT8.t8h 
	$(LINK) -f raytraceT8.l8h -t8 -h -o raytraceT8.c8h $(LINKOPT)

raytraceT8.t8h: raytraceT8.occ /Users/Pahi/d7205/libs/occam8.lib 
	$(OCCAM) raytraceT8 -t8 -h -o raytraceT8.t8h $(OCCOPT)

clean  : delete
delete :
	-$(DELETE) raytraceT8.clu
	-$(DELETE) raytraceT8.cfb
	-$(DELETE) framebufT8.c8h
	-$(DELETE) framebufT8.t8h
	-$(DELETE) cntlsysT8.c8h
	-$(DELETE) cntlsysT8.t8h
	-$(DELETE) raytraceT8.c8h
	-$(DELETE) raytraceT8.t8h
	-$(DELETE) raytraceT8.map

