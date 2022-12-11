// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds.

package morphognosis.nestingbirds;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Random;

import morphognosis.nestingbirds.Bird.RESPONSE;

public class NestingBirds
{
   // Version.
   public static final String VERSION = "2.0";

   // Dataset file names.
   public static String MALE_DATASET_FILE_NAME   = "male_dataset";
   public static String FEMALE_DATASET_FILE_NAME = "female_dataset";

   // Random numbers.
   public static int    RANDOM_NUMBER_SEED = 4517;
   public static Random Randomizer         = null;

   // Usage.
   public static final String Usage =
      "Usage:\n" +
      "    java morphognosis.nestingbirds.NestingBirds\n" +
      "      -steps <steps>\n" +
      "      [-runs <runs> (default=1)]\n" +
      "      [-responseDriver <autopilot | bird> (default=autopilot)]\n" +
      "      [-maleInitialFood <amount> (default=" + MaleBird.INITIAL_FOOD + ")]\n" +
      "      [-femaleInitialFood <amount> (default=" + FemaleBird.INITIAL_FOOD + ")]\n" +
      "      [-maleFoodDuration <amount> (default=" + MaleBird.FOOD_DURATION + ")]\n" +
      "      [-femaleFoodDuration <amount> (default=" + FemaleBird.FOOD_DURATION + ")]\n" +
      "      [-randomizeMaleFoodLevel (food level probabilistically increases 0-" + MaleBird.FOOD_DURATION + " upon eating food)]\n" +
      "      [-randomizeFemaleFoodLevel (food level probabilistically increases 0-" + FemaleBird.FOOD_DURATION + " upon eating food)]\n" +
      "      [-writeMaleDataset (write dataset file: " + MALE_DATASET_FILE_NAME + "_<run>.csv)]\n" +
      "      [-writeFemaleDataset (write dataset file: " + FEMALE_DATASET_FILE_NAME + "_<run>.csv)]\n" +
      "      [-verbose <true | false> (default=true)]\n" +
      "      [-randomSeed <seed> (default=" + RANDOM_NUMBER_SEED + ")]\n" +
      "      [-version]\n" +
      "Exit codes:\n" +
      "  0=success\n" +
      "  1=error";

   // Locale map: plain=0, forest=1, food=2, desert=3, stone=4
   public static final int  width      = 21, height = 21;
   public static final char locale[][] =
   {
      { 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 2, 1, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 1, 1, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 2, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 2, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 3, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 3, 3, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 3, 4, 4, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 3, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 3, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 4, 4, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 3, 3, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 3, 0, 0, 0 }
   };

   // Locale types.
   public static class LOCALE
   {
      public static final int DESERT = 0;
      public static final int FOREST = 1;
      public static final int PLAIN  = 2;

      // Locale to string.
      public static String toString(int locale)
      {
         switch (locale)
         {
         case LOCALE.DESERT:
            return("DESERT");

         case LOCALE.FOREST:
            return("FOREST");

         case LOCALE.PLAIN:
            return("PLAIN");
         }
         return("Unknown locale");
      }
   };

   // Object types.
   public static class OBJECT
   {
      public static final int NO_OBJECT = 0;
      public static final int EGG       = 1;
      public static final int MOUSE     = 2;
      public static final int STONE     = 3;

      // Object to string.
      public static String toString(int object)
      {
         switch (object)
         {
         case OBJECT.NO_OBJECT:
            return("NO_OBJECT");

         case OBJECT.EGG:
            return("EGG");

         case OBJECT.MOUSE:
            return("MOUSE");

         case OBJECT.STONE:
            return("STONE");
         }
         return("Unknown object");
      }
   };

   // Probability of mouse movement.
   public static double MOUSE_MOVE_PROBABILITY = 0.1;

   // Cell.
   public class Cell
   {
      public int locale;
      public int object;
      public Cell()
      {
         locale = LOCALE.DESERT;
         object = OBJECT.NO_OBJECT;
      }
   };

   // World.
   public Cell[][] world;

   // Birds.
   public MaleBird   male;
   public FemaleBird female;

   // Response drivers.
   public static class RESPONSE_DRIVER
   {
      public static final int AUTOPILOT = 0;
      public static final int MANUAL    = 1;
      public static final int BIRD      = 2;


      // To string.
      public static String toString(int driver)
      {
         switch (driver)
         {
         case AUTOPILOT:
            return("autopilot");

         case MANUAL:
            return("manual");

         case BIRD:
            return("bird");
         }
         return("Unknown driver");
      }
   };
   public static int ResponseDriver = RESPONSE_DRIVER.AUTOPILOT;
   public void setResponseDriver(int driver) { ResponseDriver = driver; }

   // Autopilot controls.
   public boolean femaleWantFood;
   public boolean femaleWantStone;
   public int     femaleNestSequence;

   // Dataset writers.
   public static PrintWriter MaleDatasetWriter   = null;
   public static PrintWriter FemaleDatasetWriter = null;

   // Verbosity.
   public static boolean Verbose = true;

   // Construct.
   public NestingBirds()
   {
      // Random numbers.
      if (Randomizer == null)
      {
         Randomizer = new Random(RANDOM_NUMBER_SEED);
      }

      // Initialize world.
      world = new Cell[width][height];
      for (int x = 0; x < width; x++)
      {
         for (int y = 0; y < height; y++)
         {
            world[x][y] = new Cell();
            switch (locale[x][y])
            {
            case 0:
               world[x][y].locale = LOCALE.PLAIN;
               world[x][y].object = OBJECT.NO_OBJECT;
               break;

            case 1:
               world[x][y].locale = LOCALE.FOREST;
               world[x][y].object = OBJECT.NO_OBJECT;
               break;

            case 2:
               world[x][y].locale = LOCALE.FOREST;
               world[x][y].object = OBJECT.MOUSE;
               break;

            case 3:
               world[x][y].locale = LOCALE.DESERT;
               world[x][y].object = OBJECT.NO_OBJECT;
               break;

            case 4:
               world[x][y].locale = LOCALE.DESERT;
               world[x][y].object = OBJECT.STONE;
               break;
            }
         }
      }

      // Create birds.
      female             = new FemaleBird();
      female.x           = width / 2;
      female.y           = height / 2;
      female.food        = FemaleBird.INITIAL_FOOD;
      female.response    = RESPONSE.DO_NOTHING;
      male               = new MaleBird();
      male.x             = width / 2;
      male.y             = height / 2;
      male.food          = MaleBird.INITIAL_FOOD;
      male.response      = RESPONSE.DO_NOTHING;
      femaleWantFood     = false;
      femaleWantStone    = false;
      femaleNestSequence = 0;
      setSensors(Bird.FEMALE);
      setSensors(Bird.MALE);
   }


   // Step.
   public void step()
   {
      // Do responses in world.
      doResponse(Bird.FEMALE);
      doResponse(Bird.MALE);

      // Step mice.
      stepMice();

      // Set female sensors.
      setSensors(Bird.FEMALE);

      // Produce female response.
      switch (ResponseDriver)
      {
      case RESPONSE_DRIVER.BIRD:
         female.cycle();
         break;

      case RESPONSE_DRIVER.AUTOPILOT:
         cycleAutopilot(Bird.FEMALE);
         break;
      }
      if (Verbose)
      {
         System.out.print("Female: Location: [" + female.x + "," + female.y + "], ");
         female.print();
      }

      // Set male sensors.
      setSensors(Bird.MALE);

      // Produce male response.
      switch (ResponseDriver)
      {
      case RESPONSE_DRIVER.BIRD:
         male.cycle();
         break;

      case RESPONSE_DRIVER.AUTOPILOT:
         cycleAutopilot(Bird.MALE);
         break;
      }
      if (Verbose)
      {
         System.out.print("Male: Location: [" + male.x + "," + male.y + "], ");
         male.print();
      }

      // Write stimulus-response to datasets.
      writeDatasets();
   }


   // Cycle on autopilot.
   public void cycleAutopilot(int gender)
   {
      if (gender == Bird.MALE)
      {
         maleAutopilot();
      }
      else
      {
         femaleAutopilot();
      }
   }


   // Male autopilot.
   public void maleAutopilot()
   {
      if (world[male.x][male.y].object == OBJECT.EGG)
      {
         male.response = Bird.RESPONSE.DO_NOTHING;
      }
      else
      {
         if (!getFood() && !getStone())
         {
            returnToFemale();
         }
      }
   }


   // Get food.
   public boolean getFood()
   {
      if ((male.x == female.x) && (male.y == female.y))
      {
         if (female.response == FemaleBird.RESPONSE.WANT_FOOD)
         {
            femaleWantFood = true;
         }
      }
      if ((male.food > 0) && !femaleWantFood)
      {
         return(false);
      }
      if (male.hasObject == OBJECT.MOUSE)
      {
         if (male.food < 0) { male.food = 0; }
         if (male.food == 0)
         {
            male.response = Bird.RESPONSE.EAT;
            return(true);
         }
         if ((male.x == female.x) && (male.y == female.y))
         {
            male.response  = MaleBird.RESPONSE.GIVE_FOOD;
            femaleWantFood = false;
            return(true);
         }
         else
         {
            return(false);
         }
      }
      if (male.hasObject != OBJECT.NO_OBJECT)
      {
         male.response = Bird.RESPONSE.TOSS;
         return(true);
      }
      if (world[male.x][male.y].object == OBJECT.MOUSE)
      {
         male.response = Bird.RESPONSE.GET;
         return(true);
      }
      if (male.sensors[Bird.LEFT_OBJECT_SENSOR] == OBJECT.MOUSE)
      {
         male.response = Bird.RESPONSE.TURN_LEFT;
         return(true);
      }
      if (male.sensors[Bird.FORWARD_OBJECT_SENSOR] == OBJECT.MOUSE)
      {
         male.response = Bird.RESPONSE.MOVE;
         return(true);
      }
      if (male.sensors[Bird.RIGHT_OBJECT_SENSOR] == OBJECT.MOUSE)
      {
         male.response = Bird.RESPONSE.TURN_RIGHT;
         return(true);
      }
      if ((male.sensors[Bird.CURRENT_LOCALE_SENSOR] == LOCALE.FOREST) ||
          (male.sensors[Bird.LEFT_LOCALE_SENSOR] == LOCALE.FOREST) ||
          (male.sensors[Bird.FORWARD_LOCALE_SENSOR] == LOCALE.FOREST) ||
          (male.sensors[Bird.RIGHT_LOCALE_SENSOR] == LOCALE.FOREST))
      {
         if (male.sensors[Bird.CURRENT_LOCALE_SENSOR] == LOCALE.FOREST)
         {
            if ((male.sensors[Bird.LEFT_LOCALE_SENSOR] != LOCALE.FOREST) &&
                (male.sensors[Bird.FORWARD_LOCALE_SENSOR] != LOCALE.FOREST) &&
                (male.sensors[Bird.RIGHT_LOCALE_SENSOR] != LOCALE.FOREST))
            {
               male.response = Bird.RESPONSE.TURN_RIGHT;
            }
            else
            {
               ArrayList<Integer> responses = new ArrayList<Integer>();
               if (male.sensors[Bird.FORWARD_LOCALE_SENSOR] == LOCALE.FOREST)
               {
                  responses.add(Bird.RESPONSE.MOVE);
                  responses.add(Bird.RESPONSE.MOVE);
               }
               if (male.sensors[Bird.LEFT_LOCALE_SENSOR] == LOCALE.FOREST)
               {
                  responses.add(Bird.RESPONSE.TURN_LEFT);
               }
               if (male.sensors[Bird.RIGHT_LOCALE_SENSOR] == LOCALE.FOREST)
               {
                  responses.add(Bird.RESPONSE.TURN_RIGHT);
               }
               male.response = responses.get(Randomizer.nextInt(responses.size()));
            }
         }
         else
         {
            ArrayList<Integer> responses = new ArrayList<Integer>();
            if (male.sensors[Bird.FORWARD_LOCALE_SENSOR] == LOCALE.FOREST)
            {
               responses.add(Bird.RESPONSE.MOVE);
               responses.add(Bird.RESPONSE.MOVE);
            }
            if (male.sensors[Bird.LEFT_LOCALE_SENSOR] == LOCALE.FOREST)
            {
               responses.add(Bird.RESPONSE.TURN_LEFT);
            }
            if (male.sensors[Bird.RIGHT_LOCALE_SENSOR] == LOCALE.FOREST)
            {
               responses.add(Bird.RESPONSE.TURN_RIGHT);
            }
            male.response = responses.get(Randomizer.nextInt(responses.size()));
         }
         return(true);
      }
      int mousex = 3;
      int mousey = 5;
      if (male.x > mousex)
      {
         if (male.orientation == Bird.ORIENTATION.WEST)
         {
            male.response = Bird.RESPONSE.MOVE;
         }
         else
         {
            if (male.orientation == Bird.ORIENTATION.SOUTH)
            {
               male.response = Bird.RESPONSE.TURN_RIGHT;
            }
            else
            {
               male.response = Bird.RESPONSE.TURN_LEFT;
            }
         }
         return(true);
      }
      if (male.x < mousex)
      {
         if (male.orientation == Bird.ORIENTATION.EAST)
         {
            male.response = Bird.RESPONSE.MOVE;
         }
         else
         {
            if (male.orientation == Bird.ORIENTATION.SOUTH)
            {
               male.response = Bird.RESPONSE.TURN_LEFT;
            }
            else
            {
               male.response = Bird.RESPONSE.TURN_RIGHT;
            }
         }
         return(true);
      }
      if (male.y > mousey)
      {
         if (male.orientation == Bird.ORIENTATION.NORTH)
         {
            male.response = Bird.RESPONSE.MOVE;
         }
         else
         {
            if (male.orientation == Bird.ORIENTATION.EAST)
            {
               male.response = Bird.RESPONSE.TURN_LEFT;
            }
            else
            {
               male.response = Bird.RESPONSE.TURN_RIGHT;
            }
         }
         return(true);
      }
      if (male.y < mousey)
      {
         if (male.orientation == Bird.ORIENTATION.SOUTH)
         {
            male.response = Bird.RESPONSE.MOVE;
         }
         else
         {
            if (male.orientation == Bird.ORIENTATION.EAST)
            {
               male.response = Bird.RESPONSE.TURN_RIGHT;
            }
            else
            {
               male.response = Bird.RESPONSE.TURN_LEFT;
            }
         }
      }
      return(true);
   }


   // Get stone.
   public boolean getStone()
   {
      if ((male.x == female.x) && (male.y == female.y))
      {
         if (female.response == FemaleBird.RESPONSE.WANT_STONE)
         {
            femaleWantStone = true;
         }
      }
      if (femaleWantFood || !femaleWantStone)
      {
         return(false);
      }
      if (male.hasObject == OBJECT.STONE)
      {
         if ((male.x == female.x) && (male.y == female.y))
         {
            male.response   = MaleBird.RESPONSE.GIVE_STONE;
            femaleWantStone = false;
            return(true);
         }
         else
         {
            return(false);
         }
      }
      if ((world[male.x][male.y].object == OBJECT.STONE) &&
          (world[male.x][male.y].locale == LOCALE.DESERT))
      {
         male.response = Bird.RESPONSE.GET;
         return(true);
      }
      if ((male.sensors[Bird.LEFT_OBJECT_SENSOR] == OBJECT.STONE) &&
          (male.sensors[Bird.LEFT_LOCALE_SENSOR] == LOCALE.DESERT))
      {
         male.response = Bird.RESPONSE.TURN_LEFT;
         return(true);
      }
      if ((male.sensors[Bird.FORWARD_OBJECT_SENSOR] == OBJECT.STONE) &&
          (male.sensors[Bird.FORWARD_LOCALE_SENSOR] == LOCALE.DESERT))
      {
         male.response = Bird.RESPONSE.MOVE;
         return(true);
      }
      if ((male.sensors[Bird.RIGHT_OBJECT_SENSOR] == OBJECT.STONE) &&
          (male.sensors[Bird.RIGHT_LOCALE_SENSOR] == LOCALE.DESERT))
      {
         male.response = Bird.RESPONSE.TURN_RIGHT;
         return(true);
      }
      if ((male.sensors[Bird.CURRENT_LOCALE_SENSOR] == LOCALE.DESERT) ||
          (male.sensors[Bird.LEFT_LOCALE_SENSOR] == LOCALE.DESERT) ||
          (male.sensors[Bird.FORWARD_LOCALE_SENSOR] == LOCALE.DESERT) ||
          (male.sensors[Bird.RIGHT_LOCALE_SENSOR] == LOCALE.DESERT))
      {
         if (male.sensors[Bird.CURRENT_LOCALE_SENSOR] == LOCALE.DESERT)
         {
            if ((male.sensors[Bird.LEFT_LOCALE_SENSOR] != LOCALE.DESERT) &&
                (male.sensors[Bird.FORWARD_LOCALE_SENSOR] != LOCALE.DESERT) &&
                (male.sensors[Bird.RIGHT_LOCALE_SENSOR] != LOCALE.DESERT))
            {
               male.response = Bird.RESPONSE.TURN_RIGHT;
            }
            else
            {
               ArrayList<Integer> responses = new ArrayList<Integer>();
               if (male.sensors[Bird.FORWARD_LOCALE_SENSOR] == LOCALE.DESERT)
               {
                  responses.add(Bird.RESPONSE.MOVE);
                  responses.add(Bird.RESPONSE.MOVE);
               }
               if (male.sensors[Bird.LEFT_LOCALE_SENSOR] == LOCALE.DESERT)
               {
                  responses.add(Bird.RESPONSE.TURN_LEFT);
               }
               if (male.sensors[Bird.RIGHT_LOCALE_SENSOR] == LOCALE.DESERT)
               {
                  responses.add(Bird.RESPONSE.TURN_RIGHT);
               }
               male.response = responses.get(Randomizer.nextInt(responses.size()));
            }
         }
         else
         {
            ArrayList<Integer> responses = new ArrayList<Integer>();
            if (male.sensors[Bird.FORWARD_LOCALE_SENSOR] == LOCALE.DESERT)
            {
               responses.add(Bird.RESPONSE.MOVE);
               responses.add(Bird.RESPONSE.MOVE);
            }
            if (male.sensors[Bird.LEFT_LOCALE_SENSOR] == LOCALE.DESERT)
            {
               responses.add(Bird.RESPONSE.TURN_LEFT);
            }
            if (male.sensors[Bird.RIGHT_LOCALE_SENSOR] == LOCALE.DESERT)
            {
               responses.add(Bird.RESPONSE.TURN_RIGHT);
            }
            male.response = responses.get(Randomizer.nextInt(responses.size()));
         }
         return(true);
      }
      int stonex = 14;
      int stoney = 16;
      if (male.x > stonex)
      {
         if (male.orientation == Bird.ORIENTATION.WEST)
         {
            male.response = Bird.RESPONSE.MOVE;
         }
         else
         {
            if (male.orientation == Bird.ORIENTATION.SOUTH)
            {
               male.response = Bird.RESPONSE.TURN_RIGHT;
            }
            else
            {
               male.response = Bird.RESPONSE.TURN_LEFT;
            }
         }
         return(true);
      }
      if (male.x < stonex)
      {
         if (male.orientation == Bird.ORIENTATION.EAST)
         {
            male.response = Bird.RESPONSE.MOVE;
         }
         else
         {
            if (male.orientation == Bird.ORIENTATION.SOUTH)
            {
               male.response = Bird.RESPONSE.TURN_LEFT;
            }
            else
            {
               male.response = Bird.RESPONSE.TURN_RIGHT;
            }
         }
         return(true);
      }
      if (male.y > stoney)
      {
         if (male.orientation == Bird.ORIENTATION.NORTH)
         {
            male.response = Bird.RESPONSE.MOVE;
         }
         else
         {
            if (male.orientation == Bird.ORIENTATION.EAST)
            {
               male.response = Bird.RESPONSE.TURN_LEFT;
            }
            else
            {
               male.response = Bird.RESPONSE.TURN_RIGHT;
            }
         }
         return(true);
      }
      if (male.y < stoney)
      {
         if (male.orientation == Bird.ORIENTATION.SOUTH)
         {
            male.response = Bird.RESPONSE.MOVE;
         }
         else
         {
            if (male.orientation == Bird.ORIENTATION.EAST)
            {
               male.response = Bird.RESPONSE.TURN_RIGHT;
            }
            else
            {
               male.response = Bird.RESPONSE.TURN_LEFT;
            }
         }
      }
      return(true);
   }


   // Male returns to female.
   public boolean returnToFemale()
   {
      switch (male.sensors[Bird.MATE_PROXIMITY_SENSOR])
      {
      case Bird.MATE_PROXIMITY_PRESENT:
         male.response = Bird.RESPONSE.DO_NOTHING;
         return(true);

      case Bird.MATE_PROXIMITY_LEFT:
         male.response = Bird.RESPONSE.TURN_LEFT;
         break;

      case Bird.MATE_PROXIMITY_FORWARD:
         male.response = Bird.RESPONSE.MOVE;
         break;

      case Bird.MATE_PROXIMITY_RIGHT:
         male.response = Bird.RESPONSE.TURN_RIGHT;
         break;

      case Bird.MATE_PROXIMITY_UNKNOWN:
         int centerx = width / 2;
         int centery = height / 2;
         if (male.x < centerx)
         {
            if (male.orientation == Bird.ORIENTATION.EAST)
            {
               male.response = Bird.RESPONSE.MOVE;
            }
            else
            {
               if (male.orientation == Bird.ORIENTATION.SOUTH)
               {
                  male.response = Bird.RESPONSE.TURN_LEFT;
               }
               else
               {
                  male.response = Bird.RESPONSE.TURN_RIGHT;
               }
            }
         }
         else if (male.x > centerx)
         {
            if (male.orientation == Bird.ORIENTATION.WEST)
            {
               male.response = Bird.RESPONSE.MOVE;
            }
            else
            {
               if (male.orientation == Bird.ORIENTATION.SOUTH)
               {
                  male.response = Bird.RESPONSE.TURN_RIGHT;
               }
               else
               {
                  male.response = Bird.RESPONSE.TURN_LEFT;
               }
            }
         }
         else if (male.y < centery)
         {
            if (male.orientation == Bird.ORIENTATION.SOUTH)
            {
               male.response = Bird.RESPONSE.MOVE;
            }
            else
            {
               if (male.orientation == Bird.ORIENTATION.EAST)
               {
                  male.response = Bird.RESPONSE.TURN_RIGHT;
               }
               else
               {
                  male.response = Bird.RESPONSE.TURN_LEFT;
               }
            }
         }
         else if (male.y > centery)
         {
            if (male.orientation == Bird.ORIENTATION.NORTH)
            {
               male.response = Bird.RESPONSE.MOVE;
            }
            else
            {
               if (male.orientation == Bird.ORIENTATION.WEST)
               {
                  male.response = Bird.RESPONSE.TURN_RIGHT;
               }
               else
               {
                  male.response = Bird.RESPONSE.TURN_LEFT;
               }
            }
         }
         else
         {
            male.response = Bird.RESPONSE.MOVE;
         }
         break;
      }
      return(false);
   }


   // Female autopilot.
   public void femaleAutopilot()
   {
      if (world[female.x][female.y].object == OBJECT.EGG)
      {
         female.response = Bird.RESPONSE.DO_NOTHING;
         return;
      }
      if (female.food < 0) { female.food = 0; }
      if (female.food == 0)
      {
         if (female.hasObject == OBJECT.MOUSE)
         {
            female.response = Bird.RESPONSE.EAT;
         }
         else
         {
            if (female.hasObject == OBJECT.NO_OBJECT)
            {
               female.response = FemaleBird.RESPONSE.WANT_FOOD;
            }
            else
            {
               female.response = Bird.RESPONSE.TOSS;
            }
         }
         return;
      }
      switch (femaleNestSequence)
      {
      case 0:
         if (world[female.x][female.y].object == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.GET;
         }
         else if (female.hasObject == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.TOSS;
         }
         else if (female.orientation == Bird.ORIENTATION.NORTH)
         {
            female.response = Bird.RESPONSE.MOVE;
            femaleNestSequence++;
         }
         else
         {
            female.response = Bird.RESPONSE.TURN_RIGHT;
         }
         break;

      case 1:
         if (world[female.x][female.y].object == OBJECT.STONE)
         {
            if (female.orientation == Bird.ORIENTATION.WEST)
            {
               female.response = Bird.RESPONSE.MOVE;
               femaleNestSequence++;
            }
            else
            {
               female.response = Bird.RESPONSE.TURN_LEFT;
            }
         }
         else if (female.hasObject == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.PUT;
         }
         else
         {
            female.response = FemaleBird.RESPONSE.WANT_STONE;
         }
         break;

      case 2:
         if (world[female.x][female.y].object == OBJECT.STONE)
         {
            if (female.orientation == Bird.ORIENTATION.SOUTH)
            {
               female.response = Bird.RESPONSE.MOVE;
               femaleNestSequence++;
            }
            else
            {
               female.response = Bird.RESPONSE.TURN_LEFT;
            }
         }
         else if (female.hasObject == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.PUT;
         }
         else
         {
            female.response = FemaleBird.RESPONSE.WANT_STONE;
         }
         break;

      case 3:
         if (world[female.x][female.y].object == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.MOVE;
            femaleNestSequence++;
         }
         else if (female.hasObject == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.PUT;
         }
         else
         {
            female.response = FemaleBird.RESPONSE.WANT_STONE;
         }
         break;

      case 4:
         if (world[female.x][female.y].object == OBJECT.STONE)
         {
            if (female.orientation == Bird.ORIENTATION.EAST)
            {
               female.response = Bird.RESPONSE.MOVE;
               femaleNestSequence++;
            }
            else
            {
               female.response = Bird.RESPONSE.TURN_LEFT;
            }
         }
         else if (female.hasObject == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.PUT;
         }
         else
         {
            female.response = FemaleBird.RESPONSE.WANT_STONE;
         }
         break;

      case 5:
         if (world[female.x][female.y].object == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.MOVE;
            femaleNestSequence++;
         }
         else if (female.hasObject == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.PUT;
         }
         else
         {
            female.response = FemaleBird.RESPONSE.WANT_STONE;
         }
         break;

      case 6:
         if (world[female.x][female.y].object == OBJECT.STONE)
         {
            if (female.orientation == Bird.ORIENTATION.NORTH)
            {
               female.response = Bird.RESPONSE.MOVE;
               femaleNestSequence++;
            }
            else
            {
               female.response = Bird.RESPONSE.TURN_LEFT;
            }
         }
         else if (female.hasObject == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.PUT;
         }
         else
         {
            female.response = FemaleBird.RESPONSE.WANT_STONE;
         }
         break;

      case 7:
         if (world[female.x][female.y].object == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.MOVE;
            femaleNestSequence++;
         }
         else if (female.hasObject == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.PUT;
         }
         else
         {
            female.response = FemaleBird.RESPONSE.WANT_STONE;
         }
         break;

      case 8:
         if (world[female.x][female.y].object == OBJECT.STONE)
         {
            if (female.orientation == Bird.ORIENTATION.WEST)
            {
               female.response = Bird.RESPONSE.MOVE;
               femaleNestSequence++;
            }
            else
            {
               female.response = Bird.RESPONSE.TURN_LEFT;
            }
         }
         else if (female.hasObject == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.PUT;
         }
         else
         {
            female.response = FemaleBird.RESPONSE.WANT_STONE;
         }
         break;

      case 9:
         if (world[female.x][female.y].object == OBJECT.STONE)
         {
            if (female.orientation == Bird.ORIENTATION.SOUTH)
            {
               female.response = Bird.RESPONSE.MOVE;
               femaleNestSequence++;
            }
            else
            {
               female.response = Bird.RESPONSE.TURN_LEFT;
            }
         }
         else if (female.hasObject == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.PUT;
         }
         else
         {
            female.response = FemaleBird.RESPONSE.WANT_STONE;
         }
         break;

      case 10:
         if (world[female.x][female.y].object == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.GET;
         }
         else if (female.hasObject == OBJECT.STONE)
         {
            female.response = Bird.RESPONSE.TOSS;
         }
         else if (world[female.x][female.y].object == OBJECT.NO_OBJECT)
         {
            female.response = FemaleBird.RESPONSE.LAY_EGG;
            femaleNestSequence++;
         }
         else
         {
            female.response    = Bird.RESPONSE.TURN_RIGHT;
            femaleNestSequence = 0;
         }
         break;

      case 11:
         break;
      }
   }


   // Do bird response.
   public void doResponse(int gender)
   {
      Bird bird = male;

      if (gender == Bird.FEMALE)
      {
         bird = female;
      }
      Cell cell = world[bird.x][bird.y];

      if (bird.response < Bird.RESPONSE.NUM_RESPONSES)
      {
         switch (bird.response)
         {
         case Bird.RESPONSE.DO_NOTHING:
            break;

         case Bird.RESPONSE.EAT:
            if (bird.hasObject == OBJECT.MOUSE)
            {
               if (bird.gender == Bird.MALE)
               {
                  if (MaleBird.RANDOMIZE_FOOD_LEVEL && (MaleBird.FOOD_DURATION > 0))
                  {
                     bird.food = Randomizer.nextInt(MaleBird.FOOD_DURATION + 1);
                  }
                  else
                  {
                     bird.food = MaleBird.FOOD_DURATION;
                  }
               }
               else
               {
                  if (FemaleBird.RANDOMIZE_FOOD_LEVEL && (FemaleBird.FOOD_DURATION > 0))
                  {
                     bird.food = Randomizer.nextInt(FemaleBird.FOOD_DURATION + 1);
                  }
                  else
                  {
                     bird.food = FemaleBird.FOOD_DURATION;
                  }
               }
               bird.hasObject = OBJECT.NO_OBJECT;
            }
            break;

         case Bird.RESPONSE.GET:
            if (bird.hasObject == OBJECT.NO_OBJECT)
            {
               bird.hasObject = cell.object;
               cell.object    = OBJECT.NO_OBJECT;
            }
            break;

         case Bird.RESPONSE.PUT:
            if (cell.object == OBJECT.NO_OBJECT)
            {
               cell.object    = bird.hasObject;
               bird.hasObject = OBJECT.NO_OBJECT;
            }
            break;

         case Bird.RESPONSE.TOSS:
            if (bird.hasObject == OBJECT.NO_OBJECT) { break; }

            // Object lands randomly nearby.
            int i = height / 2;
            if (i > width / 2) { i = width / 2; }
            for (int j = 5; j < i; j++)
            {
               int x = (int)(Randomizer.nextInt() % (long)j);
               int y = (int)(Randomizer.nextInt() % (long)j);
               if ((x == 0) && (y == 0)) { continue; }
               if ((Randomizer.nextInt() % 2) == 0) { x = -x; }
               if ((Randomizer.nextInt() % 2) == 0) { y = -y; }
               x += bird.x;
               if ((x < 0) || (x >= width)) { continue; }
               y += bird.y;
               if ((y < 0) || (y >= height)) { continue; }
               if (world[x][y].object == OBJECT.NO_OBJECT)
               {
                  world[x][y].object = bird.hasObject;
                  bird.hasObject     = OBJECT.NO_OBJECT;
                  break;
               }
            }
            if (bird.hasObject != OBJECT.NO_OBJECT)
            {
               // Vaporize object.
               bird.hasObject = OBJECT.NO_OBJECT;
            }
            break;

         case Bird.RESPONSE.MOVE:
            switch (bird.orientation)
            {
            case Bird.ORIENTATION.NORTH:
               if (bird.y > 0)
               {
                  bird.y--;
               }
               break;

            case Bird.ORIENTATION.SOUTH:
               if (bird.y < height - 1)
               {
                  bird.y++;
               }
               break;


            case Bird.ORIENTATION.EAST:
               if (bird.x < width - 1)
               {
                  bird.x++;
               }
               break;

            case Bird.ORIENTATION.WEST:
               if (bird.x > 0)
               {
                  bird.x--;
               }
               break;
            }
            break;

         case Bird.RESPONSE.TURN_RIGHT:
            switch (bird.orientation)
            {
            case Bird.ORIENTATION.NORTH:
               bird.orientation = Bird.ORIENTATION.EAST;
               break;

            case Bird.ORIENTATION.SOUTH:
               bird.orientation = Bird.ORIENTATION.WEST;
               break;

            case Bird.ORIENTATION.EAST:
               bird.orientation = Bird.ORIENTATION.SOUTH;
               break;

            case Bird.ORIENTATION.WEST:
               bird.orientation = Bird.ORIENTATION.NORTH;
               break;
            }
            break;

         case Bird.RESPONSE.TURN_LEFT:
            switch (bird.orientation)
            {
            case Bird.ORIENTATION.NORTH:
               bird.orientation = Bird.ORIENTATION.WEST;
               break;

            case Bird.ORIENTATION.SOUTH:
               bird.orientation = Bird.ORIENTATION.EAST;
               break;

            case Bird.ORIENTATION.EAST:
               bird.orientation = Bird.ORIENTATION.NORTH;
               break;

            case Bird.ORIENTATION.WEST:
               bird.orientation = Bird.ORIENTATION.SOUTH;
               break;
            }
            break;
         }
      }
      else
      {
         // Gender-specific response.
         if (gender == Bird.MALE)
         {
            switch (bird.response)
            {
            case MaleBird.RESPONSE.GIVE_FOOD:
               if (bird == male)
               {
                  if ((bird.hasObject == OBJECT.MOUSE) && (female.hasObject == OBJECT.NO_OBJECT))
                  {
                     if ((bird.x == female.x) && (bird.y == female.y))
                     {
                        female.hasObject = bird.hasObject;
                        bird.hasObject   = OBJECT.NO_OBJECT;
                     }
                  }
               }
               break;

            case MaleBird.RESPONSE.GIVE_STONE:
               if (bird == male)
               {
                  if ((bird.hasObject == OBJECT.STONE) && (female.hasObject == OBJECT.NO_OBJECT))
                  {
                     if ((bird.x == female.x) && (bird.y == female.y))
                     {
                        female.hasObject = bird.hasObject;
                        bird.hasObject   = OBJECT.NO_OBJECT;
                     }
                  }
               }
               break;
            }
         }
         else
         {
            // Female response.
            switch (bird.response)
            {
            case FemaleBird.RESPONSE.WANT_FOOD:
               break;

            case FemaleBird.RESPONSE.WANT_STONE:
               break;

            case FemaleBird.RESPONSE.LAY_EGG:
               if (bird == female)
               {
                  if (cell.object == OBJECT.NO_OBJECT)
                  {
                     cell.object = OBJECT.EGG;
                  }
               }
               break;
            }
         }
      }

      // Digest food.
      bird.digest();
   }


   // Set bird sensors.
   public void setSensors(int gender)
   {
      Bird bird = null;

      int[] sensors = null;

      if (gender == Bird.FEMALE)
      {
         bird    = female;
         sensors = new int[FemaleBird.NUM_SENSORS];
         female.setSensors(sensors);
      }
      else
      {
         bird    = male;
         sensors = new int[MaleBird.NUM_SENSORS];
         male.setSensors(sensors);
      }

      // Set locale, object, and mate proximity sensors.
      sensors[Bird.MATE_PROXIMITY_SENSOR] = Bird.MATE_PROXIMITY_UNKNOWN;
      int x = -1;
      int y = -1;
      for (int i = 0; i < Bird.NUM_CELL_SENSORS; i++)
      {
         switch (i)
         {
         // Current.
         case 0:
            sensors[Bird.CURRENT_LOCALE_SENSOR] = world[bird.x][bird.y].locale;
            sensors[Bird.CURRENT_OBJECT_SENSOR] = world[bird.x][bird.y].object;
            if ((male.x == female.x) && (male.y == female.y))
            {
               sensors[Bird.MATE_PROXIMITY_SENSOR] = Bird.MATE_PROXIMITY_PRESENT;
            }
            break;

         // Left.
         case 1:
            x = -1;
            y = -1;
            switch (bird.orientation)
            {
            case Bird.ORIENTATION.NORTH:
               if (bird.x > 0)
               {
                  sensors[Bird.LEFT_LOCALE_SENSOR] = world[bird.x - 1][bird.y].locale;
                  sensors[Bird.LEFT_OBJECT_SENSOR] = world[bird.x - 1][bird.y].object;
                  x = bird.x - 1;
                  y = bird.y;
               }
               break;

            case Bird.ORIENTATION.SOUTH:
               if (bird.x < width - 1)
               {
                  sensors[Bird.LEFT_LOCALE_SENSOR] = world[bird.x + 1][bird.y].locale;
                  sensors[Bird.LEFT_OBJECT_SENSOR] = world[bird.x + 1][bird.y].object;
                  x = bird.x + 1;
                  y = bird.y;
               }
               break;

            case Bird.ORIENTATION.EAST:
               if (bird.y > 0)
               {
                  sensors[Bird.LEFT_LOCALE_SENSOR] = world[bird.x][bird.y - 1].locale;
                  sensors[Bird.LEFT_OBJECT_SENSOR] = world[bird.x][bird.y - 1].object;
                  x = bird.x;
                  y = bird.y - 1;
               }
               break;

            case Bird.ORIENTATION.WEST:
               if (bird.y < height - 1)
               {
                  sensors[Bird.LEFT_LOCALE_SENSOR] = world[bird.x][bird.y + 1].locale;
                  sensors[Bird.LEFT_OBJECT_SENSOR] = world[bird.x][bird.y + 1].object;
                  x = bird.x;
                  y = bird.y + 1;
               }
               break;
            }
            if (x >= 0)
            {
               if (bird == male)
               {
                  if ((female.x == x) && (female.y == y))
                  {
                     sensors[Bird.MATE_PROXIMITY_SENSOR] = Bird.MATE_PROXIMITY_LEFT;
                  }
               }
               else
               {
                  if ((male.x == x) && (male.y == y))
                  {
                     sensors[Bird.MATE_PROXIMITY_SENSOR] = Bird.MATE_PROXIMITY_LEFT;
                  }
               }
            }
            break;

         // Forward.
         case 2:
            x = -1;
            y = -1;
            switch (bird.orientation)
            {
            case Bird.ORIENTATION.NORTH:
               if (bird.y > 0)
               {
                  sensors[Bird.FORWARD_LOCALE_SENSOR] = world[bird.x][bird.y - 1].locale;
                  sensors[Bird.FORWARD_OBJECT_SENSOR] = world[bird.x][bird.y - 1].object;
                  x = bird.x;
                  y = bird.y - 1;
               }
               break;

            case Bird.ORIENTATION.SOUTH:
               if (bird.y < height - 1)
               {
                  sensors[Bird.FORWARD_LOCALE_SENSOR] = world[bird.x][bird.y + 1].locale;
                  sensors[Bird.FORWARD_OBJECT_SENSOR] = world[bird.x][bird.y + 1].object;
                  x = bird.x;
                  y = bird.y + 1;
               }
               break;

            case Bird.ORIENTATION.EAST:
               if (bird.x < width - 1)
               {
                  sensors[Bird.FORWARD_LOCALE_SENSOR] = world[bird.x + 1][bird.y].locale;
                  sensors[Bird.FORWARD_OBJECT_SENSOR] = world[bird.x + 1][bird.y].object;
                  x = bird.x + 1;
                  y = bird.y;
               }
               break;

            case Bird.ORIENTATION.WEST:
               if (bird.x > 0)
               {
                  sensors[Bird.FORWARD_LOCALE_SENSOR] = world[bird.x - 1][bird.y].locale;
                  sensors[Bird.FORWARD_OBJECT_SENSOR] = world[bird.x - 1][bird.y].object;
                  x = bird.x - 1;
                  y = bird.y;
               }
               break;
            }
            if (x >= 0)
            {
               if (bird == male)
               {
                  if ((female.x == x) && (female.y == y))
                  {
                     sensors[Bird.MATE_PROXIMITY_SENSOR] = Bird.MATE_PROXIMITY_FORWARD;
                  }
               }
               else
               {
                  if ((male.x == x) && (male.y == y))
                  {
                     sensors[Bird.MATE_PROXIMITY_SENSOR] = Bird.MATE_PROXIMITY_FORWARD;
                  }
               }
            }
            break;

         // Right.
         case 3:
            x = -1;
            y = -1;
            switch (bird.orientation)
            {
            case Bird.ORIENTATION.NORTH:
               if (bird.x < width - 1)
               {
                  sensors[Bird.RIGHT_LOCALE_SENSOR] = world[bird.x + 1][bird.y].locale;
                  sensors[Bird.RIGHT_OBJECT_SENSOR] = world[bird.x + 1][bird.y].object;
                  x = bird.x + 1;
                  y = bird.y;
               }
               break;

            case Bird.ORIENTATION.SOUTH:
               if (bird.x > 0)
               {
                  sensors[Bird.RIGHT_LOCALE_SENSOR] = world[bird.x - 1][bird.y].locale;
                  sensors[Bird.RIGHT_OBJECT_SENSOR] = world[bird.x - 1][bird.y].object;
                  x = bird.x - 1;
                  y = bird.y;
               }
               break;

            case Bird.ORIENTATION.EAST:
               if (bird.y < height - 1)
               {
                  sensors[Bird.RIGHT_LOCALE_SENSOR] = world[bird.x][bird.y + 1].locale;
                  sensors[Bird.RIGHT_OBJECT_SENSOR] = world[bird.x][bird.y + 1].object;
                  x = bird.x;
                  y = bird.y + 1;
               }
               break;

            case Bird.ORIENTATION.WEST:
               if (bird.y > 0)
               {
                  sensors[Bird.RIGHT_LOCALE_SENSOR] = world[bird.x][bird.y - 1].locale;
                  sensors[Bird.RIGHT_OBJECT_SENSOR] = world[bird.x][bird.y - 1].object;
                  x = bird.x;
                  y = bird.y - 1;
               }
               break;
            }
            if (x >= 0)
            {
               if (bird == male)
               {
                  if ((female.x == x) && (female.y == y))
                  {
                     sensors[Bird.MATE_PROXIMITY_SENSOR] = Bird.MATE_PROXIMITY_RIGHT;
                  }
               }
               else
               {
                  if ((male.x == x) && (male.y == y))
                  {
                     sensors[Bird.MATE_PROXIMITY_SENSOR] = Bird.MATE_PROXIMITY_RIGHT;
                  }
               }
            }
            break;
         }
      }

      // Male senses female want?
      if (gender == Bird.MALE)
      {
         sensors[MaleBird.WANT_FOOD_SENSOR]  = 0;
         sensors[MaleBird.WANT_STONE_SENSOR] = 0;
         if ((bird.x == female.x) && (bird.y == female.y))
         {
            switch (female.response)
            {
            case FemaleBird.RESPONSE.WANT_FOOD:
               sensors[MaleBird.WANT_FOOD_SENSOR] = 1;
               break;

            case FemaleBird.RESPONSE.WANT_STONE:
               sensors[MaleBird.WANT_STONE_SENSOR] = 1;
               break;
            }
         }
      }
   }


   // Write datasets.
   public void writeDatasets()
   {
      // Write female dataset?
      if (FemaleDatasetWriter != null)
      {
         female.writeDataset(FemaleDatasetWriter);
      }

      // Write male dataset?
      if (MaleDatasetWriter != null)
      {
         male.writeDataset(MaleDatasetWriter);
      }
   }


   // Euclidean distance.
   public static double distance(double x1, double y1, double x2, double y2)
   {
      double xd = x1 - x2;
      double yd = y1 - y2;

      return(Math.sqrt((xd * xd) + (yd * yd)));
   }


   // Move mice in forest.
   public void stepMice()
   {
      for (int x = Randomizer.nextInt(width), x2 = 0; x2 < width; x = (x + 1) % width, x2++)
      {
         for (int y = Randomizer.nextInt(height), y2 = 0; y2 < height; y = (y + 1) % height, y2++)
         {
            if ((world[x][y].object == OBJECT.MOUSE) &&
                ((male.x != x) || (male.y != y) || (male.response != Bird.RESPONSE.GET)) &&
                (Randomizer.nextDouble() < MOUSE_MOVE_PROBABILITY))
            {
               boolean move = false;
               for (int i = Randomizer.nextInt(4), i2 = 0; i2 < 4 && !move; i = (i + 1) % 4, i2++)
               {
                  switch (i)
                  {
                  case 0:
                     if ((y > 0) && (world[x][y - 1].locale == LOCALE.FOREST) &&
                         (world[x][y - 1].object == OBJECT.NO_OBJECT))
                     {
                        world[x][y].object     = OBJECT.NO_OBJECT;
                        world[x][y - 1].object = -OBJECT.MOUSE;
                        move = true;
                     }
                     break;

                  case 1:
                     if ((y < height - 1) && (world[x][y + 1].locale == LOCALE.FOREST) &&
                         (world[x][y + 1].object == OBJECT.NO_OBJECT))
                     {
                        world[x][y].object     = OBJECT.NO_OBJECT;
                        world[x][y + 1].object = -OBJECT.MOUSE;
                        move = true;
                     }
                     break;

                  case 2:
                     if ((x > 0) && (world[x - 1][y].locale == LOCALE.FOREST) &&
                         (world[x - 1][y].object == OBJECT.NO_OBJECT))
                     {
                        world[x][y].object     = OBJECT.NO_OBJECT;
                        world[x - 1][y].object = -OBJECT.MOUSE;
                        move = true;
                     }
                     break;

                  case 3:
                     if ((x < width - 1) && (world[x + 1][y].locale == LOCALE.FOREST) &&
                         (world[x + 1][y].object == OBJECT.NO_OBJECT))
                     {
                        world[x][y].object     = OBJECT.NO_OBJECT;
                        world[x + 1][y].object = -OBJECT.MOUSE;
                        move = true;
                     }
                     break;
                  }
               }
            }
         }
      }
      for (int x = 0; x < width; x++)
      {
         for (int y = 0; y < height; y++)
         {
            if (world[x][y].object == -OBJECT.MOUSE)
            {
               world[x][y].object = OBJECT.MOUSE;
            }
         }
      }
   }


   // Main.
   public static void main(String[] args)
   {
      int     steps              = -1;
      int     runs               = 1;
      boolean writeMaleDataset   = false;
      boolean writeFemaleDataset = false;

      for (int i = 0; i < args.length; i++)
      {
         if (args[i].equals("-steps"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid steps option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               steps = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid steps option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (steps < 0)
            {
               System.err.println("Invalid steps option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-runs"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid runs option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               runs = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid runs option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (runs < 0)
            {
               System.err.println("Invalid runs option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-responseDriver"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid driver option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (args[i].equals("autopilot"))
            {
               ResponseDriver = RESPONSE_DRIVER.AUTOPILOT;
            }
            else if (args[i].equals("bird"))
            {
               ResponseDriver = RESPONSE_DRIVER.BIRD;
            }
            else
            {
               System.err.println("Invalid responseDriver option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-maleInitialFood"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid maleInitialFood option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               MaleBird.INITIAL_FOOD = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid maleInitialFood option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (MaleBird.INITIAL_FOOD < 0)
            {
               System.err.println("Invalid maleInitialFood option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-femaleInitialFood"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid femaleInitialFood option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               FemaleBird.INITIAL_FOOD = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid femaleInitialFood option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (FemaleBird.INITIAL_FOOD < 0)
            {
               System.err.println("Invalid femaleInitialFood option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-maleFoodDuration"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid maleFoodDuration option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               MaleBird.FOOD_DURATION = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid maleFoodDuration option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (MaleBird.FOOD_DURATION < 0)
            {
               System.err.println("Invalid maleFoodDuration option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-femaleFoodDuration"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid femaleFoodDuration option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               FemaleBird.FOOD_DURATION = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid femaleFoodDuration option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (FemaleBird.FOOD_DURATION < 0)
            {
               System.err.println("Invalid femaleFoodDuration option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-randomizeMaleFoodLevel"))
         {
            MaleBird.RANDOMIZE_FOOD_LEVEL = true;
            continue;
         }
         if (args[i].equals("-randomizeFemaleFoodLevel"))
         {
            FemaleBird.RANDOMIZE_FOOD_LEVEL = true;
            continue;
         }
         if (args[i].equals("-randomSeed"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid randomSeed option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               RANDOM_NUMBER_SEED = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid randomSeed option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (RANDOM_NUMBER_SEED <= 0)
            {
               System.err.println("Invalid randomSeed option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-writeMaleDataset"))
         {
            writeMaleDataset = true;
            continue;
         }
         if (args[i].equals("-writeFemaleDataset"))
         {
            writeFemaleDataset = true;
            continue;
         }
         if (args[i].equals("-verbose"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid verbose option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (args[i].equals("true"))
            {
               Verbose = true;
            }
            else if (args[i].equals("false"))
            {
               Verbose = false;
            }
            else
            {
               System.err.println("Invalid verbose option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-version"))
         {
            System.out.println("Nesting birds version = " + VERSION);
            System.exit(0);
         }
         if (args[i].equals("-help") || args[i].equals("-h") || args[i].equals("-?"))
         {
            System.out.println(Usage);
            System.exit(0);
         }
         System.err.println(Usage);
         System.exit(1);
      }
      if (steps == -1)
      {
         System.err.println(Usage);
         System.exit(1);
      }
      if (MaleBird.INITIAL_FOOD > MaleBird.FOOD_DURATION)
      {
         System.err.println("Male initial food cannot be greater than food duration");
         System.err.println(Usage);
         System.exit(1);
      }
      if (FemaleBird.INITIAL_FOOD > FemaleBird.FOOD_DURATION)
      {
         System.err.println("Female initial food cannot be greater than food duration");
         System.err.println(Usage);
         System.exit(1);
      }

      // Run birds.
      for (int run = 0; run < runs; run++)
      {
         if (writeMaleDataset)
         {
            try
            {
               MaleDatasetWriter = new PrintWriter(new FileOutputStream(new File(MALE_DATASET_FILE_NAME + "_" + run + ".csv")));
            }
            catch (IOException e)
            {
               System.err.println("Cannot open male dataset file: " + e.getMessage());
               System.err.println(Usage);
               System.exit(1);
            }
            Bird.writeDatasetHeader(MaleDatasetWriter, Bird.MALE);
         }

         if (writeFemaleDataset)
         {
            try
            {
               FemaleDatasetWriter = new PrintWriter(new FileOutputStream(new File(FEMALE_DATASET_FILE_NAME + "_" + run + ".csv")));
            }
            catch (IOException e)
            {
               System.err.println("Cannot open female dataset file: " + e.getMessage());
               System.err.println(Usage);
               System.exit(1);
            }
            Bird.writeDatasetHeader(FemaleDatasetWriter, Bird.FEMALE);
         }

         if (Verbose) { System.out.println("Run=" + run); }
         NestingBirds nestingbirds = new NestingBirds();
         int          eggLaidStep  = -1;
         for (int step = 0; step < steps; step++)
         {
            if (Verbose) { System.out.println("Step=" + step); }
            nestingbirds.step();
            if ((eggLaidStep < 0) && (nestingbirds.world[width / 2][height / 2].object == OBJECT.EGG))
            {
               eggLaidStep = step;
            }
         }

         if (Verbose)
         {
            System.out.print("Run results: ");
            if (eggLaidStep >= 0)
            {
               System.out.print("egg laid at step " + eggLaidStep);
            }
            else
            {
               System.out.print("no egg");
            }
            int mouseCount = 0;
            int stoneCount = 0;
            for (int x = 0; x < width; x++)
            {
               for (int y = 0; y < height; y++)
               {
                  if ((nestingbirds.world[x][y].locale == LOCALE.FOREST) &&
                      (nestingbirds.world[x][y].object == OBJECT.MOUSE))
                  {
                     mouseCount++;
                  }
                  if ((nestingbirds.world[x][y].locale == LOCALE.DESERT) &&
                      (nestingbirds.world[x][y].object == OBJECT.STONE))
                  {
                     stoneCount++;
                  }
               }
            }
            System.out.println(", remaining mice=" + mouseCount + ", remaining stones=" + stoneCount);
         }

         if (writeMaleDataset)
         {
            MaleDatasetWriter.close();
         }

         if (writeFemaleDataset)
         {
            FemaleDatasetWriter.close();
         }
      }
   }
}
