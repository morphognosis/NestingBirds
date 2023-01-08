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
   typedef int                  RESPONSE;
   typedef double               NEED;
   static const ID NULL_ID;
   enum { NULL_RESPONSE = 0x7fffffff };

   // Sensory-response goal.
   // Environmental stimuli and response that produce need changes.
   // Used to update receptor neuron goal values.
   class Goal
   {
public:
      vector<SENSOR> sensors;
      SENSOR_MODE    sensorMode;
      void           *receptor;
      RESPONSE       response;
      void           *motor;
      NEED           goalValue;
      bool           enabled;
   };

   NEED         need;
   int          needIndex;
   NEED         needDelta;
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


   // Set periodic need.
   void setPeriodicNeed(int frequency, NEED need);
   void clearPeriodicNeed();

   // Add goal.
   // Replace response and goal value of duplicate.
   int addGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode,
               RESPONSE response, NEED goalValue);
   int addGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode,
               NEED goalValue);

   // Add goal value to receptor if needed.
   void addGoal(void *receptor);

   // Find index of goal matching sensors, sensor mode
   // and response. Return -1 for no match.
   int findGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode,
                RESPONSE response);

   // Find index of goal matching sensors and sensor mode.
   // Return -1 for no match.
   int findGoal(vector<SENSOR>& sensors, SENSOR_MODE sensorMode);

   // Get goal information at index.
   bool getGoalInfo(int goalIndex, vector<SENSOR>& sensors,
                    SENSOR_MODE& sensorMode, RESPONSE& response,
                    NEED& goalValue, bool& enabled);

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

   // Update homeostat based on sensors.
   void sensorsUpdate();

   // Update homeostat based on response.
   void responseUpdate();

   // Load homeostat.
   void load(char *filename);
   void load(FILE *);

   // Save homeostat.
   void save(char *filename);
   void save(FILE *);

   // Print homeostat.
   void print(FILE *out = stdout);
};
#endif
