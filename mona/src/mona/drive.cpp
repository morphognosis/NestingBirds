// For conditions of distribution and use, see copyright notice in mona.hpp

#include "mona.hpp"
#include <math.h>

// Drive need changes caused by goal values through network.
void
Mona::drive()
{
   int      i;
   Neuron   *neuron;
   Receptor *receptor;
   Motor    *motor;
   Mediator *mediator;

   list<Mediator *>::iterator mediatorItr;
   MotiveAccum                motiveAccum;
   VALUE_SET needs;

#ifdef MONA_TRACE
   if (traceDrive)
   {
      printf("***Drive phase***\n");
   }
#endif

   // Copy needs from homeostats to work set.
   needs.alloc(numNeeds);
   for (i = 0; i < numNeeds; i++)
   {
      needs.set(i, homeostats[i]->getNeed());
   }

#ifdef MONA_TRACE
   if (traceDrive)
   {
      printf("Needs: ");
      needs.print();
   }
#endif

   // Initialize drive.
   for (i = 0; i < (int)receptors.size(); i++)
   {
      neuron = (Neuron *)receptors[i];
      neuron->initDrive(needs);
   }
   for (i = 0; i < (int)motors.size(); i++)
   {
      neuron = (Neuron *)motors[i];
      neuron->initDrive(needs);
   }
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      neuron = (Neuron *)(*mediatorItr);
      neuron->initDrive(needs);
   }

   // Drive.
   for (i = 0; i < (int)receptors.size(); i++)
   {
      receptor = receptors[i];
      if (receptor->goals.getValue() != 0.0)
      {
         motiveAccum.init(needs);
         motiveAccum.accumGoals(receptor->goals);
         if (motiveAccum.getValue() != 0.0)
         {
            motiveAccum.init(needs);
            clearMotiveWork();
#ifdef MONA_TRACKING
            motiveAccum.drivers.clear();
#endif
            receptor->drive(motiveAccum);
            setMotives();
         }
      }
   }

   for (i = 0; i < (int)motors.size(); i++)
   {
      motor = motors[i];
      if (motor->goals.getValue() != 0.0)
      {
         motiveAccum.init(needs);
         motiveAccum.accumGoals(motor->goals);
         if (motiveAccum.getValue() != 0.0)
         {
            motiveAccum.init(needs);
            motiveAccum.init(needs);
            clearMotiveWork();
#ifdef MONA_TRACKING
            motiveAccum.drivers.clear();
#endif
            motor->drive(motiveAccum);
            setMotives();
         }
      }
   }

   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      NEED goalValue = mediator->goals.getValue();
      if (goalValue != 0.0)
      {
         if (!mediator->goalValueSubsumed())
         {
            motiveAccum.init(needs);
            motiveAccum.accumGoals(mediator->goals);
            if (motiveAccum.getValue() != 0.0)
            {
               motiveAccum.init(needs);
               clearMotiveWork();
#ifdef MONA_TRACKING
               motiveAccum.drivers.clear();
#endif
               mediator->drive(motiveAccum);
               setMotives();
            }
         }
      }
   }
   needs.clear();

   // Finalize motives.
   finalizeMotives();
}


// Is goal value subsumed by component?
bool
Mona::Mediator::goalValueSubsumed()
{
   int    i;
   NEED   goalValue;
   Neuron *cause      = this->cause;
   Neuron *response   = this->response;
   Neuron *effect     = this->effect;
   bool   *goalBlocks = new bool[mona->numNeeds];

   assert(goalBlocks != NULL);

   for (i = 0; i < mona->numNeeds; i++)
   {
      goalBlocks[i] = false;
   }
   while (effect != NULL)
   {
      for (i = 0; i < mona->numNeeds; i++)
      {
         goalValue = goals.getValue(i);
         if (goalValue > 0.0)
         {
            if ((cause->goals.getValue(i) >= goalValue) ||
                (effect->goals.getValue(i) >= goalValue) ||
                ((response != NULL) && (response->goals.getValue(i) >= goalValue)))
            {
               goalBlocks[i] = true;
            }
         }
         else
         {
            if ((cause->goals.getValue(i) <= goalValue) ||
                (effect->goals.getValue(i) <= goalValue) ||
                ((response != NULL) && (response->goals.getValue(i) <= goalValue)))
            {
               goalBlocks[i] = true;
            }
         }
      }
      for (i = 0; i < mona->numNeeds; i++)
      {
         if (!goalBlocks[i])
         {
            break;
         }
      }
      if (i == mona->numNeeds)
      {
         break;
      }
      if (effect->type == MEDIATOR)
      {
         cause    = ((Mediator *)effect)->cause;
         response = ((Mediator *)effect)->response;
         effect   = ((Mediator *)effect)->effect;
      }
      else
      {
         effect = NULL;
      }
   }
   delete [] goalBlocks;
   if (effect != NULL)
   {
      return(true);
   }
   else
   {
      return(false);
   }
}


// Initialize neuron drive.
void
Mona::Neuron::initDrive(VALUE_SET& needs)
{
   int           i;
   ENABLEMENT    up, down, e, ce, re, ee;
   Mediator      *mediator;
   struct Notify *notify;

   motive      = 0.0;
   motiveValid = false;
   motiveWork.init(needs);
   motiveWorkValid = false;
#ifdef MONA_TRACKING
   tracker.motivePaths.clear();
   tracker.motiveWorkPaths.clear();
#endif

   /*
    * Assign drive weights to destinations:
    *
    * The input motive is divided into the "down" that
    * is driven to mediator component events and the "up"
    * that is driven to parents. The down value is proportional
    * to the mediator effective enablement. The remaining
    * fraction goes up. For receptors, all motive is driven
    * to parents; for motors, none is.
    *
    * For distributing down, each event is weighted according to
    * its share of enabling.
    *
    * For distributing up, parents for which the component is
    * an effect are weighted. For each of these, its proportion
    * is how much enablement it has available to enable the child.
    */

   driveWeights.clear();

   // Divide the down amount.
   switch (type)
   {
   case RECEPTOR:
      up = 1.0;
      break;

   case MOTOR:
      return;

   case MEDIATOR:
      mediator = (Mediator *)this;
      down     = mediator->effectiveEnablement;
      up       = 1.0 - down;
      e        = mediator->getEnablement();
      ce       = re = ee = 0.0;

      // Distribute the down amount among components.
      re = (mediator->responseEnablings.getValue() / e) * down;
      if (mediator->response != NULL)
      {
         driveWeights[mediator->response] = re;
      }
      ee = mediator->effectEnablings.getValue();
      ee = driveWeights[mediator->effect] = (ee / e) * down;
      ce = down - (re + ee);
      driveWeights[mediator->cause] = ce;
      break;
   }

   // Distribute the up amount among parents.
   for (i = 0; i < (int)notifyList.size(); i++)
   {
      notify   = notifyList[i];
      mediator = notify->mediator;
      if (notify->eventType == EFFECT_EVENT)
      {
         driveWeights[mediator] = up *
                                  (1.0 - mediator->effectiveEnablingWeight);
      }
      else
      {
         driveWeights[mediator] = 0.0;
      }
   }
}


// Clear motive working accumulators.
void
Mona::clearMotiveWork()
{
   int    i;
   Neuron *neuron;

   list<Mediator *>::iterator mediatorItr;

   for (i = 0; i < (int)receptors.size(); i++)
   {
      neuron = (Neuron *)receptors[i];
      neuron->clearMotiveWork();
   }
   for (i = 0; i < (int)motors.size(); i++)
   {
      neuron = (Neuron *)motors[i];
      neuron->clearMotiveWork();
   }
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      neuron = (Neuron *)(*mediatorItr);
      neuron->clearMotiveWork();
   }
}


// Clear motive working accumulators.
void
Mona::Neuron::clearMotiveWork()
{
   motiveWork.reset();
   motiveWorkValid = false;
#ifdef MONA_TRACKING
   tracker.motiveWorkPaths.clear();
#endif
}


// Set motives.
void
Mona::setMotives()
{
   int    i;
   Neuron *neuron;

   list<Mediator *>::iterator mediatorItr;

   for (i = 0; i < (int)receptors.size(); i++)
   {
      neuron = (Neuron *)receptors[i];
      neuron->setMotive();
   }
   for (i = 0; i < (int)motors.size(); i++)
   {
      neuron = (Neuron *)motors[i];
      neuron->setMotive();
   }
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      neuron = (Neuron *)(*mediatorItr);
      neuron->setMotive();
   }
}


// Set neuron motive.
void
Mona::Neuron::setMotive()
{
   MOTIVE m;

   m = motiveWork.getValue();
   if (!motiveValid || (motive < m))
   {
      motiveValid = true;
      motive      = m;
   }

#ifdef MONA_TRACKING
   // Accumulate motive tracking.
   accumMotiveTracking();
#endif
}


#ifdef MONA_TRACKING
// Accumulate motive tracking.
void
Mona::Neuron::accumMotiveTracking()
{
   int i, j, k;

   if (type != MOTOR)
   {
      return;
   }
   for (i = 0; i < (int)tracker.motiveWorkPaths.size(); i++)
   {
      if (tracker.motivePaths.size() < Mona::MAX_DRIVER_TRACKS)
      {
         tracker.motivePaths.push_back(tracker.motiveWorkPaths[i]);
      }
      else
      {
         for (j = 0, k = -1; j < (int)tracker.motiveWorkPaths.size(); j++)
         {
            if (tracker.motiveWorkPaths[i].motive > tracker.motivePaths[j].motive)
            {
               if ((k == -1) || (tracker.motivePaths[j].motive < tracker.motivePaths[k].motive))
               {
                  k = j;
               }
            }
         }
         if (k != -1)
         {
            tracker.motivePaths[k] = tracker.motiveWorkPaths[i];
         }
      }
   }
}


#endif

// Finalize motives.
void
Mona::finalizeMotives()
{
   int    i;
   Neuron *neuron;

   list<Mediator *>::iterator mediatorItr;

   for (i = 0; i < (int)receptors.size(); i++)
   {
      neuron = (Neuron *)receptors[i];
      neuron->finalizeMotive();
   }
   for (i = 0; i < (int)motors.size(); i++)
   {
      neuron = (Neuron *)motors[i];
      neuron->finalizeMotive();
   }
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      neuron = (Neuron *)(*mediatorItr);
      neuron->finalizeMotive();
   }
}


// Finalize neuron motive.
void
Mona::Neuron::finalizeMotive()
{
   motive = motive / mona->maxMotive;
   if (motive > 1.0)
   {
      motive = 1.0;
   }
   if (motive < -1.0)
   {
      motive = -1.0;
   }
}


// Neuron drive.
void
Mona::Neuron::drive(MotiveAccum motiveAccum)
{
   int         i;
   Mediator    *mediator;
   Receptor    *receptor;
   MOTIVE      m;
   WEIGHT      w;
   MotiveAccum accumWork;

   // Prevent looping.
   if (!motiveAccum.addPath(this))
   {
      return;
   }

   // Accumulate need change due to goal value.
   if ((type != MEDIATOR) || !((Mediator *)this)->goalValueSubsumed())
   {
      motiveAccum.accumGoals(goals);
   }

   // Accumulate motive.
   // Store greater motive except for attenuated "pain".
   m = motiveAccum.getValue();
   if (!motiveWorkValid ||
       ((m >= NEARLY_ZERO) && ((m - motiveWork.getValue()) > NEARLY_ZERO)))
   {
      motiveWorkValid = true;
      motiveWork.loadNeeds(motiveAccum);
   }
#ifndef MONA_TRACKING
   else
   {
      return;
   }
#else
   // Track motive.
   if (!trackMotive(motiveAccum, accumWork))
   {
      return;
   }
#endif

#ifdef MONA_TRACE
   if (mona->traceDrive)
   {
      printf("Drive %llu, motive=%f\n", id, m);
   }
#endif

   switch (type)
   {
   // Distribute motive to component events.
   case MEDIATOR:
      mediator = (Mediator *)this;

      // Drive motive to cause event.
      if ((w = mediator->driveWeights[mediator->cause]) > NEARLY_ZERO)
      {
         accumWork.config(motiveAccum, w * (1.0 - mona->DRIVE_ATTENUATION));
         mediator->cause->drive(accumWork);
      }

      // Drive motive to response event.
      if (mediator->response != NULL)
      {
         if ((w = mediator->driveWeights[mediator->response]) > NEARLY_ZERO)
         {
            accumWork.config(motiveAccum, w * (1.0 - mona->DRIVE_ATTENUATION));
            mediator->response->drive(accumWork);
         }
      }

      // Drive motive to effect event.
      if ((w = mediator->driveWeights[mediator->effect]) > NEARLY_ZERO)
      {
         accumWork.config(motiveAccum, w * (1.0 - mona->DRIVE_ATTENUATION));
         mediator->effect->drive(accumWork);
      }
      break;

   // Receptor drives motive to subset receptors.
   case RECEPTOR:
      receptor = (Receptor *)this;
      for (i = 0; i < (int)receptor->subSensorModes.size(); i++)
      {
         accumWork.config(motiveAccum, 1.0);
         receptor->subSensorModes[i]->drive(accumWork);
      }
      break;

   // Drive terminates on motor neurons.
   case MOTOR:
      return;
   }

   // Drive motive to parent mediators.
   for (i = 0; i < (int)notifyList.size(); i++)
   {
      mediator = notifyList[i]->mediator;
      if ((w = driveWeights[mediator]) > NEARLY_ZERO)
      {
         w *= (1.0 - mona->DRIVE_ATTENUATION);
         accumWork.config(motiveAccum, w);
         mediator->driveCause(accumWork);
      }
   }
}


// Drive mediator cause.
void
Mona::Mediator::driveCause(MotiveAccum motiveAccum)
{
   int         i;
   Mediator    *mediator;
   MOTIVE      m;
   WEIGHT      w;
   MotiveAccum accumWork;

   // Accumulate motive.
   // Store greater motive except for attenuated "pain".
   m = motiveAccum.getValue();
   if (!motiveWorkValid ||
       ((m >= NEARLY_ZERO) && ((m - motiveWork.getValue()) > NEARLY_ZERO)))
   {
      motiveWorkValid = true;
      motiveWork.loadNeeds(motiveAccum);
   }
#ifndef MONA_TRACKING
   else
   {
      return;
   }
#else
   // Track motive.
   if (!trackMotive(motiveAccum, accumWork))
   {
      return;
   }
#endif

#ifdef MONA_TRACE
   if (mona->traceDrive)
   {
      printf("Drive cause %llu, motive=%f\n", id, m);
   }
#endif

   // Drive motive to cause event.
   if ((w = driveWeights[cause]) > NEARLY_ZERO)
   {
      accumWork.config(motiveAccum, w * (1.0 - mona->DRIVE_ATTENUATION));
      cause->drive(accumWork);
   }

   // Drive motive to response event.
   if (response != NULL)
   {
      if ((w = driveWeights[response]) > NEARLY_ZERO)
      {
         accumWork.config(motiveAccum, w * (1.0 - mona->DRIVE_ATTENUATION));
         response->drive(accumWork);
      }
   }

   // Drive motive to parent mediators.
   for (i = 0; i < (int)notifyList.size(); i++)
   {
      mediator = notifyList[i]->mediator;
      if ((w = driveWeights[mediator]) > NEARLY_ZERO)
      {
         accumWork.config(motiveAccum, w * (1.0 - mona->DRIVE_ATTENUATION));
         mediator->driveCause(accumWork);
      }
   }
}


#ifdef MONA_TRACKING
// Track motive.
// Returns true if tracking continues.
bool
Mona::Neuron::trackMotive(MotiveAccum& in, MotiveAccum& out)
{
   int i, j;
   struct MotiveAccum::DriveElem e;
   struct Activation::DrivePath  d;
   VALUE_SET needs;

   for (i = 0; i < (int)in.drivers.size(); i++)
   {
      if (in.drivers[i].neuron == this)
      {
         return(false);
      }
      out.drivers.push_back(in.drivers[i]);
      d.drivers.push_back(in.drivers[i]);
   }
   needs.alloc(mona->numNeeds);
   for (i = 0; i < mona->numNeeds; i++)
   {
      needs.set(i, mona->homeostats[i]->getNeed());
   }
   d.motiveWork.init(needs);
   needs.clear();
   d.motiveWork.loadNeeds(in);
   e.neuron  = this;
   e.motive  = d.motiveWork.getValue();
   e.motive /= mona->maxMotive;
   if (e.motive > 1.0)
   {
      e.motive = 1.0;
   }
   if (e.motive < -1.0)
   {
      e.motive = -1.0;
   }
   d.motive = e.motive;
   out.drivers.push_back(e);
   d.drivers.push_back(e);
   if (tracker.motiveWorkPaths.size() < Mona::MAX_DRIVER_TRACKS)
   {
      tracker.motiveWorkPaths.push_back(d);
   }
   else
   {
      for (i = 0, j = -1; i < (int)tracker.motiveWorkPaths.size(); i++)
      {
         if (d.motiveWork.getValue() > tracker.motiveWorkPaths[i].motiveWork.getValue())
         {
            if ((j == -1) ||
                (tracker.motiveWorkPaths[i].motiveWork.getValue() <
                 tracker.motiveWorkPaths[j].motiveWork.getValue()))
            {
               j = i;
            }
         }
      }
      if (j != -1)
      {
         tracker.motiveWorkPaths[j] = d;
      }
      else
      {
         d.motiveWork.clear();
         d.drivers.clear();
         return(false);
      }
   }
   return(true);
}


#endif
