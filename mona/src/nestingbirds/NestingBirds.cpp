// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds.

#include "male.hpp"
#include "female.hpp"
#include <stdlib.h>

// Version.
const char *VERSION = "2.0";

// Random numbers.
#define DEFAULT_RANDOM_NUMBER_SEED    "4517"
int RANDOM_NUMBER_SEED = 4517;

// Usage.
const char *Usage =
   "Usage:\n"
   "    nestingbirds\n"
   "      -trainMale <male save file name> | -testMale <male load file name>\n"
   "      -trainFemale <female save file name> | -testFemale <female load file name>\n"    
   "      -steps <steps>\n"
   "      [-maleInitialFood <amount> (default=" MALE_DEFAULT_INITIAL_FOOD "]\n"
   "      [-maleFoodDuration <amount> (default=" MALE_DEFAULT_FOOD_DURATION ")]\n"
   "      [-maleRandomizeFoodLevel (food level probabilistically increases 0-" MALE_DEFAULT_FOOD_DURATION " upon eating food)]\n"
   "      [-maleMouseNeed <amount> (default=" MALE_DEFAULT_MOUSE_NEED ")]\n"
   "      [-maleFemaleMouseNeed <amount> (default=" MALE_DEFAULT_FEMALE_MOUSE_NEED ")]\n"
   "      [-maleFemaleStoneNeed <amount> (default=" MALE_DEFAULT_FEMALE_STONE_NEED ")]\n"
   "      [-maleAttendFemaleNeed <amount> (default=" MALE_DEFAULT_ATTEND_FEMALE_NEED ")]\n"
   "      [-femaleInitialFood <amount> (default=" FEMALE_DEFAULT_INITIAL_FOOD ")]\n"
   "      [-femaleFoodDuration <amount> (default" FEMALE_DEFAULT_FOOD_DURATION ")]\n"
   "      [-femaleRandomizeFoodLevel (food level probabilistically increases 0-" FEMALE_DEFAULT_FOOD_DURATION " upon eating food)]\n"
   "      [-femaleMouseNeed <amount> (default=" FEMALE_DEFAULT_MOUSE_NEED ")]\n"
   "      [-femaleStoneNeed <amount> (default=" FEMALE_DEFAULT_STONE_NEED ")]\n"
   "      [-femaleLayEggNeed <amount> (default=" FEMALE_DEFAULT_LAY_EGG_NEED ")]\n"
   "      [-femaleBroodEggNeed <amount> (default=" FEMALE_DEFAULT_BROOD_EGG_NEED ")]\n"
   "      [-verbose <true | false> (default=true)]\n"
   "      [-randomSeed <seed> (default=" DEFAULT_RANDOM_NUMBER_SEED ")]\n"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
   "      [-version]\n"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
   "Exit codes:\n"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
   "  0=success\n"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
   "  1=error\n";

// Bird sex.
static const int MALE = 0;
static const int FEMALE = 1;

// Testing vs. training.
bool MaleTest;
bool FemaleTest;

// Steps.
int Steps;

// Training controls.
bool FemaleWantMouse;
bool FemaleWantStone;
int  FemaleNestSequence;

// Save/load file names.
char *MaleFilename;
char* FemaleFilename;

// World map: plain=0, forest=1, mouse=2, desert=3, stone=4
const int  WIDTH = 21, HEIGHT = 21;
const char WorldMap[WIDTH][HEIGHT] =
{
   { 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
   { 0, 0, 0, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
   { 0, 2, 1, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
   { 0, 0, 0, 1, 1, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
   { 0, 0, 0, 2, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
   { 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
   { 0, 0, 0, 0, 2, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
   { 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 3, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 3, 3, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 0, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 3, 4, 4, 0, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 3, 0, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 3, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 4, 4, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 3, 3, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0 },
   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 3, 0, 0, 0 }
};

// Bounds.
int NEST_CENTER_X = WIDTH / 2;
int NEST_CENTER_Y = (HEIGHT / 2) + 1;
int NEST_MIN_X = NEST_CENTER_X - 1;
int NEST_MAX_X = NEST_CENTER_X + 1;
int NEST_MIN_Y = NEST_CENTER_Y - 1;
int NEST_MAX_Y = NEST_CENTER_Y + 1;
int FOREST_CENTER_X = -1;
int FOREST_CENTER_Y = -1;
int FOREST_MIN_X = -1;
int FOREST_MAX_X = -1;
int FOREST_MIN_Y = -1;
int FOREST_MAX_Y = -1;
int DESERT_CENTER_X = -1;
int DESERT_CENTER_Y = -1;
int DESERT_MIN_X = -1;
int DESERT_MAX_X = -1;
int DESERT_MIN_Y = -1;
int DESERT_MAX_Y = -1;

// Cell.
class Cell
{
public:
   int locale;
   int object;
   Cell()
   {
      locale = LOCALE::DESERT;
      object = OBJECT::NO_OBJECT;
   }
};

// World.
Cell World[WIDTH][HEIGHT];

// Birds.
Male   *male;
Female *female;

// Probability of mouse movement.
static double MOUSE_MOVE_PROBABILITY = 0.0;

// Verbosity.
bool Verbose = true;

// Prototypes.
void setSensors(int);
void doResponse(int);
void stepMice();
void train(int);
void trainMale();
void trainFemale();
bool getMouse();
bool getStone();
bool returnToFemale();
void doMaleResponse();
void doFemaleResponse();
void setMaleSensors();
void    setFemaleSensors();
int distance(int x1, int y1, int x2, int y2);

// Initialize.
void init()
{
   // Random numbers.
   srand(RANDOM_NUMBER_SEED);

   // Initialize world.
   for (int x = 0; x < WIDTH; x++)
   {
      for (int y = 0; y < HEIGHT; y++)
      {
         switch (WorldMap[x][y])
         {
         case 0:
            World[x][y].locale = LOCALE::PLAIN;
            World[x][y].object = OBJECT::NO_OBJECT;
            break;

         case 1:
            World[x][y].locale = LOCALE::FOREST;
            World[x][y].object = OBJECT::NO_OBJECT;
            if (FOREST_MIN_X == -1 || x < FOREST_MIN_X)
            {
                FOREST_MIN_X = x;
            }
            if (FOREST_MAX_X == -1 || x > FOREST_MAX_X)
            {
                FOREST_MAX_X = x;
            }
            if (FOREST_MIN_Y == -1 || y < FOREST_MIN_Y)
            {
                FOREST_MIN_Y = y;
            }
            if (FOREST_MAX_Y == -1 || y > FOREST_MAX_Y)
            {
                FOREST_MAX_Y = y;
            }
            break;

         case 2:
            World[x][y].locale = LOCALE::FOREST;
            World[x][y].object = OBJECT::MOUSE;
            if (FOREST_MIN_X == -1 || x < FOREST_MIN_X)
            {
                FOREST_MIN_X = x;
            }
            if (FOREST_MAX_X == -1 || x > FOREST_MAX_X)
            {
                FOREST_MAX_X = x;
            }
            if (FOREST_MIN_Y == -1 || y < FOREST_MIN_Y)
            {
                FOREST_MIN_Y = y;
            }
            if (FOREST_MAX_Y == -1 || y > FOREST_MAX_Y)
            {
                FOREST_MAX_Y = y;
            }
            break;

         case 3:
            World[x][y].locale = LOCALE::DESERT;
            World[x][y].object = OBJECT::NO_OBJECT;
            if (DESERT_MIN_X == -1 || x < DESERT_MIN_X)
            {
                DESERT_MIN_X = x;
            }
            if (DESERT_MAX_X == -1 || x > DESERT_MAX_X)
            {
                DESERT_MAX_X = x;
            }
            if (DESERT_MIN_Y == -1 || y < DESERT_MIN_Y)
            {
                DESERT_MIN_Y = y;
            }
            if (DESERT_MAX_Y == -1 || y > DESERT_MAX_Y)
            {
                DESERT_MAX_Y = y;
            }
            break;

         case 4:
            World[x][y].locale = LOCALE::DESERT;
            World[x][y].object = OBJECT::STONE;
            if (DESERT_MIN_X == -1 || x < DESERT_MIN_X)
            {
                DESERT_MIN_X = x;
            }
            if (DESERT_MAX_X == -1 || x > DESERT_MAX_X)
            {
                DESERT_MAX_X = x;
            }
            if (DESERT_MIN_Y == -1 || y < DESERT_MIN_Y)
            {
                DESERT_MIN_Y = y;
            }
            if (DESERT_MAX_Y == -1 || y > DESERT_MAX_Y)
            {
                DESERT_MAX_Y = y;
            }
            break;
         }
      }
   }
   FOREST_CENTER_X = (FOREST_MIN_X + FOREST_MAX_X) / 2;
   FOREST_CENTER_Y = (FOREST_MIN_Y + FOREST_MAX_Y) / 2;
   DESERT_CENTER_X = (DESERT_MIN_X + DESERT_MAX_X) / 2;
   DESERT_CENTER_Y = (DESERT_MIN_Y + DESERT_MAX_Y) / 2;

   // Create birds.
   female             = new Female();
   female->x          = WIDTH / 2;
   female->y          = HEIGHT / 2;
   female->food       = Female::INITIAL_FOOD;
   female->response   = Female::RESPONSE::DO_NOTHING;
   male               = new Male();
   male->x            = WIDTH / 2;
   male->y            = HEIGHT / 2;
   male->food         = Male::INITIAL_FOOD;
   male->response     = Male::RESPONSE::DO_NOTHING;
   FemaleWantMouse    = false;
   FemaleWantStone    = false;
   FemaleNestSequence = 0;
   setSensors(FEMALE);
   setSensors(MALE);
}


// Step.
void step()
{
   // Step mice.
   stepMice();

   // Set female sensors.
   setSensors(FEMALE);

   // Train?
   if (!FemaleTest)
   {
      train(FEMALE);
   }

   // Set female needs.
   female->setNeeds();

   // Cycle female.
   female->cycle();

   if (Verbose)
   {
      printf("Female: Location: [%d,%d], ", female->x, female->y);
      female->print();
      printf("\n");
   }

   // Do response in world.
   doResponse(FEMALE);

   // Set male sensors.
   setSensors(MALE);

   // Train?
   if (!MaleTest)
   {
      train(MALE);
   }

   // Set male needs.
   male->setNeeds();

   // Cycle male.
   male->cycle();

   if (Verbose)
   {
      printf("Male: Location: [%d,%d], ", male->x, male->y);
      male->print();
      printf("\n");
   }

   // Do response in world.
   doResponse(MALE);

   // Post-response.
   male->postResponse();
}


// Train.
void train(int gender)
{
   if (gender == MALE)
   {
      trainMale();
      male->setResponseOverride();
   }
   else
   {
      trainFemale();
      female->setResponseOverride();
   }
}


// Train male.
void trainMale()
{
    if (World[male->x][male->y].object == OBJECT::EGG)
    {
        male->response = Male::RESPONSE::DO_NOTHING;
        return;
    }

      if (!getMouse() && !getStone())
      {
         returnToFemale();
      }
}


// Get mouse.
bool getMouse()
{
    if (male->food == 0)
    {
        if (male->hasObject == OBJECT::MOUSE)
        {
            male->response = Male::RESPONSE::EAT_MOUSE;
            return(true);
        }
        if (male->hasObject == OBJECT::STONE)
        {
            male->response = Male::RESPONSE::TOSS_OBJECT;
            return(true);
        }
    }
   if (male->sensors[Male::FEMALE_PROXIMITY_SENSOR] == Male::PROXIMITY::PRESENT)
   {
       if (male->hasObject == OBJECT::MOUSE)
       {
           male->response = Male::RESPONSE::GIVE_MOUSE;
           FemaleWantMouse = false;
           return(true);
       }
       else if (female->response == Female::RESPONSE::WANT_MOUSE)
      {
         FemaleWantMouse = true;
      }
      if (male->hasObject == OBJECT::STONE)
      {
              male->response = Male::RESPONSE::GIVE_STONE;
              FemaleWantStone = false;
              return(true);
      }
      else if (female->response == Female::RESPONSE::WANT_STONE)
      {
          FemaleWantStone = true;
      }
   }
   if (male->hasObject == OBJECT::MOUSE)
   {
       return false;
   }
   if (male->food > 0 && !FemaleWantMouse)
   {
       return false;
   }
   if (male->sensors[Male::MOUSE_PROXIMITY_SENSOR] == Male::PROXIMITY::PRESENT)
   {
      male->response = Male::RESPONSE::GET_OBJECT;
      return(true);
   }
   if (male->sensors[Male::MOUSE_PROXIMITY_SENSOR] == Male::PROXIMITY::FRONT)
   {
       male->response = Male::RESPONSE::MOVE_FORWARD;
       return(true);
   }
   if (male->sensors[Male::MOUSE_PROXIMITY_SENSOR] == Male::PROXIMITY::REAR)
   {
       male->response = Male::RESPONSE::TURN_AROUND;
       return(true);
   }
   if (male->sensors[Male::MOUSE_PROXIMITY_SENSOR] == Male::PROXIMITY::LEFT)
   {
      male->response = Male::RESPONSE::TURN_LEFT;
      return(true);
   }
   if (male->sensors[Male::MOUSE_PROXIMITY_SENSOR] == Male::PROXIMITY::RIGHT)
   {
       male->response = Male::RESPONSE::TURN_RIGHT;
       return(true);
   }
   int mousex = FOREST_CENTER_X;
   int mousey = FOREST_CENTER_Y;
   male->response = Mona::Motor::gotoPlace(male->orientation, male->x, male->y, mousex, mousey);
   return(true);
}


// Get stone.
bool getStone()
{
   if (male->sensors[Male::FEMALE_PROXIMITY_SENSOR] == Male::PROXIMITY::PRESENT)
   {
      if (female->response == Female::RESPONSE::WANT_STONE)
      {
         FemaleWantStone = true;
      }
   }
   if (FemaleWantMouse || !FemaleWantStone)
   {
      return(false);
   }
   if (male->hasObject == OBJECT::STONE)
   {
      if (male->sensors[Male::FEMALE_PROXIMITY_SENSOR] == Male::PROXIMITY::PRESENT)
      {
         male->response  = Male::RESPONSE::GIVE_STONE;
         FemaleWantStone = false;
         return(true);
      }
      else
      {
         return(false);
      }
   }
   if (male->sensors[Male::STONE_PROXIMITY_SENSOR] == Male::PROXIMITY::PRESENT)
   {
      male->response = Male::RESPONSE::GET_OBJECT;
      return(true);
   }
   if (male->sensors[Male::STONE_PROXIMITY_SENSOR] == Male::PROXIMITY::FRONT)
   {
       male->response = Male::RESPONSE::MOVE_FORWARD;
       return(true);
   }
   if (male->sensors[Male::STONE_PROXIMITY_SENSOR] == Male::PROXIMITY::REAR)
   {
       male->response = Male::RESPONSE::TURN_AROUND;
       return(true);
   }
   if (male->sensors[Male::STONE_PROXIMITY_SENSOR] == Male::PROXIMITY::LEFT)
   {
       male->response = Male::RESPONSE::TURN_LEFT;
       return(true);
   }
   if (male->sensors[Male::STONE_PROXIMITY_SENSOR] == Male::PROXIMITY::RIGHT)
   {
       male->response = Male::RESPONSE::TURN_RIGHT;
       return(true);
   }
   int stonex = DESERT_CENTER_X;
   int stoney = DESERT_CENTER_Y;
   male->response = Mona::Motor::gotoPlace(male->orientation, male->x, male->y, stonex, stoney);
   return(true);
}


// Male returns to female.
bool returnToFemale()
{
   switch (male->sensors[Male::FEMALE_PROXIMITY_SENSOR])
   {
   case Male::PROXIMITY::PRESENT:
      male->response = Male::RESPONSE::DO_NOTHING;
      return(true);

   case Male::PROXIMITY::LEFT:
      male->response = Male::RESPONSE::TURN_LEFT;
      break;

   case Male::PROXIMITY::FRONT:
       male->response = Male::RESPONSE::MOVE_FORWARD;
      break;

   case Male::PROXIMITY::RIGHT:
      male->response = Male::RESPONSE::TURN_RIGHT;
      break;

   case Male::PROXIMITY::REAR:
       male->response = Male::RESPONSE::TURN_AROUND;
       break;

   case Male::PROXIMITY::UNKNOWN:
      int centerx = WIDTH / 2;
      int centery = (HEIGHT / 2) + 1;
      if (male->x < centerx)
      {
         if (male->orientation == ORIENTATION::EAST)
         {
            male->response = Male::RESPONSE::MOVE_FORWARD;
         }
         else
         {
            if (male->orientation == ORIENTATION::SOUTH)
            {
               male->response = Male::RESPONSE::TURN_LEFT;
            }
            else if (male->orientation == ORIENTATION::NORTH)
            {
               male->response = Male::RESPONSE::TURN_RIGHT;
            }
            else 
            {
                male->response = Male::RESPONSE::TURN_AROUND;
            }
         }
      }
      else if (male->x > centerx)
      {
         if (male->orientation == ORIENTATION::WEST)
         {
            male->response = Male::RESPONSE::MOVE_FORWARD;
         }
         else
         {
            if (male->orientation == ORIENTATION::SOUTH)
            {
               male->response = Male::RESPONSE::TURN_RIGHT;
            }
            else if (male->orientation == ORIENTATION::NORTH)
            {
               male->response = Male::RESPONSE::TURN_LEFT;
            }
            else 
            {
                male->response = Male::RESPONSE::TURN_AROUND;
            }
         }
      }
      else if (male->y < centery)
      {
         if (male->orientation == ORIENTATION::SOUTH)
         {
            male->response = Male::RESPONSE::MOVE_FORWARD;
         }
         else
         {
            if (male->orientation == ORIENTATION::EAST)
            {
               male->response = Male::RESPONSE::TURN_RIGHT;
            }
            else if (male->orientation == ORIENTATION::WEST)
            {
               male->response = Male::RESPONSE::TURN_LEFT;
            }
            else 
            {
                male->response = Male::RESPONSE::TURN_AROUND;
            }
         }
      }
      else if (male->y > centery)
      {
         if (male->orientation == ORIENTATION::NORTH)
         {
            male->response = Male::RESPONSE::MOVE_FORWARD;
         }
         else
         {
            if (male->orientation == ORIENTATION::WEST)
            {
               male->response = Male::RESPONSE::TURN_RIGHT;
            }
            else if (male->orientation == ORIENTATION::EAST)
            {
               male->response = Male::RESPONSE::TURN_LEFT;
            }
            else 
            {
                male->response = Male::RESPONSE::TURN_AROUND;
            }
         }
      }
      else
      {
         male->response = Male::RESPONSE::MOVE_FORWARD;
      }
      break;
   }
   return(false);
}


// Train female.
void trainFemale()
{
    // Brooding egg?
    if (World[female->x][female->y].object == OBJECT::EGG)
    {
        female->response = Female::RESPONSE::DO_NOTHING;
        return;
    }

    // Handle delivery.
      if (female->hasObject == OBJECT::MOUSE)
      {
         female->response = Female::RESPONSE::EAT_MOUSE;
         return;
      }
      if (female->hasObject == OBJECT::STONE)
      { 
          female->response = Female::RESPONSE::PUT_OBJECT;
          return;
      }

   // Hunger?
   if (female->food == 0)
   {
           female->response = Female::RESPONSE::WANT_MOUSE;
       return;
   }

   // Nest building.
   switch (FemaleNestSequence)
   {
   case 0:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         if (female->orientation == ORIENTATION::WEST)
         {
            female->response = Female::RESPONSE::MOVE_FORWARD;
            FemaleNestSequence++;
         }
         else
         {
            female->response = Female::RESPONSE::TURN_LEFT;
         }
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 1:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         if (female->orientation == ORIENTATION::SOUTH)
         {
            female->response = Female::RESPONSE::MOVE_FORWARD;
            FemaleNestSequence++;
         }
         else
         {
            female->response = Female::RESPONSE::TURN_LEFT;
         }
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 2:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         female->response = Female::RESPONSE::MOVE_FORWARD;
         FemaleNestSequence++;
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 3:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         if (female->orientation == ORIENTATION::EAST)
         {
            female->response = Female::RESPONSE::MOVE_FORWARD;
            FemaleNestSequence++;
         }
         else
         {
            female->response = Female::RESPONSE::TURN_LEFT;
         }
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 4:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         female->response = Female::RESPONSE::MOVE_FORWARD;
         FemaleNestSequence++;
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 5:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         if (female->orientation == ORIENTATION::NORTH)
         {
            female->response = Female::RESPONSE::MOVE_FORWARD;
            FemaleNestSequence++;
         }
         else
         {
            female->response = Female::RESPONSE::TURN_LEFT;
         }
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 6:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         female->response = Female::RESPONSE::MOVE_FORWARD;
         FemaleNestSequence++;
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 7:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         if (female->orientation == ORIENTATION::WEST)
         {
            female->response = Female::RESPONSE::MOVE_FORWARD;
            FemaleNestSequence++;
         }
         else
         {
            female->response = Female::RESPONSE::TURN_LEFT;
         }
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 8:
         if (female->orientation == ORIENTATION::SOUTH)
         {
            female->response = Female::RESPONSE::MOVE_FORWARD;
            FemaleNestSequence++;
         }
         else
         {
            female->response = Female::RESPONSE::TURN_LEFT;
         }
      break;

   case 9:
         female->response = Female::RESPONSE::LAY_EGG;
         FemaleNestSequence++;
      break;

   case 10:
      break;
   }
}


// Set bird sensors.
void setSensors(int gender)
{
    if (gender == MALE)
    {
        setMaleSensors();
    }
    else {
        setFemaleSensors();
    }
}

// Set male sensors.
void setMaleSensors()
{
    Male* bird = male;
    int* sensors = male->sensors;

    for (int i = 0; i < Male::NUM_SENSORS; i++)
    {
        sensors[i] = DONT_CARE;
    }

    // Set locale and object sensors.
    sensors[Male::LOCALE_SENSOR] = World[bird->x][bird->y].locale;
    sensors[Male::OBJECT_SENSOR] = World[bird->x][bird->y].object;

    // Set mouse, stone, and female proximity sensors.
    int mousex = -1;
    int mousey = -1;
    int mouseDist = -1;
    int stonex = -1;
    int stoney = -1;
    int stoneDist = -1;
    int femaleDist = -1;
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            if (bird->x >= FOREST_MIN_X && bird->x <= FOREST_MAX_X &&
                bird->y >= FOREST_MIN_Y && bird->y <= FOREST_MAX_Y)
            {
                if (World[x][y].object == OBJECT::MOUSE)
                {
                    int d = distance(x, y, bird->x, bird->y);
                    if (mouseDist < 0 || d < mouseDist)
                    {
                        mouseDist = d;
                        mousex = x;
                        mousey = y;
                    }
                }
            }
            if (bird->x >= DESERT_MIN_X && bird->x <= DESERT_MAX_X &&
                bird->y >= DESERT_MIN_Y && bird->y <= DESERT_MAX_Y)
            {
                if (World[x][y].locale == LOCALE::DESERT &&
                    World[x][y].object == OBJECT::STONE)
                {
                    int d = distance(x, y, bird->x, bird->y);
                    if (stoneDist < 0 || d < stoneDist)
                    {
                        stoneDist = d;
                        stonex = x;
                        stoney = y;
                    }
                }
            }
            if (bird->x >= NEST_MIN_X && bird->x <= NEST_MAX_X &&
                bird->y >= NEST_MIN_Y && bird->y <= NEST_MAX_Y)
            {
                femaleDist = distance(female->x, female->y, bird->x, bird->y);
            }
        }
    }
    switch (bird->orientation)
    {
    case ORIENTATION::NORTH:
        if (mouseDist >= 0)
        {
            if (mousex < bird->x)
            {
                sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
            }
            else if (mousex > bird->x)
            {
                sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else {
                if (mousey < bird->y)
                {
                    sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
                }
                else if (mousey > bird->y)
                {
                    sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::REAR;
                }
                else {
                    sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
                }
            }
        }
        else {
            sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
        }
        if (stoneDist >= 0)
        {
            if (stonex < bird->x)
            {
                sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
            }
            else if (stonex > bird->x)
            {
                sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else {
                if (stoney < bird->y)
                {
                    sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
                }
                else if (stoney > bird->y)
                {
                    sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::REAR;
                }
                else {
                    sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
                }
            }
        }
        else {
            sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
        }
        if (femaleDist >= 0)
        {
            if (female->x < bird->x)
            {
                sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
            }
            else if (female->x > bird->x)
            {
                sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else {
                if (female->y < bird->y)
                {
                    sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
                }
                else if (female->y > bird->y)
                {
                    sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::REAR;
                }
                else {
                    sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
                }
            }
        }
        else {
            sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
        }
        break;

    case ORIENTATION::SOUTH:
        if (mouseDist >= 0)
        {
            if (mousex < bird->x)
            {
                sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else if (mousex > bird->x)
            {
                sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
            }
            else {
                if (mousey < bird->y)
                {
                    sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::REAR;
                }
                else if (mousey > bird->y)
                {
                    sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
                }
                else {
                    sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
                }
            }
        }
        else {
            sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
        }
        if (stoneDist >= 0)
        {
            if (stonex < bird->x)
            {
                sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else if (stonex > bird->x)
            {
                sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
            }
            else {
                if (stoney < bird->y)
                {
                    sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::REAR;
                }
                else if (stoney > bird->y)
                {
                    sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
                }
                else {
                    sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
                }
            }
        }
        else {
            sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
        }
        if (femaleDist >= 0)
        {
            if (female->x < bird->x)
            {
                sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else if (female->x > bird->x)
            {
                sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
            }
            else {
                if (female->y < bird->y)
                {
                    sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::REAR;
                }
                else if (female->y > bird->y)
                {
                    sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
                }
                else {
                    sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
                }
            }
        }
        else {
            sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
        }
        break;

    case ORIENTATION::EAST:
        if (mouseDist >= 0)
        {
            if (mousex < bird->x)
            {
                sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::REAR;
            }
            else if (mousex > bird->x)
            {
                sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
            }
            else {
                if (mousey < bird->y)
                {
                    sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
                }
                else if (mousey > bird->y)
                {
                    sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
                }
                else {
                    sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
                }
            }
        }
        else {
            sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
        }
        if (stoneDist >= 0)
        {
            if (stonex < bird->x)
            {
                sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::REAR;
            }
            else if (stonex > bird->x)
            {
                sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
            }
            else {
                if (stoney < bird->y)
                {
                    sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
                }
                else if (stoney > bird->y)
                {
                    sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
                }
                else {
                    sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
                }
            }
        }
        else {
            sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
        }
        if (femaleDist >= 0)
        {
            if (female->x < bird->x)
            {
                sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::REAR;
            }
            else if (female->x > bird->x)
            {
                sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
            }
            else {
                if (female->y < bird->y)
                {
                    sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
                }
                else if (female->y > bird->y)
                {
                    sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
                }
                else {
                    sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
                }
            }
        }
        else {
            sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
        }
        break;

    case ORIENTATION::WEST:
        if (mouseDist >= 0)
        {
            if (mousex < bird->x)
            {
                sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
            }
            else if (mousex > bird->x)
            {
                sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::REAR;
            }
            else {
                if (mousey < bird->y)
                {
                    sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
                }
                else if (mousey > bird->y)
                {
                    sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
                }
                else {
                    sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
                }
            }
        }
        else {
            sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
        }
        if (stoneDist >= 0)
        {
            if (stonex < bird->x)
            {
                sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
            }
            else if (stonex > bird->x)
            {
                sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::REAR;
            }
            else {
                if (stoney < bird->y)
                {
                    sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
                }
                else if (stoney > bird->y)
                {
                    sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
                }
                else {
                    sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
                }
            }
        }
        else {
            sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
        }
        if (femaleDist >= 0)
        {
            if (female->x < bird->x)
            {
                sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
            }
            else if (female->x > bird->x)
            {
                sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::REAR;
            }
            else {
                if (female->y < bird->y)
                {
                    sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
                }
                else if (female->y > bird->y)
                {
                    sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
                }
                else {
                    sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
                }
            }
        }
        else {
            sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
        }
        break;
    }

    // Internal state.
    sensors[Male::ORIENTATION_SENSOR] = bird->orientation;
    if (bird->food > 0)
    {
        sensors[Male::HUNGER_SENSOR] = 0;
    }
    else {
        sensors[Male::HUNGER_SENSOR] = 1;
    }
    sensors[Male::HAS_OBJECT_SENSOR] = bird->hasObject;

    // Sense female wants.
    sensors[Male::WANT_MOUSE_SENSOR] = 0;
    sensors[Male::WANT_STONE_SENSOR] = 0;
    if ((bird->x == female->x) && (bird->y == female->y))
    {
        switch (female->response)
        {
        case Female::RESPONSE::WANT_MOUSE:
            sensors[Male::WANT_MOUSE_SENSOR] = 1;
            break;

        case Female::RESPONSE::WANT_STONE:
            sensors[Male::WANT_STONE_SENSOR] = 1;
            break;
        }
    }
}

// Set female sensors.
void setFemaleSensors()
{
    Female* bird = female;
    int* sensors = female->sensors;

    for (int i = 0; i < Female::NUM_SENSORS; i++)
    {
        sensors[i] = DONT_CARE;
    }

    // Set locale and object sensors.
    int x = -1;
    int y = -1;
    for (int i = 0; i < Female::NUM_CELL_SENSORS; i++)
    {
        switch (i)
        {
            // Current.
        case 0:
            sensors[Female::CURRENT_LOCALE_SENSOR] = World[bird->x][bird->y].locale;
            sensors[Female::CURRENT_OBJECT_SENSOR] = World[bird->x][bird->y].object;
            break;

            // Left.
        case 1:
            x = -1;
            y = -1;
            switch (bird->orientation)
            {
            case ORIENTATION::NORTH:
                if (bird->x > 0)
                {
                    sensors[Female::LEFT_LOCALE_SENSOR] = World[bird->x - 1][bird->y].locale;
                    sensors[Female::LEFT_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
                    x = bird->x - 1;
                    y = bird->y;
                }
                break;

            case ORIENTATION::SOUTH:
                if (bird->x < WIDTH - 1)
                {
                    sensors[Female::LEFT_LOCALE_SENSOR] = World[bird->x + 1][bird->y].locale;
                    sensors[Female::LEFT_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
                    x = bird->x + 1;
                    y = bird->y;
                }
                break;

            case ORIENTATION::EAST:
                if (bird->y > 0)
                {
                    sensors[Female::LEFT_LOCALE_SENSOR] = World[bird->x][bird->y - 1].locale;
                    sensors[Female::LEFT_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
                    x = bird->x;
                    y = bird->y - 1;
                }
                break;

            case ORIENTATION::WEST:
                if (bird->y < HEIGHT - 1)
                {
                    sensors[Female::LEFT_LOCALE_SENSOR] = World[bird->x][bird->y + 1].locale;
                    sensors[Female::LEFT_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
                    x = bird->x;
                    y = bird->y + 1;
                }
                break;
            }
            break;

            // Left front.
        case 2:
            x = -1;
            y = -1;
            switch (bird->orientation)
            {
            case ORIENTATION::NORTH:
                if (bird->x > 0 && bird->y > 0)
                {
                    sensors[Female::LEFT_FRONT_LOCALE_SENSOR] = World[bird->x - 1][bird->y - 1].locale;
                    sensors[Female::LEFT_FRONT_OBJECT_SENSOR] = World[bird->x - 1][bird->y - 1].object;
                    x = bird->x - 1;
                    y = bird->y - 1;
                }
                break;

            case ORIENTATION::SOUTH:
                if (bird->x < WIDTH - 1 && bird->y < HEIGHT - 1)
                {
                    sensors[Female::LEFT_FRONT_LOCALE_SENSOR] = World[bird->x + 1][bird->y + 1].locale;
                    sensors[Female::LEFT_FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y + 1].object;
                    x = bird->x + 1;
                    y = bird->y + 1;
                }
                break;

            case ORIENTATION::EAST:
                if (bird->x < WIDTH - 1 && bird->y > 0)
                {
                    sensors[Female::LEFT_FRONT_LOCALE_SENSOR] = World[bird->x + 1][bird->y - 1].locale;
                    sensors[Female::LEFT_FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y - 1].object;
                    x = bird->x + 1;
                    y = bird->y - 1;
                }
                break;

            case ORIENTATION::WEST:
                if (bird->x > 0 && bird->y < HEIGHT - 1)
                {
                    sensors[Female::LEFT_FRONT_LOCALE_SENSOR] = World[bird->x - 1][bird->y + 1].locale;
                    sensors[Female::LEFT_FRONT_OBJECT_SENSOR] = World[bird->x - 1][bird->y + 1].object;
                    x = bird->x - 1;
                    y = bird->y + 1;
                }
                break;
            }
            break;

            // Front.
        case 3:
            x = -1;
            y = -1;
            switch (bird->orientation)
            {
            case ORIENTATION::NORTH:
                if (bird->y > 0)
                {
                    sensors[Female::FRONT_LOCALE_SENSOR] = World[bird->x][bird->y - 1].locale;
                    sensors[Female::FRONT_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
                    x = bird->x;
                    y = bird->y - 1;
                }
                break;

            case ORIENTATION::SOUTH:
                if (bird->y < HEIGHT - 1)
                {
                    sensors[Female::FRONT_LOCALE_SENSOR] = World[bird->x][bird->y + 1].locale;
                    sensors[Female::FRONT_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
                    x = bird->x;
                    y = bird->y + 1;
                }
                break;

            case ORIENTATION::EAST:
                if (bird->x < WIDTH - 1)
                {
                    sensors[Female::FRONT_LOCALE_SENSOR] = World[bird->x + 1][bird->y].locale;
                    sensors[Female::FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
                    x = bird->x + 1;
                    y = bird->y;
                }
                break;

            case ORIENTATION::WEST:
                if (bird->x > 0)
                {
                    sensors[Female::FRONT_LOCALE_SENSOR] = World[bird->x - 1][bird->y].locale;
                    sensors[Female::FRONT_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
                    x = bird->x - 1;
                    y = bird->y;
                }
                break;
            }
            break;

            // Right front.
        case 4:
            x = -1;
            y = -1;
            switch (bird->orientation)
            {
            case ORIENTATION::NORTH:
                if (bird->x < WIDTH - 1 && bird->y > 0)
                {
                    sensors[Female::RIGHT_FRONT_LOCALE_SENSOR] = World[bird->x + 1][bird->y - 1].locale;
                    sensors[Female::RIGHT_FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y - 1].object;
                    x = bird->x + 1;
                    y = bird->y - 1;
                }
                break;

            case ORIENTATION::SOUTH:
                if (bird->x > 0 && bird->y < HEIGHT - 1)
                {
                    sensors[Female::RIGHT_FRONT_LOCALE_SENSOR] = World[bird->x - 1][bird->y + 1].locale;
                    sensors[Female::RIGHT_FRONT_OBJECT_SENSOR] = World[bird->x - 1][bird->y + 1].object;
                    x = bird->x - 1;
                    y = bird->y + 1;
                }
                break;

            case ORIENTATION::EAST:
                if (bird->x < WIDTH - 1 && bird->y < HEIGHT - 1)
                {
                    sensors[Female::RIGHT_FRONT_LOCALE_SENSOR] = World[bird->x + 1][bird->y + 1].locale;
                    sensors[Female::RIGHT_FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y + 1].object;
                    x = bird->x + 1;
                    y = bird->y + 1;
                }
                break;

            case ORIENTATION::WEST:
                if (bird->x > 0 && bird->y > 0)
                {
                    sensors[Female::RIGHT_FRONT_LOCALE_SENSOR] = World[bird->x - 1][bird->y - 1].locale;
                    sensors[Female::RIGHT_FRONT_OBJECT_SENSOR] = World[bird->x - 1][bird->y - 1].object;
                    x = bird->x - 1;
                    y = bird->y - 1;
                }
                break;
            }
            break;

            // Right.
        case 5:
            x = -1;
            y = -1;
            switch (bird->orientation)
            {
            case ORIENTATION::NORTH:
                if (bird->x < WIDTH - 1)
                {
                    sensors[Female::RIGHT_LOCALE_SENSOR] = World[bird->x + 1][bird->y].locale;
                    sensors[Female::RIGHT_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
                    x = bird->x + 1;
                    y = bird->y;
                }
                break;

            case ORIENTATION::SOUTH:
                if (bird->x > 0)
                {
                    sensors[Female::RIGHT_LOCALE_SENSOR] = World[bird->x - 1][bird->y].locale;
                    sensors[Female::RIGHT_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
                    x = bird->x - 1;
                    y = bird->y;
                }
                break;

            case ORIENTATION::EAST:
                if (bird->y < HEIGHT - 1)
                {
                    sensors[Female::RIGHT_LOCALE_SENSOR] = World[bird->x][bird->y + 1].locale;
                    sensors[Female::RIGHT_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
                    x = bird->x;
                    y = bird->y + 1;
                }
                break;

            case ORIENTATION::WEST:
                if (bird->y > 0)
                {
                    sensors[Female::RIGHT_LOCALE_SENSOR] = World[bird->x][bird->y - 1].locale;
                    sensors[Female::RIGHT_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
                    x = bird->x;
                    y = bird->y - 1;
                }
                break;
            }
            break;

            // Right rear.
        case 6:
            x = -1;
            y = -1;
            switch (bird->orientation)
            {
            case ORIENTATION::NORTH:
                if (bird->x < WIDTH - 1 && bird->y < HEIGHT - 1)
                {
                    sensors[Female::RIGHT_REAR_LOCALE_SENSOR] = World[bird->x + 1][bird->y + 1].locale;
                    sensors[Female::RIGHT_REAR_OBJECT_SENSOR] = World[bird->x + 1][bird->y + 1].object;
                    x = bird->x + 1;
                    y = bird->y + 1;
                }
                break;

            case ORIENTATION::SOUTH:
                if (bird->x > 0 && bird->y > 0)
                {
                    sensors[Female::RIGHT_REAR_LOCALE_SENSOR] = World[bird->x - 1][bird->y - 1].locale;
                    sensors[Female::RIGHT_REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y - 1].object;
                    x = bird->x - 1;
                    y = bird->y - 1;
                }
                break;

            case ORIENTATION::EAST:
                if (bird->x > 0 && bird->y < HEIGHT - 1)
                {
                    sensors[Female::RIGHT_REAR_LOCALE_SENSOR] = World[bird->x - 1][bird->y + 1].locale;
                    sensors[Female::RIGHT_REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y + 1].object;
                    x = bird->x - 1;
                    y = bird->y + 1;
                }
                break;

            case ORIENTATION::WEST:
                if (bird->x < WIDTH - 1 && bird->y > 0)
                {
                    sensors[Female::RIGHT_REAR_LOCALE_SENSOR] = World[bird->x + 1][bird->y - 1].locale;
                    sensors[Female::RIGHT_REAR_OBJECT_SENSOR] = World[bird->x + 1][bird->y - 1].object;
                    x = bird->x + 1;
                    y = bird->y - 1;
                }
                break;
            }
            break;

            // Rear.
        case 7:
            x = -1;
            y = -1;
            switch (bird->orientation)
            {
            case ORIENTATION::NORTH:
                if (bird->y < HEIGHT - 1)
                {
                    sensors[Female::REAR_LOCALE_SENSOR] = World[bird->x][bird->y + 1].locale;
                    sensors[Female::REAR_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
                    x = bird->x;
                    y = bird->y + 1;
                }
                break;

            case ORIENTATION::SOUTH:
                if (bird->y > 0)
                {
                    sensors[Female::REAR_LOCALE_SENSOR] = World[bird->x][bird->y - 1].locale;
                    sensors[Female::REAR_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
                    x = bird->x;
                    y = bird->y - 1;
                }
                break;

            case ORIENTATION::EAST:
                if (bird->x > 0)
                {
                    sensors[Female::REAR_LOCALE_SENSOR] = World[bird->x - 1][bird->y].locale;
                    sensors[Female::REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
                    x = bird->x - 1;
                    y = bird->y;
                }
                break;

            case ORIENTATION::WEST:
                if (bird->x < WIDTH - 1)
                {
                    sensors[Female::REAR_LOCALE_SENSOR] = World[bird->x + 1][bird->y].locale;
                    sensors[Female::REAR_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
                    x = bird->x + 1;
                    y = bird->y;
                }
                break;
            }
            break;

            // Left rear.
        case 8:
            x = -1;
            y = -1;
            switch (bird->orientation)
            {
            case ORIENTATION::NORTH:
                if (bird->x > 0 && bird->y < HEIGHT - 1)
                {
                    sensors[Female::LEFT_REAR_LOCALE_SENSOR] = World[bird->x - 1][bird->y + 1].locale;
                    sensors[Female::LEFT_REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y + 1].object;
                    x = bird->x - 1;
                    y = bird->y + 1;
                }
                break;

            case ORIENTATION::SOUTH:
                if (bird->x < WIDTH - 1 && bird->y > 0)
                {
                    sensors[Female::LEFT_REAR_LOCALE_SENSOR] = World[bird->x + 1][bird->y - 1].locale;
                    sensors[Female::LEFT_REAR_OBJECT_SENSOR] = World[bird->x + 1][bird->y - 1].object;
                    x = bird->x + 1;
                    y = bird->y - 1;
                }
                break;

            case ORIENTATION::EAST:
                if (bird->x > 0 && bird->y > 0)
                {
                    sensors[Female::LEFT_REAR_LOCALE_SENSOR] = World[bird->x - 1][bird->y - 1].locale;
                    sensors[Female::LEFT_REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y - 1].object;
                    x = bird->x - 1;
                    y = bird->y - 1;
                }
                break;

            case ORIENTATION::WEST:
                if (bird->x < WIDTH - 1 && bird->y < HEIGHT - 1)
                {
                    sensors[Female::LEFT_REAR_LOCALE_SENSOR] = World[bird->x + 1][bird->y + 1].locale;
                    sensors[Female::LEFT_REAR_OBJECT_SENSOR] = World[bird->x + 1][bird->y + 1].object;
                    x = bird->x + 1;
                    y = bird->y + 1;
                }
                break;
            }
            break;
        }
    }

    // Internal state.
    sensors[Female::ORIENTATION_SENSOR] = bird->orientation;
    if (bird->food > 0)
    {
        sensors[Female::HUNGER_SENSOR] = 0;
    }
    else {
        sensors[Female::HUNGER_SENSOR] = 1;
    }
    sensors[Female::HAS_OBJECT_SENSOR] = bird->hasObject;
}

// City block distance.
int distance(int x1, int y1, int x2, int y2)
{
    int xd = x1 - x2;
    int yd = y1 - y2;
    if (xd < 0.0) xd = -xd;
    if (yd < 0.0) yd = -yd;
    return xd + yd;
}

// Do bird response.
void doResponse(int gender)
{
    if (gender == MALE)
    {
        doMaleResponse();
    }
    else {
        doFemaleResponse();
    }
}

// Do male response.
void doMaleResponse()
{
   Male* bird = male;
   Cell *cell    = &World[bird->x][bird->y];
   int  response = bird->response;

      switch (response)
      {
      case Male::RESPONSE::DO_NOTHING:
         break;

      case Male::RESPONSE::EAT_MOUSE:
                if (bird->hasObject == OBJECT::MOUSE)
                {
                    if (Male::RANDOMIZE_FOOD_LEVEL && (Male::FOOD_DURATION > 0))
                    {
                        bird->food = rand() % (Male::FOOD_DURATION + 1);
                    }
                    else
                    {
                        bird->food = Male::FOOD_DURATION;
                    }
                    bird->hasObject = OBJECT::NO_OBJECT;
                }
         break;

      case Male::RESPONSE::GET_OBJECT:
         if (bird->hasObject == OBJECT::NO_OBJECT)
         {
            bird->hasObject = cell->object;
            cell->object    = OBJECT::NO_OBJECT;
         }
         break;

      case Male::RESPONSE::PUT_OBJECT:
         if (cell->object == OBJECT::NO_OBJECT)
         {
            cell->object    = bird->hasObject;
            bird->hasObject = OBJECT::NO_OBJECT;
         }
         break;

      case Male::RESPONSE::TOSS_OBJECT:
         if (bird->hasObject == OBJECT::NO_OBJECT)
         {
            break;
         }

         // Stone tossed back to desert.
         if (bird->hasObject == OBJECT::STONE)
         {
             for (int j = 0; j < 20; j++)
             {
                 int x = (int)(rand() % (long)13) + 8;
                 int y = (int)(rand() % (long)7) + 13;
                 if (World[x][y].locale == LOCALE::DESERT &&
                     World[x][y].object == OBJECT::NO_OBJECT)
                 {
                     World[x][y].object = OBJECT::STONE;
                     break;
                 }
             }
         }

         // Vaporize object.
         bird->hasObject = OBJECT::NO_OBJECT;
         break;

      case Male::RESPONSE::MOVE_FORWARD:
         switch (bird->orientation)
         {
         case ORIENTATION::NORTH:
            if (bird->y > 0)
            {
               bird->y--;
            }
            break;

         case ORIENTATION::SOUTH:
            if (bird->y < HEIGHT - 1)
            {
               bird->y++;
            }
            break;


         case ORIENTATION::EAST:
            if (bird->x < WIDTH - 1)
            {
               bird->x++;
            }
            break;

         case ORIENTATION::WEST:
            if (bird->x > 0)
            {
               bird->x--;
            }
            break;
         }
         break;

      case Male::RESPONSE::TURN_RIGHT:
         switch (bird->orientation)
         {
         case ORIENTATION::NORTH:
            bird->orientation = ORIENTATION::EAST;
            break;

         case ORIENTATION::SOUTH:
            bird->orientation = ORIENTATION::WEST;
            break;

         case ORIENTATION::EAST:
            bird->orientation = ORIENTATION::SOUTH;
            break;

         case ORIENTATION::WEST:
            bird->orientation = ORIENTATION::NORTH;
            break;
         }
         break;

      case Male::RESPONSE::TURN_LEFT:
         switch (bird->orientation)
         {
         case ORIENTATION::NORTH:
            bird->orientation = ORIENTATION::WEST;
            break;

         case ORIENTATION::SOUTH:
            bird->orientation = ORIENTATION::EAST;
            break;

         case ORIENTATION::EAST:
            bird->orientation = ORIENTATION::NORTH;
            break;

         case ORIENTATION::WEST:
            bird->orientation = ORIENTATION::SOUTH;
            break;
         }
         break;

      case Male::RESPONSE::TURN_AROUND:
          switch (bird->orientation)
          {
          case ORIENTATION::NORTH:
              bird->orientation = ORIENTATION::SOUTH;
              break;

          case ORIENTATION::SOUTH:
              bird->orientation = ORIENTATION::NORTH;
              break;

          case ORIENTATION::EAST:
              bird->orientation = ORIENTATION::WEST;
              break;

          case ORIENTATION::WEST:
              bird->orientation = ORIENTATION::EAST;
              break;
          }
          break;
 
         case Male::RESPONSE::GIVE_MOUSE:
               if ((male->hasObject == OBJECT::MOUSE) && (female->hasObject == OBJECT::NO_OBJECT))
               {
                  if ((male->x == female->x) && (male->y == female->y))
                  {
                     female->hasObject = male->hasObject;
                     male->hasObject   = OBJECT::NO_OBJECT;
                  }
               }
            break;

         case Male::RESPONSE::GIVE_STONE:
               if ((male->hasObject == OBJECT::STONE) && (female->hasObject == OBJECT::NO_OBJECT))
               {
                  if ((male->x == female->x) && (male->y == female->y))
                  {
                     female->hasObject = bird->hasObject;
                     male->hasObject   = OBJECT::NO_OBJECT;
                  }
               }
            break;
         }

   // Digest food.
   bird->digest();
}

// Do female response.
void doFemaleResponse()
{
    Female* bird = female;
    Cell* cell = &World[bird->x][bird->y];
    int  response = bird->response;

    switch (response)
    {
    case Female::RESPONSE::DO_NOTHING:
        break;

    case Female::RESPONSE::EAT_MOUSE:
        if (bird->hasObject == OBJECT::MOUSE)
        {
            if (Female::RANDOMIZE_FOOD_LEVEL && (Female::FOOD_DURATION > 0))
            {
                bird->food = rand() % (Female::FOOD_DURATION + 1);
            }
            else
            {
                bird->food = Female::FOOD_DURATION;
            }
            bird->hasObject = OBJECT::NO_OBJECT;
        }
        break;

    case Female::RESPONSE::GET_OBJECT:
        if (bird->hasObject == OBJECT::NO_OBJECT)
        {
            bird->hasObject = cell->object;
            cell->object = OBJECT::NO_OBJECT;
        }
        break;

    case Female::RESPONSE::PUT_OBJECT:
        if (cell->object == OBJECT::NO_OBJECT)
        {
            cell->object = bird->hasObject;
            bird->hasObject = OBJECT::NO_OBJECT;
        }
        break;

    case Female::RESPONSE::TOSS_OBJECT:
        if (bird->hasObject == OBJECT::NO_OBJECT)
        {
            break;
        }

        // Stone tossed back to desert.
        if (bird->hasObject == OBJECT::STONE)
        {
            for (int j = 0; j < 20; j++)
            {
                int x = (int)(rand() % (long)13) + 8;
                int y = (int)(rand() % (long)7) + 13;
                if (World[x][y].locale == LOCALE::DESERT &&
                    World[x][y].object == OBJECT::NO_OBJECT)
                {
                    World[x][y].object = OBJECT::STONE;
                    break;
                }
            }
        }

            // Vaporize object.
            bird->hasObject = OBJECT::NO_OBJECT;
        break;

    case Female::RESPONSE::MOVE_FORWARD:
        switch (bird->orientation)
        {
        case ORIENTATION::NORTH:
            if (bird->y > 0)
            {
                bird->y--;
            }
            break;

        case ORIENTATION::SOUTH:
            if (bird->y < HEIGHT - 1)
            {
                bird->y++;
            }
            break;


        case ORIENTATION::EAST:
            if (bird->x < WIDTH - 1)
            {
                bird->x++;
            }
            break;

        case ORIENTATION::WEST:
            if (bird->x > 0)
            {
                bird->x--;
            }
            break;
        }
        break;

    case Female::RESPONSE::TURN_RIGHT:
        switch (bird->orientation)
        {
        case ORIENTATION::NORTH:
            bird->orientation = ORIENTATION::EAST;
            break;

        case ORIENTATION::SOUTH:
            bird->orientation = ORIENTATION::WEST;
            break;

        case ORIENTATION::EAST:
            bird->orientation = ORIENTATION::SOUTH;
            break;

        case ORIENTATION::WEST:
            bird->orientation = ORIENTATION::NORTH;
            break;
        }
        break;

    case Female::RESPONSE::TURN_LEFT:
        switch (bird->orientation)
        {
        case ORIENTATION::NORTH:
            bird->orientation = ORIENTATION::WEST;
            break;

        case ORIENTATION::SOUTH:
            bird->orientation = ORIENTATION::EAST;
            break;

        case ORIENTATION::EAST:
            bird->orientation = ORIENTATION::NORTH;
            break;

        case ORIENTATION::WEST:
            bird->orientation = ORIENTATION::SOUTH;
            break;
        }
        break;

    case Female::RESPONSE::TURN_AROUND:
        switch (bird->orientation)
        {
        case ORIENTATION::NORTH:
            bird->orientation = ORIENTATION::SOUTH;
            break;

        case ORIENTATION::SOUTH:
            bird->orientation = ORIENTATION::NORTH;
            break;

        case ORIENTATION::EAST:
            bird->orientation = ORIENTATION::WEST;
            break;

        case ORIENTATION::WEST:
            bird->orientation = ORIENTATION::EAST;
            break;
        }
        break;

       case Female::RESPONSE::WANT_MOUSE:
           break;

       case Female::RESPONSE::WANT_STONE:
           break;

       case Female::RESPONSE::LAY_EGG:
           if (cell->object == OBJECT::NO_OBJECT)
           {
               cell->object = OBJECT::EGG;
           }
           break;
       }

   // Digest food.
   bird->digest();
}

// Move mice in forest.
void stepMice()
{
   for (int x = (rand() % WIDTH), x2 = 0; x2 < WIDTH; x = (x + 1) % WIDTH, x2++)
   {
      for (int y = (rand() % HEIGHT), y2 = 0; y2 < HEIGHT; y = (y + 1) % HEIGHT, y2++)
      {
         if ((World[x][y].object == OBJECT::MOUSE) &&
             ((male->x != x) || (male->y != y) || (male->response != Male::RESPONSE::GET_OBJECT)) &&
             ((((double)((long)rand()) / (double)(((long)RAND_MAX) + 1))) < MOUSE_MOVE_PROBABILITY))
         {
            bool move = false;
            for (int i = (rand() % 4), i2 = 0; i2 < 4 && !move; i = (i + 1) % 4, i2++)
            {
               switch (i)
               {
               case 0:
                  if ((y > 0) && (World[x][y - 1].locale == LOCALE::FOREST) &&
                      (World[x][y - 1].object == OBJECT::NO_OBJECT))
                  {
                     World[x][y].object     = OBJECT::NO_OBJECT;
                     World[x][y - 1].object = -OBJECT::MOUSE;
                     move = true;
                  }
                  break;

               case 1:
                  if ((y < HEIGHT - 1) && (World[x][y + 1].locale == LOCALE::FOREST) &&
                      (World[x][y + 1].object == OBJECT::NO_OBJECT))
                  {
                     World[x][y].object     = OBJECT::NO_OBJECT;
                     World[x][y + 1].object = -OBJECT::MOUSE;
                     move = true;
                  }
                  break;

               case 2:
                  if ((x > 0) && (World[x - 1][y].locale == LOCALE::FOREST) &&
                      (World[x - 1][y].object == OBJECT::NO_OBJECT))
                  {
                     World[x][y].object     = OBJECT::NO_OBJECT;
                     World[x - 1][y].object = -OBJECT::MOUSE;
                     move = true;
                  }
                  break;

               case 3:
                  if ((x < WIDTH - 1) && (World[x + 1][y].locale == LOCALE::FOREST) &&
                      (World[x + 1][y].object == OBJECT::NO_OBJECT))
                  {
                     World[x][y].object     = OBJECT::NO_OBJECT;
                     World[x + 1][y].object = -OBJECT::MOUSE;
                     move = true;
                  }
                  break;
               }
            }
         }
      }
   }
   for (int x = 0; x < WIDTH; x++)
   {
      for (int y = 0; y < HEIGHT; y++)
      {
         if (World[x][y].object == -OBJECT::MOUSE)
         {
            World[x][y].object = OBJECT::MOUSE;
         }
      }
   }
}


// Main.
int main(int argc, char *args[])
{
   bool gotMaleTrainTest = false;
   bool gotFemaleTrainTest = false;
   bool gotSteps     = false;

   for (int i = 1; i < argc; i++)
   {
      if (strcmp(args[i], "-trainMale") == 0)
      {
          i++;
          if (i >= argc)
          {
              fprintf(stderr, "Invalid male save file\n");
              fprintf(stderr, Usage);
              exit(1);
          }
          MaleFilename = args[i];
         MaleTest = false;
         if (gotMaleTrainTest)
         {
            fprintf(stderr, "Duplicate male train/test option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         gotMaleTrainTest = true;
         continue;
      }
      if (strcmp(args[i], "-testMale") == 0)
      {
          i++;
          if (i >= argc)
          {
              fprintf(stderr, "Invalid male load file\n");
              fprintf(stderr, Usage);
              exit(1);
          }
          MaleFilename = args[i];
         MaleTest = true;
         if (gotMaleTrainTest)
         {
            fprintf(stderr, "Duplicate male train/test option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         gotMaleTrainTest = true;
         continue;
      }
      if (strcmp(args[i], "-trainFemale") == 0)
      {
          i++;
          if (i >= argc)
          {
              fprintf(stderr, "Invalid female save file\n");
              fprintf(stderr, Usage);
              exit(1);
          }
          FemaleFilename = args[i];
          FemaleTest = false;
          if (gotFemaleTrainTest)
          {
              fprintf(stderr, "Duplicate female train/test option\n");
              fprintf(stderr, Usage);
              exit(1);
          }
          gotFemaleTrainTest = true;
          continue;
      }
      if (strcmp(args[i], "-testFemale") == 0)
      {
          i++;
          if (i >= argc)
          {
              fprintf(stderr, "Invalid female load file\n");
              fprintf(stderr, Usage);
              exit(1);
          }
          FemaleFilename = args[i];
          FemaleTest = true;
          if (gotFemaleTrainTest)
          {
              fprintf(stderr, "Duplicate female train/test option\n");
              fprintf(stderr, Usage);
              exit(1);
          }
          gotFemaleTrainTest = true;
          continue;
      }
      if (strcmp(args[i], "-steps") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid steps option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Steps = atoi(args[i]);
         if (Steps < 0)
         {
            fprintf(stderr, "Invalid steps option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         gotSteps = true;
         continue;
      }
      if (strcmp(args[i], "-maleInitialFood") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleInitialFood option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::INITIAL_FOOD = atoi(args[i]);
         if (Male::INITIAL_FOOD < 0)
         {
            fprintf(stderr, "Invalid maleInitialFood option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-maleFoodDuration") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleFoodDuration option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::FOOD_DURATION = atoi(args[i]);
         if (Male::FOOD_DURATION < 0)
         {
            fprintf(stderr, "Invalid maleFoodDuration option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-maleRandomizeFoodLevel") == 0)
      {
         Male::RANDOMIZE_FOOD_LEVEL = true;
         continue;
      }
      if (strcmp(args[i], "-maleMouseNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleMouseNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::MOUSE_NEED = strtod(args[i], 0);
         if (Male::MOUSE_NEED < 0.0)
         {
            fprintf(stderr, "Invalid maleMouseNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-maleFemaleMouseNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleFemaleMouseNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::FEMALE_MOUSE_NEED = strtod(args[i], 0);
         if (Male::FEMALE_MOUSE_NEED < 0.0)
         {
            fprintf(stderr, "Invalid maleFemaleMouseNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-maleAttendFemaleNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleAttendFemaleNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::ATTEND_FEMALE_NEED = strtod(args[i], 0);
         if (Male::ATTEND_FEMALE_NEED < 0.0)
         {
            fprintf(stderr, "Invalid maleAttendFemaleNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-maleFemaleStoneNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleFemaleStoneNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::FEMALE_STONE_NEED = strtod(args[i], 0);
         if (Male::FEMALE_STONE_NEED < 0.0)
         {
            fprintf(stderr, "Invalid maleFemaleStoneNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-femaleInitialFood") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid femaleInitialFood option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Female::INITIAL_FOOD = atoi(args[i]);
         if (Female::INITIAL_FOOD < 0)
         {
            fprintf(stderr, "Invalid femaleInitialFood option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-femaleFoodDuration") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid femaleFoodDuration option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Female::FOOD_DURATION = atoi(args[i]);
         if (Female::FOOD_DURATION < 0)
         {
            fprintf(stderr, "Invalid femaleFoodDuration option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-femaleRandomizeFoodLevel") == 0)
      {
         Female::RANDOMIZE_FOOD_LEVEL = true;
         continue;
      }
      if (strcmp(args[i], "-femaleMouseNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid femaleMouseNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Female::MOUSE_NEED = 1.0;
         Female::MOUSE_NEED = strtod(args[i], 0);
         if (Female::MOUSE_NEED < 0.0)
         {
            fprintf(stderr, "Invalid femaleMouseNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-femaleStoneNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid femaleStoneNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Female::STONE_NEED = strtod(args[i], 0);
         if (Female::STONE_NEED < 0.0)
         {
            fprintf(stderr, "Invalid femaleStoneNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-femaleLayEggNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid femaleLayEggNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Female::LAY_EGG_NEED = strtod(args[i], 0);
         if (Female::LAY_EGG_NEED < 0.0)
         {
            fprintf(stderr, "Invalid femaleLayEggNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-femaleBroodEggNeed") == 0)
      {
          i++;
          if (i >= argc)
          {
              fprintf(stderr, "Invalid femaleBroodEggNeed option\n");
              fprintf(stderr, Usage);
              exit(1);
          }
          Female::BROOD_EGG_NEED = strtod(args[i], 0);
          if (Female::BROOD_EGG_NEED < 0.0)
          {
              fprintf(stderr, "Invalid femaleBroodEggNeed option\n");
              fprintf(stderr, Usage);
              exit(1);
          }
          continue;
      }
      if (strcmp(args[i], "-randomSeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid randomSeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         RANDOM_NUMBER_SEED = atoi(args[i]);
         if (RANDOM_NUMBER_SEED <= 0)
         {
            fprintf(stderr, "Invalid randomSeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-verbose") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid verbose option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         if (strcmp(args[i], "true") == 0)
         {
            Verbose = true;
         }
         else if (strcmp(args[i], "false") == 0)
         {
            Verbose = false;
         }
         else
         {
            fprintf(stderr, "Invalid verbose option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-version") == 0)
      {
         printf("Nesting birds version = %s\n", VERSION);
         exit(0);
      }
      if ((strcmp(args[i], "-help") == 0) || (strcmp(args[i], "-h") == 0) || (strcmp(args[i], "-?") == 0))
      {
         printf(Usage);
         exit(0);
      }
      fprintf(stderr, Usage);
      exit(1);
   }
   if (!gotMaleTrainTest)
   {
      fprintf(stderr, Usage);
      exit(1);
   }
   if (!gotFemaleTrainTest)
   {
       fprintf(stderr, Usage);
       exit(1);
   }
   if (!gotSteps)
   {
      fprintf(stderr, Usage);
      exit(1);
   }
   if (strcmp(MaleFilename, FemaleFilename) == 0)
   {
       fprintf(stderr, "Male and female file names must be different\n");
       fprintf(stderr, Usage);
       exit(1);
   }
   if (Male::INITIAL_FOOD > Male::FOOD_DURATION)
   {
      fprintf(stderr, "Male initial food cannot be greater than food duration\n");
      fprintf(stderr, Usage);
      exit(1);
   }
   if (Female::INITIAL_FOOD > Female::FOOD_DURATION)
   {
      fprintf(stderr, "Female initial food cannot be greater than food duration");
      fprintf(stderr, Usage);
      exit(1);
   }

   // Initialize.
   init();

   // Load?
   if (MaleTest)
   {
       male->load(MaleFilename);
       if (Verbose)
       {
           printf("Male network loaded from file %s\n", MaleFilename);
       }

       // Initialize needs.
       male->initNeeds();
   }
   if (FemaleTest)
   {
       female->load(FemaleFilename);
       if (Verbose)
       {
           printf("Female network loaded from file %s\n", FemaleFilename);
       }

       // Initialize needs.
       female->initNeeds();
   }

   // Run birds.
   int eggLaidStep = -1;
   for (int i = 0; i < Steps; i++)
   {
      if (Verbose)
      {
         printf("Step=%d\n", i);
      }
      step();
      if ((eggLaidStep < 0) && (World[WIDTH / 2][(HEIGHT / 2) + 1].object == OBJECT::EGG))
      {
         eggLaidStep = i;
      }
   }

   if (Verbose)
   {
      printf("Run results: ");
      if (eggLaidStep >= 0)
      {
         printf("egg laid at step %d", eggLaidStep);
      }
      else
      {
         printf("no egg");
      }
      int mouseCount = 0;
      int stoneCount = 0;
      for (int x = 0; x < WIDTH; x++)
      {
         for (int y = 0; y < HEIGHT; y++)
         {
            if ((World[x][y].locale == LOCALE::FOREST) &&
                (World[x][y].object == OBJECT::MOUSE))
            {
               mouseCount++;
            }
            if ((World[x][y].locale == LOCALE::DESERT) &&
                (World[x][y].object == OBJECT::STONE))
            {
               stoneCount++;
            }
         }
      }
      printf(", remaining mice=%d, remaining stones=%d\n", mouseCount, stoneCount);
   }

   // Save training?
   if (!MaleTest)
   {
       male->save(MaleFilename);
       if (Verbose)
       {
           printf("Male training saved to file %s\n", MaleFilename);
       }
   }
   if (!FemaleTest)
   {
       female->save(FemaleFilename);
       if (Verbose)
       {
           printf("Female training saved to file %s\n", FemaleFilename);
       }
   }
   exit(0);
}
