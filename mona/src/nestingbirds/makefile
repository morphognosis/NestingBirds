# The Mona nesting birds makefile

CC = gcc

all: ../../bin/nestingbirds

../../bin/nestingbirds: NestingBirds.o male.o female.o
	$(CC) -o ../../bin/nestingbirds \
		fileio.o gettime.o log.o md5.o quaternion.o random.o RDtree.o \
		drive.o enable.o homeostat.o learn.o mona.o respond.o sense.o \
		male.o female.o NestingbBirds.o -lm -lstdc++

male.o: ../mona/mona.hpp male.hpp male.cpp NestingBirds.cpp
	$(CC) $(CCFLAGS) -c male.cpp

female.o: ../mona/mona.hpp female.hpp female.cpp NestingBirds.cpp
	$(CC) $(CCFLAGS) -c female.cpp

NestingBirds.o: ../mona/mona.hpp male.hpp female.hpp NestingBirds.cpp
	$(CC) $(CCFLAGS) -c NestingBirds.cpp

clean:
	/bin/rm -f *.o

