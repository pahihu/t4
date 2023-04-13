/* fread.c 16.11.1992z */
/* server/pixel_protocol read_command filter */

/*{{{  includes*/
#include <stdio.h>
#include <channel.h>
#include <stdlib.h>
#include <string.h>
#include <misc.h>
#include <process.h>

#include "iserver.h"
#include "ipack.h"
#include "xputer.h"
/*}}}  */
/*{{{  chan*/
Channel *StdIn,  *StdOut;
Channel *NodeIn, *NodeOut;
Channel *PixelIn;
/*}}}  */

int main(void)
{
/*{{{  vars*/
int Length;
BYTE Buffer[MAX_LINE_SIZE * sizeof(int) + 1+4+4];
BYTE *out_ptr;
BYTE *in_ptr;
int out_count;

INT16 read;
BYTE sp;
BYTE *data;
BYTE result;
/*}}}  */

    /*{{{  prime*/
    StdIn   = get_param(1);
    StdOut  = get_param(2);
    NodeIn  = get_param(3);
    NodeOut = get_param(4);
    
    Length = 0;
    /*}}}  */

    while (TRUE)
    {
      /*{{{  filter*/
      /*{{{  get buffer from node*/
      ChanIn(NodeIn, &Length, 2);
      ChanIn(NodeIn, Buffer, Length);
      /*}}}  */
      /*{{{  send buffer to cachemux*/
      ChanOut(StdOut, &Length, 2);
      ChanOut(StdOut, Buffer, Length);
      /*}}}  */

      sp = Buffer[0];
      switch(sp)
      {
        case SP_PIXEL_TAG:
          /*{{{  SP_PIXEL_TAG*/
          break;
          /*}}}  */
        case SP_READ:
        case SP_GETBLOCK:
          /*{{{  SP_READ/GETBLOCK*/
          out_ptr = Buffer;
          in_ptr = Buffer;
          out_count = 0;

          /*{{{  get reply from cachemux*/
          ChanIn(StdIn, &Length, 2);
          ChanIn(StdIn, Buffer, Length);
          /*}}}  */
          GET_BYTE(result);
          if(result == SP_CACHED)
          {
            GET_INT16(read);
            GET_INT32(data);

            PUT_BYTE(SP_SUCCESS);
          /* naive */
          /*  PUT_SLICE(read, data); */
            _memcpy(out_ptr, &read, 2);
            out_ptr += 2;
            out_count += 2;
            _memcpy(out_ptr, data, read);
            out_count += read;
            Length = out_count;
          }
          /*{{{  send reply to node*/
          ChanOut(NodeOut, &Length, 2);
          ChanOut(NodeOut, Buffer, Length);
          /*}}}  */
          break;
          /*}}}  */
        default:
          /*{{{  default*/
          /*{{{  get reply from cachemux*/
          ChanIn(StdIn, &Length, 2);
          ChanIn(StdIn, Buffer, Length);
          /*}}}  */
          /*{{{  send reply to node*/
          ChanOut(NodeOut, &Length, 2);
          ChanOut(NodeOut, Buffer, Length);
          /*}}}  */
          
          break;
          /*}}}  */
      }
      /*}}}  */
    }
}
