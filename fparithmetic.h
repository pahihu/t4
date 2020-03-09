/*
 * fparithmetic.h
 *
 * Function prototypes and constants for fparithmetic.c
 *
 */
#include <stdint.h>

typedef float   REAL32;
typedef double  REAL64;

typedef union {
        REAL64   fp;
        uint64_t bits;
} fpreal64_t;

typedef union {
        REAL32   fp;
        uint32_t bits;
} fpreal32_t;

#define PINFINITY64             ((uint64_t)0x7ff0000000000000LL)
#define NINFINITY64             ((uint64_t)0xfff0000000000000LL)
#define NAN64_UNDEFINED         ((uint64_t)0x7ff0000000000010LL)
#define NAN64_UNSTABLE          ((uint64_t)0x7ff0000000000008LL)
#define NAN64_INEXACT           ((uint64_t)0x7ff0000000000004LL)
#define NAN64_CONVERSION        ((uint64_t)0x7ff0000000000002LL)
#define ZERO64                  ((uint64_t)0x0000000000000000LL)


#define ROUND_P         1
#define ROUND_M         2
#define ROUND_Z         3
#define ROUND_N         4

void fp_setrounding (int mode);

/*
 * REAL64 operations.
 */
int fp_signdb (REAL64);
int fp_expdb  (REAL64);
uint64_t fp_fracdb (REAL64);
int fp_nandb  (REAL64);
int fp_infdb  (REAL64);

REAL64 fp_adddb (REAL64, REAL64);
REAL64 fp_subdb (REAL64, REAL64);
REAL64 fp_muldb (REAL64, REAL64);
REAL64 fp_divdb (REAL64, REAL64);
REAL64 fp_mulby2db (REAL64);
REAL64 fp_divby2db (REAL64);
REAL64 fp_expinc32db (REAL64);
REAL64 fp_expdec32db (REAL64);
REAL64 fp_absdb (REAL64);
REAL64 fp_sqrtdb (REAL64);
REAL64 fp_remfirstdb (REAL64, REAL64);
int    fp_notfinitedb (REAL64);
int    fp_gtdb (REAL64, REAL64);
int    fp_eqdb (REAL64, REAL64);
int    fp_ordereddb (REAL64, REAL64);


/*
 * REAL32 operations.
 */
int fp_signsn (REAL32);
int fp_expsn  (REAL32);
uint32_t fp_fracsn (REAL32);
int fp_nansn  (REAL32);
int fp_infsn  (REAL32);

REAL32 fp_addsn (REAL32, REAL32);
REAL32 fp_subsn (REAL32, REAL32);
REAL32 fp_mulsn (REAL32, REAL32);
REAL32 fp_divsn (REAL32, REAL32);
REAL32 fp_mulby2sn (REAL32);
REAL32 fp_divby2sn (REAL32);
REAL32 fp_expinc32sn (REAL32);
REAL32 fp_expdec32sn (REAL32);
REAL32 fp_abssn (REAL32);
REAL32 fp_sqrtsn (REAL32);
REAL32 fp_remfirstsn (REAL32, REAL32);
int    fp_notfinitesn (REAL32);
int    fp_gtsn (REAL32, REAL32);
int    fp_eqsn (REAL32, REAL32);
int    fp_orderedsn (REAL32, REAL32);

