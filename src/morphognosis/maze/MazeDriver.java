// For conditions of distribution and use, see copyright notice in Main.java

// Maze driver.

package morphognosis.maze;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;
import java.util.stream.Collectors;

public class MazeDriver
{
   // Mazes.
   public static final String MAZE_DATASET_FILE_NAME = "maze_dataset.csv";
   public ArrayList<Maze>     trainMazes;
   public ArrayList<Maze>     testMazes;
   public int                 trainOK;
   public int                 testOK;

   // Current maze.
   public Maze    maze;
   public int     step;
   public float[] sensors;
   public int     response;
   public int     target;

   // Mouse.
   public int   numSensors;
   public int   numResponses;
   public Mouse mouse;

   // Test response driver.
   public int testResponseDriver;

   // Random numbers.
   public Random random;
   public int    randomSeed;

   // Constructor.
   public MazeDriver(int testResponseDriver, int randomSeed)
   {
      // Set test driver.
      this.testResponseDriver = testResponseDriver;

      // Random numbers.
      random          = new Random();
      this.randomSeed = randomSeed;
      random.setSeed(randomSeed);

      // Load mazes.
      loadMazes();

      // Create mouse.
      mouse = new Mouse(numSensors, numResponses, random);
   }


   // Load mazes.
   @SuppressWarnings("unused")
   public void loadMazes()
   {
      trainMazes = new ArrayList<Maze>();
      testMazes  = new ArrayList<Maze>();
      List<Integer> X_train_shape = null;
      List<Float>   X_train_seq   = null;
      List<Integer> y_train_shape = null;
      List<Float>   y_train_seq   = null;
      List<Integer> X_test_shape  = null;
      List<Float>   X_test_seq    = null;
      List<Integer> y_test_shape  = null;
      List<Float>   y_test_seq    = null;
      try (BufferedReader br = new BufferedReader(new FileReader(MAZE_DATASET_FILE_NAME)))
         {
            // Load dataset.
            String line;
            if ((line = br.readLine()) != null)
            {
               X_train_shape = Arrays.stream(line.split(","))
                                  .map(Integer::parseInt)
                                  .collect(Collectors.toList());
            }
            else
            {
               System.err.println("Cannot load X_train_shape from file " + MAZE_DATASET_FILE_NAME);
               System.exit(1);
            }
            if ((line = br.readLine()) != null)
            {
               X_train_seq = Arrays.stream(line.split(","))
                                .map(Float::parseFloat)
                                .collect(Collectors.toList());
            }
            else
            {
               System.err.println("Cannot load X_train_seq from file " + MAZE_DATASET_FILE_NAME);
               System.exit(1);
            }
            if ((line = br.readLine()) != null)
            {
               y_train_shape = Arrays.stream(line.split(","))
                                  .map(Integer::parseInt)
                                  .collect(Collectors.toList());
            }
            else
            {
               System.err.println("Cannot load y_train_shape from file " + MAZE_DATASET_FILE_NAME);
               System.exit(1);
            }
            if ((line = br.readLine()) != null)
            {
               y_train_seq = Arrays.stream(line.split(","))
                                .map(Float::parseFloat)
                                .collect(Collectors.toList());
            }
            else
            {
               System.err.println("Cannot load y_train_seq from file " + MAZE_DATASET_FILE_NAME);
               System.exit(1);
            }
            if ((line = br.readLine()) != null)
            {
               X_test_shape = Arrays.stream(line.split(","))
                                 .map(Integer::parseInt)
                                 .collect(Collectors.toList());
            }
            else
            {
               System.err.println("Cannot load X_test_shape from file " + MAZE_DATASET_FILE_NAME);
               System.exit(1);
            }
            if ((line = br.readLine()) != null)
            {
               X_test_seq = Arrays.stream(line.split(","))
                               .map(Float::parseFloat)
                               .collect(Collectors.toList());
            }
            else
            {
               System.err.println("Cannot load X_test_seq from file " + MAZE_DATASET_FILE_NAME);
               System.exit(1);
            }
            if ((line = br.readLine()) != null)
            {
               y_test_shape = Arrays.stream(line.split(","))
                                 .map(Integer::parseInt)
                                 .collect(Collectors.toList());
            }
            else
            {
               System.err.println("Cannot load y_test_shape from file " + MAZE_DATASET_FILE_NAME);
               System.exit(1);
            }
            if ((line = br.readLine()) != null)
            {
               y_test_seq = Arrays.stream(line.split(","))
                               .map(Float::parseFloat)
                               .collect(Collectors.toList());
            }
            else
            {
               System.err.println("Cannot load y_test_seq from file " + MAZE_DATASET_FILE_NAME);
               System.exit(1);
            }

            // Set mouse sensor and response parameters.
            numSensors   = X_train_shape.get(2);
            numResponses = y_train_shape.get(2);

            // Initialize training mazes.
            int X_idx         = 0;
            int y_idx         = 0;
            int num_seqs      = X_train_shape.get(0);
            int num_steps     = X_train_shape.get(1);
            int num_sensors   = X_train_shape.get(2);
            int num_responses = y_train_shape.get(2);
            for (int seq = 0; seq < num_seqs; seq++)
            {
               Maze maze = new Maze();
               for (int step = 0; step < num_steps; step++)
               {
                  float[] sensors = new float[num_sensors];
                  for (int i = 0; i < num_sensors; i++)
                  {
                     sensors[i] = X_train_seq.get(X_idx++);
                  }
                  maze.addSensors(sensors);
                  for (int i = 0; i < num_responses; i++)
                  {
                     if (y_train_seq.get(y_idx++) > 0.0f)
                     {
                        maze.addResponse(i);
                     }
                  }
               }
               trainMazes.add(maze);
            }

            // Initialize test mazes.
            X_idx     = 0;
            y_idx     = 0;
            num_seqs  = X_test_shape.get(0);
            num_steps = X_test_shape.get(1);
            for (int seq = 0; seq < num_seqs; seq++)
            {
               Maze maze = new Maze();
               for (int step = 0; step < num_steps; step++)
               {
                  float[] sensors = new float[num_sensors];
                  for (int i = 0; i < num_sensors; i++)
                  {
                     sensors[i] = X_test_seq.get(X_idx++);
                  }
                  maze.addSensors(sensors);
                  for (int i = 0; i < num_responses; i++)
                  {
                     if (y_test_seq.get(y_idx++) > 0.0f)
                     {
                        maze.addResponse(i);
                     }
                  }
               }
               testMazes.add(maze);
            }
         }
         catch (FileNotFoundException e)
         {
            System.err.println("Cannot find file " + MAZE_DATASET_FILE_NAME);
            System.exit(1);
         }
         catch (Exception e)
         {
            System.err.println("Cannot load mazes from " + MAZE_DATASET_FILE_NAME);
            System.exit(1);
         }

   }


   // Run.
   public void run()
   {
      // Train mouse on mazes.
      trainMazes();

      // Train NN?
      if (testResponseDriver == ResponseDriver.METAMORPH_NN)
      {
         System.out.println("Training neural network...");
         mouse.trainMetamorphNNs();
      }

      // Validate training.
      validateMazes();

      // Test.
      testMazes();

      // Print results.
      System.out.print("Train correct/total = " + trainOK + "/" + trainMazes.size());
      if (trainMazes.size() > 0)
      {
         System.out.print(" (" + (((float)trainOK / (float)trainMazes.size()) * 100.0f) + "%)");
      }
      System.out.println();
      System.out.print("Test correct/total = " + testOK + "/" + testMazes.size());
      if (testMazes.size() > 0)
      {
         System.out.print(" (" + (((float)testOK / (float)testMazes.size()) * 100.0f) + "%)");
      }
      System.out.println();
   }


   // Train mouse on mazes.
   public void trainMazes()
   {
      mouse.responseDriver = ResponseDriver.TRAINING_OVERRIDE;
      for (int i = 0, j = trainMazes.size(); i < j; i++)
      {
         maze = trainMazes.get(i);
         mouse.reset();
         for (step = 0; (sensors = maze.getSensors(step)) != null; step++)
         {
            target   = mouse.overrideResponse = maze.getResponse(step);
            response = mouse.cycle(sensors);
         }
      }
   }


   // Validate training.
   public void validateMazes()
   {
      trainOK = 0;
      System.out.println("Train results:");
      mouse.responseDriver = testResponseDriver;
      for (int i = 0, j = trainMazes.size(); i < j; i++)
      {
         maze = trainMazes.get(i);
         System.out.print("Maze = " + i + " responses: ");
         mouse.reset();
         boolean            ok        = true;
         ArrayList<Integer> responses = new ArrayList<Integer>();
         ArrayList<Integer> targets   = new ArrayList<Integer>();
         for (step = 0; (sensors = maze.getSensors(step)) != null; step++)
         {
            response = mouse.cycle(sensors);
            responses.add(response);
            target = maze.getResponse(step);
            targets.add(target);
            if (response != target)
            {
               ok = false;
            }
         }
         for (int r : responses)
         {
            System.out.print(r + " ");
         }
         System.out.print("targets: ");
         for (int t : targets)
         {
            System.out.print(t + " ");
         }
         if (ok)
         {
            System.out.println("OK");
            trainOK++;
         }
         else
         {
            System.out.println("Error");
         }
      }
   }


   // Test mouse on mazes.
   public void testMazes()
   {
      testOK = 0;
      System.out.println("Test results:");
      mouse.responseDriver = testResponseDriver;
      for (int i = 0, j = testMazes.size(); i < j; i++)
      {
         maze = testMazes.get(i);
         System.out.print("Maze = " + i + " responses: ");
         mouse.reset();
         boolean            ok        = true;
         ArrayList<Integer> responses = new ArrayList<Integer>();
         ArrayList<Integer> targets   = new ArrayList<Integer>();
         for (step = 0; (sensors = maze.getSensors(step)) != null; step++)
         {
            response = mouse.cycle(sensors);
            responses.add(response);
            target = maze.getResponse(step);
            targets.add(target);
            if (response != target)
            {
               ok = false;
            }
         }
         for (int r : responses)
         {
            System.out.print(r + " ");
         }
         System.out.print("targets: ");
         for (int t : targets)
         {
            System.out.print(t + " ");
         }
         if (ok)
         {
            System.out.println("OK");
            testOK++;
         }
         else
         {
            System.out.println("Error");
         }
      }
   }


   // Initialize maze training run.
   public void initTrainMaze(int mazeNum, int responseDriver)
   {
      maze = trainMazes.get(mazeNum);
      step = 0;
      mouse.reset();
      mouse.responseDriver = responseDriver;
   }


   // Initialize maze testing run.
   public void initTestMaze(int mazeNum, int responseDriver)
   {
      maze = testMazes.get(mazeNum);
      step = 0;
      mouse.reset();
      mouse.responseDriver = responseDriver;
   }


   // Step maze.
   public boolean stepMaze()
   {
      if ((sensors = maze.getSensors(step)) != null)
      {
         mouse.overrideResponse = maze.getResponse(step);
         response = mouse.cycle(sensors);
         target   = maze.getResponse(step);
         step++;
         return(true);
      }
      else
      {
         response = -1;
         return(false);
      }
   }
}
