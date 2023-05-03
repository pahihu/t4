#ifdef NDEBUG
#define T4DEBUG(x)
#else
#define T4DEBUG(x)      x
#endif

#ifdef EMUDEBUG
#define EMUDBG(x)       if(emudebug)printf(x)
#define EMUDBG2(x,y)    if(emudebug)printf(x,y)
#define EMUDBG3(x,y,z)  if(emudebug)printf(x,y,z)
#define EMUDBG5(a1,a2,a3,a4,a5)  if(emudebug)printf(a1,a2,a3,a4,a5)
#define MSGDBG(x)       if(msgdebug||emudebug)printf(x)
#define MSGDBG2(x,y)    if(msgdebug||emudebug)printf(x,y)
#define MSGDBG3(x,y,z)  if(msgdebug||emudebug)printf(x,y,z)
#define MSGDBG4(a1,a2,a3,a4) if(msgdebug||emudebug)printf(a1,a2,a3,a4)
#else
#define EMUDBG(x)
#define EMUDBG2(x,y)
#define EMUDBG3(x,y,z)
#define EMUDBG5(a1,a2,a3,a4,a5)
#define MSGDBG(x)
#define MSGDBG2(x,y)
#define MSGDBG3(x,y,z)
#define MSGDBG4(a1,a2,a3,a4)
#endif