// For conditions of distribution and use, see copyright notice in Main.java

// Mouse: morphognosis organism.

package morphognosis.maze;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Random;

import morphognosis.Metamorph;
import morphognosis.Morphognostic;
import morphognosis.Orientation;
import morphognosis.Utility;
import morphognosis.Morphognostic.Neighborhood;

public class Mouse
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

   // Response driver.
   public int responseDriver;

   // Morphognostics.
   public Morphognostic[] morphognostics;

   // Metamorphs.
   public int[] currentMetamorphIdxs;
   public       ArrayList<Metamorph>[] metamorphs;

   // Metamorph neural networks.
   public MetamorphNN[] metamorphNNs;

   // Metamorph dataset file name.
   public static final String METAMORPH_DATASET_FILE_NAME = "metamorphs.csv";

   // Maximum distance between equivalent morphognostics.
   public static float EQUIVALENT_MORPHOGNOSTIC_DISTANCE = 0.0f;

   // Goal-seeking parameters.
   public static final float SOLVE_MAZE_GOAL_VALUE      = 1.0f;
   public static final float GOAL_VALUE_DISCOUNT_FACTOR = 0.9f;

   /*
    * Morphognostic event.
    *
    *      Sensory values:
    *      {
    *          <room identifier>
    *          <context room marks>
    *          <maze entry marks>
    *          <maze interior marks>
    *          <context end room marks>
    *      }
    */

   // Constructor.
   @SuppressWarnings("unchecked")
   public Mouse(int numSensors, int numResponses, Random random)
   {
      NUM_SENSORS   = numSensors;
      NUM_RESPONSES = numResponses;
      WAIT_RESPONSE = NUM_RESPONSES - 1;
      this.random   = random;

      sensors = new float[NUM_SENSORS];
      for (int n = 0; n < NUM_SENSORS; n++)
      {
         sensors[n] = 0.0f;
      }
      response         = WAIT_RESPONSE;
      overrideResponse = -1;

      // Initialize Morphognostics.
      morphognostics = new Morphognostic[2];
      int eventDimensions = NUM_SENSORS;
      boolean[][] neighborhoodEventDimensionMap = new boolean[Parameters.NUM_NEIGHBORHOODS][eventDimensions];
      for (int i = 0; i < Parameters.NUM_NEIGHBORHOODS; i++)
      {
         for (int j = 0; j < eventDimensions; j++)
         {
            neighborhoodEventDimensionMap[i][j] = true;
         }
      }
      morphognostics[1] = new Morphognostic(Orientation.NORTH,
                                            eventDimensions,
                                            neighborhoodEventDimensionMap,
                                            Parameters.NUM_NEIGHBORHOODS,
                                            Parameters.NEIGHBORHOOD_DIMENSIONS,
                                            Parameters.NEIGHBORHOOD_DURATIONS);
      neighborhoodEventDimensionMap = new boolean[1][eventDimensions];
      for (int j = 0; j < eventDimensions; j++)
      {
         neighborhoodEventDimensionMap[0][j] = true;
      }
      int numNeighborhoods = 1;
      int[][] neighborhoodDimensions = { Parameters.NEIGHBORHOOD_DIMENSIONS[0] };
      int[] neighborhoodDurations    = { Parameters.NEIGHBORHOOD_DURATIONS[0] };
      morphognostics[0] = new Morphognostic(Orientation.NORTH,
                                            eventDimensions,
                                            neighborhoodEventDimensionMap,
                                            numNeighborhoods,
                                            neighborhoodDimensions,
                                            neighborhoodDurations);

      // Create metamorphs.
      currentMetamorphIdxs    = new int[2];
      currentMetamorphIdxs[0] = currentMetamorphIdxs[1] = -1;
      metamorphs    = new ArrayList[2];
      metamorphs[0] = new ArrayList<Metamorph>();
      metamorphs[1] = new ArrayList<Metamorph>();

      // Metamorph neural networks.
      metamorphNNs = new MetamorphNN[2];

      // Initialize response driver.
      responseDriver = ResponseDriver.TRAINING_OVERRIDE;
   }


   // Reset.
   void reset()
   {
      for (int i = 0; i < NUM_SENSORS; i++)
      {
         sensors[i] = 0.0f;
      }
      response         = WAIT_RESPONSE;
      overrideResponse = -1;
      morphognostics[0].clear();
      morphognostics[1].clear();
      currentMetamorphIdxs[0] = currentMetamorphIdxs[1] = -1;
   }


   // Save mouse to file.
   public void save(String filename) throws IOException
   {
      DataOutputStream writer;

      try
      {
         writer = new DataOutputStream(new BufferedOutputStream(new FileOutputStream(new File(filename))));
      }
      catch (Exception e)
      {
         throw new IOException("Cannot open output file " + filename + ":" + e.getMessage());
      }
      save(writer);
      writer.close();
   }


   // Save mouse.
   public void save(DataOutputStream writer) throws IOException
   {
      morphognostics[0].save(writer);
      morphognostics[1].save(writer);
      Utility.saveInt(writer, responseDriver);
      writer.flush();
   }


   // Load mouse from file.
   public void load(String filename) throws IOException
   {
      DataInputStream reader;

      try
      {
         reader = new DataInputStream(new BufferedInputStream(new FileInputStream(new File(filename))));
      }
      catch (Exception e)
      {
         throw new IOException("Cannot open input file " + filename + ":" + e.getMessage());
      }
      load(reader);
      reader.close();
   }


   // Load mouse.
   public void load(DataInputStream reader) throws IOException
   {
      morphognostics[0] = Morphognostic.load(reader);
      morphognostics[1] = Morphognostic.load(reader);
      responseDriver    = Utility.loadInt(reader);
   }


   // Sense/response cycle.
   public int cycle(float[] sensors)
   {
      for (int i = 0; i < NUM_SENSORS; i++)
      {
         this.sensors[i] = sensors[i];
      }

      // Update morphognostics.
      updateMorphognostics();

      // Respond.
      switch (responseDriver)
      {
      case ResponseDriver.TRAINING_OVERRIDE:
         response = overrideResponse;
         break;

      case ResponseDriver.METAMORPH_DB:
         metamorphDBresponse();
         break;

      case ResponseDriver.METAMORPH_NN:
         metamorphNNresponse();
         break;
      }

      // Update metamorphs if training.
      if (responseDriver == ResponseDriver.TRAINING_OVERRIDE)
      {
         updateMetamorphs(morphognostics, response, goalValue(sensors, response));
      }

      return(response);
   }


   // Determine sensory-response goal value.
   public float goalValue(float[] sensors, int response)
   {
      return(0.0f);
   }


   // Update morphognostics.
   public void updateMorphognostics()
   {
      morphognostics[0].update(sensors, 0, 0);
      morphognostics[1].update(sensors, 0, 0);
   }


   // Get metamorph DB response.
   public void metamorphDBresponse()
   {
      Metamorph metamorph = null;
      float     dist      = 0.0f;
      float     d2;
      int       l = 0;

      for (int i = 0; i < metamorphs.length; i++)
      {
         for (Metamorph m : metamorphs[i])
         {
            if (!m.ambiguous)
            {
               d2 = morphognostics[i].compare(m.morphognostic);
               if ((metamorph == null) || (d2 < dist))
               {
                  dist      = d2;
                  metamorph = m;
                  l         = i;
               }
               else
               {
                  if (d2 == dist)
                  {
                     if (l < i)
                     {
                        metamorph = m;
                        l         = i;
                     }
                     else
                     {
                        if (random.nextBoolean())
                        {
                           metamorph = m;
                        }
                     }
                  }
               }
            }
         }
      }
      if (metamorph != null)
      {
         response = metamorph.response;
      }
      else
      {
         response = WAIT_RESPONSE;
      }
   }


   // Get metamorph neural network response.
   public void metamorphNNresponse()
   {
      if ((metamorphNNs[0] != null) && (metamorphNNs[1] != null))
      {
         double probabilities[][] = new double[2][];
         probabilities[0] = metamorphNNs[0].responseProbabilities(morphognostics[0]);
         probabilities[1] = metamorphNNs[1].responseProbabilities(morphognostics[1]);
         double p = 0.0;
         response = -1;
         for (int i = 0; i < 2; i++)
         {
            for (int j = 0; j < probabilities[i].length; j++)
            {
               if ((response == -1) || (probabilities[i][j] >= p))
               {
                  response = j;
                  p        = probabilities[i][j];
               }
            }
         }
         if (response == -1)
         {
            response = WAIT_RESPONSE;
         }
      }
      else
      {
         System.err.println("Must train metamorph neural network");
         response = WAIT_RESPONSE;
      }
   }


   // Get goal-seeking DB response.
   public void metamorphGoalSeekingDBresponse()
   {
      Metamorph metamorph    = null;
      float     minCompare   = 0.0f;
      float     maxGoalValue = 0.0f;

      for (int i = 0; i < metamorphs.length; i++)
      {
         for (Metamorph m : metamorphs[i])
         {
            if (!m.ambiguous)
            {
               float compare = m.morphognostic.compare(morphognostics[i]);
               if (metamorph == null)
               {
                  metamorph    = m;
                  minCompare   = compare;
                  maxGoalValue = metamorph.goalValue;
               }
               else if (compare < minCompare)
               {
                  metamorph    = m;
                  minCompare   = compare;
                  maxGoalValue = metamorph.goalValue;
               }
               else if (compare == minCompare)
               {
                  if (m.goalValue > maxGoalValue)
                  {
                     metamorph    = m;
                     maxGoalValue = metamorph.goalValue;
                  }
               }
            }
         }
      }
      if (metamorph != null)
      {
         response = metamorph.response;
      }
      else
      {
         response = WAIT_RESPONSE;
      }
   }


   // Update metamorphs.
   public void updateMetamorphs(Morphognostic[] morphognostics, int response, float goalValue)
   {
      for (int n = 0; n < morphognostics.length; n++)
      {
         Metamorph metamorph = new Metamorph(morphognostics[n].clone(), response,
                                             goalValue, getResponseName(response));

         metamorph.morphognostic.orientation = Orientation.NORTH;
         int foundIdx = -1;
         for (int i = 0, j = metamorphs[n].size(); i < j; i++)
         {
            Metamorph m = metamorphs[n].get(i);
            if (m.morphognostic.compare(metamorph.morphognostic) <= EQUIVALENT_MORPHOGNOSTIC_DISTANCE)
            {
               if (m.response == metamorph.response)
               {
                  foundIdx = i;
               }
               else
               {
                  m.ambiguous = metamorph.ambiguous = true;
               }
               break;
            }
         }
         if (foundIdx == -1)
         {
            metamorphs[n].add(metamorph);
            foundIdx = metamorphs[n].size() - 1;
         }
         if (currentMetamorphIdxs[n] != -1)
         {
            Metamorph currentMetamorph = metamorphs[n].get(currentMetamorphIdxs[n]);
            for (int i = 0, j = currentMetamorph.effectIndexes.size(); i < j; i++)
            {
               if (currentMetamorph.effectIndexes.get(i) == foundIdx)
               {
                  foundIdx = -1;
                  break;
               }
            }
            if (foundIdx != -1)
            {
               currentMetamorph.effectIndexes.add(foundIdx);
               metamorphs[n].get(foundIdx).causeIndexes.add(currentMetamorphIdxs[n]);

               // Propagate goal value.
               propagateGoalValue(n, currentMetamorph, metamorphs[n].get(foundIdx).goalValue);
            }
         }
         currentMetamorphIdxs[n] = foundIdx;
      }
   }


   // Propagate goal value.
   public void propagateGoalValue(int n, Metamorph metamorph, float effectGoalValue)
   {
      float v = effectGoalValue * Mouse.GOAL_VALUE_DISCOUNT_FACTOR;

      if (v > metamorph.goalValue)
      {
         metamorph.goalValue = v;
         for (int i = 0, j = metamorph.causeIndexes.size(); i < j; i++)
         {
            propagateGoalValue(n, metamorphs[n].get(metamorph.causeIndexes.get(i)), v);
         }
      }
   }


   // Train metamorph neural network.
   public void trainMetamorphNNs()
   {
      for (int i = 0; i < metamorphNNs.length; i++)
      {
         metamorphNNs[i] = new MetamorphNN(random);
         metamorphNNs[i].train(metamorphs[i]);
      }
   }


   // Save metamorph neural network.
   public void saveMetamorphNN(int n, String filename)
   {
      if (metamorphNNs[n] != null)
      {
         metamorphNNs[n].saveModel(filename);
      }
      else
      {
         System.err.println("Cannot save null metamorph neural network to file " + filename);
      }
   }


   // Load metamorph neural network.
   public void loadMetamorphNN(int n, String filename)
   {
      if (metamorphNNs[n] == null)
      {
         metamorphNNs[n] = new MetamorphNN(random);
      }
      metamorphNNs[n].loadModel(filename);
   }


   // Clear metamorphs.
   public void clearMetamorphs()
   {
      for (int i = 0; i < metamorphNNs.length; i++)
      {
         metamorphs[i].clear();
         currentMetamorphIdxs[i] = -1;
      }
   }


   // Write metamporph dataset.
   public void writeMetamorphDataset(int n, String filename) throws Exception
   {
      FileOutputStream output;

      try
      {
         output = new FileOutputStream(new File(filename));
      }
      catch (Exception e)
      {
         throw new IOException("Cannot open output file " + filename + ":" + e.getMessage());
      }
      if (metamorphs[n].size() > 0)
      {
         Morphognostic morphognostic = metamorphs[n].get(0).morphognostic;
         String        oldlinesep    = System.getProperty("line.separator");
         System.setProperty("line.separator", "\n");
         PrintWriter writer = new PrintWriter(new BufferedWriter(new OutputStreamWriter(output)));
         for (int i = 0; i < morphognostic.NUM_NEIGHBORHOODS; i++)
         {
            int l = morphognostic.neighborhoods.get(i).sectors.length;
            for (int x = 0; x < l; x++)
            {
               for (int y = 0; y < l; y++)
               {
                  for (int d = 0; d < morphognostic.eventDimensions; d++)
                  {
                     writer.print(i + "-" + x + "-" + y + "-" + d + ",");
                  }
               }
            }
         }
         writer.println("response");
         for (Metamorph m : metamorphs[n])
         {
            writer.println(morphognostic2csv(m.morphognostic) + "," + m.response);
         }
         writer.flush();
         writer.close();
         System.setProperty("line.separator", oldlinesep);
      }
      output.close();
   }


   // Flatten morphognostic to csv string.
   public String morphognostic2csv(Morphognostic morphognostic)
   {
      String  output    = "";
      boolean skipComma = true;
      int     dx        = 0;

      for (int i = 0; i < morphognostic.NUM_NEIGHBORHOODS; i++)
      {
         Neighborhood neighborhood = morphognostic.neighborhoods.get(i);
         float[][] values = neighborhood.rectifySectorValues();
         int n = neighborhood.sectors.length;
         for (int j = 0, j2 = n * n; j < j2; j++)
         {
            for (int d = dx, d2 = morphognostic.eventDimensions; d < d2; d++)
            {
               if (skipComma)
               {
                  skipComma = false;
               }
               else
               {
                  output += ",";
               }
               output += (values[j][d] + "");
            }
         }
      }
      return(output);
   }


   // Get response name.
   public static String getResponseName(int response)
   {
      if (response >= 0)
      {
         if (response < WAIT_RESPONSE)
         {
            return("door " + response);
         }
         else
         {
            return("wait");
         }
      }
      else
      {
         return("invalid");
      }
   }
}
