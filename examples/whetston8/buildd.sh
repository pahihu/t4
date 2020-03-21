./clean.sh
rm -f whet.occ whetston.l8h
ln -s whetd.occ whet.occ
ln -s whetstond.l8h whetston.l8h
make -f whetston.mak
mv whetston.btl whetstond.btl
