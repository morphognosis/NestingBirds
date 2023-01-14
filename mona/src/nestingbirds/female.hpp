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
   int              sensors[NUM_SENSORS];

   // Responses.
   class RESPONSE
   {
public:
      static const int WANT_MOUSE    = 10;
      static const int WANT_STONE    = 11;
      static const int LAY_EGG       = 12;
      static const int NUM_RESPONSES = 3;

      // Response to string.
      static char *toString(int response)
      {
         switch (response)
         {
         case WANT_MOUSE:
            return("WANT_MOUSE");

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
#define FEMALE_DEFAULT_FOOD_DURATION    "200"
   static int FOOD_DURATION;
#define FEMALE_DEFAULT_INITIAL_FOOD     FEMALE_DEFAULT_FOOD_DURATION
   static int  INITIAL_FOOD;
   static bool RANDOMIZE_FOOD_LEVEL;

   // Needs.
#define FEMALE_DEFAULT_MOUSE_NEED    "100.0"
   static Mona::NEED MOUSE_NEED;
#define FEMALE_DEFAULT_STONE_NEED    "0.0"
   static Mona::NEED STONE_NEED;
#define FEMALE_DEFAULT_EGG_NEED      "5.0"
   static Mona::NEED EGG_NEED;

   // Constructor.
   Female();

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
