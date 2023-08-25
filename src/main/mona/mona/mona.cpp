// For conditions of distribution and use, see copyright notice in mona.hpp

#include "mona.hpp"

// Version.
const char *MonaVersion = MONA_VERSION;

// Print version.
void
Mona::printVersion(FILE *out)
{
   fprintf(out, "%s\n", &MonaVersion[4]);
}


// Null id.
const Mona::ID Mona::NULL_ID = Homeostat::NULL_ID;

// Behavior cycle.
Mona::RESPONSE
Mona::cycle(vector<Mona::SENSOR>& sensors)
{
   // Input sensors.
   assert((int)sensors.size() == numSensors);
   this->sensors.clear();
   for (int i = 0; i < numSensors; i++)
   {
      this->sensors.push_back(sensors[i]);
   }

#ifdef MONA_TRACKING
   // Clear tracking activity.
   clearTracking();
#endif

   sense();
   enable();
   learn();
   drive();
   respond();

   return(response);
}


Mona::RESPONSE
Mona::cycle(vector<Mona::SENSOR>& sensors, int orientation, int x, int y)
{
   X = x;
   Y = y;
   this->orientation = orientation;
   return(cycle(sensors));
}


// Construct empty network.
Mona::Mona()
{
   clearVars();
   initParms();
}


// Construct network.
Mona::Mona(int numSensors, int numResponses, int numNeeds,
           RANDOM randomSeed)
{
   clearVars();
   initParms();
   initNet(numSensors, numResponses, numNeeds, randomSeed);
}


// Construct network with responses added later.
Mona::Mona(int numSensors, int numNeeds, RANDOM randomSeed)
{
   clearVars();
   initParms();
   initNet(numSensors, 0, numNeeds, randomSeed);
}


// Initialize parameters.
void Mona::initParms()
{
   MIN_ENABLEMENT = 0.05;
   assert(MIN_ENABLEMENT > NEARLY_ZERO);
   INITIAL_ENABLEMENT = 0.95;
   assert(INITIAL_ENABLEMENT >= MIN_ENABLEMENT);
   DRIVE_ATTENUATION = 0.0;
   MIN_DRIVE_MOTIVE  = 0.0;
   FIRING_STRENGTH_LEARNING_DAMPER   = 0.1;
   LEARNING_DECREASE_VELOCITY        = 0.01;
   LEARNING_INCREASE_VELOCITY        = 0.9;
   RESPONSE_RANDOMNESS               = 0.0;
   MIN_MOVEMENT_RESPONSE_PATH_LENGTH = 5;
   MAX_MOVEMENT_RESPONSE_PATH_LENGTH = 50;
   UTILITY_ASYMPTOTE = 10.0;
   DEFAULT_MAX_LEARNING_EFFECT_EVENT_INTERVAL = 2;
   DEFAULT_NUM_EFFECT_EVENT_INTERVALS         = 3;
   MAX_MEDIATORS      = 500;
   MAX_MEDIATOR_LEVEL = 0;
   MAX_RESPONSE_EQUIPPED_MEDIATOR_LEVEL   = 2;
   MIN_RESPONSE_UNEQUIPPED_MEDIATOR_LEVEL = 3;
   SENSOR_RESOLUTION                   = 0.0f;
   LEARN_MEDIATOR_GOAL_VALUE           = false;
   LEARN_MEDIATOR_GOAL_VALUE_MIN_LEVEL = 2;
   LEARN_RECEPTOR_GOAL_VALUE           = false;
}


// Initialize effect event intervals and weights.
void Mona::initEffectEventIntervals()
{
   // Create the intervals.
   effectEventIntervals.resize(MAX_MEDIATOR_LEVEL + 1);
   for (int i = 0; i <= MAX_MEDIATOR_LEVEL; i++)
   {
      effectEventIntervals[i].resize(DEFAULT_NUM_EFFECT_EVENT_INTERVALS);
      for (int j = 0; j < DEFAULT_NUM_EFFECT_EVENT_INTERVALS; j++)
      {
         effectEventIntervals[i][j] = (int)(pow(2.0, i)) * (j + 1);
      }
   }

   // Initialize the interval weights.
   initEffectEventIntervalWeights();
}


// Initialize effect event intervals and weights for specific level.
void Mona::initEffectEventInterval(int level, int numIntervals)
{
   // Create the intervals.
   effectEventIntervals[level].resize(numIntervals);
   for (int i = 0; i < numIntervals; i++)
   {
      effectEventIntervals[level][i] = (int)(pow(2.0, level)) * (i + 1);
   }

   // Create the interval weights.
   initEffectEventIntervalWeight(level);
}


// Initialize the effect event interval weights using
// the effect event intervals and a simple inverse proportional rule.
void Mona::initEffectEventIntervalWeights()
{
   effectEventIntervalWeights.resize((int)effectEventIntervals.size());
   for (int i = 0, j = (int)effectEventIntervalWeights.size(); i < j; i++)
   {
      initEffectEventIntervalWeight(i);
   }
}


// Initialize the effect event interval weights for a specific level.
void Mona::initEffectEventIntervalWeight(int level)
{
   WEIGHT sum;

   sum = 0.0;
   effectEventIntervalWeights[level].resize((int)effectEventIntervals[level].size());
   for (int i = 0, j = (int)effectEventIntervalWeights[level].size(); i < j; i++)
   {
      sum += (1.0 / (double)effectEventIntervals[level][i]);
   }
   for (int i = 0, j = (int)effectEventIntervalWeights[level].size(); i < j; i++)
   {
      effectEventIntervalWeights[level][i] =
         (1.0 / (double)effectEventIntervals[level][i]) / sum;
   }
}


// Initialize the maximum learning effect event intervals.
void Mona::initMaxLearningEffectEventIntervals()
{
   maxLearningEffectEventIntervals.clear();
   for (int i = 0, j = 0; i <= MAX_MEDIATOR_LEVEL; i++)
   {
      // Response-equipped mediators learn with immediate intervals.
      if (i <= MAX_RESPONSE_EQUIPPED_MEDIATOR_LEVEL)
      {
         j = (int)(pow(2.0, i));
      }
      else
      {
         j = (int)(pow(2.0, i)) * DEFAULT_MAX_LEARNING_EFFECT_EVENT_INTERVAL;
      }
      maxLearningEffectEventIntervals.push_back(j);
   }
}


// Audit default effect event intervals, weights, and learning intervals.
// Return false if invalid interval found.
bool Mona::auditDefaultEffectEventIntervals()
{
   WEIGHT sum;

   if (DEFAULT_NUM_EFFECT_EVENT_INTERVALS <= 0)
   {
      return(false);
   }

   if (DEFAULT_MAX_LEARNING_EFFECT_EVENT_INTERVAL <= 0)
   {
      return(false);
   }

   if (effectEventIntervals.size() != (MAX_MEDIATOR_LEVEL + 1))
   {
      return(false);
   }

   if (effectEventIntervals.size() != effectEventIntervalWeights.size())
   {
      return(false);
   }

   if (maxLearningEffectEventIntervals.size() != (MAX_MEDIATOR_LEVEL + 1))
   {
      return(false);
   }

   for (int i = 0; i <= MAX_MEDIATOR_LEVEL; i++)
   {
      if ((int)effectEventIntervals[i].size() != DEFAULT_NUM_EFFECT_EVENT_INTERVALS)
      {
         return(false);
      }
      for (int j = 0, k = (int)effectEventIntervals[i].size(); j < k; j++)
      {
         if (effectEventIntervals[i][j] < (int)(pow(2.0, i)))
         {
            return(false);
         }
      }
   }

   for (int i = 0, n = (int)effectEventIntervalWeights.size(); i < n; i++)
   {
      if ((int)effectEventIntervalWeights[i].size() !=
          DEFAULT_NUM_EFFECT_EVENT_INTERVALS)
      {
         return(false);
      }
      sum = 0.0;
      for (int j = 0, k = (int)effectEventIntervalWeights[i].size(); j < k; j++)
      {
         sum += effectEventIntervalWeights[i][j];
      }
      if (fabs(sum - 1.0) > NEARLY_ZERO)
      {
         return(false);
      }
   }

   for (int i = 0; i <= MAX_MEDIATOR_LEVEL; i++)
   {
      if (maxLearningEffectEventIntervals[i] < 0)
      {
         return(false);
      }
   }

   return(true);
}


// Mona initializer.
void Mona::initNet(int numSensors, int numResponses, int numNeeds,
                   RANDOM randomSeed)
{
   // Initialize effect event intervals.
   initEffectEventIntervals();

   // Initialize the maximum learning effect event intervals.
   initMaxLearningEffectEventIntervals();

   // Sanity checks.
   assert(numSensors > 0);
   assert(numResponses >= 0);
   assert(numNeeds > 0);
   assert(MAX_MOVEMENT_RESPONSE_PATH_LENGTH >= MIN_MOVEMENT_RESPONSE_PATH_LENGTH);
   assert((MAX_RESPONSE_EQUIPPED_MEDIATOR_LEVEL + 1) >=
          MIN_RESPONSE_UNEQUIPPED_MEDIATOR_LEVEL);
   assert((int)effectEventIntervals.size() == MAX_MEDIATOR_LEVEL + 1);
   assert((int)maxLearningEffectEventIntervals.size() == MAX_MEDIATOR_LEVEL + 1);
   assert(auditDefaultEffectEventIntervals());

   // Clear network.
   clear();

   // Save parameters.
   this->numSensors   = numSensors;
   this->numResponses = numResponses;
   this->numNeeds     = numNeeds;
   this->randomSeed   = randomSeed;

   // Set random seed.
   random.SRAND(randomSeed);

   // Initialize sensors.
   sensors.clear();
   for (int i = 0; i < numSensors; i++)
   {
      sensors.push_back(0.0f);
   }

   // Add base sensor mode.
   vector<bool> sensorMask;
   for (int i = 0; i < numSensors; i++)
   {
      sensorMask.push_back(true);
   }
   addSensorMode(sensorMask);

   // Initialize response apparatus and motor neurons.
   response = 0;
   int n = numResponses;
   numResponses = 0;
   for (int i = 0; i < n; i++)
   {
      newMotor();
   }
   responseOverride          = NULL_RESPONSE;
   responseOverridePotential = -1.0;

   // Create homeostats to manage needs.
   for (int i = 0; i < numNeeds; i++)
   {
      homeostats.push_back(new Homeostat(i, this));
      assert(homeostats[i] != NULL);
   }

   // Set default maximum cumulative motive.
   maxMotive = (MOTIVE)numNeeds;

   // Learning initialization.
   eventClock = 0;
   learningEvents.resize(MAX_MEDIATOR_LEVEL + 1);

#ifdef MONA_TRACE
   // Tracing.
   traceSense   = false;
   traceEnable  = false;
   traceLearn   = false;
   traceDrive   = false;
   traceRespond = false;
#endif
}


// Clear variables.
void Mona::clearVars()
{
   numSensors                = 0;
   numResponses              = 0;
   numNeeds                  = 0;
   responseOverride          = NULL_RESPONSE;
   responseOverridePotential = -1.0;
   randomSeed                = INVALID_RANDOM;
   idDispenser               = 0;
}


// Mona destructor.
Mona::~Mona()
{
   clear();
}


// Get need.
Mona::NEED
Mona::getNeed(int index)
{
   return(homeostats[index]->getNeed());
}


// Set need.
// Value must be in [0,1] interval.
void
Mona::setNeed(int index, NEED value)
{
   assert(value >= 0.0 && value <= 1.0);
   homeostats[index]->setNeed(value);
}


// Set need with default value.
// Value must be in [0,1] interval.
void
Mona::setNeed(int index, NEED value, NEED defaultValue)
{
   assert(value >= 0.0 && value <= 1.0);
   homeostats[index]->setNeed(value, defaultValue);
}


// Inflate need to maximum value.
void
Mona::inflateNeed(int index)
{
   NEED                currentNeed, deltaNeed, need;
   Mediator            *mediator;
   Enabling            *enabling;
   LearningEvent       *learningEvent;
   GeneralizationEvent *generalizationEvent;

   list<Mediator *>::iterator      mediatorItr;
   list<Enabling *>::iterator      enablingItr;
   list<LearningEvent *>::iterator learningEventItr;

   currentNeed = homeostats[index]->getNeed();
   deltaNeed   = 1.0 - currentNeed;
   homeostats[index]->setNeed(currentNeed + deltaNeed);
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      for (enablingItr = mediator->responseEnablings.enablings.begin();
           enablingItr != mediator->responseEnablings.enablings.end(); enablingItr++)
      {
         enabling = *enablingItr;
         need     = enabling->needs.get(index) + deltaNeed;
         enabling->needs.set(index, need);
      }
   }
   for (int i = 0, j = (int)learningEvents.size(); i < j; i++)
   {
      for (learningEventItr = learningEvents[i].begin();
           learningEventItr != learningEvents[i].end(); learningEventItr++)
      {
         learningEvent = *learningEventItr;
         need          = learningEvent->needs.get(index) + deltaNeed;
         learningEvent->needs.set(index, need);
      }
   }
   for (int i = 0, j = (int)generalizationEvents.size(); i < j; i++)
   {
      generalizationEvent = generalizationEvents[i];
      need = generalizationEvent->needs.get(index) + deltaNeed;
      generalizationEvent->needs.set(index, need);
   }
}


// Set periodic need.
void
Mona::setPeriodicNeed(int index, int frequency, NEED periodicNeed)
{
   assert(periodicNeed >= 0.0 && periodicNeed <= 1.0);
   homeostats[index]->setPeriodicNeed(frequency, periodicNeed);
}


// Clear periodic need.
void
Mona::clearPeriodicNeed(int index)
{
   homeostats[index]->clearPeriodicNeed();
}


// Print need.
void
Mona::printNeed(int index)
{
   homeostats[index]->print();
}


// Add a goal for a need.
int Mona::addGoal(int needIndex, vector<SENSOR>& sensors,
                  SENSOR_MODE sensorMode, Motor *motor, NEED goalValue)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   return(homeostats[needIndex]->addGoal(sensors, sensorMode, motor, goalValue));
}


// Add a goal for a need.
int Mona::addGoal(int needIndex, vector<SENSOR>& sensors,
                  SENSOR_MODE sensorMode, NEED goalValue)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   return(homeostats[needIndex]->addGoal(sensors, sensorMode, goalValue));
}


// Add goal for mediator.
int Mona::addGoal(int needIndex, Mediator *mediator, NEED goalValue)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   return(homeostats[needIndex]->addGoalMediator(mediator, goalValue));
}


// Find goal for need.
int Mona::findGoal(int needIndex, vector<SENSOR>& sensors,
                   SENSOR_MODE sensorMode, Motor *motor)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   return(homeostats[needIndex]->findGoal(sensors, sensorMode, motor));
}


// Find goal for need.
int Mona::findGoal(int needIndex, vector<SENSOR>& sensors,
                   SENSOR_MODE sensorMode)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   return(homeostats[needIndex]->findGoal(sensors, sensorMode));
}


// Get number of goals for need.
int Mona::getNumGoals(int needIndex)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   return((int)homeostats[needIndex]->goals.size());
}


// Get goal information for need.
bool Mona::getGoalInfo(int needIndex, int goalIndex,
                       vector<SENSOR>& sensors, int& sensorMode,
                       Motor **motor, NEED& goalValue, bool& enabled)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   return(homeostats[needIndex]->getGoalInfo(goalIndex, sensors,
                                             sensorMode, (void **)motor, goalValue, enabled));
}


// Get goal receptors for a need.
void Mona::getGoalReceptors(int needIndex, int goalIndex, vector<void *>& receptors)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   homeostats[needIndex]->getGoalReceptors(goalIndex, receptors);
}


// Get goal motor for a need.
Mona::Motor *Mona::getGoalMotor(int needIndex, int goalIndex)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   return((Mona::Motor *)homeostats[needIndex]->getGoalMotor(goalIndex));
}


// Is goal for need enabled?
bool Mona::isGoalEnabled(int needIndex, int goalIndex)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   return(homeostats[needIndex]->isGoalEnabled(goalIndex));
}


// Enable goal for need.
bool Mona::enableGoal(int needIndex, int goalIndex)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   return(homeostats[needIndex]->enableGoal(goalIndex));
}


// Disable goal for need.
bool Mona::disableGoal(int needIndex, int goalIndex)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   return(homeostats[needIndex]->disableGoal(goalIndex));
}


// Remove goal for need.
bool Mona::removeGoal(int needIndex, int goalIndex)
{
   assert(needIndex >= 0 && needIndex < (int)homeostats.size());
   return(homeostats[needIndex]->removeGoal(goalIndex));
}


// Initialize neuron.
void
Mona::Neuron::init(Mona *mona)
{
   clear();
   this->mona = mona;
   goals.init(mona->numNeeds, mona);
}


// Clear neuron.
void
Mona::Neuron::clear()
{
   id             = NULL_ID;
   creationTime   = INVALID_TIME;
   firingStrength = 0.0;
   goals.clear();
   motive      = 0.0;
   motiveValid = false;
   motiveWork.clear();
   motiveWorkValid = false;
   driveWeights.clear();
   instinct = false;
   for (int i = 0, j = (int)notifyList.size(); i < j; i++)
   {
      delete notifyList[i];
   }
   notifyList.clear();
#ifdef MONA_TRACKING
   tracker.fire   = false;
   tracker.enable = false;
   tracker.drive  = false;
#endif
}


// Does neuron have an instinct parent?
bool
Mona::Neuron::hasParentInstinct()
{
   Mediator *mediator;

   for (int i = 0, j = (int)notifyList.size(); i < j; i++)
   {
      mediator = notifyList[i]->mediator;
      if (mediator->instinct ||
          mediator->hasParentInstinct())
      {
         return(true);
      }
   }
   return(false);
}


// Load neuron.
void Mona::Neuron::load(FILE *fp)
{
   int           i, j;
   struct Notify *notify;

   clear();
   FREAD_LONG_LONG(&id, fp);
   FREAD_INT(&i, fp);
   type = (NEURON_TYPE)i;
   FREAD_LONG_LONG(&creationTime, fp);
   firingStrength = 0.0;
   goals.load(fp);
   FREAD_DOUBLE(&motive, fp);
   FREAD_BOOL(&instinct, fp);
   notifyList.clear();
   FREAD_INT(&i, fp);
   notifyList.resize(i);
   i = 0;
   for (int n = (int)notifyList.size(); i < n; i++)
   {
      notify = new struct Notify;
      assert(notify != NULL);
      notify->mediator = (Mediator *)new ID;
      assert(notify->mediator != NULL);
      FREAD_LONG_LONG((ID *)notify->mediator, fp);
      FREAD_INT(&j, fp);
      notify->eventType = (EVENT_TYPE)j;
      notifyList[i]     = notify;
   }
}


// Save neuron.
// When changing format increment FORMAT in mona.hpp
void Mona::Neuron::save(FILE *fp)
{
   int           i, j;
   struct Notify *notify;

   FWRITE_LONG_LONG(&id, fp);
   i = (int)type;
   FWRITE_INT(&i, fp);
   FWRITE_LONG_LONG(&creationTime, fp);
   goals.save(fp);
   FWRITE_DOUBLE(&motive, fp);
   FWRITE_BOOL(&instinct, fp);
   i = (int)notifyList.size();
   FWRITE_INT(&i, fp);
   i = 0;
   for (int n = (int)notifyList.size(); i < n; i++)
   {
      notify = notifyList[i];
      FWRITE_LONG_LONG(&notify->mediator->id, fp);
      j = (int)notify->eventType;
      FWRITE_INT(&j, fp);
   }
}


// Create sensor mode receptor and add to network.
Mona::Receptor *
Mona::newSensorModeReceptor(vector<SENSOR>& centroid, SENSOR_MODE sensorMode)
{
   Receptor *r = new Receptor(centroid, this);

   r->sensorMode          = sensorMode;
   r->sensorDiscriminator = -1;
   assert(r != NULL);
   r->id = idDispenser;
   idDispenser++;
   r->creationTime = eventClock;
   vector<SENSOR> *sensors = new vector<SENSOR>();
   assert(sensors != NULL);
   for (int i = 0, j = (int)r->centroid.size(); i < j; i++)
   {
      sensors->push_back(r->centroid[i]);
   }
   sensorModeCentroids[sensorMode]->insert((void *)sensors, (void *)r);
   receptors.push_back(r);
   r->addGoals();
   return(r);
}


// Create sensor discriminator receptor and add to network.
Mona::Receptor *
Mona::newSensorDiscriminatorReceptor(vector<SENSOR>& centroid, int index)
{
   Receptor *r = new Receptor(centroid, this);

   r->sensorMode          = 0;
   r->sensorDiscriminator = index;
   assert(r != NULL);
   r->id = idDispenser;
   idDispenser++;
   r->creationTime = eventClock;
   vector<SENSOR> *sensors = new vector<SENSOR>();
   assert(sensors != NULL);
   for (int i = 0, j = (int)r->centroid.size(); i < j; i++)
   {
      sensors->push_back(r->centroid[i]);
   }
   sensorDiscriminatorCentroids[index]->insert((void *)sensors, (void *)r);
   receptors.push_back(r);
   r->addGoals();
   return(r);
}


// Receptor constructor.
Mona::Receptor::Receptor(vector<SENSOR>& centroid, Mona *mona)
{
   assert((int)centroid.size() == mona->numSensors);
   this->centroid.clear();
   for (int i = 0, j = (int)centroid.size(); i < j; i++)
   {
      this->centroid.push_back(centroid[i]);
   }
   sensorMode          = 0;
   sensorDiscriminator = -1;
   init(mona);
   type   = RECEPTOR;
   motive = 0.0;
}


// Receptor destructor.
Mona::Receptor::~Receptor()
{
   subSensorModes.clear();
   superSensorModes.clear();
   clear();
}


// Is given receptor a duplicate of this?
bool Mona::Receptor::isDuplicate(Receptor *receptor)
{
   if (sensorMode != receptor->sensorMode)
   {
      return(false);
   }
   if (sensorDiscriminator != receptor->sensorDiscriminator)
   {
      return(false);
   }
   if (centroidDistance(receptor->centroid) == 0.0)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// Load receptor.
void Mona::Receptor::load(FILE *fp)
{
   int      i, j;
   float    s;
   Receptor *receptor;

   clear();
   ((Neuron *)this)->load(fp);
   FREAD_INT(&sensorMode, fp);
   FREAD_INT(&sensorDiscriminator, fp);
   centroid.resize(mona->numSensors);
   for (i = 0; i < mona->numSensors; i++)
   {
      FREAD_FLOAT(&s, fp);
      centroid[i] = (SENSOR)s;
   }
   FREAD_INT(&j, fp);
   subSensorModes.clear();
   for (i = 0; i < j; i++)
   {
      receptor = (Receptor *)new ID;
      assert(receptor != NULL);
      FREAD_LONG_LONG((ID *)receptor, fp);
      subSensorModes.push_back(receptor);
   }
   FREAD_INT(&j, fp);
   superSensorModes.clear();
   for (i = 0; i < j; i++)
   {
      receptor = (Receptor *)new ID;
      assert(receptor != NULL);
      FREAD_LONG_LONG((ID *)receptor, fp);
      superSensorModes.push_back(receptor);
   }
}


// Save receptor.
// When changing format increment FORMAT in mona.hpp
void Mona::Receptor::save(FILE *fp)
{
   int   i, j;
   float s;
   ID    id;

   ((Neuron *)this)->save(fp);
   FWRITE_INT(&sensorMode, fp);
   FWRITE_INT(&sensorDiscriminator, fp);
   for (i = 0; i < mona->numSensors; i++)
   {
      s = (float)centroid[i];
      FWRITE_FLOAT(&s, fp);
   }
   j = (int)subSensorModes.size();
   FWRITE_INT(&j, fp);
   for (i = 0; i < j; i++)
   {
      id = subSensorModes[i]->id;
      FWRITE_LONG_LONG(&id, fp);
   }
   j = (int)superSensorModes.size();
   FWRITE_INT(&j, fp);
   for (i = 0; i < j; i++)
   {
      id = superSensorModes[i]->id;
      FWRITE_LONG_LONG(&id, fp);
   }
}


// Print receptor.
#ifdef MONA_TRACKING
void Mona::Receptor::print(FILE *out)
{
   print((TRACKING_FLAGS)0, out);
}


void Mona::Receptor::print(TRACKING_FLAGS tracking, FILE *out)
#else
void Mona::Receptor::print(FILE *out)
#endif
{
   fprintf(out, "<receptor><id>%llu</id>", id);
   fprintf(out, "<sensorMode>%d</sensorMode>", sensorMode);
   fprintf(out, "<sensorDiscriminator>%d</sensorDiscriminator>", sensorDiscriminator);
   fprintf(out, "<centroid>");
   for (int i = 0; i < mona->numSensors; i++)
   {
      fprintf(out, "<value>%f</value>", centroid[i]);
   }
   fprintf(out, "</centroid>");
   fprintf(out, "<goals>");
   goals.print(out);
   fprintf(out, "</goals>");
   if (instinct)
   {
      fprintf(out, "<instinct>true</instinct>");
   }
   else
   {
      fprintf(out, "<instinct>false</instinct>");
   }
#ifdef MONA_TRACKING
   if ((tracking & TRACK_FIRE) && tracker.fire)
   {
      fprintf(out, "<fire>true</fire>");
   }
   if ((tracking & TRACK_ENABLE) && tracker.enable)
   {
      fprintf(out, "<enable>true</enable>");
   }
   if ((tracking & TRACK_DRIVE) && tracker.drive)
   {
      fprintf(out, "<motive>%f</motive>", tracker.motive);
   }
#endif
   fprintf(out, "</receptor>");
}


// Create motor and add to network.
Mona::Motor *
Mona::newMotor()
{
   Motor *motor = new Motor(MOVEMENT_TYPE::NONE, this);

   assert(motor != NULL);
   motor->id = idDispenser;
   idDispenser++;
   motor->creationTime = eventClock;
   motors.push_back(motor);
   responsePotentials.push_back(0.0);
   numResponses++;
   return(motor);
}


// Create movement motor and add to network.
Mona::Motor *
Mona::newMovementMotor(int movementType)
{
   Motor *motor = new Motor(movementType, this);

   assert(motor != NULL);
   motor->id = idDispenser;
   idDispenser++;
   motor->creationTime = eventClock;
   motors.push_back(motor);
   responsePotentials.push_back(0.0);
   numResponses++;
   if (movementType == MOVEMENT_TYPE::BEGIN)
   {
      assert(movementBeginResponse == -1);
      movementBeginResponse = numResponses - 1;
   }
   if (movementType == MOVEMENT_TYPE::END)
   {
      assert(movementEndResponse == -1);
      movementEndResponse = numResponses - 1;
   }
   return(motor);
}


// Create place motor and add to network.
Mona::Motor *
Mona::newPlaceMotor(int x, int y)
{
   assert(movementBeginResponse != -1 && movementEndResponse != -1);
   Motor *motor;
   for (int i = 0, j = placeMotors.size(); i < j; i++)
   {
      motor = placeMotors[i];
      if ((motor->x == x) && (motor->y == y))
      {
         return(motor);
      }
   }
   motor = new Motor(x, y, this);
   assert(motor != NULL);
   motor->id = idDispenser;
   idDispenser++;
   motor->creationTime = eventClock;
   placeMotors.push_back(motor);
   return(motor);
}


// Motor constructor.
Mona::Motor::Motor(int movementType, Mona *mona)
{
   init(mona);
   type               = MOTOR;
   response           = mona->motors.size();
   this->movementType = movementType;
   x = y = -1;
}


// Place motor constructor.
Mona::Motor::Motor(int x, int y, Mona *mona)
{
   init(mona);
   type         = MOTOR;
   response     = -1;
   movementType = MOVEMENT_TYPE::NONE;
   this->x      = x;
   this->y      = y;
}


// Motor destructor.
Mona::Motor::~Motor()
{
   clear();
}


// Load motor.
void Mona::Motor::load(FILE *fp)
{
   clear();
   ((Neuron *)this)->load(fp);
   FREAD_INT(&response, fp);
   FREAD_INT(&movementType, fp);
   FREAD_INT(&x, fp);
   FREAD_INT(&y, fp);
}


// Save motor.
// When changing format increment FORMAT in mona.hpp
void Mona::Motor::save(FILE *fp)
{
   ((Neuron *)this)->save(fp);
   FWRITE_INT(&response, fp);
   FWRITE_INT(&movementType, fp);
   FWRITE_INT(&x, fp);
   FWRITE_INT(&y, fp);
}


// Print motor.
#ifdef MONA_TRACKING
void Mona::Motor::print(FILE *out)
{
   print((TRACKING_FLAGS)0, out);
}


void Mona::Motor::print(TRACKING_FLAGS tracking, FILE *out)
#else
void Mona::Motor::print(FILE *out)
#endif
{
   fprintf(out, "<motor><id>%llu</id>", id);
   if (!isPlaceMotor())
   {
      fprintf(out, "<response>%d</response>", response);

      if (movementType != MOVEMENT_TYPE::NONE)
      {
         fprintf(out, "<movement_type>");
         switch (movementType)
         {
         case MOVEMENT_TYPE::BEGIN:
            fprintf(out, "BEGIN");
            break;

         case MOVEMENT_TYPE::END:
            fprintf(out, "END");
            break;

         case MOVEMENT_TYPE::MOVE_FORWARD:
            fprintf(out, "MOVE_FORWARD");
            break;

         case MOVEMENT_TYPE::TURN_RIGHT:
            fprintf(out, "TURN_RIGHT");
            break;

         case MOVEMENT_TYPE::TURN_LEFT:
            fprintf(out, "TURN_LEFT");
            break;
         }
         fprintf(out, "</movement_type>");
      }
   }
   else
   {
      fprintf(out, "<x>%d</x><y>%d</y>", x, y);
   }
   fprintf(out, "<goals>");
   goals.print(out);
   fprintf(out, "</goals>");
#ifdef MONA_TRACKING
   if ((tracking & TRACK_FIRE) && tracker.fire)
   {
      fprintf(out, "<fire>true</fire>");
   }
   if ((tracking & TRACK_ENABLE) && tracker.enable)
   {
      fprintf(out, "<enable>true</enable>");
   }
   if ((tracking & TRACK_DRIVE) && tracker.drive)
   {
      fprintf(out, "<motive>%f</motive>", tracker.motive);
   }
#endif
   fprintf(out, "</motor>");
}


// Create mediator and add to network.
Mona::Mediator *
Mona::newMediator(ENABLEMENT enablement)
{
   Mediator *m;

   m = new Mediator(enablement, this);
   assert(m != NULL);
   m->id = idDispenser;
   idDispenser++;
   m->creationTime = eventClock;
   mediators.push_back(m);
   return(m);
}


// Mediator constructor.
Mona::Mediator::Mediator(ENABLEMENT enablement, Mona *mona)
{
   init(mona);
   type                     = MEDIATOR;
   level                    = 0;
   baseEnablement           = enablement;
   effectiveEnablement      = 0.0;
   effectiveEnablingWeight  = 0.0;
   effectiveEnablementValid = false;
   utilityWeight            = 0.0;
   updateUtility(0.0);
   cause      = response = effect = NULL;
   causeBegin = 0;
}


// Mediator destructor.
Mona::Mediator::~Mediator()
{
   struct Notify *notify;

   vector<struct Notify *>::iterator notifyItr;

   if (cause != NULL)
   {
      for (notifyItr = cause->notifyList.begin();
           notifyItr != cause->notifyList.end(); notifyItr++)
      {
         notify = *notifyItr;
         if (notify->mediator == this)
         {
            cause->notifyList.erase(notifyItr);
            delete notify;
            break;
         }
      }
   }
   cause = NULL;
   if (response != NULL)
   {
      for (notifyItr = response->notifyList.begin();
           notifyItr != response->notifyList.end(); notifyItr++)
      {
         notify = *notifyItr;
         if (notify->mediator == this)
         {
            response->notifyList.erase(notifyItr);
            delete notify;
            break;
         }
      }
   }
   response = NULL;
   if (effect != NULL)
   {
      for (notifyItr = effect->notifyList.begin();
           notifyItr != effect->notifyList.end(); notifyItr++)
      {
         notify = *notifyItr;
         if (notify->mediator == this)
         {
            effect->notifyList.erase(notifyItr);
            delete notify;
            break;
         }
      }
   }
   effect = NULL;
   responseEnablings.clear();
   effectEnablings.clear();
   clear();
}


// Update mediator utility using its enablement
// and an asymptotic increment based on use.
void
Mona::Mediator::updateUtility(WEIGHT updateWeight)
{
   // Check for floating point overflow.
   WEIGHT w = utilityWeight + updateWeight + mona->UTILITY_ASYMPTOTE;

   if (w > (utilityWeight + updateWeight))
   {
      utilityWeight += updateWeight;
   }

   // Boost utility.
   w       = utilityWeight / (mona->UTILITY_ASYMPTOTE + utilityWeight);
   utility = ((UTILITY)getEnablement() + w) / 2.0;
}


// Get mediator effective utility,
// which is the greatest utility of itself and its parents.
Mona::UTILITY
Mona::Mediator::getEffectiveUtility()
{
   Mediator *mediator;
   UTILITY  bestUtility, utilityWork;

   bestUtility = utility;
   for (int i = 0, j = (int)notifyList.size(); i < j; i++)
   {
      mediator    = notifyList[i]->mediator;
      utilityWork = mediator->getEffectiveUtility();
      if (utilityWork > bestUtility)
      {
         bestUtility = utilityWork;
      }
   }
   return(bestUtility);
}


// Add mediator event.
void
Mona::Mediator::addEvent(EVENT_TYPE type, Neuron *neuron)
{
   struct Notify *notify;
   Mediator      *mediator;

   switch (type)
   {
   case CAUSE_EVENT:
      assert(cause == NULL);
      cause = neuron;
      break;

   case RESPONSE_EVENT:
      assert(response == NULL);
      response = neuron;
      break;

   case EFFECT_EVENT:
      assert(effect == NULL);
      effect = neuron;
      break;
      assert(false);
   }
   if (neuron->type == MEDIATOR)
   {
      mediator = (Mediator *)neuron;
      if (mediator->level + 1 > level)
      {
         level = mediator->level + 1;
         assert(level <= mona->MAX_MEDIATOR_LEVEL);
      }
   }
   notify = new struct Notify;
   assert(notify != NULL);
   notify->mediator  = this;
   notify->eventType = type;
   neuron->notifyList.push_back(notify);

   // Sort by type: effect, response, cause.
   for (int i = 0, j = (int)neuron->notifyList.size(); i < j; i++)
   {
      for (int k = i + 1; k < j; k++)
      {
         switch (neuron->notifyList[k]->eventType)
         {
         case EFFECT_EVENT:
            switch (neuron->notifyList[i]->eventType)
            {
            case EFFECT_EVENT:
               break;

            case RESPONSE_EVENT:
            case CAUSE_EVENT:
               notify = neuron->notifyList[i];
               neuron->notifyList[i] = neuron->notifyList[k];
               neuron->notifyList[k] = notify;
               break;
            }
            break;

         case RESPONSE_EVENT:
            switch (neuron->notifyList[i]->eventType)
            {
            case EFFECT_EVENT:
               break;

            case RESPONSE_EVENT:
               break;

            case CAUSE_EVENT:
               notify = neuron->notifyList[i];
               neuron->notifyList[i] = neuron->notifyList[k];
               neuron->notifyList[k] = notify;
               break;
            }
            break;

         case CAUSE_EVENT:
            break;
         }
      }
   }
}


// Is given mediator a duplicate of this?
bool Mona::Mediator::isDuplicate(Mediator *mediator)
{
   if (cause != mediator->cause)
   {
      return(false);
   }
   if (response != mediator->response)
   {
      return(false);
   }
   if (effect != mediator->effect)
   {
      return(false);
   }
   return(true);
}


// Load mediator.
void Mona::Mediator::load(FILE *fp)
{
   int i;

   clear();
   ((Neuron *)this)->load(fp);
   FREAD_INT(&level, fp);
   FREAD_DOUBLE(&baseEnablement, fp);
   FREAD_DOUBLE(&utility, fp);
   FREAD_DOUBLE(&utilityWeight, fp);
   cause = (Neuron *)new ID;
   assert(cause != NULL);
   FREAD_LONG_LONG((ID *)cause, fp);
   FREAD_INT(&i, fp);
   if (i == 1)
   {
      response = (Neuron *)new ID;
      assert(response != NULL);
      FREAD_LONG_LONG((ID *)response, fp);
   }
   else
   {
      response = NULL;
   }
   effect = (Neuron *)new ID;
   assert(effect != NULL);
   FREAD_LONG_LONG((ID *)effect, fp);
   responseEnablings.load(fp);
   effectEnablings.load(fp);
   FREAD_LONG_LONG(&causeBegin, fp);
}


// Save mediator.
// When changing format increment FORMAT in mona.hpp
void Mona::Mediator::save(FILE *fp)
{
   int i;

   ((Neuron *)this)->save(fp);
   FWRITE_INT(&level, fp);
   FWRITE_DOUBLE(&baseEnablement, fp);
   FWRITE_DOUBLE(&utility, fp);
   FWRITE_DOUBLE(&utilityWeight, fp);
   FWRITE_LONG_LONG(&cause->id, fp);
   if (response != NULL)
   {
      i = 1;
      FWRITE_INT(&i, fp);
      FWRITE_LONG_LONG(&response->id, fp);
   }
   else
   {
      i = 0;
      FWRITE_INT(&i, fp);
   }
   FWRITE_LONG_LONG(&effect->id, fp);
   responseEnablings.save(fp);
   effectEnablings.save(fp);
   FWRITE_LONG_LONG(&causeBegin, fp);
}


// Print mediator.
#ifdef MONA_TRACKING
void Mona::Mediator::print(FILE *out)
{
   print((TRACKING_FLAGS)0, out);
}


void Mona::Mediator::print(TRACKING_FLAGS tracking, FILE *out)
{
   print(tracking, false, 0, out);
}


#else
void Mona::Mediator::print(FILE *out)
{
   print(false, 0, out);
}


#endif

#ifdef MONA_TRACKING
void Mona::Mediator::printBrief(FILE *out)
{
   print((TRACKING_FLAGS)0, true, 0, out);
}


void Mona::Mediator::printBrief(TRACKING_FLAGS tracking, FILE *out)
{
   print(tracking, true, 0, out);
}


#else
void Mona::Mediator::printBrief(FILE *out)
{
   print(true, 0, out);
}


#endif

#ifdef MONA_TRACKING
void Mona::Mediator::print(TRACKING_FLAGS tracking, bool brief,
                           int level, FILE *out)
#else
void Mona::Mediator::print(bool brief, int level, FILE *out)
#endif
{
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "<mediator>\n");
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "  <id>%llu</id>\n", id);
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "  <level>%d</level>\n", this->level);
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "  <creationTime>%llu</creationTime>\n", this->creationTime);
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   if (instinct)
   {
      fprintf(out, "  <instinct>true</instinct>\n");
   }
   else
   {
      fprintf(out, "  <instinct>false</instinct>\n");
   }
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "  <enablement>%f</enablement>\n", getEnablement());
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "  <utility>%f</utility>\n", utility);
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "  <utilityWeight>%f</utilityWeight>\n", utilityWeight);
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "  <goals>");
   goals.print(out);
   fprintf(out, "</goals>\n");
#ifdef MONA_TRACKING
   if ((tracking & TRACK_FIRE) && tracker.fire)
   {
      for (int i = 0; i < level; i++)
      {
         fprintf(out, "    ");
      }
      fprintf(out, "  <fire>true</fire>\n");
   }
   if ((tracking & TRACK_ENABLE) && tracker.enable)
   {
      for (int i = 0; i < level; i++)
      {
         fprintf(out, "    ");
      }
      fprintf(out, "  <enable>true</enable>\n");
   }
   if ((tracking & TRACK_DRIVE) && tracker.drive)
   {
      for (int i = 0; i < level; i++)
      {
         fprintf(out, "    ");
      }
      fprintf(out, "  <motive>%f</motive>\n", tracker.motive);
   }
#endif
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "  <cause>\n");
   if (cause->type == MEDIATOR)
   {
      if (brief)
      {
         for (int i = 0; i <= level; i++)
         {
            fprintf(out, "    ");
         }
         fprintf(out, "<mediator><id>%llu</id></mediator>\n", cause->id);
      }
      else
      {
#ifdef MONA_TRACKING
         ((Mediator *)cause)->print(tracking, false, level + 1, out);
#else
         ((Mediator *)cause)->print(false, level + 1, out);
#endif
      }
   }
   else
   {
      if (brief)
      {
         for (int i = 0; i <= level; i++)
         {
            fprintf(out, "    ");
         }
         fprintf(out, "<receptor><id>%llu</id></receptor>\n", cause->id);
      }
      else
      {
         for (int i = 0; i <= level; i++)
         {
            fprintf(out, "    ");
         }
#ifdef MONA_TRACKING
         ((Receptor *)cause)->print(tracking, out);
#else
         ((Receptor *)cause)->print(out);
#endif
         fprintf(out, "\n");
      }
   }
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "  </cause>\n");
   if (response != NULL)
   {
      if (brief)
      {
         for (int i = 0; i < level; i++)
         {
            fprintf(out, "    ");
         }
         fprintf(out, "  <response>\n");
         for (int i = 0; i <= level; i++)
         {
            fprintf(out, "    ");
         }
         fprintf(out, "<motor><id>%llu</id></motor>\n", response->id);
         for (int i = 0; i < level; i++)
         {
            fprintf(out, "    ");
         }
         fprintf(out, "  </response>\n");
      }
      else
      {
         for (int i = 0; i < level; i++)
         {
            fprintf(out, "    ");
         }
         fprintf(out, "  <response>\n");
         for (int i = 0; i <= level; i++)
         {
            fprintf(out, "    ");
         }
#ifdef MONA_TRACKING
         ((Motor *)response)->print(tracking, out);
#else
         ((Motor *)response)->print(out);
#endif
         fprintf(out, "\n");
         for (int i = 0; i < level; i++)
         {
            fprintf(out, "    ");
         }
         fprintf(out, "  </response>\n");
      }
   }
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "  <effect>\n");
   if (effect->type == MEDIATOR)
   {
      if (brief)
      {
         for (int i = 0; i <= level; i++)
         {
            fprintf(out, "    ");
         }
         fprintf(out, "<mediator><id>%llu</id></mediator>\n", effect->id);
      }
      else
      {
#ifdef MONA_TRACKING
         ((Mediator *)effect)->print(tracking, false, level + 1, out);
#else
         ((Mediator *)effect)->print(false, level + 1, out);
#endif
      }
   }
   else
   {
      if (brief)
      {
         for (int i = 0; i <= level; i++)
         {
            fprintf(out, "    ");
         }
         fprintf(out, "<receptor><id>%llu</id></receptor>\n", effect->id);
      }
      else
      {
         for (int i = 0; i <= level; i++)
         {
            fprintf(out, "    ");
         }
#ifdef MONA_TRACKING
         ((Receptor *)effect)->print(tracking, out);
#else
         ((Receptor *)effect)->print(out);
#endif
         fprintf(out, "\n");
      }
   }
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "  </effect>\n");
   for (int i = 0; i < level; i++)
   {
      fprintf(out, "    ");
   }
   fprintf(out, "</mediator>\n");
}


// Remove neuron from network.
void
Mona::deleteNeuron(Neuron *neuron)
{
   struct Notify *notify;
   LearningEvent *learningEvent;

   list<LearningEvent *>::iterator learningEventItr;
   Receptor *receptor, *refReceptor;

   // Delete parents.
   while (neuron->notifyList.size() > 0)
   {
      notify = neuron->notifyList[0];
      deleteNeuron((Neuron *)notify->mediator);
   }

   // Remove from learning space.
   int i = 0;
   if (neuron->type == MEDIATOR)
   {
      i = ((Mediator *)neuron)->level + 1;
   }
   if (i < (int)learningEvents.size())
   {
      for (learningEventItr = learningEvents[i].begin();
           learningEventItr != learningEvents[i].end(); )
      {
         learningEvent = *learningEventItr;
         if (learningEvent->neuron == neuron)
         {
            learningEventItr =
               learningEvents[i].erase(learningEventItr);
            delete learningEvent;
         }
         else
         {
            learningEventItr++;
         }
      }
   }

   // Delete neuron.
   switch (neuron->type)
   {
   case RECEPTOR:
      receptor = (Receptor *)neuron;
      for (int i = 0, j = (int)receptors.size(); i < j; i++)
      {
         if (receptors[i] == receptor)
         {
            receptors.erase(receptors.begin() + i);
            break;
         }
      }
      for (int i = 0, n = (int)receptor->subSensorModes.size(); i < n; i++)
      {
         refReceptor = receptor->subSensorModes[i];
         for (int j = 0, k = (int)refReceptor->superSensorModes.size(); j < k; j++)
         {
            if (refReceptor->superSensorModes[j] == receptor)
            {
               refReceptor->superSensorModes.erase(refReceptor->superSensorModes.begin() + j);
               break;
            }
         }
      }
      for (int i = 0, n = (int)receptor->superSensorModes.size(); i < n; i++)
      {
         refReceptor = receptor->superSensorModes[i];
         for (int j = 0, k = (int)refReceptor->subSensorModes.size(); j < k; j++)
         {
            if (refReceptor->subSensorModes[j] == receptor)
            {
               refReceptor->subSensorModes.erase(refReceptor->subSensorModes.begin() + j);
               break;
            }
         }
      }
      for (int i = 0, j = (int)homeostats.size(); i < j; i++)
      {
         homeostats[i]->removeNeuron(neuron);
      }
      if ((int)sensorModeCentroids.size() > 0)
      {
         sensorModeCentroids[receptor->sensorMode]->remove((void *)&(receptor->centroid));
      }
      delete receptor;
      break;

   case MOTOR:
      if (!((Motor *)neuron)->isPlaceMotor())
      {
         for (int i = 0, j = (int)motors.size(); i < j; i++)
         {
            if (motors[i] == (Motor *)neuron)
            {
               motors.erase(motors.begin() + i);
               break;
            }
         }
      }
      else
      {
         for (int i = 0, j = (int)placeMotors.size(); i < j; i++)
         {
            if (placeMotors[i] == (Motor *)neuron)
            {
               placeMotors.erase(placeMotors.begin() + i);
               break;
            }
         }
      }
      for (int i = 0, j = (int)homeostats.size(); i < j; i++)
      {
         homeostats[i]->removeNeuron(neuron);
      }
      delete (Motor *)neuron;
      break;

   case MEDIATOR:
      mediators.remove((Mediator *)neuron);
      for (int i = 0, j = (int)homeostats.size(); i < j; i++)
      {
         homeostats[i]->removeNeuron(neuron);
      }
      delete (Mediator *)neuron;
      break;
   }
}


// Get mediator with worst utility.
// Skip instinct mediators and those with instinct parents.
Mona::Mediator *
Mona::getWorstMediator(int minLevel)
{
   Mediator *mediator;

   list<Mediator *>::iterator mediatorItr;
   vector<Mediator *>         worstMediators;
   UTILITY utility, worstUtility;

   worstUtility = 0.0;
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      if (mediator->level < minLevel)
      {
         continue;
      }
      if (mediator->instinct || mediator->hasParentInstinct())
      {
         continue;
      }
      utility = mediator->getEffectiveUtility();
      if ((worstMediators.size() == 0) || (utility < worstUtility))
      {
         worstMediators.clear();
         worstMediators.push_back(mediator);
         worstUtility = utility;
      }
      else if ((utility - worstUtility) <= NEARLY_ZERO)
      {
         worstMediators.push_back(mediator);
      }
   }
   if (worstMediators.size() == 0)
   {
      return(NULL);
   }
   else
   {
      // Return a random choice of worst.
      return(worstMediators[random.RAND_CHOICE((int)worstMediators.size())]);
   }
}


// Get mediator with best utility.
Mona::Mediator *
Mona::getBestMediator(int minLevel)
{
   Mediator *mediator;

   list<Mediator *>::iterator mediatorItr;
   vector<Mediator *>         bestMediators;
   UTILITY utility, bestUtility;

   bestUtility = 0.0;
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      if (mediator->level < minLevel)
      {
         continue;
      }
      utility = mediator->getEffectiveUtility();
      if ((bestMediators.size() == 0) || (utility > bestUtility))
      {
         bestMediators.clear();
         bestMediators.push_back(mediator);
         bestUtility = utility;
      }
      else if ((bestUtility - utility) <= NEARLY_ZERO)
      {
         bestMediators.push_back(mediator);
      }
   }
   if (bestMediators.size() == 0)
   {
      return(NULL);
   }
   else
   {
      // Return a random choice.
      return(bestMediators[random.RAND_CHOICE((int)bestMediators.size())]);
   }
}


// Load network from a file.
bool
Mona::load(char *filename)
{
   FILE *fp;

   if ((fp = FOPEN_READ(filename)) == NULL)
   {
      return(false);
   }
   bool ret = load(fp);
   FCLOSE(fp);
   return(ret);
}


bool
Mona::load(FILE *fp)
{
   int                 format;
   TIME                t;
   double              d;
   SensorMode          *sensorMode;
   SensorDiscriminator *sensorDiscriminator;
   RDtree              *rdTree;
   LearningEvent       *learningEvent;

   list<LearningEvent *>::iterator learningEventItr;
   Receptor *receptor;
   Motor    *motor;
   Mediator *mediator;
   list<Mediator *>::iterator mediatorItr;
   struct Notify              *notify;
   ID *id;

   // Check format compatibility.
   FREAD_INT(&format, fp);
   if (format != FORMAT)
   {
      fprintf(stderr, "File format %d is incompatible with expected format %d\n", format, FORMAT);
      return(false);
   }
   char buf[40];
   FREAD_STRING(buf, 40, fp);

   clear();
   FREAD_DOUBLE(&MIN_ENABLEMENT, fp);
   FREAD_DOUBLE(&INITIAL_ENABLEMENT, fp);
   FREAD_DOUBLE(&DRIVE_ATTENUATION, fp);
   FREAD_DOUBLE(&MIN_DRIVE_MOTIVE, fp);
   FREAD_DOUBLE(&FIRING_STRENGTH_LEARNING_DAMPER, fp);
   FREAD_DOUBLE(&LEARNING_DECREASE_VELOCITY, fp);
   FREAD_DOUBLE(&LEARNING_INCREASE_VELOCITY, fp);
   FREAD_DOUBLE(&RESPONSE_RANDOMNESS, fp);
   FREAD_INT(&MIN_MOVEMENT_RESPONSE_PATH_LENGTH, fp);
   FREAD_INT(&MAX_MOVEMENT_RESPONSE_PATH_LENGTH, fp);
   FREAD_DOUBLE(&UTILITY_ASYMPTOTE, fp);
   FREAD_INT(&DEFAULT_MAX_LEARNING_EFFECT_EVENT_INTERVAL, fp);
   FREAD_INT(&DEFAULT_NUM_EFFECT_EVENT_INTERVALS, fp);
   FREAD_INT(&MAX_MEDIATORS, fp);
   FREAD_INT(&MAX_MEDIATOR_LEVEL, fp);
   FREAD_INT(&MAX_RESPONSE_EQUIPPED_MEDIATOR_LEVEL, fp);
   FREAD_INT(&MIN_RESPONSE_UNEQUIPPED_MEDIATOR_LEVEL, fp);
   FREAD_FLOAT(&SENSOR_RESOLUTION, fp);
   FREAD_BOOL(&LEARN_MEDIATOR_GOAL_VALUE, fp);
   FREAD_INT(&LEARN_MEDIATOR_GOAL_VALUE_MIN_LEVEL, fp);
   FREAD_BOOL(&LEARN_RECEPTOR_GOAL_VALUE, fp);
   effectEventIntervals.resize(MAX_MEDIATOR_LEVEL + 1);
   for (int i = 0; i <= MAX_MEDIATOR_LEVEL; i++)
   {
      int k;
      FREAD_INT(&k, fp);
      effectEventIntervals[i].resize(k);
      for (int j = 0; j < k; j++)
      {
         FREAD_LONG_LONG(&t, fp);
         effectEventIntervals[i][j] = t;
      }
   }
   effectEventIntervalWeights.resize((int)effectEventIntervals.size());
   for (int i = 0, n = (int)effectEventIntervalWeights.size(); i < n; i++)
   {
      effectEventIntervalWeights[i].resize((int)effectEventIntervals[i].size());
      for (int j = 0, k = (int)effectEventIntervalWeights[i].size(); j < k; j++)
      {
         FREAD_DOUBLE(&d, fp);
         effectEventIntervalWeights[i][j] = d;
      }
   }
   maxLearningEffectEventIntervals.resize(MAX_MEDIATOR_LEVEL + 1);
   for (int i = 0; i <= MAX_MEDIATOR_LEVEL; i++)
   {
      FREAD_LONG_LONG(&t, fp);
      maxLearningEffectEventIntervals[i] = t;
   }
   FREAD_INT(&numSensors, fp);
   FREAD_INT(&numResponses, fp);
   FREAD_INT(&numNeeds, fp);
   FREAD_LONG_LONG(&randomSeed, fp);
   int r = numResponses;
   initNet(numSensors, 0, numNeeds, randomSeed);
   numResponses = r;
   sensorModes.clear();
   int n = 0;
   FREAD_INT(&n, fp);
   for (int i = 0; i < n; i++)
   {
      sensorMode = new SensorMode();
      assert(sensorMode != NULL);
      sensorMode->load(fp);
      sensorModes.push_back(sensorMode);
   }
   sensorDiscriminators.clear();
   n = 0;
   FREAD_INT(&n, fp);
   for (int i = 0; i < n; i++)
   {
      sensorDiscriminator = new SensorDiscriminator();
      assert(sensorDiscriminator != NULL);
      sensorDiscriminator->load(fp);
      sensorDiscriminators.push_back(sensorDiscriminator);
   }
   random.RAND_LOAD(fp);
   for (int i = 0, j = (int)sensors.size(); i < j; i++)
   {
      FREAD_FLOAT(&sensors[i], fp);
   }
   FREAD_INT(&response, fp);
   FREAD_LONG_LONG(&eventClock, fp);
   idDispenser = 0;
   receptors.clear();
   n = 0;
   FREAD_INT(&n, fp);
   for (int i = 0; i < n; i++)
   {
      receptor = new Receptor(sensors, this);
      assert(receptor != NULL);
      receptor->load(fp);
      receptors.push_back(receptor);
      if (receptor->id > idDispenser)
      {
         idDispenser = receptor->id + 1;
      }
   }
   motors.clear();
   n = 0;
   FREAD_INT(&n, fp);
   for (int i = 0; i < n; i++)
   {
      motor = new Motor(-1, this);
      assert(motor != NULL);
      motor->load(fp);
      motors.push_back(motor);
      if (motor->id > idDispenser)
      {
         idDispenser = motor->id + 1;
      }
   }
   placeMotors.clear();
   n = 0;
   FREAD_INT(&n, fp);
   for (int i = 0; i < n; i++)
   {
      motor = new Motor(0, 0, this);
      assert(motor != NULL);
      motor->load(fp);
      placeMotors.push_back(motor);
      if (motor->id > idDispenser)
      {
         idDispenser = motor->id + 1;
      }
   }
   mediators.clear();
   n = 0;
   FREAD_INT(&n, fp);
   for (int i = 0; i < n; i++)
   {
      mediator = new Mediator(0.0, this);
      assert(mediator != NULL);
      mediator->load(fp);
      mediators.push_back(mediator);
      if (mediator->id > idDispenser)
      {
         idDispenser = mediator->id + 1;
      }
   }

   // Resolve neuron addresses using id.
   for (int i = 0, n = (int)receptors.size(); i < n; i++)
   {
      receptor = receptors[i];
      for (int j = 0, k = (int)receptor->subSensorModes.size(); j < k; j++)
      {
         id = (ID *)(receptor->subSensorModes[j]);
         receptor->subSensorModes[j] = (Receptor *)findByID(*id);
         assert(receptor->subSensorModes[j] != NULL);
         delete id;
      }
      for (int j = 0, k = (int)receptor->superSensorModes.size(); j < k; j++)
      {
         id = (ID *)(receptor->superSensorModes[j]);
         receptor->superSensorModes[j] = (Receptor *)findByID(*id);
         assert(receptor->superSensorModes[j] != NULL);
         delete id;
      }
      for (int j = 0, k = (int)receptor->notifyList.size(); j < k; j++)
      {
         notify           = receptor->notifyList[j];
         id               = (ID *)(notify->mediator);
         notify->mediator = (Mediator *)findByID(*id);
         assert(notify->mediator != NULL);
         delete id;
      }
   }
   for (int i = 0, n = (int)motors.size(); i < n; i++)
   {
      motor = motors[i];
      for (int j = 0, k = (int)motor->notifyList.size(); j < k; j++)
      {
         notify           = motor->notifyList[j];
         id               = (ID *)(notify->mediator);
         notify->mediator = (Mediator *)findByID(*id);
         assert(notify->mediator != NULL);
         delete id;
      }
   }
   for (int i = 0, n = (int)placeMotors.size(); i < n; i++)
   {
      motor = placeMotors[i];
      for (int j = 0, k = (int)motor->notifyList.size(); j < k; j++)
      {
         notify           = motor->notifyList[j];
         id               = (ID *)(notify->mediator);
         notify->mediator = (Mediator *)findByID(*id);
         assert(notify->mediator != NULL);
         delete id;
      }
   }
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator        = *mediatorItr;
      id              = (ID *)(mediator->cause);
      mediator->cause = findByID(*id);
      assert(mediator->cause != NULL);
      delete id;
      if (mediator->response != NULL)
      {
         id = (ID *)(mediator->response);
         mediator->response = findByID(*id);
         assert(mediator->response != NULL);
         delete id;
      }
      id = (ID *)(mediator->effect);
      mediator->effect = findByID(*id);
      assert(mediator->effect != NULL);
      delete id;
      for (int i = 0, j = (int)mediator->notifyList.size(); i < j; i++)
      {
         notify           = mediator->notifyList[i];
         id               = (ID *)(notify->mediator);
         notify->mediator = (Mediator *)findByID(*id);
         assert(notify->mediator != NULL);
         delete id;
      }
   }
   for (int i = 0, j = (int)learningEvents.size(); i < j; i++)
   {
      for (learningEventItr = learningEvents[i].begin();
           learningEventItr != learningEvents[i].end(); learningEventItr++)
      {
         learningEvent         = *learningEventItr;
         id                    = (ID *)(learningEvent->neuron);
         learningEvent->neuron = findByID(*id);
         assert(learningEvent->neuron != NULL);
         delete id;
      }
   }
   for (int i = 0; i < numNeeds; i++)
   {
      homeostats[i]->load(fp);
   }
   sensorModeCentroids.clear();
   n = 0;
   FREAD_INT(&n, fp);
   for (int i = 0; i < n; i++)
   {
      rdTree = new RDtree(Mona::Receptor::patternDistance,
                          Mona::Receptor::deletePattern);
      assert(rdTree != NULL);
      rdTree->load(fp, this, Mona::Receptor::loadPattern,
                   Mona::Receptor::loadClient);
      sensorModeCentroids.push_back(rdTree);
   }
   sensorDiscriminatorCentroids.clear();
   n = 0;
   FREAD_INT(&n, fp);
   for (int i = 0; i < n; i++)
   {
      rdTree = new RDtree(Mona::Receptor::patternDistance,
                          Mona::Receptor::deletePattern);
      assert(rdTree != NULL);
      rdTree->load(fp, this, Mona::Receptor::loadPattern,
                   Mona::Receptor::loadClient);
      sensorDiscriminatorCentroids.push_back(rdTree);
   }
   FREAD_INT(&movementBeginResponse, fp);
   FREAD_INT(&movementEndResponse, fp);
   return(true);
}


// Find neuron by id.
Mona::Neuron *
Mona::findByID(ID id)
{
   Receptor *receptor;
   Motor    *motor;
   Mediator *mediator;

   list<Mediator *>::iterator mediatorItr;

   for (int i = 0, j = (int)receptors.size(); i < j; i++)
   {
      receptor = receptors[i];
      if (receptor->id == id)
      {
         return((Neuron *)receptor);
      }
   }
   for (int i = 0, j = (int)motors.size(); i < j; i++)
   {
      motor = motors[i];
      if (motor->id == id)
      {
         return((Neuron *)motor);
      }
   }
   for (int i = 0, j = (int)placeMotors.size(); i < j; i++)
   {
      motor = placeMotors[i];
      if (motor->id == id)
      {
         return((Neuron *)motor);
      }
   }
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      if (mediator->id == id)
      {
         return((Neuron *)mediator);
      }
   }
   return(NULL);
}


// Save network to file.
bool
Mona::save(char *filename)
{
   FILE *fp;

   if ((fp = FOPEN_WRITE(filename)) == NULL)
   {
      return(false);
   }
   bool ret = save(fp);
   FCLOSE(fp);
   return(ret);
}


// When changing format increment FORMAT in mona.hpp
bool
Mona::save(FILE *fp)
{
   TIME          t;
   double        d;
   int           format;
   LearningEvent *learningEvent;

   list<LearningEvent *>::iterator learningEventItr;
   Receptor *receptor;
   Motor    *motor;
   Mediator *mediator;
   list<Mediator *>::iterator mediatorItr;

   // Save format, including searchable string.
   format = FORMAT;
   FWRITE_INT(&format, fp);
   char buf[40];
   sprintf(buf, "%s(#) Mona format %d", "@", FORMAT);
   FWRITE_STRING(buf, 40, fp);

   FWRITE_DOUBLE(&MIN_ENABLEMENT, fp);
   FWRITE_DOUBLE(&INITIAL_ENABLEMENT, fp);
   FWRITE_DOUBLE(&DRIVE_ATTENUATION, fp);
   FWRITE_DOUBLE(&MIN_DRIVE_MOTIVE, fp);
   FWRITE_DOUBLE(&FIRING_STRENGTH_LEARNING_DAMPER, fp);
   FWRITE_DOUBLE(&LEARNING_DECREASE_VELOCITY, fp);
   FWRITE_DOUBLE(&LEARNING_INCREASE_VELOCITY, fp);
   FWRITE_DOUBLE(&RESPONSE_RANDOMNESS, fp);
   FWRITE_INT(&MIN_MOVEMENT_RESPONSE_PATH_LENGTH, fp);
   FWRITE_INT(&MAX_MOVEMENT_RESPONSE_PATH_LENGTH, fp);
   FWRITE_DOUBLE(&UTILITY_ASYMPTOTE, fp);
   FWRITE_INT(&DEFAULT_MAX_LEARNING_EFFECT_EVENT_INTERVAL, fp);
   FWRITE_INT(&DEFAULT_NUM_EFFECT_EVENT_INTERVALS, fp);
   FWRITE_INT(&MAX_MEDIATORS, fp);
   FWRITE_INT(&MAX_MEDIATOR_LEVEL, fp);
   FWRITE_INT(&MAX_RESPONSE_EQUIPPED_MEDIATOR_LEVEL, fp);
   FWRITE_INT(&MIN_RESPONSE_UNEQUIPPED_MEDIATOR_LEVEL, fp);
   FWRITE_FLOAT(&SENSOR_RESOLUTION, fp);
   FWRITE_BOOL(&LEARN_MEDIATOR_GOAL_VALUE, fp);
   FWRITE_INT(&LEARN_MEDIATOR_GOAL_VALUE_MIN_LEVEL, fp);
   FWRITE_BOOL(&LEARN_RECEPTOR_GOAL_VALUE, fp);
   for (int i = 0, n = (int)effectEventIntervals.size(); i < n; i++)
   {
      int k = (int)effectEventIntervals[i].size();
      FWRITE_INT(&k, fp);
      for (int j = 0; j < k; j++)
      {
         t = effectEventIntervals[i][j];
         FWRITE_LONG_LONG(&t, fp);
      }
   }
   for (int i = 0, n = (int)effectEventIntervalWeights.size(); i < n; i++)
   {
      for (int j = 0, k = (int)effectEventIntervalWeights[i].size(); j < k; j++)
      {
         d = effectEventIntervalWeights[i][j];
         FWRITE_DOUBLE(&d, fp);
      }
   }
   for (int i = 0, j = (int)maxLearningEffectEventIntervals.size(); i < j; i++)
   {
      t = maxLearningEffectEventIntervals[i];
      FWRITE_LONG_LONG(&t, fp);
   }
   FWRITE_INT(&numSensors, fp);
   FWRITE_INT(&numResponses, fp);
   FWRITE_INT(&numNeeds, fp);
   FWRITE_LONG_LONG(&randomSeed, fp);
   int j = (int)sensorModes.size();
   FWRITE_INT(&j, fp);
   for (int i = 0; i < j; i++)
   {
      sensorModes[i]->save(fp);
   }
   j = (int)sensorDiscriminators.size();
   FWRITE_INT(&j, fp);
   for (int i = 0; i < j; i++)
   {
      sensorDiscriminators[i]->save(fp);
   }
   random.RAND_SAVE(fp);
   for (int i = 0, j = (int)sensors.size(); i < j; i++)
   {
      FWRITE_FLOAT(&sensors[i], fp);
   }
   FWRITE_INT(&response, fp);
   FWRITE_LONG_LONG(&eventClock, fp);
   int n = (int)receptors.size();
   FWRITE_INT(&n, fp);
   for (int i = 0; i < n; i++)
   {
      receptor = receptors[i];
      receptor->save(fp);
   }
   n = (int)motors.size();
   FWRITE_INT(&n, fp);
   for (int i = 0; i < n; i++)
   {
      motor = motors[i];
      motor->save(fp);
   }
   n = (int)placeMotors.size();
   FWRITE_INT(&n, fp);
   for (int i = 0; i < n; i++)
   {
      motor = placeMotors[i];
      motor->save(fp);
   }
   n = (int)mediators.size();
   FWRITE_INT(&n, fp);
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      mediator->save(fp);
   }
   for (int i = 0; i < numNeeds; i++)
   {
      homeostats[i]->save(fp);
   }
   j = (int)sensorModeCentroids.size();
   FWRITE_INT(&j, fp);
   for (int i = 0; i < j; i++)
   {
      sensorModeCentroids[i]->save(fp, Mona::Receptor::savePattern,
                                   Mona::Receptor::saveClient);
   }
   j = (int)sensorDiscriminatorCentroids.size();
   FWRITE_INT(&j, fp);
   for (int i = 0; i < j; i++)
   {
      sensorDiscriminatorCentroids[i]->save(fp, Mona::Receptor::savePattern,
                                            Mona::Receptor::saveClient);
   }
   FWRITE_INT(&movementBeginResponse, fp);
   FWRITE_INT(&movementEndResponse, fp);
   return(true);
}


// Clear the network.
void
Mona::clear()
{
   Receptor *receptor;
   Motor    *motor;

   list<Mediator *>::iterator      mediatorItr;
   LearningEvent                   *learningEvent;
   list<LearningEvent *>::iterator learningEventItr;

   random.RAND_CLEAR();
   sensors.clear();
   for (int i = 0, j = (int)sensorModes.size(); i < j; i++)
   {
      delete sensorModes[i];
   }
   sensorModes.clear();
   for (int i = 0, j = (int)sensorModeCentroids.size(); i < j; i++)
   {
      delete sensorModeCentroids[i];
   }
   sensorModeCentroids.clear();
   sensorDiscriminators.clear();
   for (int i = 0, j = (int)sensorDiscriminatorCentroids.size(); i < j; i++)
   {
      delete sensorDiscriminatorCentroids[i];
   }
   sensorDiscriminatorCentroids.clear();
   while ((int)receptors.size() > 0)
   {
      receptor = receptors[0];
      deleteNeuron(receptor);
   }
   receptors.clear();
   responsePotentials.clear();
   for (int i = 0, j = (int)motors.size(); i < j; i++)
   {
      motor = motors[i];
      delete motor;
   }
   motors.clear();
   for (int i = 0, j = (int)placeMotors.size(); i < j; i++)
   {
      motor = placeMotors[i];
      delete motor;
   }
   placeMotors.clear();
   numResponses = 0;
   movementLearningPathActive = false;
   movementLearningPathLength = 0;
   movementLearningCauses.clear();
   movementLearningEffects.clear();
   movementBeginResponse = -1;
   movementEndResponse   = -1;
   activePlaceMotor      = NULL;
   orientation           = ORIENTATION::NORTH;
   X = Y = -1;
   for (int i = 0, j = (int)homeostats.size(); i < j; i++)
   {
      delete homeostats[i];
   }
   homeostats.clear();
   for (int i = 0, j = (int)learningEvents.size(); i < j; i++)
   {
      for (learningEventItr = learningEvents[i].begin();
           learningEventItr != learningEvents[i].end(); learningEventItr++)
      {
         learningEvent = *learningEventItr;
         delete learningEvent;
      }
      learningEvents[i].clear();
   }
   learningEvents.clear();
   clearVars();
}


// Print network.
bool
Mona::print(char *filename)
{
   if (filename == NULL)
   {
      print();
   }
   else
   {
      FILE *fp = fopen(filename, "w");
      if (fp == NULL)
      {
         return(false);
      }
      print(fp);
      fclose(fp);
   }
   return(true);
}


#ifdef MONA_TRACKING
void
Mona::print(FILE *out)
{
   print((TRACKING_FLAGS)0, out);
}


void
Mona::print(TRACKING_FLAGS tracking, FILE *out)
{
   print(tracking, false, out);
}


#else
void
Mona::print(FILE *out)
{
   print(false, out);
}


#endif

#ifdef MONA_TRACKING
void
Mona::printBrief(FILE *out)
{
   print((TRACKING_FLAGS)0, true, out);
}


void
Mona::printBrief(TRACKING_FLAGS tracking, FILE *out)
{
   print(tracking, true, out);
}


#else
void
Mona::printBrief(FILE *out)
{
   print(true, out);
}


#endif

#ifdef MONA_TRACKING
void
Mona::print(TRACKING_FLAGS tracking, bool brief, FILE *out)
#else
void
Mona::print(bool brief, FILE *out)
#endif
{
   Receptor *receptor;
   Motor    *motor;
   Mediator *mediator;

   list<Mediator *>::iterator mediatorItr;
#ifdef MONA_TRACKING
   int          p, q, r, s, t;
   Motor        *driveMotor;
   Neuron       *neuron;
   vector<bool> pathDone;
#endif

   fprintf(out, "<network>\n");
   fprintf(out, "<format>%d</format>\n", FORMAT);
   printParms(out);
   fprintf(out, "<numSensors>%d</numSensors>\n", numSensors);
   fprintf(out, "<sensorModes>");
   for (int i = 0, j = (int)sensorModes.size(); i < j; i++)
   {
      sensorModes[i]->print(out);
   }
   fprintf(out, "</sensorModes>\n");
   fprintf(out, "<sensorDiscriminators");
   for (int i = 0, j = (int)sensorDiscriminators.size(); i < j; i++)
   {
      sensorDiscriminators[i]->print(out);
   }
   fprintf(out, "</sensorDiscriminators>\n");
   fprintf(out, "<numResponses>%d</numResponses>\n", numResponses);
   fprintf(out, "<numNeeds>%d</numNeeds>\n", numNeeds);
   fprintf(out, "<randomSeed>%llu</randomSeed>\n", randomSeed);
   fprintf(out, "<needs>\n");
   for (int i = 0; i < numNeeds; i++)
   {
      homeostats[i]->print(out);
      fprintf(out, "\n");
   }
   fprintf(out, "</needs>\n");
   fprintf(out, "<eventClock>%llu</eventClock>\n", eventClock);
#ifdef MONA_TRACKING
   if (tracking & TRACK_DRIVE)
   {
      int i = 0;
      for (int j = (int)motors.size(); i < j; i++)
      {
         driveMotor = motors[i];
         if (driveMotor->tracker.fire)
         {
            break;
         }
      }
      if (i == (int)motors.size())
      {
         driveMotor = NULL;
         q          = 1;
      }
      else
      {
         q = (int)driveMotor->tracker.motivePaths.size();
         for (p = 0; p < q; p++)
         {
            pathDone.push_back(false);
         }
      }
   }
   else
   {
      driveMotor = NULL;
      q          = 1;
   }
   for (p = 0; p < q; p++)
   {
      if (driveMotor != NULL)
      {
         for (r = 0, t = -1, s = (int)driveMotor->tracker.motivePaths.size(); r < s; r++)
         {
            if (pathDone[r])
            {
               continue;
            }
            if ((t == -1) ||
                (driveMotor->tracker.motivePaths[r].motive >= driveMotor->tracker.motivePaths[t].motive))
            {
               t = r;
            }
         }
         pathDone[t] = true;
         fprintf(out, "<drive_network motive=%f>\n", driveMotor->tracker.motivePaths[t].motive);
         for (r = 0, s = (int)driveMotor->tracker.motivePaths[t].drivers.size(); r < s; r++)
         {
            neuron = driveMotor->tracker.motivePaths[t].drivers[r].neuron;
            neuron->tracker.drive  = true;
            neuron->tracker.motive = driveMotor->tracker.motivePaths[t].drivers[r].motive;
         }
      }
#endif
   fprintf(out, "<receptors>\n");
   for (int i = 0, j = (int)receptors.size(); i < j; i++)
   {
      receptor = receptors[i];
#ifdef MONA_TRACKING
      if (((tracking & TRACK_FIRE) && receptor->tracker.fire) ||
          ((tracking & TRACK_ENABLE) && receptor->tracker.enable) ||
          ((tracking & TRACK_DRIVE) && receptor->tracker.drive))
      {
         receptor->print(tracking, out);
         fprintf(out, "\n");
      }
#else
      receptor->print(out);
      fprintf(out, "\n");
#endif
   }
   fprintf(out, "</receptors>\n");
   fprintf(out, "<motors>\n");
   for (int i = 0, j = (int)motors.size(); i < j; i++)
   {
      motor = motors[i];
#ifdef MONA_TRACKING
      if (((tracking & TRACK_FIRE) && motor->tracker.fire) ||
          ((tracking & TRACK_ENABLE) && motor->tracker.enable) ||
          ((tracking & TRACK_DRIVE) && motor->tracker.drive))
      {
         motor->print(tracking, out);
         fprintf(out, "\n");
      }
#else
      motor->print(out);
      fprintf(out, "\n");
#endif
   }
   fprintf(out, "</motors>\n");
   fprintf(out, "<place_motors>\n");
   for (int i = 0, j = (int)placeMotors.size(); i < j; i++)
   {
      motor = placeMotors[i];
#ifdef MONA_TRACKING
      if (((tracking & TRACK_FIRE) && motor->tracker.fire) ||
          ((tracking & TRACK_ENABLE) && motor->tracker.enable) ||
          ((tracking & TRACK_DRIVE) && motor->tracker.drive))
      {
         motor->print(tracking, out);
         fprintf(out, "\n");
      }
#else
      motor->print(out);
      fprintf(out, "\n");
#endif
   }
   fprintf(out, "</place_motors>\n");
   fprintf(out, "<mediators>\n");
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
#ifdef MONA_TRACKING
      if (((tracking & TRACK_FIRE) && mediator->tracker.fire) ||
          ((tracking & TRACK_ENABLE) && mediator->tracker.enable) ||
          ((tracking & TRACK_DRIVE) && mediator->tracker.drive))
      {
         mediator->print(tracking, brief, 0, out);
      }
#else
      mediator->print(brief, 0, out);
#endif
   }
   fprintf(out, "</mediators>\n");
   fprintf(out, "<orientation>");
   switch (orientation)
   {
   case ORIENTATION::NORTH:
      fprintf(out, "NORTH");
      break;

   case ORIENTATION::SOUTH:
      fprintf(out, "SOUTH");
      break;

   case ORIENTATION::EAST:
      fprintf(out, "EAST");
      break;

   case ORIENTATION::WEST:
      fprintf(out, "WEST");
      break;
   }
   fprintf(out, "</orientation>\n");
   fprintf(out, "<X>%d</X>\n", X);
   fprintf(out, "<Y>%d</Y>\n", Y);
#ifdef MONA_TRACKING
   if (driveMotor != NULL)
   {
      fprintf(out, "</drive_network>\n");
      for (r = 0, s = (int)driveMotor->tracker.motivePaths[t].drivers.size(); r < s; r++)
      {
         neuron = driveMotor->tracker.motivePaths[t].drivers[r].neuron;
         neuron->tracker.drive  = false;
         neuron->tracker.motive = 0.0;
      }
   }
}
#endif





   fprintf(out, "</network>\n");
   fflush(out);
}


// Print parameters.
void
Mona::printParms(FILE *out)
{
   fprintf(out, "<parameters>\n");
   fprintf(out, "<parameter>MIN_ENABLEMENT</parameter><value>%f</value>\n", MIN_ENABLEMENT);
   fprintf(out, "<parameter>INITIAL_ENABLEMENT</parameter><value>%f</value>\n", INITIAL_ENABLEMENT);
   fprintf(out, "<parameter>DRIVE_ATTENUATION</parameter><value>%f</value>\n", DRIVE_ATTENUATION);
   fprintf(out, "<parameter>MIN_DRIVE_MOTIVE</parameter><value>%f</value>\n", MIN_DRIVE_MOTIVE);
   fprintf(out, "<parameter>FIRING_STRENGTH_LEARNING_DAMPER</parameter><value>%f</value>\n", FIRING_STRENGTH_LEARNING_DAMPER);
   fprintf(out, "<parameter>LEARNING_DECREASE_VELOCITY</parameter><value>%f</value>\n", LEARNING_DECREASE_VELOCITY);
   fprintf(out, "<parameter>LEARNING_INCREASE_VELOCITY</parameter><value>%f</value>\n", LEARNING_INCREASE_VELOCITY);
   fprintf(out, "<parameter>RESPONSE_RANDOMNESS</parameter><value>%f</value>\n", RESPONSE_RANDOMNESS);
   fprintf(out, "<parameter>MIN_MOVEMENT_RESPONSE_PATH_LENGTH</parameter><value>%d</value>\n", MIN_MOVEMENT_RESPONSE_PATH_LENGTH);
   fprintf(out, "<parameter>MAX_MOVEMENT_RESPONSE_PATH_LENGTH</parameter><value>%d</value>\n", MAX_MOVEMENT_RESPONSE_PATH_LENGTH);
   fprintf(out, "<parameter>UTILITY_ASYMPTOTE</parameter><value>%f</value>\n", UTILITY_ASYMPTOTE);
   fprintf(out, "<parameter>DEFAULT_MAX_LEARNING_EFFECT_EVENT_INTERVAL</parameter><value>%d</value>\n", DEFAULT_MAX_LEARNING_EFFECT_EVENT_INTERVAL);
   fprintf(out, "<parameter>DEFAULT_NUM_EFFECT_EVENT_INTERVALS</parameter><value>%d</value>\n", DEFAULT_NUM_EFFECT_EVENT_INTERVALS);
   fprintf(out, "<parameter>MAX_MEDIATORS</parameter><value>%d</value>\n", MAX_MEDIATORS);
   fprintf(out, "<parameter>MAX_MEDIATOR_LEVEL</parameter><value>%d</value>\n", MAX_MEDIATOR_LEVEL);
   fprintf(out, "<parameter>MAX_RESPONSE_EQUIPPED_MEDIATOR_LEVEL</parameter><value>%d</value>\n", MAX_RESPONSE_EQUIPPED_MEDIATOR_LEVEL);
   fprintf(out, "<parameter>MIN_RESPONSE_UNEQUIPPED_MEDIATOR_LEVEL</parameter><value>%d</value>\n", MIN_RESPONSE_UNEQUIPPED_MEDIATOR_LEVEL);
   fprintf(out, "<parameter>SENSOR_RESOLUTION</parameter><value>%f</value>\n", SENSOR_RESOLUTION);
   if (LEARN_MEDIATOR_GOAL_VALUE)
   {
      fprintf(out, "<parameter>LEARN_MEDIATOR_GOAL_VALUE</parameter><value>true</value>\n");
   }
   else
   {
      fprintf(out, "<parameter>LEARN_MEDIATOR_GOAL_VALUE</parameter><value>false</value>\n");
   }
   fprintf(out, "<parameter>LEARN_MEDIATOR_GOAL_VALUE_MIN_LEVEL</parameter><value>%d</value>\n", LEARN_MEDIATOR_GOAL_VALUE_MIN_LEVEL);
   if (LEARN_RECEPTOR_GOAL_VALUE)
   {
      fprintf(out, "<parameter>LEARN_RECEPTOR_GOAL_VALUE</parameter><value>true</value>\n");
   }
   else
   {
      fprintf(out, "<parameter>LEARN_RECEPTOR_GOAL_VALUE</parameter><value>false</value>\n");
   }
   fprintf(out, "<effect_event_intervals>\n");
   for (int i = 0, n = (int)effectEventIntervals.size(); i < n; i++)
   {
      fprintf(out, "<intervals><level>%d</level>", i);
      for (int j = 0, k = (int)effectEventIntervals[i].size(); j < k; j++)
      {
         fprintf(out, "<time>%llu</time>", effectEventIntervals[i][j]);
      }
      fprintf(out, "</intervals>\n");
   }
   fprintf(out, "</effect_event_intervals>\n");
   fprintf(out, "<effect_event_interval_weights>\n");
   for (int i = 0, n = (int)effectEventIntervalWeights.size(); i < n; i++)
   {
      fprintf(out, "<weights><level>%d</level>", i);
      for (int j = 0, k = (int)effectEventIntervalWeights[i].size(); j < k; j++)
      {
         fprintf(out, "<weight>%f</weight>", effectEventIntervalWeights[i][j]);
      }
      fprintf(out, "</weights>\n");
   }
   fprintf(out, "</effect_event_interval_weights>\n");
   fprintf(out, "<maximum_learning_effect_event_intervals>\n");
   for (int i = 0; i <= MAX_MEDIATOR_LEVEL; i++)
   {
      fprintf(out, "<interval><level>%d</level><time>%llu</time></interval>\n",
              i, maxLearningEffectEventIntervals[i]);
   }
   fprintf(out, "</maximum_learning_effect_event_intervals>\n");
   fprintf(out, "</parameters>\n");
   fflush(out);
}


#ifdef MONA_TRACKING
// Clear tracking activity.
void
Mona::clearTracking()
{
   Receptor *receptor;
   Motor    *motor;
   Mediator *mediator;

   list<Mediator *>::iterator mediatorItr;

   for (int i = 0, j = (int)receptors.size(); i < j; i++)
   {
      receptor = receptors[i];
      receptor->tracker.clear();
   }
   for (int i = 0, j = (int)motors.size(); i < j; i++)
   {
      motor = motors[i];
      motor->tracker.clear();
   }
   for (int i = 0, j = (int)placeMotors.size(); i < j; i++)
   {
      motor = placeMotors[i];
      motor->tracker.clear();
   }
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      mediator->tracker.clear();
   }
}


#endif
