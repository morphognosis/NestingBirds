/*
 * Copyright (c) 2011-2023 Tom Portegys (portegys@gmail.com). All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY TOM PORTEGYS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Mona artificial neural network.

#ifndef __MONA__
#define __MONA__

// Version (SCCS "what" format).
#define MONA_VERSION    "@(#)Mona version 6.0"
extern const char *MonaVersion;

// Tracing and event tracking compilation symbols:
// MONA_TRACE: use with run-time tracing flags below for execution flow trace.
// MONA_TRACKING: use with dump and print options for firing, enabling and drive
// event tracking.
#define MONA_TRACE 1

#ifdef WIN32
#define PUBLIC_API    __declspec(dllexport)
#pragma warning( disable: 4251 )
#else
#define PUBLIC_API
#endif

#include "../common/common.h"
#include "homeostat.hpp"

// Mona: sensory/response, neural network, and needs.
class PUBLIC_API Mona
{
public:

   // Content format.
   enum { FORMAT=11 };

   // Data types.
   typedef Homeostat::ID            ID;
   typedef Homeostat::SENSOR        SENSOR;
   typedef Homeostat::SENSOR_MODE   SENSOR_MODE;
   typedef int                      RESPONSE;
   typedef double                   RESPONSE_POTENTIAL;
   typedef Homeostat::NEED          NEED;
   typedef double                   MOTIVE;
   typedef double                   ENABLEMENT;
   typedef double                   UTILITY;
   typedef double                   WEIGHT;
   typedef unsigned long long       COUNTER;
   enum NEURON_TYPE
   {
      RECEPTOR,
      MOTOR,
      MEDIATOR
   };
   enum EVENT_TYPE
   {
      CAUSE_EVENT,
      RESPONSE_EVENT,
      EFFECT_EVENT
   };
   enum EVENT_OUTCOME
   {
      EXPIRE,
      FIRE
   };

   static const ID NULL_ID;
   enum { NULL_RESPONSE = Homeostat::NULL_RESPONSE };
   enum { DEFAULT_RANDOM_SEED = 4517 };

   // Print version.
   static void printVersion(FILE *out = stdout);

   // Construct/initialize.
   Mona();
   Mona(int numSensors, int numResponses, int numNeeds,
        RANDOM randomSeed = DEFAULT_RANDOM_SEED);
   void initParms();
   void initNet(int numSensors, int numResponses, int numNeeds,
                RANDOM randomSeed = DEFAULT_RANDOM_SEED);
   bool setSensorResolution(SENSOR sensorResolution);
   int addSensorMode(vector<bool>& sensorMask);
   int addSensorMode(vector<bool>& sensorMask, SENSOR sensorResolution);
   bool delSensorMode(int mode);

   // Destructor.
   ~Mona();

   // Include auxiliary classes.
#include "mona-aux.hpp"

   // Parameters.
   // Initialize, load, save, and print in mona.cpp and mona_jni.cpp
   ENABLEMENT MIN_ENABLEMENT;
   ENABLEMENT INITIAL_ENABLEMENT;
   WEIGHT     DRIVE_ATTENUATION;
   WEIGHT     FIRING_STRENGTH_LEARNING_DAMPER;
   WEIGHT     LEARNING_DECREASE_VELOCITY;
   WEIGHT     LEARNING_INCREASE_VELOCITY;
   WEIGHT     RESPONSE_RANDOMNESS;
   int MIN_MOVEMENT_RESPONSE_PATH_LENGTH;
   int MAX_MOVEMENT_RESPONSE_PATH_LENGTH;
   WEIGHT     UTILITY_ASYMPTOTE;
   int        DEFAULT_MAX_LEARNING_EFFECT_EVENT_INTERVAL;
   int        DEFAULT_NUM_EFFECT_EVENT_INTERVALS;
   int        MAX_MEDIATORS;
   int        MAX_MEDIATOR_LEVEL;
   int        MAX_RESPONSE_EQUIPPED_MEDIATOR_LEVEL;
   int        MIN_RESPONSE_UNEQUIPPED_MEDIATOR_LEVEL;
   SENSOR     SENSOR_RESOLUTION;
   bool       LEARN_MEDIATOR_GOAL_VALUE;
   int        LEARN_MEDIATOR_GOAL_VALUE_MIN_LEVEL;
   bool       LEARN_RECEPTOR_GOAL_VALUE;

#ifdef MONA_TRACE
   // Tracing.
   bool traceSense;
   bool traceEnable;
   bool traceLearn;
   bool traceDrive;
   bool traceRespond;
#endif

   // Sensors.
   vector<SENSOR> sensors;
   int            numSensors;

   // Sensor modes.
   // A sensor mode is a set of sensors related
   // by "modality", analogous to sight, hearing,
   // touch. Sensing for a specific sensor mode
   // maps the next sensor vector to the closest
   // receptor in a space reserved for the mode. This
   // provides the network with selective attention
   // capabilities.
   vector<SensorMode *> sensorModes;
   void                 applySensorMode(vector<SENSOR> &in, vector<SENSOR> &out, SENSOR_MODE);
   void                 applySensorMode(vector<SENSOR> &sensors, SENSOR_MODE);

   // Sensor centroid search spaces.
   // Each sensor mode defines a space.
   vector<RDtree *> sensorCentroids;

   // Find the receptor having the centroid closest to
   // the sensor vector for the give sensor mode.
   Receptor *getCentroidReceptor(vector<SENSOR>& sensors,
                                 SENSOR_MODE sensorMode, SENSOR& distance);

   // Response.
   RESPONSE response;
   int      numResponses;
   vector<RESPONSE_POTENTIAL> responsePotentials;
   RESPONSE_POTENTIAL         getResponsePotential(RESPONSE);
   RESPONSE           responseOverride;
   RESPONSE_POTENTIAL responseOverridePotential;
   bool               overrideResponse(RESPONSE);
   bool               overrideResponseConditional(RESPONSE, RESPONSE_POTENTIAL);
   void clearResponseOverride();
   Motor *findMotorByResponse(RESPONSE response);
   int movementResponsePathLength;
   vector<LearningEvent*> movementCauses;
   vector<LearningEvent*> movementEffects;

   // Needs.
   int                 numNeeds;
   vector<Homeostat *> homeostats;

   // Need management.
   NEED getNeed(int index);
   void setNeed(int index, NEED value);
   void inflateNeed(int index);
   void setPeriodicNeed(int needIndex,
                        int frequency, NEED periodicNeed);
   void clearPeriodicNeed(int needIndex);
   void printNeed(int index);

   // Goal management.
   int addGoal(int needIndex, vector<SENSOR>& sensors,
               SENSOR_MODE sensorMode, RESPONSE response, NEED goalValue);
   int addGoal(int needIndex, vector<SENSOR>& sensors,
               SENSOR_MODE sensorMode, NEED goalValue);
   int addGoal(int needIndex, Mediator* mediator, NEED goalValue);
   int findGoal(int needIndex, vector<SENSOR>& sensors,
                SENSOR_MODE sensorMode, RESPONSE response);
   int findGoal(int needIndex, vector<SENSOR>& sensors,
                SENSOR_MODE sensorMode);
   int findGoal(int needIndex, Mediator *mediator);
   int getNumGoals(int needIndex);
   bool getGoalInfo(int needIndex, int goalIndex,
                    vector<SENSOR>& sensors, SENSOR_MODE& sensorMode,
                    RESPONSE& response, NEED& goalValue, bool& enabled);
   void getGoalReceptors(int needIndex, int goalIndex, vector<void*>& receptors);
   Motor* getGoalMotor(int needIndex, int goalIndex);
   Mediator* getGoalMediator(int needIndex, int goalIndex);
   bool isGoalEnabled(int needIndex, int goalIndex);
   bool enableGoal(int needIndex, int goalIndex);
   bool disableGoal(int needIndex, int goalIndex);
   bool removeGoal(int needIndex, int goalIndex);

   // Effect event intervals.
   // The maximum time for an effect event to fire after a cause fires.
   // 1. There must be at least one interval per mediator level.
   // 2. Response-equipped mediators are considered "immediate" mediators
   //    for which only the first interval is applicable with 100% weight.
   // 3. Customizable by application at initialization time. See initEffectIntervals.
   vector<vector<TIME> > effectEventIntervals;

   // Effect interval weights determine how enablement is distributed to enablings
   // timed by the effect event intervals. Causes that have more immediate effects
   // are plausibly more strongly related than those with more temporally distance
   // effects. This can be represented by weighting smaller intervals more heavily
   // than larger ones. effectIntervalWeights[n] must sum to 1.0
   // Customizable by application at initialization time. See initEffectIntervals.
   vector<vector<WEIGHT> > effectEventIntervalWeights;

   // Maximum learning effect event intervals.
   // 1. These are the maximum firing intervals between events allowable
   //     for the learning of new mediators.
   // 2. These can be shorter than the intervals allowed for the events of
   //     existing mediators as a means of improving the quality and throttling
   //     the quantity of learned mediators.
   // 3. Customizable by application at initialization time. See initEffectIntervals.
   vector<TIME> maxLearningEffectEventIntervals;

   // Initialize effect event intervals and weights.
   void initEffectEventIntervals();
   void initEffectEventInterval(int level, int numIntervals);
   void initEffectEventIntervalWeights();
   void initEffectEventIntervalWeight(int level);
   void initMaxLearningEffectEventIntervals();
   bool auditDefaultEffectEventIntervals();

   // Behavior cycle.
   RESPONSE cycle(vector<SENSOR>& sensors);
   void sense();
   void enable();
   void learn();
   void drive();
   void respond();

   // Cause event firing notifications.
   list<struct FiringNotify> causeFirings;

   // Motive.
   MOTIVE maxMotive;
   void clearMotiveWork();
   void setMotives();
   void finalizeMotives();

   // Unique identifier dispenser.
   COUNTER idDispenser;

   // Event clock.
   TIME eventClock;

   // Learning event lists.
   vector<list<LearningEvent *> > learningEvents;
   vector<GeneralizationEvent *>  generalizationEvents;

   // Mediator generation.
   void createPlaceMediators();
   void createMediator(LearningEvent *event);
   void generalizeMediator(GeneralizationEvent *event);
   bool isDuplicateMediator(Mediator *);

   // Random numbers.
   RANDOM randomSeed;
   Random random;

#ifdef MONA_TRACKING
   // Tracking flags.
   typedef unsigned int   TRACKING_FLAGS;
   enum { TRACK_FIRE=1, TRACK_ENABLE=2, TRACK_DRIVE=4 };
#endif

   // Neuron.
   class Neuron
   {
public:
      // Initialize/clear.
      void init(Mona *mona);
      void clear();

      // Identifier.
      ID id;

      // Neuron type.
      NEURON_TYPE type;

      // Creation time.
      TIME creationTime;

      // Firing strength.
      ENABLEMENT firingStrength;

      // Goal value.
      GoalValue goals;

      // Motive.
      MOTIVE motive;
      bool   motiveValid;
      void   drive(MotiveAccum);
      void initDrive(VALUE_SET& needs);
      void clearMotiveWork();
      void setMotive();
      void finalizeMotive();

      MotiveAccum           motiveWork;
      bool                  motiveWorkValid;
      map<Neuron *, double> driveWeights;

      // Instinct?
      bool instinct;

      // Parent is instinct?
      bool hasParentInstinct();

      // Event notification.
      vector<struct Notify *> notifyList;

      // Neural network.
      Mona *mona;

      // Load.
      void load(FILE *fp);

      // Save.
      void save(FILE *fp);

#ifdef MONA_TRACKING
      // Track neuron activity.
      class Activation
      {
public:
         bool   fire;
         bool   enable;
         bool   drive;
         MOTIVE motive;
         struct DrivePath
         {
            vector<struct MotiveAccum::DriveElem> drivers;
            MotiveAccum                           motiveWork;
            MOTIVE                                motive;
         };
         vector<struct DrivePath> motivePaths;
         vector<struct DrivePath> motiveWorkPaths;
         void clear()
         {
            fire   = enable = drive = false;
            motive = 0.0;
            motivePaths.clear();
            motiveWorkPaths.clear();
         }
      }
      tracker;

      // Track driven motive.
      bool trackMotive(MotiveAccum& in, MotiveAccum& out);

      // Accumulate motive tracking.
      void accumMotiveTracking();
#endif
   };

   // Receptor neuron.
   // Contains a centroid in sensor vector space.
   // Fires when centroid is closest to input sensor vector.
   class Receptor : public Neuron
   {
public:

      // Construct/destruct.
      Receptor(vector<SENSOR>& centroid, SENSOR_MODE sensorMode, Mona *mona);
      ~Receptor();

      // Centroid sensor vector.
      vector<SENSOR> centroid;

      // Sensor mode and links to receptors
      // with subset and superset sensor modes.
      SENSOR_MODE        sensorMode;
      vector<Receptor *> subSensorModes;
      vector<Receptor *> superSensorModes;

      // Get distance from centroid to given sensor vector.
      SENSOR centroidDistance(vector<SENSOR>& sensors);

      // Get distance between sensor vectors.
      static SENSOR sensorDistance(vector<SENSOR> *sensorsA,
                                   vector<SENSOR> *sensorsB);

      // Is given receptor a duplicate of this?
      bool isDuplicate(Receptor *);

      // Add goals.
      void addGoals();

      // Update needs.
      void updateNeeds();

      // Update goal values.
      void updateGoalValues();

      // Load receptor.
      void load(FILE *fp);

      // Save receptor.
      void save(FILE *fp);

      // RDtree sensor vector search.
      static SENSOR patternDistance(void *sensorsA, void *sensorsB);
      static void *loadPattern(void *mona, FILE *fp);
      static void savePattern(void *sensors, FILE *fp);
      static void *loadClient(void *mona, FILE *fp);
      static void saveClient(void *receptor, FILE *fp);
      static void deletePattern(void *pattern);

      // Print receptor.
      void print(FILE *out = stdout);

#ifdef MONA_TRACKING
      void print(TRACKING_FLAGS tracking, FILE *out = stdout);
#endif
   };

   // Motor neuron.
   class Motor : public Neuron
   {
public:

      // Construct/destruct.
      Motor(int movementType, Mona *mona);
      Motor(int x, int y, Mona* mona);
      ~Motor();

      // Response.
      RESPONSE response;

      // Movement type.
      int movementType;

      // Place coordinates.
      // If these are non-zero, this is a place motor, 
      // meaning that the response navigates to these coordinates.
      int x, y;

      // Set response to move to place coordinates.
      void placeResponse();
      static int gotoPlace(int orientation, int fromX, int fromY, int toX, int toY);

      // Is given motor a duplicate of this?
      bool isDuplicate(Motor *);

      // Load motor.
      void load(FILE *fp);

      // Save motor.
      void save(FILE *fp);

      // Print motor.
      void print(FILE *out = stdout);

#ifdef MONA_TRACKING
      void print(TRACKING_FLAGS tracking, FILE *out = stdout);
#endif
   };

   // Mediator neuron.
   // Mediates a set of neuron events.
   class Mediator : public Neuron
   {
public:
      // Construct/destruct.
      Mediator(ENABLEMENT enablement, Mona *mona);
      ~Mediator();

      // Level 0 mediator is composed of non-mediator neurons.
      // Level n mediator is composed of at most level n-1 mediators.
      int level;

      // Enablement.
      ENABLEMENT baseEnablement;
      ENABLEMENT getEnablement();
      void updateEnablement(EVENT_OUTCOME outcome,
                            WEIGHT        updateWeight);

      // Effective enablement.
      ENABLEMENT effectiveEnablement;
      WEIGHT     effectiveEnablingWeight;
      bool       effectiveEnablementValid;
      void updateEffectiveEnablement();

      // Utility.
      UTILITY utility;
      WEIGHT  utilityWeight;
      void updateUtility(WEIGHT updateWeight);
      UTILITY getEffectiveUtility();

      // Update goal value.
      void updateGoalValue(VALUE_SET& needs);

      // Is goal value subsumed by component?
      bool goalValueSubsumed();

      // Events.
      Neuron *cause;
      Neuron *response;
      Neuron *effect;
      void   addEvent(EVENT_TYPE, Neuron *);

      // Enablings.
      EnablingSet responseEnablings;
      EnablingSet effectEnablings;

      // Time of causation.
      TIME causeBegin;

      // Event firing.
      void causeFiring(WEIGHT notifyStrength, TIME causeBegin);
      void responseFiring(WEIGHT notifyStrength);
      void effectFiring(WEIGHT notifyStrength);
      void retireEnablings(bool force = false);

      // Drive.
      void driveCause(MotiveAccum);

      // Is given mediator a duplicate of this?
      bool isDuplicate(Mediator *);

      // Load mediator.
      void load(FILE *fp);

      // Save mediator.
      void save(FILE *fp);

      // Print mediator.
      void print(FILE *out = stdout);
      void printBrief(FILE *out = stdout);

#ifdef MONA_TRACKING
      void print(TRACKING_FLAGS tracking, FILE *out = stdout);
      void printBrief(TRACKING_FLAGS tracking, FILE *out = stdout);
      void print(TRACKING_FLAGS tracking, bool brief, int level, FILE *out);

#else
      void print(bool brief, int level, FILE *out);
#endif
   };

   // Network.
   vector<Receptor *> receptors;
   vector<Motor *>    motors;
   vector<Motor*> placeMotors;
   list<Mediator *>   mediators;

   // Place.

   // Movement type.
   class MOVEMENT_TYPE
   {
   public:
       static const int DO_NOTHING = 0;
       static const int MOVE_FORWARD = 1;
       static const int TURN_RIGHT = 2;
       static const int TURN_LEFT = 3;
       static const int TURN_AROUND = 4;
   };

   // Orientation.
   class ORIENTATION
   {
   public:
       static const int NORTH = 0;
       static const int SOUTH = 1;
       static const int EAST = 2;
       static const int WEST = 3;
   };
   int orientation;
   int X, Y;

   // Add/delete neurons to/from network.
   Receptor *newReceptor(vector<SENSOR>& centroid, SENSOR_MODE sensorMode);
   Motor *newMotor();
   Motor* newMovementMotor(int movementType);
   Motor* newPlaceMotor(int x, int y);
   Mediator *newMediator(ENABLEMENT enablement);
   void deleteNeuron(Neuron *);

   // Clear memory.
   void expireResponseEnablings(RESPONSE);
   void expireMediatorEnablings(Mediator *);
   void clearWorkingMemory();
   void clearLongTermMemory();

   // Get mediators with worst and best utilities.
   Mediator *getWorstMediator(int minLevel = 0);
   Mediator *getBestMediator(int minLevel = 0);

   // Load network.
   bool load(char *filename);
   bool load(FILE *fp);
   Neuron *findByID(ID id);

   // Save network.
   bool save(char *filename);
   bool save(FILE *fp);

   // Clear network.
   void clear();

   // Print network.
   bool print(char *filename);
   void print(FILE *out = stdout);
   void printBrief(FILE *out = stdout);
   void printParms(FILE *out = stdout);

#ifdef MONA_TRACKING
   void print(TRACKING_FLAGS tracking, FILE *out = stdout);
   void printBrief(TRACKING_FLAGS tracking, FILE *out = stdout);
#endif

   // Print helpers.
#ifdef MONA_TRACKING
   void print(TRACKING_FLAGS tracking, bool brief, FILE *out = stdout);

#else
   void print(bool brief, FILE *out = stdout);
#endif

#ifdef MONA_TRACKING
   enum { MAX_DRIVER_TRACKS=3 };

   // Clear tracking activity.
   void clearTracking();
#endif

private:
   // Clear variables.
   void clearVars();
};
#endif
