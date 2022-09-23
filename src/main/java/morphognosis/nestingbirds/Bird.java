// For conditions of distribution and use, see copyright notice in Morphognosis.java

// Nesting bird.

package morphognosis.nestingbirds;

import morphognosis.nestingbirds.Environment.LOCALE;
import morphognosis.nestingbirds.Environment.OBJECT;

public class Bird
{
   // Orientation.
   public static class ORIENTATION
   {
      public static final int NORTH = 0;
      public static final int SOUTH = 1;
      public static final int EAST  = 2;
      public static final int WEST  = 3;
   };

   // Gender.
   public static final int MALE   = 0;
   public static final int FEMALE = 1;

   // Sensors.
   public static final int LOCALE_SENSOR     = 0;
   public static final int OBJECT_SENSOR     = 1;
   public static final int NUM_SENSORS       = 2;
   public int[]            sensors;
   
   // Responses.
   public static class RESPONSE
   {
      public static final int DO_NOTHING = 0;
      public static final int EAT        = 1;
      public static final int GET        = 2;
      public static final int PUT        = 3;
      public static final int TOSS       = 4;
      public static final int STEP       = 5;
      public static final int TURN       = 6;
   };
   public int response;

   // Food duration.
   public static final int FOOD_DURATION = 60;

   // State.
   public int gender;
   public int x, y;
   public int orientation;
   public int food;
   public int hasObject;

   // Constructor.
   public Bird(int gender)
   {
      this.gender = gender;
      x           = y = 0;
      orientation = ORIENTATION.NORTH;
      food        = 0;
      hasObject   = OBJECT.NO_OBJECT;
      sensors     = new int[NUM_SENSORS];
      response = RESPONSE.DO_NOTHING;
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
	  String s = Environment.localeToString(sensors[Bird.LOCALE_SENSOR]);
	  s += ",";
	  s += Environment.objectToString(sensors[Bird.OBJECT_SENSOR]);
      return s;
   }
   
   // Print state.
   public void printState()
   {
      System.out.print("Orientation: ");
      System.out.print(orientationToString(orientation));
      System.out.print(", Food: " + food);
      System.out.print(", Has_object: ");
      System.out.print(Environment.objectToString(hasObject));
   }
   
   // Orientation to string.
   public static String orientationToString(int orientation)
   {
      switch (orientation)
      {
      case ORIENTATION.NORTH:
         return "NORTH";

      case ORIENTATION.SOUTH:
          return "SOUTH";

      case ORIENTATION.EAST:
          return "EAST";
          
      case ORIENTATION.WEST:
          return "WEST";
      }
      return "Unknown orientation";
   }
         
   // Print response.
   public void printResponse(int response)
   {
	   System.out.print(responseToString(response));
   }
   
   // Response to string.
   public static String responseToString(int response)
   {
      switch (response)
      {
      case RESPONSE.DO_NOTHING:
         return("DO_NOTHING");

      case RESPONSE.EAT:
    	  return("EAT");

      case RESPONSE.GET:
    	  return("GET");

      case RESPONSE.PUT:
    	  return("PUT");

      case RESPONSE.TOSS:
    	  return("TOSS");

      case RESPONSE.STEP:
    	  return("STEP");

      case RESPONSE.TURN:
    	  return("TURN");
      }
      return "Unknown response";
   }
}
