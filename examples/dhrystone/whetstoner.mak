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
REALPREC=4


##### IMAKEF CUT #####

whetstoner.b8h: whetstoner.c8h 
	$(COLLECT) whetstoner.c8h -t  -o whetstoner.b8h $(COLLECTOPT)

whetstoner.c8h: whetstoner.lnk whetstoner.t8h
	$(LINK) -f whetstoner.lnk -t8 -h -o whetstoner.c8h $(LINKOPT)

whetstoner.t8h: whetstone.c
	$(CC) whetstone.c -DREALPREC=$(REALPREC) -t8 -o whetstoner.t8h $(COPT)

clean  : delete
delete :
	-$(DELETE) whetstoner.c8h
	-$(DELETE) whetstoner.t8h
	-$(DELETE) whetstoner.cfb

