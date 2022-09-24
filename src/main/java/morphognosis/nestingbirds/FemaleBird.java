// For conditions of distribution and use, see copyright notice in Morphognosis.java

// Nesting female bird.

package morphognosis.nestingbirds;

public class FemaleBird extends Bird
{
   // Sensors.
   public static final int NUM_SENSORS = 2;

   // Responses.
   public static class RESPONSE
   {
      public static final int WANT_FOOD  = 8;
      public static final int WANT_STONE = 10;
      public static final int LAY_EGG    = 12;

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

   // Constructor.
   public FemaleBird()
   {
      super(FEMALE);
      sensors = new int[NUM_SENSORS];
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
