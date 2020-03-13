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
COPT=   #-g
REALPREC=8


##### IMAKEF CUT #####

whetstone.b8h: whetstone.c8h 
	$(COLLECT) whetstone.c8h -t  -o whetstone.b8h $(COLLECTOPT)

whetstone.c8h: whetstone.lnk whetstone.t8h
	$(LINK) -f whetstone.lnk -t8 -h -o whetstone.c8h $(LINKOPT)

whetstone.t8h: whetstone.c
	$(CC) whetstone.c -DREALPREC=$(REALPREC) -t8 -o whetstone.t8h $(COPT)

clean  : delete
delete :
	-$(DELETE) whetstone.c8h
	-$(DELETE) whetstone.t8h

