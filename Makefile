# Choose a C compiler. (Must be ANSI C capable).
CC = cc
# CC = gcc -ffloat-store -frounding-math

# For a well-optimised version compiled with gcc on a Sun use:
# CFLAGS = -O2 -DSUN -finline -finline-functions -fexpensive-optimizations
# For a reasonably optimised version for NetBSD.
CFLAGS = -O2 -Wall -DCURTERM=1
# For a reasonably optimised version for SunOS 4.1.4.
# CFLAGS = -O2 -Wall -DSUN

# Put any additional libraries here.
LFLAGS	= -lm

SRC	= curterm.c arithmetic.c fparithmetic.c server.c p.c main.c
OBJ	= curterm.o arithmetic.o fparithmetic.o server.o p.o main.o

$(BIN)/jserver : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LFLAGS) -o t4

clean	:
	rm -f *.o
