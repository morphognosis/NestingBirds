# Common makefile

COMMON_SOURCES = random.cpp fileio.cpp gettime.cpp log.cpp \
           quaternion.cpp md5.cpp RDtree.cpp

COMMON_OBJECTS = $(COMMON_SOURCES:%.cpp=%.o)

CC = gcc

all : $(COMMON_OBJECTS)

random.o: random.hpp random.cpp
	$(CC) $(CCFLAGS) -c random.cpp

fileio.o: fileio.h fileio.cpp
	$(CC) $(CCFLAGS) -c fileio.cpp

gettime.o: gettime.h gettime.cpp
	$(CC) $(CCFLAGS) -c gettime.cpp
	
log.o: log.cpp log.hpp
	$(CC) $(CCFLAGS) -c log.cpp

quaternion.o: quaternion.cpp quaternion.hpp
	$(CC) $(CCFLAGS) -c quaternion.cpp

md5.o: md5.cpp md5.h
	$(CC) $(CCFLAGS) -c md5.cpp

RDtree.o: RDtree.cpp RDtree.hpp
	$(CC) $(CCFLAGS) -c RDtree.cpp

clean:
	/bin/rm -f *.o
