// For conditions of distribution and use, see copyright notice in Mandala.java

// World grammar.
// Generate a grammar that can produce paths from a starting state to a goal state in the world.

package mandala;

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

import org.apache.commons.lang3.StringUtils;
import org.nd4j.linalg.primitives.Pair;

public class WorldGrammar
{
	// Grammar generation parameters.
	public static int NUM_NONTERMINALS = 5;
	public static int MIN_NONTERMINAL_PRODUCTIONS = 1;		
	public static int MAX_NONTERMINAL_PRODUCTIONS = 3;	
	public static int MIN_PRODUCTION_RHS_LENGTH = 2;
	public static int MAX_PRODUCTION_RHS_LENGTH = 5;
	
	// Path expansion parameters.
	public static int NUM_PATHS = 5;
	public static int NUM_PATH_EXPANSIONS = 3;
	
	// Dataset exports.
	public static String PATH_RNN_DATASET_FILENAME = "world_path_rnn_dataset.py";
	public static float PATH_RNN_DATASET_TRAIN_FRACTION = 0.75f;
	public static String PATH_TCN_DATASET_FILENAME = "world_path_tcn_dataset.py";
	public static float PATH_TCN_DATASET_TRAIN_FRACTION = 0.75f;	
	public static String PATH_TDNN_DATASET_FILENAME = "world_path_tdnn_dataset.py";
	public static int TDNN_FRAME_LENGTH = 4;
	public static float PATH_TDNN_DATASET_TRAIN_FRACTION = 0.75f;
	
	// Random numbers.
	public static int RANDOM_SEED = 4517;
	public Random randomizer = null;
	
	// Verbosity.
	public static boolean VERBOSE = true;
	
	// File.
	public static String GRAMMAR_FILENAME = null;	
	
	// Grammar.
	public Map<String, List<String>> grammar;
	
	// World paths production.
	public static String INITIAL_WORLD_PATH = "sAg";
	public List<String> worldPaths;

    // Usage.
    public static final String Usage =
      "Usage:\n" +
      "    java mandala.WorldGrammar\n" +
      "      Grammar:\n" +    		  
      "        -generateGrammar (terminals: [a-z] s=start/g=goal, nonterminals: [A-Z])\n" +
      "          [-numNonterminals <quantity> (default=" + NUM_NONTERMINALS + ")]\n" +
      "          [-minNonterminalProductions <quantity> (default=" + MIN_NONTERMINAL_PRODUCTIONS + ")]\n" +
      "          [-maxNonterminalProductions <quantity> (default=" + MAX_NONTERMINAL_PRODUCTIONS + ")]\n" +
      "          [-minProductionRightHandSideLength <quantity> (default=" + MIN_PRODUCTION_RHS_LENGTH + ")]\n" +
      "          [-maxProductionRightHandSideLength <quantity> (default=" + MAX_PRODUCTION_RHS_LENGTH + ")]\n" +
      "          [-saveGrammar <file name>]\n" +       
      "        | -loadGrammar <file name>\n" +
      "      World path expansion:\n" +
      "          [-initialPath <string of terminals and nonterminals>\n" +
      "              (starting with unique terminal \"s\"tart and ending with unique terminal \"g\"oal, default=\"" + INITIAL_WORLD_PATH + "\")]\n" +      
      "          [-numPaths <quantity> (default=" + NUM_PATHS + ")]\n" +
      "          [-numPathExpansions <quantity> (number of breadth first expansions, default=" + NUM_PATH_EXPANSIONS + ")]\n" +      
      "          [-exportPathRNNdataset [<file name (default=\"" + PATH_RNN_DATASET_FILENAME + "\")>]\n" +
      "              [-RNNdatasetTrainFraction <fraction> (default=" + PATH_RNN_DATASET_TRAIN_FRACTION + ")]]\n" +
      "          [-exportPathTCNdataset [<file name (default=\"" + PATH_TCN_DATASET_FILENAME + "\")>]\n" +
      "              [-TCNdatasetTrainFraction <fraction> (default=" + PATH_TCN_DATASET_TRAIN_FRACTION + ")]]\n" +      
      "          [-exportPathTDNNdataset [<file name (default=\"" + PATH_TDNN_DATASET_FILENAME + "\")>]\n" +
      "              [-TDNNframeLength <length> (default=" + TDNN_FRAME_LENGTH + ")]]\n" +      
      "              [-TDNNdatasetTrainFraction <fraction> (default=" + PATH_TDNN_DATASET_TRAIN_FRACTION + ")]]\n" +      
      "      [-randomSeed <seed> (default=" + RANDOM_SEED + ")]\n" +
      "      [-verbose <\"true\" | \"false\"> (verbosity, default=" + VERBOSE + ")]\n" +      
      "Exit codes:\n" +
      "  0=success\n" +
      "  1=error";
	
    // Main.
    public static void main(String[] args) 
    {
    	boolean generate = false;
    	boolean gotGenparm = false;
    	boolean load = false;
    	boolean gotExportPathRNNdataset = false;
    	boolean gotRNNdatasetTrainFraction = false;
    	boolean gotExportPathTCNdataset = false;
    	boolean gotTCNdatasetTrainFraction = false;    	
    	boolean gotExportPathTDNNdataset = false;
    	boolean gotTDNNframeLength = false;
    	boolean gotTDNNdatasetTrainFraction = false;
    	
        for (int i = 0; i < args.length; i++)
        {
           if (args[i].equals("-generateGrammar"))
           {
              generate = true;
              continue;
           }       
           if (args[i].equals("-numNonterminals"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid numNonterminals option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 NUM_NONTERMINALS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid numNonterminals option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (NUM_NONTERMINALS < 0)
              {
                 System.err.println("Invalid numNonterminals option");
                 System.err.println(Usage);
                 System.exit(1);
              }              
              if (NUM_NONTERMINALS > 24)
              {
                 System.err.println("Maximum of 24 nonterminals");
                 System.err.println(Usage);
                 System.exit(1);
              }
              gotGenparm = true;
              continue;
           }
           if (args[i].equals("-minNonterminalProductions"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid minNonterminalProductions option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 MIN_NONTERMINAL_PRODUCTIONS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid minNonterminalProductions option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (MIN_NONTERMINAL_PRODUCTIONS < 0)
              {
                 System.err.println("Invalid minNonterminalProductions option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              gotGenparm = true;
              continue;
           }
           if (args[i].equals("-maxNonterminalProductions"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid maxNonterminalProductions option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 MAX_NONTERMINAL_PRODUCTIONS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid maxNonterminalProductions option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (MAX_NONTERMINAL_PRODUCTIONS < 0)
              {
                 System.err.println("Invalid maxNonterminalProductions option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              gotGenparm = true;
              continue;
           }
           if (args[i].equals("-minProductionRightHandSideLength"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid minProductionRightHandSideLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 MIN_PRODUCTION_RHS_LENGTH = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid minProductionRightHandSideLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (MIN_PRODUCTION_RHS_LENGTH < 0)
              {
                 System.err.println("Invalid minProductionRightHandSideLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              gotGenparm = true;
              continue;
           }
           if (args[i].equals("-maxProductionRightHandSideLength"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid maxProductionRightHandSideLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
            	  MAX_PRODUCTION_RHS_LENGTH = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid maxProductionRightHandSideLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (MAX_PRODUCTION_RHS_LENGTH < 0)
              {
                 System.err.println("Invalid maxProductionRightHandSideLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              gotGenparm = true;
              continue;
           }            
           if (args[i].equals("-saveGrammar"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid saveGrammar option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              GRAMMAR_FILENAME = args[i];
              gotGenparm = true;
              continue;
           }
           if (args[i].equals("-loadGrammar"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid loadGrammar option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              GRAMMAR_FILENAME = args[i];
              load = true;
              continue;
           }
           if (args[i].equals("-initialPath"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid initialPath option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              INITIAL_WORLD_PATH = args[i];
              continue;
           }            
           if (args[i].equals("-numPaths"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid numPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 NUM_PATHS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid numPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (NUM_PATHS < 0)
              {
                 System.err.println("Invalid numPaths option");
                 System.err.println(Usage);
                 System.exit(1);
              }              
              continue;
           } 
           if (args[i].equals("-numPathExpansions"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid numPathExpansions option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 NUM_PATH_EXPANSIONS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid numPathExpansions option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (NUM_PATH_EXPANSIONS < 0)
              {
                 System.err.println("Invalid numPathExpansions option");
                 System.err.println(Usage);
                 System.exit(1);
              }              
              continue;
           }                     
           if (args[i].equals("-exportPathRNNdataset"))
           {
        	  gotExportPathRNNdataset = true;
              if (i < args.length - 1 && !args[i + 1].startsWith("-"))
              {
            	 i++;
            	 PATH_RNN_DATASET_FILENAME = args[i];
              }
              continue;
           }           
           if (args[i].equals("-pathRNNdatasetTrainFraction"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid pathRNNdatasetTrainFraction option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
            	  PATH_RNN_DATASET_TRAIN_FRACTION = Float.parseFloat(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid pathRNNdatasetTrainFraction option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (PATH_RNN_DATASET_TRAIN_FRACTION < 0.0f || PATH_RNN_DATASET_TRAIN_FRACTION > 1.0f)
              {
                 System.err.println("Invalid pathRNNdatasetTrainFraction option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              gotRNNdatasetTrainFraction = true;
              continue;
           }
           if (args[i].equals("-exportPathTCNdataset"))
           {
        	  gotExportPathTCNdataset = true;
              if (i < args.length - 1 && !args[i + 1].startsWith("-"))
              {
            	 i++;
            	 PATH_RNN_DATASET_FILENAME = args[i];
              }
              continue;
           }           
           if (args[i].equals("-pathTCNdatasetTrainFraction"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid pathTCNdatasetTrainFraction option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
            	  PATH_TCN_DATASET_TRAIN_FRACTION = Float.parseFloat(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid pathTCNdatasetTrainFraction option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (PATH_TCN_DATASET_TRAIN_FRACTION < 0.0f || PATH_TCN_DATASET_TRAIN_FRACTION > 1.0f)
              {
                 System.err.println("Invalid pathTCNdatasetTrainFraction option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              gotTCNdatasetTrainFraction = true;
              continue;
           }           
           if (args[i].equals("-exportPathTDNNdataset"))
           {
        	  gotExportPathTDNNdataset = true;
              if (i < args.length - 1 && !args[i + 1].startsWith("-"))
              {
            	 i++;
            	 PATH_TDNN_DATASET_FILENAME = args[i];
              }
              continue;
           }
           if (args[i].equals("-TDNNframeLength"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid TDNNframeLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 TDNN_FRAME_LENGTH = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid TDNNframeLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (TDNN_FRAME_LENGTH < 1)
              {
                 System.err.println("Invalid TDNNframeLength option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              gotTDNNframeLength = true;              
              continue;
           }                      
           if (args[i].equals("-pathTDNNdatasetTrainFraction"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid pathTDNNdatasetTrainFraction option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
            	  PATH_TDNN_DATASET_TRAIN_FRACTION = Float.parseFloat(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid pathTDNNdatasetTrainFraction option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (PATH_TDNN_DATASET_TRAIN_FRACTION < 0.0f || PATH_TDNN_DATASET_TRAIN_FRACTION > 1.0f)
              {
                 System.err.println("Invalid pathTDNNdatasetTrainFraction option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              gotTDNNdatasetTrainFraction = true;
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
        if (generate)
        {
        	if (load)
        	{
                System.err.println(Usage);
                System.exit(1);
        	}
        } else {
        	if (!load)
        	{
	            System.err.println(Usage);
	            System.exit(1); 
        	} else {
        		if (gotGenparm)
        		{
                    System.err.println(Usage);
                    System.exit(1);        			
        		}
        	}
        }
        if (MIN_NONTERMINAL_PRODUCTIONS > MAX_NONTERMINAL_PRODUCTIONS)
        {
            System.err.println(Usage);
            System.exit(1);           	
        }              
        if (MIN_PRODUCTION_RHS_LENGTH > MAX_PRODUCTION_RHS_LENGTH)
        {
            System.err.println(Usage);
            System.exit(1);           	
        }      	
        if (INITIAL_WORLD_PATH.length() < 2 || !INITIAL_WORLD_PATH.matches("[a-zA-Z]+") ||
        		INITIAL_WORLD_PATH.contains("S") || INITIAL_WORLD_PATH.contains("G") ||        		
        		!INITIAL_WORLD_PATH.startsWith("s") || !INITIAL_WORLD_PATH.endsWith("g") ||        		
        		StringUtils.countMatches(INITIAL_WORLD_PATH, "s") > 1 ||
        		StringUtils.countMatches(INITIAL_WORLD_PATH, "g") > 1)
        {
            System.err.println("Invalid initial path");
            System.exit(1);         	
        }
        String terminals = "abcdefhijklmnopqrtuvwxyz";
        String nonterminals = "ABCDEFHIJKLMNOPQRTUVWXYZ";        
        char[] symbols = INITIAL_WORLD_PATH.toCharArray();
        for (int i = 0; i < symbols.length; i++)
        {
        	if (symbols[i] >= 'a' && symbols[i] <= 'z')
        	{
        		if (symbols[i] != 's' && symbols[i] != 'g')
        		{
	        		int n = terminals.indexOf(symbols[i]);
	        		if (n >= NUM_NONTERMINALS)
	        		{
	                    System.err.println("Initial path contains terminal " + symbols[i] + " that cannot be processed");
	                    System.exit(1);         			
	        		} 
        		}
        	} else if (symbols[i] >= 'A' && symbols[i] <= 'Z')
        	{
        		int n = nonterminals.indexOf(symbols[i]);
        		if (n >= NUM_NONTERMINALS)
        		{
                    System.err.println("Initial path contains nonterminal " + symbols[i] + " that cannot be processed");
                    System.exit(1);         			
        		}       		
        	}
        }
        if (!gotExportPathRNNdataset && gotRNNdatasetTrainFraction)
    	{
            System.err.println(Usage);
            System.exit(1);    	
    	}
        if (!gotExportPathTCNdataset && gotTCNdatasetTrainFraction)
    	{
            System.err.println(Usage);
            System.exit(1);    	
    	}        
        if (!gotExportPathTDNNdataset && (gotTDNNframeLength || gotTDNNdatasetTrainFraction))
    	{
            System.err.println(Usage);
            System.exit(1);    	
    	}
        
        // Create world grammar.
        WorldGrammar worldGrammar = new WorldGrammar();
        
        // Generate grammar?
        if (generate)
        {
        	worldGrammar.generateGrammar();
        	
        	// Save grammar?
        	if (GRAMMAR_FILENAME != null)
        	{
        		worldGrammar.saveGrammar(GRAMMAR_FILENAME);
        	}
        } else {
        	worldGrammar.loadGrammar(GRAMMAR_FILENAME);
        }
        if (VERBOSE)
        {
	        System.out.println("Grammar:");
	        System.out.println("NUM_NONTERMINALS = " + NUM_NONTERMINALS);
	        System.out.println("MIN_NONTERMINAL_PRODUCTIONS = " + MIN_NONTERMINAL_PRODUCTIONS);		
	        System.out.println("MAX_NONTERMINAL_PRODUCTIONS = " + MAX_NONTERMINAL_PRODUCTIONS);	
	        System.out.println("MIN_PRODUCTION_RHS_LENGTH = " + MIN_PRODUCTION_RHS_LENGTH);
	        System.out.println("MAX_PRODUCTION_RHS_LENGTH = " + MAX_PRODUCTION_RHS_LENGTH); 
	        for (Map.Entry<String, List<String>> entry : worldGrammar.grammar.entrySet())
	        {
	        	String key = entry.getKey();
	        	for (String value : entry.getValue())
	        	{
	        		System.out.println(key + " ::= " + value);
	        	}
	        }
        }
        
        // Produce world paths.
        worldGrammar.produceWorldPaths(INITIAL_WORLD_PATH, NUM_PATHS, VERBOSE);
        
        // Export datasets?
        if (gotExportPathRNNdataset)
        {
        	worldGrammar.exportPathDataset(PATH_RNN_DATASET_FILENAME, PATH_RNN_DATASET_TRAIN_FRACTION);
        }
        if (gotExportPathTCNdataset)
        {
        	worldGrammar.exportPathDataset(PATH_TCN_DATASET_FILENAME, PATH_TCN_DATASET_TRAIN_FRACTION);
        }        
        if (gotExportPathTDNNdataset)
        {
        	worldGrammar.exportPathTDNNdataset(PATH_TDNN_DATASET_FILENAME, TDNN_FRAME_LENGTH, PATH_TDNN_DATASET_TRAIN_FRACTION);
        }
        
        System.exit(0);
    }
    
    // Constructor.
    public WorldGrammar()
    {
        // Initialize random numbers.
        randomizer = new Random(RANDOM_SEED);
    }
       
    // Generate grammar.
    public void generateGrammar()
    {    	
        grammar = new HashMap<String, List<String>>();
        if (NUM_NONTERMINALS == 0) return;
    	ArrayList<String> open = new ArrayList<String>();
    	open.add("A");
    	ArrayList<String> openNonterminals = new ArrayList<String>();
        openNonterminals.add("BCDEFHIJKLMNOPQRTUVWXYZ".substring(0, NUM_NONTERMINALS - 1));
        while (open.size() > 0)
        {
        	String lhs = open.get(0);
        	open.remove(0);
        	String nonterminals = openNonterminals.get(0);
        	openNonterminals.remove(0);
        	if (grammar.containsKey(lhs)) continue;        	
        	int n = MIN_NONTERMINAL_PRODUCTIONS;
        	if (MAX_NONTERMINAL_PRODUCTIONS > MIN_NONTERMINAL_PRODUCTIONS)
        	{
        		n += randomizer.nextInt(MAX_NONTERMINAL_PRODUCTIONS - MIN_NONTERMINAL_PRODUCTIONS);
        	}        	
        	int l = MIN_PRODUCTION_RHS_LENGTH;
        	if (MAX_PRODUCTION_RHS_LENGTH > MIN_PRODUCTION_RHS_LENGTH)
        	{
        		l += randomizer.nextInt(MAX_PRODUCTION_RHS_LENGTH - MIN_PRODUCTION_RHS_LENGTH);
        	}
        	if (l > nonterminals.length()) 
        	{
        		l = nonterminals.length();
        	}
        	ArrayList<String> next = new ArrayList<String>();
        	for (int i = 0; i < n; i++)
        	{
        		String rhs = "";
        		for (int j = 0; j < l; j++)
        		{
            		int idx = randomizer.nextInt(nonterminals.length());
            		String nonterminal = nonterminals.substring(idx, idx + 1);      		
            		if (!produces(nonterminal, lhs))            		
            		{
	            		rhs += nonterminal;
	            		if (!next.contains(nonterminal))
	            		{
	            			next.add(nonterminal);
	            		}
            		}
        		}
        		if (!rhs.isEmpty())
        		{
	        		if (grammar.containsKey(lhs))
	        		{
			        	List<String> value = grammar.get(lhs);
			        	boolean found = false;
			        	for (String v : value)
			        	{
			        		if (v.equals(rhs))
			        		{
			        			found = true;
			        			break;
			        		}
			        	}
			        	if (!found)
			        	{
				        	value.add(rhs);
			        	}
	        		} else {
			        	ArrayList<String> value = new ArrayList<String>();
				        value.add(rhs);
				        grammar.put(lhs, value);
	        		}
        		}
	        }
        	for (String nonterminal : next)
        	{
        		nonterminals = nonterminals.replace(nonterminal, "");
        	}
        	for (String nonterminal : next)
        	{     	
				open.add(nonterminal);
				openNonterminals.add(nonterminals);
        	}
        }
    }
    
    // Given nonterminal produces another? 
	public boolean produces(String from, String to)
    {
    	if (grammar.containsKey(from))
    	{  		
    		List<String> values = grammar.get(from);
    		for (String rhs : values)
    		{
    			if (rhs.indexOf(to) != -1)
    			{
    				return true;
    			} else {
    				for (int i = 0; i < rhs.length(); i++) 
    				{
    					produces(rhs.substring(i, i + 1), to);
    					{
    						return true;
    					}
    				}
    			}
    		}
    	}
    	return false;
    }
    
    // Save grammar.
    public void saveGrammar(String filename)
    {	        
    	try
    	{
            FileWriter fileWriter = new FileWriter(filename);
            PrintWriter printWriter = new PrintWriter(fileWriter);
        	printWriter.println("NUM_NONTERMINALS = " + NUM_NONTERMINALS);
        	printWriter.println("MIN_NONTERMINAL_PRODUCTIONS = " + MIN_NONTERMINAL_PRODUCTIONS);		
        	printWriter.println("MAX_NONTERMINAL_PRODUCTIONS = " + MAX_NONTERMINAL_PRODUCTIONS);	
        	printWriter.println("MIN_PRODUCTION_RHS_LENGTH = " + MIN_PRODUCTION_RHS_LENGTH);
        	printWriter.println("MAX_PRODUCTION_RHS_LENGTH = " + MAX_PRODUCTION_RHS_LENGTH);        	            
            for (Map.Entry<String, List<String>> entry : grammar.entrySet())
            {
            	String key = entry.getKey();
            	for (String value : entry.getValue())
            	{
            		printWriter.println(key + " ::= " + value);
            	}
            }
            printWriter.close();
    	} catch (IOException e)
    	{
    		System.err.println("Cannot save grammar to file " + filename);
    		System.exit(1);
    	}
    }
    
    // Load grammar from file.
    public void loadGrammar(String filename)
    {
    	List<String> productions = null;    	
    	try
    	{
    		productions = Files.readAllLines(Paths.get(filename), StandardCharsets.UTF_8);
    	} catch (IOException e)
    	{
    		System.err.println("Cannot load grammar from file " + filename);
    		System.exit(1);
    	}
    	int i = 0;
    	String parm = productions.get(i);
    	if (parm.startsWith("NUM_NONTERMINALS"))
    	{
        	String[] parts = parm.split("=");
        	NUM_NONTERMINALS = Integer.parseInt(parts[1].trim());
    		i++;
    	}
    	parm = productions.get(i);
    	if (parm.startsWith("MIN_NONTERMINAL_PRODUCTIONS"))
    	{
        	String[] parts = parm.split("=");
        	MIN_NONTERMINAL_PRODUCTIONS = Integer.parseInt(parts[1].trim());
    		i++;
    	}
    	parm = productions.get(i);
    	if (parm.startsWith("MAX_NONTERMINAL_PRODUCTIONS"))
    	{
        	String[] parts = parm.split("=");
        	MAX_NONTERMINAL_PRODUCTIONS = Integer.parseInt(parts[1].trim());
    		i++;
    	}
    	parm = productions.get(i);
    	if (parm.startsWith("MIN_PRODUCTION_RHS_LENGTH"))
    	{
        	String[] parts = parm.split("=");
        	MIN_PRODUCTION_RHS_LENGTH = Integer.parseInt(parts[1].trim());
    		i++;
    	}
    	parm = productions.get(i);
    	if (parm.startsWith("MAX_PRODUCTION_RHS_LENGTH"))
    	{
        	String[] parts = parm.split("=");
        	MAX_PRODUCTION_RHS_LENGTH = Integer.parseInt(parts[1].trim());
    		i++;
    	}    	     	
        grammar = new HashMap<String, List<String>>(); 
        for (; i < productions.size(); i++)
        {
        	String production = productions.get(i);
        	String[] parts = production.split("::=");
        	if (parts == null || parts.length < 2)
        	{
        		System.err.println("Invalid line " + production + " in file " + filename);
        		System.exit(1);
        	}
        	String lhs = parts[0].trim();
        	String rhs = parts[1].trim();
        	if (grammar.containsKey(lhs))
        	{
        		List<String> value = grammar.get(lhs);
        		value.add(rhs);	        		
        	} else {
        		ArrayList<String> value = new ArrayList<String>();
        		value.add(rhs);
        		grammar.put(lhs, value);
        	}
        }
    }
    
    // Produce world paths.
    public void produceWorldPaths(String initialWorldPath, int numPaths)
    {
    	produceWorldPaths(initialWorldPath, numPaths, false);    	
    }
    
    public void produceWorldPaths(String initialWorldPath, int numPaths, boolean verbose)
    {
    	worldPaths = new ArrayList<String>();
    	for (int pathnum = 0; pathnum < NUM_PATHS; pathnum++)
    	{
    		if (verbose) System.out.println("Path #" + pathnum + ":");
    		String path = new String(initialWorldPath);
	        if (verbose) System.out.println("Initial world path: " + path);
	        HashMap<String, String> expandedNonterminals = new HashMap<String, String>();
	        ArrayList<Pair<String, Integer>> open = new ArrayList<Pair<String, Integer>>();
        	char[] symbols = path.toCharArray();
        	for (int i = 0; i < symbols.length; i++)
        	{
        		String symbol = String.valueOf(symbols[i]);
        		if (grammar.containsKey(symbol))
        		{
        			open.add(new Pair<String, Integer>(symbol, 0));
        		}
        	}
	        while (open.size() > 0)
	        {	        			
	        	Pair<String, Integer> pair = open.get(0);
	        	String lhs = pair.getFirst();
	        	int pass = pair.getSecond();
	        	if (pass >= NUM_PATH_EXPANSIONS) break;
	        	open.remove(0);
	        	String rhs = "";
	        	if (expandedNonterminals.containsKey(lhs))
	        	{
	        		rhs = expandedNonterminals.get(lhs);
	        	} else {
		        	List<String> productions = grammar.get(lhs);
		        	int n = randomizer.nextInt(productions.size());
		        	rhs = productions.get(n);
	        		expandedNonterminals.put(lhs, rhs);		        	
	        	}
	        	int n = path.indexOf(lhs);
	        	path = path.substring(0, n) + rhs + path.substring(n + 1);
	        	if (verbose) System.out.println("Expansion: " + lhs + " ::= " + rhs + 
	        			", at position " + n + ", world path: " + path);
	        	symbols = rhs.toCharArray();
	        	for (int i = 0; i < symbols.length; i++)
	        	{
	        		String symbol = String.valueOf(symbols[i]);
	        		if (grammar.containsKey(symbol))
	        		{
	        			open.add(new Pair<String, Integer>(symbol, pass + 1));
	        		}
	        	}	        	
	        }
	        symbols = path.toCharArray();
	        path = "";
	    	for (int j = 0; j < symbols.length; j++)
	    	{
	    		path += Character.toLowerCase(symbols[j]);
	    	}
	    	worldPaths.add(path);	    	
	    	if (verbose) System.out.println("Final world path: " + path);
    	}
    }
    
    // Export path RNN dataset.
    public void exportPathRNNdataset()
    {
    	exportPathDataset(PATH_RNN_DATASET_FILENAME, PATH_RNN_DATASET_TRAIN_FRACTION);
    }
    
    // Export path TCN dataset.
    public void exportPathTCNdataset()
    {
    	exportPathDataset(PATH_TCN_DATASET_FILENAME, PATH_TCN_DATASET_TRAIN_FRACTION);
    }
    
    public void exportPathDataset(String filename, float trainFraction)
    {
    	if (worldPaths == null)
    	{
    		System.err.println("No world paths");
    		System.exit(1);
    	}
    	try
    	{
            FileWriter fileWriter = new FileWriter(filename);
            PrintWriter printWriter = new PrintWriter(fileWriter);
            int numPaths = worldPaths.size();
            int pathLength = 0;
            for (String s : worldPaths)
            {
            	if (pathLength < s.length())
            	{
            		pathLength = s.length();
            	}
            }
            int n = (int)((float)numPaths * trainFraction);
            printWriter.println("X_train_shape = [ " + n + ", " + pathLength + ", " + 26 + " ]");
            printWriter.print("X_train_seq = [ ");
            for (int i = 0; i < n; i++)
            {
            	char[] terminals = worldPaths.get(i).toCharArray();
            	for (int j = 0; j < pathLength; j++)
        		{
            		if (j < terminals.length) 
            		{
            			printWriter.print(oneHot(terminals[j]));
            		} else {
            			printWriter.print(oneHot('g'));          			
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
            printWriter.println("y_train_shape = [ " + n + ", " + pathLength + ", " + 26 + " ]");
            printWriter.print("y_train_seq = [ ");
            for (int i = 0; i < n; i++)
            {
            	char[] terminals = worldPaths.get(i).toCharArray();
            	for (int j = 1; j < pathLength; j++)
        		{
            		if (j < terminals.length) 
            		{
            			printWriter.print(oneHot(terminals[j]));
            		} else {
            			printWriter.print(oneHot('g'));          			
            		}
                	printWriter.print(", ");          		
        		}
    			printWriter.print(oneHot('g'));            	
            	if (i < n - 1)
            	{
            		printWriter.print(", ");
            	}
            }
            printWriter.println(" ]");
            printWriter.println("X_test_shape = [ " + (numPaths - n) + ", " + pathLength + ", " + 26 + " ]");
            printWriter.print("X_test_seq = [ ");
            for (int i = n; i < numPaths; i++)
            {
            	char[] terminals = worldPaths.get(i).toCharArray();
            	for (int j = 0; j < pathLength; j++)
        		{
            		if (j < terminals.length) 
            		{
            			printWriter.print(oneHot(terminals[j]));
            		} else {
            			printWriter.print(oneHot('g'));          			
            		}
                	if (j < pathLength - 1)
                	{
                		printWriter.print(", ");
                	}            		
        		}
            	if (i < numPaths - 1)
            	{
            		printWriter.print(", ");
            	}
            }
            printWriter.println(" ]");
            printWriter.println("y_test_shape = [ " + (numPaths - n) + ", " + pathLength + ", " + 26 + " ]");
            printWriter.print("y_test_seq = [ ");
            for (int i = n; i < numPaths; i++)
            {
            	char[] terminals = worldPaths.get(i).toCharArray();
            	for (int j = 1; j < pathLength; j++)
        		{
            		if (j < terminals.length) 
            		{
            			printWriter.print(oneHot(terminals[j]));
            		} else {
            			printWriter.print(oneHot('g'));          			
            		}
                	printWriter.print(", ");          		
        		}
    			printWriter.print(oneHot('g'));            	
            	if (i < numPaths - 1)
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
    
    // Export path TDNN dataset.
    public void exportPathTDNNdataset()
    {
    	exportPathTDNNdataset(PATH_TDNN_DATASET_FILENAME, TDNN_FRAME_LENGTH, PATH_TDNN_DATASET_TRAIN_FRACTION);
    }
    
    public void exportPathTDNNdataset(String filename, int frameLength, float trainFraction)
    {
    	if (worldPaths == null)
    	{
    		System.err.println("No world paths");
    		System.exit(1);
    	}
    	try
    	{
            FileWriter fileWriter = new FileWriter(filename);
            PrintWriter printWriter = new PrintWriter(fileWriter);
            int numTrainPaths = (int)((float)worldPaths.size() * trainFraction);            
            int numSeqs = 0;
            for (int i = 0; i < numTrainPaths; i++)
            {
            	char[] terminals = worldPaths.get(i).toCharArray();            	
            	if (terminals.length > frameLength)
            	{
            		numSeqs += terminals.length - frameLength;
            	}            	
            }
            printWriter.println("X_train_shape = [ " + numSeqs + ", " + frameLength + ", " + 26 + " ]");
            printWriter.print("X_train_seq = [ ");
            for (int i = 0; i < numTrainPaths; i++)
            {
            	char[] terminals = worldPaths.get(i).toCharArray();
            	for (int j = 0, n = terminals.length - frameLength; j < n; j++)
        		{
            		for (int k = 0; k < frameLength; k++)
            		{
            			printWriter.print(oneHot(terminals[j + k]));
                    	if (k < frameLength - 1)
                    	{
                    		printWriter.print(", ");
                    	}             			
            		}
                	if (j < n - 1)
                	{
                		printWriter.print(", ");
                	}            		
        		}
            	if (i < numTrainPaths - 1)
            	{
            		printWriter.print(", ");
            	}
            }
            printWriter.println(" ]");
            printWriter.println("y_train_shape = [ " + numSeqs + ", " + 1 + ", " + 26 + " ]");
            printWriter.print("y_train_seq = [ ");
            for (int i = 0; i < numTrainPaths; i++)
            {
            	char[] terminals = worldPaths.get(i).toCharArray();
            	for (int j = 0, n = terminals.length - frameLength; j < n; j++)
        		{
            		printWriter.print(oneHot(terminals[j + frameLength]));
                	if (j < n - 1)
                	{
                		printWriter.print(", ");
                	}               		
        		}           	
            	if (i < numTrainPaths - 1)
            	{
            		printWriter.print(", ");
            	}
            }
            printWriter.println(" ]");
            int numTestPaths = worldPaths.size() - numTrainPaths;            
            numSeqs = 0;
            for (int i = 0; i < numTestPaths; i++)
            {
            	char[] terminals = worldPaths.get(i + numTrainPaths).toCharArray();            	
            	if (terminals.length > frameLength)
            	{
            		numSeqs += terminals.length - frameLength;
            	}            	
            }            
            printWriter.println("X_test_shape = [ " + numSeqs + ", " + frameLength + ", " + 26 + " ]");
            printWriter.print("X_test_seq = [ ");
            for (int i = 0; i < numTestPaths; i++)
            {
            	char[] terminals = worldPaths.get(i + numTrainPaths).toCharArray();
            	for (int j = 0, n = terminals.length - frameLength; j < n; j++)
        		{
            		for (int k = 0; k < frameLength; k++)
            		{
            			printWriter.print(oneHot(terminals[j + k]));
                    	if (k < frameLength - 1)
                    	{
                    		printWriter.print(", ");
                    	}               			
            		}
                	if (j < n - 1)
                	{
                		printWriter.print(", ");
                	}            		
        		}	
            	if (i < numTestPaths - 1)
            	{
            		printWriter.print(", ");
            	}
            }
            printWriter.println(" ]");
            printWriter.println("y_test_shape = [ " + numSeqs + ", " + 1 + ", " + 26 + " ]");
            printWriter.print("y_test_seq = [ ");
            for (int i = 0; i < numTestPaths; i++)
            {
            	char[] terminals = worldPaths.get(i + numTrainPaths).toCharArray();
            	for (int j = 0, n = terminals.length - frameLength; j < n; j++)
        		{
            		printWriter.print(oneHot(terminals[j + frameLength]));
                	if (j < n - 1)
                	{
                		printWriter.print(", ");
                	}               		
        		}           	
            	if (i < numTestPaths - 1)
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
    
    // One-hot coding of terminal.
    public String oneHot(char terminal)
    {
    	int t = terminal - 'a';
    	String code = "";
    	for (int i = 0; i < 26; i++)
    	{
    		if (i == t)
    		{
    			code += "1";
    		} else {
    			code += "0";
    		}
    		if (i < 25)
    		{
    			code += ", ";
    		}
    	}
    	return code;
    }
}
