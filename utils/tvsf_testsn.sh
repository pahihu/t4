# --- SINGLE ---

#
# clean up
#
rm -f tmp/*

#
# run tests
#
./t4 -sv tbo/tbo/fpdup__sn.tbo         inp/inp/fpu_1sn.bin tmp/fpdup__sn.sim  #1000
./t4 -sv tbo/tbo/fprev__sn.tbo         inp/inp/fpu_1sn.bin tmp/fprev__sn.sim  #1000
./t4 -sv tbo/tbo/fpurn__sn.tbo         inp/inp/fpu_1sn.bin tmp/fpurn__sn.sim  #1000
./t4 -sv tbo/tbo/fpurz__sn.tbo         inp/inp/fpu_1sn.bin tmp/fpurz__sn.sim  #1000
./t4 -sv tbo/tbo/fpurp__sn.tbo         inp/inp/fpu_1sn.bin tmp/fpurp__sn.sim  #1000
./t4 -sv tbo/tbo/fpurm__sn.tbo         inp/inp/fpu_1sn.bin tmp/fpurm__sn.sim  #1000
./t4 -sv tbo/tbo/fpadd__sn.tbo         inp/inp/fpu_2sn.bin tmp/fpadd__sn.sim  #80000
./t4 -sv tbo/tbo/fpsub__sn.tbo         inp/inp/fpu_2sn.bin tmp/fpsub__sn.sim  #80000
./t4 -sv tbo/tbo/fpmul__sn.tbo         inp/inp/fpu_2sn.bin tmp/fpmul__sn.sim  #80000
./t4 -sv tbo/tbo/fpdiv__sn.tbo         inp/inp/fpu_2sn.bin tmp/fpdiv__sn.sim  #80000
./t4 -sv tbo/tbo/fpuabs__sn.tbo        inp/inp/fpu_1sn.bin tmp/fpuabs__sn.sim  #1000
./t4 -sv tbo/tbo/fpumulby2__sn.tbo     inp/inp/fpu_1sn.bin tmp/fpumulby2__sn.sim  #1000
./t4 -sv tbo/tbo/fpudivby2__sn.tbo     inp/inp/fpu_1sn.bin tmp/fpudivby2__sn.sim  #1000
./t4 -sv tbo/tbo/fpuexpinc32__sn.tbo   inp/inp/fpu_1sn.bin tmp/fpuexpinc32__sn.sim  #1000
./t4 -sv tbo/tbo/fpuexpdec32__sn.tbo   inp/inp/fpu_1sn.bin tmp/fpuexpdec32__sn.sim  #1000
./t4 -sv tbo/tbo/fpchkerr__sn.tbo      inp/inp/fpu_1sn.bin tmp/fpchkerr__sn.sim  #1000
./t4 -sv tbo/tbo/fptesterr__sn.tbo     inp/inp/fpu_1sn.bin tmp/fptesterr__sn.sim  #1000
./t4 -sv tbo/tbo/fpuseterr__sn.tbo     inp/inp/fpu_1sn.bin tmp/fpuseterr__sn.sim  #1000
./t4 -sv tbo/tbo/fpuclrerr__sn.tbo     inp/inp/fpu_1sn.bin tmp/fpuclrerr__sn.sim  #1000
./t4 -sv tbo/tbo/fpgt__sn.tbo          inp/inp/fpu_2sn.bin tmp/fpgt__sn.sim  #80000
./t4 -sv tbo/tbo/fpeq__sn.tbo          inp/inp/fpu_2sn.bin tmp/fpeq__sn.sim  #80000
./t4 -sv tbo/tbo/fpordered__sn.tbo     inp/inp/fpu_2sn.bin tmp/fpordered__sn.sim  #80000
./t4 -sv tbo/tbo/fpnan__sn.tbo         inp/inp/fpu_1sn.bin tmp/fpnan__sn.sim  #1000
./t4 -sv tbo/tbo/fpnotfinite__sn.tbo   inp/inp/fpu_1sn.bin tmp/fpnotfinite__sn.sim  #1000
./t4 -sv tbo/tbo/fpint__sn.tbo         inp/inp/fpu_1sn.bin tmp/fpint__sn.sim  #1000
./t4 -sv tbo/tbo/fpuchki32__sn.tbo     inp/inp/fpu_1sn.bin tmp/fpuchki32__sn.sim  #1000
./t4 -sv tbo/tbo/fpuchki64__sn.tbo     inp/inp/fpu_1sn.bin tmp/fpuchki64__sn.sim  #1000
./t4 -sv tbo/tbo/fprtoi32__sn.tbo      inp/inp/fpu_1sn.bin tmp/fprtoi32__sn.sim  #1000
./t4 -sv tbo/tbo/fprem__sn.tbo         inp/inp/fpu_2sn.bin tmp/fprem__sn.sim #80000

./t4 -sv tbo/tbo/fpduprnsn.tbo         inp/inp/fpu_1sn.bin tmp/fpduprnsn.sim  #1000
./t4 -sv tbo/tbo/fprevrnsn.tbo         inp/inp/fpu_1sn.bin tmp/fprevrnsn.sim  #1000
./t4 -sv tbo/tbo/fpurnrnsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurnrnsn.sim  #1000
./t4 -sv tbo/tbo/fpurzrnsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurzrnsn.sim  #1000
./t4 -sv tbo/tbo/fpurprnsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurprnsn.sim  #1000
./t4 -sv tbo/tbo/fpurmrnsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurmrnsn.sim  #1000
./t4 -sv tbo/tbo/fpaddrnsn.tbo         inp/inp/fpu_2sn.bin tmp/fpaddrnsn.sim  #80000
./t4 -sv tbo/tbo/fpsubrnsn.tbo         inp/inp/fpu_2sn.bin tmp/fpsubrnsn.sim  #80000
./t4 -sv tbo/tbo/fpmulrnsn.tbo         inp/inp/fpu_2sn.bin tmp/fpmulrnsn.sim  #80000
./t4 -sv tbo/tbo/fpdivrnsn.tbo         inp/inp/fpu_2sn.bin tmp/fpdivrnsn.sim  #80000
./t4 -sv tbo/tbo/fpuabsrnsn.tbo        inp/inp/fpu_1sn.bin tmp/fpuabsrnsn.sim  #1000
./t4 -sv tbo/tbo/fpumulby2rnsn.tbo     inp/inp/fpu_1sn.bin tmp/fpumulby2rnsn.sim  #1000
./t4 -sv tbo/tbo/fpudivby2rnsn.tbo     inp/inp/fpu_1sn.bin tmp/fpudivby2rnsn.sim  #1000
./t4 -sv tbo/tbo/fpuexpinc32rnsn.tbo   inp/inp/fpu_1sn.bin tmp/fpuexpinc32rnsn.sim  #1000
./t4 -sv tbo/tbo/fpuexpdec32rnsn.tbo   inp/inp/fpu_1sn.bin tmp/fpuexpdec32rnsn.sim  #1000
./t4 -sv tbo/tbo/fpchkerrrnsn.tbo      inp/inp/fpu_1sn.bin tmp/fpchkerrrnsn.sim  #1000
./t4 -sv tbo/tbo/fptesterrrnsn.tbo     inp/inp/fpu_1sn.bin tmp/fptesterrrnsn.sim  #1000
./t4 -sv tbo/tbo/fpuseterrrnsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuseterrrnsn.sim  #1000
./t4 -sv tbo/tbo/fpuclrerrrnsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuclrerrrnsn.sim  #1000
./t4 -sv tbo/tbo/fpgtrnsn.tbo          inp/inp/fpu_2sn.bin tmp/fpgtrnsn.sim  #80000
./t4 -sv tbo/tbo/fpeqrnsn.tbo          inp/inp/fpu_2sn.bin tmp/fpeqrnsn.sim  #80000
./t4 -sv tbo/tbo/fporderedrnsn.tbo     inp/inp/fpu_2sn.bin tmp/fporderedrnsn.sim  #80000
./t4 -sv tbo/tbo/fpnanrnsn.tbo         inp/inp/fpu_1sn.bin tmp/fpnanrnsn.sim  #1000
./t4 -sv tbo/tbo/fpnotfiniternsn.tbo   inp/inp/fpu_1sn.bin tmp/fpnotfiniternsn.sim  #1000
./t4 -sv tbo/tbo/fpintrnsn.tbo         inp/inp/fpu_1sn.bin tmp/fpintrnsn.sim  #1000
./t4 -sv tbo/tbo/fpuchki32rnsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuchki32rnsn.sim  #1000
./t4 -sv tbo/tbo/fpuchki64rnsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuchki64rnsn.sim  #1000
./t4 -sv tbo/tbo/fprtoi32rnsn.tbo      inp/inp/fpu_1sn.bin tmp/fprtoi32rnsn.sim  #1000

./t4 -sv tbo/tbo/fpduprzsn.tbo         inp/inp/fpu_1sn.bin tmp/fpduprzsn.sim  #1000
./t4 -sv tbo/tbo/fprevrzsn.tbo         inp/inp/fpu_1sn.bin tmp/fprevrzsn.sim  #1000
./t4 -sv tbo/tbo/fpurnrzsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurnrzsn.sim  #1000
./t4 -sv tbo/tbo/fpurzrzsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurzrzsn.sim  #1000
./t4 -sv tbo/tbo/fpurprzsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurprzsn.sim  #1000
./t4 -sv tbo/tbo/fpurmrzsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurmrzsn.sim  #1000
./t4 -sv tbo/tbo/fpaddrzsn.tbo         inp/inp/fpu_2sn.bin tmp/fpaddrzsn.sim  #80000
./t4 -sv tbo/tbo/fpsubrzsn.tbo         inp/inp/fpu_2sn.bin tmp/fpsubrzsn.sim  #80000
./t4 -sv tbo/tbo/fpmulrzsn.tbo         inp/inp/fpu_2sn.bin tmp/fpmulrzsn.sim  #80000
./t4 -sv tbo/tbo/fpdivrzsn.tbo         inp/inp/fpu_2sn.bin tmp/fpdivrzsn.sim  #80000
./t4 -sv tbo/tbo/fpuabsrzsn.tbo        inp/inp/fpu_1sn.bin tmp/fpuabsrzsn.sim  #1000
./t4 -sv tbo/tbo/fpumulby2rzsn.tbo     inp/inp/fpu_1sn.bin tmp/fpumulby2rzsn.sim  #1000
./t4 -sv tbo/tbo/fpudivby2rzsn.tbo     inp/inp/fpu_1sn.bin tmp/fpudivby2rzsn.sim  #1000
./t4 -sv tbo/tbo/fpuexpinc32rzsn.tbo   inp/inp/fpu_1sn.bin tmp/fpuexpinc32rzsn.sim  #1000
./t4 -sv tbo/tbo/fpuexpdec32rzsn.tbo   inp/inp/fpu_1sn.bin tmp/fpuexpdec32rzsn.sim  #1000
./t4 -sv tbo/tbo/fpchkerrrzsn.tbo      inp/inp/fpu_1sn.bin tmp/fpchkerrrzsn.sim  #1000
./t4 -sv tbo/tbo/fptesterrrzsn.tbo     inp/inp/fpu_1sn.bin tmp/fptesterrrzsn.sim  #1000
./t4 -sv tbo/tbo/fpuseterrrzsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuseterrrzsn.sim  #1000
./t4 -sv tbo/tbo/fpuclrerrrzsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuclrerrrzsn.sim  #1000
./t4 -sv tbo/tbo/fpgtrzsn.tbo          inp/inp/fpu_2sn.bin tmp/fpgtrzsn.sim  #80000
./t4 -sv tbo/tbo/fpeqrzsn.tbo          inp/inp/fpu_2sn.bin tmp/fpeqrzsn.sim  #80000
./t4 -sv tbo/tbo/fporderedrzsn.tbo     inp/inp/fpu_2sn.bin tmp/fporderedrzsn.sim  #80000
./t4 -sv tbo/tbo/fpnanrzsn.tbo         inp/inp/fpu_1sn.bin tmp/fpnanrzsn.sim  #1000
./t4 -sv tbo/tbo/fpnotfiniterzsn.tbo   inp/inp/fpu_1sn.bin tmp/fpnotfiniterzsn.sim  #1000
./t4 -sv tbo/tbo/fpintrzsn.tbo         inp/inp/fpu_1sn.bin tmp/fpintrzsn.sim  #1000
./t4 -sv tbo/tbo/fpuchki32rzsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuchki32rzsn.sim  #1000
./t4 -sv tbo/tbo/fpuchki64rzsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuchki64rzsn.sim  #1000
./t4 -sv tbo/tbo/fprtoi32rzsn.tbo      inp/inp/fpu_1sn.bin tmp/fprtoi32rzsn.sim  #1000

./t4 -sv tbo/tbo/fpduprpsn.tbo         inp/inp/fpu_1sn.bin tmp/fpduprpsn.sim  #1000
./t4 -sv tbo/tbo/fprevrpsn.tbo         inp/inp/fpu_1sn.bin tmp/fprevrpsn.sim  #1000
./t4 -sv tbo/tbo/fpurnrpsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurnrpsn.sim  #1000
./t4 -sv tbo/tbo/fpurzrpsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurzrpsn.sim  #1000
./t4 -sv tbo/tbo/fpurprpsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurprpsn.sim  #1000
./t4 -sv tbo/tbo/fpurmrpsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurmrpsn.sim  #1000
./t4 -sv tbo/tbo/fpaddrpsn.tbo         inp/inp/fpu_2sn.bin tmp/fpaddrpsn.sim  #80000
./t4 -sv tbo/tbo/fpsubrpsn.tbo         inp/inp/fpu_2sn.bin tmp/fpsubrpsn.sim  #80000
./t4 -sv tbo/tbo/fpmulrpsn.tbo         inp/inp/fpu_2sn.bin tmp/fpmulrpsn.sim  #80000
./t4 -sv tbo/tbo/fpdivrpsn.tbo         inp/inp/fpu_2sn.bin tmp/fpdivrpsn.sim  #80000
./t4 -sv tbo/tbo/fpuabsrpsn.tbo        inp/inp/fpu_1sn.bin tmp/fpuabsrpsn.sim  #1000
./t4 -sv tbo/tbo/fpumulby2rpsn.tbo     inp/inp/fpu_1sn.bin tmp/fpumulby2rpsn.sim    #1000
./t4 -sv tbo/tbo/fpudivby2rpsn.tbo     inp/inp/fpu_1sn.bin tmp/fpudivby2rpsn.sim    #1000
./t4 -sv tbo/tbo/fpuexpinc32rpsn.tbo   inp/inp/fpu_1sn.bin tmp/fpuexpinc32rpsn.sim  #1000
./t4 -sv tbo/tbo/fpuexpdec32rpsn.tbo   inp/inp/fpu_1sn.bin tmp/fpuexpdec32rpsn.sim  #1000
./t4 -sv tbo/tbo/fpchkerrrpsn.tbo      inp/inp/fpu_1sn.bin tmp/fpchkerrrpsn.sim     #1000
./t4 -sv tbo/tbo/fptesterrrpsn.tbo     inp/inp/fpu_1sn.bin tmp/fptesterrrpsn.sim    #1000
./t4 -sv tbo/tbo/fpuseterrrpsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuseterrrpsn.sim    #1000
./t4 -sv tbo/tbo/fpuclrerrrpsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuclrerrrpsn.sim    #1000
./t4 -sv tbo/tbo/fpgtrpsn.tbo          inp/inp/fpu_2sn.bin tmp/fpgtrpsn.sim        #80000
./t4 -sv tbo/tbo/fpeqrpsn.tbo          inp/inp/fpu_2sn.bin tmp/fpeqrpsn.sim        #80000
./t4 -sv tbo/tbo/fporderedrpsn.tbo     inp/inp/fpu_2sn.bin tmp/fporderedrpsn.sim   #80000
./t4 -sv tbo/tbo/fpnanrpsn.tbo         inp/inp/fpu_1sn.bin tmp/fpnanrpsn.sim        #1000
./t4 -sv tbo/tbo/fpnotfiniterpsn.tbo   inp/inp/fpu_1sn.bin tmp/fpnotfiniterpsn.sim  #1000
./t4 -sv tbo/tbo/fpintrpsn.tbo         inp/inp/fpu_1sn.bin tmp/fpintrpsn.sim        #1000
./t4 -sv tbo/tbo/fpuchki32rpsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuchki32rpsn.sim    #1000
./t4 -sv tbo/tbo/fpuchki64rpsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuchki64rpsn.sim    #1000
./t4 -sv tbo/tbo/fprtoi32rpsn.tbo      inp/inp/fpu_1sn.bin tmp/fprtoi32rpsn.sim     #1000

./t4 -sv tbo/tbo/fpduprmsn.tbo         inp/inp/fpu_1sn.bin tmp/fpduprmsn.sim        #1000
./t4 -sv tbo/tbo/fprevrmsn.tbo         inp/inp/fpu_1sn.bin tmp/fprevrmsn.sim        #1000
./t4 -sv tbo/tbo/fpurnrmsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurnrmsn.sim        #1000
./t4 -sv tbo/tbo/fpurzrmsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurzrmsn.sim        #1000
./t4 -sv tbo/tbo/fpurprmsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurprmsn.sim        #1000
./t4 -sv tbo/tbo/fpurmrmsn.tbo         inp/inp/fpu_1sn.bin tmp/fpurmrmsn.sim        #1000
./t4 -sv tbo/tbo/fpaddrmsn.tbo         inp/inp/fpu_2sn.bin tmp/fpaddrmsn.sim       #80000
./t4 -sv tbo/tbo/fpsubrmsn.tbo         inp/inp/fpu_2sn.bin tmp/fpsubrmsn.sim       #80000
./t4 -sv tbo/tbo/fpmulrmsn.tbo         inp/inp/fpu_2sn.bin tmp/fpmulrmsn.sim       #80000
./t4 -sv tbo/tbo/fpdivrmsn.tbo         inp/inp/fpu_2sn.bin tmp/fpdivrmsn.sim       #80000
./t4 -sv tbo/tbo/fpuabsrmsn.tbo        inp/inp/fpu_1sn.bin tmp/fpuabsrmsn.sim       #1000
./t4 -sv tbo/tbo/fpumulby2rmsn.tbo     inp/inp/fpu_1sn.bin tmp/fpumulby2rmsn.sim    #1000
./t4 -sv tbo/tbo/fpudivby2rmsn.tbo     inp/inp/fpu_1sn.bin tmp/fpudivby2rmsn.sim    #1000
./t4 -sv tbo/tbo/fpuexpinc32rmsn.tbo   inp/inp/fpu_1sn.bin tmp/fpuexpinc32rmsn.sim  #1000
./t4 -sv tbo/tbo/fpuexpdec32rmsn.tbo   inp/inp/fpu_1sn.bin tmp/fpuexpdec32rmsn.sim  #1000
./t4 -sv tbo/tbo/fpchkerrrmsn.tbo      inp/inp/fpu_1sn.bin tmp/fpchkerrrmsn.sim     #1000
./t4 -sv tbo/tbo/fptesterrrmsn.tbo     inp/inp/fpu_1sn.bin tmp/fptesterrrmsn.sim    #1000
./t4 -sv tbo/tbo/fpuseterrrmsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuseterrrmsn.sim    #1000
./t4 -sv tbo/tbo/fpuclrerrrmsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuclrerrrmsn.sim    #1000
./t4 -sv tbo/tbo/fpgtrmsn.tbo          inp/inp/fpu_2sn.bin tmp/fpgtrmsn.sim        #80000
./t4 -sv tbo/tbo/fpeqrmsn.tbo          inp/inp/fpu_2sn.bin tmp/fpeqrmsn.sim        #80000
./t4 -sv tbo/tbo/fporderedrmsn.tbo     inp/inp/fpu_2sn.bin tmp/fporderedrmsn.sim   #80000
./t4 -sv tbo/tbo/fpnanrmsn.tbo         inp/inp/fpu_1sn.bin tmp/fpnanrmsn.sim        #1000
./t4 -sv tbo/tbo/fpnotfinitermsn.tbo   inp/inp/fpu_1sn.bin tmp/fpnotfinitermsn.sim  #1000
./t4 -sv tbo/tbo/fpintrmsn.tbo         inp/inp/fpu_1sn.bin tmp/fpintrmsn.sim        #1000
./t4 -sv tbo/tbo/fpuchki32rmsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuchki32rmsn.sim    #1000
./t4 -sv tbo/tbo/fpuchki64rmsn.tbo     inp/inp/fpu_1sn.bin tmp/fpuchki64rmsn.sim    #1000
./t4 -sv tbo/tbo/fprtoi32rmsn.tbo      inp/inp/fpu_1sn.bin tmp/fprtoi32rmsn.sim     #1000

./t4 -sv tbo/tbo/fpur32tor64__sn.tbo   inp/inp/fpu_1sn.bin tmp/fpur32tor64__sn.sim  #1800
./t4 -sv tbo/tbo/fpur32tor64rnsn.tbo   inp/inp/fpu_1sn.bin tmp/fpur32tor64rnsn.sim  #1800
./t4 -sv tbo/tbo/fpur32tor64rzsn.tbo   inp/inp/fpu_1sn.bin tmp/fpur32tor64rzsn.sim  #1800
./t4 -sv tbo/tbo/fpur32tor64rpsn.tbo   inp/inp/fpu_1sn.bin tmp/fpur32tor64rpsn.sim  #1800
./t4 -sv tbo/tbo/fpur32tor64rmsn.tbo   inp/inp/fpu_1sn.bin tmp/fpur32tor64rmsn.sim  #1800
./t4 -sv tbo/tbo/fpldzerosn__sn.tbo    inp/inp/fpu_1sn.bin tmp/fpldzerosn__sn.sim   #1000
./t4 -sv tbo/tbo/fpldzerosnrnsn.tbo    inp/inp/fpu_1sn.bin tmp/fpldzerosnrnsn.sim   #1000
./t4 -sv tbo/tbo/fpldzerosnrzsn.tbo    inp/inp/fpu_1sn.bin tmp/fpldzerosnrzsn.sim   #1000
./t4 -sv tbo/tbo/fpldzerosnrpsn.tbo    inp/inp/fpu_1sn.bin tmp/fpldzerosnrpsn.sim   #1000
./t4 -sv tbo/tbo/fpldzerosnrmsn.tbo    inp/inp/fpu_1sn.bin tmp/fpldzerosnrmsn.sim   #1000
./t4 -sv tbo/tbo/fpusqrt__sn.tbo       inp/inp/fpu_1sn.bin tmp/fpusqrt__sn.sim      #1000
./t4 -sv tbo/tbo/fpusqrtrnsn.tbo       inp/inp/fpu_1sn.bin tmp/fpusqrtrnsn.sim      #1000
./t4 -sv tbo/tbo/fpusqrtrzsn.tbo       inp/inp/fpu_1sn.bin tmp/fpusqrtrzsn.sim      #1000
./t4 -sv tbo/tbo/fpusqrtrpsn.tbo       inp/inp/fpu_1sn.bin tmp/fpusqrtrpsn.sim      #1000
./t4 -sv tbo/tbo/fpusqrtrmsn.tbo       inp/inp/fpu_1sn.bin tmp/fpusqrtrmsn.sim      #1000

./t4 -sv tbo/tbo/fpi32tor32__sn.tbo    inp/inp/fpumisn.bin tmp/fpi32tor32__sn.sim   #3000
./t4 -sv tbo/tbo/fpldnlsn__sn.tbo      inp/inp/fpum1sn.bin tmp/fpldnlsn__sn.sim     #1800
./t4 -sv tbo/tbo/fpldnladdsn__sn.tbo   inp/inp/fpum2sn.bin tmp/fpldnladdsn__sn.sim #C0000
./t4 -sv tbo/tbo/fpldnlmulsn__sn.tbo   inp/inp/fpum2sn.bin tmp/fpldnlmulsn__sn.sim #C0000
./t4 -sv tbo/tbo/fpstnlsn__sn.tbo      inp/inp/fpum1sn.bin tmp/fpstnlsn__sn.sim     #1800
./t4 -sv tbo/tbo/fpstnli32__sn.tbo     inp/inp/fpum1sn.bin tmp/fpstnli32__sn.sim    #1800
./t4 -sv tbo/tbo/fpi32tor32rnsn.tbo    inp/inp/fpumisn.bin tmp/fpi32tor32rnsn.sim   #3000
./t4 -sv tbo/tbo/fpldnlsnrnsn.tbo      inp/inp/fpum1sn.bin tmp/fpldnlsnrnsn.sim     #1800
./t4 -sv tbo/tbo/fpldnladdsnrnsn.tbo   inp/inp/fpum2sn.bin tmp/fpldnladdsnrnsn.sim #C0000
./t4 -sv tbo/tbo/fpldnlmulsnrnsn.tbo   inp/inp/fpum2sn.bin tmp/fpldnlmulsnrnsn.sim #C0000
./t4 -sv tbo/tbo/fpstnlsnrnsn.tbo      inp/inp/fpum1sn.bin tmp/fpstnlsnrnsn.sim     #1800
./t4 -sv tbo/tbo/fpstnli32rnsn.tbo     inp/inp/fpum1sn.bin tmp/fpstnli32rnsn.sim    #1800
./t4 -sv tbo/tbo/fpi32tor32rzsn.tbo    inp/inp/fpumisn.bin tmp/fpi32tor32rzsn.sim   #3000
./t4 -sv tbo/tbo/fpldnlsnrzsn.tbo      inp/inp/fpum1sn.bin tmp/fpldnlsnrzsn.sim     #1800
./t4 -sv tbo/tbo/fpldnladdsnrzsn.tbo   inp/inp/fpum2sn.bin tmp/fpldnladdsnrzsn.sim #C0000
./t4 -sv tbo/tbo/fpldnlmulsnrzsn.tbo   inp/inp/fpum2sn.bin tmp/fpldnlmulsnrzsn.sim #C0000
./t4 -sv tbo/tbo/fpstnlsnrzsn.tbo      inp/inp/fpum1sn.bin tmp/fpstnlsnrzsn.sim     #1800
./t4 -sv tbo/tbo/fpstnli32rzsn.tbo     inp/inp/fpum1sn.bin tmp/fpstnli32rzsn.sim    #1800
./t4 -sv tbo/tbo/fpi32tor32rpsn.tbo    inp/inp/fpumisn.bin tmp/fpi32tor32rpsn.sim   #3000
./t4 -sv tbo/tbo/fpldnlsnrpsn.tbo      inp/inp/fpum1sn.bin tmp/fpldnlsnrpsn.sim     #1800
./t4 -sv tbo/tbo/fpldnladdsnrpsn.tbo   inp/inp/fpum2sn.bin tmp/fpldnladdsnrpsn.sim #C0000
./t4 -sv tbo/tbo/fpldnlmulsnrpsn.tbo   inp/inp/fpum2sn.bin tmp/fpldnlmulsnrpsn.sim #C0000
./t4 -sv tbo/tbo/fpstnlsnrpsn.tbo      inp/inp/fpum1sn.bin tmp/fpstnlsnrpsn.sim     #1800
./t4 -sv tbo/tbo/fpstnli32rpsn.tbo     inp/inp/fpum1sn.bin tmp/fpstnli32rpsn.sim    #1800
./t4 -sv tbo/tbo/fpi32tor32rmsn.tbo    inp/inp/fpumisn.bin tmp/fpi32tor32rmsn.sim   #3000
./t4 -sv tbo/tbo/fpldnlsnrmsn.tbo      inp/inp/fpum1sn.bin tmp/fpldnlsnrmsn.sim     #1800
./t4 -sv tbo/tbo/fpldnladdsnrmsn.tbo   inp/inp/fpum2sn.bin tmp/fpldnladdsnrmsn.sim #C0000
./t4 -sv tbo/tbo/fpldnlmulsnrmsn.tbo   inp/inp/fpum2sn.bin tmp/fpldnlmulsnrmsn.sim #C0000
./t4 -sv tbo/tbo/fpstnlsnrmsn.tbo      inp/inp/fpum1sn.bin tmp/fpstnlsnrmsn.sim     #1800
./t4 -sv tbo/tbo/fpstnli32rmsn.tbo     inp/inp/fpum1sn.bin tmp/fpstnli32rmsn.sim    #1800
