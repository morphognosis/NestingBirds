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
      public static final int WANT_FOOD  = 7;
      public static final int GIVE_FOOD  = 8;
      public static final int WANT_STONE = 9;
      public static final int GIVE_STONE = 10;
      public static final int LAY_EGG    = 11;
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
   public void print(int gender)
   {
      System.out.print("Sensors: [");
      switch (sensors[Bird.LOCALE_SENSOR])
      {
      case LOCALE.DESERT:
         System.out.print("DESERT,");
         break;

      case LOCALE.FOREST:
         System.out.print("FOREST,");
         break;

      case LOCALE.GRASSLAND:
         System.out.print("GRASSLAND,");
         break;
      }
      switch (sensors[Bird.OBJECT_SENSOR])
      {
      case OBJECT.NO_OBJECT:
         System.out.print("NO_OBJECT");
         break;

      case OBJECT.EGG:
         System.out.print("EGG");
         break;

      case OBJECT.MOUSE:
         System.out.print("MOUSE");
         break;

      case OBJECT.STONE:
         System.out.print("STONE");
         break;
      }
      if (gender == MALE)
      {
	      if (sensors[MaleBird.WANT_FOOD_SENSOR] == 0)
	      {
	         System.out.print(",!WANT_FOOD");
	      }
	      else
	      {
	         System.out.print(",WANT_FOOD");
	      }
	      if (sensors[MaleBird.WANT_STONE_SENSOR] == 0)
	      {
	         System.out.print(",!WANT_STONE]");
	      }
	      else
	      {
	         System.out.print(",WANT_STONE]");
	      }
      } else {
    	  System.out.print("]");
      }
      System.out.print(", Orientation: ");
      switch (orientation)
      {
      case ORIENTATION.NORTH:
    	 System.out.print("NORTH, ");
         break;

      case ORIENTATION.SOUTH:
     	 System.out.print("SOUTH, ");
         break;

      case ORIENTATION.EAST:
     	 System.out.print("EAST, ");
         break;

      case ORIENTATION.WEST:
     	 System.out.print("WEST, ");
         break;
      }
      System.out.print("Food: " + food + ", ");      
      System.out.print(", Has_object: ");
      switch (hasObject)
      {
      case OBJECT.EGG:
         System.out.print("HAS_EGG, ");
         break;

      case OBJECT.MOUSE:
         System.out.print("HAS_MOUSE, ");
         break;

      case OBJECT.STONE:
         System.out.print("HAS_STONE, ");
         break;
      }
      System.out.print("Food: " + food + ", ");
      System.out.print("Response: ");
      printResponse(response);
      System.out.println();
   }

   // Print response.
   public void printResponse(int response)
   {
	   System.out.print(getResponseName(response));
   }
   
   public String getResponseName(int response)
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

      case RESPONSE.WANT_FOOD:
    	  return("WANT_FOOD");

      case RESPONSE.GIVE_FOOD:
    	  return("GIVE_FOOD");

      case RESPONSE.WANT_STONE:
    	  return("WANT_STONE");

      case RESPONSE.GIVE_STONE:
    	  return("GIVE_STONE");

      case RESPONSE.LAY_EGG:
    	  return("LAY_EGG");
      }
      return "UNKNOWN";
   }
}
