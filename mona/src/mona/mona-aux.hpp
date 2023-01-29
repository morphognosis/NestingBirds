// For conditions of distribution and use, see copyright notice in mona.hpp

// Mona auxiliary classes.

#ifndef __MONA_AUX__
#define __MONA_AUX__

// Forward declarations.
class Neuron;
class Receptor;
class Motor;
class Mediator;
class EnablingSet;

// Sensor mode.
class SensorMode
{
public:
   SENSOR_MODE         mode;
   vector<bool>        mask;
   SENSOR              resolution;
   vector<SENSOR_MODE> subsets;
   vector<SENSOR_MODE> supersets;

   // Constructor.
   SensorMode()
   {
      mode       = 0;
      resolution = 0.0f;
   }


   // Initialize.
   // Return mode.
   int init(vector<bool>& mask, SENSOR resolution,
            vector<SensorMode *> *sensorModes)
   {
      bool       sub, super;
      SensorMode *s;

      // Initialize.
      this->mask.clear();
      for (int i = 0, j = (int)mask.size(); i < j; i++)
      {
         this->mask.push_back(mask[i]);
      }
      mode             = (int)sensorModes->size();
      this->resolution = resolution;

      // Determine subset/supersets.
      for (int i = 0, j = (int)sensorModes->size(); i < j; i++)
      {
         s = (*sensorModes)[i];
         subSuper(s, sub, super);
         if (sub && super) { continue; }
         if (sub)
         {
            if (!subTransitive(i, sensorModes))
            {
               subsets.push_back(i);
               s->supersets.push_back(j);
            }
         }
         else if (super)
         {
            if (!superTransitive(i, sensorModes))
            {
               supersets.push_back(i);
               s->subsets.push_back(j);
            }
         }
      }
      sensorModes->push_back(this);
      return(mode);
   }


   // Given mode is subset/superset?
   void subSuper(SensorMode *sensorMode, bool& sub, bool& super)
   {
      sub = super = true;
      for (int i = 0, j = (int)mask.size() && (sub || super); i < j; i++)
      {
         if (mask[i])
         {
            if (!sensorMode->mask[i])
            {
               sub = false;
            }
         }
         else
         {
            if (sensorMode->mask[i])
            {
               super = false;
            }
         }
      }
   }


   // Given mode is a transitive subset?
   // if A->B and B->C, then C is transitive.
   bool subTransitive(int idx, vector<SensorMode *> *sensorModes)
   {
      bool       sub, super;
      SensorMode *s;

      for (int i = 0, j = (int)subsets.size(); i < j; i++)
      {
         s = (*sensorModes)[i];
         s->subSuper((*sensorModes)[idx], sub, super);
         if (sub || s->subTransitive(idx, sensorModes))
         {
            return(true);
         }
      }
      return(false);
   }


   // Given mode is a transitive superset?
   // if A->B and B->C, then C is transitive.
   bool superTransitive(int idx, vector<SensorMode *> *sensorModes)
   {
      bool       sub, super;
      SensorMode *s;

      for (int i = 0, j = (int)supersets.size(); i < j; i++)
      {
         s = (*sensorModes)[i];
         s->subSuper((*sensorModes)[idx], sub, super);
         if (super || s->superTransitive(idx, sensorModes))
         {
            return(true);
         }
      }
      return(false);
   }


   // Load.
   void load(FILE *fp)
   {
      int  i, j, size;
      bool v;

      FREAD_INT(&mode, fp);
      mask.clear();
      subsets.clear();
      supersets.clear();
      FREAD_INT(&size, fp);
      for (i = 0; i < size; i++)
      {
         FREAD_BOOL(&v, fp);
         mask.push_back(v);
      }
      FREAD_FLOAT(&resolution, fp);
      FREAD_INT(&size, fp);
      for (i = 0; i < size; i++)
      {
         FREAD_INT(&j, fp);
         subsets.push_back(j);
      }
      FREAD_INT(&size, fp);
      for (i = 0; i < size; i++)
      {
         FREAD_INT(&j, fp);
         supersets.push_back(j);
      }
   }


   // Save.
   // When changing format increment FORMAT in mona.hpp
   void save(FILE *fp)
   {
      int  i, j, size;
      bool v;

      FWRITE_INT(&mode, fp);
      size = (int)mask.size();
      FWRITE_INT(&size, fp);
      for (i = 0; i < size; i++)
      {
         v = mask[i];
         FWRITE_BOOL(&v, fp);
      }
      FWRITE_FLOAT(&resolution, fp);
      size = (int)subsets.size();
      FWRITE_INT(&size, fp);
      for (i = 0; i < size; i++)
      {
         j = subsets[i];
         FWRITE_INT(&j, fp);
      }
      size = (int)supersets.size();
      FWRITE_INT(&size, fp);
      for (i = 0; i < size; i++)
      {
         j = supersets[i];
         FWRITE_INT(&j, fp);
      }
   }


   // Print.
   void print(FILE *out = stdout)
   {
      int i, j, size;

      fprintf(out, "<mode>%d</mode>", mode);
      fprintf(out, "<mask>");
      size = (int)mask.size();
      for (i = 0; i < size; i++)
      {
         j = (int)mask[i];
         fprintf(out, "%d", j);
      }
      fprintf(out, "</mask>");
      fprintf(out, "<resolution>%f</resolution>", resolution);
      fprintf(out, "<subsets>");
      size = (int)subsets.size();
      for (i = 0; i < size; i++)
      {
         j = subsets[i];
         fprintf(out, "%d", j);
      }
      fprintf(out, "</subsets>");
      fprintf(out, "<supersets>");
      size = (int)supersets.size();
      for (i = 0; i < size; i++)
      {
         j = supersets[i];
         fprintf(out, "%d", j);
      }
      fprintf(out, "</supersets>");
   }
};

// Goal value.
class GoalValue
{
public:
   VALUE_SET values;
   COUNTER   updateCount;
   Mona      *mona;

   // Constructors.
   GoalValue(int numGoals, Mona *mona)
   {
      values.alloc(numGoals);
      updateCount = 0;
      this->mona  = mona;
   }


   GoalValue()
   {
      updateCount = 0;
      mona        = NULL;
   }


   // Initialize.
   void init(int numGoals, Mona *mona)
   {
      values.alloc(numGoals);
      this->mona = mona;
   }


   // Destructor.
   ~GoalValue()
   {
      clear();
   }


   // Get total goal value.
   inline NEED getValue()
   {
      return(values.sum());
   }


   // Get specific goal value.
   inline NEED getValue(int index)
   {
      return(values.get(index));
   }


   // Set specific goal value.
   inline void setValue(int index, NEED value)
   {
      values.set(index, value);
   }


   // Set initial goals.
   inline void setGoals(VALUE_SET& goals)
   {
      values.load(goals);
   }


   // Get number of goals.
   inline int getNumGoals()
   {
      return(values.size());
   }


   // Update.
   void update(VALUE_SET& needs, VALUE_SET& needDeltas)
   {
      int    i, j;
      double v;

      updateCount++;
      for (i = 0, j = needs.size(); i < j; i++)
      {
         if (needDeltas.get(i) > 0.0)
         {
            v = values.get(i);
            values.set(i, v - ((v / (double)updateCount) *
                               mona->LEARNING_DECREASE_VELOCITY));
            continue;
         }
         if (needDeltas.get(i) == 0.0)
         {
            if (needs.get(i) > 0.0)
            {
               v = values.get(i);
               values.set(i, v - ((v / (double)updateCount) *
                                  mona->LEARNING_DECREASE_VELOCITY));
            }
            continue;
         }
         v = values.get(i) + needDeltas.get(i);
         if (needs.get(i) == 0.0)
         {
            if (v >= 0.0)
            {
               continue;
            }
         }
         values.set(i, values.get(i) + ((-v / (double)updateCount) *
                                        mona->LEARNING_INCREASE_VELOCITY));
      }
   }


   // Clear.
   inline void clear()
   {
      values.clear();
      updateCount = 0;
   }


   // Load.
   void load(FILE *fp)
   {
      clear();
      values.load(fp);
      FREAD_LONG_LONG(&updateCount, fp);
   }


   // Save.
   void save(FILE *fp)
   {
      values.save(fp);
      FWRITE_LONG_LONG(&updateCount, fp);
   }


   // Print.
   void print(FILE *out = stdout)
   {
      for (int i = 0; i < values.size(); i++)
      {
         fprintf(out, "<value>%f</value>", values.get(i));
      }
      fprintf(out, "<updateCount>%llu</updateCount>", updateCount);
   }
};

// Motive accumulator.
// Accumulates need change caused by goals.
class MotiveAccum
{
public:
   VALUE_SET        base;
   VALUE_SET        delta;
   WEIGHT           weight;
   vector<Neuron *> path;
#ifdef MONA_TRACKING
   struct DriveElem
   {
      Neuron *neuron;
      MOTIVE motive;
   };
   vector<struct DriveElem> drivers;
#endif

   // Constructor.
   MotiveAccum()
   {
      clear();
   }


   // Destructor.
   ~MotiveAccum()
   {
      clear();
   }


   // Get value of accumulator.
   // This is the sum of the delta values
   // bounded by the base need values.
   inline MOTIVE getValue()
   {
      double b, d, v;

      v = 0.0;
      for (int i = 0; i < delta.size(); i++)
      {
         b = base.get(i);
         d = delta.get(i);
         if (d < 0.0)
         {
            if ((b + d) < 0.0) { v += b; } else{ v -= d; }
         }
         else
         {
            if ((b + d) > 1.0) { v -= (1.0 - b); } else{ v -= d; }
         }
      }
      return(v);
   }


   // Initialize accumulator.
   inline void init(VALUE_SET& needs)
   {
      base.load(needs);
      delta.alloc(base.size());
      delta.zero();
      weight = 1.0;
      path.clear();
   }


   // Configure accumulator.
   inline void config(MotiveAccum& accum, WEIGHT weight)
   {
      init(accum.base);
      loadNeeds(accum);
      scale(weight);
      this->weight = accum.weight * weight;
      for (int i = 0, j = (int)accum.path.size(); i < j; i++)
      {
         path.push_back(accum.path[i]);
      }
   }


   // Load need delta values.
   void loadNeeds(MotiveAccum& motiveAccum)
   {
      delta.load(motiveAccum.delta);
   }


   // Accumulate need delta values.
   void accumNeeds(MotiveAccum& motiveAccum)
   {
      delta.add(motiveAccum.delta);
   }


   // Accumulate goal values.
   inline void accumGoals(GoalValue& goals)
   {
      VALUE_SET v;

      v.load(goals.values);
      v.multiply(weight);
      delta.subtract(v);
   }


   // Add a neuron to the path.
   // Return true if no loop detected.
   inline bool addPath(Neuron *neuron)
   {
      for (int i = 0, j = (int)path.size(); i < j; i++)
      {
         if (path[i] == neuron) { return(false); }
      }
      path.push_back(neuron);
      return(true);
   }


   // Scale accumulator.
   inline void scale(WEIGHT weight)
   {
      delta.multiply(weight);
   }


   // Reset accumulator.
   inline void reset()
   {
      delta.zero();
      weight = 1.0;
   }


   // Clear accumulator.
   inline void clear()
   {
      base.clear();
      delta.clear();
      weight = 1.0;
      path.clear();
   }


   // Load.
   void load(FILE *fp)
   {
      clear();
      base.load(fp);
      delta.load(fp);
      FREAD_DOUBLE(&weight, fp);
   }


   // Save.
   // When changing format increment FORMAT in mona.hpp
   void save(FILE *fp)
   {
      base.save(fp);
      delta.save(fp);
      FWRITE_DOUBLE(&weight, fp);
   }


   // Print.
   void print(FILE *out = stdout)
   {
      int i;

      fprintf(out, "<base>");
      for (i = 0; i < base.size(); i++)
      {
         fprintf(out, "<value>%f</value>", base.get(i));
      }
      fprintf(out, "</base><delta>");
      for (i = 0; i < delta.size(); i++)
      {
         fprintf(out, "<value>%f</value>", delta.get(i));
      }
      fprintf(out, "</delta><weight>%f</weight>", weight);
      fprintf(out, "<value>%f</value>", getValue());
   }
};


// Event enabling.
class Enabling
{
public:
   ENABLEMENT  value;
   MOTIVE      motive;
   TIME        age;
   int         timerIndex;
   EnablingSet *set;
   bool        newInSet;
   TIME        causeBegin;
   VALUE_SET   needs;

   // Constructor.
   Enabling(ENABLEMENT value, MOTIVE motive, TIME age,
            int timerIndex, TIME causeBegin)
   {
      clear();
      this->value      = value;
      this->motive     = motive;
      this->age        = age;
      this->timerIndex = timerIndex;
      this->causeBegin = causeBegin;
   }


   Enabling()
   {
      clear();
   }


   // Destructor.
   ~Enabling()
   {
      clear();
   }


   // Set needs.
   void setNeeds(vector<Homeostat *>& homeostats)
   {
      int n = (int)homeostats.size();

      needs.alloc(n);
      for (int i = 0; i < n; i++)
      {
         needs.set(i, homeostats[i]->getNeed());
      }
   }


   // Clone.
   inline Enabling *clone()
   {
      Enabling *enabling;

      enabling = new Enabling(value, motive, age,
                              timerIndex, causeBegin);
      assert(enabling != NULL);
      enabling->newInSet = newInSet;
      enabling->needs.load(needs);
      return(enabling);
   }


   // Clear enabling.
   inline void clear()
   {
      value      = 0.0;
      motive     = 0.0;
      age        = 0;
      timerIndex = -1;
      set        = NULL;
      newInSet   = false;
      causeBegin = 0;
      needs.clear();
   }


   // Load.
   void load(FILE *fp)
   {
      clear();
      FREAD_DOUBLE(&value, fp);
      FREAD_DOUBLE(&motive, fp);
      FREAD_LONG_LONG(&age, fp);
      FREAD_INT(&timerIndex, fp);
      set = NULL;
      FREAD_BOOL(&newInSet, fp);
      FREAD_LONG_LONG(&causeBegin, fp);
      needs.load(fp);
   }


   // Save.
   // When changing format increment FORMAT in mona.hpp
   void save(FILE *fp)
   {
      FWRITE_DOUBLE(&value, fp);
      FWRITE_DOUBLE(&motive, fp);
      FWRITE_LONG_LONG(&age, fp);
      FWRITE_INT(&timerIndex, fp);
      FWRITE_BOOL(&newInSet, fp);
      FWRITE_LONG_LONG(&causeBegin, fp);
      needs.save(fp);
   }


   // Print.
   void print(FILE *out = stdout)
   {
      fprintf(out, "<value>%f</value>", value);
      fprintf(out, "<motive>%f</motive>", value);
      fprintf(out, "<age>%llu</age>", age);
      fprintf(out, "<timerIndex>%d</timerIndex>", timerIndex);
      fprintf(out, "<newInSet>");
      if (newInSet) { fprintf(out, "true"); } else{ fprintf(out, "false"); }
      fprintf(out, "</newInSet><causeBegin>%llu</causeBegin>", causeBegin);
      fprintf(out, "<needs>");
      int n = needs.size();
      for (int i = 0; i < n; i++)
      {
         fprintf(out, "<need>%f</need>", needs.get(i));
      }
      fprintf(out, "/<needs>");
   }
};

// Set of event enablings.
class EnablingSet
{
public:
   list<Enabling *> enablings;

   // Constructor.
   EnablingSet()
   {
      clear();
   }


   // Destructor.
   ~EnablingSet()
   {
      clear();
   }


   // Get size of set.
   inline int size()
   {
      return((int)enablings.size());
   }


   // Insert an enabling.
   inline void insert(Enabling *enabling)
   {
      enabling->set      = this;
      enabling->newInSet = true;
      enablings.push_back(enabling);
   }


   // Remove an enabling.
   inline void remove(Enabling *enabling)
   {
      enablings.remove(enabling);
   }


   // Get enabling value.
   inline ENABLEMENT getValue()
   {
      Enabling   *enabling;
      ENABLEMENT e;

      list<Enabling *>::iterator listItr;

      e = 0.0;
      for (listItr = enablings.begin();
           listItr != enablings.end(); listItr++)
      {
         enabling = *listItr;
         e       += enabling->value;
      }
      return(e);
   }


   // Get value of new enablings.
   inline ENABLEMENT getNewValue()
   {
      Enabling   *enabling;
      ENABLEMENT e;

      list<Enabling *>::iterator listItr;

      e = 0.0;
      for (listItr = enablings.begin();
           listItr != enablings.end(); listItr++)
      {
         enabling = *listItr;
         if (enabling->newInSet)
         {
            e += enabling->value;
         }
      }
      return(e);
   }


   // Get value of old enablings.
   inline ENABLEMENT getOldValue()
   {
      Enabling   *enabling;
      ENABLEMENT e;

      list<Enabling *>::iterator listItr;

      e = 0.0;
      for (listItr = enablings.begin();
           listItr != enablings.end(); listItr++)
      {
         enabling = *listItr;
         if (!enabling->newInSet)
         {
            e += enabling->value;
         }
      }
      return(e);
   }


   // Clear new flags.
   inline void clearNewInSet()
   {
      Enabling *enabling;

      list<Enabling *>::iterator listItr;

      for (listItr = enablings.begin();
           listItr != enablings.end(); listItr++)
      {
         enabling           = *listItr;
         enabling->newInSet = false;
      }
   }


   // Clear.
   inline void clear()
   {
      Enabling *enabling;

      list<Enabling *>::iterator listItr;

      for (listItr = enablings.begin();
           listItr != enablings.end(); listItr++)
      {
         enabling = *listItr;
         delete enabling;
      }
      enablings.clear();
   }


   // Load.
   void load(FILE *fp)
   {
      int      size;
      Enabling *enabling;

      clear();
      FREAD_INT(&size, fp);
      for (int i = 0; i < size; i++)
      {
         enabling = new Enabling();
         assert(enabling != NULL);
         enabling->load(fp);
         enabling->set = this;
         enablings.push_back(enabling);
      }
   }


   // Save.
   // When changing format increment FORMAT in mona.hpp
   void save(FILE *fp)
   {
      int      size;
      Enabling *enabling;

      list<Enabling *>::iterator listItr;

      size = (int)enablings.size();
      FWRITE_INT(&size, fp);
      for (listItr = enablings.begin();
           listItr != enablings.end(); listItr++)
      {
         enabling = *listItr;
         enabling->save(fp);
      }
   }


   // Print.
   void print(FILE *out = stdout)
   {
      Enabling *enabling;

      list<Enabling *>::iterator listItr;

      fprintf(out, "<enablingSet>");
      for (listItr = enablings.begin();
           listItr != enablings.end(); listItr++)
      {
         enabling = *listItr;
         enabling->print(out);
      }
      fprintf(out, "</enablingSet>");
   }
};

// Mediator event notifier.
struct Notify
{
   Mediator   *mediator;
   EVENT_TYPE eventType;
};

// Mediator firing notification.
struct FiringNotify
{
   struct Notify *notify;
   WEIGHT        notifyStrength;
   TIME          causeBegin;
};

// Learning event.
class LearningEvent
{
public:
   Neuron      *neuron;
   WEIGHT      firingStrength;
   TIME        begin;
   TIME        end;
   PROBABILITY probability;
   VALUE_SET   needs;

   LearningEvent(Neuron *neuron)
   {
      this->neuron   = neuron;
      firingStrength = neuron->firingStrength;
      if (neuron->type == MEDIATOR)
      {
         begin = ((Mediator *)neuron)->causeBegin;
      }
      else
      {
         begin = neuron->mona->eventClock;
      }
      end = neuron->mona->eventClock;
      if (firingStrength > NEARLY_ZERO)
      {
         probability =
            pow(firingStrength, neuron->mona->FIRING_STRENGTH_LEARNING_DAMPER);
      }
      else
      {
         probability = 0.0;
      }
      int n = neuron->mona->numNeeds;
      needs.alloc(n);
      for (int i = 0; i < n; i++)
      {
         needs.set(i, neuron->mona->homeostats[i]->getNeed());
      }
   }


   LearningEvent()
   {
      neuron         = NULL;
      firingStrength = 0.0;
      begin          = end = 0;
      probability    = 0.0;
      needs.clear();
   }


   // Load.
   void load(FILE *fp)
   {
      neuron = (Neuron *)new ID;
      assert(neuron != NULL);
      FREAD_LONG_LONG((ID *)neuron, fp);
      FREAD_DOUBLE(&firingStrength, fp);
      FREAD_LONG_LONG(&begin, fp);
      FREAD_LONG_LONG(&end, fp);
      FREAD_DOUBLE(&probability, fp);
      needs.load(fp);
   }


   // Save.
   // When changing format increment FORMAT in mona.hpp
   void save(FILE *fp)
   {
      FWRITE_LONG_LONG(&neuron->id, fp);
      FWRITE_DOUBLE(&firingStrength, fp);
      FWRITE_LONG_LONG(&begin, fp);
      FWRITE_LONG_LONG(&end, fp);
      FWRITE_DOUBLE(&probability, fp);
      needs.save(fp);
   }


   // Print.
   void print(FILE *out = stdout)
   {
      if (neuron != NULL)
      {
         fprintf(out, "<neuron>\n");
         switch (neuron->type)
         {
#ifdef MONA_TRACKING
         case RECEPTOR:
            ((Receptor *)neuron)->print((TRACKING_FLAGS)0, out);
            fprintf(out, "\n");
            break;

         case MOTOR:
            ((Motor *)neuron)->print((TRACKING_FLAGS)0, out);
            fprintf(false, false, out, "\n");
            break;

         case MEDIATOR:
            ((Mediator *)neuron)->print((TRACKING_FLAGS)0, out);
            break;

#else
         case RECEPTOR:
            ((Receptor *)neuron)->print(out);
            fprintf(out, "\n");
            break;

         case MOTOR:
            ((Motor *)neuron)->print(out);
            fprintf(out, "\n");
            break;

         case MEDIATOR:
            ((Mediator *)neuron)->print(out);
            break;
#endif
         }
      }
      else
      {
         fprintf(out, "<neuron>NULL");
      }
      fprintf(out, "</neuron>\n");
      fprintf(out, "<firingStrength>%f</firingStrength>", firingStrength);
      fprintf(out, "<begin>%llu</begin>", begin);
      fprintf(out, "<end>%llu</end>", end);
      fprintf(out, "<probability>%f</probability>", probability);
      fprintf(out, "<needs>");
      int n = needs.size();
      for (int i = 0; i < n; i++)
      {
         fprintf(out, "<need>%f</need>", needs.get(i));
      }
      fprintf(out, "/<needs>");
   }
};

// Generalization learning event.
class GeneralizationEvent
{
public:
   Mediator   *mediator;
   ENABLEMENT enabling;
   TIME       begin;
   TIME       end;
   VALUE_SET  needs;

   GeneralizationEvent(Mediator *mediator, ENABLEMENT enabling)
   {
      this->mediator = mediator;
      this->enabling = enabling;
      begin          = mediator->causeBegin;
      end            = mediator->mona->eventClock;
      int n = mediator->mona->numNeeds;
      needs.alloc(n);
      for (int i = 0; i < n; i++)
      {
         needs.set(i, mediator->mona->homeostats[i]->getNeed());
      }
   }


   GeneralizationEvent()
   {
      mediator = NULL;
      enabling = 0.0;
      begin    = end = 0;
      needs.clear();
   }


   // Load.
   void load(FILE *fp)
   {
      mediator = (Mediator *)new ID;
      assert(mediator != NULL);
      FREAD_LONG_LONG((ID *)mediator, fp);
      FREAD_DOUBLE(&enabling, fp);
      FREAD_LONG_LONG(&begin, fp);
      FREAD_LONG_LONG(&end, fp);
      needs.load(fp);
   }


   // Save.
   // When changing format increment FORMAT in mona.hpp
   void save(FILE *fp)
   {
      FWRITE_LONG_LONG(&mediator->id, fp);
      FWRITE_DOUBLE(&enabling, fp);
      FWRITE_LONG_LONG(&begin, fp);
      FWRITE_LONG_LONG(&end, fp);
      needs.save(fp);
   }


   // Print.
   void print(FILE *out = stdout)
   {
      if (mediator != NULL)
      {
         fprintf(out, "<mediator>\n");
#ifdef MONA_TRACKING
         mediator->print((TRACKING_FLAGS)0, out);
#else
         mediator->print(out);
#endif
      }
      else
      {
         fprintf(out, "<mediator>NULL");
      }
      fprintf(out, "</mediator>\n");
      fprintf(out, "<enabling>%f</enabling>", enabling);
      fprintf(out, "<begin>%llu</begin>", begin);
      fprintf(out, "<end>%llu</end>", end);
      int n = needs.size();
      for (int i = 0; i < n; i++)
      {
         fprintf(out, "<need>%f</need>", needs.get(i));
      }
      fprintf(out, "/<needs>");
   }
};
#endif
