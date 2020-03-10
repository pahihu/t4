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
#define MINFINITY64             ((uint64_t)0xfff0000000000000LL)
#define NAN64_UNDEFINED         ((uint64_t)0x7ff0000000000010LL)
#define NAN64_UNSTABLE          ((uint64_t)0x7ff0000000000008LL)
#define NAN64_INEXACT           ((uint64_t)0x7ff0000000000004LL)
#define ZERO64                  ((uint64_t)0x0000000000000000LL)
#define NAN32_CONVERSION64      ((uint32_t)0x7f800002)


/*
 * REAL32 constants.
 */
extern REAL32 Real32PlusInf;
extern REAL32 Real32MinusInf;
extern REAL32 Real32UndefinedNaN;
extern REAL32 Real32UnstableNaN;
extern REAL32 Real32InexactNaN;
extern REAL32 Real32Zero;


/*
 * REAL64 constants.
 */
extern REAL64 Real64PlusInf;
extern REAL64 Real64MinusInf;
extern REAL64 Real64UndefinedNaN;
extern REAL64 Real64UnstableNaN;
extern REAL64 Real64InexactNaN;
extern REAL64 Real64Zero;
extern REAL32 Real32ConversionNaN;


/*
 * Floating point initialization and exception handling.
 */
void fp_init (void);
void fp_chkexcept (char *msg);
void fp_clrexcept (void);


/*
 * Rounding mode.
 */
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
REAL64 fp_r32tor64 (REAL32);
REAL64 fp_intdb (REAL64);
int    fp_chki32db (REAL64);
int    fp_chki64db (REAL64);
REAL64 fp_rtoi32db (REAL64);
REAL32 fp_norounddb (REAL64);


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
REAL32 fp_r64tor32 (REAL64);
REAL32 fp_intsn (REAL32);
int    fp_chki32sn (REAL32);
int    fp_chki64sn (REAL32);
REAL32 fp_rtoi32sn (REAL32);

