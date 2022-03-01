// For conditions of distribution and use, see copyright notice in Mandala.java

// World grammar.
// Generate a grammar that can produce paths from a starting state to a goal state in the world.

package mandala;

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
	public static int MIN_PRODUCTION_OUTPUT_SYMBOLS = 2;
	public static int MAX_PRODUCTION_OUTPUT_SYMBOLS = 5;
	public static float TERMINAL_PRODUCTION_PROBABILITY = 0.5f;
	
	// Path generation parameters.
	public static int NUM_NONTERMINAL_EXPANSIONS = 10;
	
	// Random numbers.
	public static int RANDOM_SEED = 4517;
	public static Random randomizer = null;
	
	// Files.
	public static String GRAMMAR_SAVE_FILE = null;	
	public static String GRAMMAR_LOAD_FILE = null;
	
	// Grammar.
	public static Map<String, List<String>> grammar;
	
	// World path production string.
	public static String worldPath = "sAg";

    // Usage.
    public static final String Usage =
      "Usage:\n" +
      "    java mandala.WorldGrammar\n" +
      "      Grammar:\n" +    		  
      "        -generateGrammar\n" +
      "          [-numTerminals <quantity> (default=" + NUM_TERMINALS + ")]\n" +
      "          [-numNonterminals <quantity> (default=" + NUM_NONTERMINALS + ")]\n" +
      "          [-minProductionOutputSymbols <quantity> (default=" + MIN_PRODUCTION_OUTPUT_SYMBOLS + ")]\n" +
      "          [-maxProductionOutputSymbols <quantity> (default=" + MAX_PRODUCTION_OUTPUT_SYMBOLS + ")]\n" +
      "          [-terminalProductionProbability <probability of terminal in production> (default=" + TERMINAL_PRODUCTION_PROBABILITY + ")]\n" +
      "          [-saveGrammar <file name>]\n" +       
      "        | -loadGrammar <file name>\n" +
      "      World path production:\n" +
      "          [-initialPath <string of terminals and nonterminals>\n" +
      "              (starting with unique terminal \"s\"tart and ending with unique terminal \"g\"oal, default=\"" + worldPath + "\")]\n" +      
      "          [-numNonterminalExpansions <quantity> (default=" + NUM_NONTERMINAL_EXPANSIONS + ")]\n" +
      "      [-randomSeed <seed> (default=" + RANDOM_SEED + ")]\n" +       
      "Exit codes:\n" +
      "  0=success\n" +
      "  1=error";
	   
    public static void main(String[] args) 
    {
    	boolean gotGen = false;
    	boolean gotGenparm = false;
        for (int i = 0; i < args.length; i++)
        {
           if (args[i].equals("-generateGrammar"))
           {
              gotGen = true;
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
           if (args[i].equals("-minProductionOutputSymbols"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid minProductionOutputSymbols option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 MIN_PRODUCTION_OUTPUT_SYMBOLS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid minProductionOutputSymbols option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (MIN_PRODUCTION_OUTPUT_SYMBOLS < 0)
              {
                 System.err.println("Invalid minProductionOutputSymbols option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              gotGenparm = true;
              continue;
           }
           if (args[i].equals("-maxProductionOutputSymbols"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid maxProductionOutputSymbols option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              try
              {
                 MAX_PRODUCTION_OUTPUT_SYMBOLS = Integer.parseInt(args[i]);
              }
              catch (NumberFormatException e) {
                 System.err.println("Invalid maxProductionOutputSymbols option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              if (MAX_PRODUCTION_OUTPUT_SYMBOLS < 0)
              {
                 System.err.println("Invalid maxProductionOutputSymbols option");
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
              GRAMMAR_SAVE_FILE = args[i];
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
              GRAMMAR_LOAD_FILE = args[i];
              continue;
           }
           if (args[i].equals("-initPath"))
           {
              i++;
              if (i >= args.length)
              {
                 System.err.println("Invalid initPath option");
                 System.err.println(Usage);
                 System.exit(1);
              }
              worldPath = args[i];
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
        if (gotGen)
        {
        	if (GRAMMAR_LOAD_FILE != null)
        	{
                System.err.println(Usage);
                System.exit(1);
        	}
        } else {
        	if (GRAMMAR_LOAD_FILE == null)
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
        if (MIN_PRODUCTION_OUTPUT_SYMBOLS > MAX_PRODUCTION_OUTPUT_SYMBOLS)
        {
            System.err.println(Usage);
            System.exit(1);           	
        }      	
        if (worldPath.length() < 2 || !worldPath.matches("[a-zA-Z]+") ||
        		!worldPath.startsWith("s") || !worldPath.endsWith("g") ||
        		StringUtils.countMatches(worldPath, "s") > 1 ||
        		StringUtils.countMatches(worldPath, "g") > 1)
        {
            System.err.println("Invalid initial path");
            System.exit(1);         	
        }
        char[] symbols = worldPath.toCharArray();
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

        // Initialize random numbers.
        randomizer = new Random(RANDOM_SEED);
         
        // Generate grammar.
        grammar = new HashMap<String, List<String>>();
        char[] terminals = "abcdefhijklmnopqrtuvwxyz".toCharArray();
        int i = 0;
        for (; i < NUM_NONTERMINALS && i < NUM_PRODUCTIONS; i++)
        {
        	int key = i;
        	String rhs = "";
        	int n = MIN_PRODUCTION_OUTPUT_SYMBOLS;
        	if (MAX_PRODUCTION_OUTPUT_SYMBOLS > MIN_PRODUCTION_OUTPUT_SYMBOLS)
        	{
        		n += randomizer.nextInt(MAX_PRODUCTION_OUTPUT_SYMBOLS - MIN_PRODUCTION_OUTPUT_SYMBOLS);
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
	        	int n = MIN_PRODUCTION_OUTPUT_SYMBOLS;
	        	if (MAX_PRODUCTION_OUTPUT_SYMBOLS > MIN_PRODUCTION_OUTPUT_SYMBOLS)
	        	{
	        		n += randomizer.nextInt(MAX_PRODUCTION_OUTPUT_SYMBOLS - MIN_PRODUCTION_OUTPUT_SYMBOLS);
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
	        	value.add(rhs);
	        } 
        }
        System.out.println("Grammar:");
        for (Map.Entry<String, List<String>> entry : grammar.entrySet())
        {
        	for (String value : entry.getValue())
        	{
        		System.out.println(entry.getKey() + " ::= " + value);
        	}
        }
        
        // Expand world path.

        System.exit(0);
    }
}
