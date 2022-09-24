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
      System.out.print("Sensors: [");
      printSensors();
      System.out.print("], ");
      printState();
      System.out.print(", Response: ");
      printResponse(response);
      System.out.println();	   
   }
   
   // Print sensors.
   public void printSensors()
   {
      System.out.print(sensorsToString());
   }
   
   // Sensors to string.
   public String sensorsToString()
   {
	  String s = super.sensorsToString();
	  s += ",";
      if (sensors[WANT_FOOD_SENSOR] == 0)
      {
         s += "!WANT_FOOD";
      }
      else
      {
    	  s += "WANT_FOOD";
      }
      s += ",";
      if (sensors[WANT_STONE_SENSOR] == 0)
      {
    	  s += "!WANT_STONE";
      }
      else
      {
    	  s += "WANT_STONE";
      }
      return s;
   }
      
   // Print response.
   public void printResponse(int response)
   {
	   System.out.print(responseToString(response));
   } 
   
   // Get response name.
   public static String responseToString(int response)
   {
      switch (response)
      {
      case RESPONSE.GIVE_FOOD:
    	  return("GIVE_FOOD");

      case RESPONSE.GIVE_STONE:
    	  return("GIVE_STONE");
      }
      return Bird.responseToString(response);
   }   
}
