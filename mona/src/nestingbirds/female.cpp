// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The female bird.

#include "female.hpp"

// Food.
int Female:: FOOD_DURATION        = atoi(FEMALE_DEFAULT_FOOD_DURATION);
int Female:: INITIAL_FOOD         = atoi(FEMALE_DEFAULT_INITIAL_FOOD);
bool Female::RANDOMIZE_FOOD_LEVEL = false;

// Needs.
Mona::NEED Female::MOUSE_NEED = strtod(FEMALE_DEFAULT_MOUSE_NEED, 0);
Mona::NEED Female::STONE_NEED = strtod(FEMALE_DEFAULT_STONE_NEED, 0);
Mona::NEED Female::EGG_NEED   = strtod(FEMALE_DEFAULT_EGG_NEED, 0);

extern int RANDOM_NUMBER_SEED;

// Construct female bird.
Female::Female() : Bird(FEMALE)
{
    // Create Mona bird brain.
    brain = new Mona(NUM_SENSORS, Bird::RESPONSE::NUM_RESPONSES +
        RESPONSE::NUM_RESPONSES, NUM_NEEDS, RANDOM_NUMBER_SEED);
    vector<bool> sensorMask;
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        sensorMask.push_back(true);
    }
    brain->addSensorMode(sensorMask);

    // Motors:
    Mona::Motor *doNothing = brain->newMotor(Bird::RESPONSE::DO_NOTHING);
    Mona::Motor *eat = brain->newMotor(Bird::RESPONSE::EAT);
    Mona::Motor *get = brain->newMotor(Bird::RESPONSE::GET);
    Mona::Motor *put = brain->newMotor(Bird::RESPONSE::PUT);
    Mona::Motor *toss = brain->newMotor(Bird::RESPONSE::TOSS);
    Mona::Motor *move = brain->newMotor(Bird::RESPONSE::MOVE);
    Mona::Motor *turnRight = brain->newMotor(Bird::RESPONSE::TURN_RIGHT);
    Mona::Motor *turnLeft = brain->newMotor(Bird::RESPONSE::TURN_LEFT);
    Mona::Motor *stateOn = brain->newMotor(Bird::RESPONSE::STATE_ON);
    Mona::Motor *stateOff = brain->newMotor(Bird::RESPONSE::STATE_OFF);
    Mona::Motor *wantMouse = brain->newMotor(Female::RESPONSE::WANT_MOUSE);
    Mona::Motor *wantStone = brain->newMotor(Female::RESPONSE::WANT_STONE);
    Mona::Motor *layEgg = brain->newMotor(Female::RESPONSE::LAY_EGG);

    // Needs.
    brain->setNeed(MOUSE_NEED_INDEX, MOUSE_NEED);
    brain->setNeed(STONE_NEED_INDEX, STONE_NEED);
    brain->setNeed(EGG_NEED_INDEX, EGG_NEED);

    // Goals.

    // Food goal:
    vector<Mona::SENSOR> sensors;
    setSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::MOUSE, DONT_CARE);
    Mona::Receptor* hasMouse = brain->newReceptor(sensors, 0);
    setSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, 0.0, DONT_CARE, DONT_CARE);
    Mona::Receptor* mouseEaten = brain->newReceptor(sensors, 0);
    Mona::Mediator *eatFood = brain->newMediator(brain->MIN_ENABLEMENT);
    eatFood->addEvent(Mona::CAUSE_EVENT, (Mona::Neuron*)hasMouse);
    eatFood->addEvent(Mona::RESPONSE_EVENT, (Mona::Neuron*)eat);
    eatFood->addEvent(Mona::EFFECT_EVENT, (Mona::Neuron*)mouseEaten);
    //brain->addGoal(MOUSE_NEED_INDEX, eatFood);

    // Set initial response.
    response = Bird::RESPONSE::DO_NOTHING;
}

// Set sensors.
void Female::setSensors(vector<Mona::SENSOR>& sensors,
    Mona::SENSOR currentLocale, Mona::SENSOR currentObject,
    Mona::SENSOR leftLocale, Mona::SENSOR leftObject,
    Mona::SENSOR forwardLocale, Mona::SENSOR forwardObject,
    Mona::SENSOR rightLocale, Mona::SENSOR rightObject,
    Mona::SENSOR mateProximity,
    Mona::SENSOR orientation, Mona::SENSOR hunger,
    Mona::SENSOR hasObject, Mona::SENSOR state)
{
    sensors.clear();
    sensors.push_back(currentLocale);
    sensors.push_back(currentObject);
    sensors.push_back(leftLocale);
    sensors.push_back(leftObject);
    sensors.push_back(forwardLocale);
    sensors.push_back(forwardObject);
    sensors.push_back(rightLocale);
    sensors.push_back(rightObject);
    sensors.push_back(mateProximity);
    sensors.push_back(orientation);
    sensors.push_back(hunger);
    sensors.push_back(hasObject);
    sensors.push_back(state);
}

// Cycle female.
int Female::cycle()
{
   vector<Mona::SENSOR> brainSensors(NUM_SENSORS + 2);
   for (int i = 0; i < NUM_SENSORS; i++)
   {
      brainSensors[i] = (Mona::SENSOR)brain->sensors[i];
   }
   brainSensors[NUM_SENSORS]     = (Mona::SENSOR)hasObject;
   brainSensors[NUM_SENSORS + 1] = (Mona::SENSOR)food;
   return(brain->cycle(brainSensors));
}


// Print female.
void Female::print()
{
   printf("Sensors: [");
   printSensors();
   printf("], ");
   printState();
   printf(", Response: ");
   printResponse();
}


// Print sensors.
void Female::printSensors()
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
}


// Print response.
void Female::printResponse()
{
   printf(RESPONSE::toString(response));
}
