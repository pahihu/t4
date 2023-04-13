LIBRARIAN=ilibr
OCCAM=oc
LINK=ilink
CONFIG=icconf
OCONFIG=occonf
COLLECT=icollect
CC=icc
DELETE=del
LIBOPT=
OCCOPT= 
LINKOPT=
CONFOPT=
OCONFOPT=
COLLECTOPT=
COPT= -dFILE_SUPPORT # -dG300_SUPPORT 


##### IMAKEF CUT #####

all: povray.btl dummy.c8x muxdemux.c8x spmux.c8x fread.c8x

povray.btl: povray.cfb
	$(COLLECT) povray.cfb -o povray.btl $(COLLECTOPT)

povray.cfb: povray.cfs povray.c8x dummy.c8x muxdemux.c8x spmux.c8x fread.c8x
	$(CONFIG) povray.cfs -g -o povray.cfb $(CONFOPT)

povray.c8x: povray.lnk bezier.t8x blob.t8x boxes.t8x colour.t8x csg.t8x \
		dump.t8x gif.t8x gifdecod.t8x hfield.t8x iff.t8x lighting.t8x \
		matrices.t8x objects.t8x parse.t8x planes.t8x point.t8x poly.t8x \
		sendpix.t8x povray.t8x prioq.t8x quadrics.t8x raw.t8x ray.t8x \
		render.t8x spheres.t8x targa.t8x texture.t8x tokenize.t8x \
		triangle.t8x txtbump.t8x txtcolor.t8x txtmap.t8x txttest.t8x \
		vect.t8x viewpnt.t8x tranmain.t8x
		$(LINK) -f povray.lnk -t8 -x -o povray.c8x $(LINKOPT)

bezier.t8x: bezier.c frame.h config.h vector.h povproto.h
	$(CC) bezier.c -g -t8 -o bezier.t8x $(COPT)

blob.t8x: blob.c frame.h config.h vector.h povproto.h
	$(CC) blob.c -g -t8 -o blob.t8x $(COPT)

boxes.t8x: boxes.c frame.h config.h vector.h povproto.h
	$(CC) boxes.c -g -t8 -o boxes.t8x $(COPT)

colour.t8x: colour.c frame.h config.h vector.h povproto.h
	$(CC) colour.c -g -t8 -o colour.t8x $(COPT)

csg.t8x: csg.c frame.h config.h vector.h povproto.h
	$(CC) csg.c -g -t8 -o csg.t8x $(COPT)

dump.t8x: dump.c frame.h config.h povproto.h
	$(CC) dump.c -g -t8 -o dump.t8x $(COPT)

gif.t8x: gif.c frame.h config.h povproto.h
	$(CC) gif.c -g -t8 -o gif.t8x $(COPT)

gifdecod.t8x: gifdecod.c frame.h config.h povproto.h
	$(CC) gifdecod.c -g -t8 -o gifdecod.t8x $(COPT)

hfield.t8x: hfield.c frame.h config.h vector.h povproto.h
	$(CC) hfield.c -g -t8 -o hfield.t8x $(COPT)

iff.t8x: iff.c frame.h config.h povproto.h
	$(CC) iff.c -g -t8 -o iff.t8x $(COPT)

lighting.t8x: lighting.c frame.h config.h vector.h povproto.h
	$(CC) lighting.c -g -t8 -o lighting.t8x $(COPT)

matrices.t8x: matrices.c frame.h config.h vector.h povproto.h
	$(CC) matrices.c -g -t8 -o matrices.t8x $(COPT)

objects.t8x: objects.c frame.h config.h vector.h povproto.h
	$(CC) objects.c -g -t8 -o objects.t8x $(COPT)

parse.t8x: parse.c frame.h config.h vector.h povproto.h
	$(CC) parse.c -g -t8 -o parse.t8x $(COPT)

planes.t8x: planes.c frame.h config.h vector.h povproto.h
	$(CC) planes.c -g -t8 -o planes.t8x $(COPT)

point.t8x: point.c frame.h config.h vector.h povproto.h
	$(CC) point.c -g -t8 -o point.t8x $(COPT)

poly.t8x: poly.c frame.h config.h vector.h povproto.h
	$(CC) poly.c -g -t8 -o poly.t8x $(COPT)

sendpix.t8x: sendpix.c config.h xputer.h
	$(CC) sendpix.c -g -t8 -o sendpix.t8x $(COPT)

povray.t8x: povray.c frame.h config.h povproto.h
	$(CC) povray.c -g -t8 -o povray.t8x $(COPT)

prioq.t8x: prioq.c frame.h config.h povproto.h
	$(CC) prioq.c -g -t8 -o prioq.t8x $(COPT)

quadrics.t8x: quadrics.c frame.h config.h vector.h povproto.h
	$(CC) quadrics.c -g -t8 -o quadrics.t8x $(COPT)

raw.t8x: raw.c frame.h config.h povproto.h
	$(CC) raw.c -g -t8 -o raw.t8x $(COPT)

ray.t8x: ray.c frame.h config.h vector.h povproto.h
	$(CC) ray.c -g -t8 -o ray.t8x $(COPT)

render.t8x: render.c frame.h config.h vector.h povproto.h
	$(CC) render.c -g -t8 -o render.t8x $(COPT)

spheres.t8x: spheres.c frame.h config.h vector.h povproto.h
	$(CC) spheres.c -g -t8 -o spheres.t8x $(COPT)

targa.t8x: targa.c frame.h config.h povproto.h
	$(CC) targa.c -g -t8 -o targa.t8x $(COPT)

texture.t8x: texture.c frame.h config.h vector.h povproto.h texture.h
	$(CC) texture.c -g -t8 -o texture.t8x $(COPT)

tokenize.t8x: tokenize.c frame.h config.h povproto.h
	$(CC) tokenize.c -g -t8 -o tokenize.t8x $(COPT)

triangle.t8x: triangle.c frame.h config.h vector.h povproto.h
	$(CC) triangle.c -g -t8 -o triangle.t8x $(COPT)

txtbump.t8x: txtbump.c frame.h config.h vector.h povproto.h texture.h
	$(CC) txtbump.c -g -t8 -o txtbump.t8x $(COPT)

txtcolor.t8x: txtcolor.c frame.h config.h vector.h povproto.h texture.h
	$(CC) txtcolor.c -g -t8 -o txtcolor.t8x $(COPT)

txtmap.t8x: txtmap.c frame.h config.h vector.h povproto.h texture.h
	$(CC) txtmap.c -g -t8 -o txtmap.t8x $(COPT)

txttest.t8x: txttest.c frame.h config.h vector.h povproto.h texture.h
	$(CC) txttest.c -g -t8 -o txttest.t8x $(COPT)

vect.t8x: vect.c frame.h config.h povproto.h vector.h
	$(CC) vect.c -g -t8 -o vect.t8x $(COPT)

viewpnt.t8x: viewpnt.c frame.h config.h vector.h povproto.h
	$(CC) viewpnt.c -g -t8 -o viewpnt.t8x $(COPT)

tranmain.t8x: tranmain.c frame.h config.h povproto.h
	$(CC) tranmain.c -g -t8 -o tranmain.t8x $(COPT)

fread.c8x: fread.lnk fread.t8x
	$(LINK) -f fread.lnk -t8 -x -o fread.c8x $(LINKOPT)

fread.t8x: fread.c
	$(CC) fread.c -g -t8 -o fread.t8x $(COPT)

spmux.c8x: spmux.lnk spmux.t8x
	$(LINK) -f spmux.lnk -t8 -x -o spmux.c8x $(LINKOPT)

spmux.t8x: spmux.c ipack.h iserver.h xputer.h
	$(CC) spmux.c -g -t8 -o spmux.t8x $(COPT)

muxdemux.c8x: muxdemux.lnk muxdemux.t8x
	$(LINK) -f muxdemux.lnk -t8 -x -o muxdemux.c8x $(LINKOPT)

muxdemux.t8x: muxdemux.c ipack.h iserver.h fcache.h xputer.h
	$(CC) muxdemux.c -g -t8 -o muxdemux.t8x $(COPT)

dummy.c8x: dummy.lnk dummy.t8x
	$(LINK) -f dummy.lnk -t8 -x -o dummy.c8x $(LINKOPT)

dummy.t8x: dummy.c
	$(CC) dummy.c -g -t8 -o dummy.t8x $(COPT)

clean  : delete
delete :
	-$(DELETE) povray.cfb
	-$(DELETE) povray.c8x
	-$(DELETE) bezier.t8x
	-$(DELETE) blob.t8x
	-$(DELETE) boxes.t8x
	-$(DELETE) colour.t8x
	-$(DELETE) csg.t8x
	-$(DELETE) dump.t8x
	-$(DELETE) gif.t8x
	-$(DELETE) gifdecod.t8x
	-$(DELETE) hfield.t8x
	-$(DELETE) iff.t8x
	-$(DELETE) lighting.t8x
	-$(DELETE) matrices.t8x
	-$(DELETE) objects.t8x
	-$(DELETE) parse.t8x
	-$(DELETE) planes.t8x
	-$(DELETE) point.t8x
	-$(DELETE) poly.t8x
	-$(DELETE) sendpix.t8x
	-$(DELETE) povray.t8x
	-$(DELETE) prioq.t8x
	-$(DELETE) quadrics.t8x
	-$(DELETE) raw.t8x
	-$(DELETE) ray.t8x
	-$(DELETE) render.t8x
	-$(DELETE) spheres.t8x
	-$(DELETE) targa.t8x
	-$(DELETE) texture.t8x
	-$(DELETE) tokenize.t8x
	-$(DELETE) triangle.t8x
	-$(DELETE) txtbump.t8x
	-$(DELETE) txtcolor.t8x
	-$(DELETE) txtmap.t8x
	-$(DELETE) txttest.t8x
	-$(DELETE) vect.t8x
	-$(DELETE) viewpnt.t8x
	-$(DELETE) tranmain.t8x
	-$(DELETE) fread.c8x
	-$(DELETE) fread.t8x
	-$(DELETE) spmux.c8x
	-$(DELETE) spmux.t8x
	-$(DELETE) muxdemux.c8x
	-$(DELETE) muxdemux.t8x
	-$(DELETE) dummy.c8x
	-$(DELETE) dummy.t8x
