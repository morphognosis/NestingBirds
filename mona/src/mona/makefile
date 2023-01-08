# Mona makefile

MONA_SOURCES = mona.cpp sense.cpp enable.cpp drive.cpp \
           respond.cpp learn.cpp homeostat.cpp

MONA_OBJECTS = $(MONA_SOURCES:%.cpp=%.o)

CC = gcc

all: $(MONA_OBJECTS)

mona.o: mona.hpp mona-aux.hpp mona.cpp
	$(CC) $(CCFLAGS) -c mona.cpp

sense.o: mona.hpp mona-aux.hpp sense.cpp
	$(CC) $(CCFLAGS) -c sense.cpp

enable.o: mona.hpp mona-aux.hpp enable.cpp
	$(CC) $(CCFLAGS) -c enable.cpp

drive.o: mona.hpp mona-aux.hpp drive.cpp
	$(CC) $(CCFLAGS) -c drive.cpp

learn.o: mona.hpp mona-aux.hpp learn.cpp
	$(CC) $(CCFLAGS) -c learn.cpp

respond.o: mona.hpp mona-aux.hpp respond.cpp
	$(CC) $(CCFLAGS) -c respond.cpp

homeostat.o: mona.hpp homeostat.hpp homeostat.cpp
	$(CC) $(CCFLAGS) -c homeostat.cpp

clean:
	/bin/rm -f *.o

