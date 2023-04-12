/****************************************************************************
*                tranmain.c
*---------------------------------------------------------------------------
*                THIS IS SPECIAL VERSION OF POV 1.0 !!!!!!!!!
*                          NOT SUPPORTED BY THE 
*                                POV-Team
*---------------------------------------------------------------------------
* If there are any problems with the transputer version please contact
*
*                   Rainer Menes
*                   Internet: menes@statistik.tu-muenchen.de
*                   Phone: +49 (89) 2105 8226
*                   Fax: +49 (89) 2105 8228
*
*--------------------------------------------------------------------------   
*  This module contains the entry routine for the raytracer and the code to
*  parse the parameters on the command line.
*
*  from Persistence of Vision Raytracer 
*       Copyright 1992 Persistence of Vision Team
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
#define TRANSMAIN

#include "frame.h"         /* common to ALL modules in this program */
#include "povproto.h"


int main(void)
{
	register int i;
	int *helpargc;
	int argc;
	char *argv[256];
	
	PixelOut = get_param(3);
	helpargc = get_param(4);
	worker_id = *helpargc;
	helpargc = get_param(5);
	number_off_worker = *helpargc;
	helpargc = (int*)get_param(first);
	argc = *helpargc;
	for(i=0; i<argc; i++)
	{
		argv[i] = get_param(first + i + 1);
		/*printf("argv[%d] = %s\n",i,argv[i]);*/
	}
	alt_main(argc, argv);
}


