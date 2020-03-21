./clean.sh
rm -f whet.occ whetston.l8h
ln -s whetr.occ whet.occ
ln -s whetstonr.l8h whetston.l8h
make -f whetston.mak
mv whetston.btl whetstonr.btl
