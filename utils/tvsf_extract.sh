#
# Extract differences
#
if [ $# -ne 1 ];
then
  echo "usage: extract.sh <instr>"
  exit 1
fi

indirect()      # $1 variable name
{
  eval echo $`echo ${1}`
}

export TVS1=~/tvs1

# clean up
rm -f $tst.diff

# get the instruction
tst=$1
echo "Extracting failed test cases for $tst..."

# extract the input file
inpfil=`grep "/$tst.tbo" ./tvsf_testsn.sh | awk '{print $4;}'`

if [ "X$inpfil" = "X" ];
then
  echo "Input file for $tst is not found"
  exit 1
fi
 
echo "Input file: $inpfil"
inp="`basename $inpfil`"

#
# convert input to HEX
# NB. columns should be changed according to input
#        1,2,3,B,F,P,S
#
IUT1="adc eqc ldc"
IUT1_INP=3
IUT1_OUT=4

IUT2="add and bcnt bitcnt bitrevnbits bitrevword bsub \
      ccnt1 cflerr crcbyte crcword csngl csub0 \
      cword diff div dup \
      fmul gt \
      ladd ldiff ldinf ldiv \
      lmul lshl lshr lsub lsum \
      mint mul \
      norm not or prod \
      rem rev roundsn \
      shl shr  \
      sub sum unpacksn \
      wcnt wsub wsubdb xdble xor xword"
IUT2_INP=3
IUT2_OUT=4

IUT3="postnormsn"
IUT3_INP=4
IUT3_OUT=4

XALT="alt talt"
XALT_INP=0
XALT_OUT=10

FPT1="fpadd__sn fpaddrmsn fpdiv__sn fpsub__sn fpeq__sn fpgt__sn fpudivby2__sn"
FPT1_INP=8
FPT1_OUT=8

FPT2="fpldnladdsn__sn"
FPT2_INP=12
FPT2_OUT=12

TESTS="IUT1 IUT2 IUT3 XALT FPT1 FPT2"

tstcas="NONE"
for t in $TESTS
do
  for instr in `indirect $t`
  do
    if [ "$tst" = "$instr" ];
    then
      tstcas=$t
    fi
  done
done

echo "Found $tst in $tstcas..."
if [ $tstcas = "NONE" ];
then
  echo "Test case $tst is not found!"
  exit 1
fi

inpcols="`indirect ${tstcas}_INP`"
outcols="`indirect ${tstcas}_OUT`"
inpcols=`expr 4 \* ${inpcols}`
outcols=`expr 4 \* ${outcols}`
echo " #input columns = $inpcols"
echo "#output columns = $outcols"

# convert ref and sim file to HEX
echo "Converting ref and sim to HEX..."
myxxd -e 16 -c $outcols ref/T801B/$tst.out ref.hex
myxxd -e 16 -c $outcols tmp/$tst.sim sim.hex

# combine files line-by-line
if [ "X$inpcols" != "X0" ];
then
  echo "Converting input to HEX..."
  myxxd -b 16 -c $inpcols inp/inp/$inp inp.hex

  echo "Combining files..."
  paste inp.hex ref.hex >ref2.hex
  paste inp.hex sim.hex >sim2.hex
else
  cp ref.hex ref2.hex
  cp sim.hex sim2.hex
fi

# make difference
echo -n "Differences..."
diff ref2.hex sim2.hex>$tst.diff
echo "$tst.diff"

# count cases
fails=`grep ':' $tst.diff | wc -l`
fails=`expr $fails / 2`
echo "#failed = $fails"

rm -f *.hex
