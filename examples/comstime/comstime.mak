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

comstime.btl: comstime.cfb 
	$(COLLECT) comstime.cfb -o comstime.btl $(COLLECTOPT)

comstime.cfb: comstime.pgm /Users/Pahi/d7205/libs/hostio.inc comstime.cah 
	$(OCONFIG) comstime.pgm -o comstime.cfb $(OCONFOPT)

comstime.cah: comstime.lah comstime.tah 
	$(LINK) -f comstime.lah -ta -h -o comstime.cah $(LINKOPT)

comstime.tah: comstime.occ /Users/Pahi/d7205/libs/hostio.inc \
              /Users/Pahi/d7205/libs/hostio.lib \
              /Users/Pahi/d7205/libs/hostio.liu \
              /Users/Pahi/d7205/libs/convert.lib 
	$(OCCAM) comstime -ta -h -o comstime.tah $(OCCOPT)

clean  : delete
delete :
	-$(DELETE) comstime.clu
	-$(DELETE) comstime.cfb
	-$(DELETE) comstime.cah
	-$(DELETE) comstime.tah

