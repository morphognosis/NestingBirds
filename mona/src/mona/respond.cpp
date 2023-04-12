// For conditions of distribution and use, see copyright notice in mona.hpp

#include "mona.hpp"

// Respond.
void
Mona::respond()
{
   Motor              *motor, *maxMotiveMotor;

#ifdef MONA_TRACE
   if (traceRespond)
   {
      printf("***Respond phase***\n");
   }
#endif

   // Determine response that leads to place motor coordinates.
   assert(placeMotors.size() == 0 ||
       (movementBeginResponse != -1 && movementEndResponse != -1));
   for (int i = 0, j = (int)placeMotors.size(); i < j; i++)
   {
       motor = placeMotors[i];
       motor->placeResponse();
   }

   // Get response potentials from motor motives.
   for (int i = 0; i < numResponses; i++)
   {
      responsePotentials[i] = 0.0;
   }
   maxMotiveMotor = NULL;
   int n = (int)motors.size();
   int j = 0;
   if (n > 0)
   {
       j = random.RAND_CHOICE(n);
   }
   for (int i = 0; i < n; i++, j = (j + 1) % n)
   {
      motor = motors[j];
      motor->firingStrength = 0.0;
      if (motor->motive > responsePotentials[motor->response])
      {
          responsePotentials[motor->response] = motor->motive;
      }
      if (motor->motive > 0.0)
      {
          if (maxMotiveMotor == NULL)
          {
              maxMotiveMotor = motor;
          }
          else if (motor->motive > maxMotiveMotor->motive)
          {
              maxMotiveMotor = motor;
          }
          else if (motor->motive == maxMotiveMotor->motive)
          {
              if (random.RAND_CHOICE(2))
              {
                  maxMotiveMotor = motor;
              }
          }
      }
   }
   n = (int)placeMotors.size();
   j = 0;
   if (n > 0)
   {
       j = random.RAND_CHOICE(n);
   }
   for (int i = 0; i < n; i++, j = (j + 1) % n)
   {
       motor = placeMotors[j];
       motor->firingStrength = 0.0;
       if (motor->motive > responsePotentials[motor->response])
       {
           responsePotentials[motor->response] = motor->motive;
       }
       if (motor->motive > 0.0)
       {
           if (maxMotiveMotor == NULL)
           {
               maxMotiveMotor = motor;
           }
           else if (motor->motive > maxMotiveMotor->motive)
           {
               maxMotiveMotor = motor;
           }
           else if (motor->motive == maxMotiveMotor->motive)
           {
                   if (maxMotiveMotor->isPlaceMotor())
                   {
                       if (random.RAND_CHOICE(2))
                       {
                           maxMotiveMotor = motor;
                       }
                   }
                   else {
                       maxMotiveMotor = motor;
                   }
           }
       }
   }

   // Incorporate minimal randomness.
   for (int i = 0; i < numResponses; i++)
   {
      if (responsePotentials[i] <= NEARLY_ZERO)
      {
         responsePotentials[i] = (random.RAND_PROB() * RESPONSE_RANDOMNESS);
      }
   }

   // Make a random response selection?
   if (random.RAND_CHANCE(RESPONSE_RANDOMNESS))
   {
      if (numResponses > 0)
      {
         double *responseWeights = new double[numResponses];
         assert(responseWeights != NULL);
         for (int i = 0; i < numResponses; i++)
         {
            responseWeights[i] = responsePotentials[i];
            if (i > 0)
            {
               responseWeights[i] += responseWeights[i - 1];
            }
         }
         int    j = numResponses - 1;
         double n = random.RAND_INTERVAL(0.0, responseWeights[j]);
         int i;
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
      int j = random.RAND_CHOICE(numResponses);
      RESPONSE_POTENTIAL max = 0.0;
      for (int i = response = 0; i < numResponses; i++)
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
       activePlaceMotor = maxMotiveMotor = NULL;

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

   // Place motor execution.
   if (activePlaceMotor != NULL)
   {
       response = activePlaceMotor->response;
       if (activePlaceMotor->response == movementEndResponse)
       {
           activePlaceMotor->firingStrength = 1.0;
#ifdef MONA_TRACE
           if (traceRespond)
           {
               printf("Place motor firing: %llu\n", activePlaceMotor->id);
           }
#endif
#ifdef MONA_TRACKING
           activePlaceMotor->tracker.fire = true;
#endif
           activePlaceMotor = NULL;
       }
   }
   else {
       if (maxMotiveMotor != NULL && maxMotiveMotor->isPlaceMotor() &&
           maxMotiveMotor->response == movementBeginResponse)
       {
           response = movementBeginResponse;
           activePlaceMotor = maxMotiveMotor;
       }
   }

   // Fire responding motor.
       for (int i = 0, j = (int)motors.size(); i < j; i++)
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
       }

   // Update need based on motor firing.
   for (int i = 0; i < numNeeds; i++)
   {
       homeostats[i]->motorUpdate();
   }

#ifdef MONA_TRACE
   if (traceRespond)
   {
       printf("Response potentials:\n");
       for (int i = 0; i < numResponses; i++)
       {
          printf("%f ", responsePotentials[i]);
       }
       printf("\n");
      printf("Response = %d\n", response);
   }
#endif
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

// Set response to move to place location.
void Mona::Motor::placeResponse()
{
    if (mona->activePlaceMotor == this)
    {
            if (mona->X != x || mona->Y != y)
            {
                response = gotoPlace(mona->orientation, mona->X, mona->Y, x, y);
            }
            else {
                response = mona->movementEndResponse;
            }
    }
    else {
        response = mona->movementBeginResponse;
    }
}

// Get response from current to target. 
int Mona::Motor::gotoPlace(int orientation, int fromX, int fromY, int toX, int toY)
{
    if (fromX > toX)
    {
        if (orientation == Mona::ORIENTATION::WEST)
        {
            return Mona::MOVEMENT_TYPE::MOVE_FORWARD;
        }
        else
        {
            if (orientation == Mona::ORIENTATION::SOUTH)
            {
                return Mona::MOVEMENT_TYPE::TURN_RIGHT;
            }
            else if (orientation == Mona::ORIENTATION::NORTH)
            {
                return Mona::MOVEMENT_TYPE::TURN_LEFT;
            }
            else {
                return Mona::MOVEMENT_TYPE::TURN_AROUND;
            }
        }
        return(true);
    }
    if (fromX < toX)
    {
        if (orientation == Mona::ORIENTATION::EAST)
        {
            return Mona::MOVEMENT_TYPE::MOVE_FORWARD;
        }
        else
        {
            if (orientation == Mona::ORIENTATION::SOUTH)
            {
                return Mona::MOVEMENT_TYPE::TURN_LEFT;
            }
            else if (orientation == Mona::ORIENTATION::NORTH)
            {
                return Mona::MOVEMENT_TYPE::TURN_RIGHT;
            }
            else
            {
                return Mona::MOVEMENT_TYPE::TURN_AROUND;
            }
        }
        return(true);
    }
    if (fromY > toY)
    {
        if (orientation == Mona::ORIENTATION::NORTH)
        {
            return Mona::MOVEMENT_TYPE::MOVE_FORWARD;
        }
        else
        {
            if (orientation == Mona::ORIENTATION::EAST)
            {
                return Mona::MOVEMENT_TYPE::TURN_LEFT;
            }
            else if (orientation == Mona::ORIENTATION::WEST)
            {
                return Mona::MOVEMENT_TYPE::TURN_RIGHT;
            }
            else
            {
                return Mona::MOVEMENT_TYPE::TURN_AROUND;
            }
        }
        return(true);
    }
    if (fromY < toY)
    {
        if (orientation == Mona::ORIENTATION::SOUTH)
        {
            return Mona::MOVEMENT_TYPE::MOVE_FORWARD;
        }
        else
        {
            if (orientation == Mona::ORIENTATION::EAST)
            {
                return Mona::MOVEMENT_TYPE::TURN_RIGHT;
            }
            else if (orientation == Mona::ORIENTATION::WEST)
            {
                return Mona::MOVEMENT_TYPE::TURN_LEFT;
            }
            else
            {
                return Mona::MOVEMENT_TYPE::TURN_AROUND;
            }
        }
    }
    return Mona::MOVEMENT_TYPE::DO_NOTHING;
}

// Is given motor a duplicate of this?
bool Mona::Motor::isDuplicate(Motor* motor)
{
    if (!isPlaceMotor())
    {
        if (!motor->isPlaceMotor())
        {
            if (response == motor->response)
            {
                assert(movementType == motor->movementType);
                return(true);
            }
        }
    }
    else {
        if (motor->isPlaceMotor())
        {
            if (x == motor->x && y == motor->y)
            {
                assert(movementType == motor->movementType);
                return(true);
            }
        }
    }
    return false;
}

