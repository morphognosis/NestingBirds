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
   "      [-maleStoneNeed <amount> (default=" MALE_DEFAULT_STONE_NEED ")]\n"
   "      [-maleFemaleStoneNeed <amount> (default=" MALE_DEFAULT_FEMALE_STONE_NEED ")]\n"
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
static double MOUSE_MOVE_PROBABILITY = 0.1;

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
            break;

         case 2:
            World[x][y].locale = LOCALE::FOREST;
            World[x][y].object = OBJECT::MOUSE;
            break;

         case 3:
            World[x][y].locale = LOCALE::DESERT;
            World[x][y].object = OBJECT::NO_OBJECT;
            break;

         case 4:
            World[x][y].locale = LOCALE::DESERT;
            World[x][y].object = OBJECT::STONE;
            break;
         }
      }
   }

   // Create birds.
   female             = new Female();
   female->x          = WIDTH / 2;
   female->y          = HEIGHT / 2;
   female->food       = Female::INITIAL_FOOD;
   female->response   = Bird::RESPONSE::DO_NOTHING;
   male               = new Male();
   male->x            = WIDTH / 2;
   male->y            = HEIGHT / 2;
   male->food         = Male::INITIAL_FOOD;
   male->response     = Bird::RESPONSE::DO_NOTHING;
   FemaleWantMouse    = false;
   FemaleWantStone    = false;
   FemaleNestSequence = 0;
   setSensors(Bird::FEMALE);
   setSensors(Bird::MALE);
}


// Step.
void step()
{
   // Do responses in world.
   doResponse(Bird::FEMALE);
   doResponse(Bird::MALE);

   // Step mice.
   stepMice();

   // Set female sensors.
   setSensors(Bird::FEMALE);

   // Train?
   if (!FemaleTest)
   {
      train(Bird::FEMALE);
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

   // Set male sensors.
   setSensors(Bird::MALE);

   // Train?
   if (!MaleTest)
   {
      train(Bird::MALE);
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
}


// Train.
void train(int gender)
{
   if (gender == Bird::MALE)
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
        male->response = Bird::RESPONSE::DO_NOTHING;
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
   if ((male->x == female->x) && (male->y == female->y))
   {
      if (female->response == Female::RESPONSE::WANT_MOUSE)
      {
         FemaleWantMouse = true;
      }
   }
   if ((male->food > 0) && !FemaleWantMouse)
   {
      return(false);
   }
   if (male->hasObject == OBJECT::MOUSE)
   {
      if (male->food < 0)
      {
         male->food = 0;
      }
      if (male->food == 0)
      {
         male->response = Bird::RESPONSE::EAT_MOUSE;
         return(true);
      }
      if ((male->x == female->x) && (male->y == female->y))
      {
         male->response  = Male::RESPONSE::GIVE_MOUSE;
         FemaleWantMouse = false;
         return(true);
      }
      else
      {
         return(false);
      }
   }
   if (male->hasObject == OBJECT::STONE)
   {
       if ((male->x == female->x) && (male->y == female->y))
       {
           male->response = Male::RESPONSE::GIVE_STONE;
           FemaleWantStone = false;
           return(true);
       }
       else
       {
           return(false);
       }
   }
   if (World[male->x][male->y].object == OBJECT::MOUSE)
   {
      male->response = Bird::RESPONSE::GET_OBJECT;
      return(true);
   }
   if (male->sensors[Bird::LEFT_OBJECT_SENSOR] == OBJECT::MOUSE)
   {
      male->response = Bird::RESPONSE::TURN_LEFT;
      return(true);
   }
   if (male->sensors[Bird::LEFT_FRONT_OBJECT_SENSOR] == OBJECT::MOUSE)
   {
       male->response = Bird::RESPONSE::MOVE_FORWARD;
       return(true);
   }
   if (male->sensors[Bird::FRONT_OBJECT_SENSOR] == OBJECT::MOUSE)
   {
      male->response = Bird::RESPONSE::MOVE_FORWARD;
      return(true);
   }
   if (male->sensors[Bird::RIGHT_FRONT_OBJECT_SENSOR] == OBJECT::MOUSE)
   {
      male->response = Bird::RESPONSE::MOVE_FORWARD;
      return(true);
   }
   if (male->sensors[Bird::RIGHT_OBJECT_SENSOR] == OBJECT::MOUSE)
   {
       male->response = Bird::RESPONSE::TURN_RIGHT;
       return(true);
   }
   if (male->sensors[Bird::RIGHT_REAR_OBJECT_SENSOR] == OBJECT::MOUSE)
   {
       male->response = Bird::RESPONSE::TURN_AROUND;
       return(true);
   }
   if (male->sensors[Bird::REAR_OBJECT_SENSOR] == OBJECT::MOUSE)
   {
       male->response = Bird::RESPONSE::TURN_AROUND;
       return(true);
   }
   if (male->sensors[Bird::LEFT_REAR_OBJECT_SENSOR] == OBJECT::MOUSE)
   {
       male->response = Bird::RESPONSE::TURN_AROUND;
       return(true);
   }
   if ((male->sensors[Bird::CURRENT_LOCALE_SENSOR] == LOCALE::FOREST) ||
       (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::FOREST) ||
       (male->sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] == LOCALE::FOREST) ||
       (male->sensors[Bird::FRONT_LOCALE_SENSOR] == LOCALE::FOREST) ||
       (male->sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] == LOCALE::FOREST) ||
       (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::FOREST) ||
       (male->sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] == LOCALE::FOREST) ||
       (male->sensors[Bird::REAR_LOCALE_SENSOR] == LOCALE::FOREST) ||
       (male->sensors[Bird::LEFT_REAR_LOCALE_SENSOR] == LOCALE::FOREST))
   {
      if (male->sensors[Bird::CURRENT_LOCALE_SENSOR] == LOCALE::FOREST)
      {
         vector<int> responses;
         if ((male->sensors[Bird::LEFT_LOCALE_SENSOR] != LOCALE::FOREST) &&
             (male->sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] != LOCALE::FOREST) &&
             (male->sensors[Bird::FRONT_LOCALE_SENSOR] != LOCALE::FOREST) &&
             (male->sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] != LOCALE::FOREST) &&
             (male->sensors[Bird::RIGHT_LOCALE_SENSOR] != LOCALE::FOREST) &&
             (male->sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] != LOCALE::FOREST) &&
             (male->sensors[Bird::REAR_LOCALE_SENSOR] != LOCALE::FOREST) &&
             (male->sensors[Bird::LEFT_REAR_LOCALE_SENSOR] != LOCALE::FOREST))
         {
            int r = Bird::RESPONSE::MOVE_FORWARD;
            responses.push_back(r);
            responses.push_back(r);
            r = Bird::RESPONSE::TURN_LEFT;
            responses.push_back(r);
            r = Bird::RESPONSE::TURN_RIGHT;
            responses.push_back(r);
            r = Bird::RESPONSE::TURN_AROUND;
            responses.push_back(r);
         }
         else
         {
            if (male->sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] == LOCALE::FOREST ||
                male->sensors[Bird::FRONT_LOCALE_SENSOR] == LOCALE::FOREST ||
                male->sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] == LOCALE::FOREST)
            {
               int r = Bird::RESPONSE::MOVE_FORWARD;
               responses.push_back(r);
               responses.push_back(r);
            }
            if (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::FOREST)
            {
               int r = Bird::RESPONSE::TURN_LEFT;
               responses.push_back(r);
            }
            if (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::FOREST)
            {
               int r = Bird::RESPONSE::TURN_RIGHT;
               responses.push_back(r);
            }
            if (male->sensors[Bird::LEFT_REAR_LOCALE_SENSOR] == LOCALE::FOREST ||
                male->sensors[Bird::REAR_LOCALE_SENSOR] == LOCALE::FOREST ||
                male->sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] == LOCALE::FOREST)
            {
                int r = Bird::RESPONSE::TURN_AROUND;
                responses.push_back(r);
            }
         }
         male->response = responses[rand() % responses.size()];
      }
      else
      {
         vector<int> responses;
         if (male->sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] == LOCALE::FOREST ||
             male->sensors[Bird::FRONT_LOCALE_SENSOR] == LOCALE::FOREST ||
             male->sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] == LOCALE::FOREST)
         {
            int r = Bird::RESPONSE::MOVE_FORWARD;
            responses.push_back(r);
            responses.push_back(r);
         }
         if (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::FOREST)
         {
            int r = Bird::RESPONSE::TURN_LEFT;
            responses.push_back(r);
         }
         if (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::FOREST)
         {
            int r = Bird::RESPONSE::TURN_RIGHT;
            responses.push_back(r);
         }
         if (male->sensors[Bird::LEFT_REAR_LOCALE_SENSOR] == LOCALE::FOREST ||
             male->sensors[Bird::REAR_LOCALE_SENSOR] == LOCALE::FOREST ||
             male->sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] == LOCALE::FOREST)
         {
             int r = Bird::RESPONSE::TURN_AROUND;
             responses.push_back(r);
         }
         male->response = responses[rand() % responses.size()];
      }
      return(true);
   }
   int mousex = 3;
   int mousey = 5;
   if (male->x > mousex)
   {
      if (male->orientation == Bird::ORIENTATION::WEST)
      {
         male->response = Bird::RESPONSE::MOVE_FORWARD;
      }
      else
      {
         if (male->orientation == Bird::ORIENTATION::SOUTH)
         {
            male->response = Bird::RESPONSE::TURN_RIGHT;
         }
         else if (male->orientation == Bird::ORIENTATION::NORTH)
         {
            male->response = Bird::RESPONSE::TURN_LEFT;
         } else {
             male->response = Bird::RESPONSE::TURN_AROUND;
         }
      }
      return(true);
   }
   if (male->x < mousex)
   {
      if (male->orientation == Bird::ORIENTATION::EAST)
      {
         male->response = Bird::RESPONSE::MOVE_FORWARD;
      }
      else
      {
         if (male->orientation == Bird::ORIENTATION::SOUTH)
         {
            male->response = Bird::RESPONSE::TURN_LEFT;
         }
         else if (male->orientation == Bird::ORIENTATION::NORTH)
         {
             male->response = Bird::RESPONSE::TURN_RIGHT;
         }
         else
         {
            male->response = Bird::RESPONSE::TURN_AROUND;
         }
      }
      return(true);
   }
   if (male->y > mousey)
   {
      if (male->orientation == Bird::ORIENTATION::NORTH)
      {
         male->response = Bird::RESPONSE::MOVE_FORWARD;
      }
      else
      {
         if (male->orientation == Bird::ORIENTATION::EAST)
         {
            male->response = Bird::RESPONSE::TURN_LEFT;
         }
         else if (male->orientation == Bird::ORIENTATION::WEST)
         {
            male->response = Bird::RESPONSE::TURN_RIGHT;
         }
         else 
         {
             male->response = Bird::RESPONSE::TURN_AROUND;
         }
      }
      return(true);
   }
   if (male->y < mousey)
   {
      if (male->orientation == Bird::ORIENTATION::SOUTH)
      {
         male->response = Bird::RESPONSE::MOVE_FORWARD;
      }
      else
      {
         if (male->orientation == Bird::ORIENTATION::EAST)
         {
            male->response = Bird::RESPONSE::TURN_RIGHT;
         }
         else if(male->orientation == Bird::ORIENTATION::WEST)
         {
            male->response = Bird::RESPONSE::TURN_LEFT;
         }
         else 
         {
             male->response = Bird::RESPONSE::TURN_AROUND;
         }
      }
      return true;
   }
   vector<int> responses;
   int r = Bird::RESPONSE::MOVE_FORWARD;
   responses.push_back(r);
   responses.push_back(r);
   r = Bird::RESPONSE::TURN_LEFT;
   responses.push_back(r);
   r = Bird::RESPONSE::TURN_RIGHT;
   responses.push_back(r);
   r = Bird::RESPONSE::TURN_AROUND;
   responses.push_back(r);
   male->response = responses[rand() % responses.size()];
   return(true);
}


// Get stone.
bool getStone()
{
   if ((male->x == female->x) && (male->y == female->y))
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
      if ((male->x == female->x) && (male->y == female->y))
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
   if ((World[male->x][male->y].object == OBJECT::STONE) &&
       (World[male->x][male->y].locale == LOCALE::DESERT))
   {
      male->response = Bird::RESPONSE::GET_OBJECT;
      return(true);
   }
   if ((male->sensors[Bird::LEFT_OBJECT_SENSOR] == OBJECT::STONE) &&
       (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::DESERT))
   {
      male->response = Bird::RESPONSE::TURN_LEFT;
      return(true);
   }
   if ((male->sensors[Bird::LEFT_FRONT_OBJECT_SENSOR] == OBJECT::STONE) &&
       (male->sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] == LOCALE::DESERT))
   {
      male->response = Bird::RESPONSE::MOVE_FORWARD;
      return(true);
   }
   if ((male->sensors[Bird::FRONT_OBJECT_SENSOR] == OBJECT::STONE) &&
       (male->sensors[Bird::FRONT_LOCALE_SENSOR] == LOCALE::DESERT))
   {
       male->response = Bird::RESPONSE::MOVE_FORWARD;
       return(true);
   }
   if ((male->sensors[Bird::RIGHT_FRONT_OBJECT_SENSOR] == OBJECT::STONE) &&
       (male->sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] == LOCALE::DESERT))
   {
       male->response = Bird::RESPONSE::MOVE_FORWARD;
       return(true);
   }
   if ((male->sensors[Bird::RIGHT_OBJECT_SENSOR] == OBJECT::STONE) &&
       (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::DESERT))
   {
      male->response = Bird::RESPONSE::TURN_RIGHT;
      return(true);
   }
   if ((male->sensors[Bird::RIGHT_REAR_OBJECT_SENSOR] == OBJECT::STONE) &&
       (male->sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] == LOCALE::DESERT))
   {
       male->response = Bird::RESPONSE::TURN_AROUND;
       return(true);
   }
   if ((male->sensors[Bird::REAR_OBJECT_SENSOR] == OBJECT::STONE) &&
       (male->sensors[Bird::REAR_LOCALE_SENSOR] == LOCALE::DESERT))
   {
       male->response = Bird::RESPONSE::TURN_AROUND;
       return(true);
   }
   if ((male->sensors[Bird::LEFT_REAR_OBJECT_SENSOR] == OBJECT::STONE) &&
       (male->sensors[Bird::LEFT_REAR_LOCALE_SENSOR] == LOCALE::DESERT))
   {
       male->response = Bird::RESPONSE::TURN_AROUND;
       return(true);
   }
   if ((male->sensors[Bird::CURRENT_LOCALE_SENSOR] == LOCALE::DESERT) ||
       (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::DESERT) ||
       (male->sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] == LOCALE::DESERT) ||
       (male->sensors[Bird::FRONT_LOCALE_SENSOR] == LOCALE::DESERT) ||
       (male->sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] == LOCALE::DESERT) ||
       (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::DESERT) ||
       (male->sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] == LOCALE::DESERT) ||
       (male->sensors[Bird::REAR_LOCALE_SENSOR] == LOCALE::DESERT) ||
       (male->sensors[Bird::LEFT_REAR_LOCALE_SENSOR] == LOCALE::DESERT))
   {
      if (male->sensors[Bird::CURRENT_LOCALE_SENSOR] == LOCALE::DESERT)
      {
         vector<int> responses;
         if ((male->sensors[Bird::LEFT_LOCALE_SENSOR] != LOCALE::DESERT) &&
             (male->sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] != LOCALE::DESERT) &&
             (male->sensors[Bird::FRONT_LOCALE_SENSOR] != LOCALE::DESERT) &&
             (male->sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] != LOCALE::DESERT) &&
             (male->sensors[Bird::RIGHT_LOCALE_SENSOR] != LOCALE::DESERT) &&
             (male->sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] != LOCALE::DESERT) &&
             (male->sensors[Bird::REAR_LOCALE_SENSOR] != LOCALE::DESERT) &&
             (male->sensors[Bird::LEFT_REAR_LOCALE_SENSOR] != LOCALE::DESERT))
         {
             int r = Bird::RESPONSE::MOVE_FORWARD;
             responses.push_back(r);
             responses.push_back(r);
             r = Bird::RESPONSE::TURN_LEFT;
             responses.push_back(r);
             r = Bird::RESPONSE::TURN_RIGHT;
             responses.push_back(r);
             r = Bird::RESPONSE::TURN_AROUND;
             responses.push_back(r);
         }
         else
         {
             if (male->sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] == LOCALE::DESERT ||
                 male->sensors[Bird::FRONT_LOCALE_SENSOR] == LOCALE::DESERT ||
                 male->sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] == LOCALE::DESERT)
            {
               int r = Bird::RESPONSE::MOVE_FORWARD;
               responses.push_back(r);
               responses.push_back(r);
            }
            if (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::DESERT)
            {
               int r = Bird::RESPONSE::TURN_LEFT;
               responses.push_back(r);
            }
            if (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::DESERT)
            {
               int r = Bird::RESPONSE::TURN_RIGHT;
               responses.push_back(r);
            }
            if (male->sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] == LOCALE::DESERT  ||
                male->sensors[Bird::REAR_LOCALE_SENSOR] == LOCALE::DESERT  ||
                male->sensors[Bird::LEFT_REAR_LOCALE_SENSOR] == LOCALE::DESERT)
            {
                int r = Bird::RESPONSE::TURN_AROUND;
                responses.push_back(r);
            }
         }
         male->response = responses[rand() % responses.size()];
      }
      else
      {
         vector<int> responses;
         if (male->sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] == LOCALE::DESERT ||
             male->sensors[Bird::FRONT_LOCALE_SENSOR] == LOCALE::DESERT ||
             male->sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] == LOCALE::DESERT)
         {
            int r = Bird::RESPONSE::MOVE_FORWARD;
            responses.push_back(r);
            responses.push_back(r);
         }
         if (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::DESERT)
         {
            int r = Bird::RESPONSE::TURN_LEFT;
            responses.push_back(r);
         }
         if (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::DESERT)
         {
            int r = Bird::RESPONSE::TURN_RIGHT;
            responses.push_back(r);
         }
         if (male->sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] == LOCALE::DESERT ||
             male->sensors[Bird::REAR_LOCALE_SENSOR] == LOCALE::DESERT ||
             male->sensors[Bird::LEFT_REAR_LOCALE_SENSOR] == LOCALE::DESERT)
         {
             int r = Bird::RESPONSE::TURN_AROUND;
             responses.push_back(r);
         }
         male->response = responses[rand() % responses.size()];
      }
      return(true);
   }
   int stonex = 14;
   int stoney = 16;
   if (male->x > stonex)
   {
      if (male->orientation == Bird::ORIENTATION::WEST)
      {
         male->response = Bird::RESPONSE::MOVE_FORWARD;
      }
      else
      {
         if (male->orientation == Bird::ORIENTATION::SOUTH)
         {
            male->response = Bird::RESPONSE::TURN_RIGHT;
         }
         else if (male->orientation == Bird::ORIENTATION::NORTH)
         {
            male->response = Bird::RESPONSE::TURN_LEFT;
         }
         else 
         {
             male->response = Bird::RESPONSE::TURN_AROUND;
         }
      }
      return(true);
   }
   if (male->x < stonex)
   {
      if (male->orientation == Bird::ORIENTATION::EAST)
      {
         male->response = Bird::RESPONSE::MOVE_FORWARD;
      }
      else
      {
         if (male->orientation == Bird::ORIENTATION::SOUTH)
         {
            male->response = Bird::RESPONSE::TURN_LEFT;
         }
         else if (male->orientation == Bird::ORIENTATION::NORTH)
         {
            male->response = Bird::RESPONSE::TURN_RIGHT;
         }
         else 
         {
             male->response = Bird::RESPONSE::TURN_AROUND;
         }
      }
      return(true);
   }
   if (male->y > stoney)
   {
      if (male->orientation == Bird::ORIENTATION::NORTH)
      {
         male->response = Bird::RESPONSE::MOVE_FORWARD;
      }
      else
      {
         if (male->orientation == Bird::ORIENTATION::EAST)
         {
            male->response = Bird::RESPONSE::TURN_LEFT;
         }
         else if (male->orientation == Bird::ORIENTATION::WEST)
         {
            male->response = Bird::RESPONSE::TURN_RIGHT;
         }
         else 
         {
             male->response = Bird::RESPONSE::TURN_AROUND;
         }
      }
      return(true);
   }
   if (male->y < stoney)
   {
      if (male->orientation == Bird::ORIENTATION::SOUTH)
      {
         male->response = Bird::RESPONSE::MOVE_FORWARD;
      }
      else
      {
         if (male->orientation == Bird::ORIENTATION::EAST)
         {
            male->response = Bird::RESPONSE::TURN_RIGHT;
         }
         else if (male->orientation == Bird::ORIENTATION::WEST)
         {
            male->response = Bird::RESPONSE::TURN_LEFT;
         }
         else 
         {
             male->response = Bird::RESPONSE::TURN_AROUND;
         }
      }
      return true;
   }
   vector<int> responses;
   int r = Bird::RESPONSE::MOVE_FORWARD;
   responses.push_back(r);
   responses.push_back(r);
   r = Bird::RESPONSE::TURN_LEFT;
   responses.push_back(r);
   r = Bird::RESPONSE::TURN_RIGHT;
   responses.push_back(r);
   r = Bird::RESPONSE::TURN_AROUND;
   responses.push_back(r);
   male->response = responses[rand() % responses.size()];
   return(true);
}


// Male returns to female.
bool returnToFemale()
{
   switch (male->sensors[Male::MATE_PROXIMITY_SENSOR])
   {
   case Male::MATE_PROXIMITY_PRESENT:
      male->response = Bird::RESPONSE::DO_NOTHING;
      return(true);

   case Male::MATE_PROXIMITY_LEFT:
      male->response = Bird::RESPONSE::TURN_LEFT;
      break;

   case Male::MATE_PROXIMITY_LEFT_FRONT:
   case Male::MATE_PROXIMITY_FRONT:
   case Male::MATE_PROXIMITY_RIGHT_FRONT:
      male->response = Bird::RESPONSE::MOVE_FORWARD;
      break;

   case Male::MATE_PROXIMITY_RIGHT:
      male->response = Bird::RESPONSE::TURN_RIGHT;
      break;

   case Male::MATE_PROXIMITY_RIGHT_REAR:
   case Male::MATE_PROXIMITY_REAR:
   case Male::MATE_PROXIMITY_LEFT_REAR:
       male->response = Bird::RESPONSE::TURN_AROUND;
       break;

   case Male::MATE_PROXIMITY_UNKNOWN:
      int centerx = WIDTH / 2;
      int centery = (HEIGHT / 2) + 1;
      if (male->x < centerx)
      {
         if (male->orientation == Bird::ORIENTATION::EAST)
         {
            male->response = Bird::RESPONSE::MOVE_FORWARD;
         }
         else
         {
            if (male->orientation == Bird::ORIENTATION::SOUTH)
            {
               male->response = Bird::RESPONSE::TURN_LEFT;
            }
            else if (male->orientation == Bird::ORIENTATION::NORTH)
            {
               male->response = Bird::RESPONSE::TURN_RIGHT;
            }
            else 
            {
                male->response = Bird::RESPONSE::TURN_AROUND;
            }
         }
      }
      else if (male->x > centerx)
      {
         if (male->orientation == Bird::ORIENTATION::WEST)
         {
            male->response = Bird::RESPONSE::MOVE_FORWARD;
         }
         else
         {
            if (male->orientation == Bird::ORIENTATION::SOUTH)
            {
               male->response = Bird::RESPONSE::TURN_RIGHT;
            }
            else if (male->orientation == Bird::ORIENTATION::NORTH)
            {
               male->response = Bird::RESPONSE::TURN_LEFT;
            }
            else 
            {
                male->response = Bird::RESPONSE::TURN_AROUND;
            }
         }
      }
      else if (male->y < centery)
      {
         if (male->orientation == Bird::ORIENTATION::SOUTH)
         {
            male->response = Bird::RESPONSE::MOVE_FORWARD;
         }
         else
         {
            if (male->orientation == Bird::ORIENTATION::EAST)
            {
               male->response = Bird::RESPONSE::TURN_RIGHT;
            }
            else if (male->orientation == Bird::ORIENTATION::WEST)
            {
               male->response = Bird::RESPONSE::TURN_LEFT;
            }
            else 
            {
                male->response = Bird::RESPONSE::TURN_AROUND;
            }
         }
      }
      else if (male->y > centery)
      {
         if (male->orientation == Bird::ORIENTATION::NORTH)
         {
            male->response = Bird::RESPONSE::MOVE_FORWARD;
         }
         else
         {
            if (male->orientation == Bird::ORIENTATION::WEST)
            {
               male->response = Bird::RESPONSE::TURN_RIGHT;
            }
            else if (male->orientation == Bird::ORIENTATION::EAST)
            {
               male->response = Bird::RESPONSE::TURN_LEFT;
            }
            else 
            {
                male->response = Bird::RESPONSE::TURN_AROUND;
            }
         }
      }
      else
      {
         male->response = Bird::RESPONSE::MOVE_FORWARD;
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
        female->response = Bird::RESPONSE::DO_NOTHING;
        return;
    }

    // Hunger.
   if (female->food < 0)
   {
      female->food = 0;
   }
   if (female->food == 0)
   {
      if (female->hasObject == OBJECT::MOUSE)
      {
         female->response = Bird::RESPONSE::EAT_MOUSE;
      }
      else if (female->hasObject == OBJECT::STONE)
        {
        female->response = Bird::RESPONSE::PUT_OBJECT;
        }
        else
        {
        female->response = Female::RESPONSE::WANT_MOUSE;
        }
      return;
   }

   // Nest building.
   switch (FemaleNestSequence)
   {
   case 0:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         if (female->orientation == Bird::ORIENTATION::WEST)
         {
            female->response = Bird::RESPONSE::MOVE_FORWARD;
            FemaleNestSequence++;
         }
         else
         {
            female->response = Bird::RESPONSE::TURN_LEFT;
         }
      }
      else if (female->hasObject == OBJECT::STONE)
      {
         female->response = Bird::RESPONSE::PUT_OBJECT;
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 1:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         if (female->orientation == Bird::ORIENTATION::SOUTH)
         {
            female->response = Bird::RESPONSE::MOVE_FORWARD;
            FemaleNestSequence++;
         }
         else
         {
            female->response = Bird::RESPONSE::TURN_LEFT;
         }
      }
      else if (female->hasObject == OBJECT::STONE)
      {
         female->response = Bird::RESPONSE::PUT_OBJECT;
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 2:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         female->response = Bird::RESPONSE::MOVE_FORWARD;
         FemaleNestSequence++;
      }
      else if (female->hasObject == OBJECT::STONE)
      {
         female->response = Bird::RESPONSE::PUT_OBJECT;
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 3:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         if (female->orientation == Bird::ORIENTATION::EAST)
         {
            female->response = Bird::RESPONSE::MOVE_FORWARD;
            FemaleNestSequence++;
         }
         else
         {
            female->response = Bird::RESPONSE::TURN_LEFT;
         }
      }
      else if (female->hasObject == OBJECT::STONE)
      {
         female->response = Bird::RESPONSE::PUT_OBJECT;
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 4:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         female->response = Bird::RESPONSE::MOVE_FORWARD;
         FemaleNestSequence++;
      }
      else if (female->hasObject == OBJECT::STONE)
      {
         female->response = Bird::RESPONSE::PUT_OBJECT;
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 5:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         if (female->orientation == Bird::ORIENTATION::NORTH)
         {
            female->response = Bird::RESPONSE::MOVE_FORWARD;
            FemaleNestSequence++;
         }
         else
         {
            female->response = Bird::RESPONSE::TURN_LEFT;
         }
      }
      else if (female->hasObject == OBJECT::STONE)
      {
         female->response = Bird::RESPONSE::PUT_OBJECT;
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 6:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         female->response = Bird::RESPONSE::MOVE_FORWARD;
         FemaleNestSequence++;
      }
      else if (female->hasObject == OBJECT::STONE)
      {
         female->response = Bird::RESPONSE::PUT_OBJECT;
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 7:
      if (World[female->x][female->y].object == OBJECT::STONE)
      {
         if (female->orientation == Bird::ORIENTATION::WEST)
         {
            female->response = Bird::RESPONSE::MOVE_FORWARD;
            FemaleNestSequence++;
         }
         else
         {
            female->response = Bird::RESPONSE::TURN_LEFT;
         }
      }
      else if (female->hasObject == OBJECT::STONE)
      {
         female->response = Bird::RESPONSE::PUT_OBJECT;
      }
      else
      {
         female->response = Female::RESPONSE::WANT_STONE;
      }
      break;

   case 8:
         if (female->orientation == Bird::ORIENTATION::SOUTH)
         {
            female->response = Bird::RESPONSE::MOVE_FORWARD;
            FemaleNestSequence++;
         }
         else
         {
            female->response = Bird::RESPONSE::TURN_LEFT;
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


// Do bird response.
void doResponse(int gender)
{
   int  d;
   Bird *bird = (Bird *)male;

   if (gender == Bird::FEMALE)
   {
      bird = (Bird *)female;
   }
   Cell *cell    = &World[bird->x][bird->y];
   int  response = male->response;
   if (gender == Bird::FEMALE)
   {
      response = female->response;
   }

   if (response < Bird::RESPONSE::NUM_RESPONSES)
   {
      switch (response)
      {
      case Bird::RESPONSE::DO_NOTHING:
         break;

      case Bird::RESPONSE::EAT_MOUSE:
         if (bird->hasObject == OBJECT::MOUSE)
         {
            if (gender == Bird::MALE)
            {
               if (Male::RANDOMIZE_FOOD_LEVEL && (Male::FOOD_DURATION > 0))
               {
                  male->food = rand() % (Male::FOOD_DURATION + 1);
               }
               else
               {
                  male->food = Male::FOOD_DURATION;
               }
            }
            else
            {
               if (Female::RANDOMIZE_FOOD_LEVEL && (Female::FOOD_DURATION > 0))
               {
                  female->food = rand() % (Female::FOOD_DURATION + 1);
               }
               else
               {
                  female->food = Female::FOOD_DURATION;
               }
            }
            bird->hasObject = OBJECT::NO_OBJECT;
         }
         break;

      case Bird::RESPONSE::GET_OBJECT:
         if (bird->hasObject == OBJECT::NO_OBJECT)
         {
            bird->hasObject = cell->object;
            cell->object    = OBJECT::NO_OBJECT;
         }
         break;

      case Bird::RESPONSE::PUT_OBJECT:
         if (cell->object == OBJECT::NO_OBJECT)
         {
            cell->object    = bird->hasObject;
            bird->hasObject = OBJECT::NO_OBJECT;
         }
         break;

      case Bird::RESPONSE::TOSS_OBJECT:
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
             bird->hasObject = OBJECT::NO_OBJECT;
             break;
         }

         // Object lands randomly nearby.
         d = HEIGHT / 2;
         if (d > WIDTH / 2)
         {
            d = WIDTH / 2;
         }
         for (int j = 5; j < d; j++)
         {
            int x = (int)(rand() % (long)j);
            int y = (int)(rand() % (long)j);
            if ((x == 0) && (y == 0))
            {
               continue;
            }
            if ((rand() % 2) == 0)
            {
               x = -x;
            }
            if ((rand() % 2) == 0)
            {
               y = -y;
            }
            x += bird->x;
            if ((x < 0) || (x >= WIDTH))
            {
               continue;
            }
            y += bird->y;
            if ((y < 0) || (y >= HEIGHT))
            {
               continue;
            }
            if (World[x][y].object == OBJECT::NO_OBJECT)
            {
               World[x][y].object = bird->hasObject;
               bird->hasObject    = OBJECT::NO_OBJECT;
               break;
            }
         }
         if (bird->hasObject != OBJECT::NO_OBJECT)
         {
            // Vaporize object.
            bird->hasObject = OBJECT::NO_OBJECT;
         }
         break;

      case Bird::RESPONSE::MOVE_FORWARD:
         switch (bird->orientation)
         {
         case Bird::ORIENTATION::NORTH:
            if (bird->y > 0)
            {
               bird->y--;
            }
            break;

         case Bird::ORIENTATION::SOUTH:
            if (bird->y < HEIGHT - 1)
            {
               bird->y++;
            }
            break;


         case Bird::ORIENTATION::EAST:
            if (bird->x < WIDTH - 1)
            {
               bird->x++;
            }
            break;

         case Bird::ORIENTATION::WEST:
            if (bird->x > 0)
            {
               bird->x--;
            }
            break;
         }
         break;

      case Bird::RESPONSE::TURN_RIGHT:
         switch (bird->orientation)
         {
         case Bird::ORIENTATION::NORTH:
            bird->orientation = Bird::ORIENTATION::EAST;
            break;

         case Bird::ORIENTATION::SOUTH:
            bird->orientation = Bird::ORIENTATION::WEST;
            break;

         case Bird::ORIENTATION::EAST:
            bird->orientation = Bird::ORIENTATION::SOUTH;
            break;

         case Bird::ORIENTATION::WEST:
            bird->orientation = Bird::ORIENTATION::NORTH;
            break;
         }
         break;

      case Bird::RESPONSE::TURN_LEFT:
         switch (bird->orientation)
         {
         case Bird::ORIENTATION::NORTH:
            bird->orientation = Bird::ORIENTATION::WEST;
            break;

         case Bird::ORIENTATION::SOUTH:
            bird->orientation = Bird::ORIENTATION::EAST;
            break;

         case Bird::ORIENTATION::EAST:
            bird->orientation = Bird::ORIENTATION::NORTH;
            break;

         case Bird::ORIENTATION::WEST:
            bird->orientation = Bird::ORIENTATION::SOUTH;
            break;
         }
         break;

      case Bird::RESPONSE::TURN_AROUND:
          switch (bird->orientation)
          {
          case Bird::ORIENTATION::NORTH:
              bird->orientation = Bird::ORIENTATION::SOUTH;
              break;

          case Bird::ORIENTATION::SOUTH:
              bird->orientation = Bird::ORIENTATION::NORTH;
              break;

          case Bird::ORIENTATION::EAST:
              bird->orientation = Bird::ORIENTATION::WEST;
              break;

          case Bird::ORIENTATION::WEST:
              bird->orientation = Bird::ORIENTATION::EAST;
              break;
          }
          break;

      case Bird::RESPONSE::STATE_OFF:
         bird->state = 0;
         break;

      case Bird::RESPONSE::STATE_ON:
         bird->state = 1;
         break;
      }
   }
   else
   {
      // Gender-specific response.
      if (gender == Bird::MALE)
      {
         switch (male->response)
         {
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
      }
      else
      {
         // Female response.
         switch (female->response)
         {
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
      }
   }

   // Digest food.
   bird->digest();
}


// Set bird sensors.
void setSensors(int gender)
{
   Bird *bird;
   int  *sensors;

   if (gender == Bird::FEMALE)
   {
      bird    = female;
      sensors = female->sensors;
      for (int i = 0; i < Female::NUM_SENSORS; i++)
      {
          sensors[i] = DONT_CARE;
      }
   }
   else
   {
      bird    = male;
      sensors = male->sensors;
      for (int i = 0; i < Male::NUM_SENSORS; i++)
      {
          sensors[i] = DONT_CARE;
      }
   }

   // Set locale, object, and mate proximity sensors.
   if (gender == Bird::MALE)
   {
       sensors[Male::MATE_PROXIMITY_SENSOR] = Male::MATE_PROXIMITY_UNKNOWN;
   }
   int x = -1;
   int y = -1;
   for (int i = 0; i < Bird::NUM_CELL_SENSORS; i++)
   {
      switch (i)
      {
      // Current.
      case 0:
         sensors[Bird::CURRENT_LOCALE_SENSOR] = World[bird->x][bird->y].locale;
         sensors[Bird::CURRENT_OBJECT_SENSOR] = World[bird->x][bird->y].object;
         if (gender == Bird::MALE)
         {
            if ((female->x == male->x) && (female->y == male->y))
            {
                sensors[Male::MATE_PROXIMITY_SENSOR] = Male::MATE_PROXIMITY_PRESENT;
            }
         }
         break;

      // Left.
      case 1:
         x = -1;
         y = -1;
         switch (bird->orientation)
         {
         case Bird::ORIENTATION::NORTH:
            if (bird->x > 0)
            {
               sensors[Bird::LEFT_LOCALE_SENSOR] = World[bird->x - 1][bird->y].locale;
               sensors[Bird::LEFT_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
               x = bird->x - 1;
               y = bird->y;
            }
            break;

         case Bird::ORIENTATION::SOUTH:
            if (bird->x < WIDTH - 1)
            {
               sensors[Bird::LEFT_LOCALE_SENSOR] = World[bird->x + 1][bird->y].locale;
               sensors[Bird::LEFT_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
               x = bird->x + 1;
               y = bird->y;
            }
            break;

         case Bird::ORIENTATION::EAST:
            if (bird->y > 0)
            {
               sensors[Bird::LEFT_LOCALE_SENSOR] = World[bird->x][bird->y - 1].locale;
               sensors[Bird::LEFT_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
               x = bird->x;
               y = bird->y - 1;
            }
            break;

         case Bird::ORIENTATION::WEST:
            if (bird->y < HEIGHT - 1)
            {
               sensors[Bird::LEFT_LOCALE_SENSOR] = World[bird->x][bird->y + 1].locale;
               sensors[Bird::LEFT_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
               x = bird->x;
               y = bird->y + 1;
            }
            break;
         }
         if (gender == Bird::MALE)
         {
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
            {
               if ((female->x == x) && (female->y == y))
               {
                  sensors[Male::MATE_PROXIMITY_SENSOR] = Male::MATE_PROXIMITY_LEFT;
               }
            }
         }
         break;

      // Left front.
      case 2:
          x = -1;
          y = -1;
          switch (bird->orientation)
          {
          case Bird::ORIENTATION::NORTH:
              if (bird->x > 0 && bird->y > 0)
              {
                  sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] = World[bird->x - 1][bird->y - 1].locale;
                  sensors[Bird::LEFT_FRONT_OBJECT_SENSOR] = World[bird->x - 1][bird->y - 1].object;
                  x = bird->x - 1;
                  y = bird->y - 1;
              }
              break;

          case Bird::ORIENTATION::SOUTH:
              if (bird->x < WIDTH - 1 && bird->y < HEIGHT - 1)
              {
                  sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] = World[bird->x + 1][bird->y + 1].locale;
                  sensors[Bird::LEFT_FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y + 1].object;
                  x = bird->x + 1;
                  y = bird->y + 1;
              }
              break;

          case Bird::ORIENTATION::EAST:
              if (bird->x < WIDTH - 1 && bird->y > 0)
              {
                  sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] = World[bird->x + 1][bird->y - 1].locale;
                  sensors[Bird::LEFT_FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y - 1].object;
                  x = bird->x + 1;
                  y = bird->y - 1;
              }
              break;

          case Bird::ORIENTATION::WEST:
              if (bird->x > 0 && bird->y < HEIGHT - 1)
              {
                  sensors[Bird::LEFT_FRONT_LOCALE_SENSOR] = World[bird->x - 1][bird->y + 1].locale;
                  sensors[Bird::LEFT_FRONT_OBJECT_SENSOR] = World[bird->x - 1][bird->y + 1].object;
                  x = bird->x - 1;
                  y = bird->y + 1;
              }
              break;
          }
          if (gender == Bird::MALE)
          {
              if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
              {
                  if ((female->x == x) && (female->y == y))
                  {
                      sensors[Male::MATE_PROXIMITY_SENSOR] = Male::MATE_PROXIMITY_LEFT_FRONT;
                  }
              }
          }
          break;

      // Front.
      case 3:
         x = -1;
         y = -1;
         switch (bird->orientation)
         {
         case Bird::ORIENTATION::NORTH:
            if (bird->y > 0)
            {
               sensors[Bird::FRONT_LOCALE_SENSOR] = World[bird->x][bird->y - 1].locale;
               sensors[Bird::FRONT_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
               x = bird->x;
               y = bird->y - 1;
            }
            break;

         case Bird::ORIENTATION::SOUTH:
            if (bird->y < HEIGHT - 1)
            {
               sensors[Bird::FRONT_LOCALE_SENSOR] = World[bird->x][bird->y + 1].locale;
               sensors[Bird::FRONT_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
               x = bird->x;
               y = bird->y + 1;
            }
            break;

         case Bird::ORIENTATION::EAST:
            if (bird->x < WIDTH - 1)
            {
               sensors[Bird::FRONT_LOCALE_SENSOR] = World[bird->x + 1][bird->y].locale;
               sensors[Bird::FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
               x = bird->x + 1;
               y = bird->y;
            }
            break;

         case Bird::ORIENTATION::WEST:
            if (bird->x > 0)
            {
               sensors[Bird::FRONT_LOCALE_SENSOR] = World[bird->x - 1][bird->y].locale;
               sensors[Bird::FRONT_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
               x = bird->x - 1;
               y = bird->y;
            }
            break;
         }
         if (gender == Bird::MALE)
         {
             if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
             {
                 if ((female->x == x) && (female->y == y))
                 {
                     sensors[Male::MATE_PROXIMITY_SENSOR] = Male::MATE_PROXIMITY_FRONT;
                 }
             }
         }
         break;

      // Right front.
      case 4:
          x = -1;
          y = -1;
          switch (bird->orientation)
          {
          case Bird::ORIENTATION::NORTH:
              if (bird->x < WIDTH - 1 && bird->y > 0)
              {
                  sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] = World[bird->x + 1][bird->y - 1].locale;
                  sensors[Bird::RIGHT_FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y - 1].object;
                  x = bird->x + 1;
                  y = bird->y - 1;
              }
              break;

          case Bird::ORIENTATION::SOUTH:
              if (bird->x > 0 && bird->y < HEIGHT - 1)
              {
                  sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] = World[bird->x - 1][bird->y + 1].locale;
                  sensors[Bird::RIGHT_FRONT_OBJECT_SENSOR] = World[bird->x - 1][bird->y + 1].object;
                  x = bird->x - 1;
                  y = bird->y + 1;
              }
              break;

          case Bird::ORIENTATION::EAST:
              if (bird->x < WIDTH - 1 && bird->y < HEIGHT - 1)
              {
                  sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] = World[bird->x + 1][bird->y + 1].locale;
                  sensors[Bird::RIGHT_FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y + 1].object;
                  x = bird->x + 1;
                  y = bird->y + 1;
              }
              break;

          case Bird::ORIENTATION::WEST:
              if (bird->x > 0 && bird->y > 0)
              {
                  sensors[Bird::RIGHT_FRONT_LOCALE_SENSOR] = World[bird->x - 1][bird->y - 1].locale;
                  sensors[Bird::RIGHT_FRONT_OBJECT_SENSOR] = World[bird->x - 1][bird->y - 1].object;
                  x = bird->x - 1;
                  y = bird->y - 1;
              }
              break;
          }
          if (gender == Bird::MALE)
          {
              if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
              {
                  if ((female->x == x) && (female->y == y))
                  {
                      sensors[Male::MATE_PROXIMITY_SENSOR] = Male::MATE_PROXIMITY_RIGHT_FRONT;
                  }
              }
          }
          break;

      // Right.
      case 5:
         x = -1;
         y = -1;
         switch (bird->orientation)
         {
         case Bird::ORIENTATION::NORTH:
            if (bird->x < WIDTH - 1)
            {
               sensors[Bird::RIGHT_LOCALE_SENSOR] = World[bird->x + 1][bird->y].locale;
               sensors[Bird::RIGHT_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
               x = bird->x + 1;
               y = bird->y;
            }
            break;

         case Bird::ORIENTATION::SOUTH:
            if (bird->x > 0)
            {
               sensors[Bird::RIGHT_LOCALE_SENSOR] = World[bird->x - 1][bird->y].locale;
               sensors[Bird::RIGHT_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
               x = bird->x - 1;
               y = bird->y;
            }
            break;

         case Bird::ORIENTATION::EAST:
            if (bird->y < HEIGHT - 1)
            {
               sensors[Bird::RIGHT_LOCALE_SENSOR] = World[bird->x][bird->y + 1].locale;
               sensors[Bird::RIGHT_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
               x = bird->x;
               y = bird->y + 1;
            }
            break;

         case Bird::ORIENTATION::WEST:
            if (bird->y > 0)
            {
               sensors[Bird::RIGHT_LOCALE_SENSOR] = World[bird->x][bird->y - 1].locale;
               sensors[Bird::RIGHT_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
               x = bird->x;
               y = bird->y - 1;
            }
            break;
         }
         if (gender == Bird::MALE)
         {
             if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
             {
                 if ((female->x == x) && (female->y == y))
                 {
                     sensors[Male::MATE_PROXIMITY_SENSOR] = Male::MATE_PROXIMITY_RIGHT;
                 }
             }
         }
         break;

      // Right rear.
      case 6:
          x = -1;
          y = -1;
          switch (bird->orientation)
          {
          case Bird::ORIENTATION::NORTH:
              if (bird->x < WIDTH - 1 && bird->y < HEIGHT - 1)
              {
                  sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] = World[bird->x + 1][bird->y + 1].locale;
                  sensors[Bird::RIGHT_REAR_OBJECT_SENSOR] = World[bird->x + 1][bird->y + 1].object;
                  x = bird->x + 1;
                  y = bird->y + 1;
              }
              break;

          case Bird::ORIENTATION::SOUTH:
              if (bird->x > 0 && bird->y > 0)
              {
                  sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] = World[bird->x - 1][bird->y - 1].locale;
                  sensors[Bird::RIGHT_REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y - 1].object;
                  x = bird->x - 1;
                  y = bird->y - 1;
              }
              break;

          case Bird::ORIENTATION::EAST:
              if (bird->x > 0 && bird->y < HEIGHT - 1)
              {
                  sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] = World[bird->x - 1][bird->y + 1].locale;
                  sensors[Bird::RIGHT_REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y + 1].object;
                  x = bird->x - 1;
                  y = bird->y + 1;
              }
              break;

          case Bird::ORIENTATION::WEST:
              if (bird->x < WIDTH - 1 && bird->y > 0)
              {
                  sensors[Bird::RIGHT_REAR_LOCALE_SENSOR] = World[bird->x + 1][bird->y - 1].locale;
                  sensors[Bird::RIGHT_REAR_OBJECT_SENSOR] = World[bird->x + 1][bird->y - 1].object;
                  x = bird->x + 1;
                  y = bird->y - 1;
              }
              break;
          }
          if (gender == Bird::MALE)
          {
              if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
              {
                  if ((female->x == x) && (female->y == y))
                  {
                      sensors[Male::MATE_PROXIMITY_SENSOR] = Male::MATE_PROXIMITY_RIGHT_REAR;
                  }
              }
          }
          break;

      // Rear.
      case 7:
          x = -1;
          y = -1;
          switch (bird->orientation)
          {
          case Bird::ORIENTATION::NORTH:
              if (bird->y < HEIGHT - 1)
              {
                  sensors[Bird::REAR_LOCALE_SENSOR] = World[bird->x][bird->y + 1].locale;
                  sensors[Bird::REAR_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
                  x = bird->x;
                  y = bird->y + 1;
              }
              break;

          case Bird::ORIENTATION::SOUTH:
              if (bird->y > 0)
              {
                  sensors[Bird::REAR_LOCALE_SENSOR] = World[bird->x][bird->y - 1].locale;
                  sensors[Bird::REAR_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
                  x = bird->x;
                  y = bird->y - 1;
              }
              break;

          case Bird::ORIENTATION::EAST:
              if (bird->x > 0)
              {
                  sensors[Bird::REAR_LOCALE_SENSOR] = World[bird->x - 1][bird->y].locale;
                  sensors[Bird::REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
                  x = bird->x - 1;
                  y = bird->y;
              }
              break;

          case Bird::ORIENTATION::WEST:
              if (bird->x < WIDTH - 1)
              {
                  sensors[Bird::REAR_LOCALE_SENSOR] = World[bird->x + 1][bird->y].locale;
                  sensors[Bird::REAR_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
                  x = bird->x + 1;
                  y = bird->y;
              }
              break;
          }
          if (gender == Bird::MALE)
          {
              if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
              {
                  if ((female->x == x) && (female->y == y))
                  {
                      sensors[Male::MATE_PROXIMITY_SENSOR] = Male::MATE_PROXIMITY_REAR;
                  }
              }
          }
          break;

      // Left rear.
      case 8:
          x = -1;
          y = -1;
          switch (bird->orientation)
          {
          case Bird::ORIENTATION::NORTH:
              if (bird->x > 0 && bird->y < HEIGHT - 1)
              {
                  sensors[Bird::LEFT_REAR_LOCALE_SENSOR] = World[bird->x - 1][bird->y + 1].locale;
                  sensors[Bird::LEFT_REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y + 1].object;
                  x = bird->x - 1;
                  y = bird->y + 1;
              }
              break;

          case Bird::ORIENTATION::SOUTH:
              if (bird->x < WIDTH - 1 && bird->y > 0)
              {
                  sensors[Bird::LEFT_REAR_LOCALE_SENSOR] = World[bird->x + 1][bird->y - 1].locale;
                  sensors[Bird::LEFT_REAR_OBJECT_SENSOR] = World[bird->x + 1][bird->y - 1].object;
                  x = bird->x + 1;
                  y = bird->y - 1;
              }
              break;

          case Bird::ORIENTATION::EAST:
              if (bird->x > 0 && bird->y > 0)
              {
                  sensors[Bird::LEFT_REAR_LOCALE_SENSOR] = World[bird->x - 1][bird->y - 1].locale;
                  sensors[Bird::LEFT_REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y - 1].object;
                  x = bird->x - 1;
                  y = bird->y - 1;
              }
              break;

          case Bird::ORIENTATION::WEST:
              if (bird->x < WIDTH - 1 && bird->y < HEIGHT - 1)
              {
                  sensors[Bird::LEFT_REAR_LOCALE_SENSOR] = World[bird->x + 1][bird->y + 1].locale;
                  sensors[Bird::LEFT_REAR_OBJECT_SENSOR] = World[bird->x + 1][bird->y + 1].object;
                  x = bird->x + 1;
                  y = bird->y + 1;
              }
              break;
          }
          if (gender == Bird::MALE)
          {
              if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
              {
                  if ((female->x == x) && (female->y == y))
                  {
                      sensors[Male::MATE_PROXIMITY_SENSOR] = Male::MATE_PROXIMITY_LEFT_REAR;
                  }
              }
          }
          break;
      }
   }

   // Internal state.
   sensors[Bird::ORIENTATION_SENSOR] = bird->orientation;
   if (bird->food > 0)
   {
       sensors[Bird::HUNGER_SENSOR] = 0;
   } else {
       sensors[Bird::HUNGER_SENSOR] = 1;
   }
   sensors[Bird::HAS_OBJECT_SENSOR] = bird->hasObject;
   sensors[Bird::STATE_SENSOR] = bird->state;

   // Male senses female want?
   if (gender == Bird::MALE)
   {
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
}


// Euclidean distance.
static double distance(double x1, double y1, double x2, double y2)
{
   double xd = x1 - x2;
   double yd = y1 - y2;

   return(sqrt((xd * xd) + (yd * yd)));
}

// Move mice in forest.
void stepMice()
{
   for (int x = (rand() % WIDTH), x2 = 0; x2 < WIDTH; x = (x + 1) % WIDTH, x2++)
   {
      for (int y = (rand() % HEIGHT), y2 = 0; y2 < HEIGHT; y = (y + 1) % HEIGHT, y2++)
      {
         if ((World[x][y].object == OBJECT::MOUSE) &&
             ((male->x != x) || (male->y != y) || (male->response != Bird::RESPONSE::GET_OBJECT)) &&
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
      if (strcmp(args[i], "-maleStoneNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleStoneNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::STONE_NEED = strtod(args[i], 0);
         if (Male::STONE_NEED < 0.0)
         {
            fprintf(stderr, "Invalid maleStoneNeed option\n");
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
