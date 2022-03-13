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

public class WorldGrammar
{
	// Grammar generation parameters.
	public static int NUM_TERMINALS = 10;
	public static int NUM_NONTERMINALS = 5;
	public static int NUM_PRODUCTIONS = 10;
	public static int MIN_PRODUCTION_RHS_LENGTH = 2;
	public static int MAX_PRODUCTION_RHS_LENGTH = 5;
	public static float TERMINAL_PRODUCTION_PROBABILITY = 0.5f;
	
	// Path generation parameters.
	public static int NUM_NONTERMINAL_EXPANSIONS = 10;
	public static int NUM_PATHS = 1;
	
	// Dataset exports.
	public static String PATH_RNN_DATASET_FILENAME = "world_path_rnn_dataset.py";
	public static float PATH_RNN_DATASET_TRAIN_FRACTION = 0.75f;
	public static String PATH_TDNN_DATASET_FILENAME = "world_path_tdnn_dataset.py";
	public static int TDNN_FRAME_LENGTH = 4;
	public static float PATH_TDNN_DATASET_TRAIN_FRACTION = 0.75f;
	
	// Random numbers.
	public static int RANDOM_SEED = 4517;
	public Random randomizer = null;
	
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
      "          [-numTerminals <quantity> (default=" + NUM_TERMINALS + ")]\n" +
      "          [-numNonterminals <quantity> (default=" + NUM_NONTERMINALS + ")]\n" +
      "          [-minProductionRightHandSideLength <quantity> (default=" + MIN_PRODUCTION_RHS_LENGTH + ")]\n" +
      "          [-maxProductionRightHandSideLength <quantity> (default=" + MAX_PRODUCTION_RHS_LENGTH + ")]\n" +
      "          [-terminalProductionProbability <probability>\n" +
      "              (probability of generating terminal vs. nonterminal in production, default=" + TERMINAL_PRODUCTION_PROBABILITY + ")]\n" +
      "          [-saveGrammar <file name>]\n" +       
      "        | -loadGrammar <file name>\n" +
      "      World path production:\n" +
      "          [-initialPath <string of terminals and nonterminals>\n" +
      "              (starting with unique terminal \"s\"tart and ending with unique terminal \"g\"oal, default=\"" + INITIAL_WORLD_PATH + "\")]\n" +      
      "          [-numNonterminalExpansions <quantity> (default=" + NUM_NONTERMINAL_EXPANSIONS + ")]\n" +
      "          [-numPaths <quantity> (default=" + NUM_PATHS + ")]\n" +
      "          [-exportPathRNNdataset [<file name (default=\"" + PATH_RNN_DATASET_FILENAME + "\")>]\n" +
      "              [-RNNdatasetTrainFraction <fraction> (default=" + PATH_RNN_DATASET_TRAIN_FRACTION + ")]]\n" +
      "          [-exportPathTDNNdataset [<file name (default=\"" + PATH_TDNN_DATASET_FILENAME + "\")>]\n" +
      "              [-TDNNframeLength <length> (default=" + TDNN_FRAME_LENGTH + ")]]\n" +      
      "              [-TDNNdatasetTrainFraction <fraction> (default=" + PATH_TDNN_DATASET_TRAIN_FRACTION + ")]]\n" +      
      "      [-randomSeed <seed> (default=" + RANDOM_SEED + ")]\n" +
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
           if (args[i].equals("-numTerminals"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid numTerminals option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 NUM_TERMINALS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid numTerminals option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (NUM_TERMINALS < 2 || NUM_TERMINALS > 26)
              {
                 System.err.println("Minimum of two and maximum of 26 terminals");
                 System.err.println(Usage);
                 System.exit(1);
              }
              gotGenparm = true;
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
              if (NUM_NONTERMINALS > 26)
              {
                 System.err.println("Maximum of 26 nonterminals");
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
           if (args[i].equals("-terminalProductionProbability"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid terminalProductionProbability option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 TERMINAL_PRODUCTION_PROBABILITY = Float.parseFloat(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid terminalProductionProbability option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (TERMINAL_PRODUCTION_PROBABILITY < 0.0f || TERMINAL_PRODUCTION_PROBABILITY > 1.0f)
              {
                 System.err.println("Invalid terminalProductionProbability option");
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
           if (args[i].equals("-numNonterminalExpansions"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid numNonterminalExpansions option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 NUM_NONTERMINAL_EXPANSIONS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid numNonterminalExpansions option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (NUM_NONTERMINAL_EXPANSIONS < 0)
              {
                 System.err.println("Invalid numNonterminalExpansions option");
                 System.err.println(Usage);
                 System.exit(1);
              }              
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
        if (MIN_PRODUCTION_RHS_LENGTH > MAX_PRODUCTION_RHS_LENGTH)
        {
            System.err.println(Usage);
            System.exit(1);           	
        }      	
        if (INITIAL_WORLD_PATH.length() < 2 || !INITIAL_WORLD_PATH.matches("[a-zA-Z]+") ||
        		!INITIAL_WORLD_PATH.startsWith("s") || !INITIAL_WORLD_PATH.endsWith("g") ||
        		StringUtils.countMatches(INITIAL_WORLD_PATH, "s") > 1 ||
        		StringUtils.countMatches(INITIAL_WORLD_PATH, "g") > 1)
        {
            System.err.println("Invalid initial path");
            System.exit(1);         	
        }
        char[] symbols = INITIAL_WORLD_PATH.toCharArray();
        for (int i = 0; i < symbols.length; i++)
        {
        	if (symbols[i] >= 'a' && symbols[i] <= 'z')
        	{
        		if (symbols[i] != 's' && symbols[i] != 'g')
        		{
	        		int n = symbols[i] - 'a';
	        		if (n >= NUM_TERMINALS)
	        		{
	                    System.err.println("Initial path contains terminal " + symbols[i] + " that cannot be processed");
	                    System.exit(1);         			
	        		} 
        		}
        	} else if (symbols[i] >= 'A' && symbols[i] <= 'Z')
        	{
        		int n = symbols[i] - 'A';
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
        System.out.println("Grammar:");
        for (Map.Entry<String, List<String>> entry : worldGrammar.grammar.entrySet())
        {
        	String key = entry.getKey();
        	for (String value : entry.getValue())
        	{
        		System.out.println(key + " ::= " + value);
        	}
        }
        
        // Produce world paths.
        worldGrammar.produceWorldPaths(INITIAL_WORLD_PATH, NUM_PATHS, true);
        
        // Export datasets?
        if (gotExportPathRNNdataset)
        {
        	worldGrammar.exportPathRNNdataset(PATH_RNN_DATASET_FILENAME, PATH_RNN_DATASET_TRAIN_FRACTION);
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
        char[] terminals = "abcdefhijklmnopqrtuvwxyz".toCharArray();
        int i = 0;
        for (; i < NUM_NONTERMINALS && i < NUM_PRODUCTIONS; i++)
        {
        	int key = i;
        	String rhs = "";
        	int n = MIN_PRODUCTION_RHS_LENGTH;
        	if (MAX_PRODUCTION_RHS_LENGTH > MIN_PRODUCTION_RHS_LENGTH)
        	{
        		n += randomizer.nextInt(MAX_PRODUCTION_RHS_LENGTH - MIN_PRODUCTION_RHS_LENGTH);
        	}
        	for (int j = 0; j < n; j++)
        	{
	        	if (NUM_TERMINALS > 2 && randomizer.nextFloat() < TERMINAL_PRODUCTION_PROBABILITY)
	    		{
	        		int k = randomizer.nextInt(NUM_TERMINALS - 2);
	        		rhs += terminals[k];
	    		} else if (NUM_NONTERMINALS > 0)
	    		{
	        		char c = (char)((int)'A' + randomizer.nextInt(NUM_NONTERMINALS));
	        		rhs += c;
	    		}
        	}
        	String lhs = "" + (char)((int)key + 'A');
        	ArrayList<String> value = new ArrayList<String>();
	        value.add(rhs);
	        grammar.put(lhs, value);
        }
        if (NUM_NONTERMINALS > 0)
        {
	        for (; i < NUM_PRODUCTIONS; i++)
	        {
	        	int key = randomizer.nextInt(NUM_NONTERMINALS);
	        	String rhs = "";
	        	int n = MIN_PRODUCTION_RHS_LENGTH;
	        	if (MAX_PRODUCTION_RHS_LENGTH > MIN_PRODUCTION_RHS_LENGTH)
	        	{
	        		n += randomizer.nextInt(MAX_PRODUCTION_RHS_LENGTH - MIN_PRODUCTION_RHS_LENGTH);
	        	}
	        	for (int j = 0; j < n; j++)
	        	{
		        	if (NUM_TERMINALS > 2 && randomizer.nextFloat() < TERMINAL_PRODUCTION_PROBABILITY)
		    		{
		        		int k = randomizer.nextInt(NUM_TERMINALS - 2);
		        		rhs += terminals[k];
		    		} else if (NUM_NONTERMINALS > 0)
		    		{
		        		char c = (char)((int)'A' + randomizer.nextInt(NUM_NONTERMINALS));
		        		rhs += c;
		    		}
	        	}
	        	String lhs = "" + (char)((int)key + 'A');
	        	List<String> value = grammar.get(lhs);
	        	boolean found = false;
	        	for (String s : value)
	        	{
	        		if (s.equals(rhs))
	        		{
	        			found = true;
	        			break;
	        		}
	        	}
	        	if (!found)
	        	{
		        	value.add(rhs);
	        	}		        	
	        } 
        }
    }
    
    // Save grammar.
    public void saveGrammar(String filename)
    {	        
    	try
    	{
            FileWriter fileWriter = new FileWriter(filename);
            PrintWriter printWriter = new PrintWriter(fileWriter);
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
        grammar = new HashMap<String, List<String>>(); 
        for (String production : productions)
        {
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
	        for (int i = 0; i < NUM_NONTERMINAL_EXPANSIONS; i++)
	        {
	        	char[] symbols = path.toCharArray();
	        	int c = 0;
	        	for (int j = 0; j < symbols.length; j++)
	        	{
	        		if (symbols[j] >= 'A' && symbols[j] <= 'Z')
	        		{
	        			c++;
	        		}
	        	}
	        	if (c == 0) break;
	        	int n = randomizer.nextInt(c);
	        	c = 0;
	        	int k = 0;
	        	for (int j = 0; j < symbols.length; j++)
	        	{
	        		if (symbols[j] >= 'A' && symbols[j] <= 'Z')
	        		{
	        			if (c == n)
	        			{
	        				k = j;
	        				break;
	        			}
	        			c++;
	        		}
	        	}
	        	String lhs = path.substring(k, k + 1);
	        	List<String> productions = grammar.get(lhs);
	        	n = randomizer.nextInt(productions.size());
	        	String rhs = productions.get(n);
	        	path = path.substring(0, k) + rhs + path.substring(k + 1);
	        	if (verbose) System.out.println("Expansion #" + i + ", " + lhs + " ::= " + rhs + ", at position " + k + ", world path: " + path);
	        }
	        char[] symbols = path.toCharArray();
	        path = "";
	    	for (int j = 0; j < symbols.length; j++)
	    	{
	    		if (symbols[j] >= 'a' && symbols[j] <= 'z')
	    		{
	    			path += symbols[j];
	    		}
	    	}
	    	worldPaths.add(path);	    	
	    	if (verbose) System.out.println("Final world path: " + path);
    	}
    }
    
    // Export path RNN dataset.
    public void exportPathRNNdataset()
    {
    	exportPathRNNdataset(PATH_RNN_DATASET_FILENAME, PATH_RNN_DATASET_TRAIN_FRACTION);
    }
    
    public void exportPathRNNdataset(String filename, float trainFraction)
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
