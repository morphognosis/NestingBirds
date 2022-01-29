/*
 * Copyright (c) 2021 Tom Portegys (portegys@gmail.com). All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY TOM PORTEGYS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Main.

package morphognosis.maze;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Random;
import javax.swing.UIManager;
import morphognosis.Morphognosis;

public class Main
{
   // Version.
   public static final String VERSION = "1.0";

   // Default random seed.
   public static final int DEFAULT_RANDOM_SEED = 4517;

   // Usage.
   public static final String Usage =
      "Usage:\n" +
      "    java morphognosis.maze.Main\n" +
      "      [-batch (batch mode)]\n" +
      "      [-responseDriver <metamorphDB | metamorphNN> (response driver, default=metamorphDB)]\n" +
      "      [-randomSeed <random number seed> (default=" + DEFAULT_RANDOM_SEED + ")]\n" +
      "      [-writeMetamorphDataset [<file name>] (default=" + Mouse.METAMORPH_DATASET_FILE_NAME + ")]\n" +
      "      Maze maker parameters:\n" +
      "        [-numDoors <quantity> (default=" + Parameters.NUM_DOORS + ")]\n" +
      "        [-mazeInteriorLength <length> (default=" + Parameters.MAZE_INTERIOR_LENGTH + ")]\n" +
      "        [-numContextMazes <quantity> (default=" + Parameters.NUM_CONTEXT_MAZES + ")]\n" +
      "        [-numIndependentMazes <quantity> (default=" + Parameters.NUM_INDEPENDENT_MAZES + ")]\n" +
      "      Morphognosis parameters:\n" +
      "        [-neighborhoodDurations <comma-separated values> (implies number of neighborhoods)]\n" +
      "      Metamorph Weka neural network parameters:\n" +
      "        [-NNlearningRate <quantity> (default=" + Parameters.NN_LEARNING_RATE + ")]\n" +
      "        [-NNmomentum <quantity> (default=" + Parameters.NN_MOMENTUM + ")]\n" +
      "        [-NNhiddenLayers <string of comma-separated numbers specifying number of neurons in each layer> (default=\"" + Parameters.NN_HIDDEN_LAYERS + "\")]\n" +
      "        [-NNtrainingTime <quantity> (default=" + Parameters.NN_TRAINING_TIME + ")]\n" +
      "  Print parameters:\n" +
      "    java morphognosis.maze.Main -printParameters\n" +
      "  Version:\n" +
      "    java morphognosis.maze.Main -version\n" +
      "Exit codes:\n" +
      "  0=success\n" +
      "  1=error";

   // Maze driver.
   public static MazeDriver mazeDriver;

   // Response driver.
   public static int responseDriver;

   // Maze dashboard.
   public static MazeDashboard mazeDashboard;

   // Random numbers.
   public static int    randomSeed = DEFAULT_RANDOM_SEED;
   public static Random random;

   // Run.
   public static void run()
   {
      random.setSeed(randomSeed);

      // Create dashboard.
      mazeDashboard = new MazeDashboard(mazeDriver);

      // Clear maze success counts.
      int trainOK = 0;
      int testOK  = 0;

      // Train mazes.
      for (int i = 0, j = mazeDriver.trainMazes.size(); i < j; i++)
      {
         mazeDriver.initTrainMaze(i, ResponseDriver.TRAINING_OVERRIDE);
         while (mazeDriver.stepMaze()) {}
      }

      // Train NN?
      if (responseDriver == ResponseDriver.METAMORPH_NN)
      {
         mazeDashboard.log("Training neural networks...");
         mazeDriver.mouse.trainMetamorphNNs();
      }

      // Validate training.
      mazeDashboard.log("Train results:");
      for (int i = 0, j = mazeDriver.trainMazes.size(); i < j; i++)
      {
         mazeDriver.initTrainMaze(i, responseDriver);
         mazeDashboard.log("Maze = " + i);
         boolean            ok        = true;
         ArrayList<Integer> responses = new ArrayList<Integer>();
         ArrayList<Integer> targets   = new ArrayList<Integer>();
         String             s         = "";
         while (mazeDashboard.update() && mazeDriver.stepMaze())
         {
            responses.add(mazeDriver.response);
            targets.add(mazeDriver.target);
            s = "Maze = " + i + " responses: ";
            for (int r : responses)
            {
               s += r + " ";
            }
            s += "targets: ";
            for (int r : targets)
            {
               s += r + " ";
            }
            mazeDashboard.logLast(s);
            if (mazeDriver.response != mazeDriver.target)
            {
               ok = false;
            }
         }
         if (ok)
         {
            trainOK++;
            mazeDashboard.logLast(s + " OK");
         }
         else
         {
            mazeDashboard.logLast(s + " Error");
         }
      }

      // Test mazes.
      mazeDashboard.log("Test results:");
      for (int i = 0, j = mazeDriver.testMazes.size(); i < j; i++)
      {
         mazeDriver.initTestMaze(i, responseDriver);
         mazeDashboard.log("Maze = " + i);
         boolean            ok        = true;
         ArrayList<Integer> responses = new ArrayList<Integer>();
         ArrayList<Integer> targets   = new ArrayList<Integer>();
         String             s         = "";
         while (mazeDashboard.update() && mazeDriver.stepMaze())
         {
            responses.add(mazeDriver.response);
            targets.add(mazeDriver.target);
            s = "Maze = " + i + " responses: ";
            for (int r : responses)
            {
               s += r + " ";
            }
            s += "targets: ";
            for (int r : targets)
            {
               s += r + " ";
            }
            mazeDashboard.logLast(s);
            if (mazeDriver.response != mazeDriver.target)
            {
               ok = false;
            }
         }
         if (ok)
         {
            testOK++;
            mazeDashboard.logLast(s + " OK");
         }
         else
         {
            mazeDashboard.logLast(s + " Error");
         }
      }

      // Show results.
      String message = "Train correct/total = " + trainOK + "/" + mazeDriver.trainMazes.size();
      if (mazeDriver.trainMazes.size() > 0)
      {
         message += " (" + (((float)trainOK / (float)mazeDriver.trainMazes.size()) * 100.0f) + "%)";
      }
      mazeDashboard.log(message);
      message = "Test correct/total = " + testOK + "/" + mazeDriver.testMazes.size();
      if (mazeDriver.testMazes.size() > 0)
      {
         message += " (" + (((float)testOK / (float)mazeDriver.testMazes.size()) * 100.0f) + "%)";
      }
      mazeDashboard.log(message);

      // Wait for quit.
      mazeDashboard.disableControls();
      while (mazeDashboard.update()) {}
   }


   // Main.
   // Exit codes:
   // 0=success
   // 1=fail
   // 2=error
   public static void main(String[] args)
   {
      // Get options.
      responseDriver = ResponseDriver.METAMORPH_DB;
      boolean batch           = false;
      boolean printParms      = false;
      boolean gotDatasetParm  = false;
      String  datasetFilename = Mouse.METAMORPH_DATASET_FILE_NAME;

      for (int i = 0; i < args.length; i++)
      {
         if (args[i].equals("-batch"))
         {
            batch = true;
            continue;
         }
         if (args[i].equals("-responseDriver"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid responseDriver option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (args[i].equals("metamorphDB"))
            {
               responseDriver = ResponseDriver.METAMORPH_DB;
            }
            else if (args[i].equals("metamorphNN"))
            {
               responseDriver = ResponseDriver.METAMORPH_NN;
            }
            else
            {
               System.err.println("Invalid responseDriver option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-numDoors"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid numDoors option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               Parameters.NUM_DOORS = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid numDoors option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (Parameters.NUM_DOORS < 0)
            {
               System.err.println("Invalid numDoors option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-mazeInteriorLength"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid mazeInteriorLength option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               Parameters.MAZE_INTERIOR_LENGTH = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid mazeInteriorLength option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (Parameters.MAZE_INTERIOR_LENGTH < 0)
            {
               System.err.println("Invalid mazeInteriorLength option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-numContextMazes"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid numContextMazes option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               Parameters.NUM_CONTEXT_MAZES = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid numContextMazes option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (Parameters.NUM_CONTEXT_MAZES < 0)
            {
               System.err.println("Invalid numContextMazes option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-numIndependentMazes"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid numIndependentMazes option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               Parameters.NUM_INDEPENDENT_MAZES = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid numIndependentMazes option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (Parameters.NUM_INDEPENDENT_MAZES < 0)
            {
               System.err.println("Invalid numIndependentMazes option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-neighborhoodDurations"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid neighborhoodDurations option");
               System.err.println(Usage);
               System.exit(1);
            }
            String[] durations = args[i].split(",");
            int n = durations.length;
            Parameters.NUM_NEIGHBORHOODS       = n;
            Parameters.NEIGHBORHOOD_DIMENSIONS = new int[n][2];
            Parameters.NEIGHBORHOOD_DURATIONS  = new int[n];
            for (int j = 0; j < n; j++)
            {
               Parameters.NEIGHBORHOOD_DIMENSIONS[j][0] = 1;
               Parameters.NEIGHBORHOOD_DIMENSIONS[j][1] = 1;
               try
               {
                  Parameters.NEIGHBORHOOD_DURATIONS[j] = Integer.parseInt(durations[j]);
               }
               catch (NumberFormatException e) {
                  System.err.println("Invalid neighborhoodDurations option");
                  System.err.println(Usage);
                  System.exit(1);
               }
            }
            continue;
         }
         if (args[i].equals("-NNlearningRate"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid NNlearningRate option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               Parameters.NN_LEARNING_RATE = Double.parseDouble(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid NNlearningRate option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (Parameters.NN_LEARNING_RATE < 0.0)
            {
               System.err.println("Invalid NNlearningRate option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-NNmomentum"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid NNmomentum option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               Parameters.NN_MOMENTUM = Double.parseDouble(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid NNmomentum option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (Parameters.NN_MOMENTUM < 0.0)
            {
               System.err.println("Invalid NNmomentum option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-NNhiddenLayers"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid NNhiddenLayers option");
               System.err.println(Usage);
               System.exit(1);
            }
            Parameters.NN_HIDDEN_LAYERS = new String(args[i]);
            if (Parameters.NN_HIDDEN_LAYERS.isEmpty())
            {
               System.err.println("Invalid NNhiddenLayers option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-NNtrainingTime"))
         {
            i++;
            if (i >= args.length)
            {
               System.err.println("Invalid NNtrainingTime option");
               System.err.println(Usage);
               System.exit(1);
            }
            try
            {
               Parameters.NN_TRAINING_TIME = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid NNtrainingTime option");
               System.err.println(Usage);
               System.exit(1);
            }
            if (Parameters.NN_TRAINING_TIME < 0)
            {
               System.err.println("Invalid NNtrainingTime option");
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
               randomSeed = MazeMaker.RANDOM_SEED = Integer.parseInt(args[i]);
            }
            catch (NumberFormatException e) {
               System.err.println("Invalid randomSeed option");
               System.err.println(Usage);
               System.exit(1);
            }
            continue;
         }
         if (args[i].equals("-printParameters"))
         {
            printParms = true;
            continue;
         }
         if (args[i].equals("-writeMetamorphDataset"))
         {
            gotDatasetParm = true;
            if (i < args.length - 1)
            {
               if (!args[i + 1].startsWith("-"))
               {
                  i++;
                  datasetFilename = args[i];
               }
            }
            continue;
         }
         if (args[i].equals("-help") || args[i].equals("-h") || args[i].equals("-?"))
         {
            System.out.println(Usage);
            System.exit(0);
         }
         if (args[i].equals("-version"))
         {
            System.out.println("Maze version = " + VERSION);
            System.out.println("Morphognosis version = " + Morphognosis.VERSION);
            System.exit(0);
         }
         System.err.println("Invalid option: " + args[i]);
         System.err.println(Usage);
         System.exit(1);
      }

      // Print parameters?
      if (printParms)
      {
         System.out.println("Parameters:");
         Parameters.print();
         System.exit(0);
      }

      // Set look and feel.
      try
      {
         UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
      }
      catch (Exception e)
      {
         System.err.println("Warning: cannot set look and feel");
      }

      // Initialize random numbers.
      random = new Random();
      random.setSeed(randomSeed);

      // Create mazes.
      try
      {
         MazeMaker.makeMazes();
      }
      catch (IOException e) {
         System.err.println("Cannot create mazes: " + e.getMessage());
         System.exit(1);
      }

      // Create maze driver.
      mazeDriver = new MazeDriver(responseDriver, randomSeed);

      // Batch run?
      if (batch)
      {
         mazeDriver.run();
      }
      else
      {
         // Run with dashboards.
         run();
      }

      // Write metamorph dataset?
      if (gotDatasetParm)
      {
         try
         {
            mazeDriver.mouse.writeMetamorphDataset(mazeDriver.mouse.metamorphs.length - 1, datasetFilename);
         }
         catch (Exception e)
         {
            System.err.println("Cannot write metamorph dataset to file " + datasetFilename + ": " + e.getMessage());
            System.exit(1);
         }
      }
      System.exit(0);
   }
}
