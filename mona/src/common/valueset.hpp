// Value set definitions

// Store and manage a set of floating point values.

#ifndef __VALUESET__
#define __VALUESET__

#include "fileio.h"

class ValueSet
{
public:

   // Constructors.
   ValueSet() {}

   ValueSet(int size)
   {
      values.resize(size);
   }


   // Destructor.
   ~ValueSet()
   {
      clear();
   }


   // Clear
   inline void clear()
   {
      values.clear();
   }


   // Get size.
   inline int size()
   {
      return((int)values.size());
   }


   // Allocate
   inline void alloc(int size)
   {
      clear();
      values.resize(size);
   }


   // Zero
   inline void zero()
   {
      for (int i = 0; i < (int)values.size(); i++) { values[i] = 0.0; }
   }


   // Get a specified value.
   inline double get(int index)
   {
      assert(index >= 0 && index < (int)values.size());
      return(values[index]);
   }


   // Get sum of values.
   inline double sum()
   {
      double d;

      d = 0.0;
      for (int i = 0; i < (int)values.size(); i++) { d += values[i]; }
      return(d);
   }


   // Set a scalar value.
   inline void set(int index, double value)
   {
      assert(index >= 0 && index < (int)values.size());
      values[index] = value;
   }


   // Add a scalar value.
   inline void add(int index, double value)
   {
      assert(index >= 0 && index < (int)values.size());
      values[index] += value;
   }


   // Subtract a scalar value.
   inline void subtract(int index, double value)
   {
      assert(index >= 0 && index < (int)values.size());
      values[index] -= value;
   }


   // Multiply by a scalar value.
   inline void multiply(int index, double value)
   {
      assert(index >= 0 && index < (int)values.size());
      values[index] *= value;
   }


   // Divide by a scalar value.
   inline void divide(int index, double value)
   {
      assert(index >= 0 && index < (int)values.size());
      assert(value != 0.0);
      values[index] /= value;
   }


   // Add a scalar value to all.
   inline void add(double value)
   {
      for (int i = 0; i < (int)values.size(); i++)
      {
         values[i] += value;
      }
   }


   // Subtract a scalar value from all.
   inline void subtract(double value)
   {
      for (int i = 0; i < (int)values.size(); i++)
      {
         values[i] -= value;
      }
   }


   // Multiply all by a scalar value.
   inline void multiply(double value)
   {
      for (int i = 0; i < (int)values.size(); i++)
      {
         values[i] *= value;
      }
   }


   // Divide all by a scalar value.
   inline void divide(double value)
   {
      assert(value != 0.0);
      for (int i = 0; i < (int)values.size(); i++)
      {
         values[i] /= value;
      }
   }


   // Load given set of values into this.
   inline void load(class ValueSet& loadSet)
   {
      if (loadSet.size() != (int)values.size())
      {
         values.resize(loadSet.size());
      }

      for (int i = 0; i < (int)values.size(); i++)
      {
         values[i] = loadSet.values[i];
      }
   }


   // Vector addition.
   inline void add(class ValueSet& addSet)
   {
      assert(size() == addSet.size());
      for (int i = 0; i < (int)values.size(); i++)
      {
         values[i] += addSet.values[i];
      }
   }


   // Vector subtraction.
   inline void subtract(class ValueSet& subSet)
   {
      assert(size() == subSet.size());
      for (int i = 0; i < (int)values.size(); i++)
      {
         values[i] -= subSet.values[i];
      }
   }


   // Load.
   inline void load(FILE *fp)
   {
      int i, j;

      values.clear();
      FREAD_INT(&i, fp);
      values.resize(i);
      for (i = 0, j = (int)values.size(); i < j; i++)
      {
         FREAD_DOUBLE(&values[i], fp);
      }
   }


   // Save.
   inline void save(FILE *fp)
   {
      int i = (int)values.size();

      FWRITE_INT(&i, fp);
      int n = (int)values.size();
      for (i = 0; i < n; i++)
      {
         FWRITE_DOUBLE(&values[i], fp);
      }
   }


   // Print.
   inline void print(FILE *out = stdout)
   {
      for (int i = 0, j = (int)values.size(); i < j; i++)
      {
         fprintf(out, "%f ", values[i]);
      }
      fprintf(out, "\n");
   }


   vector<double> values;                         // Value set
};

typedef class ValueSet   VALUE_SET;
#endif
