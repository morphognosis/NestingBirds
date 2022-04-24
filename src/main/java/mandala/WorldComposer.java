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
	public static boolean PREDICT_PATH = false;
	public static int BASE_PATH_LENGTH = 10;
	public static int NUM_MODULAR_PATHS = 5;
	public static int MIN_MODULAR_PATH_LENGTH = 2;
	public static int MAX_MODULAR_PATH_LENGTH = 5;
	
	// Test paths.
	public static int NUM_INSERTION_TEST_PATHS = 5;
	public static int NUM_SUBSTITUTION_TEST_PATHS = 5; 
	public static int NUM_DELETION_TEST_PATHS = 5;
 	
	// Dataset exports.
	public static String PATH_NN_DATASET_CSV_FILENAME = "world_path_nn_dataset.csv";
	public static String PATH_NN_DATASET_FILENAME = "world_path_nn_dataset.py";	
	public static String PATH_RNN_DATASET_FILENAME = "world_path_rnn_dataset.py";
	public static String PATH_TCN_DATASET_FILENAME = "world_path_tcn_dataset.py";	
	
	// Sizes.
	public int pathEncodedTypeSize;
	public int pathEncodedValueSize;
	
	// Paths.
	public int[] basePath;
	public int[] baseResponses;
	public List<int[]> modularPaths;
	public List<int[]> modularResponses;
	public List<int[]> testPaths;
	public List<int[]> testPathTypes;
	public List<int[]> testResponses;	
	
	// Event dilation.
	public static final int NO_DILATION = 0;
	public static final int OVERLAY_DILATION = 1;
	public static final int ACCUMULATE_DILATION = 2;
	public static final int NORMALIZE_DILATION = 3;
	public static int DILATE_EVENTS = NO_DILATION;	
	
	// Random numbers.
	public static int RANDOM_SEED = 4517;
	public Random randomizer = null;
	
    // Usage.
    public static final String Usage =
      "Usage:\n" +
      "    java mandala.WorldComposer\n" +
      "        [-predictPath (path prediction responses)]\n" +    		  
      "        [-basePathLength <quantity> (default=" + BASE_PATH_LENGTH + ")]\n" +
      "        [-minModularPathLength <quantity> (default=" + MIN_MODULAR_PATH_LENGTH + ")]\n" +
      "        [-maxModularPathLength <quantity> (default=" + MAX_MODULAR_PATH_LENGTH + ")]\n" +    
      "        [-numModularPaths <quantity> (default=" + NUM_MODULAR_PATHS + ")]\n" +
      "        [-numInsertionTestPaths <quantity> (default=" + NUM_INSERTION_TEST_PATHS + ")]\n" + 
      "        [-numSubstitutionTestPaths <quantity> (default=" + NUM_SUBSTITUTION_TEST_PATHS + ")]\n" + 
      "        [-numDeletionTestPaths <quantity> (default=" + NUM_DELETION_TEST_PATHS + ")]\n" +
      "        [-dilateEvents <\"overlay\" | \"accumulate\" | \"normalize\"> (dilate relatively recent events into the past)]\n" +     
      "        [-exportPathNNdatasetCSV (default=\"" + PATH_NN_DATASET_CSV_FILENAME + "\")>]\n" + 
      "        [-exportPathNNdataset (default=\"" + PATH_NN_DATASET_FILENAME + "\")>]\n" +         
      "        [-exportPathRNNdataset (default=\"" + PATH_RNN_DATASET_FILENAME + "\")>]\n" +
      "        [-exportPathTCNdataset (default=\"" + PATH_TCN_DATASET_FILENAME + "\")>]\n" +
      "        [-randomSeed <seed> (default=" + RANDOM_SEED + ")]\n" +           
      "Exit codes:\n" +
      "  0=success\n" +
      "  1=error";
	
    // Main.
    public static void main(String[] args) 
    { 
        for (int i = 0; i < args.length; i++)
        {
           if (args[i].equals("-predictPath"))
           {
        	  PREDICT_PATH = true;    
              continue;
           }
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
              } else if (args[i].equals("accumulate"))
              {
            	  DILATE_EVENTS = ACCUMULATE_DILATION;
              } else if (args[i].equals("normalize"))
              {
            	  DILATE_EVENTS = NORMALIZE_DILATION;            	  
              } else {
                  System.err.println("Invalid dilateEvents option");
                  System.err.println(Usage);
                  System.exit(1);            	  
              }
              continue;
           }
           if (args[i].equals("-exportPathNNdatasetCSV"))
           {
              if (i < args.length - 1 && !args[i + 1].startsWith("-"))
              {
            	 i++;
            	 PATH_NN_DATASET_CSV_FILENAME = args[i];
              }
              continue;
           }                  
           if (args[i].equals("-exportPathNNdataset"))
           {
              if (i < args.length - 1 && !args[i + 1].startsWith("-"))
              {
            	 i++;
            	 PATH_NN_DATASET_FILENAME = args[i];
              }
              continue;
           }                              
           if (args[i].equals("-exportPathRNNdataset"))
           {
              if (i < args.length - 1 && !args[i + 1].startsWith("-"))
              {
            	 i++;
            	 PATH_RNN_DATASET_FILENAME = args[i];
              }
              continue;
           }           
           if (args[i].equals("-exportPathTCNdataset"))
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
        System.out.println(PATH_NN_DATASET_CSV_FILENAME + " and " + PATH_NN_DATASET_FILENAME);
        worldComposer.exportPathNNdatasets();
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
    	System.out.println("Base path: ");
    	System.out.print("path: ");    	
        basePath = new int[BASE_PATH_LENGTH];       
        for (int i = 0; i < BASE_PATH_LENGTH; i++)
        {
        	basePath[i] = randomizer.nextInt(pathEncodedValueSize);
        	System.out.print("0:" + basePath[i] + " ");
        }
        System.out.println();
        baseResponses = new int[BASE_PATH_LENGTH];        
        for (int i = 0; i < BASE_PATH_LENGTH; i++)
        {
        	baseResponses[i] = randomizer.nextInt(pathEncodedValueSize);
        }
        if (!PREDICT_PATH)
        {
	    	System.out.print("responses: ");      
	        for (int i = 0; i < BASE_PATH_LENGTH; i++)
	        {
	        	System.out.print(baseResponses[i] + " ");
	        }
	        System.out.println();
        }        
    	System.out.println("Modular paths: ");        
        modularPaths = new ArrayList<int[]>();
        modularResponses = new ArrayList<int[]>();        
        for (int i = 0; i < NUM_MODULAR_PATHS; i++)
        {
        	System.out.print("path: ");         	
        	int[] path;
        	int n = MAX_MODULAR_PATH_LENGTH - MIN_MODULAR_PATH_LENGTH;
        	if (n > 0)
        	{
        		n = randomizer.nextInt(n) + MIN_MODULAR_PATH_LENGTH;
        	} else {
        		n = MIN_MODULAR_PATH_LENGTH;
        	}
        	path = new int[n];
            for (int j = 0; j < path.length; j++)
            {
            	path[j] = randomizer.nextInt(pathEncodedValueSize);
            	System.out.print((i + 1) + ":" + path[j] + " ");
            }
        	System.out.println();            
        	modularPaths.add(path);         	
        	int[] responses = new int[n];
            for (int j = 0; j < responses.length; j++)
            {
            	responses[j] = randomizer.nextInt(pathEncodedValueSize);
            }            
        	modularResponses.add(responses);       	
        	if (!PREDICT_PATH)
        	{
	        	System.out.print("responses: ");         	
	            for (int j = 0; j < responses.length; j++)
	            {
	            	System.out.print(responses[j] + " ");
	            }
	        	System.out.println();            
        	}
        }
        
        // Testing paths.
        System.out.println("Test paths:");
        testPaths = new ArrayList<int[]>();
        testPathTypes = new ArrayList<int[]>();
        testResponses = new ArrayList<int[]>();        
        if (NUM_MODULAR_PATHS == 0) return;
        System.out.println("Insertion:");
        for (int i = 0; i < NUM_INSERTION_TEST_PATHS; i++)
        {
        	System.out.print("path: ");
        	int m = randomizer.nextInt(NUM_MODULAR_PATHS);        	
        	int[] mpath = modularPaths.get(m);
        	int[] mresponses = modularResponses.get(m);        	
        	int n = randomizer.nextInt(BASE_PATH_LENGTH + 1);        	
        	int[] path = new int[BASE_PATH_LENGTH + mpath.length];
        	int[] types = new int[BASE_PATH_LENGTH + mpath.length];
        	int[] responses = new int[BASE_PATH_LENGTH + mpath.length];        	
        	int j = 0;
            for (; j < n; j++)
            {
            	path[j] = basePath[j];
            	types[j] = 0;
            	responses[j] = baseResponses[j];
            	System.out.print(types[j] + ":" + path[j] + " ");
            }
            for (int k = 0; k < mpath.length; k++, j++)
            {
            	path[j] = mpath[k];
            	types[j] = m + 1;
            	responses[j] = mresponses[k];
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            }
            for (int k = n; k < BASE_PATH_LENGTH; k++, j++)
            {
            	path[j] = basePath[k];
            	types[j] = 0;
            	responses[j] = baseResponses[k];
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            } 
            System.out.println();
            if (!PREDICT_PATH)
            {
            	System.out.print("responses: ");
            	for (int k = 0; k < responses.length; k++)
            	{
                	System.out.print(responses[k] + " ");            		
            	}
            	System.out.println();
            }
        	testPaths.add(path); 
        	testPathTypes.add(types);
        	testResponses.add(responses);
        }
        System.out.println("Substitution:");        
        for (int i = 0; i < NUM_SUBSTITUTION_TEST_PATHS; i++)
        {
        	System.out.print("path: ");        	
        	int m = randomizer.nextInt(NUM_MODULAR_PATHS);         	
        	int[] mpath = modularPaths.get(m);
        	int[] mresponses = modularResponses.get(m);        	
        	int n = randomizer.nextInt(BASE_PATH_LENGTH);
        	int[] path = new int[BASE_PATH_LENGTH];
        	int[] types = new int[BASE_PATH_LENGTH];
        	int[] responses = new int[BASE_PATH_LENGTH];        	
        	int j = 0;
            for (; j < n; j++)
            {
            	path[j] = basePath[j];
            	types[j] = 0;
            	responses[j] = baseResponses[j];
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            }
            for (int k = 0; k < mpath.length && j < BASE_PATH_LENGTH; k++, j++)
            {
            	path[j] = mpath[k];
            	types[j] = m + 1;
            	responses[j] = mresponses[k];
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            }
            for (int k = mpath.length + n; k < BASE_PATH_LENGTH; k++, j++)
            {
            	path[j] = basePath[k];
            	types[j] = 0;
            	responses[j] = baseResponses[k];
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            }
        	System.out.println();            
            if (!PREDICT_PATH)
            {
            	System.out.print("responses: ");
            	for (int k = 0; k < responses.length; k++)
            	{
                	System.out.print(responses[k] + " ");            		
            	}
            	System.out.println();
            }
        	testPaths.add(path); 
        	testPathTypes.add(types);
        	testResponses.add(responses);       	
        }
        System.out.println("Deletion:");        
        for (int i = 0; i < NUM_DELETION_TEST_PATHS; i++)
        {
        	System.out.print("path: ");        	
        	int m = randomizer.nextInt(NUM_MODULAR_PATHS);
        	int[] mpath = modularPaths.get(m);       	
        	int n = randomizer.nextInt(BASE_PATH_LENGTH);       	
        	int[] path;
        	int j = BASE_PATH_LENGTH - (n + mpath.length);
        	if (j >= 0)
        	{
        		path = new int[BASE_PATH_LENGTH - mpath.length];
        	} else {
        		path = new int[BASE_PATH_LENGTH - (mpath.length + j)];
        	}
        	int[] types = new int[path.length];
        	int[] responses = new int[path.length];        	
        	j = 0;
            for (; j < n; j++)
            {
            	path[j] = basePath[j];
            	types[j] = 0;
            	responses[j] = baseResponses[j];
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            }
            for (int k = mpath.length + n; k < BASE_PATH_LENGTH; k++, j++)
            {
            	path[j] = basePath[k];
            	types[j] = 0;
            	responses[j] = baseResponses[k];
            	System.out.print(types[j] + ":" + path[j] + " ");            	
            }
            System.out.println();
            if (!PREDICT_PATH)
            {
            	System.out.print("responses: ");
            	for (int k = 0; k < responses.length; k++)
            	{
                	System.out.print(responses[k] + " ");            		
            	}
            	System.out.println();
            }
        	testPaths.add(path); 
        	testPathTypes.add(types);
        	testResponses.add(responses);
        }               
    }
    
    // Export path NN datasets.
    public void exportPathNNdatasets()
    {
    	exportPathNNdatasetCSV(PATH_NN_DATASET_CSV_FILENAME);
    	exportPathNNdatasetPy(PATH_NN_DATASET_FILENAME);    	
    }
    
    public void exportPathNNdatasetCSV(String filename)
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
    		int numPaths = basePath.length - 1;
            for (int[] p : modularPaths)
            {
            	if (pathLength < p.length)
            	{
            		pathLength = p.length;
            	}
            	numPaths += p.length - 1;
            }
            if (!PREDICT_PATH)
            {
            	numPaths += modularPaths.size() + 1;
            }
            for (int[] p : testPaths)
            {
            	if (pathLength < p.length)
            	{
            		pathLength = p.length;
            	}
            } 
        	if (DILATE_EVENTS == NO_DILATION)
        	{            
	            printWriter.println("X_train_shape, " + numPaths + ", " + 
	            		((pathEncodedTypeSize + pathEncodedValueSize) * pathLength));
        	} else {
	            printWriter.println("X_train_shape, " + numPaths + ", " + 
	            		((pathEncodedTypeSize * pathEncodedValueSize) * pathLength));        		
        	}
    		System.out.println("X_train:");
    		int n = modularPaths.size() + 1;
            for (int i = 0; i < n; i++)
            { 
            	int[] values;
            	if (i == 0)
            	{
            		values = basePath;
                	System.out.println("base path");            		
            	} else {
            		values = modularPaths.get(i - 1);
                	System.out.println("modular path " + (i - 1));            		
            	}
        		int[] valueFrame = new int[pathLength];
        		for (int j = 0; j < pathLength; j++)
        		{
        			valueFrame[j] = -1;
        		}
        		int vl = values.length;
        		if (PREDICT_PATH) vl--;
            	for (int j = 0; j < vl; j++)
            	{
	            	for (int k = 0; k < values.length; k++)
	        		{
	            		int idx = k + pathLength - j - 1;
	            		if (idx < pathLength)
	            		{
	            			valueFrame[idx] = values[k]; 
	            		}
	        		}
	            	if (DILATE_EVENTS == NO_DILATION)
	            	{	            		
		            	for (int k = 0; k < pathLength; k++)
		        		{
		            		if (valueFrame[k] != -1)
		            		{
			            		printWriter.print(oneHot(i, valueFrame[k]));
			            		System.out.print(i + ":" + valueFrame[k]);
		            		} else {
			            		printWriter.print(oneHot(-1, valueFrame[k]));
			            		System.out.print("?:-1");		            			
		            		}
		                	if (k < pathLength - 1)
		                	{
		                		printWriter.print(", ");
		                		System.out.print(", ");
		                	}            		
		        		}
		            	printWriter.println();
		            	System.out.println();
	            	} else {
		        		ArrayList<ArrayList<Integer>> typeAccum = new ArrayList<ArrayList<Integer>>();
		        		ArrayList<ArrayList<Integer>> valueAccum = new ArrayList<ArrayList<Integer>>();	        		
		        		for (int k = 0; k < pathLength; k++)
		        		{
		        			typeAccum.add(new ArrayList<Integer>());
		        			valueAccum.add(new ArrayList<Integer>());	        			
		        		}
		            	for (int k = 0; k < pathLength; k++)
		        		{
		            		if (valueFrame[k] != -1)
		            		{
			            		for (int p = 0; p <= k; p++)
			            		{
			            			typeAccum.get(p).add(i);
			            			valueAccum.get(p).add(valueFrame[k]);
			            		}
		            		}        		
		        		}
		            	for (int k = 0; k < pathLength; k++)
		        		{
		            		System.out.print("{");
		            		ArrayList<Integer> t = typeAccum.get(k);
		            		ArrayList<Integer> v = valueAccum.get(k);
		            		printWriter.print(multiHot(t, v));		            		
		            		for (int p = 0, q = t.size(); p < q; p++)
		            		{		            			
		            			System.out.print(t.get(p) + ":" + v.get(p));
		            			if (p < q - 1)
		            			{		            				
		            				System.out.print(",");
		            			}		
		            		}
		            		if (k < pathLength - 1)
		            		{
		                		printWriter.print(", ");		            			
		            		}
		                	System.out.print("} ");           		
		        		}
		            	printWriter.println();		            	
		            	System.out.println();	            		
	            	}
            	}
            }
            if (PREDICT_PATH)
            {
	        	if (DILATE_EVENTS == NO_DILATION)
	        	{            
	                printWriter.println("y_train_shape, " + numPaths + ", " + 
	                		(pathEncodedTypeSize + pathEncodedValueSize));
	        	} else {
	                printWriter.println("y_train_shape, " + numPaths + ", " +
	                		(pathEncodedTypeSize * pathEncodedValueSize));     		
	        	} 
            } else {
                printWriter.println("y_train_shape, " + numPaths + ", " + 
                		pathEncodedValueSize);            	
            }
    		System.out.println("y_train:");
            for (int i = 0; i < n; i++)
            {
               	int[] values;
               	if (PREDICT_PATH)
               	{
	            	if (i == 0)
	            	{
	            		values = basePath;
	                	System.out.println("base path");            		
	            	} else {
	            		values = modularPaths.get(i - 1);
	                	System.out.println("modular path " + (i - 1));            		
	            	}            	
	            	if (DILATE_EVENTS == NO_DILATION)
	            	{ 
		            	for (int j = 1, j2 = values.length; j < j2; j++)        		
		            	{
		            		printWriter.print(oneHot(i, values[j])); 
			            	System.out.print(i + ":" + values[j]);	            			
		                	printWriter.println();
		                	System.out.println();
		        		}
	            	} else {
		        		ArrayList<Integer> t = new ArrayList<Integer>();
		        		ArrayList<Integer> v = new ArrayList<Integer>();	           		
		            	for (int j = 1, j2 = values.length; j < j2; j++)        		
		            	{
		            		t.clear();
		            		v.clear();		            		
			            	t.add(i);	            			
		            		v.add(values[j]);
		            		printWriter.print(multiHot(t, v)); 
			            	System.out.print(i + ":" + values[j]);	            			            			
		                	printWriter.println();
		                	System.out.println();
		        		}           		
	            	}
               	} else {
	            	if (i == 0)
	            	{
	            		values = baseResponses;
	                	System.out.println("base responses");            		
	            	} else {
	            		values = modularResponses.get(i - 1);
	                	System.out.println("modular responses " + (i - 1));            		
	            	}            	          	
	            	for (int j = 0, j2 = values.length; j < j2; j++)        		
	            	{
	            		printWriter.println(oneHot(values[j])); 
		            	System.out.println(values[j]);	            			
	        		}        		
               	}
            }
            numPaths = 0;
            for (int[] p : testPaths)
            {
            	numPaths += p.length - 1;
            }
            if (!PREDICT_PATH)
            {
            	numPaths += testPaths.size();
            }
        	if (DILATE_EVENTS == NO_DILATION)
        	{             
	            printWriter.println("X_test_shape, " + numPaths + ", " + 
	            		((pathEncodedTypeSize + pathEncodedValueSize) * pathLength));
        	} else {
	            printWriter.println("X_test_shape, " + numPaths + ", " +
	            		((pathEncodedTypeSize * pathEncodedValueSize) * pathLength));        		
        	}
    		System.out.println("X_test:");
    		n = testPaths.size(); 
            for (int i = 0; i < n; i++)
            {
                System.out.println("path " + i);            		       	
            	int[] values = testPaths.get(i);
            	int[] types = testPathTypes.get(i);
        		int[] valuesFrame = new int[pathLength];
        		int[] typesFrame = new int[pathLength];
        		for (int j = 0; j < pathLength; j++)
        		{
        			valuesFrame[j] = -1;
        			typesFrame[j] = -1;
        		}
        		int vl = values.length;
        		if (PREDICT_PATH) vl--;        		
            	for (int j = 0; j < vl; j++)
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
	            	if (DILATE_EVENTS == NO_DILATION)
	            	{	            	
		            	for (int k = 0; k < pathLength; k++)
		        		{
		            		printWriter.print(oneHot(typesFrame[k], valuesFrame[k]));
		            		System.out.print(typesFrame[k] + ":" + valuesFrame[k]);
		                	if (k < pathLength - 1)
		                	{
		                		printWriter.print(", ");
		                		System.out.print(", ");
		                	}            		
		        		}
		            	printWriter.println(); 
		            	System.out.println();
	            	} else {
		        		ArrayList<ArrayList<Integer>> typeAccum = new ArrayList<ArrayList<Integer>>();
		        		ArrayList<ArrayList<Integer>> valueAccum = new ArrayList<ArrayList<Integer>>();	        		
		        		for (int k = 0; k < pathLength; k++)
		        		{
		        			typeAccum.add(new ArrayList<Integer>());
		        			valueAccum.add(new ArrayList<Integer>());	        			
		        		}
		            	for (int k = 0; k < pathLength; k++)
		        		{
		            		if (valuesFrame[k] != -1)
		            		{
			            		for (int p = 0; p <= k; p++)
			            		{
			            			typeAccum.get(p).add(typesFrame[k]);
			            			valueAccum.get(p).add(valuesFrame[k]);
			            		}
		            		}        		
		        		}
		            	for (int k = 0; k < pathLength; k++)
		        		{
		            		System.out.print("{");
		            		ArrayList<Integer> t = typeAccum.get(k);
		            		ArrayList<Integer> v = valueAccum.get(k);
		            		printWriter.print(multiHot(t, v));		            		
		            		for (int p = 0, q = t.size(); p < q; p++)
		            		{		            			
		            			System.out.print(t.get(p) + ":" + v.get(p));
		            			if (p < q - 1)
		            			{		            				
		            				System.out.print(",");
		            			}		
		            		}
		            		if (k < pathLength - 1)
		            		{
		                		printWriter.print(", ");		            			
		            		}
		                	System.out.print("} ");           		
		        		}
		            	printWriter.println();		            	
		            	System.out.println();	            		
	            	}
            	}
            }
            if (PREDICT_PATH)
            {
	        	if (DILATE_EVENTS == NO_DILATION)
	        	{            
		            printWriter.println("y_test_shape, " + numPaths + ", " + 
		            		(pathEncodedTypeSize + pathEncodedValueSize));
	        	} else {
		            printWriter.println("y_test_shape, " + numPaths + ", " + 
		            		(pathEncodedTypeSize * pathEncodedValueSize));        		
	        	}
            } else {
	            printWriter.println("y_test_shape, " + numPaths + ", " + 
	            		pathEncodedValueSize);             	
            }
    		System.out.println("y_test:");        	
            for (int i = 0; i < n; i++)
            {
            	if (PREDICT_PATH)
            	{
	                System.out.println("path " + i);               	
	            	int[] values = testPaths.get(i);
	            	int[] types = testPathTypes.get(i);
	            	if (DILATE_EVENTS == NO_DILATION)
	            	{ 
		            	for (int j = 1, j2 = values.length; j < j2; j++)
		        		{
		            		printWriter.print(oneHot(types[j], values[j]));
		            		System.out.print(types[j] + ":" + values[j]);	            			
		                	printWriter.println();
		                	System.out.println();
		        		}   			
	            	} else {
		        		ArrayList<Integer> t = new ArrayList<Integer>();
		        		ArrayList<Integer> v = new ArrayList<Integer>();	           		
		            	for (int j = 1, j2 = values.length; j < j2; j++)       		
		            	{
		            		t.clear();
		            		v.clear();		            		
			            	t.add(types[j]);	            			
		            		v.add(values[j]);
		            		printWriter.print(multiHot(t, v)); 
			            	System.out.print(types[j] + ":" + values[j]);	            			            			
		                	printWriter.println();
		                	System.out.println();
		        		}          		
	            	}
            	} else {
	                System.out.println("responses " + i);               	
	            	int[] values = testResponses.get(i);            	
	            	for (int j = 0, j2 = values.length; j < j2; j++)
	        		{
	            		printWriter.println(oneHot(values[j]));
	            		System.out.println(values[j]);	            			
	        		}   			           		
            	}
            }               
            printWriter.close();
    	} catch (IOException e)
    	{
    		System.err.println("Cannot write path dataset to file " + filename);
    		System.exit(1);
    	}    	
    }
    
    // Export dataset as python.
    public void exportPathNNdatasetPy(String filename)
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
    		int numPaths = basePath.length - 1;
            for (int[] p : modularPaths)
            {
            	if (pathLength < p.length)
            	{
            		pathLength = p.length;
            	}
            	numPaths += p.length - 1;
            }
            if (!PREDICT_PATH)
            {
            	numPaths += modularPaths.size() + 1;
            }
            for (int[] p : testPaths)
            {
            	if (pathLength < p.length)
            	{
            		pathLength = p.length;
            	}
            }            
        	if (DILATE_EVENTS == NO_DILATION)
        	{            
	            printWriter.println("X_train_shape = [ " + numPaths + ", " + 
	            		((pathEncodedTypeSize + pathEncodedValueSize) * pathLength) + " ]");
        	} else {
	            printWriter.println("X_train_shape = [ " + numPaths + ", " + 
	            		((pathEncodedTypeSize * pathEncodedValueSize) * pathLength) + " ]");        		
        	}
            printWriter.print("X_train = [ ");
            String X_train = "";
    		int n = modularPaths.size() + 1;
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
        		int vl = values.length;
        		if (PREDICT_PATH) vl--;
            	for (int j = 0; j < vl; j++)
            	{
	            	for (int k = 0; k < values.length; k++)
	        		{
	            		int idx = k + pathLength - j - 1;
	            		if (idx < pathLength)
	            		{
	            			valueFrame[idx] = values[k]; 
	            		}
	        		}
	            	if (DILATE_EVENTS == NO_DILATION)
	            	{	            		
		            	for (int k = 0; k < pathLength; k++)
		        		{
		            		if (valueFrame[k] != -1)
		            		{
			            		X_train += oneHot(i, valueFrame[k]);
		            		} else {
		            			X_train += oneHot(-1, valueFrame[k]);		            			
		            		}
		                	if (k < pathLength - 1 || j < vl - 1 || i < n - 1)
		                	{
		                		X_train += ", ";		                		
		                	}
		        		}		            	
	            	} else {
		        		ArrayList<ArrayList<Integer>> typeAccum = new ArrayList<ArrayList<Integer>>();
		        		ArrayList<ArrayList<Integer>> valueAccum = new ArrayList<ArrayList<Integer>>();	        		
		        		for (int k = 0; k < pathLength; k++)
		        		{
		        			typeAccum.add(new ArrayList<Integer>());
		        			valueAccum.add(new ArrayList<Integer>());	        			
		        		}
		            	for (int k = 0; k < pathLength; k++)
		        		{
		            		if (valueFrame[k] != -1)
		            		{
			            		for (int p = 0; p <= k; p++)
			            		{
			            			typeAccum.get(p).add(i);
			            			valueAccum.get(p).add(valueFrame[k]);
			            		}
		            		}        		
		        		}
		            	for (int k = 0; k < pathLength; k++)
		        		{
		            		ArrayList<Integer> t = typeAccum.get(k);
		            		ArrayList<Integer> v = valueAccum.get(k);
		            		X_train += multiHot(t, v);		            		
		                	if (k < pathLength - 1 || j < vl - 1 || i < n - 1)
		            		{
		                		X_train += ", ";		            			
		            		}          		
		        		}		            		            		
	            	}
            	}
            }
            if (X_train.endsWith(", "))
            {
            	X_train = X_train.substring(0, X_train.length() - 2);
            }            
            printWriter.println(X_train + " ]");   
            if (PREDICT_PATH)
            {
	        	if (DILATE_EVENTS == NO_DILATION)
	        	{            
	                printWriter.println("y_train_shape = [ " + numPaths + ", " + 
	                		(pathEncodedTypeSize + pathEncodedValueSize) + " ]");
	        	} else {
	                printWriter.println("y_train_shape = [ " + numPaths + ", " +
	                		(pathEncodedTypeSize * pathEncodedValueSize) + " ]");     		
	        	} 
            } else {
                printWriter.println("y_train_shape = [ " + numPaths + ", " + 
                		pathEncodedValueSize + " ]");            	
            }
            printWriter.print("y_train = [ ");
            String y_train = "";
            for (int i = 0; i < n; i++)
            {
               	int[] values;
               	if (PREDICT_PATH)
               	{
	            	if (i == 0)
	            	{
	            		values = basePath;           		
	            	} else {
	            		values = modularPaths.get(i - 1);            		
	            	}            	
	            	if (DILATE_EVENTS == NO_DILATION)
	            	{ 	            		
		            	for (int j = 1, j2 = values.length; j < j2; j++)
		        		{
		            		y_train += oneHot(i, values[j]);        		
			            	if (j < j2 - 1 || i < n - 1)
			            	{
			            		y_train += ", ";
			            	}
		        		}
	            	} else {
		        		ArrayList<Integer> t = new ArrayList<Integer>();
		        		ArrayList<Integer> v = new ArrayList<Integer>();	           		
		            	for (int j = 1, j2 = values.length; j < j2; j++)        		
		            	{
		            		t.clear();
		            		v.clear();		            		
			            	t.add(i);	            			
		            		v.add(values[j]); 	            			            			
		            		y_train += multiHot(t,v);
			            	if (j < j2 - 1 || i < n - 1)
			            	{
			            		y_train += ", ";
			            	}
		            	}			            	
	            	}
               	} else {
	            	if (i == 0)
	            	{
	            		values = baseResponses;           		
	            	} else {
	            		values = modularResponses.get(i - 1);           		
	            	}            	          	
	            	for (int j = 0, j2 = values.length; j < j2; j++)        		
	            	{
	            		y_train += oneHot(values[j]);		            	
		            	if (j < j2 - 1 || i < n - 1)
		            	{
		            		y_train += ", ";
		            	}             		
	        		}          	
               	}
            }
            if (y_train.endsWith(", "))
            {
            	y_train = y_train.substring(0, y_train.length() - 2);
            }            
            printWriter.println(y_train + " ]");                            
            numPaths = 0;
            for (int[] p : testPaths)
            {
            	numPaths += p.length - 1;
            }
            if (!PREDICT_PATH)
            {
            	numPaths += testPaths.size();
            }
        	if (DILATE_EVENTS == NO_DILATION)
        	{             
	            printWriter.println("X_test_shape = [ " + numPaths + ", " + 
	            		((pathEncodedTypeSize + pathEncodedValueSize) * pathLength) + " ]");
        	} else {
	            printWriter.println("X_test_shape = [ " + numPaths + ", " +
	            		((pathEncodedTypeSize * pathEncodedValueSize) * pathLength) + " ]");        		
        	}
        	printWriter.print("X_test = [ ");
            String X_test = "";
    		n = testPaths.size(); 
            for (int i = 0; i < n; i++)
            {            		       	
            	int[] values = testPaths.get(i);
            	int[] types = testPathTypes.get(i);
        		int[] valuesFrame = new int[pathLength];
        		int[] typesFrame = new int[pathLength];
        		for (int j = 0; j < pathLength; j++)
        		{
        			valuesFrame[j] = -1;
        			typesFrame[j] = -1;
        		}
        		int vl = values.length;
        		if (PREDICT_PATH) vl--;        		
            	for (int j = 0; j < vl; j++)
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
	            	if (DILATE_EVENTS == NO_DILATION)
	            	{	            	
		            	for (int k = 0; k < pathLength; k++)
		        		{
		            		X_test += oneHot(typesFrame[k], valuesFrame[k]);
		                	if (k < pathLength - 1 || j < vl - 1 || i < n - 1)
		                	{
		                		X_test += ", ";			                		
		                	}
		        		}
	            	} else {
		        		ArrayList<ArrayList<Integer>> typeAccum = new ArrayList<ArrayList<Integer>>();
		        		ArrayList<ArrayList<Integer>> valueAccum = new ArrayList<ArrayList<Integer>>();	        		
		        		for (int k = 0; k < pathLength; k++)
		        		{
		        			typeAccum.add(new ArrayList<Integer>());
		        			valueAccum.add(new ArrayList<Integer>());	        			
		        		}
		            	for (int k = 0; k < pathLength; k++)
		        		{
		            		if (valuesFrame[k] != -1)
		            		{
			            		for (int p = 0; p <= k; p++)
			            		{
			            			typeAccum.get(p).add(typesFrame[k]);
			            			valueAccum.get(p).add(valuesFrame[k]);
			            		}
		            		}        		
		        		}
		            	for (int k = 0; k < pathLength; k++)
		        		{
		            		ArrayList<Integer> t = typeAccum.get(k);
		            		ArrayList<Integer> v = valueAccum.get(k);
		            		X_test += multiHot(t, v);		            		
		                	if (k < pathLength - 1 || j < vl - 1 || i < n - 1)
		            		{
		                		X_test += ", ";		            			
		            		}          		
		        		}            		
	            	}
            	}
            }
            if (X_test.endsWith(", "))
            {
            	X_test = X_test.substring(0, X_test.length() - 2);
            }            
            printWriter.println(X_test + " ]");            
            if (PREDICT_PATH)
            {
	        	if (DILATE_EVENTS == NO_DILATION)
	        	{            
		            printWriter.println("y_test_shape = [ " + numPaths + ", " + 
		            		(pathEncodedTypeSize + pathEncodedValueSize) + " ]");
	        	} else {
		            printWriter.println("y_test_shape = [ " + numPaths + ", " + 
		            		(pathEncodedTypeSize * pathEncodedValueSize) + " ]");        		
	        	}
            } else {
	            printWriter.println("y_test_shape = [ " + numPaths + ", " + 
	            		pathEncodedValueSize + " ]");             	
            }
            printWriter.print("y_test = [ ");
            String y_test = "";
            for (int i = 0; i < n; i++)
            {            	
            	if (PREDICT_PATH)
            	{               	
	            	int[] values = testPaths.get(i);
	            	int[] types = testPathTypes.get(i);
	            	if (DILATE_EVENTS == NO_DILATION)
	            	{
		            	for (int j = 1, j2 = values.length; j < j2; j++)
		        		{
		            		y_test += oneHot(types[j], values[j]);
			            	if (j < j2 - 1 || i < n - 1)
			            	{
			            		y_test += ", ";
			            	}
		        		}   			
	            	} else {
		        		ArrayList<Integer> t = new ArrayList<Integer>();
		        		ArrayList<Integer> v = new ArrayList<Integer>();	           		
		            	for (int j = 1, j2 = values.length; j < j2; j++)       		
		            	{
		            		t.clear();
		            		v.clear();		            		
			            	t.add(types[j]);	            			
		            		v.add(values[j]);
		            		y_test += multiHot(t, v); 
			            	if (j < j2 - 1 || i < n - 1)
			            	{
			            		y_test += ", ";
			            	}
		        		}          		
	            	}
            	} else {              	
	            	int[] values = testResponses.get(i);            	
	            	for (int j = 0, j2 = values.length; j < j2; j++)
	        		{
	            		y_test += oneHot(values[j]);
		            	if (j < j2 - 1 || i < n - 1)
		            	{
		            		y_test += ", ";
		            	}	            			
	        		}   			           		
            	}            	
            }
            if (y_test.endsWith(", "))
            {
            	y_test = y_test.substring(0, y_test.length() - 2);
            }
            printWriter.println(y_test + " ]");            
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
            if (PREDICT_PATH)
            {
	            printWriter.println("y_train_shape = [ " + n + ", " + pathLength + ", " + 
	            		(pathEncodedTypeSize + pathEncodedValueSize) + " ]");
            } else {
	            printWriter.println("y_train_shape = [ " + n + ", " + pathLength + ", " + 
	            		pathEncodedValueSize + " ]");            	
            }
            printWriter.print("y_train_seq = [ ");
            for (int i = 0; i < n; i++)
            {
            	int[] values;
            	if (PREDICT_PATH)
            	{
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
            	} else {
	            	if (i == 0)
	            	{
	            		values = baseResponses;
	            	} else {
	            		values = modularResponses.get(i - 1);
	            	}
	            	for (int j = 0; j < pathLength; j++)
	        		{
	            		if (j < values.length)
	            		{
	            			printWriter.print(oneHot(values[j]));
	            		} else {
	            			printWriter.print(oneHot(-1));	            			
	            		}
		            	if (j < pathLength - 1 || i < n - 1)
		            	{
		            		printWriter.print(", ");
		            	}   	       		
	        		}            	   		
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
            if (PREDICT_PATH)
            {
	            printWriter.println("y_test_shape = [ " + n + ", " + pathLength + ", " + 
	            		(pathEncodedTypeSize + pathEncodedValueSize) + " ]");
            } else {
	            printWriter.println("y_test_shape = [ " + n + ", " + pathLength + ", " + 
	            		pathEncodedValueSize + " ]");            	
            }
            printWriter.print("y_test_seq = [ ");
            for (int i = 0; i < n; i++)
            {
            	if (PREDICT_PATH)
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
            	} else {
	            	int[] values = testResponses.get(i);
	            	for (int j = 0; j < pathLength; j++)
	        		{
	            		if (j < values.length) 
	            		{
	            			printWriter.print(oneHot(values[j]));
	            		} else {
	            			printWriter.print(oneHot(-1));	            			
	            		}
		            	if (j < pathLength - 1 || i < n - 1)
		            	{
		            		printWriter.print(", ");
		            	}   	            		          		
	        		}            	         		
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
    
    public boolean hotDebug = false;
    
    // One-hot coding of value.
    public String oneHot(int value)
    {
    	if (hotDebug)
    	{
    		return  value + ", ";
    	}
    	String code = "";
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
    
    // One-hot coding of path type and value.
    public String oneHot(int type, int value)
    {
    	if (hotDebug)
    	{
    		return  type + ":" + value + ", ";
    	}    	
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
    
    // Multi-hot coding of types and values.
    public String multiHot(ArrayList<Integer> types, ArrayList<Integer> values)
    {
    	String code = "";    	
    	if (hotDebug)
    	{
    		code = "{";
    		for (int i : types)
    		{
    			code += i + " ";
    		}
    		code += "}";
    		code += ":{";
    		for (int i : values)
    		{
    			code += i + " ";
    		}
    		code += "}, ";    		
    		return  code;
    	} 
		float[] hotValues = new float[pathEncodedValueSize * pathEncodedValueSize];    	
    	for (int i = 0; i < pathEncodedTypeSize; i++)
    	{   		
    		for (int j = 0, k = types.size(); j < k; j++)
    		{
    			int t = types.get(j);
    			if (t == i)
    			{
    				int v = values.get(j);
    				if (v != -1)
    				{
    					hotValues[(t * pathEncodedTypeSize) + v] += 1.0f;
    				}
    			}
    		}
    	}
    	float accum = 0.0f;
    	for (int i = 0; i < hotValues.length; i++)
    	{
    		if (DILATE_EVENTS == OVERLAY_DILATION)
    		{
    			if (hotValues[i] > 1.0f)
				{
					hotValues[i] = 1.0f;
				}
    		} else if (DILATE_EVENTS == NORMALIZE_DILATION)
    		{
				accum += hotValues[i];
    		}
    	}
    	if (DILATE_EVENTS == NORMALIZE_DILATION && accum > 0.0f) 
    	{
        	for (int i = 0; i < hotValues.length; i++)
        	{    		
        		hotValues[i] /= accum;
        	}
    	}
    	for (int i = 0; i < hotValues.length; i++)
    	{      	
    		code += hotValues[i] + "";
    		if (i < hotValues.length - 1)
    		{
    			code += ", ";
    		}
    	}
    	return code;
    }
}
