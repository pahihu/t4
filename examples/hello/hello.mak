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

hello.cfb: hello.pgm hello.cah 
	$(OCONFIG) hello.pgm -o hello.cfb $(OCONFOPT)

hello.cah: hello.lah hello.tah 
	$(LINK) -f hello.lah -ta -h -o hello.cah $(LINKOPT)

hello.tah: hello.occ
	$(OCCAM) hello -ta -h -o hello.tah $(OCCOPT)

