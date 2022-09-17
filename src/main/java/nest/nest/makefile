# The Mona nesting birds program makefile

CC = gcc

# MONA_TRACE: tracing.
# VIEWER: produces pre-filtered output for NestViewer program.
# ACTIVITY_TRACKING: tracks firing, enablement, and drive activity for network dump.
#CCFLAGS = -DUNIX -DNEST_ENV -DMONA_TRACE -DVIEWER -DACTIVITY_TRACKING
CCFLAGS = -DUNIX -DNEST_ENV -DVIEWER -DACTIVITY_TRACKING

all: ../../bin/nest

../../bin/nest: nest.o monami.o male.o female.o environment.o \
	../mona/mona.hpp ../mona/mona.cpp ../mona/sense.cpp ../mona/enable.cpp \
	../mona/drive.cpp ../mona/respond.cpp ../mona/utility.cpp \
	../common/pointerlist.o ../common/valueset.o ../common/point.o ../common/random.o
	/bin/cp ../mona/mona.hpp .
	/bin/cp ../mona/mona.cpp .
	$(CC) $(CCFLAGS) -c mona.cpp
	/bin/cp ../mona/sense.cpp .
	$(CC) $(CCFLAGS) -c sense.cpp
	/bin/cp ../mona/enable.cpp .
	$(CC) $(CCFLAGS) -c enable.cpp
	/bin/cp ../mona/drive.cpp .
	$(CC) $(CCFLAGS) -c drive.cpp
	/bin/cp ../mona/respond.cpp .
	$(CC) $(CCFLAGS) -c respond.cpp
	/bin/cp ../mona/utility.cpp .
	$(CC) $(CCFLAGS) -c utility.cpp
	$(CC) -o ../../bin/nest mona.o sense.o enable.o drive.o respond.o utility.o \
		../common/pointerlist.o ../common/valueset.o ../common/point.o ../common/random.o \
		monami.o male.o female.o environment.o nest.o -lm -lstdc++
	/bin/rm -f mona.hpp mona.cpp mona.o sense.cpp sense.o enable.cpp enable.o \
		drive.cpp drive.o respond.cpp respond.o utility.cpp utility.o

monami.o: ../mona/mona.hpp environment.hpp monami.hpp monami.cpp
	$(CC) $(CCFLAGS) -c monami.cpp

male.o: ../mona/mona.hpp environment.hpp monami.hpp male.hpp male.cpp
	$(CC) $(CCFLAGS) -c male.cpp

female.o: ../mona/mona.hpp environment.hpp monami.hpp female.hpp female.cpp
	$(CC) $(CCFLAGS) -c female.cpp

environment.o: environment.hpp environment.cpp
	$(CC) $(CCFLAGS) -c environment.cpp

nest.o: ../mona/mona.hpp monami.hpp male.hpp female.hpp environment.hpp nest.cpp
	$(CC) $(CCFLAGS) -c nest.cpp

clean:
	/bin/rm -f *.o
