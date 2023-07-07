# Pattern detector makefile

CC = gcc

all: ../../../../bin/pattern_detector

../../../../bin/pattern_detector: NeuralNetwork.o PatternDetector.o
	$(CC) -o ../../../../bin/pattern_detector NeuralNetwork.o PatternDetector.o -lstdc++

NeuralNetwork.o: NeuralNetwork.h NeuralNetwork.cpp
	$(CC) $(CCFLAGS) -c NeuralNetwork.cpp

PatternDetector.o: NeuralNetwork.h PatternDetector.cpp
	$(CC) $(CCFLAGS) -c PatternDetector.cpp

clean:
	/bin/rm -f *.o
