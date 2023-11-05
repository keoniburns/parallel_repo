INCLUDE_DIRS = -I/opt/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/include/
LIB_DIRS = -L/opt/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/lib/debug -L/opt/intel/compilers_and_libraries_2020.0.166/linux/mpi/intel64/lib
MPICC = mpicc
CPP = g++
CC = gcc
CFLAGS= -g -Wall $(INCLUDE_DIRS) $(CDEFS)
MPIFLAGS= -g -Wall -O2 $(INCLUDE_DIRS) 
CFLAGS2= -O0 -g -Wall $(INCLUDE_DIRS) $(CDEFS)
OMPFLAGS = -g -Wall 

PRODUCT= ex2 ex3 gewpp ex4 piseriesreduce
# MPIFILES= ex2.c
CPPFILES= ex2.cpp ex3.cpp ex4.cpp 
CFILES= gewpp.c piseriesreduce.c
SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	${PRODUCT}

clean:
	-rm -f *.o *.NEW *~
	-rm -f ${PRODUCT} ${DERIVED} ${GARBAGE}

ex2: ex2.cpp
	$(CPP) $(OMPFLAGS) -o $@ ex2.cpp -fopenmp

ex3: ex3.cpp
	$(CPP) $(OMPFLAGS) -o $@ ex3.cpp -fopenmp

gewpp: gewpp.c
	$(CC) $(CFLAGS2) -o $@ gewpp.c

ex4: ex4.cpp
	$(CPP) $(OMPFLAGS) -o $@ ex4.cpp -fopenmp

piseriesreduce: piseriesreduce.c
	$(MPICCCC) $(MPIFLAGS) -o $@ piseriesreduce.c $(LIB_DIRS) -lm


