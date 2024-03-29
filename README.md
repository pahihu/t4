Thanks
======

I would like to say thanks to 

* Julian Highfield writing his [Transputer Emulator](https://web.archive.org/web/20130515034826/http://spirit.lboro.ac.uk/emulator.html)
* Mike's [transputer.net](http://www.transputer.net/welcome.asp) for the Transputer Validation Suite (TVS1 and TVS1F), for invaluable documentation
* Mike Bruestle for the implementation details of bitrevnbits, postnormsn, div, roundsn, fpstnli32 and fpunoround
* Mike Bruestle and Gavin Crate for the discussion of using the native FPU to emulate the T800 FPU instructions
* Gavin Crate [Transputer Emulator](https://sites.google.com/site/transputeremulator/Home) for the email conversation about the Portakit

Goals
=====

* emulate the first generation T414/T800 transputers
* instruction tracing
* to run the D7205A occam 2 toolset on macOS and Linux 64bit

Non-goals
=========

* cycle accurate emulation
* support undefined behavior (DReg/EReg)

Notes
=====

Passing the T800 floating point tests is NOT easy. I use
functions from [OpenLibm](https://openlibm.org) with my own fixes.
The following operating system and compiler combinations are passing 
the T800 FPU tests:

* macOS High Sierra (10.13) + clang 9.1.0 64bit/32bit
* macOS Mojave (10.14) + clang 10.0.1 or MacPorts gcc8 8.4.0_0 64bit
* Windows + VisualStudio 2017 64bit/32bit
* Ubuntu 18.04 LTS + gcc 7.5.0 Ubuntu 7.5.0-3ubuntu1~18.04 64bit/32bit
* Raspbian 9 + gcc 6.3.0 Raspbian 6.3.0-18+rpi1+debu9u1 32bit
* SLES10SP4 IBM Power + gcc 4.1.2 SUSE Linux 64bit/32bit

The compiled binaries are in the `bin/` directory.

Changes
=======

* removed 32bit binaries, all platforms are networking enabled
* startup/shutdown of SHM link transputer networks using the `-sn` flag
* binaries use profile guided optimization (PGO)
* added some instruction combinations, which are detected at runtime
* simplified OReg clearing
* eliminated IntEnabled tests using the Icache[]
* Minix demo runs again, implemented idle mode, adaptive timer updates, updated networking binaries
* added POV-Ray 1.0 sources and boot loader file (see examples/povray/pictures.zip for the generated images)
* added COMSTIME benchmark
* updated Windows and Linux binaries, they support both nework and shared-memory links
* if you define -DSHLINKS=1 the communication links are placed in shared memory, added `-sl` flag
* added neworking binary for Raspberry Pi, fixed kill in multi.sh
* added more raytracer binaries and Windows batch file to run them (multi.bat)
* see examples/transputer-raytracer/bin/README how to run multiple emulators
* added networking enabled bin/t4netwin64.exe, Makefile.netwin32, bin/nanomsg115.zip static library
* added networking enabled bin/t4netmac64, bin/t4netlnx64 for macOS, Linux
* see `spy.net` for network configuration examples
* if you define -DSPYNET=1 and link the executable with the [nanomsg](https://nanomsg.org) library
  you can run multiple transputers (experimental)
* updated macOS, Linux, Windows binaries
* accept either '-' or '/' as switch options (G.Crate)
* using 64bit/32bit arithmetic operations, instead of 16bit
* added `-su` instruction profiling, output in `profile`, changed command line options
* added macOS High Sierra 32bit binary
* added Linux-POWER binaries 64bit/32bit
* added Linux-x86 binary
* added single/double precision versions of C Whetstone
* added Raspbian binary
* replaced macOS binary with macOS High Sierra (SSE2)
* fixed Windows compilation, Linux x64 port, binaries for Windows/Linux/macOS
* more replacement functions: remainder()/sqrt()
* updated Windows binaries with replacement ldexp()/ldexpf() functions
* the TVS1F floating-point results depend on the compiler, compiler flags and C math runtime
* successfully runs Mike's TVS1F (using Apple clang)
* successfully runs Mike's TVS1
* Savage runs with tbmaths.lib
* added -sv flag to support Mike's TVS1
* Windows port, added Win32/Win64 executables
* added C and occam versions of Savage benchmark, fixed fpremfirst
* added C versions of Dhrystone and Whetstone benchmarks
* D7214C C toolset successfully built hello example, added hello example in C
* runs the T800 sqrroots example, added sqrroots source by Daniel Hyde
* runs the T800 Whetstone benchmark
* changed program arguments, memory size can be changed from the command line
* runs the Minix demo (does NOT need a microcode emulator or an iserver supporting block operations)
* D7205A occam 2 toolset successfully built hello example
* itools directory contains D7205A environment setup and some tool scripts
* instruction execution tracing
* memory reference tracing
* fixed some process scheduling bugs
* the simulator is ported to macOS/Linux 64bit architecture

**NOTE**: There is the excellent [Transputer Emulator](https://sites.google.com/site/transputeremulator/Home) from Gavin Crate. Otherwise here is the code. Good luck!


pahihu,  05 / 04 / 2023


Performance
===========
The original [INMOS occam raytracer](https://github.com/pahihu/t4/tree/master/examples/raytracer) performance on a 16 processor Linux virtual machine.

Values are in seconds.

|Workers|Type|Mod1|Mod2|Mod3|Mod4|Notes|
|--|--|--|--|--|--|--|
|16|T800E/2MB|3.2|3.4|2.3|2.4|16x T4 emulator with SHM links
|8|T800E/2MB|5.0|5.5|2.7|2.3|8x T4 emulator with SHM links
|4|T800E/2MB|8.5|9.6|3.7|2.9|4x T4 emulator with SHM links
|2|T800E/2MB|16.3|17.8|5.9|4.7|2x T4 emulator with SHM links
|1|T800E/2MB|31.5|34.9|11.4|8.6|1x T4 emulator



Performance of the system compared on Model2 as in [Exploiting concurrency: a ray tracing example](http://transputer.net/tn/07/tn07.html#x1-100004).

|transputers|speed|relative speed|linearity|
|--|--|--|--|
|1|1877.8|1.00|100.0|
|2|3681.8|1.96|98.0|
|4|6826.7|3.64|90.9|
|8|11915.6|6.35|79.3|
|16|19275.3|10.26|64.2|



Command line arguments
======================

        Usage : t4 [options] [program arguments]

        t4 V1.6a    04/04/2023

        Options: use either -sX or /sX
            -sa                  Analyse transputer.
            -sb filename         Boot program "filename".
            -sc filename         Copy file "filename" to transputer.
            -se                  Terminate on transputer error.
            -si                  Output progress messages.
            -sr                  Reset transputer.
            -sp number           Peek "number" kilobytes on analyse.
            -ss                  Provide host services to transputer.
        Extra options:
            -s4                  Select T414 mode. (default)
            -s8                  Select T800 mode.
            -sg                  Halt on uninitialized memory read.
            -sl                  Links in shared memory.
            -sm #bits            Memory size in address bits (default 21, 2Mbyte).
            -sn id               Node ID.
            -su                  Instruction profiling.
            -sv inp.tbo inp.bin out.bin
                                 Select Mike's TVS: T800 + T414 FP support.
            -sw "string"         Trigger execution trace on SP_WRITE (string).
            -sx [number]         Execution trace (4 - mem ld/st, 2 - iserver, 1 - instructions).


Running the Minix demo
======================

        Secondary bootstrap entered.
        Free mem starts at 800007bc
        Sizing memory finished.
        Found 00200800 bytes.
        Loading boot monitor ok.

        Minix boot monitor : Version 0.06 (built Apr 11 1994 17:44:04)

        Creating new module
        Creating new module
        Creating new module
        Creating new module
        > boot
        Booting
        Module kernel text size 00004f70 data size 00009bf4
        Loaded from 80009d00 to 8000ec70
        Module mm text size 00002cc0 data size 000069d0
        Loaded from 8000ed00 to 800119c0
        Module fs text size 0000682c data size 0005a29c
        Loaded from 80018400 to 8001ec2c
        Module init text size 0000120c data size 0000166c
        Loaded from 80079000 to 8007a20c
        Booting MINIX 1.5.  Copyright 1990 Prentice-Hall, Inc.
        Memory size = 2050K     MINIX = 495K     RAM disk =    0K     Available = 1555K

        Using a 1.0 iserver. Max packet size 470. No block operstions.
        Welcome to transputer MINIX
        Root file system built Wed Oct 20 16:37:24 BST 1993
        login: root
        Welcome to Transputer MINIX
        # ls
        .profile   bin        dev        etc        usr        
        # 



Example output from the instruction tracing
===========================================

        -IPtr------Code-----------------------Mnemonic------------HE---AReg-----BReg-----CReg-------WPtr-----WPtr[0]-
         80000202: 12                         LDLP    #2          --   80000148 80000077 80000077   80000230 80000048
         80000203: 71                         LDL     #1          --   80000238 80000148 80000077   80000230 80000048
         80000204: 41                         LDC     #1          --   80000010 80000238 80000148   80000230 80000048
         80000205: F7                         IN                  --          1 80000010 80000238   80000230 80000048
        -I-EMUDBG: In(1): Channel=#80000010, to memory at #80000238, length #1.
        -I-EMUDBG: RunProcess: Empty process list. Cannot start!
        -I-EMUDBG: StartProcess: Empty process list. Update comms.
        -I-EMUSRV: BootTemp = 0. Loaded 0 bytes.
        -I-EMUSRV: To server buffer 0; From server buffer 16384.
        -I-EMUSRV: FromServerLen = #4000 Link0InLength = #1, loop = #1.
        -I-EMUSRV: Comms request satisfied. Reschedule process #80000231.
        -I-EMUDBG: Schedule(1): Process = #80000230 at priority = Lo
        -I-EMUDBG: Schedule(2): Get front of process list pointer.
        -I-EMUDBG: Schedule(3): Empty process list, create.
        -I-EMUDBG: RunProcess: LoPriority process list non-empty.
        -I-EMUDBG: RunProcess: CurPriority = Lo, ptr = #80000230. FPtrReg0 (Hi) = #80000000, FPtrReg1 (Lo) = #80000230.
         80000206: 70                         LDL     #0          --          1 80000010 80000238   80000230 80000048
         80000207: 71                         LDL     #1          --   80000048        1 80000010   80000230 80000048
         80000208: 72                         LDL     #2          --   80000010 80000048        1   80000230 80000048
         80000209: F7                         IN                  --         58 80000010 80000048   80000230 80000048
        -I-EMUDBG: In(1): Channel=#80000010, to memory at #80000048, length #58.
        -I-EMUDBG: RunProcess: Empty process list. Cannot start!
        -I-EMUDBG: StartProcess: Empty process list. Update comms.
        -I-EMUSRV: BootTemp = 1. Loaded 1 bytes.
        -I-EMUSRV: To server buffer 0; From server buffer 16384.
        -I-EMUSRV: FromServerLen = #4000 Link0InLength = #58, loop = #58.
        -I-EMUSRV: Comms request satisfied. Reschedule process #80000231.
        -I-EMUDBG: Schedule(1): Process = #80000230 at priority = Lo
        -I-EMUDBG: Schedule(2): Get front of process list pointer.
        -I-EMUDBG: Schedule(3): Empty process list, create.
        -I-EMUDBG: RunProcess: LoPriority process list non-empty.
        -I-EMUDBG: RunProcess: CurPriority = Lo, ptr = #80000230. FPtrReg0 (Hi) = #80000000, FPtrReg1 (Lo) = #80000230.
         8000020A: 71                         LDL     #1          --         58 80000010 80000048   80000230 80000048
         8000020B: 70                         LDL     #0          --   80000010       58 80000010   80000230 80000048
         8000020C: 72                         LDL     #2          --   80000048 80000010       58   80000230 80000048
         8000020D: F5                         ADD                 --         58 80000048 80000010   80000230 80000048
         8000020E: 23 FF                      WCNT                --   800000A0 80000010 80000010   80000230 80000048
         80000210: F0                         REV                 --   20000028        0 80000010   80000230 80000048
         80000211: C0                         EQC     #0          --          0 20000028 80000010   80000230 80000048
         80000212: C0                         EQC     #0          --          1 20000028 80000010   80000230 80000048
         80000213: F5                         ADD                 --          0 20000028 80000010   80000230 80000048
         80000214: 23 F4                      BCNT                --   20000028 80000010 80000010   80000230 80000048
         80000216: D3                         STL     #3          --   800000A0 80000010 80000010   80000230 80000048
         80000217: 70                         LDL     #0          --   80000010 80000010 80000010   80000230 80000048
         80000218: 73                         LDL     #3          --   80000048 80000010 80000010   80000230 80000048
         80000219: 23 FC                      GAJW                --   800000A0 80000048 80000010   80000230 80000048
         8000021B: F0                         REV                 --   80000230 80000048 80000010   800000A0       62
         8000021C: F6                         GCALL               --   80000048 80000230 80000010   800000A0       62
         80000048: 21 B4                      AJW     #14         --   8000021D 80000230 80000010   800000A0       62


General
=======

This (free) program emulates a combination of an Inmos T414 transputer (with 2M RAM) and the standard iserver host interface program.

The emulator runs single transputer binaries as though invoked using "iserver -se -sb bootfile.btl". The "-se" stop-on-error flag may be turned off using the options menu. The other options menu entry allows you to enable instruction profiling, which counts how many instructions of each kind are executed and saves the result to a file called "profile".

Bug reports are welcome. My email address is J.C.Highfield@lut.ac.uk, however this will change shortly. Bug reports should take the form of a summary of the machine the emulator is being run on, the compiler the transputer binary was generated by and whatever the binary does. Since bugs in an emulator can take a long time to track down, with some recent ones appearing after the emulator had executed 100 million instructions, then to actually solve a bug report I would probably need the binary and any data files it uses. Arithmetic bugs etc. would however be much simpler to solve than this, with a one line example program being quite adequate.

Precompiled Macintosh version
=============================

This runs on both 68k and PowerPC based Macintoshes. It needs 2.5 MBytes of memory. It has been tested on a PowerMac 6100, a IIci, an LCII, a quadra 650 and a powerbook 165.

Source-code version
===================

The source should compile on Macintoshes (I use CodeWarrior) and most unixes. I have compiled and tested it under the following unix / unix-like systems with no problems: NetBSD 1.1, SunOS 4.1.4, SGI Irix 5.2. I also compiled it under HP-UX 8 or 9, but that required some tweaking of the source. Read the Makefile for more hints.

A "Hello World" transputer binary is included as an example for testing the emulator.


Julian Highfield, 25 / 7 / 96.


