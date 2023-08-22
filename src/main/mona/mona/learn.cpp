// For conditions of distribution and use, see copyright notice in mona.hpp

/*
 * Learn environment by creating mediator neurons that mediate
 * cause-effect sequences of neuron firing events.
 */

#include "mona.hpp"

void
Mona::learn()
{
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
   int n = MAX_RESPONSE_EQUIPPED_MEDIATOR_LEVEL;
   if (n > MAX_MEDIATOR_LEVEL)
   {
      n = MAX_MEDIATOR_LEVEL;
   }
   for (int i = 0, j = (int)learningEvents.size(), k = 0; i < j; i++)
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
               // Place motor?
               Motor *motor = (Motor *)(learningEvent->neuron);
               if (motor->isPlaceMotor())
               {
                  k = -1;
               }
               else
               {
                  k = n;
               }
            }
            else
            {
               k = i;
            }
            if (k == -1)
            {
               if ((int)(eventClock - learningEvent->end) <= MAX_MOVEMENT_RESPONSE_PATH_LENGTH)
               {
                  learningEventItr++;
               }
            }
            else if ((int)(eventClock - learningEvent->end) <= maxLearningEffectEventIntervals[k])
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
   movementLearningEffects.resize(sensorModes.size());
   for (int i = 0, j = (int)receptors.size(); i < j; i++)
   {
      receptor = receptors[i];
      if (receptor->firingStrength > NEARLY_ZERO)
      {
         learningEvent = new LearningEvent(receptor);
         assert(learningEvent != NULL);
         learningEvents[0].push_back(learningEvent);
         movementLearningEffects[receptor->sensorMode] = new LearningEvent(receptor);
      }
   }
   Motor *movementMotor = NULL;
   for (int i = 0, j = (int)motors.size(); i < j; i++)
   {
      motor = motors[i];
      if (motor->firingStrength > NEARLY_ZERO)
      {
         movementMotor = motor;
         if ((motor->movementType != MOVEMENT_TYPE::BEGIN) &&
             (motor->movementType != MOVEMENT_TYPE::END) &&
             !movementLearningPathActive)
         {
            learningEvent = new LearningEvent(motor);
            assert(learningEvent != NULL);
            learningEvents[0].push_back(learningEvent);
         }
      }
   }

   // Create movement mediators that use place motors.
   if (movementLearningCauses.size() == 0)
   {
      resetMovementLearningPath();
   }
   else
   {
      switch (movementMotor->movementType)
      {
      case MOVEMENT_TYPE::BEGIN:
         if (movementLearningPathActive)
         {
            resetMovementLearningPath();
         }
         else
         {
            movementLearningPathActive = true;
            for (int i = 0, j = movementLearningEffects.size(); i < j; i++)
            {
               delete movementLearningEffects[i];
            }
            movementLearningEffects.clear();
         }
         break;

      case MOVEMENT_TYPE::END:
         if (movementLearningPathActive)
         {
            if (movementLearningPathLength >= MIN_MOVEMENT_RESPONSE_PATH_LENGTH)
            {
               createPlaceMediators();
            }
         }
         resetMovementLearningPath();
         break;

      case MOVEMENT_TYPE::NONE:
         resetMovementLearningPath();
         break;

      default:
         if (movementLearningPathActive)
         {
            if (movementLearningPathLength < MAX_MOVEMENT_RESPONSE_PATH_LENGTH)
            {
               for (int i = 0, j = movementLearningEffects.size(); i < j; i++)
               {
                  delete movementLearningEffects[i];
               }
               movementLearningEffects.clear();
               movementLearningPathLength++;
            }
            else
            {
               resetMovementLearningPath();
            }
         }
         else
         {
            resetMovementLearningPath();
         }
         break;
      }
   }

   // Create new mediators based on potential effect events.
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
   for (int i = 0, j = (int)learningEvents.size(); i < j; i++)
   {
      for (learningEventItr = learningEvents[i].begin();
           learningEventItr != learningEvents[i].end(); learningEventItr++)
      {
         learningEvent = *learningEventItr;
         if ((learningEvent->end == eventClock) &&
             ((learningEvent->neuron->type == MEDIATOR) ||
              ((learningEvent->neuron->type == RECEPTOR))))
         {
            createMediator(learningEvent);
         }
      }
   }

   // Create mediators with generalized receptor effects.
   for (int i = 0, j = (int)generalizationEvents.size(); i < j; i++)
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


// Create place mediators.
void
Mona::createPlaceMediators()
{
   for (int i = 0, j = sensorModes.size(); i < j; i++)
   {
      Mediator *mediator = newMediator(INITIAL_ENABLEMENT);
      mediator->addEvent(CAUSE_EVENT, movementLearningCauses[i]->neuron);
      mediator->addEvent(RESPONSE_EVENT, newPlaceMotor(X, Y));
      mediator->addEvent(EFFECT_EVENT, movementLearningEffects[i]->neuron);
      mediator->updateGoalValue(movementLearningCauses[i]->needs);

      // Duplicate?
      if (isDuplicateMediator(mediator))
      {
         deleteNeuron(mediator);
         continue;
      }

      // Make new mediator available for learning.
      if ((mediator->level + 1) < (int)learningEvents.size())
      {
         mediator->causeBegin     = movementLearningCauses[i]->begin;
         mediator->firingStrength = movementLearningCauses[i]->firingStrength *
                                    movementLearningEffects[i]->firingStrength;
         LearningEvent *learningEvent = new LearningEvent(mediator);
         assert(learningEvent != NULL);
         learningEvents[mediator->level + 1].push_back(learningEvent);
      }

#ifdef MONA_TRACE
      if (traceLearn)
      {
         printf("Create place mediator:\n");
         mediator->print();
      }
#endif
   }
}


// Create new mediators for given effect.
void
Mona::createMediator(LearningEvent *effectEvent)
{
   int           level;
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
      if ((level == 0) && (((Receptor *)causeEvent->neuron)->sensorMode != ((Receptor *)effectEvent->neuron)->sensorMode))
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
      PROBABILITY p = 0.0;
      int         i = 0;
      for (int j = (int)candidates.size(); i < j; i++)
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
      for (int i = 0, j = (int)effectReceptor->superSensorModes.size(); i < j; i++)
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
      p          = 0.0;
      int i = 0;
      for (int j = (int)candidates.size(); i < j; i++)
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
   struct Notify *notify;

   if (mediator->cause->notifyList.size() < mediator->effect->notifyList.size())
   {
      if ((mediator->response == NULL) ||
          (mediator->cause->notifyList.size() < mediator->response->notifyList.size()))
      {
         for (int i = 0, j = (int)mediator->cause->notifyList.size(); i < j; i++)
         {
            notify = mediator->cause->notifyList[i];
            if (notify->mediator != mediator)
            {
               if (notify->mediator->effect == mediator->effect)
               {
                  if (notify->mediator->response == mediator->response)
                  {
                     return(true);
                  }
                  else
                  {
                     if ((notify->mediator->response != NULL) && (mediator->response != NULL))
                     {
                        Motor *notifyMotor = (Motor *)notify->mediator->response;
                        Motor *motor       = (Motor *)mediator->response;
                        if (motor->isDuplicate(notifyMotor))
                        {
                           return(true);
                        }
                     }
                  }
               }
            }
         }
         return(false);
      }
      else
      {
         for (int i = 0, j = (int)mediator->response->notifyList.size(); i < j; i++)
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
         for (int i = 0, j = (int)mediator->effect->notifyList.size(); i < j; i++)
         {
            notify = mediator->effect->notifyList[i];
            if (notify->mediator != mediator)
            {
               if (notify->mediator->cause == mediator->cause)
               {
                  if (notify->mediator->response == mediator->response)
                  {
                     return(true);
                  }
                  else
                  {
                     if ((notify->mediator->response != NULL) && (mediator->response != NULL))
                     {
                        Motor *notifyMotor = (Motor *)notify->mediator->response;
                        Motor *motor       = (Motor *)mediator->response;
                        if (motor->isDuplicate(notifyMotor))
                        {
                           return(true);
                        }
                     }
                  }
               }
            }
         }
         return(false);
      }
      else
      {
         for (int i = 0, j = (int)mediator->response->notifyList.size(); i < j; i++)
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
   Receptor *receptor;
   Motor    *motor;
   Mediator *mediator;

   list<Mediator *>::iterator      mediatorItr;
   LearningEvent                   *learningEvent;
   list<LearningEvent *>::iterator learningEventItr;

   for (int i = 0, j = (int)receptors.size(); i < j; i++)
   {
      receptor = receptors[i];
      receptor->firingStrength = 0.0;
      receptor->motive         = 0.0;
#ifdef MONA_TRACKING
      receptor->tracker.clear();
#endif
   }
   for (int i = 0, j = (int)motors.size(); i < j; i++)
   {
      motor = motors[i];
      motor->firingStrength = 0.0;
      motor->motive         = 0.0;
#ifdef MONA_TRACKING
      motor->tracker.clear();
#endif
   }
   for (int i = 0, j = (int)placeMotors.size(); i < j; i++)
   {
      motor = placeMotors[i];
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
   movementLearningPathLength = 0;
   for (int i = 0, j = movementLearningCauses.size(); i < j; i++)
   {
      delete movementLearningCauses[i];
   }
   movementLearningCauses.clear();
   for (int i = 0, j = movementLearningEffects.size(); i < j; i++)
   {
      delete movementLearningEffects[i];
   }
   movementLearningEffects.clear();
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


// Reset movement response path.
void Mona::resetMovementLearningPath()
{
   movementLearningPathLength = 0;
   for (int i = 0, j = movementLearningCauses.size(); i < j; i++)
   {
      delete movementLearningCauses[i];
   }
   movementLearningCauses.clear();
   for (int i = 0, j = sensorModes.size(); i < j; i++)
   {
      movementLearningCauses.push_back(movementLearningEffects[i]);
   }
   movementLearningEffects.clear();
   movementLearningPathActive = false;
}
