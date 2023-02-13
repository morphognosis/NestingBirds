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
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, 1.0, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE);
    wantMouseGoal = brain->addGoal(MOUSE_NEED_INDEX, sensors, 0, RESPONSE::WANT_MOUSE, MOUSE_NEED);
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, 1.0, (Mona::SENSOR)OBJECT::MOUSE, DONT_CARE);
    eatMouseGoal = brain->addGoal(MOUSE_NEED_INDEX, sensors, 0, Bird::RESPONSE::EAT_MOUSE, MOUSE_NEED);

    // Stone goals.
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE);
    wantStoneGoal = brain->addGoal(STONE_NEED_INDEX, sensors, 0, RESPONSE::WANT_STONE, STONE_NEED);
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE);
    putStoneGoal = brain->addGoal(STONE_NEED_INDEX, sensors, 0, Bird::RESPONSE::PUT_OBJECT, STONE_NEED);

    // Lay egg goals.
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::EGG, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE,
        DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE,
        DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE,
        (Mona::SENSOR)ORIENTATION::SOUTH, DONT_CARE, DONT_CARE, DONT_CARE);
    layEggGoal = brain->addGoal(LAY_EGG_NEED_INDEX, sensors, 0, LAY_EGG_NEED);

    // Brooding on egg.
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::EGG, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
    broodEggGoal = brain->addGoal(BROOD_EGG_NEED_INDEX, sensors, 0, Bird::RESPONSE::DO_NOTHING, BROOD_EGG_NEED);

    // Set initial response.
    response = Bird::RESPONSE::DO_NOTHING;
}

// Load sensors.
void Female::loadSensors(vector<Mona::SENSOR>& sensors,
    Mona::SENSOR currentLocale, Mona::SENSOR currentObject,
    Mona::SENSOR leftLocale, Mona::SENSOR leftObject,
    Mona::SENSOR leftFrontLocale, Mona::SENSOR leftFrontObject,
    Mona::SENSOR frontLocale, Mona::SENSOR frontObject,
    Mona::SENSOR rightFrontLocale, Mona::SENSOR rightFrontObject,
    Mona::SENSOR rightLocale, Mona::SENSOR rightObject,
    Mona::SENSOR rightRearLocale, Mona::SENSOR rightRearObject,
    Mona::SENSOR rearLocale, Mona::SENSOR rearObject,
    Mona::SENSOR leftRearLocale, Mona::SENSOR leftRearObject,
    Mona::SENSOR orientation, Mona::SENSOR hunger,
    Mona::SENSOR hasObject, Mona::SENSOR state)
{
    sensors.clear();
    sensors.push_back(currentLocale);
    sensors.push_back(currentObject);
    sensors.push_back(leftLocale);
    sensors.push_back(leftObject);
    sensors.push_back(leftFrontLocale);
    sensors.push_back(leftFrontObject);
    sensors.push_back(frontLocale);
    sensors.push_back(frontObject);
    sensors.push_back(rightFrontLocale);
    sensors.push_back(rightFrontObject);
    sensors.push_back(rightLocale);
    sensors.push_back(rightObject);
    sensors.push_back(rightRearLocale);
    sensors.push_back(rightRearObject);
    sensors.push_back(rearLocale);
    sensors.push_back(rearObject);
    sensors.push_back(leftRearLocale);
    sensors.push_back(leftRearObject);
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
        brain->setNeed(MOUSE_NEED_INDEX, 0.0);
        brain->setNeed(STONE_NEED_INDEX, 0.0);
        brain->setNeed(LAY_EGG_NEED_INDEX, 0.0);
        brain->setNeed(BROOD_EGG_NEED_INDEX, BROOD_EGG_NEED);
    }
}

// Set response override.
void Female::setResponseOverride()
{
    brain->responseOverride = response;
}

// Cycle female.
int Female::cycle()
{
   vector<Mona::SENSOR> brainSensors(NUM_SENSORS);
   for (int i = 0; i < NUM_SENSORS; i++)
   {
      brainSensors[i] = (Mona::SENSOR)sensors[i];
   }
   response = brain->cycle(brainSensors);
   return response;
}

// Load.
void Female::load(char *filename)
{
    FILE* fp;
    if ((fp = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Cannot load female");
        exit(1);
    }
    brain->load(fp);
    fclose(fp);
}

// Save.
void Female::save(char *filename)
{
    FILE* fp;
    if ((fp = fopen(filename, "w")) == NULL)
    {
        fprintf(stderr, "Cannot save female");
        exit(1);
    }
    brain->save(fp);
    fclose(fp);
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
          printf("Left front: ");
          break;

      case 3:
         printf("Front: ");
         break;

      case 4:
          printf("Right front: ");
          break;

      case 5:
         printf("Right: ");
         break;

      case 6:
          printf("Right rear: ");
          break;

      case 7:
          printf("Rear: ");
          break;

      case 8:
          printf("Left rear: ");
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
