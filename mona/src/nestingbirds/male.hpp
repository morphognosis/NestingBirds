// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The male bird definitions

#ifndef __MALE__
#define __MALE__

#include "object.hpp"
#include "locale.hpp"
#include "orientation.hpp"
#include "../mona/mona.hpp"

class Male
{
public:

    // Proximity.
    class PROXIMITY
    {
    public:
        static const int UNKNOWN = 0;
        static const int PRESENT = 1;
        static const int LEFT = 2;
        static const int FRONT = 3;
        static const int RIGHT = 4;
        static const int REAR = 5;

        // Proximity to string.
        static const char* toString(int proximity)
        {
            switch (proximity)
            {

            case PRESENT:
                return("PRESENT");

            case LEFT:
                return("LEFT");

            case FRONT:
                return("FRONT");

            case RIGHT:
                return("RIGHT");

            case REAR:
                return("REAR");

            default:
                return("UNKNOWN");
            }
        }
    };

    // Sensors.
    static const int LOCALE_SENSOR = 0;
    static const int MOUSE_PROXIMITY_SENSOR = 1;
    static const int STONE_PROXIMITY_SENSOR = 2;
    static const int FEMALE_PROXIMITY_SENSOR = 3;
    static const int HUNGER_SENSOR = 4;
    static const int HAS_OBJECT_SENSOR = 5;
    static const int WANT_MOUSE_SENSOR = 6;
    static const int WANT_STONE_SENSOR = 7;
    static const int NUM_SENSORS = 8;
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
        static const int GIVE_MOUSE = 9;
        static const int GIVE_STONE = 10;
        static const int NUM_RESPONSES = 11;

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

            case GIVE_MOUSE:
                return("GIVE_MOUSE");

            case GIVE_STONE:
                return("GIVE_STONE");

            default:
                return("Unknown response");
            }
        }
    };

    // Response.
    int response;

   // Food.
#define MALE_DEFAULT_FOOD_DURATION    "200"
   static int FOOD_DURATION;
#define MALE_DEFAULT_INITIAL_FOOD     "0"
   static int  INITIAL_FOOD;
   static bool RANDOMIZE_FOOD_LEVEL;

   // Needs.
   static const int MOUSE_NEED_INDEX = 0;
#define MALE_DEFAULT_MOUSE_NEED    "0.4"
   static Mona::NEED MOUSE_NEED;
   static const int FEMALE_MOUSE_NEED_INDEX = 1;
#define MALE_DEFAULT_FEMALE_MOUSE_NEED    "0.3"
   static Mona::NEED FEMALE_MOUSE_NEED;
   static const int FEMALE_STONE_NEED_INDEX = 2;
#define MALE_DEFAULT_FEMALE_STONE_NEED    "0.2"
   static Mona::NEED FEMALE_STONE_NEED;
   static const int ATTEND_FEMALE_NEED_INDEX = 3;
#define MALE_DEFAULT_ATTEND_FEMALE_NEED      "0.1"
   static Mona::NEED ATTEND_FEMALE_NEED;
   static const int NUM_NEEDS = 4;

   // Brain.
   Mona* brain;

   // Constructor.
   Male();

   // Needs.
   void initNeeds();
   void setNeeds();

   // Set response override.
   void setResponseOverride();

   // Cycle.
   int cycle();

   // Post-response.
   void postResponse();

   // Digest food.
   void digest();

   // Load.
   void load(char* filename);

   // Save.
   void save(char* filename);

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
        bool currentLocale,
        bool mouseProximity, bool stoneProximity, bool femaleProximity,
        bool hunger, bool hasObject,
        bool wantMouse, bool wantStone);

    // Load sensors.
    void loadSensors(vector<Mona::SENSOR>& sensors,
        Mona::SENSOR currentLocale,
        Mona::SENSOR mouseProximity, Mona::SENSOR stoneProximity, Mona::SENSOR femaleProximity,
        Mona::SENSOR hunger, Mona::SENSOR hasObject,
        Mona::SENSOR wantMouse, Mona::SENSOR wantStone);
};
#endif
