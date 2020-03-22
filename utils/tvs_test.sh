#
# clean up
#
rm -f tmp/*

#
# run tests
#
echo "Running tests..."

export TVS1=~/tvs1
./t4 -sv $TVS1/tbo/tbo/bitrevnbits.tbo $TVS1/inp/inp/i32_S.bin $TVS1/tmp/bitrevnbits.sim
./t4 -sv $TVS1/tbo/tbo/crcbyte.tbo     $TVS1/inp/inp/i32_3.bin $TVS1/tmp/crcbyte.sim
./t4 -sv $TVS1/tbo/tbo/crcword.tbo     $TVS1/inp/inp/i32_3.bin $TVS1/tmp/crcword.sim
./t4 -sv $TVS1/tbo/tbo/cword.tbo       $TVS1/inp/inp/i32_B.bin $TVS1/tmp/cword.sim
./t4 -sv $TVS1/tbo/tbo/div.tbo         $TVS1/inp/inp/i32_2.bin $TVS1/tmp/div.sim
./t4 -sv $TVS1/tbo/tbo/fmul.tbo        $TVS1/inp/inp/i32_2.bin $TVS1/tmp/fmul.sim
./t4 -sv $TVS1/tbo/tbo/ldiv.tbo        $TVS1/inp/inp/i32_3.bin $TVS1/tmp/ldiv.sim
./t4 -sv $TVS1/tbo/tbo/mul.tbo         $TVS1/inp/inp/i32_2.bin $TVS1/tmp/mul.sim
./t4 -sv $TVS1/tbo/tbo/postnormsn.tbo  $TVS1/inp/inp/i32_P.bin $TVS1/tmp/postnormsn.sim
./t4 -sv $TVS1/tbo/tbo/rem.tbo         $TVS1/inp/inp/i32_2.bin $TVS1/tmp/rem.sim
./t4 -sv $TVS1/tbo/tbo/roundsn.tbo     $TVS1/inp/inp/i32_3.bin $TVS1/tmp/roundsn.sim
./t4 -sv $TVS1/tbo/tbo/unpacksn.tbo    $TVS1/inp/inp/i32_F.bin $TVS1/tmp/unpacksn.sim
./t4 -sv $TVS1/tbo/tbo/wcnt.tbo        $TVS1/inp/inp/i32_1.bin $TVS1/tmp/wcnt.sim
./t4 -sv $TVS1/tbo/tbo/xword.tbo       $TVS1/inp/inp/i32_B.bin $TVS1/tmp/xword.sim
