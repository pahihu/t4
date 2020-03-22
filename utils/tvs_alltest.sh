#
# clean up
#
rm -f tmp/*

#
# run tests
#
export TVS1=~/tvs1
./t4 -sv $TVS1/tbo/tbo/adc.tbo         $TVS1/inp/inp/i32_1.bin $TVS1/tmp/adc.sim
./t4 -sv $TVS1/tbo/tbo/add.tbo         $TVS1/inp/inp/i32_3.bin $TVS1/tmp/add.sim
./t4 -sv $TVS1/tbo/tbo/alt.tbo         ""                      $TVS1/tmp/alt.sim
./t4 -sv $TVS1/tbo/tbo/and.tbo         $TVS1/inp/inp/i32_2.bin $TVS1/tmp/and.sim
./t4 -sv $TVS1/tbo/tbo/bcnt.tbo        $TVS1/inp/inp/i32_1.bin $TVS1/tmp/bcnt.sim
./t4 -sv $TVS1/tbo/tbo/bitcnt.tbo      $TVS1/inp/inp/i32_2.bin $TVS1/tmp/bitcnt.sim
./t4 -sv $TVS1/tbo/tbo/bitrevnbits.tbo $TVS1/inp/inp/i32_S.bin $TVS1/tmp/bitrevnbits.sim
./t4 -sv $TVS1/tbo/tbo/bitrevword.tbo  $TVS1/inp/inp/i32_1.bin $TVS1/tmp/bitrevword.sim
./t4 -sv $TVS1/tbo/tbo/bsub.tbo        $TVS1/inp/inp/i32_2.bin $TVS1/tmp/bsub.sim
./t4 -sv $TVS1/tbo/tbo/ccnt1.tbo       $TVS1/inp/inp/i32_2.bin $TVS1/tmp/ccnt1.sim
./t4 -sv $TVS1/tbo/tbo/cflerr.tbo      $TVS1/inp/inp/i32_F.bin $TVS1/tmp/cflerr.sim
./t4 -sv $TVS1/tbo/tbo/crcbyte.tbo     $TVS1/inp/inp/i32_3.bin $TVS1/tmp/crcbyte.sim
./t4 -sv $TVS1/tbo/tbo/crcword.tbo     $TVS1/inp/inp/i32_3.bin $TVS1/tmp/crcword.sim
./t4 -sv $TVS1/tbo/tbo/csngl.tbo       $TVS1/inp/inp/i32_2.bin $TVS1/tmp/csngl.sim
./t4 -sv $TVS1/tbo/tbo/csub0.tbo       $TVS1/inp/inp/i32_2.bin $TVS1/tmp/csub0.sim
./t4 -sv $TVS1/tbo/tbo/cword.tbo       $TVS1/inp/inp/i32_B.bin $TVS1/tmp/cword.sim
./t4 -sv $TVS1/tbo/tbo/diff.tbo        $TVS1/inp/inp/i32_2.bin $TVS1/tmp/diff.sim
./t4 -sv $TVS1/tbo/tbo/div.tbo         $TVS1/inp/inp/i32_2.bin $TVS1/tmp/div.sim
./t4 -sv $TVS1/tbo/tbo/dup.tbo         $TVS1/inp/inp/i32_1.bin $TVS1/tmp/dup.sim
./t4 -sv $TVS1/tbo/tbo/eqc.tbo         $TVS1/inp/inp/i32_1.bin $TVS1/tmp/eqc.sim
./t4 -sv $TVS1/tbo/tbo/fmul.tbo        $TVS1/inp/inp/i32_2.bin $TVS1/tmp/fmul.sim
./t4 -sv $TVS1/tbo/tbo/gt.tbo          $TVS1/inp/inp/i32_2.bin $TVS1/tmp/gt.sim
./t4 -sv $TVS1/tbo/tbo/ladd.tbo        $TVS1/inp/inp/i32_3.bin $TVS1/tmp/ladd.sim
./t4 -sv $TVS1/tbo/tbo/ldc.tbo         $TVS1/inp/inp/i32_1.bin $TVS1/tmp/ldc.sim
./t4 -sv $TVS1/tbo/tbo/ldiff.tbo       $TVS1/inp/inp/i32_3.bin $TVS1/tmp/ldiff.sim
./t4 -sv $TVS1/tbo/tbo/ldinf.tbo       $TVS1/inp/inp/i32_F.bin $TVS1/tmp/ldinf.sim
./t4 -sv $TVS1/tbo/tbo/ldiv.tbo        $TVS1/inp/inp/i32_3.bin $TVS1/tmp/ldiv.sim
./t4 -sv $TVS1/tbo/tbo/lmul.tbo        $TVS1/inp/inp/i32_3.bin $TVS1/tmp/lmul.sim
./t4 -sv $TVS1/tbo/tbo/lshl.tbo        $TVS1/inp/inp/i32_S.bin $TVS1/tmp/lshl.sim
./t4 -sv $TVS1/tbo/tbo/lshr.tbo        $TVS1/inp/inp/i32_S.bin $TVS1/tmp/lshr.sim
./t4 -sv $TVS1/tbo/tbo/lsub.tbo        $TVS1/inp/inp/i32_3.bin $TVS1/tmp/lsub.sim
./t4 -sv $TVS1/tbo/tbo/lsum.tbo        $TVS1/inp/inp/i32_3.bin $TVS1/tmp/lsum.sim
./t4 -sv $TVS1/tbo/tbo/mint.tbo        $TVS1/inp/inp/i32_1.bin $TVS1/tmp/mint.sim
./t4 -sv $TVS1/tbo/tbo/mul.tbo         $TVS1/inp/inp/i32_2.bin $TVS1/tmp/mul.sim
./t4 -sv $TVS1/tbo/tbo/norm.tbo        $TVS1/inp/inp/i32_3.bin $TVS1/tmp/norm.sim
./t4 -sv $TVS1/tbo/tbo/not.tbo         $TVS1/inp/inp/i32_1.bin $TVS1/tmp/not.sim
./t4 -sv $TVS1/tbo/tbo/or.tbo          $TVS1/inp/inp/i32_2.bin $TVS1/tmp/or.sim
./t4 -sv $TVS1/tbo/tbo/postnormsn.tbo  $TVS1/inp/inp/i32_P.bin $TVS1/tmp/postnormsn.sim
./t4 -sv $TVS1/tbo/tbo/prod.tbo        $TVS1/inp/inp/i32_2.bin $TVS1/tmp/prod.sim
./t4 -sv $TVS1/tbo/tbo/rem.tbo         $TVS1/inp/inp/i32_2.bin $TVS1/tmp/rem.sim
./t4 -sv $TVS1/tbo/tbo/rev.tbo         $TVS1/inp/inp/i32_1.bin $TVS1/tmp/rev.sim
./t4 -sv $TVS1/tbo/tbo/roundsn.tbo     $TVS1/inp/inp/i32_3.bin $TVS1/tmp/roundsn.sim
./t4 -sv $TVS1/tbo/tbo/shl.tbo         $TVS1/inp/inp/i32_S.bin $TVS1/tmp/shl.sim
./t4 -sv $TVS1/tbo/tbo/shr.tbo         $TVS1/inp/inp/i32_S.bin $TVS1/tmp/shr.sim
./t4 -sv $TVS1/tbo/tbo/sub.tbo         $TVS1/inp/inp/i32_2.bin $TVS1/tmp/sub.sim
./t4 -sv $TVS1/tbo/tbo/sum.tbo         $TVS1/inp/inp/i32_2.bin $TVS1/tmp/sum.sim
./t4 -sv $TVS1/tbo/tbo/talt.tbo        ""                      $TVS1/tmp/talt.sim
./t4 -sv $TVS1/tbo/tbo/unpacksn.tbo    $TVS1/inp/inp/i32_F.bin $TVS1/tmp/unpacksn.sim
./t4 -sv $TVS1/tbo/tbo/wcnt.tbo        $TVS1/inp/inp/i32_1.bin $TVS1/tmp/wcnt.sim
./t4 -sv $TVS1/tbo/tbo/wsub.tbo        $TVS1/inp/inp/i32_2.bin $TVS1/tmp/wsub.sim
./t4 -sv $TVS1/tbo/tbo/wsubdb.tbo      $TVS1/inp/inp/i32_2.bin $TVS1/tmp/wsubdb.sim
./t4 -sv $TVS1/tbo/tbo/xdble.tbo       $TVS1/inp/inp/i32_1.bin $TVS1/tmp/xdble.sim
./t4 -sv $TVS1/tbo/tbo/xor.tbo         $TVS1/inp/inp/i32_2.bin $TVS1/tmp/xor.sim
./t4 -sv $TVS1/tbo/tbo/xword.tbo       $TVS1/inp/inp/i32_B.bin $TVS1/tmp/xword.sim
