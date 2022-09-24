// For conditions of distribution and use, see copyright notice in Morphognosis.java

// The nesting birds environment.

package morphognosis.nestingbirds;

import java.util.Random;

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
   public Bird male;
   public Bird female;

   // Disrupt nest.
   private int disruptNest = 0;

   // Construct environment.
   Environment()
   {
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
      male      = new MaleBird();
      male.x    = width / 2;
      male.y    = height / 2;
      male.food = Bird.FOOD_DURATION;
      male.sensors[Bird.LOCALE_SENSOR] = world[male.x][male.y].locale;
      male.sensors[Bird.OBJECT_SENSOR] = world[male.x][male.y].object;
      female      = new FemaleBird();
      female.x    = width / 2;
      female.y    = height / 2;
      female.food = 0;
      female.sensors[Bird.LOCALE_SENSOR] = world[female.x][female.y].locale;
      female.sensors[Bird.OBJECT_SENSOR] = world[female.x][female.y].object;
   }


   // Step environment.
   public void step()
   {
      cycle(Bird.FEMALE);
      cycle(Bird.MALE);
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
      if ((disruptNest == 1) && (bird.gender == Bird.FEMALE)) { disruptNest++; }
      if ((disruptNest == 2) && (bird.gender == Bird.FEMALE))
      {
         disruptNest++;

         // Remove stone from nest and drop on egg.
         world[bird.x][bird.y - 1].object = OBJECT.NO_OBJECT;
         cell.object = OBJECT.STONE;
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
            bird.hasObject = OBJECT.NO_OBJECT;                      // vaporize
         }
         break;

      case Bird.RESPONSE.MOVE:
         switch (bird.orientation)
         {
         case Bird.ORIENTATION.NORTH:
            if (bird.y < height - 1)
            {
               bird.y++;
            }
            break;

         case Bird.ORIENTATION.SOUTH:
            if (bird.y > 0)
            {
               bird.y--;
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

      case Bird.RESPONSE.TURN:
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


   // Main.
   public static void main(String[] args)
   {
      Environment environment = new Environment();

      environment.male.response   = Bird.RESPONSE.MOVE;
      environment.female.response = FemaleBird.RESPONSE.LAY_EGG;
      environment.step();
      environment.male.print();
      environment.female.print();
   }
}
