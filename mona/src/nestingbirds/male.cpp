// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The male bird.

#include "male.hpp"

// Food.
int Male:: FOOD_DURATION        = atoi(MALE_DEFAULT_FOOD_DURATION);
int Male:: INITIAL_FOOD         = atoi(MALE_DEFAULT_INITIAL_FOOD);
bool Male::RANDOMIZE_FOOD_LEVEL = false;

// Needs.
Mona::NEED Male::GOTO_FOREST_NEED = strtod(MALE_DEFAULT_GOTO_FOREST_NEED, 0);
Mona::NEED Male::FIND_MOUSE_NEED = strtod(MALE_DEFAULT_FIND_MOUSE_NEED, 0);
Mona::NEED Male::GET_MOUSE_NEED = strtod(MALE_DEFAULT_GET_MOUSE_NEED, 0);
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
    brain = new Mona(NUM_SENSORS, NUM_NEEDS, RANDOM_NUMBER_SEED);

    // Install sensor modes: nest mode ignores hunger.
    vector<bool> mask;
    loadMask(mask, true, false, false, false, false, false,
        false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, true, false,
        false, false, false);
    brain->addSensorMode(mask);
    int baseMode = 0;
    int gotoMode = 1;

    // Motors:
    Mona::Motor* doNothing = brain->newMotor();
    Mona::Motor* move = brain->newMovementMotor(Bird::RESPONSE::MOVE_FORWARD);
    Mona::Motor* turnRight = brain->newMovementMotor(Bird::RESPONSE::TURN_RIGHT);
    Mona::Motor* turnLeft = brain->newMovementMotor(Bird::RESPONSE::TURN_LEFT);
    Mona::Motor* turnAround = brain->newMovementMotor(Bird::RESPONSE::TURN_AROUND);
    Mona::Motor* eat = brain->newMotor();
    Mona::Motor* get = brain->newMotor();
    Mona::Motor* put = brain->newMotor();
    Mona::Motor* toss = brain->newMotor();
    Mona::Motor* stateOn = brain->newMotor();
    Mona::Motor* stateOff = brain->newMotor();
    Mona::Motor* giveMouse = brain->newMotor();
    Mona::Motor* giveStone = brain->newMotor();

    // Needs.
    initNeeds();

    // Goals:

    // Food goals.
    vector<Mona::SENSOR> sensors;
    loadSensors(sensors, LOCALE::FOREST, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, OBJECT::NO_OBJECT, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE);
    int gotoForestGoal = brain->addGoal(GOTO_FOREST_NEED_INDEX, sensors, gotoMode, GOTO_FOREST_NEED);
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, 0.0, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE);
    int eatMouseGoal = brain->addGoal(MOUSE_NEED_INDEX, sensors, baseMode, MOUSE_NEED);
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE,
        MATE_PROXIMITY_PRESENT, 0.0, DONT_CARE);
    int giveMouseGoal = brain->addGoal(FEMALE_MOUSE_NEED_INDEX, sensors, baseMode, FEMALE_MOUSE_NEED);
 
    // Stone goals.
    loadSensors(sensors, (Mona::SENSOR)LOCALE::DESERT, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE);
    int getStoneGoal = brain->addGoal(STONE_NEED_INDEX, sensors, baseMode, STONE_NEED);
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE,
        MATE_PROXIMITY_PRESENT, DONT_CARE, 0.0);
    int giveStoneGoal = brain->addGoal(FEMALE_STONE_NEED_INDEX, sensors, baseMode, FEMALE_STONE_NEED);

    // Attend female goal.
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        MATE_PROXIMITY_PRESENT, DONT_CARE, DONT_CARE);
    int attendFemaleGoal = brain->addGoal(ATTEND_FEMALE_NEED_INDEX, sensors, baseMode, doNothing, ATTEND_FEMALE_NEED);

    // Set initial response.
    response = Bird::RESPONSE::DO_NOTHING;
}

// Initialize male needs.
void Male::initNeeds()
{
    brain->setNeed(GOTO_FOREST_NEED_INDEX, 0.0);
    brain->setNeed(FIND_MOUSE_NEED_INDEX, 0.0);
    brain->setNeed(GET_MOUSE_NEED_INDEX, 0.0);
    brain->setNeed(MOUSE_NEED_INDEX, 0.0);
    brain->setNeed(FEMALE_MOUSE_NEED_INDEX, 0.0);
    brain->setNeed(STONE_NEED_INDEX, 0.0);
    brain->setNeed(FEMALE_STONE_NEED_INDEX, 0.0);
    brain->setNeed(ATTEND_FEMALE_NEED_INDEX, ATTEND_FEMALE_NEED);
}

// Set male needs.
void Male::setNeeds()
{
    if (food == 0)
    {
        if (sensors[Male::HAS_OBJECT_SENSOR] != OBJECT::MOUSE &&
            sensors[Male::CURRENT_OBJECT_SENSOR] != OBJECT::MOUSE &&
            sensors[Male::CURRENT_LOCALE_SENSOR] != LOCALE::FOREST)
        {
            brain->setNeed(GOTO_FOREST_NEED_INDEX, GOTO_FOREST_NEED);
        }
        if (sensors[Male::HAS_OBJECT_SENSOR] != OBJECT::MOUSE &&
            sensors[Male::CURRENT_OBJECT_SENSOR] != OBJECT::MOUSE &&
            sensors[Male::CURRENT_LOCALE_SENSOR] == LOCALE::FOREST)
        {
            brain->setNeed(FIND_MOUSE_NEED_INDEX, FIND_MOUSE_NEED);
        }
        if (sensors[Male::HAS_OBJECT_SENSOR] != OBJECT::MOUSE &&
            sensors[Male::CURRENT_OBJECT_SENSOR] == OBJECT::MOUSE &&
            sensors[Male::CURRENT_LOCALE_SENSOR] == LOCALE::FOREST)
        {
            brain->setNeed(GET_MOUSE_NEED_INDEX, GET_MOUSE_NEED);
        }
        if (sensors[Male::HAS_OBJECT_SENSOR] == OBJECT::MOUSE &&
            sensors[Male::CURRENT_OBJECT_SENSOR] == OBJECT::MOUSE &&
            sensors[Male::CURRENT_LOCALE_SENSOR] == LOCALE::FOREST)
        {
            brain->setNeed(MOUSE_NEED_INDEX, MOUSE_NEED);
        }
    }

    if (sensors[Male::WANT_MOUSE_SENSOR] == 1)
    {
        brain->setNeed(FEMALE_MOUSE_NEED_INDEX, FEMALE_MOUSE_NEED);
    }

    if (sensors[Male::WANT_STONE_SENSOR] == 1)
    {
        brain->setNeed(STONE_NEED_INDEX, STONE_NEED);
        brain->setNeed(FEMALE_STONE_NEED_INDEX, FEMALE_STONE_NEED);
    }

    if (sensors[Bird::CURRENT_OBJECT_SENSOR] == OBJECT::EGG)
    {
        brain->setNeed(GOTO_FOREST_NEED_INDEX, 0.0);
        brain->setNeed(FIND_MOUSE_NEED_INDEX, 0.0);
        brain->setNeed(GET_MOUSE_NEED_INDEX, 0.0);
        brain->setNeed(MOUSE_NEED_INDEX, 0.0);
        brain->setNeed(FEMALE_MOUSE_NEED_INDEX, 0.0);
        brain->setNeed(STONE_NEED_INDEX, 0.0);
        brain->setNeed(FEMALE_STONE_NEED_INDEX, 0.0);
    }
}

// Set response override.
void Male::setResponseOverride()
{
    brain->responseOverride = response;
}

// Cycle male.
int Male::cycle()
{
   vector<Mona::SENSOR> brainSensors(NUM_SENSORS);
   for (int i = 0; i < NUM_SENSORS; i++)
   {
      brainSensors[i] = (Mona::SENSOR)sensors[i];
   }
   response = brain->cycle(brainSensors);
   return response;
}

// Post response.
void Male::postResponse()
{
    brain->postResponse(orientation, x, y);
}

// Load.
void Male::load(char* filename)
{
    FILE* fp;
    if ((fp = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Cannot load male");
        exit(1);
    }
    brain->load(fp);
    fclose(fp);
}

// Save.
void Male::save(char* filename)
{
    FILE* fp;
    if ((fp = fopen(filename, "w")) == NULL)
    {
        fprintf(stderr, "Cannot save male");
        exit(1);
    }
    brain->save(fp);
    fclose(fp);
}

// Print male.
void Male::print()
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

   case MATE_PROXIMITY_LEFT_FRONT:
       printf("LEFT_FRONT");
       break;

   case MATE_PROXIMITY_FRONT:
      printf("FRONT");
      break;

   case MATE_PROXIMITY_RIGHT_FRONT:
       printf("RIGHT_FRONT");
       break;

   case MATE_PROXIMITY_RIGHT:
      printf("RIGHT");
      break;

   case MATE_PROXIMITY_RIGHT_REAR:
       printf("RIGHT_REAR");
       break;

   case MATE_PROXIMITY_REAR:
       printf("REAR");
       break;

   case MATE_PROXIMITY_LEFT_REAR:
       printf("LEFT_REAR");
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

// Print needs.
void Male::printNeeds()
{
    for (int i = 0; i < NUM_NEEDS; i++)
    {
        switch (i)
        {
        case GOTO_FOREST_NEED_INDEX:
            printf("[Go to forest: %f],", brain->getNeed(GOTO_FOREST_NEED_INDEX));
            break;

        case FIND_MOUSE_NEED_INDEX:
            printf("[Find mouse: %f],", brain->getNeed(FIND_MOUSE_NEED_INDEX));
            break;

        case GET_MOUSE_NEED_INDEX:
            printf("[Get mouse: %f],", brain->getNeed(GET_MOUSE_NEED_INDEX));
            break;

        case MOUSE_NEED_INDEX:
            printf("[Mouse: %f],", brain->getNeed(MOUSE_NEED_INDEX));
            break;

        case FEMALE_MOUSE_NEED_INDEX:
            printf("[Female mouse: %f],", brain->getNeed(FEMALE_MOUSE_NEED_INDEX));
            break;

        case STONE_NEED_INDEX:
            printf("[Stone: %f],", brain->getNeed(STONE_NEED_INDEX));
            break;

        case FEMALE_STONE_NEED_INDEX:
            printf("[Female stone: %f]", brain->getNeed(FEMALE_STONE_NEED_INDEX));
            break;

        case ATTEND_FEMALE_NEED_INDEX:
            printf("[Attend female: %f]", brain->getNeed(ATTEND_FEMALE_NEED_INDEX));
            break;
        }
    }
}

// Print response.
void Male::printResponse()
{
   printf(RESPONSE::toString(response));
}

// Load mask.
void Male::loadMask(vector<bool>& mask,
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
    bool hasObject, bool state,
    bool mateProximity, bool wantMouse, bool wantStone)
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
    mask.push_back(mateProximity);
    mask.push_back(wantMouse);
    mask.push_back(wantStone);
}

// Load sensors.
void Male::loadSensors(vector<Mona::SENSOR>& sensors,
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
    Mona::SENSOR hasObject, Mona::SENSOR state,
    Mona::SENSOR mateProximity, Mona::SENSOR wantMouse, Mona::SENSOR wantStone)
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
    sensors.push_back(mateProximity);
    sensors.push_back(wantMouse);
    sensors.push_back(wantStone);
}

