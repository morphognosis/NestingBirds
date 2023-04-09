// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The female bird definitions

#ifndef __FEMALE__
#define __FEMALE__

#include "object.hpp"
#include "locale.hpp"
#include "orientation.hpp"
#include "../mona/mona.hpp"

class Female
{
public:

    // Sensors.
    class CELL_SENSOR
    {
    public:
        static const int NUM_SENSORS = 2;
        int              locale;
        int              object;
    };

    // Bird senses current, left, front, right, and rear cells.
    static const int NUM_CELL_SENSORS = 9;
    static const int CURRENT_LOCALE_SENSOR = 0;
    static const int CURRENT_OBJECT_SENSOR = 1;
    static const int LEFT_LOCALE_SENSOR = 2;
    static const int LEFT_OBJECT_SENSOR = 3;
    static const int LEFT_FRONT_LOCALE_SENSOR = 4;
    static const int LEFT_FRONT_OBJECT_SENSOR = 5;
    static const int FRONT_LOCALE_SENSOR = 6;
    static const int FRONT_OBJECT_SENSOR = 7;
    static const int RIGHT_FRONT_LOCALE_SENSOR = 8;
    static const int RIGHT_FRONT_OBJECT_SENSOR = 9;
    static const int RIGHT_LOCALE_SENSOR = 10;
    static const int RIGHT_OBJECT_SENSOR = 11;
    static const int RIGHT_REAR_LOCALE_SENSOR = 12;
    static const int RIGHT_REAR_OBJECT_SENSOR = 13;
    static const int REAR_LOCALE_SENSOR = 14;
    static const int REAR_OBJECT_SENSOR = 15;
    static const int LEFT_REAR_LOCALE_SENSOR = 16;
    static const int LEFT_REAR_OBJECT_SENSOR = 17;

    // State sensors.
    static const int ORIENTATION_SENSOR = NUM_CELL_SENSORS * CELL_SENSOR::NUM_SENSORS;
    static const int HUNGER_SENSOR = ORIENTATION_SENSOR + 1;
    static const int HAS_OBJECT_SENSOR = HUNGER_SENSOR + 1;
    static const int NUM_SENSORS = HAS_OBJECT_SENSOR + 1;

    // Sensors.
    int              sensors[NUM_SENSORS];

    // State.
    int x, y;
    int orientation;
    int food;
    int hasObject;

    // Responses.
    class RESPONSE
    {
    public:
        static const int DO_NOTHING = 0;
        static const int MOVE_FORWARD = 1;
        static const int TURN_RIGHT = 2;
        static const int TURN_LEFT = 3;
        static const int TURN_AROUND = 4;
        static const int EAT_MOUSE = 5;
        static const int GET_OBJECT = 6;
        static const int PUT_OBJECT = 7;
        static const int TOSS_OBJECT = 8;
        static const int WANT_MOUSE = 9;
        static const int WANT_STONE = 10;
        static const int LAY_EGG = 11;
        static const int NUM_RESPONSES = 12;

        // Response to string.
        static const char* toString(int response)
        {
            switch (response)
            {
            case DO_NOTHING:
                return("DO_NOTHING");

            case EAT_MOUSE:
                return("EAT_MOUSE");

            case GET_OBJECT:
                return("GET_OBJECT");

            case PUT_OBJECT:
                return("PUT_OBJECT");

            case TOSS_OBJECT:
                return("TOSS_OBJECT");

            case MOVE_FORWARD:
                return("MOVE_FORWARD");

            case TURN_RIGHT:
                return("TURN_RIGHT");

            case TURN_LEFT:
                return("TURN_LEFT");

            case TURN_AROUND:
                return("TURN_AROUND");

            case WANT_MOUSE:
                return("WANT_MOUSE");

            case WANT_STONE:
                return("WANT_STONE");

            case LAY_EGG:
                return("LAY_EGG");

            default:
                return("Unknown response");
            }
        }
    };

    // Response.
    int response;

   // Food.
#define FEMALE_DEFAULT_FOOD_DURATION    "100"
   static int FOOD_DURATION;
#define FEMALE_DEFAULT_INITIAL_FOOD     "100"
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

   // Brain.
   Mona* brain;

   // Constructor.
   Female();

   // Needs.
   void initNeeds();
   void setNeeds();

   // Set response override.
   void setResponseOverride();

   // Cycle.
   int cycle();

   // Digest food.
   void digest();

   // Load.
   void load(char *filename);

   // Save.
   void save(char *filename);

   // Print.
   void print();

   // Print sensors.
   void printSensors();

   // Print state.
   void printState();

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
        bool hasObject);

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
        Mona::SENSOR hasObject);
};
#endif
