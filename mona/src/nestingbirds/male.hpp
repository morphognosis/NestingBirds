// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The male bird definitions

#ifndef __MALE__
#define __MALE__

#include "bird.hpp"
#include "../mona/mona.hpp"

class Male : public Bird
{
public:

    // Sensors.
    static const int WANT_FOOD_SENSOR = Bird::NUM_SENSORS;
    static const int WANT_STONE_SENSOR = Bird::NUM_SENSORS + 1;
    static const int NUM_SENSORS = Bird::NUM_SENSORS + 2;
    int sensors[NUM_SENSORS];

    // Responses.
    class RESPONSE
    {
    public:
        static const int GIVE_FOOD = 8;
        static const int GIVE_STONE = 9;
        static const int NUM_RESPONSES = 2;

        // Response to string.
        static string toString(int response)
        {
            switch (response)
            {
            case GIVE_FOOD:
                return("GIVE_FOOD");

            case GIVE_STONE:
                return("GIVE_STONE");

            default:
                return(Bird::RESPONSE::toString(response));
            }
        }
    };
    int response;

    // Food.
    static int     FOOD_DURATION;
    static int     INITIAL_FOOD;
    static bool RANDOMIZE_FOOD_LEVEL;

	// Needs.
	static Mona::NEED MOUSE_NEED;
	static Mona::NEED FOOD_NEED;
	static Mona::NEED FEMALE_FOOD_NEED;
	static Mona::NEED STONE_NEED;
	static Mona::NEED FEMALE_STONE_NEED;
	static Mona::NEED ATTEND_FEMALE_NEED;

	// Constructor.
	Male();

    // Cycle.
    int cycle();

    // Print.
    void print();

    // Print sensors.
    void printSensors();

    // Sensors to string.
    string sensorsToString();

    // Print response.
    void printResponse();
};
#endif
