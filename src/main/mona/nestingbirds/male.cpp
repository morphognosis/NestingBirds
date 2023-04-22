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
Mona::NEED Male::FEMALE_STONE_NEED  = strtod(MALE_DEFAULT_FEMALE_STONE_NEED, 0);
Mona::NEED Male::ATTEND_FEMALE_NEED = strtod(MALE_DEFAULT_ATTEND_FEMALE_NEED, 0);

extern int RANDOM_NUMBER_SEED;

// Construct male bird.
Male::Male()
{
   // Initialize state.
   x           = y = 0;
   orientation = ORIENTATION::NORTH;
   food        = 0;
   goal        = GOAL::ATTEND_FEMALE;
   hasObject   = OBJECT::NO_OBJECT;
   flying      = false;
   trainFlying = false;
   Verbose     = false;

   // Create Mona bird brain.
   brain = new Mona();
   brain->MAX_MEDIATOR_LEVEL = 0;
   brain->initNet(NUM_SENSORS, 0, NUM_NEEDS, RANDOM_NUMBER_SEED);

   // Motors:
   Mona::Motor *doNothing  = brain->newMotor();
   Mona::Motor *move       = brain->newMovementMotor(RESPONSE::MOVE_FORWARD);
   Mona::Motor *turnRight  = brain->newMovementMotor(RESPONSE::TURN_RIGHT);
   Mona::Motor *turnLeft   = brain->newMovementMotor(RESPONSE::TURN_LEFT);
   Mona::Motor *turnAround = brain->newMovementMotor(RESPONSE::TURN_AROUND);
   Mona::Motor *eat        = brain->newMotor();
   Mona::Motor *get        = brain->newMotor();
   Mona::Motor *put        = brain->newMotor();
   Mona::Motor *toss       = brain->newMotor();
   Mona::Motor *giveMouse  = brain->newMotor();
   Mona::Motor *giveStone  = brain->newMotor();
   Mona::Motor *fly        = brain->newMovementMotor(Mona::MOVEMENT_TYPE::BEGIN);
   Mona::Motor *alight     = brain->newMovementMotor(Mona::MOVEMENT_TYPE::END);

   // Needs.
   initNeeds();

   // Goals:

   // Food goals.
   vector<Mona::SENSOR> sensors;
   loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
               (Mona::SENSOR)GOAL::MOUSE_FOR_FEMALE, (Mona::SENSOR)OBJECT::NO_OBJECT, 0.0, DONT_CARE, DONT_CARE);
   brain->addGoal(MOUSE_NEED_INDEX, sensors, 0, MOUSE_NEED);
   loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
               (Mona::SENSOR)GOAL::STONE_FOR_FEMALE, (Mona::SENSOR)OBJECT::NO_OBJECT, 0.0, DONT_CARE, DONT_CARE);
   brain->addGoal(MOUSE_NEED_INDEX, sensors, 0, MOUSE_NEED);
   loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
               (Mona::SENSOR)GOAL::ATTEND_FEMALE, (Mona::SENSOR)OBJECT::NO_OBJECT, 0.0, DONT_CARE, DONT_CARE);
   brain->addGoal(MOUSE_NEED_INDEX, sensors, 0, MOUSE_NEED);

   // Attend female goal.
   loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, PROXIMITY::PRESENT,
               DONT_CARE, DONT_CARE, 0.0, DONT_CARE, DONT_CARE);
   brain->addGoal(ATTEND_FEMALE_NEED_INDEX, sensors, 0, ATTEND_FEMALE_NEED);

   // Female want mouse goal.
   loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, PROXIMITY::PRESENT,
               DONT_CARE, DONT_CARE, 0.0, 0.0, 0.0);
   brain->addGoal(FEMALE_MOUSE_NEED_INDEX, sensors, 0, FEMALE_MOUSE_NEED);

   // Female want stone goal.
   loadSensors(sensors, DONT_CARE, DONT_CARE, DONT_CARE, PROXIMITY::PRESENT,
               DONT_CARE, DONT_CARE, 0.0, 0.0, 0.0);
   brain->addGoal(FEMALE_STONE_NEED_INDEX, sensors, 0, FEMALE_STONE_NEED);

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
   brain->setNeed(ATTEND_FEMALE_NEED_INDEX, 0.0, ATTEND_FEMALE_NEED);
}


// Set male needs.
void Male::setNeeds()
{
   if (!flying && (sensors[FEMALE_PROXIMITY_SENSOR] == 1))
   {
      if ((brain->getNeed(MOUSE_NEED_INDEX) == 0.0) &&
          (brain->getNeed(FEMALE_MOUSE_NEED_INDEX) == 0.0) &&
          (brain->getNeed(FEMALE_STONE_NEED_INDEX) == 0.0))
      {
         if (food == 0)
         {
            brain->setNeed(MOUSE_NEED_INDEX, MOUSE_NEED);
         }
         else if (sensors[Male::FEMALE_WANTS_MOUSE_SENSOR] == 1)
         {
            brain->setNeed(FEMALE_MOUSE_NEED_INDEX, FEMALE_MOUSE_NEED);
         }
         else if (sensors[Male::FEMALE_WANTS_STONE_SENSOR] == 1)
         {
            brain->setNeed(FEMALE_STONE_NEED_INDEX, FEMALE_STONE_NEED);
         }
      }
   }
   if (brain->getNeed(MOUSE_NEED_INDEX) > 0.0)
   {
      goal = GOAL::EAT_MOUSE;
   }
   else if (brain->getNeed(FEMALE_MOUSE_NEED_INDEX) > 0.0)
   {
      goal = GOAL::MOUSE_FOR_FEMALE;
   }
   else if (brain->getNeed(FEMALE_STONE_NEED_INDEX) > 0.0)
   {
      goal = GOAL::STONE_FOR_FEMALE;
   }
   else
   {
      goal = GOAL::ATTEND_FEMALE;
   }
   sensors[Male::GOAL_SENSOR] = goal;
}


// Set response override.
void Male::setResponseOverride()
{
   if (response != brain->response)
   {
      for (int i = 0, j = (int)brain->motors.size(); i < j; i++)
      {
         Mona::Motor *motor = brain->motors[i];
         if (motor->response == response)
         {
            motor->firingStrength = 1.0;
         }
         else
         {
            motor->firingStrength = 0.0;
         }
      }
      brain->activePlaceMotor = NULL;
      for (int i = 0, j = (int)brain->placeMotors.size(); i < j; i++)
      {
         Mona::Motor *motor = brain->placeMotors[i];
         motor->firingStrength = 0.0;
      }
      brain->response = response;
   }
   if (response == RESPONSE::FLY)
   {
      trainFlying = true;
   }
   else if (response == RESPONSE::ALIGHT)
   {
      trainFlying = false;
   }
   flying = trainFlying;
   if (Verbose)
   {
      printf("Male training response: %s, flying=", RESPONSE::toString(response));
      if (flying)
      {
         printf("true\n");
      }
      else
      {
         printf("false\n");
      }
   }
}


// Cycle male.
int Male::cycle()
{
   if (Verbose)
   {
      printState();
   }

   vector<Mona::SENSOR> brainSensors(NUM_SENSORS);

   for (int i = 0; i < NUM_SENSORS; i++)
   {
      brainSensors[i] = (Mona::SENSOR)sensors[i];
   }
   response = brain->cycle(brainSensors, orientation, x, y);
   if (response == RESPONSE::FLY)
   {
      flying = true;
   }
   else if (response == RESPONSE::ALIGHT)
   {
      flying = false;
   }

   if (Verbose)
   {
      printf(", ");
      printResponse();
      printf("\n");
   }

   return(response);
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
void Male::load(char *filename)
{
   FILE *fp;

   if ((fp = fopen(filename, "r")) == NULL)
   {
      fprintf(stderr, "Cannot load male");
      exit(1);
   }
   brain->load(fp);
   fclose(fp);
}


// Save.
void Male::save(char *filename)
{
   FILE *fp;

   if ((fp = fopen(filename, "w")) == NULL)
   {
      fprintf(stderr, "Cannot save male");
      exit(1);
   }
   brain->save(fp);
   fclose(fp);
}


// Print state.
void Male::printState(FILE *fp)
{
   printSensors(fp);
   fprintf(fp, ", Food: %d, ", food);
   printNeeds(fp);
   printResponse(fp);
}


// Print sensors.
void Male::printSensors(FILE *fp)
{
   fprintf(fp, "Sensors: [");
   fprintf(fp, "Locale: ");
   fprintf(fp, "%s", LOCALE::toString(sensors[LOCALE_SENSOR]));
   fprintf(fp, ", Mouse proximity: ");
   fprintf(fp, "%s", PROXIMITY::toString(sensors[MOUSE_PROXIMITY_SENSOR]));
   fprintf(fp, ", Stone proximity: ");
   fprintf(fp, "%s", PROXIMITY::toString(sensors[STONE_PROXIMITY_SENSOR]));
   fprintf(fp, ", Female proximity: ");
   fprintf(fp, "%s", PROXIMITY::toString(sensors[FEMALE_PROXIMITY_SENSOR]));
   fprintf(fp, ", Goal: ");
   fprintf(fp, "%s", GOAL::toString(sensors[GOAL_SENSOR]));
   fprintf(fp, ", Has object: ");
   fprintf(fp, "%s", OBJECT::toString(sensors[HAS_OBJECT_SENSOR]));
   fprintf(fp, ", Flying: ");
   if (sensors[FLYING_SENSOR] == 1)
   {
       fprintf(fp, "true");
   }
   else
   {
       fprintf(fp, "false");
   }
   fprintf(fp, ", Female wants mouse: ");
   if (sensors[FEMALE_WANTS_MOUSE_SENSOR] == 1)
   {
       fprintf(fp, "true");
   }
   else
   {
       fprintf(fp, "false");
   }
   fprintf(fp, ", Female wants stone: ");
   if (sensors[FEMALE_WANTS_STONE_SENSOR] == 1)
   {
       fprintf(fp, "true");
   }
   else
   {
       fprintf(fp, "false");
   }
   fprintf(fp, "]");
}


// Print needs.
void Male::printNeeds(FILE *fp)
{
   fprintf(fp, "Needs: [");
   for (int i = 0; i < NUM_NEEDS; i++)
   {
      switch (i)
      {
      case MOUSE_NEED_INDEX:
         fprintf(fp, "Mouse: %f, ", brain->getNeed(MOUSE_NEED_INDEX));
         break;

      case FEMALE_MOUSE_NEED_INDEX:
         fprintf(fp, "Female mouse: %f, ", brain->getNeed(FEMALE_MOUSE_NEED_INDEX));
         break;

      case FEMALE_STONE_NEED_INDEX:
         fprintf(fp, "Female stone: %f, ", brain->getNeed(FEMALE_STONE_NEED_INDEX));
         break;

      case ATTEND_FEMALE_NEED_INDEX:
         fprintf(fp, "Attend female: %f", brain->getNeed(ATTEND_FEMALE_NEED_INDEX));
         break;
      }
   }
   fprintf(fp, "]");
}


// Print response.
void Male::printResponse(FILE *fp)
{
    fprintf(fp, "Response: %s", RESPONSE::toString(response));
}


// Load mask.
void Male::loadMask(vector<bool>& mask,
                    bool currentLocale,
                    bool mouseProximity, bool stoneProximity, bool femaleProximity,
                    bool goal, bool hasObject, bool flying,
                    bool wantMouse, bool wantStone)
{
   mask.clear();
   mask.push_back(currentLocale);
   mask.push_back(mouseProximity);
   mask.push_back(stoneProximity);
   mask.push_back(femaleProximity);
   mask.push_back(goal);
   mask.push_back(hasObject);
   mask.push_back(flying);
   mask.push_back(wantMouse);
   mask.push_back(wantStone);
}


// Load sensors.
void Male::loadSensors(vector<Mona::SENSOR>& sensors,
                       Mona::SENSOR currentLocale,
                       Mona::SENSOR mouseProximity, Mona::SENSOR stoneProximity, Mona::SENSOR femaleProximity,
                       Mona::SENSOR goal, Mona::SENSOR hasObject, Mona::SENSOR flying,
                       Mona::SENSOR wantMouse, Mona::SENSOR wantStone)
{
   sensors.clear();
   sensors.push_back(currentLocale);
   sensors.push_back(mouseProximity);
   sensors.push_back(stoneProximity);
   sensors.push_back(femaleProximity);
   sensors.push_back(goal);
   sensors.push_back(hasObject);
   sensors.push_back(flying);
   sensors.push_back(wantMouse);
   sensors.push_back(wantStone);
}
