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
Mona::NEED Male::FEMALE_STONE_NEED         = strtod(MALE_DEFAULT_FEMALE_STONE_NEED, 0);
Mona::NEED Male::ATTEND_FEMALE_NEED = strtod(MALE_DEFAULT_ATTEND_FEMALE_NEED, 0);

extern int RANDOM_NUMBER_SEED;

// Construct male bird.
Male::Male()
{
    // Initialize state.
    x = y = 0;
    orientation = ORIENTATION::NORTH;
    food = 0;
    hasObject = OBJECT::NO_OBJECT;
    flying = false;

    // Create Mona bird brain.
    brain = new Mona();
    brain->MAX_MEDIATOR_LEVEL = 0;
    brain->initNet(NUM_SENSORS, 0, NUM_NEEDS, RANDOM_NUMBER_SEED);

    // Motors:
    Mona::Motor* doNothing = brain->newMotor();
    Mona::Motor* move = brain->newMovementMotor(RESPONSE::MOVE_FORWARD);
    Mona::Motor* turnRight = brain->newMovementMotor(RESPONSE::TURN_RIGHT);
    Mona::Motor* turnLeft = brain->newMovementMotor(RESPONSE::TURN_LEFT);
    Mona::Motor* turnAround = brain->newMovementMotor(RESPONSE::TURN_AROUND);
    Mona::Motor* eat = brain->newMotor();
    Mona::Motor* get = brain->newMotor();
    Mona::Motor* put = brain->newMotor();
    Mona::Motor* toss = brain->newMotor();
    Mona::Motor* giveMouse = brain->newMotor();
    Mona::Motor* giveStone = brain->newMotor();
    Mona::Motor* fly = brain->newMovementMotor(Mona::MOVEMENT_TYPE::BEGIN);
    Mona::Motor* alight = brain->newMovementMotor(Mona::MOVEMENT_TYPE::END);

    // Needs.
    initNeeds();

    // Sensor modes:
    
    // Remove base sensor mode.
    brain->sensorModes.erase(brain->sensorModes.begin());

    // Hunger sensor mode to obtain mouse for male.
    vector<bool> mask;
    loadMask(mask, true, true, false, false, true, true, true, false, false);
    brain->addSensorMode(mask);
    int hungerMode = 0;

    // Attend female mode.
    loadMask(mask, false, false, false, true, false, false, true, false, false);
    brain->addSensorMode(mask);
    int attendFemaleMode = 1;

    // Female hunger sensor mode to obtain mouse for female.
    loadMask(mask, true, true, false, true, false, true, true, true, false);
    brain->addSensorMode(mask);
    int femaleHungerMode = 2;

    // Female want stone sensor mode to obtain stone for female.
    loadMask(mask, true, false, true, true, false, true, true, false, true);
    brain->addSensorMode(mask);
    int femaleStoneMode = 3;

    // Goals:

    // Hunger goal.
    vector<Mona::SENSOR> sensors;
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
        0.0, (Mona::SENSOR)OBJECT::NO_OBJECT, 0.0, DONT_CARE, DONT_CARE);
    int hungerGoal = brain->addGoal(MOUSE_NEED_INDEX, sensors, hungerMode, MOUSE_NEED);

    // Attend female goal.
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, PROXIMITY::PRESENT,
        DONT_CARE, DONT_CARE, 0.0, DONT_CARE, DONT_CARE);
    int attendFemaleGoal = brain->addGoal(ATTEND_FEMALE_NEED_INDEX, sensors, attendFemaleMode, ATTEND_FEMALE_NEED);

    // Female hunger goal.
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, PROXIMITY::PRESENT,
        DONT_CARE, DONT_CARE, 0.0, 0.0, DONT_CARE);
    int femaleHungerGoal = brain->addGoal(FEMALE_MOUSE_NEED_INDEX, sensors, femaleHungerMode, FEMALE_MOUSE_NEED);

    // Female want stone goal.
    loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, PROXIMITY::PRESENT,
        DONT_CARE, DONT_CARE, 0.0, DONT_CARE, 0.0);
    int femaleStoneGoal = brain->addGoal(FEMALE_STONE_NEED_INDEX, sensors, femaleStoneMode, FEMALE_STONE_NEED);

    // Set initial response.
    response = RESPONSE::DO_NOTHING;
}

// Initialize male needs.
void Male::initNeeds()
{
    brain->setNeed(MOUSE_NEED_INDEX, 0.0);
    brain->setNeed(FEMALE_MOUSE_NEED_INDEX, 0.0);
    brain->setNeed(FEMALE_STONE_NEED_INDEX, 0.0);
    brain->setNeed(FEMALE_STONE_NEED_INDEX, 0.0);
    brain->setNeed(ATTEND_FEMALE_NEED_INDEX, ATTEND_FEMALE_NEED);
}

// Set male needs.
void Male::setNeeds()
{
    if (!flying && sensors[FEMALE_PROXIMITY_SENSOR] == 1)
    {
        if (brain->getNeed(MOUSE_NEED_INDEX) == 0.0 &&
            brain->getNeed(FEMALE_MOUSE_NEED_INDEX) == 0.0 &&
            brain->getNeed(FEMALE_STONE_NEED_INDEX) == 0.0)
        {
            if (food == 0)
            {
                brain->setNeed(MOUSE_NEED_INDEX, MOUSE_NEED);
            }
            else if (sensors[Male::WANT_MOUSE_SENSOR] == 1)
            {
                brain->setNeed(FEMALE_MOUSE_NEED_INDEX, FEMALE_MOUSE_NEED);
            }
            else if (sensors[Male::WANT_STONE_SENSOR] == 1)
            {
                brain->setNeed(FEMALE_STONE_NEED_INDEX, FEMALE_STONE_NEED);
            }
        }
    }
    brain->setNeed(ATTEND_FEMALE_NEED_INDEX, ATTEND_FEMALE_NEED);
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
   response = brain->cycle(brainSensors, orientation, x, y);

   // Set random response to do nothing.
   if (brain->getNeed(MOUSE_NEED_INDEX) == 0.0 &&
       brain->getNeed(FEMALE_MOUSE_NEED_INDEX) == 0.0 &&
       brain->getNeed(FEMALE_STONE_NEED_INDEX) == 0.0 &&
       brain->getNeed(FEMALE_STONE_NEED_INDEX) == 0.0 &&
       brain->getNeed(ATTEND_FEMALE_NEED_INDEX) == 0.0)
   {
       response = brain->response = RESPONSE::DO_NOTHING;
   }

   if (response == RESPONSE::FLY)
   {
       flying = true;
   }
   else if (response == RESPONSE::ALIGHT)
   {
       flying = false;
   }
   return response;
}

// Digest food.
void Male::digest()
{
    if (food > 0)
    {
        food--;
    }
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
   printf("Locale: ");
      printf("%s", LOCALE::toString(sensors[LOCALE_SENSOR]));
      printf(", Mouse proximity: ");
      printf("%s", PROXIMITY::toString(sensors[MOUSE_PROXIMITY_SENSOR]));
      printf(", Stone proximity: ");
      printf("%s", PROXIMITY::toString(sensors[STONE_PROXIMITY_SENSOR]));
      printf(", Female proximity: ");
   printf("%s", PROXIMITY::toString(sensors[FEMALE_PROXIMITY_SENSOR]));
   printf(", Want mouse sensor: ");
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

// Print state.
void Male::printState()
{
    printf("Food: %d", food);
    printf(", Has_object: %s", OBJECT::toString(hasObject));
    if (sensors[FLYING_SENSOR])
    {
        printf(", Flying: true");
    }
    else {
        printf(", Flying: false");
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

        case FEMALE_STONE_NEED_INDEX:
            printf("[Female stone: %f],", brain->getNeed(FEMALE_STONE_NEED_INDEX));
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
    bool currentLocale,
    bool mouseProximity, bool stoneProximity, bool femaleProximity,
    bool hunger, bool hasObject, bool flying,
    bool wantMouse, bool wantStone)
{
    mask.clear();
    mask.push_back(currentLocale);
    mask.push_back(mouseProximity);
    mask.push_back(stoneProximity);
    mask.push_back(femaleProximity);
    mask.push_back(hunger);
    mask.push_back(hasObject);
    mask.push_back(flying);
    mask.push_back(wantMouse);
    mask.push_back(wantStone);
}

// Load sensors.
void Male::loadSensors(vector<Mona::SENSOR>& sensors,
    Mona::SENSOR currentLocale,
    Mona::SENSOR mouseProximity, Mona::SENSOR stoneProximity, Mona::SENSOR femaleProximity,
    Mona::SENSOR hunger, Mona::SENSOR hasObject, Mona::SENSOR flying, 
    Mona::SENSOR wantMouse, Mona::SENSOR wantStone)
{
    sensors.clear();
    sensors.push_back(currentLocale);
    sensors.push_back(mouseProximity);
    sensors.push_back(stoneProximity);
    sensors.push_back(femaleProximity);
    sensors.push_back(hunger);
    sensors.push_back(hasObject);
    sensors.push_back(flying);
    sensors.push_back(wantMouse);
    sensors.push_back(wantStone);
}

