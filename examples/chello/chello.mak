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


##### IMAKEF CUT #####

chello.b4h: chello.c4h 
	time $(COLLECT) chello.c4h -t  -o chello.b4h $(COLLECTOPT)

chello.c4h: chello.lnk chello.t4h
	time $(LINK) -f chello.lnk -t4 -h -o chello.c4h $(LINKOPT)

chello.t4h: chello.c
	time $(CC) chello.c -t4 -o chello.t4h $(COPT)

clean  : delete
delete :
	-$(DELETE) chello.c4h
	-$(DELETE) chello.t4h

