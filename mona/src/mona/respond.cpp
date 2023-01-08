// For conditions of distribution and use, see copyright notice in mona.hpp

#include "mona.hpp"

// Respond.
void
Mona::respond()
{
   int                i, j;
   Motor              *motor;
   RESPONSE_POTENTIAL max;

#ifdef MONA_TRACE
   if (traceRespond)
   {
      printf("***Respond phase***\n");
   }
#endif

   // Get response potentials from motor motives.
   for (i = 0; i < numResponses; i++)
   {
      responsePotentials[i] = 0.0;
   }
   for (i = 0; i < (int)motors.size(); i++)
   {
      motor = motors[i];
      responsePotentials[motor->response] += motor->motive;
   }

   // Incorporate minimal randomness.
   for (i = 0; i < numResponses; i++)
   {
      if (responsePotentials[i] <= NEARLY_ZERO)
      {
         responsePotentials[i] =
            (random.RAND_PROB() * RESPONSE_RANDOMNESS);
      }
   }

   // Make a random response selection?
   if (random.RAND_CHANCE(RESPONSE_RANDOMNESS))
   {
      if (numResponses > 0)
      {
         double *responseWeights = new double[numResponses];
         assert(responseWeights != NULL);
         for (i = 0; i < numResponses; i++)
         {
            responseWeights[i] = responsePotentials[i];
            if (i > 0)
            {
               responseWeights[i] += responseWeights[i - 1];
            }
         }
         int    j = numResponses - 1;
         double n = random.RAND_INTERVAL(0.0, responseWeights[j]);
         for (i = 0; i < j; i++)
         {
            if (n <= responseWeights[i])
            {
               break;
            }
         }
         response = i;
         delete [] responseWeights;
      }
      else
      {
         response = 0;
      }
   }
   else
   {
      // Select maximum response potential.
      bool first = true;
      j = random.RAND_CHOICE(numResponses);
      for (i = response = 0, max = 0.0; i < numResponses; i++)
      {
         if (first || (responsePotentials[j] > max))
         {
            first    = false;
            response = (RESPONSE)j;
            max      = responsePotentials[j];
         }
         j++;
         if (j == numResponses)
         {
            j = 0;
         }
      }
   }

   // Response overridden?
   if (responseOverride != NULL_RESPONSE)
   {
      // Conditional override?
      if (responseOverridePotential >= 0.0)
      {
         if (responsePotentials[(int)response] < responseOverridePotential)
         {
            response = responseOverride;
         }
      }
      else
      {
         response = responseOverride;
      }
      responseOverride          = NULL_RESPONSE;
      responseOverridePotential = -1.0;
   }

   // Fire responding motor.
   for (i = 0; i < (int)motors.size(); i++)
   {
      motor = motors[i];
      if (motor->response == response)
      {
         motor->firingStrength = 1.0;

#ifdef MONA_TRACE
         if (traceRespond)
         {
            printf("Motor firing: %llu\n", motor->id);
         }
#endif
#ifdef MONA_TRACKING
         motor->tracker.fire = true;
#endif
      }
      else
      {
         motor->firingStrength = 0.0;
      }
   }

#ifdef MONA_TRACE
   if (traceRespond)
   {
      printf("Response = %d\n", response);
   }
#endif

   // Update need based on response.
   for (i = 0; i < numNeeds; i++)
   {
      homeostats[i]->responseUpdate();
   }
}


// Add a response.
Mona::RESPONSE Mona::addResponse()
{
   int i;

   numResponses++;
   responsePotentials.resize(numResponses);
   for (i = 0; i < numResponses; i++)
   {
      responsePotentials[i] = 0.0;
   }
   i = numResponses - 1;
   newMotor(i);
   return(i);
}


// Get response potential.
Mona::RESPONSE_POTENTIAL Mona::getResponsePotential(RESPONSE response)
{
   if ((response >= 0) && (response < numResponses))
   {
      return(responsePotentials[response]);
   }
   else
   {
      return(0.0);
   }
}


// Override response.
// Auto-cleared each cycle.
bool Mona::overrideResponse(RESPONSE responseOverride)
{
   if ((responseOverride >= 0) && (responseOverride < numResponses))
   {
      this->responseOverride    = responseOverride;
      responseOverridePotential = -1.0;
      return(true);
   }
   else
   {
      return(false);
   }
}


// Conditionally override response if potential is less than given value.
// Auto-cleared each cycle.
bool Mona::overrideResponseConditional(RESPONSE           responseOverride,
                                       RESPONSE_POTENTIAL responseOverridePotential)
{
   if ((responseOverride >= 0) && (responseOverride < numResponses))
   {
      this->responseOverride          = responseOverride;
      this->responseOverridePotential = responseOverridePotential;
      return(true);
   }
   else
   {
      return(false);
   }
}


// Clear response override.
void Mona::clearResponseOverride()
{
   responseOverride          = NULL_RESPONSE;
   responseOverridePotential = -1.0;
}


// Find motor by response.
Mona::Motor *Mona::findMotorByResponse(RESPONSE response)
{
   Motor *motor;

   for (int i = 0; i < (int)motors.size(); i++)
   {
      motor = motors[i];
      if (motor->response == response)
      {
         return(motor);
      }
   }
   return(NULL);
}
