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

whetstone.b8h: whetstone.c8h 
	$(COLLECT) whetstone.c8h -t  -o whetstone.b8h $(COLLECTOPT)

whetstone.c8h: whetstone.lnk whetstone.t8h \
               /Users/pahi/ictools/libs/startup.lnk \
               /Users/pahi/ictools/libs/centry.lib \
               /Users/pahi/ictools/libs/libc.lib 
	$(LINK) -f whetstone.lnk -t8 -h -o whetstone.c8h $(LINKOPT)

whetstone.t8h: whetstone.c /Users/pahi/ictools/libs/stdlib.h \
               /Users/pahi/ictools/libs/stdio.h \
               /Users/pahi/ictools/libs/subsem.h \
               /Users/pahi/ictools/libs/subchan.h \
               /Users/pahi/ictools/libs/string.h \
               /Users/pahi/ictools/libs/math.h /Users/pahi/ictools/libs/time.h 
	$(CC) whetstone.c -g -t8 -o whetstone.t8h $(COPT)

clean  : delete
delete :
	-$(DELETE) whetstone.c8h
	-$(DELETE) whetstone.t8h

