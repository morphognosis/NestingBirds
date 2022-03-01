// For conditions of distribution and use, see copyright notice in Mandala.java

// Bird: morphognosis organism.

package morphognosis.nestingbirds;

import java.util.Random;

public class Bird
{
   // Random numbers.
   public Random random;

   // Sensors.
   public static int NUM_SENSORS = -1;
   public float[]    sensors;

   // Response.
   public static int NUM_RESPONSES = -1;
   public static int WAIT_RESPONSE = -1;
   public int        response;
   public int        overrideResponse;
}
