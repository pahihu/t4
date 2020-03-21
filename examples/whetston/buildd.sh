./clean.sh
rm -f whet.occ whetston.lah
ln -s whetd.occ whet.occ
ln -s whetstond.lah whetston.lah
make -f whetston.mak
mv whetston.btl whetstond.btl
