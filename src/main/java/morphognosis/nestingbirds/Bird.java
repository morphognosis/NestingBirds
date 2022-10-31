// For conditions of distribution and use, see copyright notice in Morphognosis.java

// Nesting bird.

package morphognosis.nestingbirds;

import java.io.PrintWriter;
import java.util.Random;

import morphognosis.nestingbirds.NestingBirds.LOCALE;
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
   public static final int NUM_CELL_SENSORS      = 4;
   public static final int CURRENT_LOCALE_SENSOR = 0;
   public static final int CURRENT_OBJECT_SENSOR = 1;
   public static final int LEFT_LOCALE_SENSOR    = 2;
   public static final int LEFT_OBJECT_SENSOR    = 3;
   public static final int FORWARD_LOCALE_SENSOR = 4;
   public static final int FORWARD_OBJECT_SENSOR = 5;
   public static final int RIGHT_LOCALE_SENSOR   = 6;
   public static final int RIGHT_OBJECT_SENSOR   = 7;

   public static final int MATE_PROXIMITY_SENSOR  = NUM_CELL_SENSORS * CELL_SENSOR.NUM_SENSORS;
   public static final int MATE_PROXIMITY_UNKNOWN = -1;
   public static final int MATE_PROXIMITY_PRESENT = 0;
   public static final int MATE_PROXIMITY_LEFT    = 1;
   public static final int MATE_PROXIMITY_FORWARD = 2;
   public static final int MATE_PROXIMITY_RIGHT   = 3;

   public static final int NUM_SENSORS = MATE_PROXIMITY_SENSOR + 1;
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
      if (food > 0) { food--; } else{ food = 0; }
   }


   // Write dataset header.
   public static void writeDatasetHeader(PrintWriter writer, int gender)
   {
      writer.print("current_desert,current_forest,current_plain");
      writer.print(",");
      writer.print("current_no_object,current_egg,current_mouse,current_stone");
      writer.print(",");
      writer.print("left_desert,left_forest,left_plain");
      writer.print(",");
      writer.print("left_no_object,left_egg,left_mouse,left_stone");
      writer.print(",");
      writer.print("forward_desert,forward_forest,forward_plain");
      writer.print(",");
      writer.print("forward_no_object,forward_egg,forward_mouse,forward_stone");
      writer.print(",");
      writer.print("right_desert,right_forest,right_plain");
      writer.print(",");
      writer.print("right_no_object,right_egg,right_mouse,right_stone");
      writer.print(",");
      writer.print("mate_proximity_unknown,mate_proximity_present,mate_proximity_left,mate_proximity_forward,mate_proximity_right");
      writer.print(",");
      if (gender == MALE)
      {
         writer.print("female_want_food,female_want_stone");
         writer.print(",");
      }
      writer.print("orientation_north,orientation_south,orientation_east,orientation_west");
      writer.print(",");
      writer.print("food");
      writer.print(",");
      writer.print("has_object_no_object,has_object_mouse,has_object_stone");
      writer.print(",");
      writer.print("do_nothing,eat,get,put,toss,move,turn_right,turn_left");
      writer.print(",");
      if (gender == MALE)
      {
         writer.println("give_food,give_stone");
      }
      else
      {
         writer.println("want_food,want_stone,lay_egg");
      }
      writer.flush();
   }


   // Write dataset.
   public void writeDataset(PrintWriter writer)
   {
      // Write locale and object sensors.
      for (int i = 0, j = NUM_CELL_SENSORS * CELL_SENSOR.NUM_SENSORS; i < j; i++)
      {
         if ((i % 2) == 0)
         {
            // Locale.
            switch (sensors[i])
            {
            case LOCALE.DESERT:
               writer.print("1,0,0");
               break;

            case LOCALE.FOREST:
               writer.print("0,1,0");
               break;

            case LOCALE.PLAIN:
               writer.print("0,0,1");
               break;
            }
         }
         else
         {
            // Object.
            switch (sensors[i])
            {
            case OBJECT.NO_OBJECT:
               writer.print("1,0,0,0");
               break;

            case OBJECT.EGG:
               writer.print("0,1,0,0");
               break;

            case OBJECT.MOUSE:
               writer.print("0,0,1,0");
               break;

            case OBJECT.STONE:
               writer.print("0,0,0,1");
               break;
            }
         }
         writer.print(",");
      }

      // Write mate proximity sensor.
      switch (sensors[MATE_PROXIMITY_SENSOR])
      {
      case Bird.MATE_PROXIMITY_UNKNOWN:
         writer.print("1,0,0,0,0");
         break;

      case Bird.MATE_PROXIMITY_PRESENT:
         writer.print("0,1,0,0,0");
         break;

      case Bird.MATE_PROXIMITY_LEFT:
         writer.print("0,0,1,0,0");
         break;

      case Bird.MATE_PROXIMITY_FORWARD:
         writer.print("0,0,0,1,0");
         break;

      case Bird.MATE_PROXIMITY_RIGHT:
         writer.print("0,0,0,0,1");
         break;
      }
      writer.print(",");
      if (gender == Bird.MALE)
      {
         writer.print(sensors[MaleBird.WANT_FOOD_SENSOR] + "");
         writer.print(",");
         writer.print(sensors[MaleBird.WANT_STONE_SENSOR] + "");
         writer.print(",");
      }

      // Write properties.
      switch (orientation)
      {
      case ORIENTATION.NORTH:
         writer.print("1,0,0,0");
         break;

      case ORIENTATION.SOUTH:
         writer.print("0,1,0,0");
         break;

      case ORIENTATION.EAST:
         writer.print("0,0,1,0");
         break;

      case ORIENTATION.WEST:
         writer.print("0,0,0,1");
         break;
      }
      writer.print(",");
      if (food > 0)
      {
         writer.print("1");
      }
      else
      {
         writer.print("0");
      }
      writer.print(",");
      switch (hasObject)
      {
      case OBJECT.NO_OBJECT:
         writer.print("1,0,0");
         break;

      case OBJECT.MOUSE:
         writer.print("0,1,0");
         break;

      case OBJECT.STONE:
         writer.print("0,0,1");
         break;
      }
      writer.print(",");

      // Write response.
      if (response < Bird.RESPONSE.NUM_RESPONSES)
      {
         switch (response)
         {
         case Bird.RESPONSE.DO_NOTHING:
            if (gender == Bird.MALE)
            {
               writer.println("1,0,0,0,0,0,0,0,0,0");
            }
            else
            {
               writer.println("1,0,0,0,0,0,0,0,0,0,0");
            }
            break;

         case Bird.RESPONSE.EAT:
            if (gender == Bird.MALE)
            {
               writer.println("0,1,0,0,0,0,0,0,0,0");
            }
            else
            {
               writer.println("0,1,0,0,0,0,0,0,0,0,0");
            }
            break;

         case Bird.RESPONSE.GET:
            if (gender == Bird.MALE)
            {
               writer.println("0,0,1,0,0,0,0,0,0,0");
            }
            else
            {
               writer.println("0,0,1,0,0,0,0,0,0,0,0");
            }
            break;

         case Bird.RESPONSE.PUT:
            if (gender == Bird.MALE)
            {
               writer.println("0,0,0,1,0,0,0,0,0,0");
            }
            else
            {
               writer.println("0,0,0,1,0,0,0,0,0,0,0");
            }
            break;

         case Bird.RESPONSE.TOSS:
            if (gender == Bird.MALE)
            {
               writer.println("0,0,0,0,1,0,0,0,0,0");
            }
            else
            {
               writer.println("0,0,0,0,1,0,0,0,0,0,0");
            }
            break;

         case Bird.RESPONSE.MOVE:
            if (gender == Bird.MALE)
            {
               writer.println("0,0,0,0,0,1,0,0,0,0");
            }
            else
            {
               writer.println("0,0,0,0,0,1,0,0,0,0,0");
            }
            break;

         case Bird.RESPONSE.TURN_RIGHT:
            if (gender == Bird.MALE)
            {
               writer.println("0,0,0,0,0,0,1,0,0,0");
            }
            else
            {
               writer.println("0,0,0,0,0,0,1,0,0,0,0");
            }
            break;

         case Bird.RESPONSE.TURN_LEFT:
            if (gender == Bird.MALE)
            {
               writer.println("0,0,0,0,0,0,0,1,0,0");
            }
            else
            {
               writer.println("0,0,0,0,0,0,0,1,0,0,0");
            }
            break;
         }
      }
      else
      {
         // Gender-specific response.
         if (gender == Bird.MALE)
         {
            switch (response)
            {
            case MaleBird.RESPONSE.GIVE_FOOD:
               writer.println("0,0,0,0,0,0,0,0,1,0");
               break;

            case MaleBird.RESPONSE.GIVE_STONE:
               writer.println("0,0,0,0,0,0,0,0,0,1");
               break;
            }
         }
         else
         {
            // Female response.
            switch (response)
            {
            case FemaleBird.RESPONSE.WANT_FOOD:
               writer.println("0,0,0,0,0,0,0,0,1,0,0");
               break;

            case FemaleBird.RESPONSE.WANT_STONE:
               writer.println("0,0,0,0,0,0,0,0,0,1,0");
               break;

            case FemaleBird.RESPONSE.LAY_EGG:
               writer.println("0,0,0,0,0,0,0,0,0,0,1");
               break;
            }
         }
      }
      writer.flush();
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
      s += "], Mate proximity: ";
      switch (sensors[MATE_PROXIMITY_SENSOR])
      {
      case MATE_PROXIMITY_UNKNOWN:
         s += "UNKNOWN";
         break;

      case MATE_PROXIMITY_PRESENT:
         s += "PRESENT";
         break;

      case MATE_PROXIMITY_LEFT:
         s += "LEFT";
         break;

      case MATE_PROXIMITY_FORWARD:
         s += "FORWARD";
         break;

      case MATE_PROXIMITY_RIGHT:
         s += "RIGHT";
         break;
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
