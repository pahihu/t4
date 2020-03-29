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

# extract precision
prec=`echo $tst | tail -c 3`
if [ "$prec" != "sn" ] && [ "$prec" != "db" ];
then
  prec="iu"
fi

# extract the input file
inpfil=`grep "/$tst.tbo" ./tvsf_test${prec}.sh | awk '{print $4;}'`

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

IUT="call"
IUT_INP=0
IUT_OUT=4

XALT="alt talt enbs"
XALT_INP=0
XALT_OUT=10

FPT1="fpadd__sn fpaddrmsn fpdiv__sn fpsub__sn \
      fpeq__sn fpgt__sn \
      fpudivby2__sn \
      fpudivby2rmsn \
      fpudivby2rpsn \
      fpumulby2__sn \
      fpumulby2rmsn \
      fpumulby2rpsn \
      fpumulby2rzsn \
      fpusqrt__sn fpuchki32__sn \
      fprem__sn \
      fpintrmsn \
      fprtoi32rmsn \
      fpuchki64__sn"
FPT1_INP=8
FPT1_OUT=8

FPT2="fpldnladdsn__sn fpur64tor32__db \
        fpuchki32__db fpuchki64__db \
        fprtoi32__db \
        fprtoi32rmdb \
        fprtoi32rzdb \
        fpstnli32__sn \
        fprem__db \
        fpunoround__db \
        fpintrmdb \
        fpudivby2__db \
        fpusqrt__db \
        fpusqrtrmdb \
        fpusqrtrpdb \
        fpusqrtrzdb" 
FPT2_INP=12
FPT2_OUT=12

FPT3="fpur32tor64__sn"
FPT3_INP=8
FPT3_OUT=12

FPT4="fpldnldbi__db"
FPT4_INP=16
FPT4_OUT=16

TESTS="IUT1 IUT2 IUT3 IUT XALT FPT1 FPT2 FPT3 FPT4"

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
myxxd -b 16 -c $outcols ref/T801B/$tst.out ref.hex
myxxd -b 16 -c $outcols tmp/$tst.sim sim.hex

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
