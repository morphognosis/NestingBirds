// For conditions of distribution and use, see copyright notice in Main.java

// Mandala is a hybrid of Morphognosis and Mona.

package mandala;

import java.util.ArrayList;
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

public class Mandala
{
	// Dimensions of cause, code, and effect.
	public static int cause_dim = 16;
	public static int code_dim = 8;
	public static int effect_dim = 16;

	// Hidden layer dimensions.
	public static int hidden_dim = 128;

	// Causations.
	public static int num_causations = 2;
	public static int num_cause_features = 2;
	public static int num_effect_features = 2;

	// Dataset size.
	public static int dataset_size = num_causations + 1;

    public static void main(String[] args) 
    {
        // Configure model.
        MultiLayerConfiguration conf = new NeuralNetConfiguration.Builder()
                .seed(12345)
                .weightInit(WeightInit.XAVIER)
                .updater(new Adam(1e-3))
                .list()
                .layer(0, new DenseLayer.Builder().nIn(cause_dim).nOut(hidden_dim)
                		.activation(Activation.RELU)
                        .build())
                .layer(1, new DenseLayer.Builder().nIn(hidden_dim).nOut(code_dim)
                		.activation(Activation.RELU)
                		.l1(10e-5)                		
                        .build())
                .layer(2, new DenseLayer.Builder().nIn(code_dim).nOut(hidden_dim)                		
                		.activation(Activation.RELU)
                        .build())
                .layer(3, new DenseLayer.Builder().nIn(hidden_dim).nOut(hidden_dim)                		
                		.activation(Activation.RELU)
                        .build())                   
                .layer(4, new OutputLayer.Builder().nIn(hidden_dim).nOut(effect_dim)
                        .activation(Activation.SIGMOID)
                        .lossFunction(LossFunctions.LossFunction.MSE)
                        .build())
                .setInputType(InputType.feedForward(cause_dim))                
                .build();

        // Build network model.
        MultiLayerNetwork causation_model = new MultiLayerNetwork(conf);
        causation_model.init();
        causation_model.setListeners(Collections.singletonList(new ScoreIterationListener(10)));

        // Generate cause and effect pairs.
        // off=0.0, on=1.0
        ArrayList<ArrayList<Integer>> cause_feature_idxs = new ArrayList<ArrayList<Integer>>();
        ArrayList<Integer> shuffle_idxs = new ArrayList<Integer>();
        for (int i = 0; i < cause_dim; i++)
        {
        	shuffle_idxs.add(i);
        }
        for (int i = 0; i < num_causations; i++)
        {
            while (true)
            {
        		Collections.shuffle(shuffle_idxs);
                ArrayList<Integer> test_idxs = new ArrayList<Integer>();
                for (int j = 0; j < num_cause_features; j++)
                {
                	test_idxs.add(shuffle_idxs.get(j));
                }
                Collections.sort(test_idxs);
                boolean found = false;
                for (int j = 0; j < cause_feature_idxs.size() && !found; j++)
                {
                	ArrayList<Integer> idxs = cause_feature_idxs.get(j);
                	for (int k = 0; k < test_idxs.size(); k++)
                	{
                		if (idxs.contains(test_idxs.get(k)))
                		{
                			found = true;
                			break;
                		}
                	}
                }
                if (!found)
                {
                    cause_feature_idxs.add(test_idxs);
                    break;
                }
            }
        }
        ArrayList<ArrayList<Integer>> effect_feature_idxs = new ArrayList<ArrayList<Integer>>();
        shuffle_idxs = new ArrayList<Integer>();
        for (int i = 0; i < effect_dim; i++)
        {
        	shuffle_idxs.add(i);
        }
        for (int i = 0; i < num_causations; i++)
        {
            while (true)
            {
        		Collections.shuffle(shuffle_idxs);
                ArrayList<Integer> test_idxs = new ArrayList<Integer>();
                for (int j = 0; j < num_effect_features; j++)
                {
                	test_idxs.add(shuffle_idxs.get(j));
                }
                Collections.sort(test_idxs);
                boolean found = false;
                for (int j = 0; j < effect_feature_idxs.size() && !found; j++)
                {
                	ArrayList<Integer> idxs = effect_feature_idxs.get(j);
                	for (int k = 0; k < idxs.size(); k++)
                	{
                		if (idxs.contains(test_idxs.get(k)))
                		{
                			found = true;
                			break;
                		}
                	}
                }
                if (!found)
                {
                    effect_feature_idxs.add(test_idxs);
                    break;
                }
            }
        }        
        INDArray cause_data = Nd4j.create(dataset_size, cause_dim);
        float[] vals = new float[cause_dim];
        float[] accum_vals = new float[cause_dim];
    	for (int j = 0; j < cause_dim; j++)
    	{       
    		accum_vals[j] = 0.0f;
    	}                
        for (int i = 0; i < cause_feature_idxs.size(); i++)
        {
        	for (int j = 0; j < cause_dim; j++)
        	{       
        		vals[j] = 0.0f;
        	}            	
        	ArrayList<Integer> idxs = cause_feature_idxs.get(i);    	
        	for (int j = 0; j < num_cause_features; j++)
        	{
        		int k = idxs.get(j);
        		vals[k] = 1.0f;
        		accum_vals[k] = 1.0f;
        	}
        	cause_data.putRow(i, Nd4j.createFromArray(vals));
        }
    	cause_data.putRow(dataset_size - 1, Nd4j.createFromArray(accum_vals));        
        INDArray effect_data = Nd4j.create(dataset_size, effect_dim);        
        vals = new float[effect_dim];
        accum_vals = new float[effect_dim];
    	for (int j = 0; j < effect_dim; j++)
    	{       
    		accum_vals[j] = 0.0f;
    	}                
        for (int i = 0; i < effect_feature_idxs.size(); i++)
        {
        	for (int j = 0; j < effect_dim; j++)
        	{       
        		vals[j] = 0.0f;
        	}            	
        	ArrayList<Integer> idxs = effect_feature_idxs.get(i);    	
        	for (int j = 0; j < num_effect_features; j++)
        	{
        		int k = idxs.get(j);
        		vals[k] = 1.0f;
        		accum_vals[k] = 1.0f;
        	}
        	effect_data.putRow(i, Nd4j.createFromArray(vals));
        }
    	effect_data.putRow(dataset_size - 1, Nd4j.createFromArray(accum_vals));
    	
        // Train model.    	
    	DataSet train_data = new DataSet(cause_data, effect_data);
        int nEpochs = 100;
        for (int epoch=0; epoch < nEpochs; epoch++)
        {
            causation_model.fit(train_data);
            System.out.println("Epoch " + epoch + " complete");
        }
        
        // Predict.
    	System.out.println("causations: " + train_data);        
        INDArray predictions = causation_model.output(cause_data);   	        
        System.out.println("predictions: " + predictions);        
        INDArray codes = causation_model.activateSelectedLayers(0, 1, cause_data);
        System.out.println("codes: " + codes);
    }
}
