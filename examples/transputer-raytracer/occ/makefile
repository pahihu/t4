# File: makefile

OC=oc
ILINK=ilink
OCCONF=occonf
ICOLLECT=icollect

OCOPT=-Y -T8 -A -U
ILINKOPT=-Y -T8
OCCONFOPT=-Y 
ICOLLECTOPT=

all: raytrace.btl

clean:
	$(RM) raytrace.btl
	$(RM) raytrace.cfb
	$(RM) raytrace.clu
	$(RM) raytrace.map
	$(RM) cntlsys.lku
	$(RM) cntlsys.tco
	$(RM) framebuf.lku
	$(RM) framebuf.tco
	$(RM) raytrace.lku
	$(RM) raytrace.tco
	$(RM) ray.ppm

raytrace.btl: raytrace.cfb makefile
	$(ICOLLECT) raytrace.cfb -O $@ -P raytrace.map $(ICOLLECTOPT)

raytrace.cfb: raytrace.cfs cntlsys.lku framebuf.lku raytrace.lku makefile
	$(OCCONF) raytrace.cfs -O $@ $(OCCONFOPT)

cntlsys.lku: cntlsys.tco makefile
	$(ILINK) cntlsys.tco -O $@ -F occam8.lnk $(ILINKOPT)

cntlsys.tco: cntlsys.occ raytrace.inc makefile
	$(OC) cntlsys.occ -O $@ $(OCOPT)

framebuf.lku: framebuf.tco makefile
	$(ILINK) framebuf.tco hostio.lib convert.lib -O $@ -F occam8.lnk $(ILINKOPT)

framebuf.tco: framebuf.occ raytrace.inc makefile
	$(OC) framebuf.occ -O $@ $(OCOPT)

raytrace.lku: raytrace.tco makefile
	$(ILINK) raytrace.tco -O $@ -F occam8.lnk $(ILINKOPT)

raytrace.tco: raytrace.occ raytrace.inc makefile
	$(OC) raytrace.occ -O $@ $(OCOPT)

# -=EOF=-
