// For conditions of distribution and use, see copyright notice in Morphognosis.java

package morphognosis;

// Orientation.
public class Orientation
{
   public static final int NORTH            = 0;
   public static final int NORTHEAST        = 1;
   public static final int EAST             = 2;
   public static final int SOUTHEAST        = 3;
   public static final int SOUTH            = 4;
   public static final int SOUTHWEST        = 5;
   public static final int WEST             = 6;
   public static final int NORTHWEST        = 7;
   public static final int NUM_ORIENTATIONS = 8;

   public static String toName(int orientation)
   {
      switch (orientation)
      {
      case NORTH:
         return("north");

      case NORTHEAST:
         return("northeast");

      case EAST:
         return("east");

      case SOUTHEAST:
         return("southeast");

      case SOUTH:
         return("south");

      case SOUTHWEST:
         return("southwest");

      case WEST:
         return("west");

      case NORTHWEST:
         return("northwest");
      }
      return("unknown");
   }
}
