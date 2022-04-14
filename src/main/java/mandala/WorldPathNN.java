// For conditions of distribution and use, see copyright notice in Mandala.java

// World path neural network.

package mandala;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

import org.nd4j.linalg.api.ndarray.INDArray;

public class WorldPathNN
{
	// Dataset file name.
	public static String DATASET_FILE_NAME = "world_path_nn_dataset.csv";
	
	// Results file name.
	public static String RESULTS_FILE_NAME = "world_path_nn_results.json";
	
	// Dimensions of code and hidden vectors.
	public static int CODE_DIMENSION = 384;
	public static int HIDDEN_DIMENSION = 384;
	
    // Usage.
    public static final String Usage =
      "Usage:\n" +
      "    java mandala.WorldPathNN\n" +
      "      [-datasetFilename <file name> (default=" + DATASET_FILE_NAME + ")]\n" +     		  
      "      [-codeDimension <quantity> (default=" + CODE_DIMENSION + ")]\n" +
      "      [-hiddenDimension <quantity> (default=" + HIDDEN_DIMENSION + ")]\n" +
      "      [-resultsFilename <file name> (default=" + RESULTS_FILE_NAME + ")]\n" +      
      "Exit codes:\n" +
      "  0=success\n" +
      "  1=error";   
	
    // Main.
    public static void main(String[] args) 
    {
        for (int i = 0; i < args.length; i++)
        {
           if (args[i].equals("-datasetFilename"))
           {
              DATASET_FILE_NAME = args[i];
              continue;
           }       
           if (args[i].equals("-codeDimension"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid codeDimension option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 CODE_DIMENSION = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid codeDimension option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (CODE_DIMENSION < 0)
              {
                 System.err.println("Invalid codeDimension option");
                 System.err.println(Usage);
                 System.exit(1);
              }              
              continue;
           }
           if (args[i].equals("-hiddenDimension"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid hiddenDimension option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
            	  HIDDEN_DIMENSION = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid hiddenDimension option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (HIDDEN_DIMENSION < 0)
              {
                 System.err.println("Invalid hiddenDimension option");
                 System.err.println(Usage);
                 System.exit(1);
              }              
              continue;
           }
           if (args[i].equals("-resultsFilename"))
           {
              RESULTS_FILE_NAME = args[i];
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
        
        // Create NN.
        MandalaNN.CODE_DIMENSION = CODE_DIMENSION;
        MandalaNN.HIDDEN_DIMENSION = HIDDEN_DIMENSION;
        MandalaNN mandalaNN = new MandalaNN();
        
        // Import dataset file.
        mandalaNN.importDataset(DATASET_FILE_NAME);
        
        // Build NN.
        mandalaNN.build();
        
        // Train.
        mandalaNN.train(1000);
        
        // Validate.
        mandalaNN.validate();
                
        // Test.
        mandalaNN.test();
        
        // Results.
        INDArray effects = mandalaNN.trainEffectData;
        INDArray predictions = mandalaNN.trainPredictions;
        int trainErrors = 0;
        int trainTotal = 0;
        for (int i = 0, j = predictions.rows(); i < j; i++)
        { 
    		if (maxValIdx(effects.getRow(i)) != maxValIdx(predictions.getRow(i)))
			{
    			trainErrors++;			
			}
    		trainTotal++;     		
        }
        System.out.print("Train prediction errors/total = " + trainErrors + "/" + trainTotal);
        float trainErrorPct = 0.0f;
        if (trainTotal > 0)
        {
            trainErrorPct = ((float)trainErrors / (float)trainTotal) * 100.0f;
            System.out.print(" (");
            System.out.printf("%.2f", trainErrorPct);
            System.out.print("%)"); 
        }
        System.out.println();
        effects = mandalaNN.testEffectData;
        predictions = mandalaNN.testPredictions;
        int testErrors = 0;
        int testTotal = 0;
        for (int i = 0, j = predictions.rows(); i < j; i++)
        { 
    		if (maxValIdx(effects.getRow(i)) != maxValIdx(predictions.getRow(i)))
			{
    			testErrors++;			
			}
    		testTotal++;     		
        }
        System.out.print("Test prediction errors/total = " + testErrors + "/" + testTotal);
        float testErrorPct = 0.0f;
        if (testTotal > 0)
        {
            testErrorPct = ((float)testErrors / (float)testTotal) * 100.0f;
            System.out.print(" (");
            System.out.printf("%.2f", testErrorPct);
            System.out.print("%)"); 
        }
        System.out.println();        
		try
		{
	        FileWriter fileWriter = new FileWriter(RESULTS_FILE_NAME);
	        PrintWriter printWriter = new PrintWriter(fileWriter);
	        printWriter.write("{");        	        
	        printWriter.write("\"train_prediction_errors\":\"" + trainErrors + "\",");	        
	        printWriter.write("\"train_total_predictions\":\"" + trainTotal + "\",");	  	        
	        printWriter.write("\"train_error_pct\":\"" + trainErrorPct + "\",");	        	        	        
	        printWriter.write("\"test_prediction_errors\":\"" + testErrors + "\",");	        
	        printWriter.write("\"test_total_predictions\":\"" + testTotal + "\",");	  	        
	        printWriter.write("\"test_error_pct\":\"" + testErrorPct + "\"");	        	        
	        printWriter.write("}\n");    
	        printWriter.close();
		} catch (IOException e)
		{
			System.err.println("Cannot write results to file " + RESULTS_FILE_NAME);
			System.exit(1);
		}
		
        System.exit(0);
    }
    
    // Get index of max value in row.
    public static int maxValIdx(INDArray row)
    {
    	if (row.length() == 0)
    	{
    		System.err.println("maxValIdx: empty row");
    		System.exit(1);
    	}
    	int maxIdx = 0;
    	float maxVal = row.getFloat(0);
    	for (int i = 1; i < row.length(); i++)
    	{
    		float val = row.getFloat(i);
    		if (val > maxVal)
    		{
    			maxIdx = i;
    			maxVal = val;
    		}
    	}
    	return maxIdx;    	
    }
}
