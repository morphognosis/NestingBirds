// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The male bird.

#include "male.hpp"

// Food.
int Male:: FOOD_DURATION        = atoi(MALE_DEFAULT_FOOD_DURATION);
int Male:: INITIAL_FOOD         = atoi(MALE_DEFAULT_INITIAL_FOOD);
bool Male::RANDOMIZE_FOOD_LEVEL = false;

// Needs.
Mona::NEED Male::MOUSE_NEED         = strtod(MALE_DEFAULT_MOUSE_NEED, 0);
Mona::NEED Male::FEMALE_MOUSE_NEED  = strtod(MALE_DEFAULT_FEMALE_MOUSE_NEED, 0);
Mona::NEED Male::STONE_NEED         = strtod(MALE_DEFAULT_STONE_NEED, 0);
Mona::NEED Male::FEMALE_STONE_NEED  = strtod(MALE_DEFAULT_FEMALE_STONE_NEED, 0);
Mona::NEED Male::ATTEND_FEMALE_NEED = strtod(MALE_DEFAULT_ATTEND_FEMALE_NEED, 0);

extern int RANDOM_NUMBER_SEED;

// Construct male bird.
Male::Male() : Bird(MALE)
{
    // Create Mona bird brain.
    brain = new Mona(NUM_SENSORS, Bird::RESPONSE::NUM_RESPONSES +
        RESPONSE::NUM_RESPONSES, NUM_NEEDS, RANDOM_NUMBER_SEED);

    // Motors:
    Mona::Motor* doNothing = brain->newMotor(Bird::RESPONSE::DO_NOTHING);
    Mona::Motor* eat = brain->newMotor(Bird::RESPONSE::EAT);
    Mona::Motor* get = brain->newMotor(Bird::RESPONSE::GET);
    Mona::Motor* put = brain->newMotor(Bird::RESPONSE::PUT);
    Mona::Motor* toss = brain->newMotor(Bird::RESPONSE::TOSS);
    Mona::Motor* move = brain->newMotor(Bird::RESPONSE::MOVE);
    Mona::Motor* turnRight = brain->newMotor(Bird::RESPONSE::TURN_RIGHT);
    Mona::Motor* turnLeft = brain->newMotor(Bird::RESPONSE::TURN_LEFT);
    Mona::Motor* stateOn = brain->newMotor(Bird::RESPONSE::STATE_ON);
    Mona::Motor* stateOff = brain->newMotor(Bird::RESPONSE::STATE_OFF);
    Mona::Motor* giveMouse = brain->newMotor(Male::RESPONSE::GIVE_MOUSE);
    Mona::Motor* giveStone = brain->newMotor(Male::RESPONSE::GIVE_STONE);

    // Needs.
    brain->setNeed(MOUSE_NEED_INDEX, MOUSE_NEED);
    brain->setNeed(STONE_NEED_INDEX, STONE_NEED);
    brain->setNeed(FEMALE_MOUSE_NEED_INDEX, FEMALE_MOUSE_NEED);
    brain->setNeed(STONE_NEED_INDEX, STONE_NEED);
    brain->setNeed(FEMALE_STONE_NEED_INDEX, FEMALE_STONE_NEED);
    brain->setNeed(ATTEND_FEMALE_NEED_INDEX, ATTEND_FEMALE_NEED);

    // Set initial response.
    response = Bird::RESPONSE::DO_NOTHING;
}


// Cycle male.
int Male::cycle()
{
   vector<Mona::SENSOR> brainSensors(NUM_SENSORS);
   for (int i = 0; i < NUM_SENSORS; i++)
   {
      brainSensors[i] = (Mona::SENSOR)sensors[i];
   }
   return(brain->cycle(brainSensors));
}


// Print male.
void Male::print()
{
   printf("Sensors: [");
   printSensors();
   printf("], ");
   printState();
   printf(", Response: ");
   printResponse();
}


// Print sensors.
void Male::printSensors()
{
   printf("[Cell sensors: ");

   for (int i = 0; i < Bird::NUM_CELL_SENSORS; i++)
   {
      printf("[");
      switch (i)
      {
      case 0:
         printf("Current: ");
         break;

      case 1:
         printf("Left: ");
         break;

      case 2:
         printf("Forward: ");
         break;

      case 3:
         printf("Right: ");
         break;
      }
      printf("%s", LOCALE::toString(sensors[i * Bird::CELL_SENSOR::NUM_SENSORS]));
      printf(",");
      printf("%s", OBJECT::toString(sensors[(i *Bird::CELL_SENSOR::NUM_SENSORS)+1]));
      printf("]");
      if (i < NUM_CELL_SENSORS - 1)
      {
         printf(",");
      }
   }
   printf("], Mate proximity: ");
   switch (sensors[MATE_PROXIMITY_SENSOR])
   {
   case MATE_PROXIMITY_UNKNOWN:
      printf("UNKNOWN");
      break;

   case MATE_PROXIMITY_PRESENT:
      printf("PRESENT");
      break;

   case MATE_PROXIMITY_LEFT:
      printf("LEFT");
      break;

   case MATE_PROXIMITY_FORWARD:
      printf("FORWARD");
      break;

   case MATE_PROXIMITY_RIGHT:
      printf("RIGHT");
      break;
   }
   printf(", Want food sensor: ");
   if (sensors[WANT_MOUSE_SENSOR] == 1)
   {
      printf("true");
   }
   else
   {
      printf("false");
   }
   printf(", Want stone sensor: ");
   if (sensors[WANT_STONE_SENSOR] == 1)
   {
      printf("true");
   }
   else
   {
      printf("false");
   }
}


// Print response.
void Male::printResponse()
{
   printf(RESPONSE::toString(response));
}
