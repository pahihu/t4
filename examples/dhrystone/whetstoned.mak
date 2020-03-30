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

whetstoned.b8h: whetstoned.c8h 
	$(COLLECT) whetstoned.c8h -t  -o whetstoned.b8h $(COLLECTOPT)

whetstoned.c8h: whetstoned.lnk whetstoned.t8h
	$(LINK) -f whetstoned.lnk -t8 -h -o whetstoned.c8h $(LINKOPT)

whetstoned.t8h: whetstone.c
	$(CC) whetstone.c -DREALPREC=$(REALPREC) -t8 -o whetstoned.t8h $(COPT)

clean  : delete
delete :
	-$(DELETE) whetstoned.c8h
	-$(DELETE) whetstoned.t8h
	-$(DELETE) whetstoned.cfb

