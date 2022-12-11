// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds morphognosis neural network.

package morphognosis.nestingbirds;

import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Scanner;

public class NestingBirdsNN
{
   // Gender.
   public static String GENDER = null;

   // Datasets.
   // Import: <gender>_dataset_<run number>.csv
   // Export:
   public static String NN_DATASET_FILENAME = "nestingbirds_nn_dataset.py";

   // Number of datasets.
   public static int NUM_DATASETS      = 1;
   public static int NUM_TEST_DATASETS = 0;

   // Dataset content.
   public     ArrayList < ArrayList < int[] >> trainSensors;
   public     ArrayList < ArrayList < int[] >> trainResponses;
   public     ArrayList < ArrayList < int[] >> testSensors;
   public     ArrayList < ArrayList < int[] >> testResponses;
   public int sensorSize   = -1;
   public int responseSize = -1;

   // Event dilation.
   public static final int NO_DILATION         = 0;
   public static final int OVERLAY_DILATION    = 1;
   public static final int ACCUMULATE_DILATION = 2;
   public static final int NORMALIZE_DILATION  = 3;
   public static int       DILATE_EVENTS       = NO_DILATION;

   // Context length of sensory-response events.
   // e.g. 10 means that the NN can sense and respond to a sequence of 10 events.
   public static int CONTEXT_LENGTH = 100;

   // Interval span multiplier.
   // e.g. 2 means double interval temporal span and halve number of intervals.
   public static int INTERVAL_SPAN_MULTIPLIER = 1;

   // Interval span skew.
   // Skews interval spans into past, larger intervals contain more events.
   // 0.0: intervals are unskewed and equal size.
   // > 0.0 <= 1.0: intervals in the past are greater spans and have greater event capacity.
   public static float INTERVAL_SPAN_SKEW = 0.0f;

   // mapIntervals() output.
   public int   numIntervals;
   public int[] intervalMap;

   // Random numbers.
   public static int RANDOM_SEED = 4517;
   public Random     randomizer  = null;

   // Usage.
   public static final String Usage =
      "Usage:\n" +
      "    java morphognosis.nestingbirds.NestingBirdsNN\n" +
      "        -gender <\"male or female\">\n" +
      "        [-numDatasets <number> (default=" + NUM_DATASETS + ")]\n" +
      "        [-numTestDatasets <number> (default=" + NUM_TEST_DATASETS + ")]\n" +
      "        [-contextLength <length> (sensory-response event sequence length, default=" + CONTEXT_LENGTH + ")]\n" +
      "        [-dilateEvents <\"overlay\" | \"accumulate\" | \"normalize\"> (stretch events over time)\n" +
      "            [-consolidateIntervals <multiplier> (consolidate time interval spans, default=" + INTERVAL_SPAN_MULTIPLIER + ")]\n" +
      "            [-skewIntervals <skew> (skew interval spans into past, larger intervals contain more events, default=" + INTERVAL_SPAN_SKEW + ")]]\n" +
      "        [-randomSeed <seed> (default=" + RANDOM_SEED + ")]\n" +
      "Exit codes:\n" +
      "  0=success\n" +
      "  1=error";

   // Main.
   public static void main(String[] args)
   {
      boolean gotDilate      = false;
      boolean gotConsolidate = false;
      boolean gotSkew        = false;

      for (int i = 0; i < args.length; i++)
      {
         if (args[i].equals("-gender"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid gender option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (args[i].equals("male"))
            {
               GENDER = "male";
            }
            else if (args[i].equals("female"))
            {
               GENDER = "female";
            }
            else
            {
               System.err.println("Invalid gender option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-numDatasets"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid numDatasets option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               NUM_DATASETS = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid numDatasets option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (NUM_DATASETS <= 0)
            {
               System.err.println("Invalid numDatasets option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-numTestDatasets"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid numTestDatasets option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               NUM_TEST_DATASETS = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid numTestDatasets option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (NUM_TEST_DATASETS < 0)
            {
               System.err.println("Invalid numTestDatasets option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-contextLength"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid contextLength option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               CONTEXT_LENGTH = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid contextLength option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (CONTEXT_LENGTH < 0)
            {
               System.err.println("Invalid contextLength option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-dilateEvents"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid dilateEvents option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (args[i].equals("overlay"))
            {
               DILATE_EVENTS = OVERLAY_DILATION;
            }
            else if (args[i].equals("accumulate"))
            {
               DILATE_EVENTS = ACCUMULATE_DILATION;
            }
            else if (args[i].equals("normalize"))
            {
               DILATE_EVENTS = NORMALIZE_DILATION;
            }
            else
            {
               System.err.println("Invalid dilateEvents option");
               System.err.println(Usage);
               System.exit(1);
            }
            gotDilate = true;
            continue;
         }
         if (args[i].equals("-consolidateIntervals"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid consolidateIntervals option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               INTERVAL_SPAN_MULTIPLIER = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid consolidateIntervals option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (INTERVAL_SPAN_MULTIPLIER < 1)
            {
               System.err.println("Invalid consolidateIntervals option");
               System.err.println(Usage);
               System.exit(1);
            }
            gotConsolidate = true;
            continue;
         }
         if (args[i].equals("-skewIntervals"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid skewIntervals option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               INTERVAL_SPAN_SKEW = Float.parseFloat(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid skewIntervals option");
               System.err.println(Usage);
               System.exit(1);
            }
            if ((INTERVAL_SPAN_SKEW < 0.0f) || (INTERVAL_SPAN_SKEW > 1.0f))
            {
               System.err.println("Invalid skewIntervals option");
               System.err.println(Usage);
               System.exit(1);
            }
            gotSkew = true;
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
               RANDOM_SEED = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid randomSeed option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-help"))
         {
            System.out.println(Usage);
            System.exit(0);
         }
         System.err.println("Invalid option: " + args[i]);
         System.err.println(Usage);
         System.exit(1);
      }

      // Validate options.
      if (GENDER == null)
      {
         System.err.println(Usage);
         System.exit(1);
      }
      if (NUM_TEST_DATASETS >= NUM_DATASETS)
      {
         System.err.println("Number of datasets must be greater than number of test datasets");
         System.err.println(Usage);
         System.exit(1);
      }
      if (gotConsolidate && (!gotDilate || (DILATE_EVENTS == NO_DILATION)))
      {
         System.err.println("Incompatible dilateEvents and consolidateIntervals options");
         System.err.println(Usage);
         System.exit(1);
      }
      if (gotSkew && (!gotDilate || (DILATE_EVENTS == NO_DILATION)))
      {
         System.err.println("Incompatible dilateEvents and skewIntervals options");
         System.err.println(Usage);
         System.exit(1);
      }

      // Create NN.
      NestingBirdsNN nestingBirdsNN = new NestingBirdsNN();

      // Import CSV dataset.
      nestingBirdsNN.importCSVdatasets();

      // Export NN dataset.
      nestingBirdsNN.exportNNdatasetPy();

      // Run NN.
      nestingBirdsNN.run();

      System.exit(0);
   }


   // Constructor.
   public NestingBirdsNN()
   {
      // Initialize random numbers.
      randomizer = new Random(RANDOM_SEED);
   }


   // Import CSV datasets.
   public void importCSVdatasets()
   {
      trainSensors   = new ArrayList < ArrayList < int[] >> ();
      trainResponses = new ArrayList < ArrayList < int[] >> ();
      int numTrain = NUM_DATASETS - NUM_TEST_DATASETS;
      for (int i = 0; i < numTrain; i++)
      {
         trainSensors.add(new ArrayList<int[]>());
         trainResponses.add(new ArrayList<int[]>());
      }
      testSensors   = new ArrayList < ArrayList < int[] >> ();
      testResponses = new ArrayList < ArrayList < int[] >> ();
      for (int i = 0; i < NUM_TEST_DATASETS; i++)
      {
         testSensors.add(new ArrayList<int[]>());
         testResponses.add(new ArrayList<int[]>());
      }
      for (int i = 0; i < NUM_DATASETS; i++)
      {
         boolean header = true;

         String filename = GENDER + "_dataset_" + i + ".csv";
         try
         {
            Scanner reader = new Scanner(new File(filename));
            while (reader.hasNextLine())
            {
               String line = reader.nextLine();
               if (header)
               {
                  header = false;
                  if (sensorSize == -1)
                  {
                     String[] cols = line.split(",");
                     for (int j = 0; j < cols.length; j++)
                     {
                        if (cols[j].equals("do_nothing"))
                        {
                           sensorSize   = j;
                           responseSize = cols.length - sensorSize;
                           break;
                        }
                     }
                  }
               }
               else
               {
                  String[] cols   = line.split(",");
                  int[] sensors   = new int[sensorSize];
                  int[] responses = new int[responseSize];
                  for (int j = 0; j < cols.length; j++)
                  {
                     int k = Integer.parseInt(cols[j]);
                     if (j < sensorSize)
                     {
                        sensors[j] = k;
                     }
                     else
                     {
                        responses[j - sensorSize] = k;
                     }
                  }
                  if (i < numTrain)
                  {
                     trainSensors.get(i).add(sensors);
                     trainResponses.get(i).add(responses);
                  }
                  else
                  {
                     testSensors.get(i).add(sensors);
                     testResponses.get(i).add(responses);
                  }
               }
            }
            reader.close();
         }
         catch (Exception e)
         {
            System.err.println("Cannot open dataset file " + filename + ":" + e.getMessage());
            System.exit(1);
         }
      }
   }


   // Export dataset as python.
   public void exportNNdatasetPy()
   {
      try
      {
         FileWriter  fileWriter  = new FileWriter(NN_DATASET_FILENAME);
         PrintWriter printWriter = new PrintWriter(fileWriter);
         int         numTrain    = 0;
         for (int i = 0; i < trainSensors.size(); i++)
         {
            numTrain += trainSensors.get(i).size();
         }
         printWriter.println("X_train_shape = [ " + numTrain + ", " + (numIntervals * sensorSize) + " ]");
         printWriter.print("X_train = [ ");
         String X_train = "";
         mapIntervals();
         for (int i = 0; i < trainSensors.size(); i++)
         {
            ArrayList<int[]> sequence = trainSensors.get(i);
            for (int j = 0; j < sequence.size(); j++)
            {
               ArrayList<int[]> reverseSequence = new ArrayList<int[]>();
               for (int k = j; (j - k) < CONTEXT_LENGTH && k >= 0; k--)
               {
                  reverseSequence.add(sequence.get(k));
               }
               ArrayList < ArrayList < int[] >> intervalSequence = new ArrayList < ArrayList < int[] >> ();
               for (int k = 0; k < numIntervals; k++)
               {
                  intervalSequence.add(new ArrayList<int[]>());
               }
               for (int k = 0; k < reverseSequence.size(); k++)
               {
                  intervalSequence.get(intervalMap[k]).add(reverseSequence.get(k));
               }
               X_train += multiHot(intervalSequence);
            }
         }
         if (X_train.endsWith(","))
         {
            X_train = X_train.substring(0, X_train.length() - 1);
         }
         printWriter.println(X_train + " ]");
         printWriter.println("y_train_shape = [ " + numTrain + ", " + responseSize + " ]");
         printWriter.print("y_train = [ ");
         String y_train = "";
         for (int i = 0; i < trainResponses.size(); i++)
         {
            ArrayList<int[]> sequence = trainResponses.get(i);
            for (int j = 0; j < sequence.size(); j++)
            {
               int[] response = sequence.get(j);
               for (int k = 0; k < responseSize; k++)
               {
                  y_train += response[k] + ",";
               }
            }
         }
         if (y_train.endsWith(","))
         {
            y_train = y_train.substring(0, y_train.length() - 1);
         }
         printWriter.println(y_train + " ]");
         printWriter.close();
      }
      catch (IOException e)
      {
         System.err.println("Cannot write NN dataset to file " + NN_DATASET_FILENAME);
         System.exit(1);
      }
   }


   // Multi-hot coding of sensors.
   public String multiHot(ArrayList < ArrayList < int[] >> intervalSequence)
   {
      String code = "";

      for (int i = 0; i < numIntervals; i++)
      {
         float[] sensorValues = new float[sensorSize];
         for (int j = 0; j < sensorSize; j++)
         {
            sensorValues[j] = 0.0f;
         }
         ArrayList<int[]> sensorList = intervalSequence.get(i);
         for (int j = 0; j < sensorList.size(); j++)
         {
            int[] sensors = sensorList.get(j);
            for (int k = 0; k < sensorSize; k++)
            {
               sensorValues[k] += (float)sensors[k];
            }
         }
         float accum = 0.0f;
         for (int j = 0; j < sensorSize; j++)
         {
            if (DILATE_EVENTS == OVERLAY_DILATION)
            {
               if (sensorValues[i] > 1.0f)
               {
                  sensorValues[i] = 1.0f;
               }
            }
            else if (DILATE_EVENTS == NORMALIZE_DILATION)
            {
               accum += sensorValues[j];
            }
         }
         if ((DILATE_EVENTS == NORMALIZE_DILATION) && (accum > 0.0f))
         {
            for (int j = 0; j < sensorSize; j++)
            {
               sensorValues[j] /= accum;
            }
         }
         for (int j = 0; j < sensorSize; j++)
         {
            code += sensorValues[j] + ",";
         }
      }
      return(code);
   }


   // Run NN.
   public void run()
   {
   }


   // Interval maps.
   public int   numConsolidatedIntervals;
   public int[] consolidatedIntervalMap;
   public int   numSkewedIntervals;
   public int[] skewedIntervalMap;

   // Map intervals.
   public void mapIntervals()
   {
      consolidateIntervals();
      skewIntervals();

      // Combine maps.
      numIntervals = numSkewedIntervals;
      intervalMap  = new int[CONTEXT_LENGTH];
      for (int i = 0; i < CONTEXT_LENGTH; i++)
      {
         intervalMap[i] = skewedIntervalMap[consolidatedIntervalMap[i]];
      }
   }


   // Consolidate intervals.
   public void consolidateIntervals()
   {
      numConsolidatedIntervals = CONTEXT_LENGTH / INTERVAL_SPAN_MULTIPLIER;
      if (CONTEXT_LENGTH % INTERVAL_SPAN_MULTIPLIER != 0)
      {
         numConsolidatedIntervals++;
      }
      consolidatedIntervalMap = new int[CONTEXT_LENGTH];
      for (int i = 0; i < CONTEXT_LENGTH; i++)
      {
         consolidatedIntervalMap[i] = i / INTERVAL_SPAN_MULTIPLIER;
      }
   }


   // Skew intervals.
   public void skewIntervals()
   {
      float[] intervalSizes = new float[numConsolidatedIntervals];
      for (int i = 0; i < numConsolidatedIntervals; i++)
      {
         intervalSizes[i] = 1.0f;
      }
      float pathVal = (float)numConsolidatedIntervals - 1.0f;
      for (int i = 0, j = numConsolidatedIntervals - 1; i < j; i++)
      {
         float d = pathVal * INTERVAL_SPAN_SKEW;
         intervalSizes[i] += d;
         d      /= (float)(numConsolidatedIntervals - (i + 1));
         pathVal = 0.0f;
         for (int n = i + 1, k = n; k < numConsolidatedIntervals; k++)
         {
            intervalSizes[k] -= d;
            if (k > n) { pathVal += intervalSizes[k]; }
         }
      }
      int[] intervalShift = new int[numConsolidatedIntervals];
      ArrayList < ArrayList < Integer >> intervalMapWrk = new ArrayList < ArrayList < Integer >> ();
      for (int i = 0; i < numConsolidatedIntervals; i++)
      {
         intervalShift[i] = 1;
         ArrayList<Integer> intervals = new ArrayList<Integer>();
         intervals.add(i);
         intervalMapWrk.add(intervals);
      }
      for (int i = 0, j = numConsolidatedIntervals - 1; i < j; i++)
      {
         while (intervalSizes[i] - (float)intervalShift[i] > 1.0f && intervalShift[i + 1] == 1.0f)
         {
            for (int k = i + 1; k < numConsolidatedIntervals; k++)
            {
               if (intervalShift[k] == 1)
               {
                  intervalShift[k - 1] += 1;
                  intervalShift[k]      = 0;
                  intervalMapWrk.get(k - 1).add(intervalMapWrk.get(k).get(0));
                  intervalMapWrk.get(k).clear();
               }
            }
         }
      }
      numSkewedIntervals = 0;
      for (int i = 0; i < numConsolidatedIntervals; i++)
      {
         if (intervalShift[i] != 0) { numSkewedIntervals++; }
      }
      skewedIntervalMap = new int[numConsolidatedIntervals];
      for (int i = 0; i < numConsolidatedIntervals; i++)
      {
         for (int j : intervalMapWrk.get(i))
         {
            skewedIntervalMap[j] = i;
         }
      }
   }
}
