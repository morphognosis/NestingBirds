// For conditions of distribution and use, see copyright notice in Morphognosis.java

package morphognosis;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;

// Metamorph.
public class Metamorph
{
   // Morphognostic.
   public Morphognostic morphognostic;

   // Response.
   public int    response;
   public String responseName;

   // Are there multiple responses associated with morphognostic?
   public boolean ambiguous;

   // Cause and effect metamorphs.
   public ArrayList<Integer> causeIndexes;
   public ArrayList<Integer> effectIndexes;

   // Goal value.
   public float goalValue;

   // Constructors.
   public Metamorph(Morphognostic morphognostic, int response, float goalValue)
   {
      this.morphognostic = morphognostic;
      this.response      = response;
      responseName       = "";
      ambiguous          = false;
      causeIndexes       = new ArrayList<Integer>();
      effectIndexes      = new ArrayList<Integer>();
      this.goalValue     = goalValue;
   }


   public Metamorph(Morphognostic morphognostic, int response,
                    float goalValue, String responseName)
   {
      this.morphognostic = morphognostic;
      this.response      = response;
      this.responseName  = responseName;
      ambiguous          = false;
      causeIndexes       = new ArrayList<Integer>();
      effectIndexes      = new ArrayList<Integer>();
      this.goalValue     = goalValue;
   }


   // Equality test.
   public boolean equals(Metamorph m)
   {
      if (response != m.response)
      {
         return(false);
      }
      if (morphognostic.compare(m.morphognostic) != 0.0f)
      {
         return(false);
      }
      return(true);
   }


   // Save.
   public void save(DataOutputStream output) throws IOException
   {
      morphognostic.save(output);
      Utility.saveInt(output, response);
      Utility.saveFloat(output, goalValue);
      Utility.saveString(output, responseName);
      if (ambiguous)
      {
         Utility.saveInt(output, 1);
      }
      else
      {
         Utility.saveInt(output, 0);
      }
      int n = causeIndexes.size();
      Utility.saveInt(output, n);
      for (int i : causeIndexes)
      {
         Utility.saveInt(output, i);
      }
      n = effectIndexes.size();
      Utility.saveInt(output, n);
      for (int i : effectIndexes)
      {
         Utility.saveInt(output, i);
      }
      output.flush();
   }


   // Load.
   public static Metamorph load(DataInputStream input) throws IOException
   {
      Morphognostic morphognostic = Morphognostic.load(input);
      int           response      = Utility.loadInt(input);
      float         goalValue     = Utility.loadFloat(input);
      String        responseName  = Utility.loadString(input);
      boolean       ambiguous     = false;

      if (Utility.loadInt(input) == 1)
      {
         ambiguous = true;
      }
      Metamorph metamorph = new Metamorph(morphognostic, response, goalValue, responseName);
      metamorph.ambiguous = ambiguous;
      int n = Utility.loadInt(input);

      for (int i = 0; i < n; i++)
      {
         metamorph.causeIndexes.add(Utility.loadInt(input));
      }
      n = Utility.loadInt(input);
      for (int i = 0; i < n; i++)
      {
         metamorph.effectIndexes.add(Utility.loadInt(input));
      }
      return(metamorph);
   }


   // Print.
   public void print()
   {
      System.out.println("Morphognostic:");
      morphognostic.print();
      System.out.println("Response=" + response);
      System.out.println("ResponseName=" + responseName);
      if (ambiguous)
      {
         System.out.println("Ambiguous=true");
      }
      else
      {
         System.out.println("Ambiguous=false");
      }
      System.out.print("Cause indexes:");
      for (Integer i : causeIndexes)
      {
         System.out.print(" " + i);
      }
      System.out.println();
      System.out.print("Effect indexes:");
      for (Integer i : effectIndexes)
      {
         System.out.print(" " + i);
      }
      System.out.println();
      System.out.println("Goal value=" + goalValue);
   }
}
