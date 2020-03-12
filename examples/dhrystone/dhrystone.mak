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

dhrystone.b8h: dhrystone.c8h 
	$(COLLECT) dhrystone.c8h -t  -o dhrystone.b8h $(COLLECTOPT)

dhrystone.c8h: dhrystone.lnk dhrystone.t8h \
               /Users/pahi/ictools/libs/startup.lnk
	$(LINK) -f dhrystone.lnk -t8 -h -o dhrystone.c8h $(LINKOPT)

dhrystone.t8h: dhrystone.c /Users/pahi/ictools/libs/stdlib.h \
               /Users/pahi/ictools/libs/stdio.h \
               /Users/pahi/ictools/libs/subsem.h \
               /Users/pahi/ictools/libs/subchan.h \
               /Users/pahi/ictools/libs/limits.h 
	$(CC) dhrystone.c -g -t8 -o dhrystone.t8h $(COPT)

clean  : delete
delete :
	-$(DELETE) dhrystone.c8h
	-$(DELETE) dhrystone.t8h

