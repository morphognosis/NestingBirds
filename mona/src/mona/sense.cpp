// For conditions of distribution and use, see copyright notice in mona.hpp

#include "mona.hpp"

// Sense environment.
void
Mona::sense()
{
   SENSOR_MODE sensorMode;

   vector<SENSOR>     sensorsWork;
   Receptor           *receptor;
   SENSOR             distance;
   bool               addReceptor;
   vector<Receptor *> oldReceptorSet, newReceptorSet;

#ifdef MONA_TRACE
   if (traceSense)
   {
      printf("***Sense phase***\n");
      printf("Sensors: ");
      for (int i = 0, j = (int)sensors.size(); i < j; i++)
      {
         printf("%f ", sensors[i]);
      }
      printf("\n");
   }
#endif

   // Update need based on sensors.
   for (int i = 0; i < numNeeds; i++)
   {
      homeostats[i]->sensorsUpdate();
   }

   // Clear receptor firings.
   for (int i = 0, j = (int)receptors.size(); i < j; i++)
   {
      receptor = receptors[i];
      receptor->firingStrength = 0.0;
   }

   // Add base sensor mode?
   if (sensorModes.size() == 0)
   {
      vector<bool> mask;
      for (int i = 0; i < numSensors; i++)
      {
         mask.push_back(true);
      }
      addSensorMode(mask);
   }

   // Fire receptors matching sensor modes.
   int n = (int)sensorModes.size();
   for (sensorMode = 0; sensorMode < n; sensorMode++)
   {
      // Apply sensor mode to sensors.
      applySensorMode(sensors, sensorsWork, sensorMode);

      // Get firing receptor based on sensor vector and mode.
      receptor = getCentroidReceptor(sensorsWork, sensorMode, distance);

      // Create a receptor that matches sensor vector?
      addReceptor = false;
      if ((receptor == NULL) || ((receptor != NULL) &&
                                 (distance > sensorModes[sensorMode]->resolution) &&
                                 (distance > NEARLY_ZERO)))
      {
         receptor    = newReceptor(sensorsWork, sensorMode);
         addReceptor = true;
      }

      // Incorporate into sensor mode sets.
      if (addReceptor)
      {
         for (int i = 0, n = (int)oldReceptorSet.size(); i < n; i++)
         {
             int j, k;
            for (j = 0, k = (int)sensorModes[sensorMode]->subsets.size(); j < k; j++)
            {
               if (oldReceptorSet[i]->sensorMode == sensorModes[sensorMode]->subsets[j])
               {
                  receptor->subSensorModes.push_back(oldReceptorSet[i]);
                  oldReceptorSet[i]->superSensorModes.push_back(receptor);
                  break;
               }
            }
            if (j < (int)sensorModes[sensorMode]->subsets.size())
            {
               continue;
            }
            for (j = 0, k = (int)sensorModes[sensorMode]->supersets.size(); j < k; j++)
            {
               if (oldReceptorSet[i]->sensorMode == sensorModes[sensorMode]->supersets[j])
               {
                  receptor->superSensorModes.push_back(oldReceptorSet[i]);
                  oldReceptorSet[i]->subSensorModes.push_back(receptor);
                  break;
               }
            }
         }
      }
      for (int i = 0, n = (int)newReceptorSet.size(); i < n; i++)
      {
          int j, k;
         for (j = 0, k = (int)sensorModes[sensorMode]->subsets.size(); j < k; j++)
         {
            if (newReceptorSet[i]->sensorMode == sensorModes[sensorMode]->subsets[j])
            {
               receptor->subSensorModes.push_back(newReceptorSet[i]);
               newReceptorSet[i]->superSensorModes.push_back(receptor);
               break;
            }
         }
         if (j < (int)sensorModes[sensorMode]->subsets.size())
         {
            continue;
         }
         for (j = 0, k = (int)sensorModes[sensorMode]->supersets.size(); j < k; j++)
         {
            if (newReceptorSet[i]->sensorMode == sensorModes[sensorMode]->supersets[j])
            {
               receptor->superSensorModes.push_back(newReceptorSet[i]);
               newReceptorSet[i]->subSensorModes.push_back(receptor);
               break;
            }
         }
      }

      if (addReceptor)
      {
         newReceptorSet.push_back(receptor);
      }
      else
      {
         oldReceptorSet.push_back(receptor);
      }

      // Fire receptor.
      receptor->firingStrength = 1.0;

      // Update receptor goal value.
      receptor->updateGoalValue();

#ifdef MONA_TRACE
      if (traceSense)
      {
         printf("Receptor firing: centroid=[ ");
         for (int i = 0; i < numSensors; i++)
         {
            printf("%f ", receptor->centroid[i]);
         }
         printf("], sensorMode=%d\n", receptor->sensorMode);
      }
#endif
#ifdef MONA_TRACKING
      receptor->tracker.fire = true;
#endif
   }
}


// Apply sensor mode to sensors.
void Mona::applySensorMode(vector<SENSOR>& sensorsIn,
                           vector<SENSOR>& sensorsOut, SENSOR_MODE sensorMode)
{
   // Defaulting to base sensor mode?
   if (sensorModes.size() == 0)
   {
      assert(sensorMode == 0);
      vector<bool> mask;
      for (int i = 0; i < numSensors; i++)
      {
         mask.push_back(true);
      }
      addSensorMode(mask);
   }

   sensorsOut.clear();
   for (int i = 0, j = (int)sensorsIn.size(); i < j; i++)
   {
      if (sensorModes[sensorMode]->mask[i])
      {
         sensorsOut.push_back(sensorsIn[i]);
      }
      else
      {
         sensorsOut.push_back(0.0f);
      }
   }
}


void Mona::applySensorMode(vector<SENSOR>& sensors, SENSOR_MODE sensorMode)
{
   vector<SENSOR> sensorsWork;
   applySensorMode(sensors, sensorsWork, sensorMode);
   for (int i = 0, j = (int)sensors.size(); i < j; i++)
   {
      sensors[i] = sensorsWork[i];
   }
}


// Find the receptor containing the centroid closest to
// the sensor vector for the current sensor mode.
// Also return the centroid-vector distance.
Mona::Receptor *
Mona::getCentroidReceptor(vector<SENSOR>& sensors,
                          SENSOR_MODE sensorMode, SENSOR& distance)
{
   RDtree::RDsearch *result = sensorCentroids[sensorMode]->search((void *)&sensors, 1);

   if (result != NULL)
   {
      distance = result->distance;
      Receptor *receptor = (Receptor *)(result->node->client);
      delete result;
      return(receptor);
   }
   else
   {
      distance = 0.0f;
      return(NULL);
   }
}


// Get distance from centroid to given sensor vector.
Mona::SENSOR Mona::Receptor::centroidDistance(vector<SENSOR>& sensors)
{
   return(sensorDistance(&centroid, &sensors));
}


// Get distance between sensor vectors.
// Distance metric is Euclidean distance squared.
Mona::SENSOR Mona::Receptor::sensorDistance(vector<SENSOR> *sensorsA,
                                            vector<SENSOR> *sensorsB)
{
   SENSOR d;
   SENSOR dist = 0.0f;

   for (int i = 0, j = (int)sensorsA->size(); i < j; i++)
   {
      d     = (*sensorsA)[i] - (*sensorsB)[i];
      dist += (d * d);
   }
   return(dist);
}


// RDtree sensor vector search.
Mona::SENSOR Mona::Receptor::patternDistance(void *sensorsA, void *sensorsB)
{
   return(sensorDistance((vector<SENSOR> *)sensorsA, (vector<SENSOR> *)sensorsB));
}


void *Mona::Receptor::loadPattern(void *mona, FILE *fp)
{
   SENSOR s;

   vector<SENSOR> *sensors = new vector<SENSOR>();
   assert(sensors != NULL);
   for (int i = 0; i < ((Mona *)mona)->numSensors; i++)
   {
      FREAD_FLOAT(&s, fp);
      sensors->push_back(s);
   }
   return((void *)sensors);
}


void Mona::Receptor::savePattern(void *sensorsIn, FILE *fp)
{
   SENSOR s;

   vector<SENSOR> *sensors = (vector<SENSOR> *)sensorsIn;
   for (int i = 0, j = (int)sensors->size(); i < j; i++)
   {
      s = (*sensors)[i];
      FWRITE_FLOAT(&s, fp);
   }
}


void *Mona::Receptor::loadClient(void *mona, FILE *fp)
{
   ID id;

   FREAD_LONG_LONG(&id, fp);
   return((void *)((Mona *)mona)->findByID(id));
}


void Mona::Receptor::saveClient(void *receptor, FILE *fp)
{
   FWRITE_LONG_LONG(&((Receptor *)receptor)->id, fp);
}


void Mona::Receptor::deletePattern(void *pattern)
{
   delete (vector<SENSOR> *)pattern;
}


// Set sensor resolution.
bool Mona::setSensorResolution(SENSOR sensorResolution)
{
   if ((int)receptors.size() == 0)
   {
      SENSOR_RESOLUTION = sensorResolution;
      return(true);
   }
   else
   {
      return(false);
   }
}


// Add sensor mode.
int Mona::addSensorMode(vector<bool>& sensorMask, SENSOR sensorResolution)
{
   // Must add modes before cycling.
   if ((int)receptors.size() > 0)
   {
      return(-1);
   }

   // Duplicate?
   for (int i = 0, n = (int)sensorModes.size(); i < n; i++)
   {
      bool duplicate = true;
      for (int j = 0, k = (int)sensorMask.size(); j < k; j++)
      {
         if (sensorModes[i]->mask[j] != sensorMask[j])
         {
            duplicate = false;
            break;
         }
      }
      if (duplicate)
      {
         sensorModes[i]->resolution = sensorResolution;
         return(sensorModes[i]->mode);
      }
   }

   // Create sensor mode.
   SensorMode *s = new SensorMode();
   assert(s != NULL);
   s->init(sensorMask, sensorResolution, &sensorModes);

   // Create associated centroid search tree.
   RDtree *t = new RDtree(Mona::Receptor::patternDistance,
                          Mona::Receptor::deletePattern);
   assert(t != NULL);
   sensorCentroids.push_back(t);

   return(s->mode);
}


int Mona::addSensorMode(vector<bool>& sensorMask)
{
   return(addSensorMode(sensorMask, SENSOR_RESOLUTION));
}

// Delete sensor mode.
bool Mona::delSensorMode(int mode)
{
    // Must delete modes before cycling.
    if ((int)receptors.size() > 0)
    {
        return(false);
    }

    // Mode exists?
    if ((int)sensorModes.size() <= mode)
    {
        return(false);
    }

    // Delete mode.
    sensorModes.erase(sensorModes.begin() + mode);

    // Delete centroid.
    sensorCentroids.erase(sensorCentroids.begin() + mode);

    return(true);
}

// Update goal value.
void Mona::Receptor::updateGoalValue()
{
   VALUE_SET needs, needDeltas;

   if (!mona->LEARN_RECEPTOR_GOAL_VALUE)
   {
      return;
   }
   needs.alloc(mona->numNeeds);
   needDeltas.alloc(mona->numNeeds);
   for (int i = 0; i < mona->numNeeds; i++)
   {
      needs.set(i, mona->homeostats[i]->getNeed());
      needDeltas.set(i, mona->homeostats[i]->getAndClearNeedDelta());
   }
   goals.update(needs, needDeltas);
}
