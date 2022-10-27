// For conditions of distribution and use, see copyright notice in Morphognosis.java

// Nesting bird.

package morphognosis.nestingbirds;

import morphognosis.nestingbirds.NestingBirds.OBJECT;

public class Bird
{
   // Orientation.
   public static class ORIENTATION
   {
      public static final int NORTH = 0;
      public static final int SOUTH = 1;
      public static final int EAST  = 2;
      public static final int WEST  = 3;

      // Orientation to string.
      public static String toString(int orientation)
      {
         switch (orientation)
         {
         case ORIENTATION.NORTH:
            return("NORTH");

         case ORIENTATION.SOUTH:
            return("SOUTH");

         case ORIENTATION.EAST:
            return("EAST");

         case ORIENTATION.WEST:
            return("WEST");
         }
         return("Unknown orientation");
      }
   };

   // Gender.
   public static final int MALE   = 0;
   public static final int FEMALE = 1;

   // Sensors.
   public class CELL_SENSOR
   {
      public static final int NUM_SENSORS = 2;
      public int              locale;
      public int              object;
   };

   // Bird senses current, left, forward, and right cells.
   public static final int NUM_CELL_SENSORS = 4;

   public static final int MATE_PRESENT_SENSOR = NUM_CELL_SENSORS * CELL_SENSOR.NUM_SENSORS;
   public static final int NUM_SENSORS         = MATE_PRESENT_SENSOR + 1;
   public int[]            sensors;

   // Responses.
   public static class RESPONSE
   {
      public static final int DO_NOTHING    = 0;
      public static final int EAT           = 1;
      public static final int GET           = 2;
      public static final int PUT           = 3;
      public static final int TOSS          = 4;
      public static final int MOVE          = 5;
      public static final int TURN_RIGHT    = 6;
      public static final int TURN_LEFT     = 7;
      public static final int NUM_RESPONSES = 8;

      // Response to string.
      public static String toString(int response)
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

         case RESPONSE.MOVE:
            return("MOVE");

         case RESPONSE.TURN_RIGHT:
            return("TURN_RIGHT");

         case RESPONSE.TURN_LEFT:
            return("TURN_LEFT");
         }
         return(null);
      }
   };
   public int response;

   // State.
   public int gender;
   public int x, y;
   public int orientation;
   public int food;
   public int hasObject;

   // Brain.
   public BirdBrain brain;

   // Constructor.
   public Bird(int gender)
   {
      this.gender = gender;
      x           = y = 0;
      orientation = ORIENTATION.NORTH;
      food        = 0;
      hasObject   = OBJECT.NO_OBJECT;
      response    = RESPONSE.DO_NOTHING;
      if (gender == MALE)
      {
         brain = new BirdBrain(MaleBird.NUM_SENSORS, Bird.RESPONSE.NUM_RESPONSES +
                               MaleBird.RESPONSE.NUM_MALE_RESPONSES + FemaleBird.RESPONSE.NUM_FEMALE_RESPONSES);
      }
      else
      {
         brain = new BirdBrain(FemaleBird.NUM_SENSORS, Bird.RESPONSE.NUM_RESPONSES +
                               MaleBird.RESPONSE.NUM_MALE_RESPONSES + FemaleBird.RESPONSE.NUM_FEMALE_RESPONSES);
      }
   }


   // Cycle bird.
   public int cycle()
   {
      float[] brainSensors = new float[sensors.length + 2];
      for (int i = 0; i < sensors.length; i++)
      {
         brainSensors[i] = (float)sensors[i];
      }
      brainSensors[sensors.length]     = (float)hasObject;
      brainSensors[sensors.length + 1] = (float)food;
      return(brain.cycle(brainSensors));
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
      String s = "[Cell sensors: ";

      for (int i = 0; i < NUM_CELL_SENSORS; i++)
      {
         s += "[";
         switch (i)
         {
         case 0:
            s += "Current: ";
            break;

         case 1:
            s += "Left: ";
            break;

         case 2:
            s += "Forward: ";
            break;

         case 3:
            s += "Right: ";
            break;
         }
         s += NestingBirds.LOCALE.toString(sensors[i * CELL_SENSOR.NUM_SENSORS]);
         s += ",";
         s += NestingBirds.OBJECT.toString(sensors[(i * CELL_SENSOR.NUM_SENSORS) + 1]);
         s += "]";
         if (i < NUM_CELL_SENSORS - 1)
         {
            s += ",";
         }
      }
      s += "], Mate present: ";
      if (sensors[Bird.MATE_PRESENT_SENSOR] == 1)
      {
         s += "true";
      }
      else
      {
         s += "false";
      }
      return(s);
   }


   // Print state.
   public void printState()
   {
      System.out.print("Orientation: ");
      System.out.print(ORIENTATION.toString(orientation));
      System.out.print(", Food: " + food);
      System.out.print(", Has_object: ");
      System.out.print(NestingBirds.OBJECT.toString(hasObject));
   }


   // Print response.
   public void printResponse(int response)
   {
      System.out.print(RESPONSE.toString(response));
   }
}
