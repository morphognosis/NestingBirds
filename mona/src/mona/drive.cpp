// For conditions of distribution and use, see copyright notice in mona.hpp

#include "mona.hpp"
#include <math.h>

// Drive need changes caused by goal values through network.
void
Mona::drive()
{
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
   for (int i = 0; i < numNeeds; i++)
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
   for (int i = 0, j = (int)receptors.size(); i < j; i++)
   {
      neuron = (Neuron *)receptors[i];
      neuron->initDrive(needs);
   }
   for (int i = 0, j = (int)motors.size(); i < j; i++)
   {
      neuron = (Neuron *)motors[i];
      neuron->initDrive(needs);
   }
   for (int i = 0, j = (int)placeMotors.size(); i < j; i++)
   {
      neuron = (Neuron *)placeMotors[i];
      neuron->initDrive(needs);
   }
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      neuron = (Neuron *)(*mediatorItr);
      neuron->initDrive(needs);
   }

   // Drive.
   for (int i = 0, j = (int)receptors.size(); i < j; i++)
   {
      receptor = receptors[i];
      if (receptor->goals.getValue() != 0.0)
      {
         motiveAccum.init(needs);
         motiveAccum.accumGoals(receptor->goals);
         if (motiveAccum.getValue() >= MIN_DRIVE_MOTIVE)
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

   for (int i = 0, j = (int)motors.size(); i < j; i++)
   {
      motor = motors[i];
      if (motor->goals.getValue() != 0.0)
      {
         motiveAccum.init(needs);
         motiveAccum.accumGoals(motor->goals);
         if (motiveAccum.getValue() >= MIN_DRIVE_MOTIVE)
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

   for (int i = 0, j = (int)placeMotors.size(); i < j; i++)
   {
      motor = placeMotors[i];
      if (motor->goals.getValue() != 0.0)
      {
         motiveAccum.init(needs);
         motiveAccum.accumGoals(motor->goals);
         if (motiveAccum.getValue() >= MIN_DRIVE_MOTIVE)
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
            if (motiveAccum.getValue() >= MIN_DRIVE_MOTIVE)
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
   NEED   goalValue;
   Neuron *cause      = this->cause;
   Neuron *response   = this->response;
   Neuron *effect     = this->effect;
   bool   *goalBlocks = new bool[mona->numNeeds];

   assert(goalBlocks != NULL);

   for (int i = 0; i < mona->numNeeds; i++)
   {
      goalBlocks[i] = false;
   }
   while (effect != NULL)
   {
      for (int i = 0; i < mona->numNeeds; i++)
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
      int i;
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
   for (int i = 0, j = (int)notifyList.size(); i < j; i++)
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
   Neuron *neuron;

   list<Mediator *>::iterator mediatorItr;

   for (int i = 0, j = (int)receptors.size(); i < j; i++)
   {
      neuron = (Neuron *)receptors[i];
      neuron->clearMotiveWork();
   }
   for (int i = 0, j = (int)motors.size(); i < j; i++)
   {
      neuron = (Neuron *)motors[i];
      neuron->clearMotiveWork();
   }
   for (int i = 0, j = (int)placeMotors.size(); i < j; i++)
   {
      neuron = (Neuron *)placeMotors[i];
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
   Neuron *neuron;

   list<Mediator *>::iterator mediatorItr;

   for (int i = 0, j = (int)receptors.size(); i < j; i++)
   {
      neuron = (Neuron *)receptors[i];
      neuron->setMotive();
   }
   for (int i = 0, j = (int)motors.size(); i < j; i++)
   {
      neuron = (Neuron *)motors[i];
      neuron->setMotive();
   }
   for (int i = 0, j = (int)placeMotors.size(); i < j; i++)
   {
      neuron = (Neuron *)placeMotors[i];
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
   MOTIVE m = motiveWork.getValue();

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
   if (type != MOTOR)
   {
      return;
   }
   for (int i = 0, n = (int)tracker.motiveWorkPaths.size(); i < n; i++)
   {
      if (tracker.motivePaths.size() < Mona::MAX_DRIVER_TRACKS)
      {
         tracker.motivePaths.push_back(tracker.motiveWorkPaths[i]);
      }
      else
      {
         int k = -1;
         for (int j = 0, n2 = (int)tracker.motiveWorkPaths.size(); j < n2; j++)
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
   Neuron *neuron;

   list<Mediator *>::iterator mediatorItr;

   for (int i = 0, j = (int)receptors.size(); i < j; i++)
   {
      neuron = (Neuron *)receptors[i];
      neuron->finalizeMotive();
   }
   for (int i = 0, j = (int)motors.size(); i < j; i++)
   {
      neuron = (Neuron *)motors[i];
      neuron->finalizeMotive();
   }
   for (int i = 0, j = (int)placeMotors.size(); i < j; i++)
   {
      neuron = (Neuron *)placeMotors[i];
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
   if (m < mona->MIN_DRIVE_MOTIVE)
   {
      return;
   }
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
      switch (type)
      {
      case RECEPTOR:
         printf("Drive receptor id=%llu, motive=%f\n", id, m);
         break;

      case MOTOR:
         printf("Drive motor id=%llu, motive=%f\n", id, m);
         break;

      case MEDIATOR:
         printf("Drive mediator id=%llu, motive=%f\n", id, m);
         break;
      }
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
      for (int i = 0, j = (int)receptor->subSensorModes.size(); i < j; i++)
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
   for (int i = 0, j = (int)notifyList.size(); i < j; i++)
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
   Mediator    *mediator;
   MOTIVE      m;
   WEIGHT      w;
   MotiveAccum accumWork;

   // Accumulate motive.
   // Store greater motive except for attenuated "pain".
   m = motiveAccum.getValue();
   if (m < mona->MIN_DRIVE_MOTIVE)
   {
      return;
   }
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
   for (int i = 0, j = (int)notifyList.size(); i < j; i++)
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
   struct MotiveAccum::DriveElem e;
   struct Activation::DrivePath  d;
   VALUE_SET needs;

   for (int i = 0, j = (int)in.drivers.size(); i < j; i++)
   {
      if (in.drivers[i].neuron == this)
      {
         return(false);
      }
      out.drivers.push_back(in.drivers[i]);
      d.drivers.push_back(in.drivers[i]);
   }
   needs.alloc(mona->numNeeds);
   for (int i = 0; i < mona->numNeeds; i++)
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
      int k = -1;
      for (int i = 0, j = (int)tracker.motiveWorkPaths.size(); i < j; i++)
      {
         if (d.motiveWork.getValue() > tracker.motiveWorkPaths[i].motiveWork.getValue())
         {
            if ((k == -1) ||
                (tracker.motiveWorkPaths[i].motiveWork.getValue() <
                 tracker.motiveWorkPaths[k].motiveWork.getValue()))
            {
               k = i;
            }
         }
      }
      if (k != -1)
      {
         tracker.motiveWorkPaths[k] = d;
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
