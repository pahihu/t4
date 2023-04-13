/* muxdemux.c */
#define VERSION "V1.1[15.11.1992z]"
/*{{{  definitions*/
#define VRAM_BASE          0x80800000   /* T800-Address to access top_left */
#define MAXX 640
#define MAXY 480

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef START_TIME
#define START_TIME time(&tstart);
#endif

#ifndef STOP_TIME
#define STOP_TIME  time(&tstop);
#endif

#ifndef TIME_ELAPSED
#define TIME_ELAPSED difftime (tstop, tstart);
#endif


#define UNCACHED (Data==NULL)

#define SPR_OK 0
#define SPS_SUCCESS 999999999

/*{{{  mode defs*/
#define BINARY_1 "rb"
#define BINARY_2 "wb"
#define BINARY_3 "ab"
#define BINARY_4 "r+b"
#define BINARY_5 "w+b"
#define BINARY_6 "a+b"
#define TEXT_1 "rt"
#define TEXT_2 "wt"
#define TEXT_3 "at"
#define TEXT_4 "r+t"
#define TEXT_5 "w+t"
#define TEXT_6 "a+t"
/*}}}  */
/*}}}  */
/*{{{  include files*/
#include <stdio.h>
#include <channel.h>
#include <stdlib.h>
#include <string.h>
#include <misc.h>
#include <time.h>
#include <process.h>

#include "xputer.h"
#include "fcache.h"
#include "iserver.h"
#include "ipack.h"

/*}}}  */
/*{{{  global vars*/
Channel *HostInput   = NULL;
Channel *HostOutput  = NULL;

BOOL CocoPops;                                              /*  for DEBUG  */

INT32 gl_handle;
BYTE Buffer[MAX_LINE_SIZE * sizeof(int) + 1+4+4];

time_t tstart, tstop;
double tused;
/*}}}  */
/*{{{  static procedures*/
/*{{{  chan_chan**  CreateChannels*/
static Channel ** CreateChannels (Channel *OldChannels[], int ChannelsSize)
{
    Channel **NewChannels = NULL;

    if ((NewChannels = malloc((ChannelsSize + 1) * sizeof(Channel *))) == NULL)
        abort();
    else
    {
        int ChannelsCount = 0;

        while (ChannelsCount++ < ChannelsSize)
            NewChannels[ChannelsCount - 1] = OldChannels[ChannelsCount - 1];

        NewChannels[ChannelsSize] = NULL;
    }
    return(NewChannels);
}
/*}}}  */
/*{{{  result_int   SpExit*/
static int SpExit (int Status)
{
int Length = SP_MIN_PACKET_DATA_SIZE;
BYTE Buffer[SP_MIN_PACKET_DATA_SIZE];

  Buffer[0] = SP_EXIT;

  memcpy(&Buffer[1], &Status, 4); /* Store exit status */

  ChanOut(HostOutput, &Length, 2);
  ChanOut(HostOutput, Buffer, Length);

  ChanIn(HostInput, &Length, 2);
  ChanIn(HostInput, Buffer, Length);

  return(Buffer[0]);
}
/*}}}  */

/*{{{  result_int   SpSeek*/
int SpSeek(FILE * fd, INT32 offset, INT32 whence)
{
BYTE result;
BYTE *in_ptr, *out_ptr;
int out_count;
int Length;

  in_ptr  = Buffer;
  out_ptr = Buffer;
  out_count = 0;

  PUT_BYTE(SP_SEEK);                        DEBUG(( "SpSeek" ));
  PUT_FD(fd);
  PUT_INT32(offset);
  PUT_INT32(whence);

  MAKE_LENGTH;
  ChanOut(HostOutput, &Length, 2);
  ChanOut(HostOutput, Buffer, Length);

  ChanIn(HostInput, &Length, 2);
  ChanIn(HostInput, Buffer, Length);

  GET_BYTE(result);
  return((int)result);
}
/*}}}  */
/*{{{  pos_long     SpTell*/
INT32 SpTell(FILE * fd)
{
BYTE result;
INT32 pos;
BYTE *in_ptr, *out_ptr;
int out_count;
int Length;

  in_ptr  = Buffer;
  out_ptr = Buffer;
  out_count = 0;

  PUT_BYTE(SP_TELL);                       DEBUG(( "SpTell" ));
  PUT_FD(fd);

  MAKE_LENGTH;
  ChanOut(HostOutput, &Length, 2);
  ChanOut(HostOutput, Buffer, Length);

  ChanIn(HostInput, &Length, 2);
  ChanIn(HostInput, Buffer, Length);

  GET_BYTE(result);
  GET_INT32(pos);                          DEBUG(( "pos %d", (int)pos ));

  return(pos);
}
/*}}}  */
/*{{{  result_int   SpEof*/
int SpEof(FILE * fd)
{
BYTE result;
BYTE *in_ptr, *out_ptr;
int out_count;
int Length;

  in_ptr  = Buffer;
  out_ptr = Buffer;
  out_count = 0;

  PUT_BYTE(SP_EOF);                       DEBUG(( "SpEof" ));
  PUT_FD(fd);

  MAKE_LENGTH;
  ChanOut(HostOutput, &Length, 2);
  ChanOut(HostOutput, Buffer, Length);

  ChanIn(HostInput, &Length, 2);
  ChanIn(HostInput, Buffer, Length);

  GET_BYTE(result);                      DEBUG(( "eof %d", (int)result ));

  return((int)result);
}
/*}}}  */
/*{{{  read_long    SpRead*/
INT32 SpRead(BYTE * data, int size, FILE * fd)
{
BYTE result;
BYTE *in_ptr, *out_ptr;
int out_count;
int Length;
int read;

  in_ptr  = Buffer;
  out_ptr = Buffer;
  out_count = 0;
  read = 0;

  PUT_BYTE(SP_READ);                   DEBUG(( "SpRead"));
  PUT_FD(fd);
  PUT_INT16(size);                     DEBUG(( "%d:%d",(int)fd, (int)size ));

  MAKE_LENGTH;
  ChanOut(HostOutput, &Length, 2);
  ChanOut(HostOutput, Buffer, Length);

  ChanIn(HostInput, &Length, 2);
  result = (BYTE)ChanInChar(HostInput);
  ChanIn(HostInput, &read, 2);
  ChanIn(HostInput, data, read);
                                       DEBUG(( "read %d",(int)read ));
  return((int)read);
}
/*}}}  */
/*{{{  fd_file*     SpOpen*/
FILE * SpOpen(BYTE *name, BYTE type, BYTE mode)
{
BYTE result;
FILE * fd;
int Size = strlen(name);
BYTE *in_ptr, *out_ptr;
int out_count;
int Length;

  in_ptr  = Buffer;
  out_ptr = Buffer;
  out_count = 0;

  PUT_BYTE(SP_OPEN);                      DEBUG(( "SpOpen" ));
  PUT_SLICE(Size, name);
  PUT_BYTE(type);
  PUT_BYTE(mode);

  MAKE_LENGTH;
  ChanOut(HostOutput, &Length, 2);
  ChanOut(HostOutput, Buffer, Length);

  ChanIn(HostInput, &Length, 2);
  ChanIn(HostInput, Buffer, Length);

  GET_BYTE(result);
  GET_FD(fd);                             DEBUG(( "opened fd %d", (int)fd ));
  return(result == SP_SUCCESS ? fd : NULL);
}
/*}}}  */
/*{{{  result_int   SpClose*/
int SpClose(FILE * fd)
{
BYTE result;
BYTE *in_ptr, *out_ptr;
int out_count;
int Length;

  in_ptr  = Buffer;
  out_ptr = Buffer;
  out_count = 0;

  PUT_BYTE(SP_CLOSE);                          DEBUG(( "SpClose" ));
  PUT_FD(fd);

  MAKE_LENGTH;
  ChanOut(HostOutput, &Length, 2);
  ChanOut(HostOutput, Buffer, Length);

  ChanIn(HostInput, &Length, 2);
  ChanIn(HostInput, Buffer, Length);

  GET_BYTE(result);
  return((int)result);
}
/*}}}  */


/*{{{  init_entry(clist, hlist)*/
void init_entry(CacheEntryPtr * clist,
                HandleBufferPtr * hlist)
{
  gl_handle = 10;
  * clist = (CacheEntryPtr) calloc(1, sizeof(CacheEntry));
  * hlist = (HandleBufferPtr) calloc(1, sizeof(HandleBuffer));
}
/*}}}  */
/*{{{  entry_entry* open_cacheentry(list, name, type, mode)*/
CacheEntryPtr open_cacheentry(CacheEntryPtr * list,
                              BYTE * name,
                              BYTE mode,
                              BYTE type)
{
/*{{{  vars*/
CacheEntryPtr now;
CacheEntryPtr entry;
INT32 nread, result;
/*}}}  */

  now = * list;
  while (now->next != NULL)
  {
    /*{{{  scan list; return entry when found;*/
    if (strcmp(name, now->name) == 0)
       return(now);
    now = now->next;
    /*}}}  */
  }

  /*{{{  append entry; return NULL on failure*/
  /*{{{  allocate entry*/
  if ((entry = (CacheEntryPtr) calloc(1, sizeof(CacheEntry))) == NULL)
  {
     return(NULL);
  }
  /*}}}  */
  
  /*{{{  os_fd     (physical open)*/
  /* now->os_fd = fopen(name,mode); */
  
  now->os_fd = SpOpen(name, mode, type);
  if (now->os_fd == NULL)
    return(NULL);
  /*}}}  */
  /*{{{  name      (filename)*/
  if ((now->name = calloc(strlen(name)+1,sizeof(BYTE))) == NULL)
  {
    return(NULL);
  }
  strcpy(now->name,name);
  /*}}}  */
  /*{{{  size      (filesize)*/
  /*
  fseek(now->os_fd, 0, SEEK_END);
  now->size = ftell(now->os_fd);
  rewind(now->os_fd);
  */
  
  SpSeek(now->os_fd, 0, SPO_END);
  now->size = SpTell(now->os_fd);
  SpSeek(now->os_fd, 0, SPO_START);
  /*}}}  */
  /*{{{  filecache (physical read)*/
  /* cache file wenn kleiner als MAX_CACHE_SIZE */
  
  if (now->size < MAX_CACHE_SIZE)
  {
    if ((now->filecache = (BYTE*)calloc((size_t)(now->size + 1),(size_t)sizeof(BYTE))) != NULL)
    {
      /* nread = fread(now->filecache, sizeof(BYTE), now->size, now->os_fd); */
  
      now->size = 0;
      do
      {
        nread = SpRead((now->filecache)+(now->size), SP_READ_BLOCK_SIZE, now->os_fd);
        now->size += nread;
      }
      while (nread==SP_READ_BLOCK_SIZE);
  
      if (now->size == 0)
      {
        now->filecache = NULL;
      }
    }
  }
  /*}}}  */
  /*{{{  close     (physical close)*/
  result = SpClose(now->os_fd);
  /*}}}  */
  /*{{{  fcount    (accesses)*/
  now->fcount = 0;
  /*}}}  */
  /*}}}  */

  now->next = entry;
  entry->prev = now;
  entry->next = NULL;

  return(now);
}
/*}}}  */

/*{{{  handle_file* open_handleentry(list, entry)*/
FILE* open_handleentry(HandleBufferPtr * list,
                       CacheEntryPtr entry)
{
/*{{{  vars*/
HandleBufferPtr handle;
HandleBufferPtr now;
/*}}}  */

  if(entry == NULL)
    return(NULL);
  now = * list;
  while (now->next != NULL)
    /*{{{  scan to end of list*/
    now = now->next;
    /*}}}  */

  /*{{{  append handle; return NULL on failure*/
  /*{{{  allocate*/
  if ((handle = (HandleBufferPtr) calloc(1, sizeof(HandleBuffer))) == NULL)
  {
     return(NULL);
  }
  /*}}}  */
  
  now->my_handle = (FILE*)gl_handle++;
  
  now->my_pos = 0;
  
  now->my_eof = FALSE;
  
  now->my_entry = entry;
  
  now->my_entry->fcount++;
  /*}}}  */

  now->next = handle;
  handle->prev = now;
  handle->next = NULL;

  return(now->my_handle);
}
/*}}}  */

/*{{{  fd_file*     find_handle(list, handle, readcount, cache_buffer)*/
FILE *find_handle(HandleBufferPtr list,
                  FILE* handle,
                  int readcount,
                  int * nread,
                  BYTE* * buffer)
{
/*{{{  vars*/
HandleBufferPtr now;
/*}}}  */

  now = list;
  while (now->next != NULL)
  {
     /*{{{  scan list; return os_fd when found; buffer points to data when CACHED*/
     if (handle == now->my_handle)
     {
       if ((*buffer = (now->my_entry->filecache)) != NULL)
       {
         /*{{{  point to data*/
         * buffer += now->my_pos;    /* seek */
         if (now->my_pos + readcount >= now->my_entry->size)
         {
           *nread = (int)(now->my_entry->size - now->my_pos);
           now->my_pos = now->my_entry->size;
           now->my_eof = TRUE;
         }
         else
         {
           *nread = readcount;
           now->my_pos += readcount;
           now->my_eof = FALSE;
         }
         /*}}}  */
       }
       else
       {
         /*{{{  physical seek*/
         /*fseek(now->my_entry->os_fd, now->my_pos, SEEK_SET); */
         SpSeek(now->my_entry->os_fd, now->my_pos, SPO_START);
         now->my_eof= (SpEof(now->my_entry->os_fd) == SP_SUCCESS) ? TRUE : FALSE;
         /*}}}  */
       }
       return(now->my_entry->os_fd);
     }
     now = now->next;
     /*}}}  */
  }
  * buffer = NULL;
  return(handle);
}
/*}}}  */
/*{{{  pos_long     tell_handle(list, handle, eof)*/
long tell_handle(HandleBufferPtr list,
                 FILE *handle,
                 int *eof)
{
/*{{{  vars*/
HandleBufferPtr now;
/*}}}  */

  now = list;
  while (now->next != NULL)
  {
    /*{{{  scan list;   return SUCCESS when found*/
    if (handle == now->my_handle)
    {
      if (now->my_entry->filecache != NULL)
      {
        now->my_pos = now->my_pos;
      }
      else
      {
        /*now->my_pos = ftell(now->my_entry->os_fd); */
        now->my_pos = SpTell(now->my_entry->os_fd);
      }
      *eof = now->my_eof;
      return(now->my_pos);
    }
    now = now->next;
    /*}}}  */
  }
  return(-1);
}
/*}}}  */
/*{{{  done_int     del_entry(list, handle)*/
int del_handle(HandleBufferPtr * list,
               FILE* handle)
{
/*{{{  vars*/
HandleBufferPtr now, next, prev=NULL;
/*}}}  */

  now = * list;
  next = now->next;
  while (now->next != NULL)
  {
    /*{{{  scan list; return TRUE when found*/
    if (now->my_handle == handle)
    {
      /*{{{  remove handle when found; physical close when count==0; return TRUE*/
      if (now->my_entry->filecache != NULL)
        free(now->my_entry->filecache);
      
      if (--(now->my_entry->fcount) == 0)
      {
        /*{{{  physical close & remove entry*/
        /*fclose(now->my_entry->os_fd); */
        SpClose(now->my_entry->os_fd);
        
        now->my_entry->next->prev = now->my_entry->prev;
        now->my_entry->prev->next = now->my_entry->next;
        
        free(now->my_entry);
        /*}}}  */
      }
      if (prev != NULL)
        prev->next = now->next;
      if (next != NULL)
        next->prev = now->prev;
      free(now);
      return(TRUE);
      /*}}}  */
    }
    prev = now;
    now = prev->next;
    next = now->next;
    /*}}}  */
  }
  return(FALSE);
}
/*}}}  */

/*{{{  statistics(list)*/
void statistics(CacheEntryPtr list)
{
/*{{{  vars*/
CacheEntryPtr now;
int n_files=0, n_handles=0, n_bytes=0;
double n_kilo=0.0;

int hours, min, sec;
/*}}}  */

  printf("\nCacheMux Statistics %s :\n", VERSION);
  printf("Handles    Size Status Name\n");

  /*{{{  scan list*/
  now = list;
  while (now->next != NULL)
  {
    printf("%7d %7ld %s%s\n",
            now->fcount, now->size,
            now->filecache ? "cached " : "       ",
            now->name);
    n_bytes += (int)now->size;
    n_handles += now->fcount;
    n_kilo += (double)now->size * (double)now->fcount;
    n_files++;
    now = now->next;
  }
  /*}}}  */
  /*{{{  calculate totals*/
  n_kilo /= 1024.0;
  hours = (int)tused/3600;
  min = (int)(tused - (double)(hours*3600))/60;
  sec = (int)(tused - (double)(hours*3600 + min*60));
  /*}}}  */

  printf("\n%d files  %d handles  %d %s --> %4.2f %s throughput  %02d:%02d:%02d sec\n\n",
          n_files, n_handles,
          n_bytes > 1024 ? n_bytes / 1024 : n_bytes,
          n_bytes > 1024 ? "KBytes" : "Bytes",
          n_kilo > 1024.0 ? n_kilo / 1024.0 : n_kilo,
          n_kilo > 1024.0 ? "MBytes" : "KBytes",
          hours, min, sec);
}
/*}}}  */
/*{{{  version()*/
void version(void)
{
  printf("\nCache_Multiplexer %s\n", VERSION);
}
/*}}}  */
/*}}}  */

int main (void)
{
/*{{{  vars*/
Channel **Input = NULL, **Output = NULL;

#ifdef G300_SUPPORT
unsigned long * screen = (unsigned long *)VRAM_BASE;
#endif

#ifdef FILE_SUPPORT
    	int r,g,b;
        int i;
        unsigned long *zeile;
        unsigned long pixel;
        FILE *pictureFile;
#endif

int x=0, y=0;
unsigned long *srce;
unsigned long *dest;

int ExitCount;
int LineCount;
int InputSize = 0, OutputSize = 0;
int Index = 0;

BYTE sp;
BYTE *in_ptr, *out_ptr;
int out_count;
int Length;
int Size;
int Pos;
int Feof;
BYTE FileName[80];
BYTE Type;
BYTE Mode;

CacheEntryPtr cachelist;
HandleBufferPtr handlelist;
CacheEntryPtr entry;

FILE *Fd;
FILE *Handle;
BYTE *Data;
int Read;
int dummy=0;
/*}}}  */

    /*{{{  prime*/
    START_TIME
    
#ifdef FILE_SUPPORT
    	if((pictureFile = fopen("picture.ppm","wb")) == NULL)
    	{
    		exit(-1);
    	}
	fprintf(pictureFile,"P6\n%d %d\n%d\n",MAXX,MAXY,255);
#endif    
    init_entry(&cachelist, &handlelist);
    
    HostInput = get_param(1);
    HostOutput = get_param(2);
    
    InputSize = *((int *) get_param(4));
    Input = CreateChannels(get_param(3), InputSize);
    
    OutputSize = *((int *) get_param(6));
    Output = CreateChannels(get_param(5), OutputSize);
    
    CocoPops |= (int)getenv("COCOPOPS");
    
    Length = 0;
    LineCount = MAXY;
    /*}}}  */

    ExitCount = InputSize;
    while ((ExitCount > 0) || (LineCount > 0))
    {
      /*{{{  mutliplex*/
      Index = ProcAltList(Input);
      
      /*{{{  get buffer from node[Index]*/
      ChanIn(Input[Index], &Length, 2);
      ChanIn(Input[Index], Buffer, Length);
      /*}}}  */
      
      in_ptr  = Buffer;
      out_ptr = Buffer;
      out_count = 0;
      
      GET_BYTE(sp);                                          DEBUG(( "=%d=", sp ));
      switch(sp)
      {
        /*{{{  decode protocol*/
        /*{{{  case SP_PIXEL_TAG*/
        case SP_PIXEL_TAG:
          GET_INT32(x);
          GET_INT32(y);

#ifdef G300_SUPPORT        
          dest = (unsigned long *)(screen+(y*MAXX));
          srce = (unsigned long *)(in_ptr);
          _memcpy(screen + (y * MAXX), in_ptr, x * sizeof(int));
#endif        
#ifdef FILE_SUPPORT
       	fseek(pictureFile,(y * MAXX * 3) + 15,SEEK_SET);
       	zeile = (unsigned long *)in_ptr;
       	for(i = 0; i < MAXX; i++)
       	{
       		int a;
       		
		pixel = *zeile;
                r = (pixel >> 8) & 0xff;
                g = (pixel >> 16) & 0xff;
                b = (pixel >> 24) & 0xff;               
                fputc(r,pictureFile);
                fputc(g,pictureFile);
                fputc(b,pictureFile);
                zeile++;
        }
#endif
          LineCount--;                    DEBUG(( "lines left %d", (int)LineCount ));
        
          break;
        /*}}}  */
        /*{{{  case ISERVER_TAGS*/
        case SP_EXIT:
          /*{{{  exit*/
                                               DEBUG(( "SP.EXIT" ));
          ExitCount--;                         DEBUG(( "count %d", (int)ExitCount ));
          
          PUT_BYTE(SPR_OK); /* Successful exit */
          break;
          /*}}}  */
        case SP_OPEN:
          /*{{{  open*/
                                                          DEBUG(( "SP.OPEN" ));
          GET_SLICE( Size, FileName );                    DEBUG(( "size %d", (int)Size ));
          GET_BYTE( Type );                               DEBUG(( "type %d", (int)Type ));
          GET_BYTE( Mode );                               DEBUG(( "mode %d", (int)Mode ));
          *(FileName+Size)=0;                             DEBUG(( "\"%s\"", FileName ));
          
          /*{{{  set open_mode*/
          /*
          if( Type == SPT_BINARY )
            switch( Mode )
            {
              case SPM_INPUT           : strcpy( fmode, BINARY_1 ); break;
              case SPM_OUTPUT          : strcpy( fmode, BINARY_2 ); break;
              case SPM_APPEND          : strcpy( fmode, BINARY_3 ); break;
              case SPM_EXISTING_UPDATE : strcpy( fmode, BINARY_4 ); break;
              case SPM_NEW_UPDATE      : strcpy( fmode, BINARY_5 ); break;
              case SPM_APPEND_UPDATE   : strcpy( fmode, BINARY_6 ); break;
            }
          else
            switch( Mode )
            {
              case SPM_INPUT           : strcpy( fmode, TEXT_1 ); break;
              case SPM_OUTPUT          : strcpy( fmode, TEXT_2 ); break;
              case SPM_APPEND          : strcpy( fmode, TEXT_3 ); break;
              case SPM_EXISTING_UPDATE : strcpy( fmode, TEXT_4 ); break;
              case SPM_NEW_UPDATE      : strcpy( fmode, TEXT_5 ); break;
              case SPM_APPEND_UPDATE   : strcpy( fmode, TEXT_6 ); break;
            }
          */
          /*}}}  */
          switch( Mode )
          {
            case SPM_INPUT: /* read only */
              entry = open_cacheentry(&cachelist, FileName, Type, Mode);
              Fd = (FILE *)open_handleentry(&handlelist, entry);
              if (Fd == NULL)
              {
                PUT_BYTE( SP_ERROR );
              }
              else
              {
                PUT_BYTE( SP_SUCCESS );
                PUT_FD( Fd );                               DEBUG(( "fd %0X", (int)Fd ));
              }break;
            default: /* read_write */
              /*{{{  send buffer to host*/
              ChanOut(HostOutput, &Length, 2);
              ChanOut(HostOutput, Buffer, Length);
              /*}}}  */
              /*{{{  get reply from host*/
              ChanIn(HostInput, &Length, 2);
              ChanIn(HostInput, Buffer, Length);
              /*}}}  */
              break;
          }
          break;
          /*}}}  */
        case SP_CLOSE:
          /*{{{  close*/
                                                                 DEBUG(( "SP.CLOSE" ));
          PUT_BYTE( SP_SUCCESS );
          break;
          /*}}}  */
        case SP_READ:
        case SP_GETBLOCK:
          /*{{{  read*/
                                                      DEBUG(( "SP.READ/.BLOCK" ));
          GET_FD( Fd );                               DEBUG(( "fd %0X", (int)Fd ));
          GET_INT16( Size );                          DEBUG(( "request %d", (int)Size ));
          
          Handle = Fd;
          Fd = find_handle(handlelist, Handle, Size, &Read, &Data);
          if (UNCACHED)
          {
            PUT_BYTE( sp );
            PUT_FD(Fd);
            /*{{{  send buffer to host*/
            ChanOut(HostOutput, &Length, 2);
            ChanOut(HostOutput, Buffer, Length);
            /*}}}  */
            /*{{{  get reply from host*/
            ChanIn(HostInput, &Length, 2);
            ChanIn(HostInput, Buffer, Length);
            /*}}}  */
            out_count = 0;
            tell_handle(handlelist, Handle, &Feof);
          }
          else
          {
            /*{{{  send data-pointer to node*/
            PUT_BYTE( SP_CACHED );
            PUT_INT16( Read );
            PUT_INT32( Data );                        DEBUG(( "read %d", (int)Read ))
            
            /*
            PUT_BYTE( SP_SUCCESS );
            PUT_SLICE( Read, Data );                  DEBUG(( "read %d", (int)Read ))
            */
            /*}}}  */
          }
          break;
          /*}}}  */
        case SP_GETS:
          /*{{{  gets*/
                                                         DEBUG(( "SP.GETS" ));
          GET_FD( Fd );                                  DEBUG(( "fd %0X", (int)Fd ));
          GET_INT16( Size );                             DEBUG(( "limit %d", (int)Size ));
          
          Handle = Fd;
          Fd = find_handle(handlelist, Handle, 0, &dummy, &Data);
          
          PUT_BYTE( sp );
          PUT_FD(Fd);
          /*{{{  send buffer to host*/
          ChanOut(HostOutput, &Length, 2);
          ChanOut(HostOutput, Buffer, Length);
          /*}}}  */
          /*{{{  get reply from host*/
          ChanIn(HostInput, &Length, 2);
          ChanIn(HostInput, Buffer, Length);
          /*}}}  */
          out_count = 0;
          tell_handle(handlelist, Handle, &Feof);
          break;
          /*}}}  */
        case SP_SEEK:
          /*{{{  seek*/
                                                         DEBUG(( "SP.SEEK" ));
          GET_FD( Fd );                                  DEBUG(( "fd %0X", (int)Fd ));
          Handle = Fd;
          Fd = find_handle(handlelist, Handle, 0, &dummy, &Data);
          if (UNCACHED)
          {
            PUT_BYTE( sp );
            PUT_FD(Fd);
            /*{{{  send buffer to host*/
            ChanOut(HostOutput, &Length, 2);
            ChanOut(HostOutput, Buffer, Length);
            /*}}}  */
            /*{{{  get reply from host*/
            ChanIn(HostInput, &Length, 2);
            ChanIn(HostInput, Buffer, Length);
            /*}}}  */
            out_count = 0;
            tell_handle(handlelist, Handle, &Feof);
          }
          else
          {
            PUT_BYTE( SP_SUCCESS );
          }
          break;
          /*}}}  */
        case SP_TELL:
          /*{{{  tell*/
                                                        DEBUG(( "SP.TELL" ));
          GET_FD( Fd );                                 DEBUG(( "logFd %0X", (int)Fd ));
          
          Handle = Fd;
          Fd = find_handle(handlelist, Handle, 0, &dummy, &Data);
          Pos = (int)tell_handle(handlelist, Handle, &Feof);
          if (UNCACHED)
          {
            PUT_BYTE( sp );
            PUT_FD( Fd );                               DEBUG(( "phyFd %0X", (int)Fd ));
            /*{{{  send buffer to host*/
            ChanOut(HostOutput, &Length, 2);
            ChanOut(HostOutput, Buffer, Length);
            /*}}}  */
            /*{{{  get reply from host*/
            ChanIn(HostInput, &Length, 2);
            ChanIn(HostInput, Buffer, Length);
            /*}}}  */
            out_count = 0;
          }
          else
          {
            PUT_BYTE(SP_SUCCESS);
            PUT_INT32(Pos);
          }
          break;
          /*}}}  */
        case SP_EOF:
          /*{{{  eof*/
                                                        DEBUG(( "SP.EOF" ));
          GET_FD( Fd );                                 DEBUG(( "logFd %0X", (int)Fd ));
          
          Handle = Fd;
          Fd = find_handle(handlelist, Handle, 0, &dummy, &Data);
          tell_handle(handlelist, Handle, &Feof);
          if (UNCACHED)
          {
            PUT_BYTE( sp );
            PUT_FD( Fd );                               DEBUG(( "phyFd %0X", (int)Fd ));
            /*{{{  send buffer to host*/
            ChanOut(HostOutput, &Length, 2);
            ChanOut(HostOutput, Buffer, Length);
            /*}}}  */
            /*{{{  get reply from host*/
            ChanIn(HostInput, &Length, 2);
            ChanIn(HostInput, Buffer, Length);
            /*}}}  */
            out_count = 0;
          }
          else
          {
            PUT_BYTE(Feof ? SP_SUCCESS : SP_ERROR);
          }
          break;
          /*}}}  */
        case SP_FERROR:
          /*{{{  error*/
                                                        DEBUG(( "SP.FERROR" ));
          GET_FD( Fd );                                 DEBUG(( "logFd %0X", (int)Fd ));
          
          Handle = Fd;
          Fd = find_handle(handlelist, Handle, 0, &dummy, &Data);
          if (UNCACHED)
          {
            PUT_BYTE( sp );
            PUT_FD( Fd );                               DEBUG(( "phyFd %0X", (int)Fd ));
            /*{{{  send buffer to host*/
            ChanOut(HostOutput, &Length, 2);
            ChanOut(HostOutput, Buffer, Length);
            /*}}}  */
            /*{{{  get reply from host*/
            ChanIn(HostInput, &Length, 2);
            ChanIn(HostInput, Buffer, Length);
            /*}}}  */
            out_count = 0;
          }
          else
          {
            PUT_BYTE(SP_ERROR);
          }
          break;
          /*}}}  */
        default:
          /*{{{  pass_thru*/
          DEBUG(( "SP.OTHER" ));
          /*{{{  send buffer to host*/
          ChanOut(HostOutput, &Length, 2);
          ChanOut(HostOutput, Buffer, Length);
          /*}}}  */
          /*{{{  get reply from host*/
          ChanIn(HostInput, &Length, 2);
          ChanIn(HostInput, Buffer, Length);
          /*}}}  */
          break;
          /*}}}  */
        /*}}}  */
        /*}}}  */
      }
      
      if (sp < SP_USER_TAG)
      {
        /*{{{  send reply to node[Index]*/
        if (out_count)
          Length = out_count;
        
        ChanOut(Output[Index], &Length, 2);
        ChanOut(Output[Index], Buffer, Length);
        /*}}}  */
      }                                                             DEBUG(("+-+"));
      /*}}}  */
    }

    /*{{{  terminate*/
    STOP_TIME
    
    tused = TIME_ELAPSED
    
    statistics(cachelist);
    
#ifdef FILE_SUPPORT
        fclose(pictureFile);
#endif
    (void) SpExit(SPS_SUCCESS);
    /*}}}  */
}
