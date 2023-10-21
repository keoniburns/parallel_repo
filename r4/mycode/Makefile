MPICC = mpicxx
CPP = g++
CC = gcc
CFLAGS= -g -Wall $(INCLUDE_DIRS) $(CDEFS)
MPIFLAGS= -g -Wall -01
CFLAGS2= -O0 -g -Wall $(INCLUDE_DIRS) $(CDEFS)

PRODUCT= ex1 exercise1 ex2 exercise2
MPIFILES= ex2.clean
CPPFILES= exercise1.cpp exercise2.cpp
CFILES= ex1.c ex2.c
SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	${PRODUCT}

clean:
	-rm -f *.o *.NEW *~
	-rm -f ${PRODUCT} ${DERIVED} ${GARBAGE}

exercise1: exercise1.cpp
	$(CPP) $(CFLAGS2) -o $@ exercise1.cpp -lm

# exercise2: exercise2.cpp
# 	$(CPP) $(CFLAGS2) -o $@ exercise2.cpp -$(LIB_DIRS)

ex1: ex1.c
	$(CC) $(CFLAGS2) -o $@ ex1.c -lm

ex2: ex2.c
	$(MPICC) $(MPIFLAGS) -o $@ ex2.c
