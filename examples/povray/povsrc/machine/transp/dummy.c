#include <stdlib.h>
#include <misc.h>
#include <string.h>
#include <channel.h>
#include "iserver.h"
#include "xputer.h"

#define SPR_OK 0
#define SPS_SUCCESS 999999999

/*{{{  global vars*/
Channel *HostInput   = NULL;
Channel *HostOutput  = NULL;
/*}}}  */

/*{{{  SpExit*/
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

int main (void)
{
  /*{{{  prime*/
  HostInput = get_param(1);
  HostOutput = get_param(2);
  /*}}}  */
/*  exit_terminate(EXIT_SUCCESS); */
  (void) SpExit(SPS_SUCCESS);
}
