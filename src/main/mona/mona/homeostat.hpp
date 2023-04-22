// For conditions of distribution and use, see copyright notice in mona.hpp

/*
 * Homeostat.
 * Regulates:
 * 1. A need value based on environmental and timing conditions.
 * 2. Goal values that change the need value.
 */

#ifndef __HOMEOSTAT__
#define __HOMEOSTAT__

#include "mona.hpp"

class Mona;

// Homeostat.
class Homeostat
{
public:

   // Data types.
   typedef unsigned long long   ID;
   typedef float                SENSOR;
   typedef int                  SENSOR_MODE;
   typedef double               NEED;
   static const ID NULL_ID;

   // Sensory-response goal.
   // Receptor sensory stimuli and motor response that produce need changes.
   class Goal
   {
public:
      vector<SENSOR> sensors;
      SENSOR_MODE    sensorMode;
      vector<void *> receptors;
      void           *pendingReceptor;
      void           *motor;
      void           *mediator;
      NEED           goalValue;
      bool           enabled;
   };

   NEED         need;
   int          needIndex;
   NEED         needDelta;
   NEED         defaultNeed;
   Mona         *mona;
   NEED         periodicNeed;
   int          frequency;
   int          freqTimer;
   vector<Goal> goals;

   // Constructors.
   Homeostat();
   Homeostat(int needIndex, Mona *mona);

   // Destructor.
   ~Homeostat();

   // Get current need.
   inline NEED getNeed() { return(need); }
   inline NEED getAndClearNeedDelta()
   {
      NEED d = needDelta;

      needDelta = 0.0;
      return(d);
   }


   // Set current need.
   inline void setNeed(NEED need)
   {
      needDelta  = need - this->need;
      this->need = need;
   }


   // Set need with default.
   inline void setNeed(NEED need, NEED defaultNeed)
   {
      needDelta         = need - this->need;
      this->need        = need;
      this->defaultNeed = defaultNeed;
   }


   // Set default need.
   void setDefaultNeed();

   // Set periodic need.
   void setPeriodicNeed(int frequency, NEED need);
   void clearPeriodicNeed();

   // Add goal.
   // Replace motor and goal value of duplicate.
   int addGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode,
               void *motor, NEED goalValue);
   int addGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode,
               NEED goalValue);
   void addGoalReceptor(void *receptor);
   int addGoalMediator(void *mediator, NEED goalValue);

   // Find index of goal matching sensors, sensor mode
   // and motor. Return -1 for no match.
   int findGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode,
                void *motor);

   // Find index of goal matching sensors and sensor mode.
   // Return -1 for no match.
   int findGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode);

   // Find goal matching receptor.
   // Return false for not found.
   bool findGoalReceptor(void *receptor);

   // Find index of goal matching mediator.
   // Return -1 for no match.
   int findGoalMediator(void *mediator);

   // Get goal information at index.
   bool getGoalInfo(int goalIndex, vector<SENSOR>& sensors,
                    SENSOR_MODE& sensorMode, void **motor,
                    NEED& goalValue, bool& enabled);

   // Get receptors for goal at index.
   void getGoalReceptors(int goalIndex, vector<void *>& receptors);

   // Get motor for goal at index.
   void *getGoalMotor(int goalIndex);

   // Get mediator for goal at index.
   void *getGoalMediator(int goalIndex);

   // Is goal enabled?
   bool isGoalEnabled(int goalIndex);

   // Enable goal.
   bool enableGoal(int goalIndex);

   // Disable goal.
   bool disableGoal(int goalIndex);

   // Remove goal.
   bool removeGoal(int goalIndex);

   // Activate/deactivate goal with periodic need.
   bool activateGoal(int goalIndex,
                     int frequency, NEED periodicNeed);
   bool deactivateGoal(int goalIndex);

   // Remove neuron from goals.
   void removeNeuron(void *neuron);

   // Update homeostat based on receptor.
   void receptorUpdate(void *neuron);

   // Update homeostat based on motor firing.
   void motorUpdate();

   // Update homeostat based on mediator.
   void mediatorUpdate(void *mediator, double firingStrength);

   // Load homeostat.
   void load(char *filename);
   void load(FILE *);

   // Save homeostat.
   void save(char *filename);
   void save(FILE *);

   // Print homeostat.
   void print(FILE *out = stdout);

private:
   // Is goal superset of sensors?
   bool goalSuperset(int goalIndex, vector<SENSOR>& sensors);
};
#endif
