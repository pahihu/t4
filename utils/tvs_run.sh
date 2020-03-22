#
# Run the tests, check FAIL cases
#
sh ./tvs_test.sh
sh ./tvs_check.sh | grep FAIL
