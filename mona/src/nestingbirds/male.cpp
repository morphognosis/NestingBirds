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
    Mona::Motor* doNothing = brain->motors[Bird::RESPONSE::DO_NOTHING];
    Mona::Motor* eat = brain->motors[Bird::RESPONSE::EAT_MOUSE];
    Mona::Motor* get = brain->motors[Bird::RESPONSE::GET_OBJECT];
    Mona::Motor* put = brain->motors[Bird::RESPONSE::PUT_OBJECT];
    Mona::Motor* toss = brain->motors[Bird::RESPONSE::TOSS_OBJECT];
    Mona::Motor* move = brain->motors[Bird::RESPONSE::MOVE_FORWARD];
    Mona::Motor* turnRight = brain->motors[Bird::RESPONSE::TURN_RIGHT];
    Mona::Motor* turnLeft = brain->motors[Bird::RESPONSE::TURN_LEFT];
    Mona::Motor* turnAround = brain->motors[Bird::RESPONSE::TURN_AROUND];
    Mona::Motor* stateOn = brain->motors[Bird::RESPONSE::STATE_ON];
    Mona::Motor* stateOff = brain->motors[Bird::RESPONSE::STATE_OFF];
    Mona::Motor* giveMouse = brain->motors[Male::RESPONSE::GIVE_MOUSE];
    Mona::Motor* giveStone = brain->motors[Male::RESPONSE::GIVE_STONE];

    // Needs.
    initNeeds();

    // Goals:

    // Food goals.
    vector<Mona::SENSOR> sensors;
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, 1.0, (Mona::SENSOR)OBJECT::MOUSE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE);
    int eatMouseGoal = brain->addGoal(MOUSE_NEED_INDEX, sensors, 0, Bird::RESPONSE::EAT_MOUSE, MOUSE_NEED);
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::MOUSE, DONT_CARE,
        MATE_PROXIMITY_PRESENT, 1.0, DONT_CARE);
    int giveMouseGoal = brain->addGoal(FEMALE_MOUSE_NEED_INDEX, sensors, 0, Male::RESPONSE::GIVE_MOUSE, FEMALE_MOUSE_NEED);
 
    // Stone goals.
    loadSensors(sensors, (Mona::SENSOR)LOCALE::DESERT, (Mona::SENSOR)OBJECT::STONE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::NO_OBJECT, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE);
    int getStoneGoal = brain->addGoal(STONE_NEED_INDEX, sensors, 0, Bird::RESPONSE::GET_OBJECT, STONE_NEED);
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, (Mona::SENSOR)OBJECT::STONE, DONT_CARE,
        MATE_PROXIMITY_PRESENT, DONT_CARE, 1.0);
    int giveStoneGoal = brain->addGoal(FEMALE_STONE_NEED_INDEX, sensors, 0, Male::RESPONSE::GIVE_STONE, FEMALE_STONE_NEED);

    // Attend female goal.
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        MATE_PROXIMITY_PRESENT, DONT_CARE, DONT_CARE);
    int attendFemaleGoal = brain->addGoal(ATTEND_FEMALE_NEED_INDEX, sensors, 0, Bird::RESPONSE::DO_NOTHING, ATTEND_FEMALE_NEED);

    // Set initial response.
    response = Bird::RESPONSE::DO_NOTHING;
}

// Initialize male needs.
void Male::initNeeds()
{
    brain->setNeed(MOUSE_NEED_INDEX, 0.0);
    brain->setNeed(FEMALE_MOUSE_NEED_INDEX, 0.0);
    brain->setNeed(STONE_NEED_INDEX, 0.0);
    brain->setNeed(FEMALE_STONE_NEED_INDEX, 0.0);
    brain->setNeed(ATTEND_FEMALE_NEED_INDEX, ATTEND_FEMALE_NEED);
}

// Set male needs.
void Male::setNeeds()
{
    if (food <= 0)
    {
        brain->setNeed(MOUSE_NEED_INDEX, MOUSE_NEED);
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

