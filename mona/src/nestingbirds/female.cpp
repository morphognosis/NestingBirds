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
Mona::NEED Female::LAY_EGG_NEED   = strtod(FEMALE_DEFAULT_LAY_EGG_NEED, 0);
Mona::NEED Female::BROOD_EGG_NEED = strtod(FEMALE_DEFAULT_BROOD_EGG_NEED, 0);

extern int RANDOM_NUMBER_SEED;

// Construct female bird.
Female::Female() : Bird(FEMALE)
{
    // Create Mona bird brain.
    brain = new Mona(NUM_SENSORS, Bird::RESPONSE::NUM_RESPONSES +
        RESPONSE::NUM_RESPONSES, NUM_NEEDS, RANDOM_NUMBER_SEED);

    // Motors:
    doNothing = brain->motors[Bird::RESPONSE::DO_NOTHING];
    eat = brain->motors[Bird::RESPONSE::EAT_MOUSE];
    get = brain->motors[Bird::RESPONSE::GET_OBJECT];
    put = brain->motors[Bird::RESPONSE::PUT_OBJECT];
    toss = brain->motors[Bird::RESPONSE::TOSS_OBJECT];
    move = brain->motors[Bird::RESPONSE::MOVE_FORWARD];
    turnRight = brain->motors[Bird::RESPONSE::TURN_RIGHT];
    turnLeft = brain->motors[Bird::RESPONSE::TURN_LEFT];
    turnAround = brain->motors[Bird::RESPONSE::TURN_AROUND];
    stateOn = brain->motors[Bird::RESPONSE::STATE_ON];
    stateOff = brain->motors[Bird::RESPONSE::STATE_OFF];
    wantMouse = brain->motors[Female::RESPONSE::WANT_MOUSE];
    wantStone = brain->motors[Female::RESPONSE::WANT_STONE];
    layEgg = brain->motors[Female::RESPONSE::LAY_EGG];

    // Needs.
    initNeeds();

    // Goals.

    // Food goals.
    vector<Mona::SENSOR> sensors;
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, 1.0, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE);
    hungry = brain->newReceptor(sensors, 0);
    askForMouse = brain->newMediator(1.0);
    askForMouse->addEvent(Mona::CAUSE_EVENT, (Mona::Neuron*)hungry);
    askForMouse->addEvent(Mona::RESPONSE_EVENT, (Mona::Neuron*)wantMouse);
    askForMouse->addEvent(Mona::EFFECT_EVENT, (Mona::Neuron*)hungry);
    brain->addGoal(MOUSE_NEED_INDEX, askForMouse, MOUSE_NEED);
    askForMouse->instinct = true;
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, 1.0, (Mona::SENSOR)OBJECT::MOUSE, DONT_CARE);
    readyToEat = brain->newReceptor(sensors, 0);
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, 0.0, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE);
    notHungry = brain->newReceptor(sensors, 0);
    eatMouse = brain->newMediator(1.0);
    eatMouse->addEvent(Mona::CAUSE_EVENT, (Mona::Neuron*)readyToEat);
    eatMouse->addEvent(Mona::RESPONSE_EVENT, (Mona::Neuron*)eat);
    eatMouse->addEvent(Mona::EFFECT_EVENT, (Mona::Neuron*)notHungry);
    brain->addGoal(MOUSE_NEED_INDEX, eatMouse, MOUSE_NEED);
    eatMouse->instinct = true;

    // Stone goals.
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE);
    int goal = brain->addGoal(STONE_NEED_INDEX, sensors, 0, RESPONSE::WANT_STONE, STONE_NEED);
    missingStone = brain->getGoalReceptor(STONE_NEED_INDEX, goal);
    //missingStone = brain->newReceptor(sensors, 0);
    askForStone = brain->newMediator(1.0);
    askForStone->addEvent(Mona::CAUSE_EVENT, (Mona::Neuron*)missingStone);
    askForStone->addEvent(Mona::RESPONSE_EVENT, (Mona::Neuron*)wantStone);
    askForStone->addEvent(Mona::EFFECT_EVENT, (Mona::Neuron*)missingStone);
    //brain->addGoal(STONE_NEED_INDEX, askForStone, STONE_NEED);
    askForStone->instinct = true;
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE);
    stoneReady = brain->newReceptor(sensors, 0);
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE);
    stonePlaced = brain->newReceptor(sensors, 0);
    placeStone = brain->newMediator(1.0);
    placeStone->addEvent(Mona::CAUSE_EVENT, (Mona::Neuron*)missingStone);
    placeStone->addEvent(Mona::RESPONSE_EVENT, (Mona::Neuron*)put);
    placeStone->addEvent(Mona::EFFECT_EVENT, (Mona::Neuron*)stonePlaced);
    brain->addGoal(STONE_NEED_INDEX, placeStone, STONE_NEED);
    placeStone->instinct = true;

    // Lay egg goals.
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE, (Mona::SENSOR)OBJECT::STONE,
        DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE,
        (Mona::SENSOR)ORIENTATION::SOUTH, DONT_CARE, DONT_CARE, DONT_CARE);
    readyToLayEgg = brain->newReceptor(sensors, 0);
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::EGG, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
    goal = brain->addGoal(LAY_EGG_NEED_INDEX, sensors, 0, LAY_EGG_NEED);
    eggInNest = brain->getGoalReceptor(LAY_EGG_NEED_INDEX, goal);
    layEggInNest = brain->newMediator(1.0);
    layEggInNest->addEvent(Mona::CAUSE_EVENT, (Mona::Neuron*)readyToLayEgg);
    layEggInNest->addEvent(Mona::RESPONSE_EVENT, (Mona::Neuron*)layEgg);
    layEggInNest->addEvent(Mona::EFFECT_EVENT, (Mona::Neuron*)eggInNest);
    layEggInNest->instinct = true;

    // Brooding on egg.
    roostInNest = brain->newMediator(1.0);
    roostInNest->addEvent(Mona::CAUSE_EVENT, (Mona::Neuron*)eggInNest);
    roostInNest->addEvent(Mona::RESPONSE_EVENT, (Mona::Neuron*)doNothing);
    roostInNest->addEvent(Mona::EFFECT_EVENT, (Mona::Neuron*)eggInNest);
    brain->addGoal(BROOD_EGG_NEED_INDEX, roostInNest, BROOD_EGG_NEED);
    roostInNest->instinct = true;

    // Set initial response.
    response = Bird::RESPONSE::DO_NOTHING;
}

// Load sensors.
void Female::loadSensors(vector<Mona::SENSOR>& sensors,
    Mona::SENSOR currentLocale, Mona::SENSOR currentObject,
    Mona::SENSOR leftLocale, Mona::SENSOR leftObject,
    Mona::SENSOR frontLocale, Mona::SENSOR frontObject,
    Mona::SENSOR rightLocale, Mona::SENSOR rightObject,
    Mona::SENSOR rearLocale, Mona::SENSOR rearObject,
    Mona::SENSOR orientation, Mona::SENSOR hunger,
    Mona::SENSOR hasObject, Mona::SENSOR state)
{
    sensors.clear();
    sensors.push_back(currentLocale);
    sensors.push_back(currentObject);
    sensors.push_back(leftLocale);
    sensors.push_back(leftObject);
    sensors.push_back(frontLocale);
    sensors.push_back(frontObject);
    sensors.push_back(rightLocale);
    sensors.push_back(rightObject);
    sensors.push_back(rearLocale);
    sensors.push_back(rearObject);
    sensors.push_back(orientation);
    sensors.push_back(hunger);
    sensors.push_back(hasObject);
    sensors.push_back(state);
}
// Initialize female needs.
void Female::initNeeds()
{
    brain->setNeed(MOUSE_NEED_INDEX, 0.0);
    brain->setNeed(STONE_NEED_INDEX, 0.0);
    brain->setNeed(LAY_EGG_NEED_INDEX, LAY_EGG_NEED);
    brain->setNeed(BROOD_EGG_NEED_INDEX, 0.0);
}

// Set female needs.
void Female::setNeeds()
{
    if (food <= 0)
    {
        brain->setNeed(MOUSE_NEED_INDEX, MOUSE_NEED);
    }

    if (sensors[Bird::CURRENT_OBJECT_SENSOR] == OBJECT::NO_OBJECT)
    {
        brain->setNeed(STONE_NEED_INDEX, STONE_NEED);
    }

    if (sensors[Bird::CURRENT_OBJECT_SENSOR] == OBJECT::EGG)
    {
        brain->setNeed(BROOD_EGG_NEED_INDEX, BROOD_EGG_NEED);
    }
}

// Cycle female.
int Female::cycle()
{
   vector<Mona::SENSOR> brainSensors(NUM_SENSORS);
   for (int i = 0; i < NUM_SENSORS; i++)
   {
      brainSensors[i] = (Mona::SENSOR)sensors[i];
   }
   return(brain->cycle(brainSensors));
}


// Print female.
void Female::print()
{
   printf("Sensors: [");
   printSensors();
   printf("], ");
   printState();
   printf("], "), 
   printf("Needs: [");
   printNeeds();
   printf("], ");
   printf("Response: ");
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
         printf("Front: ");
         break;

      case 3:
         printf("Right: ");
         break;

      case 4:
          printf("Rear: ");
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
   printf("]");
}

// Print needs.
void Female::printNeeds()
{
    for (int i = 0; i < NUM_NEEDS; i++)
    {
        switch (i)
        {
        case MOUSE_NEED_INDEX:
            printf("[Mouse: %f],", brain->getNeed(MOUSE_NEED_INDEX));
            break;

        case STONE_NEED_INDEX:
            printf("[Stone: %f],", brain->getNeed(STONE_NEED_INDEX));
            break;

        case LAY_EGG_NEED_INDEX:
            printf("[Lay egg: %f],", brain->getNeed(LAY_EGG_NEED_INDEX));
            break;

        case BROOD_EGG_NEED_INDEX:
            printf("[Brood egg: %f]", brain->getNeed(BROOD_EGG_NEED_INDEX));
            break;
        }
    }
}

// Print response.
void Female::printResponse()
{
   printf(RESPONSE::toString(response));
}
