/*
 * Copyright (c) 1993-1996 Julian Highfield. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Julian Highfield.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * main.c
 *
 * Read and parse command line. Start emulator.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "processor.h"

#ifdef __MWERKS__
#include "mac_input.h"
#include <profiler.h>
#else
#define true TRUE
#define false FALSE
#ifdef SUN
#include <termio.h>
#else
#include <sys/termios.h>
#include <sys/ioctl.h>
#define TCGETS TIOCGETA
#define TCSETS TIOCSETA
#define termio termios
#endif
#endif

#undef TRUE
#undef FALSE
#define TRUE  0x0001
#define FALSE 0x0000

int analyse     = FALSE;
int copy        = FALSE;
int exitonerror = true;
int peeksize    = 8;
int emudebug    = FALSE;

extern int32_t quit;
extern int32_t quitstatus;

char CommandLineAll[256]  = "\0";
char CommandLineMost[256] = "\0";

unsigned char core[16*1024];

FILE *CopyIn;

extern unsigned char *mem;
extern uint32_t WPtr;
extern uint32_t CurPriority;

/* Profile flag. */
int profiling = false;

/* Profile array. */
int32_t profile[10];

/* Profile file. */
FILE *ProfileFile;

/* Signal handler. */
void handler (int);

#ifdef __MWERKS__
int argc; /*= 4;*/
char argv[4][40]; /* = {"jserver","-se","-sb","solport.b4"};*/
#else
/* Terminal settings. */
struct termios t_init;
struct termios t_getk;
struct termios t_poll;
enum {INIT, GETK, POLL} t_state;
#endif


#ifdef __MWERKS__
void main(void)
#else
int main (int argc, char **argv)
#endif
{
	static char CopyFileName[256];
	int verbose     = FALSE;
	int reset       = FALSE;
	int serve       = FALSE;
	int arg;
	int temp;
	int temp2;

#ifdef __MWERKS__
	/* Create some menus, etc. */
	menu ();
#else
	mem = malloc (2*1024*1024);
	if (mem == NULL)
	{
		printf ("\nFailed to allocate memory!\n");
		exit (-1);
	}
#endif

	if (argc<2)
	{
		printf("\n");
		printf("Usage : jserver [options] [program arguments]\n\n");
		printf("jserver V1.1   14/7/96\n\n");
		printf("Options:\n");
		printf("    -sa                  Analyse transputer.\n");
		printf("    -sb filename         Boot program \"filename\".\n");
		printf("    -sc filename         Copy file \"filename\" to transputer.\n");
                printf("    -sd                  Debug emulator.\n");
		printf("    -se                  Terminate on transputer error.\n");
		printf("    -si                  Output progress messages.\n");
		printf("    -sr                  Reset transputer.\n");
		printf("    -sp number           Peek \"number\" kilobytes on analyse.\n");
		printf("    -ss                  Provide host services to transputer.\n");
		printf("\n");
		handler (-1);
	}

	/* Set up signal handler. */
	signal (SIGINT, handler);

	/* Build full command line string for server to return on request. */
	for (arg=0;arg<argc;arg++)
	{
		if (arg>0) strcat(CommandLineAll, " ");
		strcat(CommandLineAll, argv[arg]);
	}
	strcat(CommandLineAll, "\n");
        if (emudebug)
	  printf("\nFull command line is : %s", CommandLineAll);

	for (arg=1;arg<argc;arg++)
	{
		if ((argv[arg][0]=='-') && (argv[arg][1]=='s'))
		{
			switch (argv[arg][2])
			{
				case 'a': if (argv[arg][3]!='\0')
					  {
						strcat (CommandLineMost, argv[arg]);
						strcat (CommandLineMost, " ");
					  }
					  else analyse=TRUE;
					  break;
				case 'b': if (argv[arg][3]!='\0')
					  {
						strcat (CommandLineMost, argv[arg]);
						strcat (CommandLineMost, " ");
					  }
					  else
					  {
						arg++;
						if (arg>=argc)
						{
							printf("\nMissing filename after -sb\n");
							handler (-1);
						}
						strcpy (CopyFileName, argv[arg]);
						reset=TRUE;
						serve=TRUE;
						verbose=TRUE;
						copy=TRUE;
					  }
					  break;
				case 'c': if (argv[arg][3]!='\0')
					  {
						strcat (CommandLineMost, argv[arg]);
						strcat (CommandLineMost, " ");
					  }
					  else
					  {
						arg++;
						if (arg>=argc)
						{
							printf("\nMissing filename after -sc\n");
							handler (-1);
						}
						strcpy (CopyFileName, argv[arg]);
						copy=TRUE;
					  }
					  break;
				case 'd': if (argv[arg][3]!='\0')
					  {
						strcat (CommandLineMost, argv[arg]);
						strcat (CommandLineMost, " ");
					  }
					  else emudebug=true;
					  break;
				case 'e': if (argv[arg][3]!='\0')
					  {
						strcat (CommandLineMost, argv[arg]);
						strcat (CommandLineMost, " ");
					  }
					  else exitonerror=true;
					  break;
				case 'i': if (argv[arg][3]!='\0')
					  {
						strcat (CommandLineMost, argv[arg]);
						strcat (CommandLineMost, " ");
					  }
					  else verbose=TRUE;
					  break;
				case 'r': if (argv[arg][3]!='\0')
					  {
						strcat (CommandLineMost, argv[arg]);
						strcat (CommandLineMost, " ");
					  }
					  else reset=TRUE;
					  break;
				case 'p': if (argv[arg][3]!='\0')
					  {
						strcat (CommandLineMost, argv[arg]);
						strcat (CommandLineMost, " ");
					  }
					  else
					  {
						arg++;
						if (arg>=argc)
						{
							printf("\nMissing number after -sp\n");
							handler (-1);
						}
						if (sscanf(argv[arg], "%d", &peeksize)!=1)
						{
							printf("\nBad number after -sp\n");
							handler (-1);
						}
						if (peeksize > 16)
						{
							printf("\nCannot peek that much memory.\n");
							handler (-1);
						}
					  }
					  break;
				case 's': if (argv[arg][3]!='\0')
					  {
						strcat (CommandLineMost, argv[arg]);
						strcat (CommandLineMost, " ");
					  }
					  else serve=TRUE;
					  break;
				default : strcat(CommandLineMost, argv[arg]);
					  strcat (CommandLineMost, " ");
					  break;
			}
		}
		else
		{
			strcat(CommandLineMost, argv[arg]);
			strcat (CommandLineMost, " ");
		}
	}
	CommandLineMost[strlen(CommandLineMost)-1] = '\n';

        if (emudebug)
        {
	        printf("Most command line is : %s", CommandLineMost);
	        printf("analyse %d; copy %d; exit %d; verbose %d; reset %d; peek %d; serve %d\n", analyse,copy,exitonerror,verbose,reset,peeksize,serve);
        }

	/* Open boot file. */
	if ((CopyIn = fopen(CopyFileName, "rb"))==NULL)
	{
		printf("Failed to open file %s for copy to link!\n", CopyFileName);
		handler (-1);
	}

	/* Load bootstrap into emulator. */
	temp = getc (CopyIn);
	if (temp < 2)
	{
		printf ("\nFile does not start with bootstrap code!\n");
		handler (-1);
	}
	for (temp2=0; temp2<temp; temp2++)
	{
		writebyte ((MemStart+temp2), getc (CopyIn));
	}
	WPtr = MemStart + temp2;
	while ((WPtr & 0x00000003) != 0x00000000)
		WPtr++;
	CurPriority = LoPriority;

	/* Initialise profiling array. */
	for (temp=0; temp<10; temp++)
		profile[temp] = 0;

#ifndef __MWERKS__
	/* Initialise t_init terminal settings. */
	if ((ioctl (0, TCGETS, &t_init)) != 0)
	{
		printf("\nBad ioctl - initialising t_init.\n");
		handler (-1);
	}
	t_state = INIT;

	/* Initialise t_getk terminal settings. */
	if ((ioctl (0, TCGETS, &t_getk)) != 0)
	{
		printf("\nBad ioctl - initialising t_getk.\n");
		handler (-1);
	}
	/* Turn off buffering and echoing. */
	t_getk.c_iflag &= ~ICRNL;
	t_getk.c_lflag &= ~(ICANON | ECHO);
	/* Get at least 1 char, wait as long as you have to. */
	t_getk.c_cc[VMIN] = 1;
	t_getk.c_cc[VTIME] = 0;

	/* Initialise t_poll terminal settings. */
	if ((ioctl (0, TCGETS, &t_poll)) != 0)
	{
		printf("\nBad ioctl - initialising t_poll.\n");
		handler (-1);
	}
	/* No buffering, no echoing. */
	t_poll.c_iflag &= ~ICRNL;
	t_poll.c_lflag &= ~(ICANON | ECHO);
	/* Don't necessarily get a charcter, wait 0.1s. */
	t_poll.c_cc[VMIN] = 0;
	t_poll.c_cc[VTIME] = 1;
#endif

#ifdef PROFILE
	if (profiling)
	{
		/* Open profiling file. */
		if ((ProfileFile = fopen("profile", "w"))==NULL)
		{
			printf("Failed to open profile file!\n");
			handler (-1);
		}
	}
#endif


#if __profile__
	/* METROWERKS PROFILER. */
	if (ProfilerInit(collectDetailed,bestTimeBase,10,10) == noErr)
	{
#endif
		
	/* Now start the emulator. */
	mainloop ();

#if __profile__
	/* METROWERKS PROFILER. */
		ProfilerDump("\pTutorial.prof");
		ProfilerTerm();
	}
#endif

	/* Close boot file. */
	fclose (CopyIn);

#ifdef PROFILE
	if (profiling)
	{
		/* Print out profile counts. */
		fprintf (ProfileFile, "\n%d instructions\n", profile[0]);
		fprintf (ProfileFile, "%d descheduling points\n", profile[1]);
		fprintf (ProfileFile, "%d server calls\n", profile[2]);
		fprintf (ProfileFile, "%d start_process calls\n", profile[3]);

		print_profile ();
		fclose (ProfileFile);
	}
#endif

#ifndef __MWERKS__
	if (t_state != INIT)
	{
		if ((ioctl (0, TCSETS, &t_init)) != 0)
		{
			printf("\nBad ioctl while resetting terminal.\n");
			handler (-1);
		}
	}
#endif

#ifdef __MWERKS__
	while (1)
		check_input ();
#endif
}


void handler (int signal)
{
#ifdef __MWERKS__
	DialogPtr dialog;
	short itemHit = 0;
#endif

#ifdef PROFILE
	if (profiling)
		fclose (ProfileFile);
#endif

#ifdef __MWERKS__
	/* Exit dialog. */
	dialog = GetNewDialog (129, nil, (void *) -1);
	
	while (itemHit == 0)
	{
		ModalDialog (nil, &itemHit);
	}
	
	DisposDialog (dialog);

	free (mem);
	ExitToShell ();
#else
	if (t_state != INIT)
	{
		if ((ioctl (0, TCSETS, &t_init)) != 0)
		{
			printf("\nBad ioctl while resetting terminal.\n");
			handler (-1);
		}
	}

	exit (-1);
#endif
}
