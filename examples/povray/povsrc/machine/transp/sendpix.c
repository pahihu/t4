/* sendpix.c 7.11.1992z (<--inmos.c)*/
/*send info to be displayed down the channel */
/* PixelProtocol */

/*{{{  include*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "config.h"
#include "xputer.h"
#include "ipack.h"
#include "iserver.h"
/*}}}  */
/*{{{  vars*/
extern Channel *PixelOut;
static int d_linenumber;
static int d_count;
static int d_flag;
static int d_line[MAX_LINE_SIZE];
int Length;
/*}}}  */

/*{{{  void display_plot (x, y, Red, Green, Blue)*/
void display_plot (int x, int y, char Red, char Green, char Blue)
{
   /*{{{  if(d_flag == TRUE)*/
   if(d_flag == TRUE)
   {
       d_line[x] = (Blue << 16) | (Green << 8) | Red;
       d_linenumber = y;
       d_count = 1;
       d_flag = FALSE;
   }
   /*}}}  */
   /*{{{  else*/
   else
   {
       if (y == d_linenumber)
       {
           d_line[x] = (Blue << 16) | (Green << 8) | Red;
           d_count++;
       }
       else
       {
           /*{{{  send info*/
           Length = d_count * sizeof(int);
           
           ChanOutInt(PixelOut, d_count);
           ChanOutInt(PixelOut, d_linenumber);
           ChanOut(PixelOut, d_line, Length);
           
           /*}}}  */
   
           d_line[x] = (Blue << 16) | (Green << 8) | Red;
           d_linenumber = y;
           d_count = 1;
           d_flag = FALSE;
       }
   }
   /*}}}  */
}
/*}}}  */
/*{{{  void display_finished (void)*/
void display_finished (void)
{
        /*{{{  send info*/
        Length = d_count * sizeof(int);
        
        ChanOutInt(PixelOut, d_count);
        ChanOutInt(PixelOut, d_linenumber);
        ChanOut(PixelOut, d_line, Length);
        
        /*}}}  */
}
/*}}}  */
/*{{{  void display_init ()*/
void display_init (void)
{
    d_flag = TRUE;
}
/*}}}  */

/*{{{  void transputer_init_povray PARAMS ()*/
void transputer_init_povray PARAMS ((void))
{
}
/*}}}  */
/*{{{  void display_close ()*/
void display_close (void)
{
}
/*}}}  */
