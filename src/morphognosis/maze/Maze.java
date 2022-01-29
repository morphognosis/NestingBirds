// For conditions of distribution and use, see copyright notice in Main.java

// Maze.

package morphognosis.maze;

import java.util.ArrayList;

public class Maze
{
   // Sensor sequence.
   public ArrayList<float[]> sensorSequence;

   // Response sequence.
   public ArrayList<Integer> responseSequence;

   // Constructor.
   public Maze()
   {
      sensorSequence   = new ArrayList<float[]>();
      responseSequence = new ArrayList<Integer>();
   }


   // Add sensors to sequence.
   public void addSensors(float[] sensors)
   {
      sensorSequence.add(sensors);
   }


   // Add response to sequence.
   public void addResponse(int response)
   {
      responseSequence.add(response);
   }


   // Get sensors.
   public float[] getSensors(int index)
   {
      if (index < sensorSequence.size())
      {
         return(sensorSequence.get(index));
      }
      else
      {
         return(null);
      }
   }


   // Get response.
   public int getResponse(int index)
   {
      if (index < responseSequence.size())
      {
         return(responseSequence.get(index));
      }
      else
      {
         return(-1);
      }
   }


   // Print.
   public void print()
   {
      for (int i = 0, j = sensorSequence.size(); i < j; i++)
      {
         float[] sensors = sensorSequence.get(i);
         for (float sensor : sensors)
         {
            System.out.print(sensor + " ");
         }
         int response = responseSequence.get(i);
         System.out.println("/ " + response);
      }
   }
}
