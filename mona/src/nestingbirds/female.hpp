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
      static const int WANT_MOUSE    = Bird::RESPONSE::NUM_RESPONSES;
      static const int WANT_STONE    = WANT_MOUSE + 1;
      static const int LAY_EGG       = WANT_STONE + 1;
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
#define FEMALE_DEFAULT_INITIAL_FOOD     "200"
   static int  INITIAL_FOOD;
   static bool RANDOMIZE_FOOD_LEVEL;

   // Needs.
   static const int MOUSE_NEED_INDEX = 0;
#define FEMALE_DEFAULT_MOUSE_NEED    "0.3"
   static Mona::NEED MOUSE_NEED;
   static const int STONE_NEED_INDEX = 1;
#define FEMALE_DEFAULT_STONE_NEED    "0.2"
   static Mona::NEED STONE_NEED;
   static const int LAY_EGG_NEED_INDEX = 2;
#define FEMALE_DEFAULT_LAY_EGG_NEED      "0.1"
   static Mona::NEED LAY_EGG_NEED;
   static const int BROOD_EGG_NEED_INDEX = 3;
#define FEMALE_DEFAULT_BROOD_EGG_NEED      "0.4"
   static Mona::NEED BROOD_EGG_NEED;
   static const int NUM_NEEDS = 4;

   // Constructor.
   Female();

   // Needs.
   void initNeeds();
   void setNeeds();

   // Set response override.
   void setResponseOverride();

   // Cycle.
   int cycle();

   // Load.
   void load(char *filename);

   // Save.
   void save(char *filename);

   // Print.
   void print();

   // Print sensors.
   void printSensors();

   // Print needs.
   void printNeeds();

   // Print response.
   void printResponse();

   // Motors:
   Mona::Motor* doNothing;
   Mona::Motor* eat;
   Mona::Motor* get;
   Mona::Motor* put;
   Mona::Motor* toss;
   Mona::Motor* move;
   Mona::Motor* turnRight;
   Mona::Motor* turnLeft;
   Mona::Motor* turnAround;
   Mona::Motor* stateOn;
   Mona::Motor* stateOff;
   Mona::Motor* wantMouse;
   Mona::Motor* wantStone;
   Mona::Motor* layEgg;

   // Goals.

    // Food.
   int wantMouseGoal;
   int eatMouseGoal;

   // Stone.
   int wantStoneGoal;
   int putStoneGoal;

   // Lay egg.
   int layEggGoal;

   // Brooding on egg.
   int broodEggGoal;
 
    // Load sensors.
    void loadSensors(vector<Mona::SENSOR>& sensors,
        Mona::SENSOR currentLocale, Mona::SENSOR currentObject,
        Mona::SENSOR leftLocale, Mona::SENSOR leftObject,
        Mona::SENSOR frontLocale, Mona::SENSOR frontObject,
        Mona::SENSOR rightLocale, Mona::SENSOR rightObject,
        Mona::SENSOR rearLocale, Mona::SENSOR rearObject,
        Mona::SENSOR orienatation, Mona::SENSOR hungerSensor,
        Mona::SENSOR hasObject, Mona::SENSOR stateSensor);
};
#endif
