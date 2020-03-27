#
# clean up
#
rm -f tmp/*

#
# run tests
#
./tvsf_testiu.sh
./tvsf_testsn.sh
./tvsf_testdb.sh
