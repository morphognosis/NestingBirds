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

   // Orientation.
   class ORIENTATION
   {
   public:
      static const int NORTH = 0;
      static const int SOUTH = 1;
      static const int EAST  = 2;
      static const int WEST  = 3;

      // Orientation to string.
      static string toString(int orientation)
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

   // Gender.
   static const int MALE   = 0;
   static const int FEMALE = 1;

   // Sensors.
   class CELL_SENSOR
   {
   public:
      static const int NUM_SENSORS = 2;
      int              locale;
      int              object;
   };

   // Bird senses current, left, forward, and right cells.
   static const int NUM_CELL_SENSORS      = 4;
   static const int CURRENT_LOCALE_SENSOR = 0;
   static const int CURRENT_OBJECT_SENSOR = 1;
   static const int LEFT_LOCALE_SENSOR    = 2;
   static const int LEFT_OBJECT_SENSOR    = 3;
   static const int FORWARD_LOCALE_SENSOR = 4;
   static const int FORWARD_OBJECT_SENSOR = 5;
   static const int RIGHT_LOCALE_SENSOR   = 6;
   static const int RIGHT_OBJECT_SENSOR   = 7;

   static const int MATE_PROXIMITY_SENSOR  = NUM_CELL_SENSORS * CELL_SENSOR::NUM_SENSORS;
   static const int MATE_PROXIMITY_UNKNOWN = -1;
   static const int MATE_PROXIMITY_PRESENT = 0;
   static const int MATE_PROXIMITY_LEFT    = 1;
   static const int MATE_PROXIMITY_FORWARD = 2;
   static const int MATE_PROXIMITY_RIGHT   = 3;

   static const int NUM_SENSORS = MATE_PROXIMITY_SENSOR + 1;

   // Responses.
   class RESPONSE
   {
   public:
      static const int DO_NOTHING    = 0;
      static const int EAT           = 1;
      static const int GET           = 2;
      static const int PUT           = 3;
      static const int TOSS          = 4;
      static const int MOVE          = 5;
      static const int TURN_RIGHT    = 6;
      static const int TURN_LEFT     = 7;
      static const int STATE_ZERO = 8;
      static const int STATE_ONE = 9;    
      static const int NUM_RESPONSES = 10;

      // Response to string.
      static string toString(int response)
      {
          switch (response)
          {
          case DO_NOTHING:
              return("DO_NOTHING");

          case EAT:
              return("EAT");

          case GET:
              return("GET");

          case PUT:
              return("PUT");

          case TOSS:
              return("TOSS");

          case MOVE:
              return("MOVE");

          case TURN_RIGHT:
              return("TURN_RIGHT");

          case TURN_LEFT:
              return("TURN_LEFT");

          case STATE_ZERO:
              return("STATE_ZERO");

          case STATE_ONE:
              return("STATE_ONE");

          default:
              return("Unknown response");
          }
      }
   };

   // State.
   int gender;
   int x, y;
   int orientation;
   int food;
   int hasObject;
   int state;

   // Brain.
   Mona *brain;

   // Constructor.
   Bird(int gender);

   // Digest food.
   void digest();

   // Print state.
   void printState();

   // State to string.
   string stateToString();
};
#endif
