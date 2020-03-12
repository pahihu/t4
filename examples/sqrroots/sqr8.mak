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

sqrroots.b8h: sqrroots.c8h 
	$(COLLECT) sqrroots.c8h -t  -o sqrroots.b8h $(COLLECTOPT)

sqrroots.c8h: sqrroots.l8h sqrroots.t8h 
	$(LINK) -f sqrroots.l8h -t8 -h -o sqrroots.c8h $(LINKOPT)

sqrroots.t8h: sqrroots.occ /Users/Pahi/d7205/libs/hostio.inc \
              /Users/Pahi/d7205/libs/hostio.lib \
              /Users/Pahi/d7205/libs/hostio.liu \
              /Users/Pahi/d7205/libs/convert.lib 
	$(OCCAM) sqrroots -t8 -h -o sqrroots.t8h $(OCCOPT)

clean  : delete
delete :
	-$(DELETE) sqrroots.c8h
	-$(DELETE) sqrroots.t8h

