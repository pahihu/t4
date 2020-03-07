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

hello.btl: hello.cfb 
	$(COLLECT) hello.cfb -o hello.btl $(COLLECTOPT)

hello.cfb: hello.pgm /Users/pahi/d7205/libs/hostio.inc hello.cah 
	$(OCONFIG) hello.pgm -o hello.cfb $(OCONFOPT)

hello.cah: hello.lah hello.tah 
	$(LINK) -f hello.lah -ta -h -o hello.cah $(LINKOPT)

hello.tah: hello.occ /Users/pahi/d7205/libs/hostio.inc \
           /Users/pahi/d7205/libs/hostio.lib /Users/pahi/d7205/libs/hostio.liu \
           /Users/pahi/d7205/libs/convert.lib 
	$(OCCAM) hello -ta -h -o hello.tah $(OCCOPT)

