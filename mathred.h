#ifndef _MATHRED_H
#define _MATHRED_H

#include <stdint.h>

double fdm_ldexp(double,int);
float  fdm_ldexpf(float,int);
double fdm_scalbn(double,int);
float  fdm_scalbnf(float,int);

#define EXTRACT_WORDS(hi,lo,x) \
{ \
        union { \
                uint64_t bits; \
                double fp; \
        } r64; \
        r64.fp = (x); \
        lo = r64.bits & 0xffffffffULL; \
        hi = (r64.bits >> 32) & 0xffffffffULL; \
}

#define GET_HIGH_WORD(hi,x) \
{ \
        union { \
                uint64_t bits; \
                double fp; \
        } r64; \
        r64.fp = (x); \
        hi = (r64.bits >> 32) & 0xffffffffULL; \
}

#define SET_HIGH_WORD(x,hi) \
{ \
        union { \
                uint64_t bits; \
                double fp; \
        } r64; \
        r64.fp = x; \
        r64.bits = (((uint64_t) (hi)) << 32) | (r64.bits & 0xffffffffULL); \
        x = r64.fp; \
}

#define GET_FLOAT_WORD(ix,x) \
{ \
        union { \
                int32_t bits; \
                float fp; \
        } r32; \
        r32.fp = (x); \
        ix = r32.bits; \
}

#define SET_FLOAT_WORD(x,ix) \
{ \
        union { \
                int32_t bits; \
                float fp; \
        } r32; \
        r32.bits = (ix); \
        x = r32.fp; \
}

#endif
