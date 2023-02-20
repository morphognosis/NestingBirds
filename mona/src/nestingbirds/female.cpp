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
    brain = new Mona(NUM_SENSORS, 0, NUM_NEEDS, RANDOM_NUMBER_SEED);

    // Install sensor modes: nest mode ignores hunger.
    vector<bool> mask;
    loadMask(mask, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true,
        true, true, true, true,
        true, false, true, true);
    brain->addSensorMode(mask);
    loadMask(mask, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true,
        true, true, true, true,
        true, true, true, true);
    brain->addSensorMode(mask);
    brain->delSensorMode(0);
    int nestMode = 0;
    int foodMode = 1;

    // Motors:
    Mona::Motor* doNothing = brain->newMotor();
    Mona::Motor* eat = brain->newMotor();
    Mona::Motor* get = brain->newMotor();
    Mona::Motor* put = brain->newMotor();
    Mona::Motor* toss = brain->newMotor();
    Mona::Motor* move = brain->newMotor();
    Mona::Motor* turnRight = brain->newMotor();
    Mona::Motor* turnLeft = brain->newMotor();
    Mona::Motor* turnAround = brain->newMotor();
    Mona::Motor* stateOn = brain->newMotor();
    Mona::Motor* stateOff = brain->newMotor();
    Mona::Motor* wantMouse = brain->newMotor();
    Mona::Motor* wantStone = brain->newMotor();
    Mona::Motor* layEgg = brain->newMotor();

    // Needs.
    initNeeds();

    // Goals:

    // Food goals.
    vector<Mona::SENSOR> sensors;
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, 1.0, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE);
    int wantMouseGoal = brain->addGoal(MOUSE_NEED_INDEX, sensors, foodMode, RESPONSE::WANT_MOUSE, MOUSE_NEED);
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, 1.0, (Mona::SENSOR)OBJECT::MOUSE, DONT_CARE);
    int eatMouseGoal = brain->addGoal(MOUSE_NEED_INDEX, sensors, foodMode, Bird::RESPONSE::EAT_MOUSE, MOUSE_NEED);

    // Stone goals.
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE);
    int wantStoneGoal = brain->addGoal(STONE_NEED_INDEX, sensors, nestMode, RESPONSE::WANT_STONE, STONE_NEED);
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE);
    int putStoneGoal = brain->addGoal(STONE_NEED_INDEX, sensors, nestMode, Bird::RESPONSE::PUT_OBJECT, STONE_NEED);

    // Lay egg goals.
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::EGG, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE,
        DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE,
        DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE,
        (Mona::SENSOR)ORIENTATION::SOUTH, DONT_CARE, DONT_CARE, DONT_CARE);
    int layEggGoal = brain->addGoal(LAY_EGG_NEED_INDEX, sensors, nestMode, LAY_EGG_NEED);

    // Brooding on egg.
    loadSensors(sensors, DONT_CARE, (Mona::SENSOR)OBJECT::EGG, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE);
    int broodEggGoal = brain->addGoal(BROOD_EGG_NEED_INDEX, sensors, nestMode, Bird::RESPONSE::DO_NOTHING, BROOD_EGG_NEED);

    // Set initial response.
    response = Bird::RESPONSE::DO_NOTHING;
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


// Load mask.
void Female::loadMask(vector<bool>& mask,
    bool currentLocale, bool currentObject,
    bool leftLocale, bool leftObject,
    bool leftFrontLocale, bool leftFrontObject,
    bool frontLocale, bool frontObject,
    bool rightFrontLocale, bool rightFrontObject,
    bool rightLocale, bool rightObject,
    bool rightRearLocale, bool rightRearObject,
    bool rearLocale, bool rearObject,
    bool leftRearLocale, bool leftRearObject,
    bool orientation, bool hunger,
    bool hasObject, bool state)
{
    mask.clear();
    mask.push_back(currentLocale);
    mask.push_back(currentObject);
    mask.push_back(leftLocale);
    mask.push_back(leftObject);
    mask.push_back(leftFrontLocale);
    mask.push_back(leftFrontObject);
    mask.push_back(frontLocale);
    mask.push_back(frontObject);
    mask.push_back(rightFrontLocale);
    mask.push_back(rightFrontObject);
    mask.push_back(rightLocale);
    mask.push_back(rightObject);
    mask.push_back(rightRearLocale);
    mask.push_back(rightRearObject);
    mask.push_back(rearLocale);
    mask.push_back(rearObject);
    mask.push_back(leftRearLocale);
    mask.push_back(leftRearObject);
    mask.push_back(orientation);
    mask.push_back(hunger);
    mask.push_back(hasObject);
    mask.push_back(state);
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
