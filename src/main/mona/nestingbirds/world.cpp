// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds world.

#include "world.hpp"
#include <stdlib.h>

// Random numbers.
int RANDOM_NUMBER_SEED = 4517;

// Probability of mouse movement.
double MOUSE_MOVE_PROBABILITY = 0.1;

// Bird gender.
const int MALE   = 0;
const int FEMALE = 1;

// Testing vs. training.
bool MaleTest;
bool FemaleTest;

// Verbosity.
bool Verbose = true;

// Controls.
bool MaleFlying;
bool MaleFlyingToForest;
bool MaleFlyingToDesert;
bool MaleFlyingToNest;
bool MaleWantsMouse;
bool MaleWantsFemale;
bool FemaleWantsMouse;
bool FemaleWantsStone;
int  FemaleNestSequence;

// Behavior file.
FILE *BehaviorFp;

// World map: plain=0, forest=1, mouse=2, desert=3, stone=4
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
int NEST_CENTER_X   = WIDTH / 2;
int NEST_CENTER_Y   = HEIGHT / 2;
int NEST_MIN_X      = NEST_CENTER_X - 1;
int NEST_MAX_X      = NEST_CENTER_X + 1;
int NEST_MIN_Y      = NEST_CENTER_Y - 1;
int NEST_MAX_Y      = NEST_CENTER_Y + 1;
int FOREST_CENTER_X = -1;
int FOREST_CENTER_Y = -1;
int FOREST_MIN_X    = -1;
int FOREST_MAX_X    = -1;
int FOREST_MIN_Y    = -1;
int FOREST_MAX_Y    = -1;
int DESERT_CENTER_X = -1;
int DESERT_CENTER_Y = -1;
int DESERT_MIN_X    = -1;
int DESERT_MAX_X    = -1;
int DESERT_MIN_Y    = -1;
int DESERT_MAX_Y    = -1;

// World.
Cell World[WIDTH][HEIGHT];

// Birds.
Male   *male;
Female *female;

// Prototypes.
void setSensors(int);
void doResponse(int);
void stepMice();
void train(int);
void trainMale();
void trainFemale();
bool doWants();
bool flyToPlace();
bool getMouse();
bool getStone();
bool goToFemale();
void doMaleResponse();
void doFemaleResponse();
void setMaleSensors();
void setFemaleSensors();
int distance(int x1, int y1, int x2, int y2);

// Initialize.
void init(bool maleTest, bool femaleTest)
{
   MaleTest   = maleTest;
   FemaleTest = femaleTest;

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
            if ((FOREST_MIN_X == -1) || (x < FOREST_MIN_X))
            {
               FOREST_MIN_X = x;
            }
            if ((FOREST_MAX_X == -1) || (x > FOREST_MAX_X))
            {
               FOREST_MAX_X = x;
            }
            if ((FOREST_MIN_Y == -1) || (y < FOREST_MIN_Y))
            {
               FOREST_MIN_Y = y;
            }
            if ((FOREST_MAX_Y == -1) || (y > FOREST_MAX_Y))
            {
               FOREST_MAX_Y = y;
            }
            break;

         case 2:
            World[x][y].locale = LOCALE::FOREST;
            World[x][y].object = OBJECT::MOUSE;
            if ((FOREST_MIN_X == -1) || (x < FOREST_MIN_X))
            {
               FOREST_MIN_X = x;
            }
            if ((FOREST_MAX_X == -1) || (x > FOREST_MAX_X))
            {
               FOREST_MAX_X = x;
            }
            if ((FOREST_MIN_Y == -1) || (y < FOREST_MIN_Y))
            {
               FOREST_MIN_Y = y;
            }
            if ((FOREST_MAX_Y == -1) || (y > FOREST_MAX_Y))
            {
               FOREST_MAX_Y = y;
            }
            break;

         case 3:
            World[x][y].locale = LOCALE::DESERT;
            World[x][y].object = OBJECT::NO_OBJECT;
            if ((DESERT_MIN_X == -1) || (x < DESERT_MIN_X))
            {
               DESERT_MIN_X = x;
            }
            if ((DESERT_MAX_X == -1) || (x > DESERT_MAX_X))
            {
               DESERT_MAX_X = x;
            }
            if ((DESERT_MIN_Y == -1) || (y < DESERT_MIN_Y))
            {
               DESERT_MIN_Y = y;
            }
            if ((DESERT_MAX_Y == -1) || (y > DESERT_MAX_Y))
            {
               DESERT_MAX_Y = y;
            }
            break;

         case 4:
            World[x][y].locale = LOCALE::DESERT;
            World[x][y].object = OBJECT::STONE;
            if ((DESERT_MIN_X == -1) || (x < DESERT_MIN_X))
            {
               DESERT_MIN_X = x;
            }
            if ((DESERT_MAX_X == -1) || (x > DESERT_MAX_X))
            {
               DESERT_MAX_X = x;
            }
            if ((DESERT_MIN_Y == -1) || (y < DESERT_MIN_Y))
            {
               DESERT_MIN_Y = y;
            }
            if ((DESERT_MAX_Y == -1) || (y > DESERT_MAX_Y))
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
   female          = new Female();
   female->Verbose = Verbose;
   female->x       = WIDTH / 2;
   female->y       = (HEIGHT / 2) - 1;
   female->food    = Female::INITIAL_FOOD;
   if (female->food == 0)
   {
      female->goal = Female::GOAL::EAT_MOUSE;
      female->brain->setNeed(Female::MOUSE_NEED_INDEX, Female::MOUSE_NEED);
   }
   female->response = Female::RESPONSE::DO_NOTHING;
   male             = new Male();
   male->Verbose    = Verbose;
   male->x          = WIDTH / 2;
   male->y          = (HEIGHT / 2) - 1;
   male->food       = Male::INITIAL_FOOD;
   if (male->food == 0)
   {
      male->goal = Male::GOAL::EAT_MOUSE;
      male->brain->setNeed(Male::MOUSE_NEED_INDEX, Male::MOUSE_NEED);
   }
   male->response     = Male::RESPONSE::DO_NOTHING;
   MaleFlying         = false;
   MaleFlyingToForest = false;
   MaleFlyingToDesert = false;
   MaleFlyingToNest   = false;
   MaleWantsMouse     = false;
   MaleWantsFemale    = false;
   FemaleWantsMouse   = false;
   FemaleWantsStone   = false;
   FemaleNestSequence = 0;
   setSensors(FEMALE);
   setSensors(MALE);
}


// Step.
void step()
{
   // Step mice.
   stepMice();

   // Pre-cycle female.
   preCycleFemale();

   // Cycle female
   female->cycle();

   // Post-cycle female.
   postCycleFemale();

   // Pre-cycle male.
   preCycleMale();

   // Cycle male.
   male->cycle();

   // Post-cycle male.
   postCycleMale();
}

// Pre-cycle female.
void preCycleFemale()
{
    // Set female sensors.
    setSensors(FEMALE);

    // Set female needs.
    female->setNeeds();

    // Train?
    if (!FemaleTest)
    {
        train(FEMALE);
    }

    if (Verbose)
    {
        printf("Female: Location: [%d,%d], ", female->x, female->y);
        printf("{ ");
        female->printState();
    }

    if (BehaviorFp != NULL)
    {
        fprintf(BehaviorFp, "\"Female\": { \"Location\": { \"x\": %d, \"y\": %d }, ", female->x, female->y);
        female->printState(BehaviorFp);
    }
}

// Post-cycle female.
void postCycleFemale()
{
    if (BehaviorFp != NULL)
    {
        fprintf(BehaviorFp, ", ");
        female->printResponse(BehaviorFp);
        fprintf(BehaviorFp, " },\n");
    }

    if (Verbose)
    {
        printf(", ");
        female->printResponse();
        printf(" }\n");
    }

    // Do response in world.
    doResponse(FEMALE);
}

// Pre-cycle male.
void preCycleMale()
{
    // Set male sensors.
    setSensors(MALE);

    // Set male needs.
    male->setNeeds();

    if (Verbose)
    {
        printf("Male: Location: [%d,%d], Orientation: %s, ", male->x, male->y, ORIENTATION::toString(male->orientation));
        printf("{ ");
        male->printState();
    }

    if (BehaviorFp != NULL)
    {
        fprintf(BehaviorFp, "\"Male\": { \"Location\": { \"x\": %d, \"y\": %d }, ", male->x, male->y);
        male->printState(BehaviorFp);
    }
}

// Post-cycle male.
void postCycleMale()
{
    // Train?
    if (!MaleTest)
    {
        train(MALE);
    }

    if (BehaviorFp != NULL)
    {
        fprintf(BehaviorFp, ", ");
        male->printResponse(BehaviorFp);
        fprintf(BehaviorFp, " }\n");
    }

    if (Verbose)
    {
        printf(", ");
        male->printResponse();
        printf(" }\n");
    }

    // Do response in world.
    doResponse(MALE);
}

// Train.
void train(int gender)
{
   if (gender == MALE)
   {
      trainMale();
      male->resetResponse();
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
   // Set wants from male needs.
   MaleWantsMouse   = false;
   MaleWantsFemale  = false;
   FemaleWantsMouse = false;
   FemaleWantsStone = false;
   if (male->brain->getNeed(Male::MOUSE_NEED_INDEX) > 0.0)
   {
      MaleWantsMouse = true;
   }
   else if (male->brain->getNeed(Male::FEMALE_MOUSE_NEED_INDEX) > 0.0)
   {
      FemaleWantsMouse = true;
   }
   else if (male->brain->getNeed(Male::FEMALE_STONE_NEED_INDEX) > 0.0)
   {
      FemaleWantsStone = true;
   }
   else
   {
      MaleWantsFemale = true;
   }

   // Get training response.
   if (flyToPlace())
   {
      if (!doWants())
      {
         if (MaleWantsMouse)
         {
            if (getMouse())
            {
               goToFemale();
            }
         }
         else if (FemaleWantsMouse)
         {
            if (getMouse())
            {
               goToFemale();
            }
         }
         else if (FemaleWantsStone)
         {
            if (getStone())
            {
               goToFemale();
            }
         }
         else
         {
            goToFemale();
         }
      }
   }
}


// Fly to a place.
// Return true when flight completed.
bool flyToPlace()
{
   if (MaleFlyingToForest)
   {
      male->response = Mona::Motor::gotoPlace(male->orientation, male->x, male->y, FOREST_CENTER_X, FOREST_CENTER_Y);
      if (male->response == Male::RESPONSE::DO_NOTHING)
      {
         MaleFlyingToForest = false;
         male->response     = Male::RESPONSE::ALIGHT;
      }
      return(false);
   }
   if (MaleFlyingToDesert)
   {
      male->response = Mona::Motor::gotoPlace(male->orientation, male->x, male->y, DESERT_CENTER_X, DESERT_CENTER_Y);
      if (male->response == Male::RESPONSE::DO_NOTHING)
      {
         MaleFlyingToDesert = false;
         male->response     = Male::RESPONSE::ALIGHT;
      }
      return(false);
   }
   if (MaleFlyingToNest)
   {
      male->response = Mona::Motor::gotoPlace(male->orientation, male->x, male->y, NEST_CENTER_X, NEST_CENTER_Y);
      if (male->response == Male::RESPONSE::DO_NOTHING)
      {
         MaleFlyingToNest = false;
         male->response   = Male::RESPONSE::ALIGHT;
      }
      return(false);
   }
   return(true);
}


// Do want actions.
// Return true when response taken.
bool doWants()
{
   if (MaleWantsMouse && (male->hasObject == OBJECT::MOUSE))
   {
      male->response = Male::RESPONSE::EAT_MOUSE;
      return(true);
   }
   if (male->sensors[Male::FEMALE_PROXIMITY_SENSOR] == Male::PROXIMITY::PRESENT)
   {
      if (FemaleWantsMouse && (male->hasObject == OBJECT::MOUSE))
      {
         male->response = Male::RESPONSE::GIVE_MOUSE;
         return(true);
      }
      if (FemaleWantsStone && (male->hasObject == OBJECT::STONE))
      {
         male->response = Male::RESPONSE::GIVE_STONE;
         return(true);
      }
   }
   return(false);
}


// Get mouse.
// Return true when have mouse.
bool getMouse()
{
   if (male->hasObject == OBJECT::MOUSE)
   {
      return(true);
   }
   switch (male->sensors[Male::MOUSE_PROXIMITY_SENSOR])
   {
   case Male::PROXIMITY::PRESENT:
      male->response = Male::RESPONSE::GET_OBJECT;
      break;

   case Male::PROXIMITY::FRONT:
      male->response = Male::RESPONSE::MOVE_FORWARD;
      break;

   case Male::PROXIMITY::LEFT:
      male->response = Male::RESPONSE::TURN_LEFT;
      break;

   case Male::PROXIMITY::RIGHT:
      male->response = Male::RESPONSE::TURN_RIGHT;
      break;

   case Male::PROXIMITY::UNKNOWN:
      MaleFlyingToForest = true;
      male->response     = Male::RESPONSE::FLY;
      break;
   }
   return(false);
}


// Get stone.
// Return true when have stone.
bool getStone()
{
   if (male->hasObject == OBJECT::STONE)
   {
      return(true);
   }
   switch (male->sensors[Male::STONE_PROXIMITY_SENSOR])
   {
   case Male::PROXIMITY::PRESENT:
      male->response = Male::RESPONSE::GET_OBJECT;
      break;

   case Male::PROXIMITY::FRONT:
      male->response = Male::RESPONSE::MOVE_FORWARD;
      break;

   case Male::PROXIMITY::LEFT:
      male->response = Male::RESPONSE::TURN_LEFT;
      break;

   case Male::PROXIMITY::RIGHT:
      male->response = Male::RESPONSE::TURN_RIGHT;
      break;

   case Male::PROXIMITY::UNKNOWN:
      MaleFlyingToDesert = true;
      male->response     = Male::RESPONSE::FLY;
      break;
   }
   return(false);
}


// Male goes to female.
// Return true when female present.
bool goToFemale()
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

   case Male::PROXIMITY::UNKNOWN:
      MaleFlyingToNest = true;
      male->response   = Male::RESPONSE::FLY;
      break;
   }
   return(false);
}


// Train female.
void trainFemale()
{
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
   if (female->brain->getNeed(Female::MOUSE_NEED_INDEX) > 0.0)
   {
      female->response = Female::RESPONSE::WANT_MOUSE;
      return;
   }

   // Brooding egg?
   if (World[female->x][female->y].object == OBJECT::EGG)
   {
      female->response = Female::RESPONSE::DO_NOTHING;
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
   else
   {
      setFemaleSensors();
   }
}


// Set male sensors.
void setMaleSensors()
{
   Male *bird    = male;
   int  *sensors = male->sensors;

   for (int i = 0; i < Male::NUM_SENSORS; i++)
   {
      sensors[i] = DONT_CARE;
   }

   // Set locale sensor.
   sensors[Male::LOCALE_SENSOR] = World[bird->x][bird->y].locale;

   // Set mouse, stone, and female proximity sensors.
   int mousex     = -1;
   int mousey     = -1;
   int mouseDist  = -1;
   int stonex     = -1;
   int stoney     = -1;
   int stoneDist  = -1;
   int femaleDist = -1;
   for (int x = 0; x < WIDTH; x++)
   {
      for (int y = 0; y < HEIGHT; y++)
      {
         if ((bird->x >= FOREST_MIN_X) && (bird->x <= FOREST_MAX_X) &&
             (bird->y >= FOREST_MIN_Y) && (bird->y <= FOREST_MAX_Y))
         {
            if (World[x][y].object == OBJECT::MOUSE)
            {
               int d = distance(x, y, bird->x, bird->y);
               if ((mouseDist < 0) || (d < mouseDist))
               {
                  mouseDist = d;
                  mousex    = x;
                  mousey    = y;
               }
            }
         }
         if ((bird->x >= DESERT_MIN_X) && (bird->x <= DESERT_MAX_X) &&
             (bird->y >= DESERT_MIN_Y) && (bird->y <= DESERT_MAX_Y))
         {
            if ((World[x][y].locale == LOCALE::DESERT) &&
                (World[x][y].object == OBJECT::STONE))
            {
               int d = distance(x, y, bird->x, bird->y);
               if ((stoneDist < 0) || (d < stoneDist))
               {
                  stoneDist = d;
                  stonex    = x;
                  stoney    = y;
               }
            }
         }
         if ((bird->x >= NEST_MIN_X) && (bird->x <= NEST_MAX_X) &&
             (bird->y >= NEST_MIN_Y) && (bird->y <= NEST_MAX_Y))
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
         else
         {
            if (mousey < bird->y)
            {
               sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
            }
            else if (mousey > bird->y)
            {
               sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else
            {
               sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
            }
         }
      }
      else
      {
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
         else
         {
            if (stoney < bird->y)
            {
               sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
            }
            else if (stoney > bird->y)
            {
               sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else
            {
               sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
            }
         }
      }
      else
      {
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
         else
         {
            if (female->y < bird->y)
            {
               sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
            }
            else if (female->y > bird->y)
            {
               sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else
            {
               sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
            }
         }
      }
      else
      {
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
         else
         {
            if (mousey < bird->y)
            {
               sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else if (mousey > bird->y)
            {
               sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
            }
            else
            {
               sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
            }
         }
      }
      else
      {
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
         else
         {
            if (stoney < bird->y)
            {
               sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else if (stoney > bird->y)
            {
               sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
            }
            else
            {
               sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
            }
         }
      }
      else
      {
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
         else
         {
            if (female->y < bird->y)
            {
               sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else if (female->y > bird->y)
            {
               sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
            }
            else
            {
               sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
            }
         }
      }
      else
      {
         sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
      }
      break;

   case ORIENTATION::EAST:
      if (mouseDist >= 0)
      {
         if (mousex < bird->x)
         {
            sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
         }
         else if (mousex > bird->x)
         {
            sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
         }
         else
         {
            if (mousey < bird->y)
            {
               sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
            }
            else if (mousey > bird->y)
            {
               sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else
            {
               sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
            }
         }
      }
      else
      {
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
            sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
         }
         else
         {
            if (stoney < bird->y)
            {
               sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
            }
            else if (stoney > bird->y)
            {
               sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else
            {
               sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
            }
         }
      }
      else
      {
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
            sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
         }
         else
         {
            if (female->y < bird->y)
            {
               sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
            }
            else if (female->y > bird->y)
            {
               sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else
            {
               sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
            }
         }
      }
      else
      {
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
            sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
         }
         else
         {
            if (mousey < bird->y)
            {
               sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else if (mousey > bird->y)
            {
               sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
            }
            else
            {
               sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
            }
         }
      }
      else
      {
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
            sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
         }
         else
         {
            if (stoney < bird->y)
            {
               sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else if (stoney > bird->y)
            {
               sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
            }
            else
            {
               sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
            }
         }
      }
      else
      {
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
            sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
         }
         else
         {
            if (female->y < bird->y)
            {
               sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
            }
            else if (female->y > bird->y)
            {
               sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
            }
            else
            {
               sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
            }
         }
      }
      else
      {
         sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
      }
      break;
   }

   // Goal.
   sensors[Male::GOAL_SENSOR] = bird->goal;

   // Has object.
   sensors[Male::HAS_OBJECT_SENSOR] = bird->hasObject;

   // If have mouse/stone, proximity is not needed.
   if (bird->hasObject == OBJECT::MOUSE)
   {
      sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
   }
   if (bird->hasObject == OBJECT::STONE)
   {
      sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
   }

   // Flying?
   if (bird->flying)
   {
      sensors[Male::FLYING_SENSOR] = 1;
   }
   else
   {
      sensors[Male::FLYING_SENSOR] = 0;
   }

   // Sense female wants.
   sensors[Male::FEMALE_WANTS_MOUSE_SENSOR] = 0;
   sensors[Male::FEMALE_WANTS_STONE_SENSOR] = 0;
   if ((bird->x == female->x) && (bird->y == female->y))
   {
      switch (female->response)
      {
      case Female::RESPONSE::WANT_MOUSE:
         sensors[Male::FEMALE_WANTS_MOUSE_SENSOR] = 1;
         break;

      case Female::RESPONSE::WANT_STONE:
         sensors[Male::FEMALE_WANTS_STONE_SENSOR] = 1;
         break;
      }
   }
}


// Set female sensors.
void setFemaleSensors()
{
   Female *bird    = female;
   int    *sensors = female->sensors;

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
               sensors[Female::LEFT_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
               x = bird->x - 1;
               y = bird->y;
            }
            break;

         case ORIENTATION::SOUTH:
            if (bird->x < WIDTH - 1)
            {
               sensors[Female::LEFT_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
               x = bird->x + 1;
               y = bird->y;
            }
            break;

         case ORIENTATION::EAST:
            if (bird->y > 0)
            {
               sensors[Female::LEFT_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
               x = bird->x;
               y = bird->y - 1;
            }
            break;

         case ORIENTATION::WEST:
            if (bird->y < HEIGHT - 1)
            {
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
            if ((bird->x > 0) && (bird->y > 0))
            {
               sensors[Female::LEFT_FRONT_OBJECT_SENSOR] = World[bird->x - 1][bird->y - 1].object;
               x = bird->x - 1;
               y = bird->y - 1;
            }
            break;

         case ORIENTATION::SOUTH:
            if ((bird->x < WIDTH - 1) && (bird->y < HEIGHT - 1))
            {
               sensors[Female::LEFT_FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y + 1].object;
               x = bird->x + 1;
               y = bird->y + 1;
            }
            break;

         case ORIENTATION::EAST:
            if ((bird->x < WIDTH - 1) && (bird->y > 0))
            {
               sensors[Female::LEFT_FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y - 1].object;
               x = bird->x + 1;
               y = bird->y - 1;
            }
            break;

         case ORIENTATION::WEST:
            if ((bird->x > 0) && (bird->y < HEIGHT - 1))
            {
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
               sensors[Female::FRONT_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
               x = bird->x;
               y = bird->y - 1;
            }
            break;

         case ORIENTATION::SOUTH:
            if (bird->y < HEIGHT - 1)
            {
               sensors[Female::FRONT_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
               x = bird->x;
               y = bird->y + 1;
            }
            break;

         case ORIENTATION::EAST:
            if (bird->x < WIDTH - 1)
            {
               sensors[Female::FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
               x = bird->x + 1;
               y = bird->y;
            }
            break;

         case ORIENTATION::WEST:
            if (bird->x > 0)
            {
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
            if ((bird->x < WIDTH - 1) && (bird->y > 0))
            {
               sensors[Female::RIGHT_FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y - 1].object;
               x = bird->x + 1;
               y = bird->y - 1;
            }
            break;

         case ORIENTATION::SOUTH:
            if ((bird->x > 0) && (bird->y < HEIGHT - 1))
            {
               sensors[Female::RIGHT_FRONT_OBJECT_SENSOR] = World[bird->x - 1][bird->y + 1].object;
               x = bird->x - 1;
               y = bird->y + 1;
            }
            break;

         case ORIENTATION::EAST:
            if ((bird->x < WIDTH - 1) && (bird->y < HEIGHT - 1))
            {
               sensors[Female::RIGHT_FRONT_OBJECT_SENSOR] = World[bird->x + 1][bird->y + 1].object;
               x = bird->x + 1;
               y = bird->y + 1;
            }
            break;

         case ORIENTATION::WEST:
            if ((bird->x > 0) && (bird->y > 0))
            {
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
               sensors[Female::RIGHT_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
               x = bird->x + 1;
               y = bird->y;
            }
            break;

         case ORIENTATION::SOUTH:
            if (bird->x > 0)
            {
               sensors[Female::RIGHT_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
               x = bird->x - 1;
               y = bird->y;
            }
            break;

         case ORIENTATION::EAST:
            if (bird->y < HEIGHT - 1)
            {
               sensors[Female::RIGHT_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
               x = bird->x;
               y = bird->y + 1;
            }
            break;

         case ORIENTATION::WEST:
            if (bird->y > 0)
            {
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
            if ((bird->x < WIDTH - 1) && (bird->y < HEIGHT - 1))
            {
               sensors[Female::RIGHT_REAR_OBJECT_SENSOR] = World[bird->x + 1][bird->y + 1].object;
               x = bird->x + 1;
               y = bird->y + 1;
            }
            break;

         case ORIENTATION::SOUTH:
            if ((bird->x > 0) && (bird->y > 0))
            {
               sensors[Female::RIGHT_REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y - 1].object;
               x = bird->x - 1;
               y = bird->y - 1;
            }
            break;

         case ORIENTATION::EAST:
            if ((bird->x > 0) && (bird->y < HEIGHT - 1))
            {
               sensors[Female::RIGHT_REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y + 1].object;
               x = bird->x - 1;
               y = bird->y + 1;
            }
            break;

         case ORIENTATION::WEST:
            if ((bird->x < WIDTH - 1) && (bird->y > 0))
            {
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
               sensors[Female::REAR_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
               x = bird->x;
               y = bird->y + 1;
            }
            break;

         case ORIENTATION::SOUTH:
            if (bird->y > 0)
            {
               sensors[Female::REAR_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
               x = bird->x;
               y = bird->y - 1;
            }
            break;

         case ORIENTATION::EAST:
            if (bird->x > 0)
            {
               sensors[Female::REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
               x = bird->x - 1;
               y = bird->y;
            }
            break;

         case ORIENTATION::WEST:
            if (bird->x < WIDTH - 1)
            {
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
            if ((bird->x > 0) && (bird->y < HEIGHT - 1))
            {
               sensors[Female::LEFT_REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y + 1].object;
               x = bird->x - 1;
               y = bird->y + 1;
            }
            break;

         case ORIENTATION::SOUTH:
            if ((bird->x < WIDTH - 1) && (bird->y > 0))
            {
               sensors[Female::LEFT_REAR_OBJECT_SENSOR] = World[bird->x + 1][bird->y - 1].object;
               x = bird->x + 1;
               y = bird->y - 1;
            }
            break;

         case ORIENTATION::EAST:
            if ((bird->x > 0) && (bird->y > 0))
            {
               sensors[Female::LEFT_REAR_OBJECT_SENSOR] = World[bird->x - 1][bird->y - 1].object;
               x = bird->x - 1;
               y = bird->y - 1;
            }
            break;

         case ORIENTATION::WEST:
            if ((bird->x < WIDTH - 1) && (bird->y < HEIGHT - 1))
            {
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
   sensors[Female::GOAL_SENSOR]        = bird->goal;
   sensors[Female::HAS_OBJECT_SENSOR]  = bird->hasObject;
}


// City block distance.
int distance(int x1, int y1, int x2, int y2)
{
   int xd = x1 - x2;
   int yd = y1 - y2;

   if (xd < 0.0)
   {
      xd = -xd;
   }
   if (yd < 0.0)
   {
      yd = -yd;
   }
   return(xd + yd);
}


// Do bird response.
void doResponse(int gender)
{
   if (gender == MALE)
   {
      doMaleResponse();
   }
   else
   {
      doFemaleResponse();
   }
}


// Do male response.
void doMaleResponse()
{
   Male *bird    = male;
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
         if (bird->food > 0)
         {
            bird->goal = Male::GOAL::ATTEND_FEMALE;
            bird->brain->setNeed(Male::MOUSE_NEED_INDEX, 0.0);
            bird->brain->setNeed(Male::ATTEND_FEMALE_NEED_INDEX, Male::ATTEND_FEMALE_NEED);
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
            if ((World[x][y].locale == LOCALE::DESERT) &&
                (World[x][y].object == OBJECT::NO_OBJECT))
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

   case Male::RESPONSE::FLY:
      MaleFlying = true;
      break;

   case Male::RESPONSE::ALIGHT:
      MaleFlying = false;
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
   Female *bird    = female;
   Cell   *cell    = &World[bird->x][bird->y];
   int    response = bird->response;

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
         cell->object    = OBJECT::NO_OBJECT;
      }
      break;

   case Female::RESPONSE::PUT_OBJECT:
      if (cell->object == OBJECT::NO_OBJECT)
      {
         cell->object    = bird->hasObject;
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
            if ((World[x][y].locale == LOCALE::DESERT) &&
                (World[x][y].object == OBJECT::NO_OBJECT))
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

   if (BehaviorFp != NULL)
   {
       int count = 0;
       for (int x = 0; x < WIDTH; x++)
       {
           for (int y = 0; y < HEIGHT; y++)
           {
               if (World[x][y].object == OBJECT::MOUSE)
               {
                   count++;
               }
           }
       }
       fprintf(BehaviorFp, "\"Mice\": [");
       int n = 0;
       for (int x = 0; x < WIDTH; x++)
       {
           for (int y = 0; y < HEIGHT; y++)
           {
               if (World[x][y].object == OBJECT::MOUSE)
               {
                   fprintf(BehaviorFp, " { \"x\": %d, \"y\": %d }", x, y);
                   if (n < count - 1)
                   {
                       fprintf(BehaviorFp, ",");
                   }
                   n++;
               }
           }
       }
       fprintf(BehaviorFp, " ],\n");
   }
}


// Save male.
void saveMale(char *filename)
{
   male->save(filename);
   if (Verbose)
   {
      printf("Male training saved to file %s\n", filename);
   }
}


// Save female.
void saveFemale(char *filename)
{
   female->save(filename);
   if (Verbose)
   {
      printf("Female training saved to file %s\n", filename);
   }
}


// Load male.
void loadMale(char *filename)
{
   male->load(filename);
   if (Verbose)
   {
      printf("Male network loaded from file %s\n", filename);
   }

   // Initialize needs.
   male->initNeeds();
}


// Load female.
void loadFemale(char *filename)
{
   female->load(filename);
   if (Verbose)
   {
      printf("Female network loaded from file %s\n", filename);
   }

   // Initialize needs.
   female->initNeeds();
}


// Open behavior file.
void openBehaviorFile(char *filename)
{
   if ((BehaviorFp = fopen(filename, "w")) == NULL)
   {
      fprintf(stderr, "Cannot open behavior file: %s\n", filename);
      exit(1);
   }
   fprintf(BehaviorFp, "[\n");
   fflush(BehaviorFp);
}


// Write to behavior file.
void writeBehaviorFile(char *text)
{
   if (BehaviorFp != NULL)
   {
      fprintf(BehaviorFp, text);
      fflush(BehaviorFp);
   }
}


// Close behavior file.
void closeBehaviorFile()
{
   if (BehaviorFp != NULL)
   {
      fprintf(BehaviorFp, "]\n");
      fclose(BehaviorFp);
      BehaviorFp = NULL;
   }
}
