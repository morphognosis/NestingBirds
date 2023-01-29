// For conditions of distribution and use, see copyright notice in mona.hpp

#include "mona.hpp"
#include <math.h>

// Enablement processing.
void
Mona::enable()
{
   Receptor *receptor;
   Mediator *mediator;

   list<Mediator *>::iterator          mediatorItr;
   struct Notify                       *notify;
   struct FiringNotify                 firingNotify;
   list<struct FiringNotify>::iterator firingNotifyItr;

#ifdef MONA_TRACE
   if (traceEnable)
   {
      printf("***Enable phase***\n");
   }
#endif

   // Clear mediators.
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      mediator->firingStrength = 0.0;
      mediator->responseEnablings.clearNewInSet();
      mediator->effectEnablings.clearNewInSet();
   }

   // Begin mediator event notifications.

   // Notify mediators of response firing events.
   // This will propagate enabling values to effect events.
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      if (mediator->response != NULL)
      {
         mediator->responseFiring(mediator->response->firingStrength);
      }
   }

   // Recursively notify mediators of effect firing events.
   // Effect events are notified regardless of firing magnitude, in order
   // to update enablement for both firing and expiration outcomes.
   // Cause firing events are recorded for later notification;
   // these are deferred to allow a cause to fire as soon as
   // its effect event fires or expires.
   causeFirings.clear();
   for (int i = 0, n = (int)receptors.size(); i < n; i++)
   {
      receptor = receptors[i];
      for (int j = 0, k = (int)receptor->notifyList.size(); j < k; j++)
      {
         notify   = receptor->notifyList[j];
         mediator = notify->mediator;
         if (notify->eventType == EFFECT_EVENT)
         {
            mediator->effectFiring(receptor->firingStrength);
         }
      }
   }

   // Notify mediators of cause receptor firing events.
   for (int i = 0, n = (int)receptors.size(); i < n; i++)
   {
      receptor = receptors[i];
      if (receptor->firingStrength > 0.0)
      {
         for (int j = 0, k = (int)receptor->notifyList.size(); j < k; j++)
         {
            notify   = receptor->notifyList[j];
            mediator = notify->mediator;
            if (notify->eventType == CAUSE_EVENT)
            {
               mediator->causeFiring(receptor->firingStrength, eventClock);
            }
         }
      }
   }

   // Notify mediators of remaining cause firing events.
   for (firingNotifyItr = causeFirings.begin();
        firingNotifyItr != causeFirings.end(); firingNotifyItr++)
   {
      firingNotify = *firingNotifyItr;
      mediator     = firingNotify.notify->mediator;
      mediator->causeFiring(firingNotify.notifyStrength,
                            firingNotify.causeBegin);
   }
   causeFirings.clear();

   // Retire timed-out enablings.
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      mediator->retireEnablings();
   }

   // Update effective enablements.
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      mediator->effectiveEnablementValid = false;
   }
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      mediator->updateEffectiveEnablement();
   }
}


// Firing of mediator cause event.
void
Mona::Mediator::causeFiring(WEIGHT notifyStrength, TIME causeBegin)
{
   Enabling   *enabling;
   ENABLEMENT delta, enablement2;

#ifdef MONA_TRACKING
   tracker.enable = true;
#endif

   // Determine notification and enabling strength.
   delta = notifyStrength * baseEnablement;
   if (delta <= 0.0)
   {
      return;
   }
   baseEnablement -= delta;

   // Distribute enablement to next neuron.
   for (int i = 0, j = (int)mona->effectEventIntervalWeights[level].size(); i < j; i++)
   {
      enablement2 = delta * mona->effectEventIntervalWeights[level][i];
      if (enablement2 > 0.0)
      {
         enabling = new Enabling(enablement2, motive, 0, i, causeBegin);
         assert(enabling != NULL);
         enabling->setNeeds(mona->homeostats);
         if (response != NULL)
         {
            responseEnablings.insert(enabling);
#ifdef MONA_TRACKING
            response->tracker.enable = true;
#endif
         }
         else
         {
            effectEnablings.insert(enabling);
#ifdef MONA_TRACKING
            effect->tracker.enable = true;
#endif
         }
      }
   }
}


// Firing of mediator response event.
void
Mona::Mediator::responseFiring(WEIGHT notifyStrength)
{
   Enabling *enabling, *enabling2;

   list<Enabling *>::iterator enablingItr;
   ENABLEMENT                 enablement;

   // Transfer enablings to effect event.
   for (enablingItr = responseEnablings.enablings.begin();
        enablingItr != responseEnablings.enablings.end(); enablingItr++)
   {
      enabling        = *enablingItr;
      enablement      = enabling->value * notifyStrength;
      baseEnablement += (enabling->value - enablement);
      enabling->value = 0.0;
      if (enablement > 0.0)
      {
         enabling2         = enabling->clone();
         enabling2->value  = enablement;
         enabling2->motive = motive;
         enabling2->age    = 1;
         effectEnablings.insert(enabling2);
#ifdef MONA_TRACKING
         effect->tracker.enable = true;
#endif
      }
   }
}


// Firing of mediator effect event.
void
Mona::Mediator::effectFiring(WEIGHT notifyStrength)
{
   Enabling *enabling;

   list<Enabling *>::iterator enablingItr;
   ENABLEMENT                 e, enablement;
   WEIGHT              strength;
   struct Notify       *notify;
   Mediator            *mediator;
   struct FiringNotify causeFiring;
   vector<WEIGHT>      fireWeights, expireWeights;
   bool                parentContext;

   // If parent enabling context active, then parent's
   // enablement will be updated instead of current mediator.
   parentContext = false;
   for (int i = 0, j = (int)notifyList.size(); i < j; i++)
   {
      notify   = notifyList[i];
      mediator = notify->mediator;
      if (notify->eventType == EFFECT_EVENT)
      {
         if (mediator->effectEnablings.getOldValue() > 0.0)
         {
            parentContext = true;
            break;
         }
      }
   }

   // Accumulate effect enablement.
   causeBegin     = INVALID_TIME;
   enablement     = getEnablement();
   firingStrength = 0.0;
   for (enablingItr = effectEnablings.enablings.begin();
        enablingItr != effectEnablings.enablings.end(); enablingItr++)
   {
      enabling = *enablingItr;
      if (enabling->value > 0.0)
      {
         if ((causeBegin == INVALID_TIME) || (enabling->causeBegin > causeBegin))
         {
            causeBegin = enabling->causeBegin;
         }
      }

      if ((e = notifyStrength * enabling->value) > 0.0)
      {
         // Accumulate firing strength.
         firingStrength += e;

         //  Restore base enablement.
         enabling->value -= e;
         baseEnablement  += e;

         // Save weight for enablement and utility updates.
         if (!parentContext)
         {
            fireWeights.push_back(e / enablement);
         }

         // Update goal value.
         updateGoalValue(enabling->needs);
      }
 
      // Handle expired enablement.
      if ((enabling->value > 0.0) &&
          (enabling->age >= mona->effectEventIntervals[level][enabling->timerIndex]))
      {
         if (!parentContext)
         {
            expireWeights.push_back(enabling->value / enablement);
         }

         // Failed mediator might be generalizable.
         if (!instinct && (effect->type == RECEPTOR))
         {
            GeneralizationEvent *event = new GeneralizationEvent(this, enabling->value);
            assert(event != NULL);
            mona->generalizationEvents.push_back(event);
         }

         // Restore enablement.
         baseEnablement += enabling->value;
         enabling->value = 0.0;
      }
   }

   // Update need based on firing strength.
   for (int i = 0; i < mona->numNeeds; i++)
   {
       mona->homeostats[i]->mediatorUpdate(this, firingStrength);
   }

   // Update enablement and utility for firing enablings.
   for (int i = 0, j = (int)fireWeights.size(); i < j; i++)
   {
      updateEnablement(FIRE, fireWeights[i]);
      updateUtility(fireWeights[i]);
   }

   // Update enablement for expired enablings.
   for (int i = 0, j = (int)expireWeights.size(); i < j; i++)
   {
      updateEnablement(EXPIRE, expireWeights[i]);
   }

#ifdef MONA_TRACE
   if ((firingStrength > 0.0) && mona->traceEnable)
   {
      printf("Mediator firing: %llu\n", id);
   }
#endif

#ifdef MONA_TRACKING
   if (firingStrength > 0.0)
   {
      tracker.fire = true;
   }
#endif

   // Notify parent mediators.
   strength = firingStrength / enablement;
   for (int i = 0, j = (int)notifyList.size(); i < j; i++)
   {
      notify   = notifyList[i];
      mediator = notify->mediator;
      if (notify->eventType == EFFECT_EVENT)
      {
         mediator->effectFiring(strength);
      }
      else if (strength > 0.0)
      {
         // Record cause notification.
         causeFiring.notify         = notify;
         causeFiring.notifyStrength = strength;
         causeFiring.causeBegin     = causeBegin;
         mona->causeFirings.push_back(causeFiring);
      }
   }
}


// Get enablement including portions in enablings.
Mona::ENABLEMENT
Mona::Mediator::getEnablement()
{
   return(baseEnablement +
          responseEnablings.getValue() +
          effectEnablings.getValue());
}


// Update enablement.
void
Mona::Mediator::updateEnablement(EVENT_OUTCOME outcome, WEIGHT updateWeight)
{
   ENABLEMENT e1, e2;
   double     r;
   Enabling   *enabling;

   list<Enabling *>::iterator enablingItr;

   // Instinct enablement cannot be updated.
   if (instinct)
   {
      return;
   }

   // Compute new enablement.
   e1 = e2 = getEnablement();
   if (outcome == FIRE)
   {
      if (updateWeight > 0.0)
      {
         e1 += (1.0 - e1) * updateWeight * mona->LEARNING_INCREASE_VELOCITY;
      }
      else
      {
         e1 += (e1 - mona->MIN_ENABLEMENT) * updateWeight * mona->LEARNING_DECREASE_VELOCITY;
      }
   }
   else                                           // EXPIRE
   {
      if (updateWeight > 0.0)
      {
         e1 += (mona->MIN_ENABLEMENT - e1) * updateWeight * mona->LEARNING_DECREASE_VELOCITY;
      }
   }
   if (e1 < mona->MIN_ENABLEMENT)
   {
      e1 = mona->MIN_ENABLEMENT;
   }
   if (e1 > 1.0)
   {
      e1 = 1.0;
   }

   // Get scaling ratio of new to old enablement.
   if (e2 <= NEARLY_ZERO)
   {
      baseEnablement = e1;
      r = 0.0;
   }
   else
   {
      r = e1 / e2;
      baseEnablement *= r;
   }

   // Scale enablings.
   for (enablingItr = responseEnablings.enablings.begin();
        enablingItr != responseEnablings.enablings.end(); enablingItr++)
   {
      enabling         = *enablingItr;
      enabling->value *= r;
   }
   for (enablingItr = effectEnablings.enablings.begin();
        enablingItr != effectEnablings.enablings.end(); enablingItr++)
   {
      enabling         = *enablingItr;
      enabling->value *= r;
   }
}


// Update mediator effective enablement.
// The effective enablement combines a mediator's enablement
// with the enablements of its overlying mediator hierarchy
// to determine its ability to predict its effect event.
void
Mona::Mediator::updateEffectiveEnablement()
{
   ENABLEMENT    e, et;
   struct Notify *notify;
   Mediator      *mediator;

   if (effectiveEnablementValid)
   {
      return;
   }
   effectiveEnablementValid = true;

   // Update parent effective enablements
   // and determine combined enabling effect.
   e = 1.0;
   for (int i = 0, j = (int)notifyList.size(); i < j; i++)
   {
      notify   = notifyList[i];
      mediator = notify->mediator;
      if (notify->eventType == EFFECT_EVENT)
      {
         mediator->updateEffectiveEnablement();
         e *= (1.0 - (mediator->effectiveEnablement *
                      mediator->effectiveEnablingWeight));
      }
   }

   // Effective enablement combines parent and current enablements.
   et = getEnablement();
   e *= (1.0 - et);
   e  = 1.0 - e;
   if (e < 0.0)
   {
      e = 0.0;
   }
   if (e > 1.0)
   {
      e = 1.0;
   }
   effectiveEnablement = e;

   // Determine effective enabling weight.
   effectiveEnablingWeight = effectEnablings.getValue() / et;
}


// Retire enablings.
void
Mona::Mediator::retireEnablings(bool force)
{
   Enabling *enabling;

   list<Enabling *>::iterator enablingItr;

   // Age and retire response enablings.
   for (enablingItr = responseEnablings.enablings.begin();
        enablingItr != responseEnablings.enablings.end(); )
   {
      enabling = *enablingItr;
      enabling->age++;
      if (force || (enabling->age > 1))
      {
         enablingItr     = responseEnablings.enablings.erase(enablingItr);
         baseEnablement += enabling->value;
         delete enabling;
      }
      else
      {
         enablingItr++;
      }
   }

   // Age and retire effect enablings.
   for (enablingItr = effectEnablings.enablings.begin();
        enablingItr != effectEnablings.enablings.end(); )
   {
      enabling = *enablingItr;
      enabling->age++;
      if (force ||
          (enabling->age > mona->effectEventIntervals[level][enabling->timerIndex]))
      {
         enablingItr     = effectEnablings.enablings.erase(enablingItr);
         baseEnablement += enabling->value;
         delete enabling;
      }
      else
      {
         enablingItr++;
      }
   }
}


// Update goal value.
void Mona::Mediator::updateGoalValue(VALUE_SET& needs)
{
   NEED      need;
   VALUE_SET needsBase, needDeltas;

   if (!mona->LEARN_MEDIATOR_GOAL_VALUE ||
       level < mona->LEARN_MEDIATOR_GOAL_VALUE_MIN_LEVEL)
   {
      return;
   }
   needsBase.alloc(mona->numNeeds);
   needDeltas.alloc(mona->numNeeds);
   for (int i = 0; i < mona->numNeeds; i++)
   {
      needsBase.set(i, mona->homeostats[i]->getNeed());
      need = needsBase.get(i) - needs.get(i);
      if (need > 1.0)
      {
         need = 1.0;
      }
      if (need < -1.0)
      {
         need = -1.0;
      }
      needDeltas.set(i, need);
   }
   goals.update(needsBase, needDeltas);
}


// Expire response enablings.
void
Mona::expireResponseEnablings(RESPONSE expiringResponse)
{
   Mediator      *mediator;
   Enabling      *enabling;
   ENABLEMENT    enablement;
   struct Notify *notify;

   vector<WEIGHT>             expireWeights;
   list<Mediator *>::iterator mediatorItr;
   list<Enabling *>::iterator enablingItr;

   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      if ((mediator->response != NULL) && (((Motor *)mediator->response)->response == expiringResponse))
      {
         enablement = mediator->getEnablement();

         expireWeights.clear();
         for (enablingItr = mediator->responseEnablings.enablings.begin();
              enablingItr != mediator->responseEnablings.enablings.end(); enablingItr++)
         {
            enabling = *enablingItr;
            if (enabling->value > 0.0)
            {
               expireWeights.push_back(enabling->value / enablement);
               mediator->baseEnablement += enabling->value;
               enabling->value           = 0.0;
            }
         }

         for (int i = 0, j = (int)expireWeights.size(); i < j; i++)
         {
            mediator->updateEnablement(EXPIRE, expireWeights[i]);
         }

         for (int i = 0, j = (int)mediator->notifyList.size(); i < j; i++)
         {
            notify = mediator->notifyList[i];
            if (notify->eventType == EFFECT_EVENT)
            {
               expireMediatorEnablings(notify->mediator);
            }
         }
      }
   }
}


// Expire mediator enablings.
void
Mona::expireMediatorEnablings(Mediator *mediator)
{
   Enabling   *enabling;
   ENABLEMENT enablement;

   vector<WEIGHT>             expireWeights;
   list<Enabling *>::iterator enablingItr;
   struct Notify              *notify;

   enablement = mediator->getEnablement();
   for (enablingItr = mediator->effectEnablings.enablings.begin();
        enablingItr != mediator->effectEnablings.enablings.end(); enablingItr++)
   {
      enabling = *enablingItr;
      if (enabling->value > 0.0)
      {
         expireWeights.push_back(enabling->value / enablement);
         mediator->baseEnablement += enabling->value;
         enabling->value           = 0.0;
      }
   }

   for (int i = 0, j = (int)expireWeights.size(); i < j; i++)
   {
      mediator->updateEnablement(EXPIRE, expireWeights[i]);
   }

   for (int i = 0, j = (int)mediator->notifyList.size(); i < j; i++)
   {
      notify = mediator->notifyList[i];
      if (notify->eventType == EFFECT_EVENT)
      {
         expireMediatorEnablings(notify->mediator);
      }
   }
}
