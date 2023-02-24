// For conditions of distribution and use, see copyright notice in LICENSE.txt

// Nesting bird.

#ifndef __BIRD__
#define __BIRD__

#include "object.hpp"
#include "locale.hpp"
#include "../mona/mona.hpp"

class Bird
{
public:

   // Gender.
   static const int MALE   = 0;
   static const int FEMALE = 1;

   // Orientation.
   class ORIENTATION
   {
public:
      static const int NORTH = 0;
      static const int SOUTH = 1;
      static const int EAST  = 2;
      static const int WEST  = 3;

      // Orientation to string.
      static char *toString(int orientation)
      {
         switch (orientation)
         {
         case NORTH:
            return("NORTH");

         case SOUTH:
            return("SOUTH");

         case EAST:
            return("EAST");

         case WEST:
            return("WEST");

         default:
            return("Unknown orientation");
         }
      }
   };

   // Sensors.
   class CELL_SENSOR
   {
public:
      static const int NUM_SENSORS = 2;
      int              locale;
      int              object;
   };

   // Bird senses current, left, front, right, and rear cells.
   static const int NUM_CELL_SENSORS      = 9;
   static const int CURRENT_LOCALE_SENSOR = 0;
   static const int CURRENT_OBJECT_SENSOR = 1;
   static const int LEFT_LOCALE_SENSOR    = 2;
   static const int LEFT_OBJECT_SENSOR    = 3;
   static const int LEFT_FRONT_LOCALE_SENSOR = 4;
   static const int LEFT_FRONT_OBJECT_SENSOR = 5;
   static const int FRONT_LOCALE_SENSOR = 6;
   static const int FRONT_OBJECT_SENSOR = 7;
   static const int RIGHT_FRONT_LOCALE_SENSOR = 8;
   static const int RIGHT_FRONT_OBJECT_SENSOR = 9;
   static const int RIGHT_LOCALE_SENSOR   = 10;
   static const int RIGHT_OBJECT_SENSOR   = 11;
   static const int RIGHT_REAR_LOCALE_SENSOR = 12;
   static const int RIGHT_REAR_OBJECT_SENSOR = 13;
   static const int REAR_LOCALE_SENSOR = 14;
   static const int REAR_OBJECT_SENSOR = 15;
   static const int LEFT_REAR_LOCALE_SENSOR = 16;
   static const int LEFT_REAR_OBJECT_SENSOR = 17;

   // State.
   int gender;
   int x, y;
   int orientation;
   int food;
   int hasObject;
   int state;

   // State sensors.
   static const int ORIENTATION_SENSOR = NUM_CELL_SENSORS * CELL_SENSOR::NUM_SENSORS;
   static const int HUNGER_SENSOR = ORIENTATION_SENSOR + 1;
   static const int HAS_OBJECT_SENSOR = HUNGER_SENSOR + 1;
   static const int STATE_SENSOR = HAS_OBJECT_SENSOR + 1;

   static const int NUM_SENSORS = STATE_SENSOR + 1;

   // Responses.
   class RESPONSE
   {
public:
    static const int DO_NOTHING = 0;
        static const int MOVE_FORWARD = 1;
        static const int TURN_RIGHT = 2;
        static const int TURN_LEFT = 3;
        static const int TURN_AROUND = 4;
      static const int EAT_MOUSE           = 5;
      static const int GET_OBJECT           = 6;
      static const int PUT_OBJECT           = 7;
      static const int TOSS_OBJECT          = 8;
      static const int STATE_ON      = 9;
      static const int STATE_OFF     = 10;
      static const int NUM_RESPONSES = 11;

      // Response to string.
      static char *toString(int response)
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

         case STATE_OFF:
            return("STATE_OFF");

         case STATE_ON:
            return("STATE_ON");

         default:
            return("Unknown response");
         }
      }
   };

   // Brain.
   Mona *brain;

   // Constructor.
   Bird(int gender);

   // Digest food.
   void digest();

   // Print state.
   void printState();
};
#endif
