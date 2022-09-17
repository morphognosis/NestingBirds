/*
 * Copyright (c) 2022-2023 Tom Portegys (portegys@gmail.com). All rights reserved.
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
 
// Mandala is a hybrid of Morphognosis and Mona.

package mandala;

import java.util.ArrayList;
import java.util.Collections;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.dataset.DataSet;
import org.nd4j.linalg.factory.Nd4j;

public class Mandala
{
    // Version.
    public static final String VERSION = "1.0";
	
	// Causations.
	public static int num_causations = 2;
	public static int num_cause_features = 2;
	public static int num_effect_features = 2;
	
	// Dataset size.
	public static int dataset_size = num_causations + 1;
	
	// Neural network.
	public static MandalaNN mandalaNN;
	
    public static void main(String[] args) 
    {
    	// Create network.
    	mandalaNN = new MandalaNN();

        // Generate cause and effect pairs.
        // off=0.0, on=1.0
        ArrayList<ArrayList<Integer>> cause_feature_idxs = new ArrayList<ArrayList<Integer>>();
        ArrayList<Integer> shuffle_idxs = new ArrayList<Integer>();
        for (int i = 0; i < mandalaNN.CAUSE_DIMENSION; i++)
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
        for (int i = 0; i < mandalaNN.EFFECT_DIMENSION; i++)
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
        INDArray cause_data = Nd4j.create(dataset_size, mandalaNN.CAUSE_DIMENSION);
        float[] vals = new float[mandalaNN.CAUSE_DIMENSION];
        float[] accum_vals = new float[mandalaNN.CAUSE_DIMENSION];
    	for (int j = 0; j < mandalaNN.CAUSE_DIMENSION; j++)
    	{       
    		accum_vals[j] = 0.0f;
    	}                
        for (int i = 0; i < cause_feature_idxs.size(); i++)
        {
        	for (int j = 0; j < mandalaNN.CAUSE_DIMENSION; j++)
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
        INDArray effect_data = Nd4j.create(dataset_size, mandalaNN.EFFECT_DIMENSION);        
        vals = new float[mandalaNN.EFFECT_DIMENSION];
        accum_vals = new float[mandalaNN.EFFECT_DIMENSION];
    	for (int j = 0; j < mandalaNN.EFFECT_DIMENSION; j++)
    	{       
    		accum_vals[j] = 0.0f;
    	}                
        for (int i = 0; i < effect_feature_idxs.size(); i++)
        {
        	for (int j = 0; j < mandalaNN.EFFECT_DIMENSION; j++)
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
        mandalaNN.trainDataset = new DataSet(cause_data, effect_data);
        mandalaNN.trainCauseData = cause_data;
        mandalaNN.trainEffectData = effect_data;
        mandalaNN.testCauseData = cause_data;
        mandalaNN.testEffectData = effect_data;
        
        // Build network.
        mandalaNN.build();
        		
        // Train network.
    	mandalaNN.train(100);
       
        // Test.      
        mandalaNN.test();
    }
}
