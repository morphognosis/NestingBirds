// For conditions of distribution and use, see copyright notice in Mandala.java

// World composer.
// Compose paths by inserting, substituting, and deleting modular segments on a base path.

package mandala;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class WorldComposer
{
	// Path parameters.
	public static int BASE_PATH_LENGTH = 10;
	public static int NUM_MODULAR_PATHS = 5;
	public static int MIN_MODULAR_PATH_LENGTH = 2;
	public static int MAX_MODULAR_PATH_LENGTH = 5;
	
	// Test paths.
	public static int NUM_INSERTION_TEST_PATHS = 5;
	public static int NUM_SUBSTITUTION_TEST_PATHS = 5; 
	public static int NUM_DELETION_TEST_PATHS = 5; 
	
	// Dataset exports.
	public static String PATH_NN_DATASET_FILENAME = "world_path_nn_dataset.csv";	
	public static String PATH_RNN_DATASET_FILENAME = "world_path_rnn_dataset.py";
	public static String PATH_TCN_DATASET_FILENAME = "world_path_tcn_dataset.py";	
	
	// Sizes.
	public int pathEncodedTypeSize;
	public int pathEncodedValueSize;
	
	// Paths.
	public int[] basePath;
	public List<int[]> modularPaths;
	public List<int[]> testPaths;
	public List<int[]> testPathTypes;
	
	// Random numbers.
	public static int RANDOM_SEED = 4517;
	public Random randomizer = null;
	
    // Usage.
    public static final String Usage =
      "Usage:\n" +
      "    java mandala.WorldComposer\n" +
      "        [-basePathLength <quantity> (default=" + BASE_PATH_LENGTH + ")]\n" +
      "        [-minModularPathLength <quantity> (default=" + MIN_MODULAR_PATH_LENGTH + ")]\n" +
      "        [-maxModularPathLength <quantity> (default=" + MAX_MODULAR_PATH_LENGTH + ")]\n" +    
      "        [-numModularPaths <quantity> (default=" + NUM_MODULAR_PATHS + ")]\n" +
      "        [-numInsertionTestPaths <quantity> (default=" + NUM_INSERTION_TEST_PATHS + ")]\n" + 
      "        [-numSubstitutionTestPaths <quantity> (default=" + NUM_SUBSTITUTION_TEST_PATHS + ")]\n" + 
      "        [-numDeletionTestPaths <quantity> (default=" + NUM_DELETION_TEST_PATHS + ")]\n" +       
      "        [-exportPathNNdatasetFilename (default=\"" + PATH_NN_DATASET_FILENAME + "\")>]\n" +     
      "        [-exportPathRNNdatasetFilename (default=\"" + PATH_RNN_DATASET_FILENAME + "\")>]\n" +
      "        [-exportPathTCNdatasetFilename (default=\"" + PATH_TCN_DATASET_FILENAME + "\")>]\n" +
      "        [-randomSeed <seed> (default=" + RANDOM_SEED + ")]\n" +           
      "Exit codes:\n" +
      "  0=success\n" +
      "  1=error";
	
    // Main.
    public static void main(String[] args) 
    { 
        for (int i = 0; i < args.length; i++)
        {
           if (args[i].equals("-basePathLength"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid basePathLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 BASE_PATH_LENGTH = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid basePathLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (BASE_PATH_LENGTH <= 0)
              {
                 System.err.println("Invalid basePathLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }              
              continue;
           }
           if (args[i].equals("-minModularPathLength"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid minModularPathLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 MIN_MODULAR_PATH_LENGTH = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid minModularPathLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (MIN_MODULAR_PATH_LENGTH <= 0)
              {
                 System.err.println("Invalid minModularPathLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              continue;
           }
           if (args[i].equals("-maxModularPathLength"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid maxModularPathLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 MAX_MODULAR_PATH_LENGTH = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid maxModularPathLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (MAX_MODULAR_PATH_LENGTH <= 0)
              {
                 System.err.println("Invalid maxModularPathLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              continue;
           }
           if (args[i].equals("-numModularPaths"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid numModularPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 NUM_MODULAR_PATHS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid numModularPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (NUM_MODULAR_PATHS < 0)
              {
                 System.err.println("Invalid numModularPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }              
              continue;
           }
           if (args[i].equals("-numInsertionTestPaths"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid numInsertionTestPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 NUM_INSERTION_TEST_PATHS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid numInsertionTestPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (NUM_INSERTION_TEST_PATHS < 0)
              {
                 System.err.println("Invalid numInsertionTestPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              continue;
           }
           if (args[i].equals("-numSubstitutionTestPaths"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid numSubstitutionTestPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 NUM_SUBSTITUTION_TEST_PATHS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid numSubstitutionTestPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (NUM_SUBSTITUTION_TEST_PATHS < 0)
              {
                 System.err.println("Invalid numSubstitutionTestPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              continue;
           }
           if (args[i].equals("-numDeletionTestPaths"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid numDeletionTestPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 NUM_DELETION_TEST_PATHS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid numDeletionTestPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (NUM_DELETION_TEST_PATHS < 0)
              {
                 System.err.println("Invalid numDeletionTestPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              continue;
           }                        
           if (args[i].equals("-exportPathNNdatasetFilename"))
           {
              if (i < args.length - 1 && !args[i + 1].startsWith("-"))
              {
            	 i++;
            	 PATH_NN_DATASET_FILENAME = args[i];
              }
              continue;
           }                              
           if (args[i].equals("-exportPathRNNdatasetFilename"))
           {
              if (i < args.length - 1 && !args[i + 1].startsWith("-"))
              {
            	 i++;
            	 PATH_RNN_DATASET_FILENAME = args[i];
              }
              continue;
           }           
           if (args[i].equals("-exportPathTCNdatasetFilename"))
           {
              if (i < args.length - 1 && !args[i + 1].startsWith("-"))
              {
            	 i++;
            	 PATH_TCN_DATASET_FILENAME = args[i];
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
        if (MIN_MODULAR_PATH_LENGTH > MAX_MODULAR_PATH_LENGTH)
        {
            System.err.println(Usage);
            System.exit(1);           	
        }              
        if (MAX_MODULAR_PATH_LENGTH > BASE_PATH_LENGTH)
        {
            System.err.println(Usage);
            System.exit(1);           	
        }            
        
        // Create world composer.
        WorldComposer worldComposer = new WorldComposer();
        
        // Create paths.
        worldComposer.createPaths();

        // Export datasets.
        System.out.println("Exporting datasets:");
        System.out.println(PATH_NN_DATASET_FILENAME);
        worldComposer.exportPathNNdataset(PATH_NN_DATASET_FILENAME);
        System.out.println(PATH_RNN_DATASET_FILENAME);        
        worldComposer.exportPathDataset(PATH_RNN_DATASET_FILENAME);
        System.out.println(PATH_TCN_DATASET_FILENAME);        
        worldComposer.exportPathDataset(PATH_TCN_DATASET_FILENAME);       
        
        System.exit(0);
    }
    
    // Constructor.
    public WorldComposer()
    {
        // Initialize random numbers.
        randomizer = new Random(RANDOM_SEED);
        
        // Encoding sizes.
        pathEncodedTypeSize = NUM_MODULAR_PATHS + 1;
        pathEncodedValueSize = Math.max(BASE_PATH_LENGTH, MAX_MODULAR_PATH_LENGTH);        
    }
       
    // Create world paths.
    public void createPaths()
    {
    	// Training paths.
    	System.out.println("Training paths:");
    	System.out.print("Base path = ");
        basePath = new int[BASE_PATH_LENGTH];
        for (int i = 0; i < BASE_PATH_LENGTH; i++)
        {
        	basePath[i] = randomizer.nextInt(pathEncodedValueSize);
        	System.out.print("0:" + basePath[i] + " ");
        }
        System.out.println();
    	System.out.println("Modular paths: ");        
        modularPaths = new ArrayList<int[]>();
        for (int i = 0; i < NUM_MODULAR_PATHS; i++)
        {
        	int[] path;
        	int n = MAX_MODULAR_PATH_LENGTH - MIN_MODULAR_PATH_LENGTH;
        	if (n > 0)
        	{
        		path = new int[randomizer.nextInt(n) + MIN_MODULAR_PATH_LENGTH];
        	} else {
        		path = new int[MIN_MODULAR_PATH_LENGTH];
        	}
            for (int j = 0; j < path.length; j++)
            {
            	path[j] = randomizer.nextInt(pathEncodedValueSize);
            	System.out.print((i + 1) + ":" + path[j] + " ");
            }
        	System.out.println();            
        	modularPaths.add(path); 
        }
        
        // Testing paths.
        System.out.println("Test paths:");
        testPaths = new ArrayList<int[]>();
        testPathTypes = new ArrayList<int[]>();        
        if (NUM_MODULAR_PATHS == 0) return;
        System.out.println("Insertion:");
        for (int i = 0; i < NUM_INSERTION_TEST_PATHS; i++)
        {
        	int m = randomizer.nextInt(NUM_MODULAR_PATHS);        	
        	int[] mpath = modularPaths.get(m);
        	int n = randomizer.nextInt(BASE_PATH_LENGTH + 1);        	
        	int[] path = new int[BASE_PATH_LENGTH + mpath.length];
        	int[] types = new int[BASE_PATH_LENGTH + mpath.length];        	
        	int j = 0;
            for (; j < n; j++)
            {
            	path[j] = basePath[j];
            	types[j] = 0;
            	System.out.print(types[j] + ":" + path[j] + " ");
            }
            for (int k = 0; k < mpath.length; k++, j++)
            {
            	path[j] = mpath[k];
            	types[j] = m + 1;
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            }
            for (int k = n; k < BASE_PATH_LENGTH; k++, j++)
            {
            	path[j] = basePath[k];
            	types[j] = 0;
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            } 
            System.out.println();
        	testPaths.add(path); 
        	testPathTypes.add(types);         	
        }
        System.out.println("Substitution:");        
        for (int i = 0; i < NUM_SUBSTITUTION_TEST_PATHS; i++)
        {
        	int m = randomizer.nextInt(NUM_MODULAR_PATHS);         	
        	int[] mpath = modularPaths.get(m);
        	int n = randomizer.nextInt(BASE_PATH_LENGTH + 1);
        	int[] path = new int[BASE_PATH_LENGTH];
        	int[] types = new int[BASE_PATH_LENGTH];        	
        	int j = 0;
            for (; j < n; j++)
            {
            	path[j] = basePath[j];
            	types[j] = 0;
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            }
            for (int k = 0; k < mpath.length && j < BASE_PATH_LENGTH; k++, j++)
            {
            	path[j] = mpath[k];
            	types[j] = m + 1; 
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            }
            for (int k = mpath.length + n; k < BASE_PATH_LENGTH; k++, j++)
            {
            	path[j] = basePath[k];
            	types[j] = 0;
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            }
        	System.out.println();            
        	testPaths.add(path); 
        	testPathTypes.add(types);        	
        }
        System.out.println("Deletion:");        
        for (int i = 0; i < NUM_DELETION_TEST_PATHS; i++)
        {
        	int[] mpath = modularPaths.get(randomizer.nextInt(NUM_MODULAR_PATHS));
        	int n = randomizer.nextInt(BASE_PATH_LENGTH + 1);        	
        	int[] path;
        	int j = BASE_PATH_LENGTH - (n + mpath.length);
        	if (j >= 0)
        	{
        		path = new int[BASE_PATH_LENGTH - mpath.length];
        	} else {
        		path = new int[BASE_PATH_LENGTH - (mpath.length + j)];
        	}
        	int[] types = new int[path.length];
        	j = 0;
            for (; j < n; j++)
            {
            	path[j] = basePath[j];
            	types[j] = 0;
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            }
            for (int k = mpath.length + n; k < BASE_PATH_LENGTH; k++, j++)
            {
            	path[j] = basePath[k];
            	types[j] = 0;
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            }
            System.out.println();
        	testPaths.add(path);
        	testPathTypes.add(types);
        }               
    }
    
    // Export path NN dataset.
    public void exportPathNNdataset()
    {
    	exportPathNNdataset(PATH_NN_DATASET_FILENAME);
    }
    
    public void exportPathNNdataset(String filename)
    {
    	if (basePath == null)
    	{
    		System.err.println("No paths");
    		System.exit(1);
    	}
    	try
    	{
            FileWriter fileWriter = new FileWriter(filename);
            PrintWriter printWriter = new PrintWriter(fileWriter);
            int pathLength = basePath.length;
            for (int[] p : modularPaths)
            {
            	if (pathLength < p.length)
            	{
            		pathLength = p.length;
            	}
            }            
            for (int[] p : testPaths)
            {
            	if (pathLength < p.length)
            	{
            		pathLength = p.length;
            	}
            }
            int n = modularPaths.size() + 1;            
            printWriter.println("X_train_shape, " + (n * pathLength) + ", " + pathLength + ", " + 
            		((pathEncodedTypeSize + pathEncodedValueSize) * pathLength));
            for (int i = 0; i < n; i++)
            {
            	int[] values;
            	if (i == 0)
            	{
            		values = basePath;            		
            	} else {
            		values = modularPaths.get(i - 1); 
            	}
        		int[] valueFrame = new int[pathLength];
        		for (int j = 0; j < pathLength; j++)
        		{
        			valueFrame[j] = -1;
        		}            	
            	for (int j = 0; j < pathLength; j++)
            	{
	            	for (int k = 0; k < values.length; k++)
	        		{
	            		int idx = k + pathLength - j - 1;
	            		if (idx < pathLength)
	            		{
	            			valueFrame[idx] = values[k]; 
	            		}
	        		}
	            	for (int k = 0; k < pathLength; k++)
	        		{
	            		printWriter.print(oneHot(i, valueFrame[k]));
	                	if (k < pathLength - 1)
	                	{
	                		printWriter.print(", ");
	                	}            		
	        		}
	            	printWriter.println();    	
            	}
            }
            printWriter.println("y_train_shape, " + (n * pathLength) + ", " + pathLength + ", " + 
            		(pathEncodedTypeSize + pathEncodedValueSize));
            for (int i = 0; i < n; i++)
            {
               	int[] values;
            	if (i == 0)
            	{
            		values = basePath;            		
            	} else {
            		values = modularPaths.get(i - 1); 
            	}
            	for (int j = 1; j < pathLength; j++)        		
            	{            		
            		if (j < values.length) 
            		{
            			printWriter.print(oneHot(i, values[j]));          			
            		} else {
            			printWriter.print(oneHot(i, -1));             			
            		}
                	printWriter.println();                	
        		}
    			printWriter.println(oneHot(i, -1));
            }
            n = testPaths.size();
            printWriter.println("X_test_shape, " + (n * pathLength) + ", " + pathLength + ", " + 
            		((pathEncodedTypeSize + pathEncodedValueSize) * pathLength));
            for (int i = 0; i < n; i++)
            {
            	int[] values = testPaths.get(i);
            	int[] types = testPathTypes.get(i);
        		int[] valuesFrame = new int[pathLength];
        		int[] typesFrame = new int[pathLength];
        		for (int j = 0; j < pathLength; j++)
        		{
        			valuesFrame[j] = -1;
        			typesFrame[j] = 0;
        		}            	
            	for (int j = 0; j < pathLength; j++)
            	{
	            	for (int k = 0; k < values.length; k++)
	        		{
	            		int idx = k + pathLength - j - 1;
	            		if (idx < pathLength)
	            		{
	            			valuesFrame[idx] = values[k];
	            			typesFrame[idx] = types[k];
	            		}
	        		}
	            	for (int k = 0; k < pathLength; k++)
	        		{
	            		printWriter.print(oneHot(typesFrame[k], valuesFrame[k]));
	                	if (k < pathLength - 1)
	                	{
	                		printWriter.print(", ");
	                	}            		
	        		}
	            	printWriter.println();         	
            	}
            }
            printWriter.println("y_test_shape, " + (n * pathLength) + ", " + pathLength + ", " + 
            		(pathEncodedTypeSize + pathEncodedValueSize));
            for (int i = 0; i < n; i++)
            {
            	int[] values = testPaths.get(i);
            	int[] types = testPathTypes.get(i);
            	for (int j = 1; j < pathLength; j++)
        		{
            		if (j < values.length) 
            		{
            			printWriter.print(oneHot(types[j], values[j]));
            		} else {
            			printWriter.print(oneHot(0, -1));          			
            		}
                	printWriter.println();          		
        		}
    			printWriter.println(oneHot(0, -1));            	
            }               
            printWriter.close();
    	} catch (IOException e)
    	{
    		System.err.println("Cannot write path dataset to file " + filename);
    		System.exit(1);
    	}    	
    }
        
    // Export path RNN dataset.
    public void exportPathRNNdataset()
    {
    	exportPathDataset(PATH_RNN_DATASET_FILENAME);
    }
    
    // Export path TCN dataset.
    public void exportPathTCNdataset()
    {
    	exportPathDataset(PATH_TCN_DATASET_FILENAME);
    }
    
    public void exportPathDataset(String filename)
    {    	
    	if (basePath == null)
    	{
    		System.err.println("No path");
    		System.exit(1);
    	}
    	try
    	{
            FileWriter fileWriter = new FileWriter(filename);
            PrintWriter printWriter = new PrintWriter(fileWriter);
            int pathLength = basePath.length;
            for (int[] p : modularPaths)
            {
            	if (pathLength < p.length)
            	{
            		pathLength = p.length;
            	}
            }            
            for (int[] p : testPaths)
            {
            	if (pathLength < p.length)
            	{
            		pathLength = p.length;
            	}
            }
            int n = modularPaths.size() + 1;            
            printWriter.println("X_train_shape = [ " + n + ", " + pathLength + ", " + 
            		(pathEncodedTypeSize + pathEncodedValueSize) + " ]");
            printWriter.print("X_train_seq = [ ");
            for (int i = 0; i < n; i++)
            {
            	int[] values;
            	if (i == 0)
            	{
            		values = basePath;
            	} else {
            		values = modularPaths.get(i - 1);
            	}
            	for (int j = 0; j < pathLength; j++)
        		{
            		if (j < values.length) 
            		{
            			printWriter.print(oneHot(i, values[j]));
            		} else {
            			printWriter.print(oneHot(i, -1));          			
            		}
                	if (j < pathLength - 1)
                	{
                		printWriter.print(", ");
                	}            		
        		}
            	if (i < n - 1)
            	{
            		printWriter.print(", ");
            	}
            }
            printWriter.println(" ]");
            printWriter.println("y_train_shape = [ " + n + ", " + pathLength + ", " + 
            		(pathEncodedTypeSize + pathEncodedValueSize) + " ]");
            printWriter.print("y_train_seq = [ ");
            for (int i = 0; i < n; i++)
            {
            	int[] values;
            	if (i == 0)
            	{
            		values = basePath;
            	} else {
            		values = modularPaths.get(i - 1);
            	}
            	for (int j = 1; j < pathLength; j++)
        		{
            		if (j < values.length) 
            		{
            			printWriter.print(oneHot(i, values[j]));
            		} else {
            			printWriter.print(oneHot(i, -1));          			
            		}
                	printWriter.print(", ");          		
        		}
    			printWriter.print(oneHot(i, -1));            	
            	if (i < n - 1)
            	{
            		printWriter.print(", ");
            	}
            }
            n = testPaths.size();
            printWriter.println(" ]");
            printWriter.println("X_test_shape = [ " + n + ", " + pathLength + ", " + 
            		(pathEncodedTypeSize + pathEncodedValueSize) + " ]");
            printWriter.print("X_test_seq = [ ");
            for (int i = 0; i < n; i++)
            {
            	int[] values = testPaths.get(i);
            	int[] types = testPathTypes.get(i);
            	for (int j = 0; j < pathLength; j++)
        		{
            		if (j < values.length) 
            		{
            			printWriter.print(oneHot(types[j], values[j]));
            		} else {
            			printWriter.print(oneHot(0, -1));          			
            		}
                	if (j < pathLength - 1)
                	{
                		printWriter.print(", ");
                	}            		
        		}
            	if (i < n - 1)
            	{
            		printWriter.print(", ");
            	}
            }
            printWriter.println(" ]");
            printWriter.println("y_test_shape = [ " + n + ", " + pathLength + ", " + 
            		(pathEncodedTypeSize + pathEncodedValueSize) + " ]");
            printWriter.print("y_test_seq = [ ");
            for (int i = 0; i < n; i++)
            {
            	int[] values = testPaths.get(i);
            	int[] types = testPathTypes.get(i);
            	for (int j = 1; j < pathLength; j++)
        		{
            		if (j < values.length) 
            		{
            			printWriter.print(oneHot(types[j], values[j]));
            		} else {
            			printWriter.print(oneHot(0, -1));          			
            		}
                	printWriter.print(", ");          		
        		}
    			printWriter.print(oneHot(0, -1));            	
            	if (i < n - 1)
            	{
            		printWriter.print(", ");
            	}
            }
            printWriter.println(" ]");               
            printWriter.close();
    	} catch (IOException e)
    	{
    		System.err.println("Cannot write path dataset to file " + filename);
    		System.exit(1);
    	}    	
    }

    // One-hot coding of path type and value.
    public String oneHot(int type, int value)
    {
    	String code = "";
    	for (int i = 0; i < pathEncodedTypeSize; i++)
    	{
    		if (i == type)
    		{
    			code += "1";
    		} else {
    			code += "0";
    		}
    		code += ", ";
    	}
    	for (int i = 0; i < pathEncodedValueSize; i++)
    	{
    		if (i == value)
    		{
    			code += "1";
    		} else {
    			code += "0";
    		}
    		if (i < pathEncodedValueSize - 1)
    		{
    			code += ", ";
    		}
    	}
    	return code;
    }    
}
