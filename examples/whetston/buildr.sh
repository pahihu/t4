./clean.sh
rm -f whet.occ whetston.lah
ln -s whetr.occ whet.occ
ln -s whetstonr.lah whetston.lah
make -f whetston.mak
mv whetston.btl whetstonr.btl
