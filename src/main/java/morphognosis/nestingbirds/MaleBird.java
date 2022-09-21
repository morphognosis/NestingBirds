// For conditions of distribution and use, see copyright notice in Morphognosis.java

// Nesting male bird.

package morphognosis.nestingbirds;

public class MaleBird extends Bird
{
   // Sensors.
   public static final int WANT_FOOD_SENSOR  = 2;
   public static final int WANT_STONE_SENSOR = 3;
   public static final int NUM_SENSORS       = 4;
   
   // Responses.
   public static class RESPONSE
   {
      public static final int GIVE_FOOD  = 8;
      public static final int GIVE_STONE = 10;
   };

   // Constructor.
   public MaleBird()
   {
	  super(MALE);
      sensors     = new int[NUM_SENSORS];
   }


   // Digest food.
   public void digest()
   {
      if (food > 0) { food--; }
   }


   // Print bird.
   public void print()
   {
	  print(MALE);
   }
}
