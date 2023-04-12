/****************************************************************************
*                unix.c
*
*  This module implements UNIX specific routines.
*
*  from Persistence of Vision Raytracer 
*  Copyright 1991 Persistence of Vision Team
*---------------------------------------------------------------------------
*  Copying, distribution and legal info is in the file povlegal.doc which
*  should be distributed with this file. If povlegal.doc is not available
*  or for more info please contact:
*
*       Drew Wells [POV-Team Leader] 
*       CIS: 73767,1244  Internet: 73767.1244@compuserve.com
*       Phone: (213) 254-4041
* 
* This program is based on the popular DKB raytracer version 2.12.
* DKBTrace was originally written by David K. Buck.
* DKBTrace Ver 2.0-2.12 were written by David K. Buck & Aaron A. Collins.
*
*****************************************************************************/

#include <math.h>
#include "config.h"

void unix_init_POVRAY PARAMS ((void))
   {
   }

int matherr (x)
   struct exception *x;
   {
   switch(x->type) 
     {
     case DOMAIN:
     case OVERFLOW:
        x->retval = 1.0e17;
        break;

     case SING:
     case UNDERFLOW:
        x->retval = 0.0;
        break;

     default:
        break;
     }
   return(1);
   }

void display_finished ()
   {
   }

void display_init ()
   {
   }

void display_close ()
   {
   }

void display_plot (x, y, Red, Green, Blue)
   int x, y;
   char Red, Green, Blue;
   {
   }
