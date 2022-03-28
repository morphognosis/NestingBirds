// For conditions of distribution and use, see copyright notice in Mandala.java

// Mandala neural network.

package mandala;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Collections;
import org.deeplearning4j.nn.conf.MultiLayerConfiguration;
import org.deeplearning4j.nn.conf.NeuralNetConfiguration;
import org.deeplearning4j.nn.conf.inputs.InputType;
import org.deeplearning4j.nn.conf.layers.DenseLayer;
import org.deeplearning4j.nn.conf.layers.OutputLayer;
import org.deeplearning4j.nn.multilayer.MultiLayerNetwork;
import org.deeplearning4j.nn.weights.WeightInit;
import org.deeplearning4j.optimize.listeners.ScoreIterationListener;
import org.nd4j.linalg.activations.Activation;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.dataset.DataSet;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.linalg.learning.config.Adam;
import org.nd4j.linalg.lossfunctions.LossFunctions;

public class MandalaNN
{	
	// Dimensions of cause (input), code (encoded cause-effect), and effect (output) vectors.
	public static int CAUSE_DIMENSION = 16;
	public static int CODE_DIMENSION = 8;
	public static int EFFECT_DIMENSION = 16;
	
	// Hidden layer dimension.
	public static int HIDDEN_DIMENSION = 128;
	
    // Network model.
    public MultiLayerNetwork causationModel;
    
    // Datasets. 
	public DataSet trainDataset;
	public INDArray causePredictionData;
	public INDArray effectPredictionData;
	
	// Training epochs.
	public int EPOCHS = 1000;
	
    // Usage.
    public static final String Usage =
      "Usage:\n" +
      "    java mandala.MandalaNN\n" + 
      "      -datasetFilename <file name>\n" +     		  
      "      [-causeDimension <quantity> (default=" + CAUSE_DIMENSION + ")]\n" +
      "      [-codeDimension <quantity> (default=" + CODE_DIMENSION + ")]\n" +
      "      [-effectDimension <quantity> (default=" + EFFECT_DIMENSION + ")]\n" +
      "      [-hiddenDimension <quantity> (default=" + HIDDEN_DIMENSION + ")]\n" +                   
      "Exit codes:\n" +
      "  0=success\n" +
      "  1=error";   
	
	// Constructors.
	public MandalaNN(int causeDim, int codeDim, int effectDim, int hiddenDim)
	{
		CAUSE_DIMENSION = causeDim;
		CODE_DIMENSION = codeDim;
		EFFECT_DIMENSION = effectDim;
		HIDDEN_DIMENSION = hiddenDim;
		build();
	}
	
    public MandalaNN() 
    {
    	build();
    }
    
    // Build NN.
    public void build()
    {   
        // Configure model.
        MultiLayerConfiguration conf = new NeuralNetConfiguration.Builder()
                .seed(12345)
                .weightInit(WeightInit.XAVIER)
                .updater(new Adam(1e-3))
                .list()
                .layer(0, new DenseLayer.Builder().nIn(CAUSE_DIMENSION).nOut(HIDDEN_DIMENSION)
                		.activation(Activation.RELU)
                        .build())
                .layer(1, new DenseLayer.Builder().nIn(HIDDEN_DIMENSION).nOut(CODE_DIMENSION)
                		.activation(Activation.RELU)
                		.l1(10e-5)                		
                        .build())
                .layer(2, new DenseLayer.Builder().nIn(CODE_DIMENSION).nOut(HIDDEN_DIMENSION)                		
                		.activation(Activation.RELU)
                        .build())
                .layer(3, new DenseLayer.Builder().nIn(HIDDEN_DIMENSION).nOut(HIDDEN_DIMENSION)                		
                		.activation(Activation.RELU)
                        .build())                   
                .layer(4, new OutputLayer.Builder().nIn(HIDDEN_DIMENSION).nOut(EFFECT_DIMENSION)
                        .activation(Activation.SIGMOID)
                        .lossFunction(LossFunctions.LossFunction.MSE)
                        .build())
                .setInputType(InputType.feedForward(CAUSE_DIMENSION))                
                .build();

        // Build model.
        causationModel = new MultiLayerNetwork(conf);
        causationModel.init();
        causationModel.setListeners(Collections.singletonList(new ScoreIterationListener(10)));
    }
    
    /* 
       Import dataset file.
       File format:
       X_train_shape=[<number of vectors>,<cause vector dimension>]
       <cause vectors>
       y_train_shape=[<number of vectors>,<effect vector dimension>]
       <effect vectors>
       Optionally:
       X_test_shape=[<number of vectors>,<cause vector dimension>]
       <cause vectors>
       y_test_shape=[<number of vectors>,<effect vector dimension>]
       <effect vectors>
     */
    public void importDataset(String filename)
    {    	
    	try (BufferedReader br = new BufferedReader(new FileReader(filename))) 
    	{
    	    String line;
    	    if ((line = br.readLine()) == null)
    	    {
	           System.err.println("Cannot read X train shape from file " + filename);
	           System.exit(1);    	    	
    	    }
    	    if (!line.startsWith("X_train_shape"))
    	    {
 	           System.err.println("Invalid X train shape from file " + filename);
 	           System.exit(1);     	    	
    	    }
    	    String[] parts = line.split(",");
    	    int numCause = Integer.parseInt(parts[1].trim());
    	    int causeDim = Integer.parseInt(parts[2].trim());
    	    INDArray causeData = Nd4j.create(numCause, causeDim);
    	    float[] values = new float[causeDim];
    	    for (int i = 0; i < numCause; i++)
    	    {
        	    if ((line = br.readLine()) == null)
        	    {
    	           System.err.println("Cannot read X train data from file " + filename);
    	           System.exit(1);    	    	
        	    }
        	    parts = line.split(",");
        	    for (int j = 0; j < causeDim; j++)
    	    	{
        	    	values[j] = Float.parseFloat(parts[j].trim());
    	    	}
            	causeData.putRow(i, Nd4j.createFromArray(values));    	    	
    	    }
    	    if ((line = br.readLine()) == null)
    	    {
	           System.err.println("Cannot read y train shape from file " + filename);
	           System.exit(1);    	    	
    	    }
    	    if (!line.startsWith("y_train_shape"))
    	    {
 	           System.err.println("Invalid y train shape from file " + filename);
 	           System.exit(1);     	    	
    	    }
    	    parts = line.split(",");
    	    int numEffect = Integer.parseInt(parts[1].trim());
    	    if (numCause != numEffect)
    	    {
  	           System.err.println("X and y training data must have equal number of vectors");
  	           System.exit(1);     	    	
    	    }
    	    int effectDim = Integer.parseInt(parts[2].trim());
    	    INDArray effectData = Nd4j.create(numEffect, effectDim);
    	    values = new float[effectDim];
    	    for (int i = 0; i < numEffect; i++)
    	    {
        	    if ((line = br.readLine()) == null)
        	    {
    	           System.err.println("Cannot read y train data from file " + filename);
    	           System.exit(1);    	    	
        	    } 
        	    parts = line.split(",");
        	    for (int j = 0; j < effectDim; j++)
    	    	{
        	    	values[j] = Float.parseFloat(parts[j].trim());
    	    	}
            	effectData.putRow(i, Nd4j.createFromArray(values));    	    	
    	    }
    	    
    	    // Create training dataset.
        	trainDataset = new DataSet(causeData, effectData);
        	
        	// Testing data?
    	    if ((line = br.readLine()) == null)
    	    {
	           return;    	    	
    	    }
    	    if (!line.startsWith("X_test_shape"))
    	    {
 	           System.err.println("Invalid X test shape from file " + filename);
 	           System.exit(1);     	    	
    	    }
    	    parts = line.split(",");
    	    numCause = Integer.parseInt(parts[1].trim());
    	    int n = Integer.parseInt(parts[2].trim());
    	    if (n != causeDim)
    	    {
   	           System.err.println("X train and X test vectors must have equal dimensions");
   	           System.exit(1);     	    	
    	    }
    	    causeDim = n;
    	    causePredictionData = Nd4j.create(numCause, causeDim);
    	    values = new float[causeDim];
    	    for (int i = 0; i < numCause; i++)
    	    {
        	    if ((line = br.readLine()) == null)
        	    {
    	           System.err.println("Cannot read X test data from file " + filename);
    	           System.exit(1);    	    	
        	    } 
        	    parts = line.split(",");
        	    for (int j = 0; j < causeDim; j++)
    	    	{
        	    	values[j] = Float.parseFloat(parts[j].trim());
    	    	}
            	causePredictionData.putRow(i, Nd4j.createFromArray(values));    	    	
    	    }
    	    if ((line = br.readLine()) == null)
    	    {
	           System.err.println("Cannot read y test shape from file " + filename);
	           System.exit(1);    	    	
    	    }
    	    if (!line.startsWith("y_test_shape"))
    	    {
 	           System.err.println("Invalid y test shape from file " + filename);
 	           System.exit(1);     	    	
    	    }
    	    parts = line.split(",");
    	    n = Integer.parseInt(parts[1].trim());
    	    if (n != effectDim)
    	    {
   	           System.err.println("y train and y test vectors must have equal dimensions");
   	           System.exit(1);     	    	
    	    }    	    
    	    numEffect = n;
    	    if (numCause != numEffect)
    	    {
  	           System.err.println("X and y testing data must have equal number of vectors");
  	           System.exit(1);     	    	
    	    }
    	    effectDim = Integer.parseInt(parts[2].trim());
    	    effectPredictionData = Nd4j.create(numEffect, effectDim);
    	    values = new float[effectDim];
    	    for (int i = 0; i < numEffect; i++)
    	    {
        	    if ((line = br.readLine()) == null)
        	    {
    	           System.err.println("Cannot read y test data from file " + filename);
    	           System.exit(1);    	    	
        	    } 
        	    parts = line.split(",");
        	    for (int j = 0; j < effectDim; j++)
    	    	{
        	    	values[j] = Float.parseFloat(parts[j].trim());
    	    	}
            	effectPredictionData.putRow(i, Nd4j.createFromArray(values));    	    	
    	    }    	     	    
        }
        catch (Exception e)
        {
           System.err.println("Cannot import dataset from file " + filename + ": " + e.getMessage());
           System.exit(1);
        }
     }
	       
    // Train NN.
    public void train()
    {
    	train(EPOCHS);    
    }
    
    public void train(int epochs)
    {
    	if (trainDataset == null)
    	{
            System.err.println("No training dataset");
            System.exit(1);    		
    	}
    	System.out.println("Training:");
        for (int epoch=0; epoch < epochs; epoch++)
        {
            causationModel.fit(trainDataset);
        }
    }
    
    // Test NN.
    public void test()
    {
    	if (causePredictionData == null)
    	{
            System.err.println("No test dataset");
            System.exit(1);    		
    	}
    	System.out.println("Testing:");
    	System.out.println("causes: " + causePredictionData);        
        INDArray predictions = causationModel.output(causePredictionData);   	        
        System.out.println("predictions: " + predictions);        
    	System.out.println("effects: " + effectPredictionData);
        INDArray codes = causationModel.activateSelectedLayers(0, 1, causePredictionData);
        System.out.println("codes: " + codes);    	
    }
    
    // Predict effect.
    public INDArray predict(INDArray cause)
    {       
        return causationModel.output(cause);   	        
    }
    
    // Encode cause-effect.
    public INDArray encode(INDArray cause)
    {               
        return causationModel.activateSelectedLayers(0, 1, cause);
    }
	
    // Main.
    public static void main(String[] args) 
    {
    	String datasetFilename = null;

        for (int i = 0; i < args.length; i++)
        {
           if (args[i].equals("-datasetFilename"))
           {
              datasetFilename = args[i];
              continue;
           }       
           if (args[i].equals("-causeDimension"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid causeDimension option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 CAUSE_DIMENSION = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid causeDimension option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (CAUSE_DIMENSION < 0)
              {
                 System.err.println("Invalid causeDimension option");
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
        datasetFilename = "world_path_tdnn_dataset.csv"; // flibber
        CAUSE_DIMENSION = 338;  // flibber
        CODE_DIMENSION = 128;
        HIDDEN_DIMENSION = 128;
        EFFECT_DIMENSION = 26;
        if (datasetFilename == null)
        {
            System.err.println(Usage);
            System.exit(1);        	
        }
        
        // Create NN.
        MandalaNN mandalaNN = new MandalaNN();
        
        // Import dataset file.
        mandalaNN.importDataset(datasetFilename);
        
        // Train.
        mandalaNN.train(1000);
        
        // Test.
        mandalaNN.test();       
        
        System.exit(0);
    }
}
