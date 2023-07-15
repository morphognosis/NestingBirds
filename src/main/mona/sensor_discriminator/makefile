# Sensor discriminator makefile

CC = gcc

all: ../../../../bin/sensor_discriminator

../../../../bin/sensor_discriminator: NeuralNetwork.o SensorDiscriminator.o
	$(CC) -o ../../../../bin/sensor_discriminator NeuralNetwork.o SensorDiscriminator.o -lstdc++

NeuralNetwork.o: NeuralNetwork.h NeuralNetwork.cpp
	$(CC) $(CCFLAGS) -c NeuralNetwork.cpp

SensorDiscriminator.o: NeuralNetwork.h SensorDiscriminator.cpp
	$(CC) $(CCFLAGS) -c SensorDiscriminator.cpp

clean:
	/bin/rm -f *.o
