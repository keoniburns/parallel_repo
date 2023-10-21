INCLUDE_DIRS = -I/opt/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/include/
LIB_DIRS = -L/opt/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/lib/debug -L/opt/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/lib
MPICC = mpicc
CPP = g++
CC = gcc
CFLAGS= -g -Wall $(INCLUDE_DIRS) $(CDEFS)
MPIFLAGS= -g -Wall -O2 $(INCLUDE_DIRS) $(CDEFS)
CFLAGS2= -O0 -g -Wall $(INCLUDE_DIRS) $(CDEFS)

PRODUCT= ex1 exercise1 ex2 exercise2
HFILES=
CFILES=
HPPFILES=
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
	$(MPICC) $(MPIFLAGSS) -o $@ ex2.c $(LIB_DIRS)
