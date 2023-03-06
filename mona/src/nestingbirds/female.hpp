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
      static const char *toString(int response)
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
#define FEMALE_DEFAULT_FOOD_DURATION    "75"
   static int FOOD_DURATION;
#define FEMALE_DEFAULT_INITIAL_FOOD     "75"
   static int  INITIAL_FOOD;
   static bool RANDOMIZE_FOOD_LEVEL;

   // Needs.
   static const int MOUSE_NEED_INDEX = 0;
#define FEMALE_DEFAULT_MOUSE_NEED    "0.3"
   static Mona::NEED MOUSE_NEED;
   static const int STONE_NEED_INDEX = 1;
#define FEMALE_DEFAULT_STONE_NEED    "0.1"
   static Mona::NEED STONE_NEED;
   static const int LAY_EGG_NEED_INDEX = 2;
#define FEMALE_DEFAULT_LAY_EGG_NEED      "0.2"
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
 
private:

    // Load mask.
    void loadMask(vector<bool>& mask,
        bool currentLocale, bool currentObject,
        bool leftLocale, bool leftObject,
        bool leftFrontLocale, bool leftFrontObject,
        bool frontLocale, bool frontObject,
        bool rightFrontLocale, bool rightFrontObject,
        bool rightLocale, bool rightObject,
        bool rightRearLocale, bool rightRearObject,
        bool rearLocale, bool rearObject,
        bool leftRearLocale, bool leftRearObject,
        bool orientation, bool hunger,
        bool hasObject, bool state);

    // Load sensors.
    void loadSensors(vector<Mona::SENSOR>& sensors,
        Mona::SENSOR currentLocale, Mona::SENSOR currentObject,
        Mona::SENSOR leftLocale, Mona::SENSOR leftObject,
        Mona::SENSOR leftFrontLocale, Mona::SENSOR leftFrontObject,
        Mona::SENSOR frontLocale, Mona::SENSOR frontObject,
        Mona::SENSOR rightFrontLocale, Mona::SENSOR rightFrontObject,
        Mona::SENSOR rightLocale, Mona::SENSOR rightObject,
        Mona::SENSOR rightRearLocale, Mona::SENSOR rightRearObject,
        Mona::SENSOR rearLocale, Mona::SENSOR rearObject,
        Mona::SENSOR leftRearLocale, Mona::SENSOR leftRearObject,
        Mona::SENSOR orientation, Mona::SENSOR hunger,
        Mona::SENSOR hasObject, Mona::SENSOR state);
};
#endif
