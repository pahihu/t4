/****************************************************************************
*                   texture.h
*
*  This file contains defines and variables for the txt*.c files
*  
*
*  from Persistence of Vision Raytracer 
*  Copyright 1992 Persistence of Vision Team
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

extern long Calls_To_Noise, Calls_To_DNoise;

#define MINX	-10000		/* Ridiculously large scaling values */
#define MINY	MINX
#define MINZ	MINX

#define MAXSIZE 267
#define RNDMASK 0x7FFF
#define RNDDIVISOR (float) RNDMASK
#define NUMBER_OF_WAVES 10
#define SINTABSIZE 1000

#define FLOOR(x) ((x) >= 0.0 ? floor(x) : (0.0 - floor(0.0 - (x)) - 1.0))
#define FABS(x) ((x) < 0.0 ? (0.0 - x) : (x))
#define SCURVE(a) ((a)*(a)*(3.0-2.0*(a)))
#define REALSCALE ( 2.0 / 65535.0 )
#define Hash3d(a,b,c) hashTable[(int)(hashTable[(int)(hashTable[(int)((a) & 0xfffL)] ^ ((b) & 0xfffL))] ^ ((c) & 0xfffL))]
#define INCRSUM(m,s,x,y,z)	((s)*(RTable[m]*0.5		\
+ RTable[m+1]*(x)	\
					+ RTable[m+2]*(y)	\
					+ RTable[m+3]*(z)))

extern int Options;
extern DBL *sintab;
extern DBL frequency[NUMBER_OF_WAVES];
extern VECTOR Wave_Sources[NUMBER_OF_WAVES];
extern DBL *RTable;
extern short *hashTable;
extern unsigned short crctab[256];
