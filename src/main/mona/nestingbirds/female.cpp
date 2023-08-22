// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The female bird.

#include "female.hpp"

#define SENSOR_DISCRIMINATION 1

// Food.
int Female:: FOOD_DURATION        = atoi(FEMALE_DEFAULT_FOOD_DURATION);
int Female:: INITIAL_FOOD         = atoi(FEMALE_DEFAULT_INITIAL_FOOD);
bool Female::RANDOMIZE_FOOD_LEVEL = false;

// Needs.
Mona::NEED Female::LAY_EGG_NEED   = strtod(FEMALE_DEFAULT_LAY_EGG_NEED, 0);
Mona::NEED Female::BROOD_EGG_NEED = strtod(FEMALE_DEFAULT_BROOD_EGG_NEED, 0);
Mona::NEED Female::MOUSE_NEED     = strtod(FEMALE_DEFAULT_MOUSE_NEED, 0);

extern int RANDOM_NUMBER_SEED;

// Construct female bird.
Female::Female()
{
   // Initialize state.
   x           = y = 0;
   orientation = ORIENTATION::NORTH;
   food        = 0;
   goal        = GOAL::EAT_MOUSE;
   hasObject   = OBJECT::NO_OBJECT;
   Verbose     = false;

   // Create Mona bird brain.
   brain = new Mona(NUM_SENSORS, NUM_NEEDS, RANDOM_NUMBER_SEED);

   // Motors:
   Mona::Motor *doNothing = brain->newMotor();
   Mona::Motor *move      = brain->newMotor();
   Mona::Motor *turnRight = brain->newMotor();
   Mona::Motor *turnLeft  = brain->newMotor();
   Mona::Motor *eat       = brain->newMotor();
   Mona::Motor *get       = brain->newMotor();
   Mona::Motor *put       = brain->newMotor();
   Mona::Motor *toss      = brain->newMotor();
   Mona::Motor *wantMouse = brain->newMotor();
   Mona::Motor *wantStone = brain->newMotor();
   Mona::Motor *layEgg    = brain->newMotor();

   // Needs.
   initNeeds();

   // Goals:

   // Food goal.
   vector<bool> mask;
   loadMask(mask, false, false, false, false,
            false, false, false, false, false, false, true, true);
#ifdef SENSOR_DISCRIMINATION
   int eatMouseMode = 0;
   int mouseDiscriminator = brain->addSensorDiscriminator(mask);
#else
   int eatMouseMode = brain->addSensorMode(mask);
#endif
   vector<Mona::SENSOR> eatMouseSensors;
   loadSensors(eatMouseSensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
               DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
               DONT_CARE, (Mona::SENSOR)GOAL::EAT_MOUSE, (Mona::SENSOR)OBJECT::MOUSE);
   int eatMouseGoal = brain->addGoal(MOUSE_NEED_INDEX, eatMouseSensors, eatMouseMode, eat, MOUSE_NEED);

   // Lay egg goal.
   vector<Mona::SENSOR> sensors;
   loadSensors(sensors, (Mona::SENSOR)OBJECT::NO_OBJECT, (Mona::SENSOR)OBJECT::STONE, (Mona::SENSOR)OBJECT::STONE,
               (Mona::SENSOR)OBJECT::STONE, (Mona::SENSOR)OBJECT::STONE, (Mona::SENSOR)OBJECT::STONE, (Mona::SENSOR)OBJECT::STONE,
               (Mona::SENSOR)OBJECT::STONE, (Mona::SENSOR)OBJECT::STONE,
               (Mona::SENSOR)ORIENTATION::SOUTH, (Mona::SENSOR)GOAL::LAY_EGG, DONT_CARE);
   int layEggGoal = brain->addGoal(LAY_EGG_NEED_INDEX, sensors, 0, layEgg, LAY_EGG_NEED);
   
   // Brooding egg.
   loadSensors(sensors, (Mona::SENSOR)OBJECT::EGG, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
               DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
               DONT_CARE, (Mona::SENSOR)GOAL::BROOD_EGG, DONT_CARE);
   int broodEggGoal = brain->addGoal(BROOD_EGG_NEED_INDEX, sensors, 0, doNothing, BROOD_EGG_NEED);

#ifdef SENSOR_DISCRIMINATION
   // Add sensor discriminator goal receptors.
   brain->newSensorDiscriminatorReceptor(eatMouseSensors, mouseDiscriminator);
   vector<Mona::SENSOR> wantMouseSensors;
   loadSensors(wantMouseSensors, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
       DONT_CARE, DONT_CARE, DONT_CARE, DONT_CARE,
       DONT_CARE, (Mona::SENSOR)GOAL::EAT_MOUSE, (Mona::SENSOR)OBJECT::NO_OBJECT);
   brain->newSensorDiscriminatorReceptor(wantMouseSensors, mouseDiscriminator);
#endif

   // Set initial response.
   response = RESPONSE::DO_NOTHING;
}

// Destroy female bird.
Female::~Female()
{
    delete brain;
}

// Initialize female needs.
void Female::initNeeds()
{
   brain->setNeed(MOUSE_NEED_INDEX, 0.0);
   brain->setNeed(LAY_EGG_NEED_INDEX, LAY_EGG_NEED);
   brain->setNeed(BROOD_EGG_NEED_INDEX, 0.0);
}


// Set female needs.
void Female::setNeeds()
{
   if ((food == 0) && ((response == Female::RESPONSE::PUT_OBJECT) ||
                       (sensors[CURRENT_OBJECT_SENSOR] == OBJECT::EGG)))
   {
      brain->setNeed(MOUSE_NEED_INDEX, MOUSE_NEED);
   }
   else if (sensors[CURRENT_OBJECT_SENSOR] == OBJECT::EGG)
   {
      brain->setNeed(LAY_EGG_NEED_INDEX, 0.0);
      brain->setNeed(BROOD_EGG_NEED_INDEX, BROOD_EGG_NEED);
   }
   for (int i = 0; i < NUM_NEEDS; i++)
   {
      if ((i == 0) || (brain->getNeed(i) > brain->getNeed(goal)))
      {
         goal = i;
      }
   }
   sensors[Female::GOAL_SENSOR] = goal;
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
   return(response);
}


// Digest food.
void Female::digest()
{
   if (food > 0)
   {
      food--;
   }
}


// Load.
void Female::load(char *filename)
{
   FILE *fp;

   if ((fp = fopen(filename, "r")) == NULL)
   {
      fprintf(stderr, "Cannot load female\n");
      exit(1);
   }
   brain->load(fp);
   fclose(fp);
}


// Save.
void Female::save(char *filename)
{
   FILE *fp;

   if ((fp = fopen(filename, "w")) == NULL)
   {
      fprintf(stderr, "Cannot save female\n");
      exit(1);
   }
   brain->save(fp);
   fclose(fp);
}


// Print state.
void Female::printState(FILE *fp)
{
   printSensors(fp);
   fprintf(fp, ", \"Food\": %d, ", food);
   printNeeds(fp);
}


// Print sensors.
void Female::printSensors(FILE *fp)
{
   fprintf(fp, "\"Sensors\": { \"Cell sensors\": { ");

   for (int i = 0; i < NUM_CELL_SENSORS; i++)
   {
      switch (i)
      {
      case 0:
         fprintf(fp, "\"Current\": ");
         break;

      case 1:
         fprintf(fp, "\"Left\": ");
         break;

      case 2:
         fprintf(fp, "\"Left front\": ");
         break;

      case 3:
         fprintf(fp, "\"Front\": ");
         break;

      case 4:
         fprintf(fp, "\"Right front\": ");
         break;

      case 5:
         fprintf(fp, "\"Right\": ");
         break;

      case 6:
         fprintf(fp, "\"Right rear\": ");
         break;

      case 7:
         fprintf(fp, "\"Rear\": ");
         break;

      case 8:
         fprintf(fp, "\"Left rear\": ");
         break;
      }
      fprintf(fp, "\"%s\"", OBJECT::toString(sensors[i]));
      if (i < NUM_CELL_SENSORS - 1)
      {
         fprintf(fp, ", ");
      }
   }
   fprintf(fp, " }");
   fprintf(fp, ", \"Orientation\": \"%s\"", ORIENTATION::toString(orientation));
   fprintf(fp, ", \"Goal\": \"%s\"", GOAL::toString(sensors[GOAL_SENSOR]));
   fprintf(fp, ", \"Has object\": \"%s\"", OBJECT::toString(hasObject));
   fprintf(fp, " }");
}


// Print needs.
void Female::printNeeds(FILE *fp)
{
   fprintf(fp, "\"Needs\": { ");
   for (int i = 0; i < NUM_NEEDS; i++)
   {
      switch (i)
      {
      case LAY_EGG_NEED_INDEX:
         fprintf(fp, "\"Lay egg\": %f, ", brain->getNeed(LAY_EGG_NEED_INDEX));
         break;

      case BROOD_EGG_NEED_INDEX:
         fprintf(fp, "\"Brood egg\": %f, ", brain->getNeed(BROOD_EGG_NEED_INDEX));
         break;

      case MOUSE_NEED_INDEX:
         fprintf(fp, "\"Mouse\": %f", brain->getNeed(MOUSE_NEED_INDEX));
         break;
      }
   }
   fprintf(fp, " }");
}


// Print response.
void Female::printResponse(FILE *fp)
{
   fprintf(fp, "\"Response\": \"%s\"", RESPONSE::toString(response));
}


// Load mask.
void Female::loadMask(vector<bool>& mask,
                      bool          currentObject,
                      bool          leftObject,
                      bool          leftFrontObject,
                      bool          frontObject,
                      bool          rightFrontObject,
                      bool          rightObject,
                      bool          rightRearObject,
                      bool          rearObject,
                      bool          leftRearObject,
                      bool          orientation,
                      bool          goal,
                      bool          hasObject)
{
   mask.clear();
   mask.push_back(currentObject);
   mask.push_back(leftObject);
   mask.push_back(leftFrontObject);
   mask.push_back(frontObject);
   mask.push_back(rightFrontObject);
   mask.push_back(rightObject);
   mask.push_back(rightRearObject);
   mask.push_back(rearObject);
   mask.push_back(leftRearObject);
   mask.push_back(orientation);
   mask.push_back(goal);
   mask.push_back(hasObject);
}


// Load sensors.
void Female::loadSensors(vector<Mona::SENSOR>& sensors,
                         Mona::SENSOR          currentObject,
                         Mona::SENSOR          leftObject,
                         Mona::SENSOR          leftFrontObject,
                         Mona::SENSOR          frontObject,
                         Mona::SENSOR          rightFrontObject,
                         Mona::SENSOR          rightObject,
                         Mona::SENSOR          rightRearObject,
                         Mona::SENSOR          rearObject,
                         Mona::SENSOR          leftRearObject,
                         Mona::SENSOR          orientation,
                         Mona::SENSOR          goal,
                         Mona::SENSOR          hasObject)
{
   sensors.clear();
   sensors.push_back(currentObject);
   sensors.push_back(leftObject);
   sensors.push_back(leftFrontObject);
   sensors.push_back(frontObject);
   sensors.push_back(rightFrontObject);
   sensors.push_back(rightObject);
   sensors.push_back(rightRearObject);
   sensors.push_back(rearObject);
   sensors.push_back(leftRearObject);
   sensors.push_back(orientation);
   sensors.push_back(goal);
   sensors.push_back(hasObject);
}
