/*{{{  include*/
#include <stdio.h>

#include "inmos.h"
#include "iserver.h"
#include "pack.h"
/*}}}  */

/* defs fuer Filecache */
#include "fcache.h"
#include "cache.c"
/*{{{  defs*/
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

/*{{{  vars*/
BOOL CocoPops;                /*  for DEBUG  */
BOOL VerboseSwitch;

int errno;

BYTE Tbuf[TRANSACTION_BUFFER_SIZE];

BYTE DataBuffer[MAX_SLICE_LENGTH+1];
int Size;

CacheEntryPtr cachelist;
HandleBufferPtr handlelist;
int gl_handle;
/*}}}  */
/*{{{  vars*/
FILE *Fd;
BYTE *Name, Type, Mode;
char fmode[10];
CacheEntryPtr entry;
int Handle;
BYTE * Data;
/*}}}  */

switch(sp)
{
  case SP_OPEN:
    /*{{{  SP_OPEN*/
    DEBUG(( "SP.OPEN" ));
    Name = &DataBuffer[0];
    GET_SLICE( Size, Name );
    *(Name+Size)=0;          DEBUG(( "\"%s\"", Name ));
    
    GET_BYTE( Type );        DEBUG(( "type %d", Type ));
    GET_BYTE( Mode );        DEBUG(( "mode %d", Mode ));
    
    if( Type == 1 )
       switch( Mode )
          {
       case 1 : strcpy( fmode, BINARY_1 ); break;
       case 2 : strcpy( fmode, BINARY_2 ); break;
       case 3 : strcpy( fmode, BINARY_3 ); break;
       case 4 : strcpy( fmode, BINARY_4 ); break;
       case 5 : strcpy( fmode, BINARY_5 ); break;
       case 6 : strcpy( fmode, BINARY_6 ); break;
    
    }
    else
       switch( Mode )
          {
       case 1 : strcpy( fmode, TEXT_1 ); break;
       case 2 : strcpy( fmode, TEXT_2 ); break;
       case 3 : strcpy( fmode, TEXT_3 ); break;
       case 4 : strcpy( fmode, TEXT_4 ); break;
       case 5 : strcpy( fmode, TEXT_5 ); break;
       case 6 : strcpy( fmode, TEXT_6 ); break;
          }
    
    switch( Mode )
    {
      case 1:
        entry = open_cacheentry(&cachelist, Name, fmode);
        Fd = (FILE *)open_handleentry(&handlelist, entry);
        break;
      default:
    
        break;
    }
    
    PUT_BYTE( SP_SUCCESS );
    PUT_FD( Fd )
    DEBUG(( "fd %0X", (int)Fd ));
    REPLY;
    /*}}}  */
    break;
  case SP_CLOSE
    /*{{{  SP_CLOSE*/
    REPLY;
    /*}}}  */
    break;
  case SP_READ
  case SP_GET_BLOCK
    /*{{{  */
    PUBLIC VOID SpRead()
    {
       BUFFER_DECLARATIONS;
       FILE *Fd;
       BYTE *Data;
       int Read;
       int Handle;
    
       DEBUG(( "SP.READ" ));
       INIT_BUFFERS;
    
       GET_FD( Fd );        DEBUG(( "fd %0X", (int)Fd ));
       switch( (int)Fd )
          {
             case 0 : Fd = stdin;
    #ifdef SUN
                      ResetTerminal();
    #endif
                      break;
             case 1 : Fd = stdout; break;
       case 2 : Fd = stderr; break;
          }
    
       GET_INT16( Size );         DEBUG(( "request %d", Size ));
    
       Handle = (int)Fd;
       Fd = find_handle(handlelist, Handle, Size, (long*) &Read, &Data);
       if (Data == NULL)
       {
      Data = &DataBuffer[0];
      Read = fread( Data, 1, Size, Fd );  DEBUG(( "read %d", Read ));
      tell_handle(handlelist, Handle);
       }
    
       PUT_BYTE( SP_SUCCESS );
       PUT_SLICE( Read, Data );
    
       REPLY;
    /*}}}  */
    break;
  case SP_GETS
    /*{{{  */
    BUFFER_DECLARATIONS;
    FILE *Fd;
    BYTE *Data;
    int Handle;
    long dummy;
    
    DEBUG(( "SP.GETS" ));
    INIT_BUFFERS;
    
    GET_FD( Fd ); DEBUG(( "fd %0X", (int)Fd ));
    switch( (int)Fd )
       {
          case 0 : Fd = stdin;
                      break;
             case 1 : Fd = stdout; break;
             case 2 : Fd = stderr; break;
          }
       GET_INT16( Size ); DEBUG(( "limit %d", Size ));
    
       Handle = (int)Fd;
       Fd = find_handle(handlelist, Handle, 0, &dummy, &Data);
    
       Data = &DataBuffer[0];
       if ( fgets( Data, Size, Fd ) == NULL )
          {
       PUT_BYTE( SP_ERROR );
          }
       else
          {
       Size = strlen( Data );
       if( *(Data+Size-1) == '\n')
          {
             *(Data+Size) = 0;
             --Size;
          }
       DEBUG(( "got %d", Size ));
       PUT_BYTE( SP_SUCCESS );
       PUT_SLICE( Size, Data );
          }
       tell_handle(handlelist, Handle);
    
       REPLY;
    /*}}}  */
    break;
  case SP_SEEK:
    /*{{{  */
    DEBUG(( "SP.SEEK" ));
    INIT_BUFFERS;
    
    GET_FD( Fd );
    DEBUG(( "fd %0X", (int)Fd ));
    switch( (int)Fd )
       {
          case 0 : Fd = stdin; break;
          case 1 : Fd = stdout; break;
          case 2 : Fd = stderr; break;
       }
    GET_INT32( Offset ); DEBUG(( "offset %ld", Offset ));
    GET_INT32( Origin ); DEBUG(( "origin %ld", Origin ));
    switch( (int)Origin )
       {
          case 1 : origin = SEEK_SET; break;
          case 2 : origin = SEEK_CUR; break;
          case 3 : origin = SEEK_END; break;
       }
    Handle = (int)Fd;
    Fd = find_handle(handlelist, Handle, 0, &dummy, &Data);
    
    if ( fseek( Fd, Offset, origin ) )
       {
          PUT_BYTE( SP_ERROR );
       }
    else
       {
          PUT_BYTE( SP_SUCCESS );
       }
    
    tell_handle(handlelist, Handle);
    
    REPLY;
    /*}}}  */
    break;
  case SP_TELL:
    /*{{{  */
    DEBUG(( "SP.TELL" ));
    INIT_BUFFERS;
    
    GET_FD( Fd );
    DEBUG(( "fd %0X", (int)Fd ));
    switch( (int)Fd )
       {
          case 0 : Fd = stdin; break;
          case 1 : Fd = stdout; break;
          case 2 : Fd = stderr; break;
       }
    
    Fd = find_handle(handlelist, (int)Fd, 0, &dummy, &Data);
    
    Position = ftell( Fd );
    if( Position == -1L )
       {
          PUT_BYTE( SP_ERROR );
       }
    else
       {
          PUT_BYTE( SP_SUCCESS );
          PUT_INT32( Position );
       }
    REPLY;
    /*}}}  */
    break;
  case SP_EOF:
    /*{{{  */
    DEBUG(( "SP.EOF" ));
    INIT_BUFFERS;
    
    GET_FD( Fd );
    DEBUG(( "fd %0X", (int)Fd ));
    switch( (int)Fd )
       {
          case 0 : Fd = stdin; break;
          case 1 : Fd = stdout; break;
          case 2 : Fd = stderr; break;
       }
    
    Fd = find_handle(handlelist, (int) Fd, 0, &dummy, &Data);
    
    if( feof( Fd ) )
       {
          PUT_BYTE( SP_SUCCESS );
       }
    else
       {
          PUT_BYTE( SP_ERROR );
       }
    REPLY;
    /*}}}  */
    break;
  cafe SP_ERROR:
    /*{{{  */
    DEBUG(( "SP.ERROR" ));
    INIT_BUFFERS;
    
    GET_FD( Fd );
    DEBUG(( "fd %0X", (int)Fd ));
    switch( (int)Fd )
       {
          case 0 : Fd = stdin; break;
          case 1 : Fd = stdout; break;
          case 2 : Fd = stderr; break;
       }
    
    Fd = find_handle(handlelist, (int) Fd, 0, &dummy, &Data);
    
    if( ferror(Fd) )
       {
          PUT_BYTE( SP_SUCCESS );
          Errno = errno;
          DEBUG(( "errno %d", Errno ));
          PUT_INT32( Errno );
             strcpy( &String[0], strerror(errno));
             DEBUG(( "error \"%s\"", &String[0]));
             Size = strlen(&String[0]);
             PUT_SLICE( Size, String[0] );
             REPLY;
          }
       else
          {
             PUT_BYTE( SP_ERROR );
             REPLY;
          }
    /*}}}  */
    break;
  default:
    /*{{{  */
    REPLY;
    /*}}}  */
    break;
}

