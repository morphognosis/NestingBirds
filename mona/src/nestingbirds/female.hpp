// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The female bird definitions

#ifndef __FEMALE__
#define __FEMALE__

#include "bird.hpp"
#include "../mona/mona.hpp"

class Female : public Bird
{
public:

    // Sensors.
    static const int NUM_SENSORS = Bird::NUM_SENSORS;
    int sensors[NUM_SENSORS];

    // Responses.
    class RESPONSE
    {
    public:
        static const int WANT_FOOD = 8;
        static const  int WANT_STONE = 9;
        static const  int LAY_EGG = 10;
        static const  int NUM_RESPONSES = 3;

        // Response to string.
        static string toString(int response)
        {
            switch (response)
            {
            case WANT_FOOD:
                return("WANT_FOOD");

            case WANT_STONE:
                return("WANT_STONE");

            case LAY_EGG:
                return("LAY_EGG");

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
	static const Mona::NEED FOOD_NEED;
	static const Mona::NEED STONE_NEED;
	static const Mona::NEED EGG_NEED;

	// Constructor.
	Female();

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
