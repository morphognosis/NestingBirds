// For conditions of distribution and use, see copyright notice in mona.hpp

#include "mona.hpp"

const Homeostat::ID Homeostat::NULL_ID = (ID)(-1);

// Homeostat constructors.
Homeostat::Homeostat()
{
   need         = 0.0;
   needIndex    = 0;
   needDelta    = 0.0;
   defaultNeed  = 0.0;
   mona         = NULL;
   periodicNeed = 0.0;
   frequency    = 0;
   freqTimer    = 0;
}


Homeostat::Homeostat(int needIndex, Mona *mona)
{
   need            = 0.0;
   this->needIndex = needIndex;
   needDelta       = 0.0;
   defaultNeed     = 0.0;
   this->mona      = mona;
   periodicNeed    = 0.0;
   frequency       = 0;
   freqTimer       = 0;
}


// Destructor.
Homeostat::~Homeostat()
{
   goals.clear();
}


// Set default need.
void Homeostat::setDefaultNeed()
{
   need = defaultNeed;
}


// Set periodic need.
void Homeostat::setPeriodicNeed(int frequency, NEED need)
{
   this->frequency = frequency;
   periodicNeed    = need;
   freqTimer       = 0;
}


// Clear periodic need.
void Homeostat::clearPeriodicNeed()
{
   periodicNeed = 0.0;
   frequency    = 0;
   freqTimer    = 0;
}


// Add sensory-response goal.
int Homeostat::addGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode,
                       void *motor, NEED goalValue)
{
   int goalIndex;

   vector<SENSOR> sensorsWork;
   Goal           goal;

   // New goal?
   if ((goalIndex = findGoal(sensors, sensorMode, motor)) == -1)
   {
      // Add entry.
      mona->applySensorMode(sensors, sensorsWork, sensorMode);
      for (int i = 0, j = (int)sensorsWork.size(); i < j; i++)
      {
         goal.sensors.push_back(sensorsWork[i]);
      }
      goal.sensorMode      = sensorMode;
      goal.pendingReceptor = NULL;
      goal.motor           = motor;
      if (goal.motor != NULL)
      {
         ((Mona::Motor *)goal.motor)->goals.setValue(needIndex, 0.0);
      }
      goal.mediator  = NULL;
      goal.goalValue = goalValue;
      goal.enabled   = true;
      goals.push_back(goal);
      goalIndex = (int)goals.size() - 1;

      // Add existing receptors.
      for (int i = 0, n = goals.size() - 1; i < n; i++)
      {
         for (int j = 0, k = goals[i].receptors.size(); j < k; j++)
         {
            if (goalSuperset(goalIndex, ((Mona::Receptor *)goals[i].receptors[j])->centroid))
            {
               bool duplicate = false;
               for (int p = 0, q = goals[goalIndex].receptors.size(); p < q; p++)
               {
                  if (goals[goalIndex].receptors[p] == goals[i].receptors[j])
                  {
                     duplicate = true;
                     break;
                  }
               }
               if (!duplicate)
               {
                  goals[goalIndex].receptors.push_back(goals[i].receptors[j]);
                  NEED value = ((Mona::Receptor *)goals[i].receptors[j])->goals.getValue(needIndex);
                  ((Mona::Receptor *)goals[i].receptors[j])->goals.setValue(needIndex, value + goalValue);
               }
            }
         }
      }
   }
   return(goalIndex);
}


int Homeostat::addGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode,
                       NEED goalValue)
{
   return(addGoal(sensors, sensorMode, NULL, goalValue));
}


// Add receptor to existing goals.
void Homeostat::addGoalReceptor(void *receptor)
{
   // Receptor already added?
   for (int i = 0, n = (int)goals.size(); i < n; i++)
   {
      for (int j = 0, k = goals[i].receptors.size(); j < k; j++)
      {
         if (goals[i].receptors[j] == receptor)
         {
            return;
         }
      }
   }

   // Add receptor to goals having superset sensors.
   ((Mona::Receptor *)receptor)->goals.setValue(needIndex, 0.0);
   for (int i = 0, n = (int)goals.size(); i < n; i++)
   {
      if (goals[i].sensorMode == ((Mona::Receptor *)receptor)->sensorMode)
      {
         if (goalSuperset(i, ((Mona::Receptor *)receptor)->centroid))
         {
            NEED value = ((Mona::Receptor *)receptor)->goals.getValue(needIndex);
            ((Mona::Receptor *)receptor)->goals.setValue(needIndex, value + goals[i].goalValue);
            goals[i].receptors.push_back(receptor);
         }
      }
   }
}


// Add goal mediator.
int Homeostat::addGoalMediator(void *mediator, NEED goalValue)
{
   int  goalIndex;
   Goal goal;

   // Check for duplicate.
   if ((goalIndex = findGoalMediator(mediator)) == -1)
   {
      // Add entry.
      goal.sensorMode      = 0;
      goal.pendingReceptor = NULL;
      goal.motor           = NULL;
      goal.mediator        = mediator;
      ((Mona::Mediator *)goal.mediator)->goals.setValue(needIndex, goalValue);
      goal.goalValue = goalValue;
      goal.enabled   = true;
      goals.push_back(goal);
      goalIndex = (int)goals.size() - 1;
   }
   return(goalIndex);
}


// Find index of goal matching sensors, sensor mode
// and motor. Return -1 for no match.
int Homeostat::findGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode,
                        void *motor)
{
   int i = -1;

   if ((i = findGoal(sensors, sensorMode)) != -1)
   {
      if (goals[i].motor == motor)
      {
         return(i);
      }
      else
      {
         return(-1);
      }
   }
   return(-1);
}


// Find index of goal matching sensors and sensor mode.
// Return -1 for no match.
int Homeostat::findGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode)
{
   vector<SENSOR> sensorsWork;

   // Apply the sensor mode.
   mona->applySensorMode(sensors, sensorsWork, sensorMode);

   for (int i = 0, j = (int)goals.size(); i < j; i++)
   {
      if ((goals[i].sensors.size() > 0) &&
          (goals[i].sensorMode == sensorMode) &&
          (Mona::Receptor::sensorDistance(&goals[i].sensors, &sensorsWork)
           <= mona->sensorModes[sensorMode]->resolution))
      {
         return(i);
      }
   }
   return(-1);
}


// Find goal matching receptor.
// Return false for not found.
bool Homeostat::findGoalReceptor(void *receptor)
{
   for (int i = 0, n = (int)goals.size(); i < n; i++)
   {
      for (int j = 0, k = goals[i].receptors.size(); j < k; j++)
      {
         if (goals[i].receptors[j] == receptor)
         {
            return(true);
         }
      }
   }
   return(false);
}


// Find index of goal matching mediator.
// Return -1 for no match.
int Homeostat::findGoalMediator(void *mediator)
{
   for (int i = 0, j = (int)goals.size(); i < j; i++)
   {
      if (goals[i].mediator == mediator)
      {
         return(i);
      }
   }
   return(-1);
}


// Get goal information at index.
bool Homeostat::getGoalInfo(int goalIndex, vector<SENSOR>& sensors,
                            SENSOR_MODE& sensorMode, void **motor,
                            NEED& goalValue, bool& enabled)
{
   if ((goalIndex < 0) || (goalIndex >= (int)goals.size()))
   {
      return(false);
   }
   sensors    = goals[goalIndex].sensors;
   sensorMode = goals[goalIndex].sensorMode;
   *motor     = goals[goalIndex].motor;
   goalValue  = goals[goalIndex].goalValue;
   enabled    = goals[goalIndex].enabled;
   return(true);
}


// Get receptors for goal at index.
void Homeostat::getGoalReceptors(int goalIndex, vector<void *>& receptors)
{
   receptors.clear();
   if ((goalIndex >= 0) && (goalIndex < (int)goals.size()))
   {
      for (int i = 0, j = goals[goalIndex].receptors.size(); i < j; i++)
      {
         receptors.push_back(goals[goalIndex].receptors[i]);
      }
   }
}


// Get motor for goal at index.
void *Homeostat::getGoalMotor(int goalIndex)
{
   if ((goalIndex < 0) || (goalIndex >= (int)goals.size()))
   {
      return(NULL);
   }
   else
   {
      return(goals[goalIndex].motor);
   }
}


// Get mediator for goal at index.
void *Homeostat::getGoalMediator(int goalIndex)
{
   if ((goalIndex < 0) || (goalIndex >= (int)goals.size()))
   {
      return(NULL);
   }
   else
   {
      return(goals[goalIndex].mediator);
   }
}


// Is goal enabled?
bool Homeostat::isGoalEnabled(int goalIndex)
{
   if ((goalIndex >= 0) && (goalIndex < (int)goals.size()))
   {
      return(goals[goalIndex].enabled);
   }
   else
   {
      return(false);
   }
}


// Enable goal.
bool Homeostat::enableGoal(int goalIndex)
{
   if ((goalIndex >= 0) && (goalIndex < (int)goals.size()))
   {
      goals[goalIndex].enabled = true;
      return(true);
   }
   else
   {
      return(false);
   }
}


// Disable goal.
bool Homeostat::disableGoal(int goalIndex)
{
   if ((goalIndex >= 0) && (goalIndex < (int)goals.size()))
   {
      goals[goalIndex].enabled = false;
      return(true);
   }
   else
   {
      return(false);
   }
}


// Remove goal at index.
bool Homeostat::removeGoal(int goalIndex)
{
   if ((goalIndex < 0) || (goalIndex >= (int)goals.size()))
   {
      return(false);
   }

   // Clear goal value of associated receptors.
   for (int i = 0, j = goals[goalIndex].receptors.size(); i < j; i++)
   {
      NEED value = ((Mona::Receptor *)goals[goalIndex].receptors[i])->goals.getValue(needIndex);
      ((Mona::Receptor *)goals[goalIndex].receptors[i])->goals.setValue(needIndex, value - goals[goalIndex].goalValue);
   }

   // Clear goal value of associated mediator.
   if (goals[goalIndex].mediator != NULL)
   {
      ((Mona::Mediator *)goals[goalIndex].mediator)->goals.setValue(needIndex, 0.0);
   }

   // Remove entry.
   goals.erase(goals.begin() + goalIndex);

   return(true);
}


// Remove neuron from goals.
void Homeostat::removeNeuron(void *neuron)
{
   bool done;

   done = false;
   while (!done)
   {
      done = true;
      for (int i = 0, n = (int)goals.size(); i < n; i++)
      {
         if (goals[i].motor == neuron)
         {
            for (int j = 0, k = goals[i].receptors.size(); j < k; j++)
            {
               NEED value = ((Mona::Receptor *)goals[i].receptors[j])->goals.getValue(needIndex);
               ((Mona::Receptor *)goals[i].receptors[j])->goals.setValue(needIndex, value - goals[i].goalValue);
            }
            goals[i].receptors.clear();
            goals[i].motor = NULL;
         }
         else if (goals[i].mediator == neuron)
         {
            ((Mona::Mediator *)goals[i].mediator)->goals.setValue(needIndex, 0.0);
         }
         else
         {
            for (int j = 0, k = goals[i].receptors.size(); j < k; j++)
            {
               if (goals[i].receptors[j] == neuron)
               {
                  NEED value = ((Mona::Receptor *)goals[i].receptors[j])->goals.getValue(needIndex);
                  ((Mona::Receptor *)goals[i].receptors[j])->goals.setValue(needIndex, value - goals[i].goalValue);
                  goals[i].receptors.erase(goals[i].receptors.begin() + j);
                  break;
               }
            }
         }
      }
   }
}


// Update homeostat based on receptor firing.
void Homeostat::receptorUpdate(void *receptor)
{
   for (int i = 0, n = (int)goals.size(); i < n; i++)
   {
      for (int j = 0, k = goals[i].receptors.size(); j < k; j++)
      {
         if (goals[i].receptors[j] == receptor)
         {
            if (goals[i].motor == NULL)
            {
               need -= goals[i].goalValue;
               if (need < 0.0)
               {
                  need = 0.0;
               }
            }
            else
            {
               goals[i].pendingReceptor = receptor;
               NEED value = ((Mona::Receptor *)receptor)->goals.getValue(needIndex);
               ((Mona::Receptor *)receptor)->goals.setValue(needIndex, value - goals[i].goalValue);
               value = ((Mona::Motor *)goals[i].motor)->goals.getValue(needIndex);
               ((Mona::Motor *)goals[i].motor)->goals.setValue(needIndex, value + goals[i].goalValue);
            }
         }
      }
   }
}


// Update homeostat based on motor firing.
void Homeostat::motorUpdate()
{
   for (int i = 0, j = (int)goals.size(); i < j; i++)
   {
      if (goals[i].pendingReceptor != NULL)
      {
         Mona::Motor *motor = (Mona::Motor *)goals[i].motor;
         if (motor->firingStrength > 0.0)
         {
            need -= goals[i].goalValue;
            if (need < 0.0)
            {
               need = 0.0;
            }
         }
         if (!motor->isPlaceMotor() || (motor->firingStrength > 0.0))
         {
            Mona::Receptor *receptor = (Mona::Receptor *)goals[i].pendingReceptor;
            NEED           value     = receptor->goals.getValue(needIndex);
            receptor->goals.setValue(needIndex, value + goals[i].goalValue);
            motor->goals.setValue(needIndex, 0.0);
            goals[i].pendingReceptor = NULL;
         }
      }
   }

   // Time to induce need?
   if (frequency > 0)
   {
      freqTimer++;
      if (freqTimer >= frequency)
      {
         freqTimer = 0;
         need      = periodicNeed;
      }
   }
}


// Update homeostat based on mediator.
void Homeostat::mediatorUpdate(void *mediator, double firingStrength)
{
   // Update the need value when sensors match.
   for (int i = 0, j = (int)goals.size(); i < j; i++)
   {
      if (goals[i].mediator == mediator)
      {
         if (firingStrength > 1.0)
         {
            firingStrength = 1.0;
         }
         if (firingStrength > 0.0)
         {
            need -= goals[i].goalValue * firingStrength;
            if (need < 0.0)
            {
               need = 0.0;
            }
         }
      }
   }
}


// Load homeostat.
void Homeostat::load(char *filename)
{
   FILE *fp;

   if ((fp = FOPEN_READ(filename)) == NULL)
   {
      fprintf(stderr, "Cannot load homeostat from file %s\n", filename);
      exit(1);
   }
   load(fp);
   FCLOSE(fp);
}


void Homeostat::load(FILE *fp)
{
   int    i, j, p, q;
   Goal   g;
   SENSOR s;
   ID     id;

   need      = 0.0;
   needDelta = 0.0;
   FREAD_INT(&needIndex, fp);
   FREAD_DOUBLE(&defaultNeed, fp);
   FREAD_DOUBLE(&periodicNeed, fp);
   FREAD_INT(&frequency, fp);
   FREAD_INT(&freqTimer, fp);
   goals.clear();
   FREAD_INT(&j, fp);
   for (i = 0; i < j; i++)
   {
      g.sensors.clear();
      FREAD_INT(&q, fp);
      for (p = 0; p < q; p++)
      {
         FREAD_FLOAT(&s, fp);
         g.sensors.push_back(s);
      }
      FREAD_INT(&g.sensorMode, fp);
      g.receptors.clear();
      FREAD_INT(&q, fp);
      for (p = 0; p < q; p++)
      {
         FREAD_LONG_LONG(&id, fp);
         g.receptors.push_back((void *)mona->findByID(id));
      }
      g.pendingReceptor = NULL;
      FREAD_LONG_LONG(&id, fp);
      if (id != NULL_ID)
      {
         g.motor = (void *)mona->findByID(id);
      }
      else
      {
         g.motor = NULL;
      }
      FREAD_LONG_LONG(&id, fp);
      g.mediator = NULL;
      if (id != NULL_ID)
      {
         g.mediator = (void *)mona->findByID(id);
      }
      FREAD_DOUBLE(&g.goalValue, fp);
      FREAD_BOOL(&g.enabled, fp);
      goals.push_back(g);
   }
}


// Save homeostat.
void Homeostat::save(char *filename)
{
   FILE *fp;

   if ((fp = FOPEN_WRITE(filename)) == NULL)
   {
      fprintf(stderr, "Cannot save homeostat to file %s\n", filename);
      exit(1);
   }
   save(fp);
   FCLOSE(fp);
}


// When changing format increment FORMAT in mona.hpp
void Homeostat::save(FILE *fp)
{
   int i, j, p, q;
   ID  id;

   FWRITE_INT(&needIndex, fp);
   FWRITE_DOUBLE(&defaultNeed, fp);
   FWRITE_DOUBLE(&periodicNeed, fp);
   FWRITE_INT(&frequency, fp);
   FWRITE_INT(&freqTimer, fp);
   j = (int)goals.size();
   FWRITE_INT(&j, fp);
   for (i = 0; i < j; i++)
   {
      q = (int)goals[i].sensors.size();
      FWRITE_INT(&q, fp);
      for (p = 0; p < q; p++)
      {
         FWRITE_FLOAT(&goals[i].sensors[p], fp);
      }
      FWRITE_INT(&goals[i].sensorMode, fp);
      q = (int)goals[i].receptors.size();
      FWRITE_INT(&q, fp);
      for (p = 0; p < q; p++)
      {
         id = ((Mona::Receptor *)(goals[i].receptors[p]))->id;
         FWRITE_LONG_LONG(&id, fp);
      }
      id = NULL_ID;
      if (goals[i].motor != NULL)
      {
         id = ((Mona::Motor *)(goals[i].motor))->id;
      }
      FWRITE_LONG_LONG(&id, fp);
      id = NULL_ID;
      if (goals[i].mediator != NULL)
      {
         id = ((Mona::Mediator *)(goals[i].mediator))->id;
      }
      FWRITE_LONG_LONG(&id, fp);
      FWRITE_DOUBLE(&goals[i].goalValue, fp);
      FWRITE_BOOL(&goals[i].enabled, fp);
   }
}


// Print homeostat.
void Homeostat::print(FILE *out)
{
   fprintf(out, "<homeostat>");
   fprintf(out, "<need>%f</need>", need);
   fprintf(out, "<needIndex>%d</needIndex>", needIndex);
   fprintf(out, "<needDelta>%f</needDelta>", needDelta);
   fprintf(out, "<defaultNeed>%f</defaultNeed>", defaultNeed);
   fprintf(out, "<periodicNeed>%f</periodicNeed>", periodicNeed);
   fprintf(out, "<frequency>%d</frequency>", frequency);
   fprintf(out, "<sensory_response_goals>");
   for (int i = 0, n = (int)goals.size(); i < n; i++)
   {
      fprintf(out, "<sensory_response_goal>");
      fprintf(out, "<sensors>");
      for (int j = 0, k = (int)goals[i].sensors.size(); j < k; j++)
      {
         fprintf(out, "<value>%f</value>", goals[i].sensors[j]);
      }
      fprintf(out, "</sensors>");
      fprintf(out, "<sensorMode>%d</sensorMode>", goals[i].sensorMode);
      fprintf(out, "<receptors>");
      for (int j = 0, k = (int)goals[i].receptors.size(); j < k; j++)
      {
         ID id = ((Mona::Receptor *)(goals[i].receptors[j]))->id;
         fprintf(out, "<receptor_id>%lld</receptor_id>", id);
      }
      fprintf(out, "</receptors>");
      if (goals[i].motor == NULL)
      {
         fprintf(out, "<motor_id>null</motor_id>");
      }
      else
      {
         ID id = ((Mona::Motor *)(goals[i].motor))->id;
         fprintf(out, "<motor_id>%lld</motor_id>", id);
      }
      if (goals[i].mediator == NULL)
      {
         fprintf(out, "<mediator_id>null</mediator_id>");
      }
      else
      {
         ID id = ((Mona::Mediator *)(goals[i].mediator))->id;
         fprintf(out, "<mediator_id>%lld</mediator_id>", id);
      }
      fprintf(out, "<goalValue>%f</goalValue>", goals[i].goalValue);
      if (goals[i].enabled)
      {
         fprintf(out, "<enabled>true</enabled>");
      }
      else
      {
         fprintf(out, "<enabled>false</enabled>");
      }
      fprintf(out, "</sensory_response_goal>");
   }
   fprintf(out, "</sensory_response_goals>");
   fprintf(out, "</homeostat>");
}


// Is goal superset of sensors?
bool Homeostat::goalSuperset(int goalIndex, vector<SENSOR>& sensors)
{
   if (goals[goalIndex].sensors.size() != sensors.size())
   {
      return(false);
   }
   for (int i = 0, j = (int)goals[goalIndex].sensors.size(); i < j; i++)
   {
      if (goals[goalIndex].sensors[i] != DONT_CARE)
      {
         if (goals[goalIndex].sensors[i] != sensors[i])
         {
            return(false);
         }
      }
   }
   return(true);
}
