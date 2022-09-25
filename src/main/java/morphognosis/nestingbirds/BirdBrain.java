// For conditions of distribution and use, see copyright notice in Morphognosis.java

// Nesting bird brain.

package morphognosis.nestingbirds;

public class BirdBrain
{
   // Number of sensors and possible responses.
   public int numSensors;
   public int numResponses;

   // Constructor.
   public BirdBrain(int numSensors, int numResponses)
   {
      this.numSensors   = numSensors;
      this.numResponses = numResponses;
   }


   // Sensory-response cycle.
   public int cycle(float[] sensors)
   {
      return(Bird.RESPONSE.DO_NOTHING);
   }
}
