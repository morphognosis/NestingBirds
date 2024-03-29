// For conditions of distribution and use, see copyright notice in LICENSE.txt

// Nesting female bird.

package morphognosis.nestingbirds;

public class FemaleBird extends Bird
{
   // Sensors.
   public static final int NUM_SENSORS = Bird.NUM_SENSORS;

   // Responses.
   public static class RESPONSE
   {
      public static final int WANT_FOOD            = 8;
      public static final int WANT_STONE           = 9;
      public static final int LAY_EGG              = 10;
      public static final int NUM_FEMALE_RESPONSES = 3;

      // Response to string.
      public static String toString(int response)
      {
         switch (response)
         {
         case RESPONSE.WANT_FOOD:
            return("WANT_FOOD");

         case RESPONSE.WANT_STONE:
            return("WANT_STONE");

         case RESPONSE.LAY_EGG:
            return("LAY_EGG");
         }
         return(Bird.RESPONSE.toString(response));
      }
   };

   // Food.
   public static int     FOOD_DURATION        = 200;
   public static int     INITIAL_FOOD         = 200;
   public static boolean RANDOMIZE_FOOD_LEVEL = false;

   // Constructor.
   public FemaleBird()
   {
      super(FEMALE);
   }


   // Set sensors.
   public void setSensors(int[] sensors)
   {
      this.sensors = sensors;
   }


   // Print bird.
   public void print()
   {
      System.out.print("Sensors: [");
      printSensors();
      System.out.print("], ");
      printState();
      System.out.print(", Response: ");
      printResponse(response);
      System.out.println();
   }


   // Print response.
   public void printResponse(int response)
   {
      System.out.print(RESPONSE.toString(response));
   }
}
