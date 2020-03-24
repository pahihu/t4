#
# checking test output
#
echo "Checking test output..."

export TVS1=~/tvs1f
for sim in `ls $TVS1/tmp`
do
  tst=`basename $sim .sim`
  ref=ref/T801B/$tst.out
  cmp -s tmp/$sim $ref
  if [ $? -eq 0 ];
  then
    echo "    PASS $tst"
  else
    echo "    FAIL $tst"
  fi
done
