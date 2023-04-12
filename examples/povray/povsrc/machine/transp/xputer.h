/* xputer.h 7.11.1992z */

#define SP_PIXEL_TAG   128
#define SP_USER_TAG    128
#define SP_TAG_SIZE    1
#define SP_LENGTH_SIZE 2
#define SP_SIZE (SP_TAG_SIZE+SP_LENGTH_SIZE)
#define MAX_LINE_SIZE  1024
#define SP_CACHED 254

#ifndef TRUE
#define TRUE (0 == 0)
#endif

#ifndef FALSE
#define FALSE !TRUE
#endif

typedef int   BOOL;
typedef char  BYTE;
typedef short INT16;
typedef long  INT32;

