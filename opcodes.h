char* Primaries[] = {
        "J",   "LDLP",  "PFIX", "LDNL",
        "LDC", "LDNLP", "NFIX", "LDL",
        "ADC", "CALL",  "CJ",   "AJW",
        "EQC", "STL",   "STNL", "OPR"
};

char* Secondaries[] = {
/*00*/  "REV",          "LB",              "BSUB",         "ENDP",
/*04*/  "DIFF",         "ADD",             "GCALL",        "IN",
/*08*/  "PROD",         "GT",              "WSUB",         "OUT",
/*0C*/  "SUB",          "STARTP",          "OUTBYTE",      "OUTWORD",

/*10*/  "SETERR",       "--11--",          "RESETCH",      "CSUB0",
/*14*/  "--14--",       "STOPP",           "LADD",         "STLB",
/*18*/  "STHF",         "NORM",            "LDIV",         "LDPI",
/*1C*/  "STLF",         "XDBLE",           "LDPRI",        "REM",

/*20*/ "RET",           "LEND",            "LDTIMER",      "--23--",
/*24*/ "TESTLDE",       "TESTLDD",         "--26--",       "TESTSTE",
/*28*/ "TESTSTD",       "TESTERR",         "TESTPRANAL",   "TIN",
/*2C*/ "DIV",           "--2D--",          "DIST",         "DISC",

/*30*/ "DISS",          "LMUL",            "NOT",          "XOR",
/*34*/ "BCNT",          "LSHR",            "LSHL",         "LSUM",
/*38*/ "LSUB",          "RUNP",            "XWORD",        "SB",
/*3C*/ "GAJW",          "SAVEL",           "SAVEH",        "WCNT",

/*40*/ "SHR",           "SHL",             "MINT",         "ALT",
/*44*/ "ALTWT",         "ALTEND",          "AND",          "ENBT",
/*48*/ "ENBC",          "ENBS",            "MOVE",         "OR",
/*4C*/ "CSNGL",         "CCNT1",           "TALT",         "LDIFF",

/*50*/ "STHB",          "TALTWT",          "SUM",          "MUL",
/*54*/ "STTIMER",       "STOPERR",         "CWORD",        "CLRHALTERR",
/*58*/ "SETHALTERR",    "TESTHALTERR",     "DUP",          "MOVE2DINIT",
/*5C*/  "MOVE2DALL",    "MOVE2DNONZERO",   "MOVE2DZERO",   "--5F--",

/*60*/ "--60--",        "--61--",          "--62--",       "UNPACKSN",
/*64*/ "--64--",        "--65--",          "--66--",       "--67--",
/*68*/ "--68--",        "--69--",          "--6A--",       "--6B--",
/*6C*/ "POSTNORMSN",    "ROUNDSN",         "--6E--",       "--6F--",

/*70*/ "--70--",        "LDINF",           "FMUL",         "CFLERR",
/*74*/ "CRCWORD",       "CRCBYTE",         "BITCNT",       "BITREVWORD",
/*78*/ "BITREVNBITS",   "--79--",          "--7A--",       "--7B--",
/*7C*/ "--7C--",        "--7D--",          "--7E--",       "--7F--",

/*80*/ "FPSTTEST",      "WSUBDB",          "FPLDNLDBI",    "FPCHKERR",
/*84*/ "FPSTNLDB",      "FPLDTEST",        "FPLDNLSNI",    "FPADD",
/*88*/ "FPSTNLSN",      "FPSUB",           "FPLDNLDB",     "FPMUL",
/*8C*/ "FPDIV",         "--8D--",          "FPLDNLSN",     "FPREMFIRST",

/*90*/ "FPREMSTEP",     "FPNAN",           "FPORDERED",    "FPNOTFINITE",
/*94*/ "FPGT",          "FPEQ",            "FPI32TOR32",   "--97--",
/*98*/ "FPI32TOR64",    "--99--",          "FPB32TOR64",   "--9B--",
/*9C*/ "FPTESTERR",     "FPRTOI32",        "FPSTNLI32",    "FPLDZEROSN",

/*A0*/ "FPLDZERODB",    "FPINT",           "--A2--",       "FPDUP",
/*A4*/ "FPREV",         "--A5--",          "FPLDNLADDDB",  "--A7--",
/*A8*/ "FPLDNLMULDB",   "--A9--",          "FPLDNLADDSN",  "FPENTRY",
/*AC*/ "FPLDNLMULSN",   "--AD--",          "--AE--",       "--AF--",

/*17C*/ "LDDEVID",
/*1FF*/ "START"
};
#define MAX_SECONDARIES 0xaf


char* FpuEntries[] = {
/*00*/ "--FPU00--",   "FPUSQRTFIRST", "FPUSQRTSTEP", "FPUSQRTLAST",
/*04*/ "FPURP",       "FPURM",        "FPURZ",       "FPUR32TOR64",
/*08*/ "FPUR64TOR32", "FPUEXPDEC32",  "FPUEXPINC32", "FPUABS",
/*0C*/ "--FPU0C--",   "FPUNOROUND",   "FPUCHKI32",   "FPUCHKI64",
/*10*/ "--FPU10--",   "FPUDIVBY2",    "FPUMULBY2",   "--FPU13--",

/*22*/ "FPURN",
/*23*/ "FPUSETERR",
/*9C*/ "FPUCLRERR"
};
#define MAX_FPUENTRIES 0x13


char* Combinations[] = {
/*100*/ "STL+LDL",       "LDL+LDNL",      "EQC+CJ",     "LDL+LDNLP",
/*104*/ "LDL+LDL",       "LDLP+FPLDNLDB", "AJW+RET",    "LDLP+LDC",
/*108*/ "LDLP+LDL",      "STL+STL",       "LDL+CFLERR", "LDLP+FPSTNLSN",
/*10C*/ "LDLP+FPLDNLSN", "LDL+ADC",       "LDL+STNL",   "LDC+LDL",
/*110*/ "STL+J",         "GT+CJ",         "WSUB+STNL",  "LDL+WSUB"
};
#define MIN_COMBINATIONS 0x100
#define MAX_COMBINATIONS 0x113
