// For conditions of distribution and use, see copyright notice in mona.hpp

#include "mona.hpp"

const Homeostat::ID Homeostat::NULL_ID = (ID)(-1);

// Homeostat constructors.
Homeostat::Homeostat()
{
   need         = 0.0;
   needIndex    = 0;
   needDelta    = 0.0;
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
// Replace duplicate response and goal value.
int Homeostat::addGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode,
                       RESPONSE response, NEED goalValue)
{
   int goalIndex, i;

   vector<SENSOR> sensorsWork;
   Mona::Receptor *receptor;
   SENSOR         distance;
   Goal           goal;

   // Check for duplicate.
   if ((goalIndex = findGoal(sensors, sensorMode, false)) != -1)
   {
      ((Mona::Receptor *)goals[goalIndex].receptor)->goals.setValue(needIndex, goalValue);
      if (goals[goalIndex].response != NULL_RESPONSE)
      {
         // Clear old motor goal value.
         ((Mona::Motor *)goals[goalIndex].motor)->goals.setValue(needIndex, 0.0);
      }
      goals[goalIndex].response = response;
      if (response != NULL_RESPONSE)
      {
         goals[goalIndex].motor = (void *)mona->findMotorByResponse(response);
         assert(goals[goalIndex].motor != NULL);
         ((Mona::Motor *)goals[goalIndex].motor)->goals.setValue(needIndex, 0.0);
      }
      else
      {
         goals[goalIndex].motor = NULL;
      }
      goals[goalIndex].goalValue = goalValue;
   }
   else
   {
      // Add entry.
      mona->applySensorMode(sensors, sensorsWork, sensorMode);
      for (i = 0; i < (int)sensorsWork.size(); i++)
      {
         goal.sensors.push_back(sensorsWork[i]);
      }
      goal.sensorMode = sensorMode;
      receptor        = mona->getCentroidReceptor(sensorsWork, sensorMode, distance);
      if ((receptor == NULL) ||
          ((receptor != NULL) && (distance > mona->sensorModes[sensorMode]->resolution)))
      {
         receptor = mona->newReceptor(sensorsWork, sensorMode);
      }
      receptor->goals.setValue(needIndex, goalValue);
      goal.receptor = (void *)receptor;
      goal.response = response;
      if (response != NULL_RESPONSE)
      {
         goal.motor = (void *)mona->findMotorByResponse(response);
         assert(goal.motor != NULL);
         ((Mona::Motor *)goal.motor)->goals.setValue(needIndex, 0.0);
      }
      else
      {
         goal.motor = NULL;
      }
      goal.goalValue = goalValue;
      goal.enabled   = true;
      goals.push_back(goal);
      goalIndex = (int)goals.size() - 1;
   }
   return(goalIndex);
}


int Homeostat::addGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode,
                       NEED goalValue)
{
   return(addGoal(sensors, sensorMode, NULL_RESPONSE, goalValue));
}


// Find index of goal matching sensors, sensor mode
// and response. Return -1 for no match.
int Homeostat::findGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode,
                        RESPONSE response)
{
   int i;

   if ((i = findGoal(sensors, sensorMode)) != -1)
   {
      if (goals[i].response == response)
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

   for (int i = 0; i < (int)goals.size(); i++)
   {
      if ((goals[i].sensorMode == sensorMode) &&
          (Mona::Receptor::sensorDistance(&goals[i].sensors, &sensorsWork)
           <= mona->sensorModes[sensorMode]->resolution))
      {
         return(i);
      }
   }
   return(-1);
}


// Get goal information at index.
bool Homeostat::getGoalInfo(int goalIndex, vector<SENSOR>& sensors,
                            SENSOR_MODE& sensorMode, RESPONSE& response,
                            NEED& goalValue, bool& enabled)
{
   if ((goalIndex < 0) || (goalIndex >= (int)goals.size()))
   {
      return(false);
   }
   sensors    = goals[goalIndex].sensors;
   sensorMode = goals[goalIndex].sensorMode;
   response   = goals[goalIndex].response;
   goalValue  = goals[goalIndex].goalValue;
   enabled    = goals[goalIndex].enabled;
   return(true);
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
      ((Mona::Receptor *)goals[goalIndex].receptor)->goals.setValue(needIndex,
                                                                    goals[goalIndex].goalValue);
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
      ((Mona::Receptor *)goals[goalIndex].receptor)->goals.setValue(needIndex, 0.0);
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
   ((Mona::Receptor *)goals[goalIndex].receptor)->goals.setValue(needIndex, 0.0);

   // Clear goal value of associated motor.
   if (goals[goalIndex].response != NULL_RESPONSE)
   {
      ((Mona::Motor *)goals[goalIndex].motor)->goals.setValue(needIndex, 0.0);
   }

   // Remove entry.
   goals.erase(goals.begin() + goalIndex);

   return(true);
}


// Remove neuron from goals.
void Homeostat::removeNeuron(void *neuron)
{
   int  i;
   bool done;

   done = false;
   while (!done)
   {
      done = true;
      for (i = 0; i < (int)goals.size(); i++)
      {
         if ((goals[i].receptor == neuron) || (goals[i].motor == neuron))
         {
            ((Mona::Receptor *)goals[i].receptor)->goals.setValue(needIndex, 0.0);
            if (goals[i].response != NULL_RESPONSE)
            {
               ((Mona::Motor *)goals[i].motor)->goals.setValue(needIndex, 0.0);
            }
            goals.erase(goals.begin() + i);
            done = false;
            break;
         }
      }
   }
}


// Update homeostat based on sensors.
void Homeostat::sensorsUpdate()
{
   int i;

   vector<SENSOR> sensors;

   // Update the need value when sensors match.
   for (i = 0; i < (int)goals.size(); i++)
   {
      mona->applySensorMode(mona->sensors, sensors, goals[i].sensorMode);
      if (Mona::Receptor::sensorDistance(&goals[i].sensors, &sensors)
          <= mona->sensorModes[goals[i].sensorMode]->resolution)
      {
         if (goals[i].response != NULL_RESPONSE)
         {
            // Shift goal value to motor.
            ((Mona::Motor *)goals[i].motor)->goals.setValue(needIndex, goals[i].goalValue);
            ((Mona::Receptor *)goals[i].receptor)->goals.setValue(needIndex, 0.0);
         }
         else
         {
            need -= goals[i].goalValue;
            if (need < 0.0)
            {
               need = 0.0;
            }
         }
      }
   }
}


// Update homeostat based on response.
void Homeostat::responseUpdate()
{
   // Update the need value when response matches.
   for (int i = 0; i < (int)goals.size(); i++)
   {
      if (goals[i].response != NULL_RESPONSE)
      {
         if (goals[i].response == mona->response)
         {
            need -= ((Mona::Motor *)goals[i].motor)->goals.getValue(needIndex);
            if (need < 0.0)
            {
               need = 0.0;
            }
         }
         ((Mona::Motor *)goals[i].motor)->goals.setValue(needIndex, 0.0);
         ((Mona::Receptor *)goals[i].receptor)->goals.setValue(needIndex, goals[i].goalValue);
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

   FREAD_DOUBLE(&need, fp);
   FREAD_INT(&needIndex, fp);
   FREAD_DOUBLE(&needDelta, fp);
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
      FREAD_LONG_LONG(&id, fp);
      g.receptor = (void *)mona->findByID(id);
      FREAD_INT(&g.response, fp);
      if (g.response != NULL_RESPONSE)
      {
         g.motor = mona->findMotorByResponse(g.response);
      }
      else
      {
         g.motor = NULL;
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

   FWRITE_DOUBLE(&need, fp);
   FWRITE_INT(&needIndex, fp);
   FWRITE_DOUBLE(&needDelta, fp);
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
      FWRITE_LONG_LONG(&((Mona::Receptor *)goals[i].receptor)->id, fp);
      FWRITE_INT(&goals[i].response, fp);
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
   fprintf(out, "<periodicNeed>%f</periodicNeed>", periodicNeed);
   fprintf(out, "<frequency>%d</frequency>", frequency);
   fprintf(out, "<sensory_response_goals>");
   for (int i = 0; i < (int)goals.size(); i++)
   {
      fprintf(out, "<sensory_response_goal>");
      fprintf(out, "<sensors>");
      for (int j = 0; j < (int)goals[i].sensors.size(); j++)
      {
         fprintf(out, "<value>%f</value>", goals[i].sensors[j]);
      }
      fprintf(out, "</sensors>");
      fprintf(out, "<sensorMode>%d</sensorMode>", goals[i].sensorMode);
      if (goals[i].response == NULL_RESPONSE)
      {
         fprintf(out, "<response>null</response>");
      }
      else
      {
         fprintf(out, "<response>%d</response>", goals[i].response);
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
