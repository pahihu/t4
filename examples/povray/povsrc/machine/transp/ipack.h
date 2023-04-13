#define LITTLE_ENDIAN
/*
#define BIG_ENDIAN
*/

#define MAKE_LENGTH\
        {int i; Length = out_count + (out_count % 2);\
         for (i=0;out_count < 8; out_count++)\
          *out_ptr++ = '\0';}


/*
 *   PACKING ROUTINES
 */


/*
 *   pack one byte with int x
 */

#define PUT_BYTE(x) {*out_ptr++ = x; out_count++;}


/*
 *  pack lower 16 bits of int x into 2 bytes
 */

#ifdef BIG_ENDIAN
#define PUT_INT16(x)\
        {BYTE *c; c=(BYTE *)&x+(sizeof(int)-1);\
        *out_ptr++ = *c--; *out_ptr++ = *c--; out_count += 2;}
#else
#define PUT_INT16(x)\
        {BYTE *c; c=(BYTE *)&x; *out_ptr++ = *c++;\
        *out_ptr++ = *c; out_count += 2;}
#endif


/*
 *  pack int32 into 4 bytes
 */

#ifdef BIG_ENDIAN
#define PUT_INT32(x)\
        {BYTE *c; c=(BYTE *)&x+(sizeof(INT32)-1);\
        *out_ptr++ = *c--; *out_ptr++ = *c--;\
        *out_ptr++ = *c--; *out_ptr++ = *c; out_count += 4;}
#else
#define PUT_INT32(x)\
        {BYTE *c; c=(BYTE *)&x; *out_ptr++ = *c++;\
        *out_ptr++ = *c++; *out_ptr++ = *c++; *out_ptr++ = *c; out_count += 4;}
#endif


/*
 *  pack FILE *x as 4 bytes
 */

#define PUT_FD(x) PUT_INT32(x)


/*
 *  pack lower 16 bits of int x as 2 bytes and then pack x bytes from y
 */

#ifdef BIG_ENDIAN
#define PUT_SLICE(x,y)\
        {int i; BYTE *c; c=(BYTE *)&x+(sizeof(int)-1);\
        *out_ptr++ = *c--; *out_ptr++ = *c--; c=(BYTE *)y;\
        for( i=0; i<x; i++ ) *out_ptr++ = *c++; out_count += x+2;}
#else
#define PUT_SLICE(x,y)\
        {int i; BYTE *c; c=(BYTE *)&x; *out_ptr++ = *c++;\
        *out_ptr++ = *c; c=(BYTE *)y;\
        for( i=0; i<x; i++ ) *out_ptr++ = *c++; out_count += x+2; }
#endif


/*
 *  pack lower 16 bits of int x into bytes 0 and 1 of Tbuf
 */


/*
 *   UNPACKING ROUTINES
 */


/*
 *  unpack one byte into int x
 */

#define GET_BYTE(x) x = *in_ptr++


/*
 *  unpack 2 bytes into int x
 */

#ifdef BIG_ENDIAN
#define GET_INT16(x)\
        { x = *in_ptr++; x += 256*(*in_ptr++); }
#else
#define GET_INT16(x)\
        {BYTE *c; x=0; c=(BYTE *)&x; *c++ = *in_ptr++; *c = *in_ptr++; }
#endif


/*
 *  unpack 4 bytes into int32 x
 */

#ifdef BIG_ENDIAN
#define GET_INT32(x)\
        {BYTE *c; c=((BYTE *)&x)+(sizeof(INT32)-1);\
        *c-- = *in_ptr++; *c-- = *in_ptr++; *c-- = *in_ptr++; *c = *in_ptr++; }
#else
#define GET_INT32(x)\
        {BYTE *c; c=(BYTE *)&x; *c++ = *in_ptr++;\
        *c++ = *in_ptr++; *c++ = *in_ptr++; *c = *in_ptr++; }
#endif


/*
 *  unpack 4 bytes into FILE *x
 */

#define GET_FD(x) GET_INT32(x)


/*
 *  unpack 2 bytes into int x and then x bytes into array y
 */

#define GET_SLICE(x,y)\
        {int i; BYTE *j; j=y; x = *in_ptr++; x += (256*(*in_ptr++));\
        for( i=0; i<x; i++ ) *j++ = *in_ptr++; }


/*
 *   Eof
 */
