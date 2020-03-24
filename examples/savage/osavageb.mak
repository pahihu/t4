LIBRARIAN=ilibr
OCCAM=oc
LINK=ilink
CONFIG=icconf
OCONFIG=occonf
COLLECT=icollect
CC=icc
DELETE=rm
LIBOPT=
OCCOPT=-w
LINKOPT=
CONFOPT=
OCONFOPT=
COLLECTOPT=
COPT=


##### IMAKEF CUT #####

savageb.bbh: savageb.cbh 
	$(COLLECT) savageb.cbh -t  -o savageb.bbh $(COLLECTOPT)

savageb.cbh: savageb.lbh savageb.tbh 
	$(LINK) -f savageb.lbh -tb -h -o savageb.cbh $(LINKOPT)

savageb.tbh: savageb.occ
	$(OCCAM) savageb -tb -h -o savageb.tbh $(OCCOPT)

clean  : delete
delete :
	-$(DELETE) savageb.cbh
	-$(DELETE) savageb.tbh

