/*
 * gettimeofday function for Windows
 *
 * http://www.cpp-programming.net/c-tidbits/gettimeofday-function-for-windows/
 *
 */
#include "gettimeofday.h"
#include <time.h>

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  116444736000000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  116444736000000000ULL
#endif

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag;

  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);

    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    /*converting file time to unix epoch*/
    tmpres /= 10;  /*convert into microseconds*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }

  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
    tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }

  return 0;
}

void sleep(int seconds)
{
  Sleep (1000 * seconds);
}


static double Freq;

void usleep(int useconds)
{
  static int init = 1;
  LARGE_INTEGER start, now;
  int counter = 0;

  if (init)
  {
    LARGE_INTEGER perfFreq;
    QueryPerformanceFrequency(&perfFreq);
    Freq = perfFreq.QuadPart;
    init = 0;
  }

  QueryPerformanceCounter(&start);
  do
  {
    if (0 == (++counter & 31)) _mm_pause ();
    QueryPerformanceCounter((LARGE_INTEGER*) &now);
  } while ((now.QuadPart - start.QuadPart) * 1000.0 * 1000.0 / Freq < useconds);
}
