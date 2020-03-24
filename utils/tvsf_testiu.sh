# --- INTEGER ---

#
# clean up
#
rm -f tmp/*

#
# run tests
#
./t4 -sv tbo/tbo/adc.tbo      inp/inp/i32_1.bin         tmp/adc.sim        #4000
./t4 -sv tbo/tbo/add.tbo      inp/inp/i32_3.bin         tmp/add.sim      #200000
./t4 -sv tbo/tbo/alt.tbo      ""                        tmp/alt.sim          #28
./t4 -sv tbo/tbo/altend.tbo   ""                        tmp/altend.sim       #30
./t4 -sv tbo/tbo/and.tbo      inp/inp/i32_2.bin         tmp/and.sim       #40000
./t4 -sv tbo/tbo/bcnt.tbo     inp/inp/i32_1.bin         tmp/bcnt.sim        #800
./t4 -sv tbo/tbo/bitcnt.tbo   inp/inp/i32_2.bin         tmp/bitcnt.sim    #40000
./t4 -sv tbo/tbo/bitrevnbits.tbo inp/inp/i32_S.bin      tmp/bitrevnbits.sim #108000
./t4 -sv tbo/tbo/bitrevword.tbo inp/inp/i32_1.bin       tmp/bitrevword.sim    #800
./t4 -sv tbo/tbo/bsub.tbo     inp/inp/i32_2.bin         tmp/bsub.sim      #40000
./t4 -sv tbo/tbo/call.tbo     ""                        tmp/call.sim         #40
./t4 -sv tbo/tbo/ccnt1.tbo    inp/inp/i32_2.bin         tmp/ccnt1.sim     #40000
./t4 -sv tbo/tbo/cj.tbo       ""                        tmp/cj.sim           #30
./t4 -sv tbo/tbo/crcbyte.tbo  inp/inp/i32_3.bin         tmp/crcbyte.sim  #200000
./t4 -sv tbo/tbo/crcword.tbo  inp/inp/i32_3.bin         tmp/crcword.sim  #200000
./t4 -sv tbo/tbo/csngl.tbo    inp/inp/i32_2.bin         tmp/csngl.sim     #40000
./t4 -sv tbo/tbo/csub0.tbo    inp/inp/i32_2.bin         tmp/csub0.sim     #40000
./t4 -sv tbo/tbo/cword.tbo    inp/inp/i32_B.bin         tmp/cword.sim     #80000
./t4 -sv tbo/tbo/diff.tbo     inp/inp/i32_2.bin         tmp/diff.sim      #40000
./t4 -sv tbo/tbo/div.tbo      inp/inp/i32_2.bin         tmp/div.sim       #40000
./t4 -sv tbo/tbo/dup.tbo      inp/inp/i32_1.bin         tmp/dup.sim         #800
./t4 -sv tbo/tbo/enbc.tbo     ""                        tmp/enbc.sim         #B0
./t4 -sv tbo/tbo/enbs.tbo     ""                        tmp/enbs.sim         #A0
./t4 -sv tbo/tbo/enbt.tbo     ""                        tmp/enbt.sim         #A0
./t4 -sv tbo/tbo/eqc.tbo      inp/inp/i32_1.bin         tmp/eqc.sim        #4000
./t4 -sv tbo/tbo/fmul.tbo     inp/inp/i32_2.bin         tmp/fmul.sim      #40000
./t4 -sv tbo/tbo/gt.tbo       inp/inp/i32_2.bin         tmp/gt.sim        #40000
./t4 -sv tbo/tbo/j.tbo        ""                        tmp/j.sim            #30
./t4 -sv tbo/tbo/ladd.tbo     inp/inp/i32_3.bin         tmp/ladd.sim     #200000
./t4 -sv tbo/tbo/ldc.tbo      inp/inp/i32_1.bin         tmp/ldc.sim        #4000
./t4 -sv tbo/tbo/ldiff.tbo    inp/inp/i32_3.bin         tmp/ldiff.sim    #200000
./t4 -sv tbo/tbo/ldiv.tbo     inp/inp/i32_3.bin         tmp/ldiv.sim     #200000
./t4 -sv tbo/tbo/lmul.tbo     inp/inp/i32_3.bin         tmp/lmul.sim     #200000
./t4 -sv tbo/tbo/lshl.tbo     inp/inp/i32_S.bin         tmp/lshl.sim     #108000
./t4 -sv tbo/tbo/lshr.tbo     inp/inp/i32_S.bin         tmp/lshr.sim     #108000
./t4 -sv tbo/tbo/lsub.tbo     inp/inp/i32_3.bin         tmp/lsub.sim     #200000
./t4 -sv tbo/tbo/lsum.tbo     inp/inp/i32_3.bin         tmp/lsum.sim     #200000
./t4 -sv tbo/tbo/mint.tbo     inp/inp/i32_1.bin         tmp/mint.sim        #800
./t4 -sv tbo/tbo/mul.tbo      inp/inp/i32_2.bin         tmp/mul.sim       #40000
./t4 -sv tbo/tbo/norm.tbo     inp/inp/i32_3.bin         tmp/norm.sim     #200000
./t4 -sv tbo/tbo/not.tbo      inp/inp/i32_1.bin         tmp/not.sim         #800
./t4 -sv tbo/tbo/or.tbo       inp/inp/i32_2.bin         tmp/or.sim        #40000
./t4 -sv tbo/tbo/prod.tbo     inp/inp/i32_2.bin         tmp/prod.sim      #40000
./t4 -sv tbo/tbo/rem.tbo      inp/inp/i32_2.bin         tmp/rem.sim       #40000
./t4 -sv tbo/tbo/rev.tbo      inp/inp/i32_1.bin         tmp/rev.sim         #800
./t4 -sv tbo/tbo/saveh.tbo    ""                        tmp/saveh.sim        #28
./t4 -sv tbo/tbo/savel.tbo    ""                        tmp/savel.sim        #28
./t4 -sv tbo/tbo/shl.tbo      inp/inp/i32_S.bin         tmp/shl.sim      #108000
./t4 -sv tbo/tbo/shr.tbo      inp/inp/i32_S.bin         tmp/shr.sim      #108000
./t4 -sv tbo/tbo/sthb.tbo     ""                        tmp/sthb.sim         #28
./t4 -sv tbo/tbo/sthf.tbo     ""                        tmp/sthf.sim         #28
./t4 -sv tbo/tbo/stlb.tbo     ""                        tmp/stlb.sim         #28
./t4 -sv tbo/tbo/stlf.tbo     ""                        tmp/stlf.sim         #28
./t4 -sv tbo/tbo/sub.tbo      inp/inp/i32_2.bin         tmp/sub.sim       #40000
./t4 -sv tbo/tbo/sum.tbo      inp/inp/i32_2.bin         tmp/sum.sim       #40000
./t4 -sv tbo/tbo/talt.tbo     ""                        tmp/talt.sim         #28
./t4 -sv tbo/tbo/wcnt.tbo     inp/inp/i32_1.bin         tmp/wcnt.sim        #800
./t4 -sv tbo/tbo/wsub.tbo     inp/inp/i32_2.bin         tmp/wsub.sim      #40000
./t4 -sv tbo/tbo/wsubdb.tbo   inp/inp/i32_2.bin         tmp/wsubdb.sim    #40000
./t4 -sv tbo/tbo/xdble.tbo    inp/inp/i32_1.bin         tmp/xdble.sim       #800
./t4 -sv tbo/tbo/xor.tbo      inp/inp/i32_2.bin         tmp/xor.sim       #40000
./t4 -sv tbo/tbo/xword.tbo    inp/inp/i32_B.bin         tmp/xword.sim     #80000
