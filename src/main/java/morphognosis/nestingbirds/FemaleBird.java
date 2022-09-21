// For conditions of distribution and use, see copyright notice in Morphognosis.java

// Nesting female bird.

package morphognosis.nestingbirds;

import morphognosis.nestingbirds.Environment.LOCALE;
import morphognosis.nestingbirds.Environment.OBJECT;

public class FemaleBird extends Bird
{
   // Sensors.
   public static final int NUM_SENSORS       = 2;
   
   // Responses.
   public static class RESPONSE
   {
      public static final int WANT_FOOD  = 7;
      public static final int WANT_STONE = 9;
      public static final int LAY_EGG    = 11;
   };

   // Constructor.
   public FemaleBird()
   {
		  super(FEMALE);
	      sensors     = new int[NUM_SENSORS];
   }

   // Print bird.
   public void print()
   {
	  print(FEMALE);
   }
}
