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

chello.b4h: chello.c4h 
	$(COLLECT) chello.c4h -t  -o chello.b4h $(COLLECTOPT)

chello.c4h: chello.lnk chello.t4h
	$(LINK) -f chello.lnk -t4 -h -o chello.c4h $(LINKOPT)

chello.t4h: chello.c
	$(CC) chello.c -g -t4 -o chello.t4h $(COPT)

clean  : delete
delete :
	-$(DELETE) chello.c4h
	-$(DELETE) chello.t4h

