#ifdef T4RELEASE
#undef EMUDEBUG
#define NDEBUG          1
#else
#define EMUDEBUG        1
#undef NDEBUG
#endif

#ifdef NDEBUG
#define T4DEBUG(x)
#else
#define T4DEBUG(x)      x
#endif

#ifdef T4PROFILE
#define PROFILE(x)      if(profiling)x
#else
#define PROFILE(x)
#endif

#ifdef EMUDEBUG
#define BADCODE(x)      if(x)goto BadCode;
#define EMUDBG(x)       if(emudebug)printf(x)
#define EMUDBG2(x,y)    if(emudebug)printf(x,y)
#define EMUDBG3(x,y,z)  if(emudebug)printf(x,y,z)
#define EMUDBG4(a1,a2,a3,a4)  if(emudebug)printf(a1,a2,a3,a4)
#define EMUDBG5(a1,a2,a3,a4,a5)  if(emudebug)printf(a1,a2,a3,a4,a5)
#define MSGDBG(x)       if(msgdebug||emudebug)printf(x)
#define MSGDBG2(x,y)    if(msgdebug||emudebug)printf(x,y)
#define MSGDBG3(x,y,z)  if(msgdebug||emudebug)printf(x,y,z)
#define MSGDBG4(a1,a2,a3,a4) if(msgdebug||emudebug)printf(a1,a2,a3,a4)
#else
#define BADCODE(x)
#define EMUDBG(x)
#define EMUDBG2(x,y)
#define EMUDBG3(x,y,z)
#define EMUDBG4(a1,a2,a3,a4)
#define EMUDBG5(a1,a2,a3,a4,a5)
#define MSGDBG(x)
#define MSGDBG2(x,y)
#define MSGDBG3(x,y,z)
#define MSGDBG4(a1,a2,a3,a4)
#endif
