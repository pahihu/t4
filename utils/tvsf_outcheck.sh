#
# checking test output
#
echo "Checking test output..."

export TVS1=~/tvs1f
for sim in `ls $TVS1/out`
do
  tst=`basename $sim .out`
  ref=ref/T801B/$tst.out
  # echo "$sim <=> $ref"
  cmp -s out/$sim $ref
  if [ $? -eq 0 ];
  then
    echo "    PASS $tst"
  else
    echo "    FAIL $tst"
  fi
done
