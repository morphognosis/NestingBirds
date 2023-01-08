// For conditions of distribution and use, see copyright notice in mona.hpp

/*
 * Learn environment by creating mediator neurons that mediate
 * cause-effect sequences of neuron firing events.
 */

#include "mona.hpp"

void
Mona::learn()
{
   int           i, j, k;
   LearningEvent *learningEvent;

   list<LearningEvent *>::iterator learningEventItr;
   Motor    *motor;
   Receptor *receptor;
   Mediator *mediator;
   list<Mediator *>::iterator mediatorItr;

#ifdef MONA_TRACE
   if (traceLearn)
   {
      printf("***Learn phase***\n");
   }
#endif

   // Purge obsolete events.
   j = MAX_RESPONSE_EQUIPPED_MEDIATOR_LEVEL;
   if (j > MAX_MEDIATOR_LEVEL)
   {
      j = MAX_MEDIATOR_LEVEL;
   }
   for (i = 0; i < (int)learningEvents.size(); i++)
   {
      for (learningEventItr = learningEvents[i].begin();
           learningEventItr != learningEvents[i].end(); )
      {
         learningEvent = *learningEventItr;
         if (learningEvent->firingStrength > NEARLY_ZERO)
         {
            // Is event still recent enough to form a new mediator?
            if (learningEvent->neuron->type == MOTOR)
            {
               k = j;
            }
            else
            {
               k = i;
            }
            if ((int)(eventClock - learningEvent->end) <= maxLearningEffectEventIntervals[k])
            {
               learningEventItr++;
            }
            else
            {
               learningEventItr = learningEvents[i].erase(learningEventItr);
               delete learningEvent;
            }
         }
         else
         {
            learningEventItr = learningEvents[i].erase(learningEventItr);
            delete learningEvent;
         }
      }
   }

   // Time-stamp and store significant events.
   for (i = 0; i < (int)receptors.size(); i++)
   {
      receptor = receptors[i];
      if (receptor->firingStrength > NEARLY_ZERO)
      {
         learningEvent = new LearningEvent(receptor);
         assert(learningEvent != NULL);
         learningEvents[0].push_back(learningEvent);
      }
   }
   for (i = 0; i < (int)motors.size(); i++)
   {
      motor = motors[i];
      if (motor->firingStrength > NEARLY_ZERO)
      {
         learningEvent = new LearningEvent(motor);
         assert(learningEvent != NULL);
         learningEvents[0].push_back(learningEvent);
      }
   }
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      if (!mediator->instinct && (mediator->firingStrength > NEARLY_ZERO))
      {
         if ((mediator->level + 1) < (int)learningEvents.size())
         {
            learningEvent = new LearningEvent(mediator);
            assert(learningEvent != NULL);
            learningEvents[mediator->level + 1].push_back(learningEvent);
         }
      }
   }

   // Create new mediators based on potential effect events.
   for (i = 0; i < (int)learningEvents.size(); i++)
   {
      for (learningEventItr = learningEvents[i].begin();
           learningEventItr != learningEvents[i].end(); learningEventItr++)
      {
         learningEvent = *learningEventItr;
         if ((learningEvent->end == eventClock) &&
             ((learningEvent->neuron->type == MEDIATOR) ||
              ((learningEvent->neuron->type == RECEPTOR) &&
               (((Receptor *)learningEvent->neuron)->sensorMode == 0))))
         {
            createMediator(learningEvent);
         }
      }
   }

   // Create mediators with generalized receptor effects.
   for (i = 0; i < (int)generalizationEvents.size(); i++)
   {
      generalizeMediator(generalizationEvents[i]);
      delete generalizationEvents[i];
   }
   generalizationEvents.clear();

   // Delete excess mediators.
   while ((int)mediators.size() > MAX_MEDIATORS)
   {
      if ((mediator = getWorstMediator()) == NULL)
      {
         break;
      }
      deleteNeuron(mediator);
   }

   // Increment event clock.
   eventClock++;
}


// Create new mediators for given effect.
void
Mona::createMediator(LearningEvent *effectEvent)
{
   int           i, level;
   bool          effectRespEq, causeRespEq;
   LearningEvent *causeEvent, *responseEvent, *learningEvent;

   list<LearningEvent *>::iterator causeEventItr,
                                   responseEventItr;
   vector<LearningEvent *>    tmpVector;
   Mediator                   *mediator;
   list<Mediator *>::iterator mediatorItr;
   vector<LearningEvent *>    candidates;
   PROBABILITY                accumProb, chooseProb, p;

   // Check event firing strength.
   if (effectEvent->firingStrength <= NEARLY_ZERO)
   {
      return;
   }

   // Find cause event candidates.
   accumProb = 0.0;
   if (effectEvent->neuron->type != MEDIATOR)
   {
      level        = 0;
      effectRespEq = true;
   }
   else
   {
      mediator = (Mediator *)effectEvent->neuron;
      level    = mediator->level + 1;
      if (level > MAX_MEDIATOR_LEVEL)
      {
         return;
      }
      if (mediator->response != NULL)
      {
         effectRespEq = true;
      }
      else
      {
         effectRespEq = false;
      }
   }
   for (causeEventItr = learningEvents[level].begin();
        causeEventItr != learningEvents[level].end(); causeEventItr++)
   {
      causeEvent = *causeEventItr;
      if (causeEvent->firingStrength <= NEARLY_ZERO)
      {
         continue;
      }
      if (causeEvent->neuron->type == MOTOR)
      {
         continue;
      }
      if (causeEvent->end >= effectEvent->begin)
      {
         continue;
      }

      // The cause and effect must have equal response-equippage status.
      if ((level == 0) || (((Mediator *)causeEvent->neuron)->response != NULL))
      {
         causeRespEq = true;
      }
      else
      {
         causeRespEq = false;
      }
      if (causeRespEq != effectRespEq)
      {
         continue;
      }

      // Save candidate.
      candidates.push_back(causeEvent);
      accumProb += causeEvent->probability;
   }

   // Choose causes and create mediators.
   while (true)
   {
      // Make a weighted probabilistic pick of a candidate.
      chooseProb = random.RAND_INTERVAL(0.0, accumProb);
      for (i = 0, p = 0.0; i < (int)candidates.size(); i++)
      {
         if (candidates[i] == NULL)
         {
            continue;
         }
         p += candidates[i]->probability;
         if (chooseProb <= p)
         {
            break;
         }
      }
      if (i == (int)candidates.size())
      {
         break;
      }
      causeEvent    = candidates[i];
      accumProb    -= causeEvent->probability;
      candidates[i] = NULL;

      // Make a probabilistic decision to create mediator.
      if (!random.RAND_CHANCE(effectEvent->probability *
                              causeEvent->probability))
      {
         continue;
      }

      // Add a response?
      // The components must also be response-equipped.
      responseEvent = NULL;
      if ((level < MIN_RESPONSE_UNEQUIPPED_MEDIATOR_LEVEL) ||
          (effectRespEq &&
           (level <= MAX_RESPONSE_EQUIPPED_MEDIATOR_LEVEL) &&
           random.RAND_BOOL()))
      {
         tmpVector.clear();
         for (responseEventItr = learningEvents[0].begin();
              responseEventItr != learningEvents[0].end(); responseEventItr++)
         {
            responseEvent = *responseEventItr;
            if ((responseEvent->neuron->type == MOTOR) &&
                (responseEvent->firingStrength > NEARLY_ZERO) &&
                (responseEvent->end == causeEvent->end + 1))
            {
               tmpVector.push_back(responseEvent);
            }
         }
         if (tmpVector.size() == 0)
         {
            continue;
         }
         responseEvent = tmpVector[random.RAND_CHOICE((int)tmpVector.size())];
      }

      // Create the mediator.
      mediator = newMediator(INITIAL_ENABLEMENT);
      mediator->addEvent(CAUSE_EVENT, causeEvent->neuron);
      if (responseEvent != NULL)
      {
         mediator->addEvent(RESPONSE_EVENT, responseEvent->neuron);
      }
      mediator->addEvent(EFFECT_EVENT, effectEvent->neuron);
      mediator->updateGoalValue(causeEvent->needs);

      // Duplicate?
      if (isDuplicateMediator(mediator))
      {
         deleteNeuron(mediator);
         continue;
      }

      // Make new mediator available for learning.
      if ((mediator->level + 1) < (int)learningEvents.size())
      {
         mediator->causeBegin     = causeEvent->begin;
         mediator->firingStrength = causeEvent->firingStrength *
                                    effectEvent->firingStrength;
         learningEvent = new LearningEvent(mediator);
         assert(learningEvent != NULL);
         learningEvents[mediator->level + 1].push_back(learningEvent);
      }

#ifdef MONA_TRACE
      if (traceLearn)
      {
         printf("Create mediator:\n");
         mediator->print();
      }
#endif
   }
}


// Create generalized mediators.
void
Mona::generalizeMediator(GeneralizationEvent *generalizationEvent)
{
   LearningEvent *candidateEvent, *learningEvent;

   list<LearningEvent *>::iterator candidateEventItr;
   Receptor                *effectReceptor, *candidateReceptor;
   Mediator                *mediator;
   vector<LearningEvent *> candidates;
   PROBABILITY             accumProb, chooseProb, p;
   int i;

   // Find effect event candidates.
   accumProb = 0.0;
   for (candidateEventItr = learningEvents[0].begin();
        candidateEventItr != learningEvents[0].end(); candidateEventItr++)
   {
      candidateEvent = *candidateEventItr;
      if (candidateEvent->firingStrength <= NEARLY_ZERO)
      {
         continue;
      }
      if (candidateEvent->neuron->type != RECEPTOR)
      {
         continue;
      }
      if (candidateEvent->end != eventClock)
      {
         continue;
      }

      // Is this a direct superset of the mediator's effect?
      candidateReceptor = (Receptor *)candidateEvent->neuron;
      effectReceptor    = (Receptor *)generalizationEvent->mediator->effect;
      for (i = 0; i < (int)effectReceptor->superSensorModes.size(); i++)
      {
         if (effectReceptor->superSensorModes[i] == candidateReceptor)
         {
            // Save candidate.
            candidates.push_back(candidateEvent);
            accumProb += candidateEvent->probability;
            break;
         }
      }
   }

   // Choose effects and create mediators.
   while (true)
   {
      // Make a weighted probabilistic pick of a candidate.
      chooseProb = random.RAND_INTERVAL(0.0, accumProb);
      for (i = 0, p = 0.0; i < (int)candidates.size(); i++)
      {
         if (candidates[i] == NULL)
         {
            continue;
         }
         p += candidates[i]->probability;
         if (chooseProb <= p)
         {
            break;
         }
      }
      if (i == (int)candidates.size())
      {
         break;
      }
      candidateEvent = candidates[i];
      accumProb     -= candidateEvent->probability;
      candidates[i]  = NULL;

      // Make a probabilistic decision to create mediator.
      if (!random.RAND_CHANCE(generalizationEvent->enabling))
      {
         continue;
      }

      // Create the mediator.
      mediator = newMediator(INITIAL_ENABLEMENT);
      mediator->addEvent(CAUSE_EVENT, generalizationEvent->mediator->cause);
      if (generalizationEvent->mediator->response != NULL)
      {
         mediator->addEvent(RESPONSE_EVENT, generalizationEvent->mediator->response);
      }
      mediator->addEvent(EFFECT_EVENT, candidateEvent->neuron);
      mediator->updateGoalValue(generalizationEvent->needs);

      // Duplicate?
      if (isDuplicateMediator(mediator))
      {
         deleteNeuron(mediator);
         continue;
      }

      // Make new mediator available for learning.
      if ((mediator->level + 1) < (int)learningEvents.size())
      {
         mediator->causeBegin     = generalizationEvent->begin;
         mediator->firingStrength = generalizationEvent->enabling *
                                    candidateEvent->firingStrength;
         learningEvent = new LearningEvent(mediator);
         assert(learningEvent != NULL);
         learningEvents[mediator->level + 1].push_back(learningEvent);
      }

#ifdef MONA_TRACE
      if (traceLearn)
      {
         printf("Create generalized mediator:\n");
         mediator->print();
      }
#endif
   }
}


// Is mediator a duplicate?
bool Mona::isDuplicateMediator(Mediator *mediator)
{
   int           i, j;
   struct Notify *notify;

   if (mediator->cause->notifyList.size() < mediator->effect->notifyList.size())
   {
      if ((mediator->response == NULL) ||
          (mediator->cause->notifyList.size() < mediator->response->notifyList.size()))
      {
         for (i = 0, j = (int)mediator->cause->notifyList.size(); i < j; i++)
         {
            notify = mediator->cause->notifyList[i];
            if (notify->mediator != mediator)
            {
               if ((notify->mediator->response == mediator->response) &&
                   (notify->mediator->effect == mediator->effect))
               {
                  return(true);
               }
            }
         }
         return(false);
      }
      else
      {
         for (i = 0, j = (int)mediator->response->notifyList.size(); i < j; i++)
         {
            notify = mediator->response->notifyList[i];
            if (notify->mediator != mediator)
            {
               if ((notify->mediator->cause == mediator->cause) &&
                   (notify->mediator->effect == mediator->effect))
               {
                  return(true);
               }
            }
         }
         return(false);
      }
   }
   else
   {
      if ((mediator->response == NULL) ||
          (mediator->effect->notifyList.size() < mediator->response->notifyList.size()))
      {
         for (i = 0, j = (int)mediator->effect->notifyList.size(); i < j; i++)
         {
            notify = mediator->effect->notifyList[i];
            if (notify->mediator != mediator)
            {
               if ((notify->mediator->cause == mediator->cause) &&
                   (notify->mediator->response == mediator->response))
               {
                  return(true);
               }
            }
         }
         return(false);
      }
      else
      {
         for (i = 0, j = (int)mediator->response->notifyList.size(); i < j; i++)
         {
            notify = mediator->response->notifyList[i];
            if (notify->mediator != mediator)
            {
               if ((notify->mediator->cause == mediator->cause) &&
                   (notify->mediator->effect == mediator->effect))
               {
                  return(true);
               }
            }
         }
         return(false);
      }
   }
}


// Clear working memory.
void Mona::clearWorkingMemory()
{
   int      i;
   Receptor *receptor;
   Motor    *motor;
   Mediator *mediator;

   list<Mediator *>::iterator      mediatorItr;
   LearningEvent                   *learningEvent;
   list<LearningEvent *>::iterator learningEventItr;

   for (i = 0; i < (int)receptors.size(); i++)
   {
      receptor = receptors[i];
      receptor->firingStrength = 0.0;
      receptor->motive         = 0.0;
#ifdef MONA_TRACKING
      receptor->tracker.clear();
#endif
   }
   for (i = 0; i < (int)motors.size(); i++)
   {
      motor = motors[i];
      motor->firingStrength = 0.0;
      motor->motive         = 0.0;
#ifdef MONA_TRACKING
      motor->tracker.clear();
#endif
   }
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      mediator->firingStrength = 0.0;
      mediator->motive         = 0.0;
      mediator->retireEnablings(true);
#ifdef MONA_TRACKING
      mediator->tracker.clear();
#endif
   }
   for (i = 0; i < (int)learningEvents.size(); i++)
   {
      for (learningEventItr = learningEvents[i].begin();
           learningEventItr != learningEvents[i].end(); learningEventItr++)
      {
         learningEvent = *learningEventItr;
         delete learningEvent;
      }
      learningEvents[i].clear();
   }
}


// Clear long term memory.
void Mona::clearLongTermMemory()
{
   Mediator *mediator;

   list<Mediator *>           tmpMediators;
   list<Mediator *>::iterator mediatorItr;

   // Must also clear working memory.
   clearWorkingMemory();

   // Delete all non-instinct mediators.
   for (mediatorItr = mediators.begin();
        mediatorItr != mediators.end();
        mediatorItr = mediators.begin())
   {
      mediator = *mediatorItr;
      if (mediator->instinct || mediator->hasParentInstinct())
      {
         tmpMediators.push_back(mediator);
         mediators.erase(mediatorItr);
      }
      else
      {
         deleteNeuron((Mona::Neuron *)(*mediatorItr));
      }
   }
   mediators.clear();
   for (mediatorItr = tmpMediators.begin();
        mediatorItr != tmpMediators.end(); mediatorItr++)
   {
      mediator = *mediatorItr;
      mediators.push_back(mediator);
   }
}
