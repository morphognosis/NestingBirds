// For conditions of distribution and use, see copyright notice in Morphognosis.java

// The nesting birds environment.

package morphognosis.nestingbirds;

import java.util.Random;

import morphognosis.nestingbirds.Bird.RESPONSE;

public class Environment
{
   // Locale map: grassland=0, forest=1, food=2, desert=3, stone=4
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
      public static final int MANUAL    = 0;
      public static final int BIRD      = 1;
      public static final int AUTOPILOT = 2;

      // To string.
      public static String toString(int driver)
      {
         switch (driver)
         {
         case MANUAL:
            return("manual");

         case BIRD:
            return("bird");

         case AUTOPILOT:
            return("autopilot");
         }
         return("Unknown driver");
      }
   };
   public int responseDriver = RESPONSE_DRIVER.MANUAL;
   public void setResponseDriver(int driver) { responseDriver = driver; }

   // Disrupt nest.
   private int disruptNest = 0;

   // Autopilot controls.
   public boolean femaleWantFood;
   public boolean femaleWantStone;
   public int     femaleNestSequence;

   // Random numbers.
   public static int RANDOM_NUMBER_SEED = 4517;
   public Random     randomizer;

   // Construct environment.
   Environment()
   {
      randomizer = new Random(RANDOM_NUMBER_SEED);

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
      female        = new FemaleBird();
      female.x      = width / 2;
      female.y      = height / 2;
      female.food   = 0;
      int[] sensors = new int[FemaleBird.NUM_SENSORS];
      sensors[Bird.LOCALE_SENSOR] = world[female.x][female.y].locale;
      sensors[Bird.OBJECT_SENSOR] = world[female.x][female.y].object;
      female.setSensors(sensors);
      female.response                     = RESPONSE.DO_NOTHING;
      male                                = new MaleBird();
      male.x                              = width / 2;
      male.y                              = height / 2;
      male.food                           = Bird.FOOD_DURATION;
      sensors                             = new int[MaleBird.NUM_SENSORS];
      sensors[Bird.LOCALE_SENSOR]         = world[male.x][male.y].locale;
      sensors[Bird.OBJECT_SENSOR]         = world[male.x][male.y].object;
      sensors[MaleBird.WANT_FOOD_SENSOR]  = 0;
      sensors[MaleBird.WANT_STONE_SENSOR] = 0;
      male.setSensors(sensors);
      male.response      = RESPONSE.DO_NOTHING;
      femaleWantFood     = false;
      femaleWantStone    = false;
      femaleNestSequence = 0;
   }


   // Step environment.
   public void step()
   {
      // Cycle female.
      Cell cell = world[female.x][female.y];

      int[] sensors = new int[FemaleBird.NUM_SENSORS];
      sensors[Bird.LOCALE_SENSOR] = cell.locale;
      sensors[Bird.OBJECT_SENSOR] = cell.object;
      female.setSensors(sensors);
      switch (responseDriver)
      {
      case RESPONSE_DRIVER.BIRD:
         female.cycle();
         break;

      case RESPONSE_DRIVER.AUTOPILOT:
         cycleAutopilot(Bird.FEMALE);
         break;
      }
      cycle(Bird.FEMALE);

      // Cycle male.
      cell    = world[male.x][male.y];
      sensors = new int[MaleBird.NUM_SENSORS];
      sensors[Bird.LOCALE_SENSOR]         = cell.locale;
      sensors[Bird.OBJECT_SENSOR]         = cell.object;
      sensors[MaleBird.WANT_FOOD_SENSOR]  = 0;
      sensors[MaleBird.WANT_STONE_SENSOR] = 0;
      if ((male.x == female.x) && (male.y == female.y))
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
      male.setSensors(sensors);
      switch (responseDriver)
      {
      case RESPONSE_DRIVER.BIRD:
         male.cycle();
         break;

      case RESPONSE_DRIVER.AUTOPILOT:
         cycleAutopilot(Bird.MALE);
         break;
      }
      cycle(Bird.MALE);

      // Step mice.
      stepMice();
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
      if (!getFood() && !getStone())
      {
         returnToFemale();
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
      int    mousex = -1;
      int    mousey = -1;
      double dist   = 0.0;
      for (int x = 0; x < width; x++)
      {
         for (int y = 0; y < height; y++)
         {
            if ((world[x][y].locale == LOCALE.FOREST) && (world[x][y].object == OBJECT.MOUSE))
            {
               double d = distance((double)male.x, (double)male.y, (double)x, (double)y);
               if ((mousex == -1) || (d < dist))
               {
                  mousex = x;
                  mousey = y;
                  dist   = d;
               }
            }
         }
      }
      if (mousex == -1)
      {
         male.response = Bird.RESPONSE.DO_NOTHING;
         return(true);
      }
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
      int    stonex = -1;
      int    stoney = -1;
      double dist   = 0.0;
      for (int x = 0; x < width; x++)
      {
         for (int y = 0; y < height; y++)
         {
            if ((world[x][y].locale == LOCALE.DESERT) && (world[x][y].object == OBJECT.STONE))
            {
               double d = distance((double)male.x, (double)male.y, (double)x, (double)y);
               if ((stonex == -1) || (d < dist))
               {
                  stonex = x;
                  stoney = y;
                  dist   = d;
               }
            }
         }
      }
      if (stonex == -1)
      {
         male.response = Bird.RESPONSE.DO_NOTHING;
         return(true);
      }
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
      if ((male.x == female.x) && (male.y == female.y))
      {
         male.response = Bird.RESPONSE.DO_NOTHING;
         return(true);
      }
      if (male.x < female.x)
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
      if (male.x > female.x)
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
      if (male.y < female.y)
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
      if (male.y > female.y)
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
      return(false);
   }


   // Female autopilot.
   public void femaleAutopilot()
   {
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
            female.response    = FemaleBird.RESPONSE.LAY_EGG;
            femaleNestSequence = 0;
         }
         break;
      }
   }


   // Bird sensory-response cycle.
   public void cycle(int gender)
   {
      Bird bird = male;

      if (gender == Bird.FEMALE)
      {
         bird = female;
      }
      Cell cell = world[bird.x][bird.y];

      // Clear sensors.
      if (bird.gender == Bird.MALE)
      {
         bird.sensors[MaleBird.WANT_FOOD_SENSOR]  = 0;
         bird.sensors[MaleBird.WANT_STONE_SENSOR] = 0;
      }

      // Disrupt nest to motivate female to fix it.
      if ((disruptNest == 1) && (bird.gender == Bird.FEMALE))
      {
         disruptNest++;

         // Blow stone from nest onto egg.
         if ((world[width / 2][height / 2].object == OBJECT.EGG) &&
             (world[width / 2][(height / 2) - 1].object == OBJECT.STONE))
         {
            world[width / 2][(height / 2) - 1].object = OBJECT.NO_OBJECT;
            world[width / 2][height / 2].object       = OBJECT.STONE;
         }
      }

      switch (bird.response)
      {
      case Bird.RESPONSE.DO_NOTHING:
         break;

      case Bird.RESPONSE.EAT:
         if (bird.hasObject == OBJECT.MOUSE)
         {
            bird.food      = Bird.FOOD_DURATION;
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
         Random rand = new Random();
         for (int j = 5; j < i; j++)
         {
            int x = (int)(rand.nextInt() % (long)j);
            int y = (int)(rand.nextInt() % (long)j);
            if ((x == 0) && (y == 0)) { continue; }
            if ((rand.nextInt() % 2) == 0) { x = -x; }
            if ((rand.nextInt() % 2) == 0) { y = -y; }
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

      case FemaleBird.RESPONSE.WANT_FOOD:
         if (bird == female)
         {
            if ((bird.x == male.x) && (bird.y == male.y))
            {
               male.sensors[MaleBird.WANT_FOOD_SENSOR] = 1;
            }
         }
         break;

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

      case FemaleBird.RESPONSE.WANT_STONE:
         if (bird == female)
         {
            if ((bird.x == male.x) && (bird.y == male.y))
            {
               male.sensors[MaleBird.WANT_STONE_SENSOR] = 1;
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

      case FemaleBird.RESPONSE.LAY_EGG:
         if (bird == female)
         {
            if (cell.object == OBJECT.NO_OBJECT)
            {
               cell.object = OBJECT.EGG;
               if (disruptNest == 0) { disruptNest = 1; }
            }
         }
         break;
      }

      // Set sensors.
      cell = world[bird.x][bird.y];
      bird.sensors[Bird.LOCALE_SENSOR] = cell.locale;
      bird.sensors[Bird.OBJECT_SENSOR] = cell.object;

      // Digest food.
      bird.digest();
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
      for (int x = randomizer.nextInt(width), x2 = 0; x2 < width; x = (x + 1) % width, x2++)
      {
         for (int y = randomizer.nextInt(height), y2 = 0; y2 < height; y = (y + 1) % height, y2++)
         {
            if ((world[x][y].object == OBJECT.MOUSE) &&
                ((male.x != x) || (male.y != y) || (male.response != Bird.RESPONSE.GET)) &&
                (randomizer.nextDouble() < MOUSE_MOVE_PROBABILITY))
            {
               boolean move = false;
               for (int i = randomizer.nextInt(4), i2 = 0; i2 < 4 && !move; i = (i + 1) % 4, i2++)
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
      // Get options.
      if (args.length == 2)
      {
         if (args[0].equals("-foodDuration"))
         {
            try
            {
               Bird.FOOD_DURATION = Integer.parseInt(args[1]);
            }
            catch (Exception e)
            {
               System.err.println("Invalid food duration: " + args[1]);
               System.exit(1);
            }
         }
         else
         {
            System.err.println("Usage: morphognosis.nestingbirds.Environment [-foodDuration <steps>]");
            System.exit(1);
         }
      }
      else if (args.length != 0)
      {
         System.err.println("Usage: morphognosis.nestingbirds.Environment [-foodDuration <steps>]");
         System.exit(1);
      }

      Environment environment = new Environment();

      environment.male.response   = Bird.RESPONSE.MOVE;
      environment.female.response = FemaleBird.RESPONSE.LAY_EGG;
      environment.step();
      environment.male.print();
      environment.female.print();
   }
}
