// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The male bird definitions

#ifndef __MALE__
#define __MALE__

#include "bird.hpp"
#include "../mona/mona.hpp"

class Male : public Bird
{
public:

    // Mate proximity.
    static const int MATE_PROXIMITY_UNKNOWN = -1;
    static const int MATE_PROXIMITY_PRESENT = 0;
    static const int MATE_PROXIMITY_LEFT = 1;
    static const int MATE_PROXIMITY_FRONT = 2;
    static const int MATE_PROXIMITY_REAR = 3;
    static const int MATE_PROXIMITY_RIGHT = 4;

   // Sensors.
   static const int MATE_PROXIMITY_SENSOR = Bird::NUM_SENSORS;
   static const int WANT_MOUSE_SENSOR = Bird::NUM_SENSORS + 1;
   static const int WANT_STONE_SENSOR = Bird::NUM_SENSORS + 2;
   static const int NUM_SENSORS       = Bird::NUM_SENSORS + 3;
   int              sensors[NUM_SENSORS];

   // Responses.
   class RESPONSE
   {
public:
      static const int GIVE_MOUSE    = 10;
      static const int GIVE_STONE    = 11;
      static const int NUM_RESPONSES = 2;

      // Response to string.
      static char *toString(int response)
      {
         switch (response)
         {
         case GIVE_MOUSE:
            return("GIVE_MOUSE");

         case GIVE_STONE:
            return("GIVE_STONE");

         default:
            return(Bird::RESPONSE::toString(response));
         }
      }
   };
   int response;

   // Food.
#define MALE_DEFAULT_FOOD_DURATION    "200"
   static int FOOD_DURATION;
#define MALE_DEFAULT_INITIAL_FOOD     MALE_DEFAULT_FOOD_DURATION
   static int  INITIAL_FOOD;
   static bool RANDOMIZE_FOOD_LEVEL;

   // Needs.
   static const int MOUSE_NEED_INDEX = 0;
#define MALE_DEFAULT_MOUSE_NEED            "0.5"
   static Mona::NEED MOUSE_NEED;
   static const int FEMALE_MOUSE_NEED_INDEX = 1;
#define MALE_DEFAULT_FEMALE_MOUSE_NEED     "0.4"
   static Mona::NEED FEMALE_MOUSE_NEED;
   static const int STONE_NEED_INDEX = 2;
#define MALE_DEFAULT_STONE_NEED            "0.2"
   static Mona::NEED STONE_NEED;
   static const int FEMALE_STONE_NEED_INDEX = 3;
#define MALE_DEFAULT_FEMALE_STONE_NEED     "0.3"
   static Mona::NEED FEMALE_STONE_NEED;
   static const int ATTEND_FEMALE_NEED_INDEX = 4;
#define MALE_DEFAULT_ATTEND_FEMALE_NEED    "0.1"
   static Mona::NEED ATTEND_FEMALE_NEED;
   static const int NUM_NEEDS = 5;

   // Constructor.
   Male();

   // Cycle.
   int cycle();

   // Print.
   void print();

   // Print sensors.
   void printSensors();

   // Print response.
   void printResponse();
};
#endif
