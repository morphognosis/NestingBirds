// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds replay frame.

package morphognosis.nestingbirds;

import java.util.ArrayList;

public class NestingBirdsReplayFrame
{
   public class Location
   {
	   int x, y;
   };
   
   public class FemaleData
   {
	   int x,y;
	   String currentLocale;
	   String currentObject;
	   String leftLocale;
	   String leftObject;
	   String leftFrontLocale;
	   String leftFrontObject;
	   String frontLocale;
	   String frontObject;
	   String rightFrontLocale;
	   String rightFrontObject;
	   String rightLocale;
	   String rightObject;
	   String rightRearLocale;
	   String rightRearObject;
	   String rearLocale;
	   String rearObject;
	   String leftRearLocale;
	   String leftRearObject;
	   String orientation;
	   String goal;
	   String hasObject;
	   String food;
	   String mouseNeed;
	   String layEggNeed;
	   String broodEggNeed;
	   String response;		   
   };
   
   public class MaleData
   {
	   int x,y;
	   String locale;
	   String mouseProximity;
	   String stoneProximity;
	   String femaleProximity;
	   String goal;
	   String hasObject;
	   String flying;
	   String food;   
	   String femaleWantsMouse;
	   String femaleWantsStone;
	   String mouseNeed;
	   String femaleMouseNeed;
	   String femaleStoneNeed;
	   String attendFemaleNeed;
	   String response;      		   
   };

	   public int step;	   
	   public ArrayList<Location> mice;
	   public FemaleData femaleData;
	   public MaleData maleData;
	   
	   public NestingBirdsReplayFrame()
	   {
		   mice = new ArrayList<Location>();
		   femaleData = new FemaleData();
		   maleData = new MaleData();
	   }
	   
	   public Location newLocation()
	   {
		   return new Location();
	   }
	   
	   public FemaleData newFemaleData()
	   {
		   return new FemaleData();
	   }
	   
	   public MaleData newMaleData()
	   {
		   return new MaleData();
	   }		   
}
