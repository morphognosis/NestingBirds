// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds morphognosis neural network.

package morphognosis.nestingbirds;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;
import org.json.JSONException;
import org.json.JSONObject;

public class NestingBirdsNN
{
   // Gender.
   public static String GENDER = null;

   // Datasets.
   // Import: <gender>_dataset_<run number>.csv
   // Export:
   public static String NN_DATASET_FILENAME = "nestingbirds_nn_dataset.py";
   public static String NN_FILENAME         = "nestingbirds_nn.py";
   public static String NN_RESULTS_FILENAME = "nestingbirds_nn_results.json";

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

   // Event aggregation.
   public static final int NO_AGGREGATION         = 0;
   public static final int MEAN_AGGREGATION       = 1;
   public static final int ROUND_AGGREGATION      = 2;
   public static final int ACCUMULATE_AGGREGATION = 3;
   public static final int NORMALIZE_AGGREGATION  = 4;
   public static int       AGGREGATE_EVENTS       = NO_AGGREGATION;

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

   // Network parameters.
   public static int NUM_NEURONS = 128;
   public static int NUM_EPOCHS  = 500;

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
      "        [-aggregateEvents <\"mean\" | \"round\" | \"accumulate\" | \"normalize\"> (aggregate events over time, default=no aggregation)\n" +
      "            [-consolidateIntervals <multiplier> (consolidate time interval spans, default=" + INTERVAL_SPAN_MULTIPLIER + ")]\n" +
      "            [-skewIntervals <skew> (skew interval spans into past, larger intervals contain more events, default=" + INTERVAL_SPAN_SKEW + ")]]\n" +
      "        [-numNeurons <number> (default=" + NUM_NEURONS + ")]\n" +
      "        [-numEpochs <number> (default=" + NUM_EPOCHS + ")]\n" +
      "        [-randomSeed <seed> (default=" + RANDOM_SEED + ")]\n" +
      "Exit codes:\n" +
      "  0=success\n" +
      "  1=error";

   // Main.
   public static void main(String[] args)
   {
      boolean gotAggregate   = false;
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
         if (args[i].equals("-aggregateEvents"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid aggregateEvents option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (args[i].equals("mean"))
            {
               AGGREGATE_EVENTS = MEAN_AGGREGATION;
            }
            else if (args[i].equals("round"))
            {
               AGGREGATE_EVENTS = ROUND_AGGREGATION;
            }
            else if (args[i].equals("accumulate"))
            {
               AGGREGATE_EVENTS = ACCUMULATE_AGGREGATION;
            }
            else if (args[i].equals("normalize"))
            {
               AGGREGATE_EVENTS = NORMALIZE_AGGREGATION;
            }
            else
            {
               System.err.println("Invalid aggregateEvents option");
               System.err.println(Usage);
               System.exit(1);
            }
            gotAggregate = true;
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
         if (args[i].equals("-numNeurons"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid numNeurons option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               NUM_NEURONS = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid numNeurons option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (NUM_NEURONS < 0)
            {
               System.err.println("Invalid numNeurons option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-numEpochs"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid numEpochs option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               NUM_EPOCHS = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid numEpochs option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (NUM_EPOCHS < 0)
            {
               System.err.println("Invalid numEpochs option");
               System.err.println(Usage);
               System.exit(1);
            }
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
      if (gotConsolidate && (!gotAggregate || (AGGREGATE_EVENTS == NO_AGGREGATION)))
      {
         System.err.println("Incompatible aggregateEvents and consolidateIntervals options");
         System.err.println(Usage);
         System.exit(1);
      }
      if (gotSkew && (!gotAggregate || (AGGREGATE_EVENTS == NO_AGGREGATION)))
      {
         System.err.println("Incompatible aggregateEvents and skewIntervals options");
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
                     testSensors.get(i - numTrain).add(sensors);
                     testResponses.get(i - numTrain).add(responses);
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
         mapIntervals();
         printWriter.println("X_train_shape = [ " + numTrain + ", " + (numIntervals * sensorSize) + " ]");
         printWriter.print("X_train = [ ");
         String X_train = "";
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
               X_train += encodeInterval(intervalSequence);
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
         int numTest = 0;
         for (int i = 0; i < testSensors.size(); i++)
         {
            numTest += testSensors.get(i).size();
         }
         printWriter.println("X_test_shape = [ " + numTest + ", " + (numIntervals * sensorSize) + " ]");
         printWriter.print("X_test = [ ");
         String X_test = "";
         for (int i = 0; i < testSensors.size(); i++)
         {
            ArrayList<int[]> sequence = testSensors.get(i);
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
               X_test += encodeInterval(intervalSequence);
            }
         }
         if (X_test.endsWith(","))
         {
            X_test = X_test.substring(0, X_test.length() - 1);
         }
         printWriter.println(X_test + " ]");
         printWriter.println("y_test_shape = [ " + numTest + ", " + responseSize + " ]");
         printWriter.print("y_test = [ ");
         String y_test = "";
         for (int i = 0; i < testResponses.size(); i++)
         {
            ArrayList<int[]> sequence = testResponses.get(i);
            for (int j = 0; j < sequence.size(); j++)
            {
               int[] response = sequence.get(j);
               for (int k = 0; k < responseSize; k++)
               {
                  y_test += response[k] + ",";
               }
            }
         }
         if (y_test.endsWith(","))
         {
            y_test = y_test.substring(0, y_test.length() - 1);
         }
         printWriter.println(y_test + " ]");
         printWriter.close();
      }
      catch (IOException e)
      {
         System.err.println("Cannot write NN dataset to file " + NN_DATASET_FILENAME);
         System.exit(1);
      }
   }


   // Encode interval.
   public String encodeInterval(ArrayList < ArrayList < int[] >> intervalSequence)
   {
      String code = "";

      for (int i = 0; i < numIntervals; i++)
      {
         float[] sensorValues = new float[sensorSize];
         int[] counts         = new int[sensorSize];
         for (int j = 0; j < sensorSize; j++)
         {
            sensorValues[j] = 0.0f;
            counts[j]       = 0;
         }
         ArrayList<int[]> sensorList = intervalSequence.get(i);
         for (int j = 0; j < sensorList.size(); j++)
         {
            int[] sensors = sensorList.get(j);
            for (int k = 0; k < sensorSize; k++)
            {
               sensorValues[k] += (float)sensors[k];
               counts[k]++;
            }
         }
         float accum = 0.0f;
         for (int j = 0; j < sensorSize; j++)
         {
            if (AGGREGATE_EVENTS == MEAN_AGGREGATION)
            {
               if (counts[j] > 0)
               {
                  sensorValues[j] /= (float)counts[j];
               }
            }
            else if (AGGREGATE_EVENTS == ROUND_AGGREGATION)
            {
               if (sensorValues[j] > 1.0f)
               {
                  sensorValues[j] = 1.0f;
               }
            }
            else if (AGGREGATE_EVENTS == NORMALIZE_AGGREGATION)
            {
               accum += sensorValues[j];
            }
         }
         if ((AGGREGATE_EVENTS == NORMALIZE_AGGREGATION) && (accum > 0.0f))
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
      // Make prediction.
      try
      {
         InputStream in = ClassLoader.getSystemClassLoader().getResourceAsStream(NN_FILENAME);
         if (in == null)
         {
            System.err.println("Cannot access " + NN_FILENAME);
            System.exit(1);
         }
         File             pythonScript = new File(NN_FILENAME);
         FileOutputStream out          = new FileOutputStream(pythonScript);
         byte[] buffer = new byte[1024];
         int bytesRead;
         while ((bytesRead = in.read(buffer)) != -1)
         {
            out.write(buffer, 0, bytesRead);
         }
         out.close();
      }
      catch (Exception e)
      {
         System.err.println("Cannot create " + NN_FILENAME);
         System.exit(1);
      }
      new File(NN_RESULTS_FILENAME).delete();
      ProcessBuilder processBuilder = new ProcessBuilder("python", NN_FILENAME,
                                                         "--neurons", (NUM_NEURONS + ""),
                                                         "--epochs", (NUM_EPOCHS + "")
                                                         );
      processBuilder.inheritIO();
      Process process;
      try
      {
         process = processBuilder.start();
         process.waitFor();
      }
      catch (InterruptedException e) {}
      catch (IOException e)
      {
         System.err.println("Cannot run " + NN_FILENAME + ":" + e.getMessage());
         System.exit(1);
      }

      // Fetch the results.
      try
      {
         BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(NN_RESULTS_FILENAME)));
         String         json;
         if ((json = br.readLine()) != null)
         {
            JSONObject jObj = null;
            try
            {
               jObj = new JSONObject(json);
            }
            catch (JSONException e)
            {
               System.err.println("Error parsing results file " + NN_RESULTS_FILENAME);
               System.exit(1);
            }
            String train_prediction_errors = jObj.getString("train_prediction_errors");
            if ((train_prediction_errors == null) || train_prediction_errors.isEmpty())
            {
               System.err.println("Error parsing results file " + NN_RESULTS_FILENAME);
               System.exit(1);
            }
            String train_total_predictions = jObj.getString("train_total_predictions");
            if ((train_total_predictions == null) || train_total_predictions.isEmpty())
            {
               System.err.println("Error parsing results file " + NN_RESULTS_FILENAME);
               System.exit(1);
            }
            String train_error_pct = jObj.getString("train_error_pct");
            if ((train_error_pct == null) || train_error_pct.isEmpty())
            {
               System.err.println("Error parsing results file " + NN_RESULTS_FILENAME);
               System.exit(1);
            }
            String test_prediction_errors = jObj.getString("test_prediction_errors");
            if ((test_prediction_errors == null) || test_prediction_errors.isEmpty())
            {
               System.err.println("Error parsing results file " + NN_RESULTS_FILENAME);
               System.exit(1);
            }
            String test_total_predictions = jObj.getString("test_total_predictions");
            if ((test_total_predictions == null) || test_total_predictions.isEmpty())
            {
               System.err.println("Error parsing results file " + NN_RESULTS_FILENAME);
               System.exit(1);
            }
            String test_error_pct = jObj.getString("test_error_pct");
            if ((test_error_pct == null) || test_error_pct.isEmpty())
            {
               System.err.println("Error parsing results file " + NN_RESULTS_FILENAME);
               System.exit(1);
            }
         }
         else
         {
            System.err.println("Cannot read results file " + NN_RESULTS_FILENAME);
         }
         br.close();
      }
      catch (Exception e)
      {
         System.err.println("Cannot read results file " + NN_RESULTS_FILENAME + ":" + e.getMessage());
      }
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


   // Print interval map.
   public void printInterMap()
   {
      System.out.println("interval map:");
      for (int i = 0; i < intervalMap.length; i++)
      {
         System.out.println(i + "->" + intervalMap[i]);
      }
   }
}
