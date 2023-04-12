/* spmux.c 16.11.1992z */
/* 3:1 mux server_protocol | pixel_protocol --> server_protocol */

/*{{{  definitions*/
#define SPR_OK 0
#define SPS_SUCCESS 999999999
/*}}}  */
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
Channel *StdIn,   *StdOut;
Channel *NodeIn0, *NodeOut0;
Channel *NodeIn1, *NodeOut1;
Channel *PixelIn;
/*}}}  */
/*{{{  static procedures*/
/*{{{  result_int   SpExit*/
static int SpExit (int Status)
{
int Length = SP_MIN_PACKET_DATA_SIZE;
BYTE Buffer[SP_MIN_PACKET_DATA_SIZE];

  Buffer[0] = SP_EXIT;

  memcpy(&Buffer[1], &Status, 4); /* Store exit status */

  ChanOut(StdOut, &Length, 2);
  ChanOut(StdOut, Buffer, Length);

  ChanIn(StdIn, &Length, 2);
  ChanIn(StdIn, Buffer, Length);

  return(Buffer[0]);
}
/*}}}  */
/*}}}  */

int main(void)
{
/*{{{  vars*/
int i;
int x;
int y;

int ExitCount;
int Length;
BYTE Buffer[MAX_LINE_SIZE * sizeof(int) + 1+4+4];
BYTE *out_ptr;
int out_count;
BYTE sp;
/*}}}  */

    /*{{{  prime*/
    StdIn    = get_param(1);
    StdOut   = get_param(2);
    NodeIn0  = get_param(3);
    NodeOut0 = get_param(4);
    NodeIn1  = get_param(5);
    NodeOut1 = get_param(6);
    PixelIn  = get_param(7);
    
    ExitCount = 2; /* 2 server_protocol channels */
    Length = 0;
    /*}}}  */

    while (ExitCount > 0)
    {
      /*{{{  multiplex*/
      i = ProcAlt(NodeIn0, NodeIn1, PixelIn, NULL);

      switch(i)
      {
        /*{{{  NodeIn0*/
        case 0:
          /*{{{  get buffer from node0*/
          ChanIn(NodeIn0, &Length, 2);
          ChanIn(NodeIn0, Buffer, Length);
          /*}}}  */
        
          sp = Buffer[0];
          if (sp == SP_EXIT)
          {
            /*{{{  send reply "OK" to node0*/
            ExitCount--;
            
            Buffer[0] = SPR_OK; /* Successful exit */
            
            ChanOut(NodeOut0, &Length, 2);
            ChanOut(NodeOut0, Buffer, Length);
            /*}}}  */
          }
          else
          {
            /*{{{  send buffer to cachemux*/
            ChanOut(StdOut, &Length, 2);
            ChanOut(StdOut, Buffer, Length);
            /*}}}  */
            if (sp < SP_USER_TAG)
            {
              /*{{{  get reply from cachemux*/
              ChanIn(StdIn, &Length, 2);
              ChanIn(StdIn, Buffer, Length);
              /*}}}  */
              /*{{{  send reply to node0*/
              ChanOut(NodeOut0, &Length, 2);
              ChanOut(NodeOut0, Buffer, Length);
              /*}}}  */
            }
          }
        
          break;
        /*}}}  */
        /*{{{  NodeIn1*/
        case 1:
          /*{{{  get buffer from node1*/
          ChanIn(NodeIn1, &Length, 2);
          ChanIn(NodeIn1, Buffer, Length);
          /*}}}  */

          sp = Buffer[0];
          if (sp == SP_EXIT)
          {
            /*{{{  send reply "OK" to node1*/
            ExitCount--;
            
            Buffer[0] = SPR_OK; /* Successful exit */
            
            ChanOut(NodeOut1, &Length, 2);
            ChanOut(NodeOut1, Buffer, Length);
            /*}}}  */
          }
          else
          {
            /*{{{  send buffer to cachemux*/
            ChanOut(StdOut, &Length, 2);
            ChanOut(StdOut, Buffer, Length);
            /*}}}  */
            if (sp < SP_USER_TAG)
            {
              /*{{{  get reply from cachemux*/
              ChanIn(StdIn, &Length, 2);
              ChanIn(StdIn, Buffer, Length);
              /*}}}  */
              /*{{{  send reply to node1*/
              ChanOut(NodeOut1, &Length, 2);
              ChanOut(NodeOut1, Buffer, Length);
              /*}}}  */
            }
          }

          break;
        /*}}}  */
        /*{{{  PixelIn*/
        case 2:
            out_ptr = Buffer;
            out_count = 0;
        
            x = ChanInInt(PixelIn);
            y = ChanInInt(PixelIn);
        
            PUT_BYTE(SP_PIXEL_TAG);
            PUT_INT32(x);
            PUT_INT32(y);
        
            Length = x * sizeof(int);
            ChanIn(PixelIn, out_ptr, Length);
            Length += out_count;
        
            /*{{{  send buffer to cachemux*/
            ChanOut(StdOut, &Length, 2);
            ChanOut(StdOut, Buffer, Length);
            /*}}}  */
        
            break;
        /*}}}  */
      }
      /*}}}  */
    }
    /*{{{  send "EXIT" to host*/
    (void) SpExit(SPS_SUCCESS);
    /*}}}  */
}
