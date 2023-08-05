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

   // Bird senses object in current, left, left front,
   // front, right front, right, right rear, rear, and left rear cells.
   static const int CURRENT_OBJECT_SENSOR     = 0;
   static const int LEFT_OBJECT_SENSOR        = 1;
   static const int LEFT_FRONT_OBJECT_SENSOR  = 2;
   static const int FRONT_OBJECT_SENSOR       = 3;
   static const int RIGHT_FRONT_OBJECT_SENSOR = 4;
   static const int RIGHT_OBJECT_SENSOR       = 5;
   static const int RIGHT_REAR_OBJECT_SENSOR  = 6;
   static const int REAR_OBJECT_SENSOR        = 7;
   static const int LEFT_REAR_OBJECT_SENSOR   = 8;
   static const int NUM_CELL_SENSORS          = 9;

   // State sensors.
   static const int ORIENTATION_SENSOR = 9;
   static const int GOAL_SENSOR        = 10;
   static const int HAS_OBJECT_SENSOR  = 11;
   static const int NUM_SENSORS        = 12;

   // Sensors.
   int sensors[NUM_SENSORS];

   // Goal.
   class GOAL
   {
public:
      static const int LAY_EGG   = 0;
      static const int BROOD_EGG = 1;
      static const int EAT_MOUSE = 2;

      // Goal to string.
      static const char *toString(int goal)
      {
         switch (goal)
         {
         case LAY_EGG:
            return("LAY_EGG");

         case BROOD_EGG:
            return("BROOD_EGG");

         case EAT_MOUSE:
            return("EAT_MOUSE");

         default:
            return("UNKNOWN");
         }
      }
   };

   // State.
   int x, y;
   int orientation;
   int food;
   int goal;
   int hasObject;

   // Responses.
   class RESPONSE
   {
public:
      static const int DO_NOTHING    = Mona::MOVEMENT_TYPE::DO_NOTHING;
      static const int MOVE_FORWARD  = Mona::MOVEMENT_TYPE::MOVE_FORWARD;
      static const int TURN_RIGHT    = Mona::MOVEMENT_TYPE::TURN_RIGHT;
      static const int TURN_LEFT     = Mona::MOVEMENT_TYPE::TURN_LEFT;
      static const int EAT_MOUSE     = 4;
      static const int GET_OBJECT    = 5;
      static const int PUT_OBJECT    = 6;
      static const int TOSS_OBJECT   = 7;
      static const int WANT_MOUSE    = 8;
      static const int WANT_STONE    = 9;
      static const int LAY_EGG       = 10;
      static const int NUM_RESPONSES = 11;

      // Response to string.
      static const char *toString(int response)
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
#define FEMALE_DEFAULT_FOOD_DURATION    "200"
   static int FOOD_DURATION;
#define FEMALE_DEFAULT_INITIAL_FOOD     "200"
   static int  INITIAL_FOOD;
   static bool RANDOMIZE_FOOD_LEVEL;

   // Needs.
   static const int LAY_EGG_NEED_INDEX = 0;
#define FEMALE_DEFAULT_LAY_EGG_NEED      "0.1"
   static Mona::NEED LAY_EGG_NEED;
   static const int  BROOD_EGG_NEED_INDEX = 1;
#define FEMALE_DEFAULT_BROOD_EGG_NEED    "0.2"
   static Mona::NEED BROOD_EGG_NEED;
   static const int  MOUSE_NEED_INDEX = 2;
#define FEMALE_DEFAULT_MOUSE_NEED        "0.3"
   static Mona::NEED MOUSE_NEED;
   static const int  NUM_NEEDS = 3;

   // Brain.
   Mona *brain;

   // Verbosity.
   bool Verbose;

   // Constructor.
   Female();

   // Destructor.
   ~Female();

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

   // Print state.
   void printState(FILE *fp = stdout);

   // Print sensors.
   void printSensors(FILE *fp = stdout);

   // Print needs.
   void printNeeds(FILE *fp = stdout);

   // Print response.
   void printResponse(FILE *fp = stdout);

private:

   // Load mask.
   void loadMask(vector<bool>& mask,
                 bool          currentObject,
                 bool          leftObject,
                 bool          leftFrontObject,
                 bool          frontObject,
                 bool          rightFrontObject,
                 bool          rightObject,
                 bool          rightRearObject,
                 bool          rearObject,
                 bool          leftRearObject,
                 bool          orientation,
                 bool          goal,
                 bool          hasObject);

   // Load sensors.
   void loadSensors(vector<Mona::SENSOR>& sensors,
                    Mona::SENSOR          currentObject,
                    Mona::SENSOR          leftObject,
                    Mona::SENSOR          leftFrontObject,
                    Mona::SENSOR          frontObject,
                    Mona::SENSOR          rightFrontObject,
                    Mona::SENSOR          rightObject,
                    Mona::SENSOR          rightRearObject,
                    Mona::SENSOR          rearObject,
                    Mona::SENSOR          leftRearObject,
                    Mona::SENSOR          orientation,
                    Mona::SENSOR          goal,
                    Mona::SENSOR          hasObject);
};
#endif
