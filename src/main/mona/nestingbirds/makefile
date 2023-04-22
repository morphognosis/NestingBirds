# The nesting birds makefile

CC = gcc

all: ../../../../bin/nestingbirds

../../../../bin/nestingbirds: NestingBirds.o male.o female.o \
		../common/fileio.o ../common/gettime.o ../common/log.o ../common/md5.o ../common/quaternion.o ../common/random.o ../common/RDtree.o \
		../mona/drive.o ../mona/enable.o ../mona/homeostat.o ../mona/learn.o ../mona/mona.o ../mona/respond.o ../mona/sense.o
	$(CC) -o ../../../../bin/nestingbirds NestingBirds.o male.o female.o \
		../common/fileio.o ../common/gettime.o ../common/log.o ../common/md5.o ../common/quaternion.o ../common/random.o ../common/RDtree.o \
		../mona/drive.o ../mona/enable.o ../mona/homeostat.o ../mona/learn.o ../mona/mona.o ../mona/respond.o ../mona/sense.o \
		-lm -lstdc++

male.o: ../mona/mona.hpp male.hpp male.cpp object.hpp locale.hpp orientation.hpp
	$(CC) $(CCFLAGS) -c male.cpp

female.o: ../mona/mona.hpp female.hpp female.cpp object.hpp locale.hpp orientation.hpp
	$(CC) $(CCFLAGS) -c female.cpp

NestingBirds.o: ../mona/mona.hpp male.hpp female.hpp object.hpp locale.hpp orientation.hpp NestingBirds.cpp
	$(CC) $(CCFLAGS) -c NestingBirds.cpp

clean:
	/bin/rm -f *.o

