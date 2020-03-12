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

sqrroots.b4h: sqrroots.c4h 
	$(COLLECT) sqrroots.c4h -t  -o sqrroots.b4h $(COLLECTOPT)

sqrroots.c4h: sqrroots.l4h sqrroots.t4h 
	$(LINK) -f sqrroots.l4h -t4 -h -o sqrroots.c4h $(LINKOPT)

sqrroots.t4h: sqrroots.occ /Users/Pahi/d7205/libs/hostio.inc \
              /Users/Pahi/d7205/libs/hostio.lib \
              /Users/Pahi/d7205/libs/hostio.liu \
              /Users/Pahi/d7205/libs/convert.lib 
	$(OCCAM) sqrroots -t4 -h -o sqrroots.t4h $(OCCOPT)

clean  : delete
delete :
	-$(DELETE) sqrroots.c4h
	-$(DELETE) sqrroots.t4h

