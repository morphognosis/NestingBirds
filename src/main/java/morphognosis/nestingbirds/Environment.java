// For conditions of distribution and use, see copyright notice in Morphognosis.java

// The nesting birds environment.

package morphognosis.nestingbirds;

import java.util.Random;

public class Environment
{
	// Locale map: grassland=0, forest=1, food=2, desert=3, stone=4
	public static final int width=21, height=21;
	public static final char locale[][] = 
	{
		{ 0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,1,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,2,1,2,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,1,1,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,2,1,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,2,1,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,3,3,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,3,3,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,4,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,3,4,4,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,4,3,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,4,3,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,4,4,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,3,3,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,3,0,0,0 }
	};
	
	// Locale types.
	public static class LOCALE
	{
		public static final int DESERT = 0;
		public static final int FOREST = 1;
		public static final int GRASSLAND = 2;
	};
	
	// Object types.
	public static class OBJECT
	{
		public static final int NO_OBJECT = 0;
		public static final int EGG = 1;
		public static final int FOOD = 2;
		public static final int STONE = 3;
	};
	
	// Orientation.
	public static class ORIENTATION
	{
		public static final int NORTH = 0;
		public static final int SOUTH = 1;
		public static final int EAST = 2;
		public static final int WEST = 3;
	};
	
	// Bird genders.
	public static class GENDER
	{
		public static final int MALE = 0;
		public static final int FEMALE = 1;
	};
	
	// Input types.
	public static class INPUT
	{
		public static final int DO_NOTHING = 0;
		public static final int EAT = 1;
		public static final int GET = 2;
		public static final int GO_TO_DESERT = 3;
		public static final int GO_TO_FOREST = 4;
		public static final int GO_TO_MATE = 5;
		public static final int LAY_EGG = 6;
		public static final int LOOK_FOR_FOOD = 7;
		public static final int LOOK_FOR_STONE = 8;
		public static final int PUT = 9;
		public static final int RECEIVE = 10;
		public static final int STEP = 11;
		public static final int TOSS = 12;
		public static final int TURN = 13;
		public static final int WANT_STONE_SET = 14;
		public static final int WANT_STONE_UNSET = 15;
	};

	// Food duration.
	public static final int FOOD_DURATION = 60;
	
	// Don't care value.
	public static final int DONT_CARE = -1;
				
	// Point.
	public class POINT
	{
		public int x;
		public int y;
		POINT()
		{
			x = y = 0;
		}
	};

	// Cell.
	public class CELL
	{
		public int locale;
		public int object;
		CELL()
		{
			locale = LOCALE.DESERT;
			object = OBJECT.NO_OBJECT;
		}
	};
	
	// World.
	public CELL[][] world;
	
	// Bird condition.
	public class CONDITION
	{
		public boolean wantsFood;
		public boolean wantsStone;
		public int hasObject;
		CONDITION()
		{
			wantsFood = false;
			wantsStone = false;
			hasObject = OBJECT.NO_OBJECT;
		}
	};
	
	// Bird mate state.
	public class MATE
	{
		public boolean present;
		public CONDITION condition;
		MATE()
		{
			present = false;
			condition = new CONDITION();
		}
	};
	
	// Environment output.
	public class OUTPUT
	{
		public int locale;
		public int object;
		public CONDITION condition;
		public MATE mate;
		OUTPUT()
		{
			locale = LOCALE.DESERT;
			object = OBJECT.NO_OBJECT;
			condition = new CONDITION();
			mate = new MATE();
		}
	};

	// Bird.
	public class BIRD
	{
		public POINT location;	
		public int orientation;	
		public int gender;
		public CONDITION condition;
		public int food;
		BIRD()
		{
			location = new POINT();
			orientation = ORIENTATION.NORTH;
			gender = GENDER.MALE;
			condition = new CONDITION();
			food = 0;
		}
	};
	
	// Birds.
	public BIRD birds[];

	// Disrupt nest.
	private int disruptNest = 0;
	
	// Construct environment.
	Environment()
	{		
		// Initialize world.
		world = new CELL[width][height];
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				world[x][y] = new CELL();
				switch(locale[x][y])
				{
				case 0:
					world[x][y].locale = LOCALE.GRASSLAND;
					world[x][y].object = OBJECT.NO_OBJECT;
					break;
				case 1:
					world[x][y].locale = LOCALE.FOREST;
					world[x][y].object = OBJECT.NO_OBJECT;
					break;
				case 2:
					world[x][y].locale = LOCALE.FOREST;
					world[x][y].object = OBJECT.FOOD;
					break;
				case 3:
					world[x][y].locale = LOCALE.DESERT;
					world[x][y].object = OBJECT.NO_OBJECT;
					break;
				case 4:
					world[x][y].locale = LOCALE.DESERT;
					world[x][y].object = OBJECT.STONE;
					break;
				}
			}
		}
	
		// Initialize birds.
		birds = new BIRD[2];
		BIRD bird = new BIRD();
		birds[GENDER.MALE] = bird;
		bird.location.x = width/2;
		bird.location.y = height/2;
		bird.gender = GENDER.MALE;
		bird.condition.wantsFood = false;
		bird.condition.wantsStone = false;
		bird.condition.hasObject = OBJECT.NO_OBJECT;
		bird.food = FOOD_DURATION;
		bird.orientation = ORIENTATION.NORTH;
		bird = new BIRD();
		birds[GENDER.FEMALE] = bird;
		bird.location.x = width/2;
		bird.location.y = height/2;
		bird.gender = GENDER.FEMALE;
		bird.condition.wantsFood = true;
		bird.condition.wantsStone = false;
		bird.condition.hasObject = OBJECT.NO_OBJECT;
		bird.food = 0;
		bird.orientation = ORIENTATION.NORTH;
	}
	
	// Environment input
	public void input(int gender, int input)
	{
		BIRD male = birds[GENDER.MALE];
		BIRD female = birds[GENDER.FEMALE];
		BIRD bird = birds[gender];
		POINT location = bird.location;
		CONDITION condition = bird.condition;
		CELL cell = world[location.x][location.y];
		
		// Disrupt nest to motivate female to fix it.
		if (disruptNest == 1 && gender == GENDER.FEMALE) disruptNest++;
		if (disruptNest == 2 && gender == GENDER.FEMALE)
		{
			disruptNest++;
	
			// Remove stone from nest and drop on egg.
			world[location.x][location.y-1].object = OBJECT.NO_OBJECT;
			cell.object = OBJECT.STONE;
		}
	
		switch(input)
		{
	
		case INPUT.DO_NOTHING:
			break;
	
		case INPUT.EAT:
			if (condition.hasObject == OBJECT.FOOD)
			{
				condition.wantsFood = false;
				bird.food = FOOD_DURATION;
				condition.hasObject = OBJECT.NO_OBJECT;
			}
			break;
	
		case INPUT.GET:
			if (condition.hasObject == OBJECT.NO_OBJECT) 
			{
				condition.hasObject = cell.object;
				cell.object = OBJECT.NO_OBJECT;
			}
			break;
	
		case INPUT.GO_TO_DESERT:
			stepTo(DONT_CARE, LOCALE.DESERT, DONT_CARE, location);
			break;
	
		case INPUT.GO_TO_FOREST:
			stepTo(DONT_CARE, LOCALE.FOREST, DONT_CARE, location);
			break;
	
		case INPUT.GO_TO_MATE:
			stepTo(((gender+1)%2), DONT_CARE, DONT_CARE, location);
			break;
	
		case INPUT.LAY_EGG:
			if (cell.object == OBJECT.NO_OBJECT)
			{
				cell.object = OBJECT.EGG;
				if (disruptNest == 0) disruptNest=1;
			}
			break;
	
		case INPUT.LOOK_FOR_FOOD:
			stepTo(DONT_CARE, DONT_CARE, OBJECT.FOOD, location);
			break;
	
		case INPUT.LOOK_FOR_STONE:
			stepTo(DONT_CARE, LOCALE.DESERT, OBJECT.STONE, location);
			break;
	
		case INPUT.PUT:
			if (cell.object == OBJECT.NO_OBJECT)
			{
				cell.object = condition.hasObject;
				condition.hasObject = OBJECT.NO_OBJECT;
			}
			break;
	
		case INPUT.RECEIVE:
			if (condition.hasObject == OBJECT.NO_OBJECT)
			{
				if (gender == GENDER.MALE)
				{
					if (location.x == female.location.x && 
							location.y == female.location.y)
					{
						if (female.condition.hasObject != OBJECT.NO_OBJECT)
						{
							condition.hasObject = female.condition.hasObject;
							female.condition.hasObject = OBJECT.NO_OBJECT;
						}
					}
				} else {
					if (location.x == male.location.x && 
							location.y == male.location.y)
					{
						if (male.condition.hasObject != OBJECT.NO_OBJECT)
						{
							condition.hasObject = male.condition.hasObject;
							male.condition.hasObject = OBJECT.NO_OBJECT;
						}
					}
				}
			}
			break;
	
		case INPUT.STEP:
			switch(bird.orientation)
			{
			case ORIENTATION.NORTH:
				if (bird.location.y < height - 1)
				{
					bird.location.y++;
				}
				break;
			case ORIENTATION.SOUTH:
				if (bird.location.y > 0)
				{
					bird.location.y--;
				}
				break;
			case ORIENTATION.EAST:
				if (bird.location.x < width - 1)
				{
					bird.location.x++;
				}
				break;
			case ORIENTATION.WEST:
				if (bird.location.x > 0)
				{
					bird.location.x--;
				}
				break;
			}
			break;
	
		case INPUT.TOSS:
			if (condition.hasObject == OBJECT.NO_OBJECT) break;
	
			// Object lands randomly nearby.
			int i = height / 2;
			if (i > width/2) i = width/2;
			Random rand = new Random();
			for (int j = 5; j < i; j++)
			{
				int x = (int)(rand.nextInt()%(long)j);
				int y = (int)(rand.nextInt()%(long)j);
				if (x == 0 && y == 0) continue;
				if ((rand.nextInt()%2) == 0) x = -x;
				if ((rand.nextInt()%2) == 0) y = -y;
				x += bird.location.x;
				if (x < 0 || x >= width) continue;
				y += bird.location.y;
				if (y < 0 || y >= height) continue;
				if (world[x][y].object == OBJECT.NO_OBJECT)
				{
					world[x][y].object = condition.hasObject;
					condition.hasObject = OBJECT.NO_OBJECT;
					break;
				}
			}
			if (condition.hasObject != OBJECT.NO_OBJECT)
			{
				condition.hasObject = OBJECT.NO_OBJECT;  // vaporize
			}
			break;
	
		case INPUT.TURN:
			switch(bird.orientation)
			{
			case ORIENTATION.NORTH:
				bird.orientation = ORIENTATION.EAST;
				break;
			case ORIENTATION.SOUTH:
				bird.orientation = ORIENTATION.WEST;
				break;
			case ORIENTATION.EAST:
				bird.orientation =ORIENTATION.SOUTH;
				break;
			case ORIENTATION.WEST:
				bird.orientation = ORIENTATION.NORTH;
				break;
			}
			break;
	
		case INPUT.WANT_STONE_SET:
			condition.wantsStone = true;
			break;
	
		case INPUT.WANT_STONE_UNSET:
			condition.wantsStone = false;
			break;
		}
	
		// Digest food.
		if (bird.food > 0) bird.food--;
		if (bird.food == 0) condition.wantsFood = true;
	}
	
	// Output.
	public void setOutput(OUTPUT out, int locale, int object, CONDITION condition, MATE mate)
	{
		out.locale = locale;
		out.object = object;
		out.condition = condition;
		out.mate = mate;
	}

	// Step to various things
	public void stepTo(int gender, int locale, int object, POINT location)
	{
		if (gender != DONT_CARE)
		{
			BIRD bird = birds[gender];
			int x = bird.location.x;
			int y = bird.location.y;
			if (locale != DONT_CARE && locale != world[x][y].locale) return;
			if (object != DONT_CARE && object != world[x][y].object) return;
			if (bird.location.x < location.x)
			{
				location.x--;
			} else if (bird.location.x > location.x)
			{
				location.x++;
			}
			if (bird.location.y < location.y)
			{
				location.y--;
			} else if (bird.location.y > location.y)
			{
				location.y++;
			}
			return;
		}
	
		double d2 = -1.0;
		int x2 = 0;
		int y2 = 0;
		for (int x = 0; x < height; x++)
		{
			for (int y = 0; y < width; y++)
			{
				if (locale != DONT_CARE && locale != world[x][y].locale) continue;
				if (object != DONT_CARE && object != world[x][y].object) continue;
				double d = distance((double)x, (double)y, (double)location.x, (double)location.y);
				if (d2 < 0.0 || d < d2)
				{
					d2 = d;
					x2 = x;
					y2 = y;
				}
			}
		}
		if (d2 < 0.0) return;
		if (x2 < location.x)
		{
			location.x--;
		} else if (x2 > location.x)
		{
			location.x++;
		}
		if (y2 < (int)location.y)
		{
			location.y--;
		} else if (y2 > location.y)
		{
			location.y++;
		}
	}
	
	// Environment output
	public void output(int gender, OUTPUT out)
	{
		out.mate.present = false;
		out.mate.condition.wantsFood = false;
		out.mate.condition.wantsStone = false;
		out.mate.condition.hasObject = OBJECT.NO_OBJECT;
		if (gender == GENDER.MALE)
		{
			BIRD bird = birds[GENDER.MALE];
			out.condition = bird.condition;
			int x = bird.location.x;
			int y = bird.location.y;
			out.locale = world[x][y].locale;
			out.object = world[x][y].object;
			if (x == birds[GENDER.FEMALE].location.x && y == birds[GENDER.FEMALE].location.y)
			{
				out.mate.present = true;
				out.mate.condition = birds[GENDER.FEMALE].condition;
			}
		} else {
			BIRD bird = birds[GENDER.FEMALE];
			out.condition = bird.condition;
			int x = bird.location.x;
			int y = bird.location.y;
			out.locale = world[x][y].locale;
			out.object = world[x][y].object;
			if (x == birds[GENDER.MALE].location.x && y == birds[GENDER.MALE].location.y)
			{
				out.mate.present = true;
				out.mate.condition = birds[GENDER.MALE].condition;
			}
		}
	}
	
	// Euclidean distance.
	public static double distance(double x1, double y1, double x2, double y2) 
	{ 
		double xd = x1 - x2;
	    double yd = y1 - y2;    
	    return Math.sqrt((xd * xd) + (yd * yd));
	}

	// Print input to stdout
	public void printInput(int input)
	{
		switch(input)
		{
		case INPUT.DO_NOTHING: System.out.print("DO_NOTHING"); break;
		case INPUT.EAT: System.out.print("EAT"); break;
		case INPUT.GET: System.out.print("GET"); break;
		case INPUT.GO_TO_DESERT: System.out.print("GO_TO_DESERT"); break;
		case INPUT.GO_TO_FOREST: System.out.print("GO_TO_FOREST"); break;
		case INPUT.GO_TO_MATE: System.out.print("GO_TO_MATE"); break;
		case INPUT.LAY_EGG: System.out.print("LAY_EGG"); break;
		case INPUT.LOOK_FOR_FOOD: System.out.print("LOOK_FOR_FOOD"); break;
		case INPUT.LOOK_FOR_STONE: System.out.print("LOOK_FOR_STONE"); break;
		case INPUT.PUT: System.out.print("PUT"); break;
		case INPUT.RECEIVE: System.out.print("RECEIVE"); break;
		case INPUT.TOSS: System.out.print("TOSS"); break;
		case INPUT.TURN: System.out.print("TURN"); break;
		case INPUT.STEP: System.out.print("STEP"); break;
		case INPUT.WANT_STONE_SET: System.out.print("WANT_STONE_SET"); break;
		case INPUT.WANT_STONE_UNSET: System.out.print("WANT_STONE_UNSET"); break;
		}
	}
	
	// Print output to stdout
	public void printOutput(OUTPUT output)
	{
		switch(output.locale)
		{
		case LOCALE.DESERT: System.out.print("DESERT "); break;
		case LOCALE.FOREST: System.out.print("FOREST "); break;
		case LOCALE.GRASSLAND: System.out.print("GRASSLAND "); break;
		}
		switch(output.object)
		{
		case OBJECT.NO_OBJECT: System.out.print("NO_OBJECT "); break;
		case OBJECT.EGG: System.out.print("EGG "); break;
		case OBJECT.FOOD: System.out.print("FOOD "); break;
		case OBJECT.STONE: System.out.print("STONE "); break;
		}
		System.out.print("CONDITION: ");
		if (output.condition.wantsFood) System.out.print("WANTS_FOOD ");
		if (output.condition.wantsStone) System.out.print("WANTS_STONE ");
		switch(output.condition.hasObject)
		{
		case OBJECT.EGG: System.out.print("HAS_EGG "); break;
		case OBJECT.FOOD: System.out.print("HAS_FOOD "); break;
		case OBJECT.STONE: System.out.print("HAS_STONE "); break;
		}
		System.out.print("MATE: ");
		if (output.mate.present)
		{
			System.out.print("PRESENT ");
			if (output.mate.condition.wantsFood) System.out.print("WANTS_FOOD ");
			if (output.mate.condition.wantsStone) System.out.print("WANTS_STONE ");
			switch(output.mate.condition.hasObject)
			{
			case OBJECT.EGG: System.out.print("HAS_EGG"); break;
			case OBJECT.FOOD: System.out.print("HAS_FOOD"); break;
			case OBJECT.STONE: System.out.print("HAS_STONE"); break;
			}
		} else System.out.print("NOT PRESENT");
	}
	
	// Main.
	public static void main(String[] args)
	{
		Environment environment = new Environment();
		environment.input(GENDER.MALE, INPUT.GO_TO_FOREST);
		environment.input(GENDER.FEMALE, INPUT.LAY_EGG);
	}
}

