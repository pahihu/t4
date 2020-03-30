# Choose a C compiler. (Must be ANSI C capable).
CC = cc
#
# 64bit Linux-x86 emulator, 32bit Linux-armhf emulator
# CC = gcc -ffloat-store -frounding-math
#
# 32bit Linux-x86 emulator
# CC = gcc -m32 -mfpmath=sse -msse2 -ffloat-store -frounding-math
#
# 32bit/64bit Linux-POWER emulator
# CC = gcc [-m64] -ffloat-store -frounding-math -std=c99
#

# For a well-optimised version compiled with gcc on a Sun use:
# CFLAGS = -O2 -DSUN -finline -finline-functions -fexpensive-optimizations
# For a reasonably optimised version for NetBSD.
CFLAGS = -I. -O2 -Wall -DCURTERM=1
# For a reasonably optimised version for SunOS 4.1.4.
# CFLAGS = -O2 -Wall -DSUN

# Put any additional libraries here.
LFLAGS	= -lm

#
# https://github.com/JuliaMath/openlibm
#
FDMSRC  = k_standard.c s_scalbn.c s_scalbnf.c s_ldexp.c s_ldexpf.c \
	  e_fmod.c e_remainder.c e_sqrt.c w_remainder.c w_sqrt.c
FDMOBJ	= k_standard.o s_scalbn.o s_scalbnf.o s_ldexp.o s_ldexpf.o \
	  e_fmod.o e_remainder.o e_sqrt.o w_remainder.o w_sqrt.o

SRC	= $(FDMSRC) curterm.c arithmetic.c fparithmetic.c server.c p.c main.c
OBJ	= $(FDMOBJ) curterm.o arithmetic.o fparithmetic.o server.o p.o main.o

t4 : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LFLAGS) -o t4

clean	:
	rm -f *.o
