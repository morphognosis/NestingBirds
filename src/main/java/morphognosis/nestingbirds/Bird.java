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
   public static final int WANT_FOOD_SENSOR  = 2;
   public static final int WANT_STONE_SENSOR = 3;
   public static final int NUM_SENSORS       = 4;
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
   public void print()
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
         System.out.print("NO_OBJECT,");
         break;

      case OBJECT.EGG:
         System.out.print("EGG,");
         break;

      case OBJECT.MOUSE:
         System.out.print("MOUSE,");
         break;

      case OBJECT.STONE:
         System.out.print("STONE,");
         break;
      }
      if (sensors[Bird.WANT_FOOD_SENSOR] == 0)
      {
         System.out.print("!WANT_FOOD,");
      }
      else
      {
         System.out.print("WANT_FOOD,");
      }
      if (sensors[Bird.WANT_STONE_SENSOR] == 0)
      {
         System.out.print("!WANT_STONE]");
      }
      else
      {
         System.out.print("WANT_STONE]");
      }
      System.out.print(", Object: ");
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
      switch (response)
      {
      case RESPONSE.DO_NOTHING:
         System.out.print("DO_NOTHING");
         break;

      case RESPONSE.EAT:
         System.out.print("EAT");
         break;

      case RESPONSE.GET:
         System.out.print("GET");
         break;

      case RESPONSE.PUT:
         System.out.print("PUT");
         break;

      case RESPONSE.TOSS:
         System.out.print("TOSS");
         break;

      case RESPONSE.STEP:
         System.out.print("STEP");
         break;

      case RESPONSE.TURN:
         System.out.print("TURN");
         break;

      case RESPONSE.WANT_FOOD:
         System.out.print("WANT_FOOD");
         break;

      case RESPONSE.GIVE_FOOD:
         System.out.print("GIVE_FOOD");
         break;

      case RESPONSE.WANT_STONE:
         System.out.print("WANT_STONE");
         break;

      case RESPONSE.GIVE_STONE:
         System.out.print("GIVE_STONE");
         break;

      case RESPONSE.LAY_EGG:
         System.out.print("LAY_EGG");
         break;
      }
   }
}
