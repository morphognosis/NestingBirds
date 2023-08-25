// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __DATASETS__
#define __DATASETS__

#include <string>
#include <vector>
#include <sstream>
#include "world.hpp"
using namespace std;

/*
 * Write training and testing datasets from generated behavior sensory-response sequences.
 * Format (one-hot encoded):
 * X_train_shape = [<number of sequences>, <sequence length>, <input dimension>]
 * X_train_seq = [ <comma-separated input sequences> ]
 * y_train_shape = [<number of sequences>, <sequence length>, <output dimension>]
 * y_train_seq = [ <comma-separted output sequences> ]
 * X_test_shape = [<number of sequences>, <sequence length>, <input dimension>]
 * X_test_seq = [ <comma-separated input sequences> ]
 * y_test_shape = [<number of sequences>, <sequence length>, <output dimension>]
 * y_test_seq = [ <comma-separted output sequences> ]
 */

// If testRandomSeed == -1, do not write test dataset.
void writeDatasets(int steps, vector<int> trainRandomSeeds, int testRandomSeed,
                   string maleDatasetName, string femaleDatasetName, bool verbose);

/*
 * Write pattern detection datasets from generated behavior sensory-response sequences.
 * Format:
 * <sensory input (comma-separated, one-hot encoded)>, <response>
 * ...
 */

// Write sensor discrimination datasets.
void writeSensorDiscriminationDatasets(int steps, vector<int> randomSeeds,
                                       string maleDatasetName, string femaleDatasetName, bool verbose);

// Sensory-response activity.
class MaleSensoryResponse
{
public:
   string locale;
   string mouseProximity;
   string stoneProximity;
   string femaleProximity;
   string goal;
   string hasObject;
   string flying;
   string femaleWantsMouse;
   string femaleWantsStone;
   string response;

   void print()
   {
      printf("locale=%s\n", locale.c_str());
      printf("mouseProximity=%s\n", mouseProximity.c_str());
      printf("stoneProximity=%s\n", stoneProximity.c_str());
      printf("femaleProximity=%s\n", femaleProximity.c_str());
      printf("goal=%s\n", goal.c_str());
      printf("hasObject=%s\n", hasObject.c_str());
      printf("flying=%s\n", flying.c_str());
      printf("femaleWantsMouse=%s\n", femaleWantsMouse.c_str());
      printf("femaleWantsStone=%s\n", femaleWantsStone.c_str());
      printf("response=%s\n", response.c_str());
   }


   // Convert sensory to one-hot encoding.
   string sensoryToOneHot()
   {
      string encoding = "";

      if (locale == "DESERT")
      {
         encoding += "1,0,0";
      }
      else if (locale == "FOREST")
      {
         encoding += "0,1,0";
      }
      else
      {
         encoding += "0,0,1";
      }
      encoding += ",";
      if (mouseProximity == "UNKNOWN")
      {
         encoding += "1,0,0,0,0";
      }
      else if (mouseProximity == "PRESENT")
      {
         encoding += "0,1,0,0,0";
      }
      else if (mouseProximity == "LEFT")
      {
         encoding += "0,0,1,0,0";
      }
      else if (mouseProximity == "FRONT")
      {
         encoding += "0,0,0,1,0";
      }
      else
      {
         encoding += "0,0,0,0,1";
      }
      encoding += ",";
      if (stoneProximity == "UNKNOWN")
      {
         encoding += "1,0,0,0,0";
      }
      else if (stoneProximity == "PRESENT")
      {
         encoding += "0,1,0,0,0";
      }
      else if (stoneProximity == "LEFT")
      {
         encoding += "0,0,1,0,0";
      }
      else if (stoneProximity == "FRONT")
      {
         encoding += "0,0,0,1,0";
      }
      else
      {
         encoding += "0,0,0,0,1";
      }
      encoding += ",";
      if (femaleProximity == "UNKNOWN")
      {
         encoding += "1,0,0,0,0";
      }
      else if (femaleProximity == "PRESENT")
      {
         encoding += "0,1,0,0,0";
      }
      else if (femaleProximity == "LEFT")
      {
         encoding += "0,0,1,0,0";
      }
      else if (femaleProximity == "FRONT")
      {
         encoding += "0,0,0,1,0";
      }
      else
      {
         encoding += "0,0,0,0,1";
      }
      encoding += ",";
      if (goal == "EAT_MOUSE")
      {
         encoding += "1,0,0,0";
      }
      else if (goal == "MOUSE_FOR_FEMALE")
      {
         encoding += "0,1,0,0";
      }
      else if (goal == "STONE_FOR_FEMALE")
      {
         encoding += "0,0,1,0";
      }
      else
      {
         encoding += "0,0,0,1";
      }
      encoding += ",";
      if (hasObject == "NO_OBJECT")
      {
         encoding += "1,0,0";
      }
      else if (hasObject == "MOUSE")
      {
         encoding += "0,1,0";
      }
      else
      {
         encoding += "0,0,1";
      }
      encoding += ",";
      if (flying == "true")
      {
         encoding += "1";
      }
      else
      {
         encoding += "0";
      }
      encoding += ",";
      if (femaleWantsMouse == "true")
      {
         encoding += "1";
      }
      else
      {
         encoding += "0";
      }
      encoding += ",";
      if (femaleWantsStone == "true")
      {
         encoding += "1";
      }
      else
      {
         encoding += "0";
      }
      return(encoding);
   }


   // Convert one-hot encoding to sensory.
   void oneHotToSensory(string encoding)
   {
      vector<string> values;
      stringstream   stream(encoding);
      string         value;

      while (getline(stream, value, ','))
      {
         values.push_back(value);
      }
      if (values.size() != oneHotSensoryLength())
      {
         fprintf(stderr, "Invalid sensory encoding: %s\n", encoding.c_str());
         exit(1);
      }
      int idx = 0;
      if (values[idx] == "1.0")
      {
         locale = "DESERT";
      }
      else if (values[idx + 1] == "1.0")
      {
         locale = "FOREST";
      }
      else if (values[idx + 2] == "1.0")
      {
         locale = "PLAIN";
      }
      else
      {
         locale = "DONT_CARE";
      }
      idx += 3;
      if (values[idx] == "1.0")
      {
         mouseProximity = "UNKNOWN";
      }
      else if (values[idx + 1] == "1.0")
      {
         mouseProximity = "PRESENT";
      }
      else if (values[idx + 2] == "1.0")
      {
         mouseProximity = "LEFT";
      }
      else if (values[idx + 3] == "1.0")
      {
         mouseProximity = "FRONT";
      }
      else if (values[idx + 4] == "1.0")
      {
         mouseProximity = "RIGHT";
      }
      else
      {
         mouseProximity = "DONT_CARE";
      }
      idx += 5;
      if (values[idx] == "1.0")
      {
         stoneProximity = "UNKNOWN";
      }
      else if (values[idx + 1] == "1.0")
      {
         stoneProximity = "PRESENT";
      }
      else if (values[idx + 2] == "1.0")
      {
         stoneProximity = "LEFT";
      }
      else if (values[idx + 3] == "1.0")
      {
         stoneProximity = "FRONT";
      }
      else if (values[idx + 4] == "1.0")
      {
         stoneProximity = "RIGHT";
      }
      else
      {
         stoneProximity = "DONT_CARE";
      }
      idx += 5;
      if (values[idx] == "1.0")
      {
         femaleProximity = "UNKNOWN";
      }
      else if (values[idx + 1] == "1.0")
      {
         femaleProximity = "PRESENT";
      }
      else if (values[idx + 2] == "1.0")
      {
         femaleProximity = "LEFT";
      }
      else if (values[idx + 3] == "1.0")
      {
         femaleProximity = "FRONT";
      }
      else if (values[idx + 4] == "1.0")
      {
         femaleProximity = "RIGHT";
      }
      else
      {
         femaleProximity = "DONT_CARE";
      }
      idx += 5;
      if (values[idx] == "1.0")
      {
         goal = "EAT_MOUSE";
      }
      else if (values[idx + 1] == "1.0")
      {
         goal = "MOUSE_FOR_FEMALE";
      }
      else if (values[idx + 2] == "1.0")
      {
         goal = "STONE_FOR_FEMALE";
      }
      else if (values[idx + 3] == "1.0")
      {
         goal = "ATTEND_FEMALE";
      }
      else
      {
         goal = "DONT_CARE";
      }
      idx += 4;
      if (values[idx] == "1.0")
      {
         hasObject = "NO_OBJECT";
      }
      else if (values[idx + 1] == "1.0")
      {
         hasObject = "MOUSE";
      }
      else if (values[idx + 2] == "1.0")
      {
         hasObject = "STONE";
      }
      else
      {
         hasObject = "DONT_CARE";
      }
      idx += 3;
      if (values[idx] == "1.0")
      {
         flying = "true";
      }
      else if (values[idx] == "0.0")
      {
         flying = "false";
      }
      else
      {
         flying = "DONT_CARE";
      }
      idx += 1;
      if (values[idx] == "1.0")
      {
         femaleWantsMouse = "true";
      }
      else if (values[idx] == "0.0")
      {
         femaleWantsMouse = "false";
      }
      else
      {
         femaleWantsMouse = "DONT_CARE";
      }
      idx += 1;
      if (values[idx] == "1.0")
      {
         femaleWantsStone = "true";
      }
      else if (values[idx] == "0.0")
      {
         femaleWantsStone = "false";
      }
      else
      {
         femaleWantsStone = "DONT_CARE";
      }
   }


   // Convert to male sensors.
   void toSensors(int *sensors)
   {
      if (locale == "DESERT")
      {
         sensors[Male::LOCALE_SENSOR] = LOCALE::DESERT;
      }
      else if (locale == "FOREST")
      {
         sensors[Male::LOCALE_SENSOR] = LOCALE::FOREST;
      }
      else if (locale == "PLAIN")
      {
         sensors[Male::LOCALE_SENSOR] = LOCALE::PLAIN;
      }
      else
      {
         sensors[Male::LOCALE_SENSOR] = DONT_CARE;
      }
      if (mouseProximity == "UNKNOWN")
      {
         sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
      }
      else if (mouseProximity == "PRESENT")
      {
         sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
      }
      else if (mouseProximity == "LEFT")
      {
         sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
      }
      else if (mouseProximity == "FRONT")
      {
         sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
      }
      else if (mouseProximity == "RIGHT")
      {
         sensors[Male::MOUSE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
      }
      else
      {
         sensors[Male::MOUSE_PROXIMITY_SENSOR] = DONT_CARE;
      }
      if (stoneProximity == "UNKNOWN")
      {
         sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
      }
      else if (stoneProximity == "PRESENT")
      {
         sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
      }
      else if (stoneProximity == "LEFT")
      {
         sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
      }
      else if (stoneProximity == "FRONT")
      {
         sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
      }
      else if (stoneProximity == "RIGHT")
      {
         sensors[Male::STONE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
      }
      else
      {
         sensors[Male::STONE_PROXIMITY_SENSOR] = DONT_CARE;
      }
      if (femaleProximity == "UNKNOWN")
      {
         sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::UNKNOWN;
      }
      else if (femaleProximity == "PRESENT")
      {
         sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::PRESENT;
      }
      else if (femaleProximity == "LEFT")
      {
         sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::LEFT;
      }
      else if (femaleProximity == "FRONT")
      {
         sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::FRONT;
      }
      else if (femaleProximity == "RIGHT")
      {
         sensors[Male::FEMALE_PROXIMITY_SENSOR] = Male::PROXIMITY::RIGHT;
      }
      else
      {
         sensors[Male::FEMALE_PROXIMITY_SENSOR] = DONT_CARE;
      }
      if (goal == "EAT_MOUSE")
      {
         sensors[Male::GOAL_SENSOR] = Male::GOAL::EAT_MOUSE;
      }
      else if (goal == "MOUSE_FOR_FEMALE")
      {
         sensors[Male::GOAL_SENSOR] = Male::GOAL::MOUSE_FOR_FEMALE;
      }
      else if (goal == "STONE_FOR_FEMALE")
      {
         sensors[Male::GOAL_SENSOR] = Male::GOAL::STONE_FOR_FEMALE;
      }
      else if (goal == "ATTEND_FEMALE")
      {
         sensors[Male::GOAL_SENSOR] = Male::GOAL::ATTEND_FEMALE;
      }
      else
      {
         sensors[Male::GOAL_SENSOR] = DONT_CARE;
      }
      if (hasObject == "NO_OBJECT")
      {
         sensors[Male::HAS_OBJECT_SENSOR] = OBJECT::NO_OBJECT;
      }
      else if (hasObject == "MOUSE")
      {
         sensors[Male::HAS_OBJECT_SENSOR] = OBJECT::MOUSE;
      }
      else if (hasObject == "STONE")
      {
         sensors[Male::HAS_OBJECT_SENSOR] = OBJECT::STONE;
      }
      else
      {
         sensors[Male::HAS_OBJECT_SENSOR] = DONT_CARE;
      }
      if (flying == "true")
      {
         sensors[Male::FLYING_SENSOR] = 1;
      }
      else if (flying == "false")
      {
         sensors[Male::FLYING_SENSOR] = 0;
      }
      else
      {
         sensors[Male::FLYING_SENSOR] = DONT_CARE;
      }
      if (femaleWantsMouse == "true")
      {
         sensors[Male::FEMALE_WANTS_MOUSE_SENSOR] = 1;
      }
      else if (femaleWantsMouse == "false")
      {
         sensors[Male::FEMALE_WANTS_MOUSE_SENSOR] = 0;
      }
      else
      {
         sensors[Male::FEMALE_WANTS_MOUSE_SENSOR] = DONT_CARE;
      }
      if (femaleWantsStone == "true")
      {
         sensors[Male::FEMALE_WANTS_STONE_SENSOR] = 1;
      }
      else if (femaleWantsStone == "false")
      {
         sensors[Male::FEMALE_WANTS_STONE_SENSOR] = 0;
      }
      else
      {
         sensors[Male::FEMALE_WANTS_STONE_SENSOR] = DONT_CARE;
      }
   }


   // Get length of sensory encoding.
   static int oneHotSensoryLength()
   {
      return(28);
   }


   // Convert response string to numeric.
   int responseToNum()
   {
      if (response == "DO_NOTHING")
      {
         return(0);
      }
      else if (response == "MOVE_FORWARD")
      {
         return(1);
      }
      else if (response == "TURN_RIGHT")
      {
         return(2);
      }
      else if (response == "TURN_LEFT")
      {
         return(3);
      }
      else if (response == "EAT_MOUSE")
      {
         return(4);
      }
      else if (response == "GET_OBJECT")
      {
         return(5);
      }
      else if (response == "PUT_OBJECT")
      {
         return(6);
      }
      else if (response == "TOSS_OBJECT")
      {
         return(7);
      }
      else if (response == "GIVE_MOUSE")
      {
         return(8);
      }
      else if (response == "GIVE_STONE")
      {
         return(9);
      }
      else if (response == "FLY")
      {
         return(10);
      }
      else if (response == "ALIGHT")
      {
         return(11);
      }
      return(-1);
   }


   // Convert response to one-hot encoding.
   string responseToOneHot()
   {
      string encoding = "";

      if (response == "DO_NOTHING")
      {
         encoding += "1,0,0,0,0,0,0,0,0,0,0,0";
      }
      else if (response == "MOVE_FORWARD")
      {
         encoding += "0,1,0,0,0,0,0,0,0,0,0,0";
      }
      else if (response == "TURN_RIGHT")
      {
         encoding += "0,0,1,0,0,0,0,0,0,0,0,0";
      }
      else if (response == "TURN_LEFT")
      {
         encoding += "0,0,0,1,0,0,0,0,0,0,0,0";
      }
      else if (response == "EAT_MOUSE")
      {
         encoding += "0,0,0,0,1,0,0,0,0,0,0,0";
      }
      else if (response == "GET_OBJECT")
      {
         encoding += "0,0,0,0,0,1,0,0,0,0,0,0";
      }
      else if (response == "PUT_OBJECT")
      {
         encoding += "0,0,0,0,0,0,1,0,0,0,0,0";
      }
      else if (response == "TOSS_OBJECT")
      {
         encoding += "0,0,0,0,0,0,0,1,0,0,0,0";
      }
      else if (response == "GIVE_MOUSE")
      {
         encoding += "0,0,0,0,0,0,0,0,1,0,0,0";
      }
      else if (response == "GIVE_STONE")
      {
         encoding += "0,0,0,0,0,0,0,0,0,1,0,0";
      }
      else if (response == "FLY")
      {
         encoding += "0,0,0,0,0,0,0,0,0,0,1,0";
      }
      else
      {
         encoding += "0,0,0,0,0,0,0,0,0,0,0,1";
      }
      return(encoding);
   }


   // Get length of response encoding.
   static int oneHotResponseLength()
   {
      return(12);
   }
};

class FemaleSensoryResponse
{
public:
   vector<string> cellSensors;
   string         orientation;
   string         goal;
   string         hasObject;
   string         response;

   void print()
   {
      printf("currentCell=%s\n", cellSensors[0].c_str());
      printf("leftCell=%s\n", cellSensors[1].c_str());
      printf("leftFrontCell=%s\n", cellSensors[2].c_str());
      printf("frontCell=%s\n", cellSensors[3].c_str());
      printf("rightFrontCell=%s\n", cellSensors[4].c_str());
      printf("rightCell=%s\n", cellSensors[5].c_str());
      printf("rightRearCell=%s\n", cellSensors[6].c_str());
      printf("rearCell=%s\n", cellSensors[7].c_str());
      printf("leftRearCell=%s\n", cellSensors[8].c_str());
      printf("orientation=%s\n", orientation.c_str());
      printf("goal=%s\n", goal.c_str());
      printf("hasObject=%s\n", hasObject.c_str());
      printf("response=%s\n", response.c_str());
   }


   // Convert sensory to one-hot encoding.
   string sensoryToOneHot()
   {
      string encoding = "";

      for (int i = 0; i < 9; i++)
      {
         if (cellSensors[i] == "NO_OBJECT")
         {
            encoding += "1,0,0,0";
         }
         else if (cellSensors[i] == "EGG")
         {
            encoding += "0,1,0,0";
         }
         else if (cellSensors[i] == "MOUSE")
         {
            encoding += "0,0,1,0";
         }
         else
         {
            encoding += "0,0,0,1";
         }
         if (i < 8)
         {
            encoding += ",";
         }
      }
      encoding += ",";
      if (orientation == "NORTH")
      {
         encoding += "1,0,0,0";
      }
      else if (orientation == "EAST")
      {
         encoding += "0,1,0,0";
      }
      else if (orientation == "SOUTH")
      {
         encoding += "0,0,1,0";
      }
      else
      {
         encoding += "0,0,0,1";
      }
      encoding += ",";
      if (goal == "LAY_EGG")
      {
         encoding += "1,0,0";
      }
      else if (goal == "BROOD_EGG")
      {
         encoding += "0,1,0";
      }
      else
      {
         encoding += "0,0,1";
      }
      encoding += ",";
      if (hasObject == "NO_OBJECT")
      {
         encoding += "1,0,0";
      }
      else if (hasObject == "MOUSE")
      {
         encoding += "0,1,0";
      }
      else
      {
         encoding += "0,0,1";
      }
      return(encoding);
   }


   // Convert one-hot encoding to sensory.
   void oneHotToSensory(string encoding)
   {
      vector<string> values;
      stringstream   stream(encoding);
      string         value;

      while (getline(stream, value, ','))
      {
         values.push_back(value);
      }
      if (values.size() != oneHotSensoryLength())
      {
         fprintf(stderr, "Invalid sensory encoding: %s\n", encoding.c_str());
         exit(1);
      }
      int idx = 0;
      cellSensors.clear();
      for (int i = 0; i < 9; i++)
      {
         if (values[idx] == "1.0")
         {
            cellSensors.push_back("NO_OBJECT");
         }
         else if (values[idx + 1] == "1.0")
         {
            cellSensors.push_back("EGG");
         }
         else if (values[idx + 2] == "1.0")
         {
            cellSensors.push_back("MOUSE");
         }
         else if (values[idx + 3] == "1.0")
         {
            cellSensors.push_back("STONE");
         }
         else
         {
            cellSensors.push_back("DONT_CARE");
         }
         idx += 4;
      }
      if (values[idx] == "1.0")
      {
         orientation = "NORTH";
      }
      else if (values[idx + 1] == "1.0")
      {
         orientation = "EAST";
      }
      else if (values[idx + 2] == "1.0")
      {
         orientation = "SOUTH";
      }
      else if (values[idx + 3] == "1.0")
      {
         orientation = "WEST";
      }
      else
      {
         orientation = "DONT_CARE";
      }
      idx += 4;
      if (values[idx] == "1.0")
      {
         goal = "LAY_EGG";
      }
      else if (values[idx + 1] == "1.0")
      {
         goal = "BROOD_EGG";
      }
      else if (values[idx + 2] == "1.0")
      {
         goal = "EAT_MOUSE";
      }
      else
      {
         goal = "DONT_CARE";
      }
      idx += 3;
      if (values[idx] == "1.0")
      {
         hasObject = "NO_OBJECT";
      }
      else if (values[idx + 1] == "1.0")
      {
         hasObject = "MOUSE";
      }
      else if (values[idx + 2] == "1.0")
      {
         hasObject = "STONE";
      }
      else
      {
         hasObject = "DONT_CARE";
      }
   }


   // Convert to female sensors.
   void toSensors(int *sensors)
   {
      for (int i = 0; i < 9; i++)
      {
         if (cellSensors[i] == "NO_OBJECT")
         {
            sensors[i] = OBJECT::NO_OBJECT;
         }
         else if (cellSensors[i] == "EGG")
         {
            sensors[i] = OBJECT::EGG;
         }
         else if (cellSensors[i] == "MOUSE")
         {
            sensors[i] = OBJECT::MOUSE;
         }
         else if (cellSensors[i] == "STONE")
         {
            sensors[i] = OBJECT::STONE;
         }
         else
         {
            sensors[i] = DONT_CARE;
         }
      }
      if (orientation == "NORTH")
      {
         sensors[Female::ORIENTATION_SENSOR] = ORIENTATION::NORTH;
      }
      else if (orientation == "EAST")
      {
         sensors[Female::ORIENTATION_SENSOR] = ORIENTATION::EAST;
      }
      else if (orientation == "SOUTH")
      {
         sensors[Female::ORIENTATION_SENSOR] = ORIENTATION::SOUTH;
      }
      else if (orientation == "WEST")
      {
         sensors[Female::ORIENTATION_SENSOR] = ORIENTATION::WEST;
      }
      else
      {
         sensors[Female::ORIENTATION_SENSOR] = DONT_CARE;
      }
      if (goal == "LAY_EGG")
      {
         sensors[Female::GOAL_SENSOR] = Female::GOAL::LAY_EGG;
      }
      else if (goal == "BROOD_EGG")
      {
         sensors[Female::GOAL_SENSOR] = Female::GOAL::BROOD_EGG;
      }
      else if (goal == "EAT_MOUSE")
      {
         sensors[Female::GOAL_SENSOR] = Female::GOAL::EAT_MOUSE;
      }
      else
      {
         sensors[Female::GOAL_SENSOR] = DONT_CARE;
      }
      if (hasObject == "NO_OBJECT")
      {
         sensors[Female::HAS_OBJECT_SENSOR] = OBJECT::NO_OBJECT;
      }
      else if (hasObject == "MOUSE")
      {
         sensors[Female::HAS_OBJECT_SENSOR] = OBJECT::MOUSE;
      }
      else if (hasObject == "STONE")
      {
         sensors[Female::HAS_OBJECT_SENSOR] = OBJECT::STONE;
      }
      else
      {
         sensors[Female::HAS_OBJECT_SENSOR] = DONT_CARE;
      }
   }


   // Get length of sensory encoding.
   static int oneHotSensoryLength()
   {
      return(46);
   }


   // Convert response string to numeric.
   int responseToNum()
   {
      if (response == "DO_NOTHING")
      {
         return(0);
      }
      else if (response == "MOVE_FORWARD")
      {
         return(1);
      }
      else if (response == "TURN_RIGHT")
      {
         return(2);
      }
      else if (response == "TURN_LEFT")
      {
         return(3);
      }
      else if (response == "EAT_MOUSE")
      {
         return(4);
      }
      else if (response == "GET_OBJECT")
      {
         return(5);
      }
      else if (response == "PUT_OBJECT")
      {
         return(6);
      }
      else if (response == "TOSS_OBJECT")
      {
         return(7);
      }
      else if (response == "WANT_MOUSE")
      {
         return(8);
      }
      else if (response == "WANT_STONE")
      {
         return(9);
      }
      else if (response == "LAY_EGG")
      {
         return(10);
      }
      return(-1);
   }


   // Convert response to one-hot encoding.
   string responseToOneHot()
   {
      string encoding = "";

      if (response == "DO_NOTHING")
      {
         encoding += "1,0,0,0,0,0,0,0,0,0,0";
      }
      else if (response == "MOVE_FORWARD")
      {
         encoding += "0,1,0,0,0,0,0,0,0,0,0";
      }
      else if (response == "TURN_RIGHT")
      {
         encoding += "0,0,1,0,0,0,0,0,0,0,0";
      }
      else if (response == "TURN_LEFT")
      {
         encoding += "0,0,0,1,0,0,0,0,0,0,0";
      }
      else if (response == "EAT_MOUSE")
      {
         encoding += "0,0,0,0,1,0,0,0,0,0,0";
      }
      else if (response == "GET_OBJECT")
      {
         encoding += "0,0,0,0,0,1,0,0,0,0,0";
      }
      else if (response == "PUT_OBJECT")
      {
         encoding += "0,0,0,0,0,0,1,0,0,0,0";
      }
      else if (response == "TOSS_OBJECT")
      {
         encoding += "0,0,0,0,0,0,0,1,0,0,0";
      }
      else if (response == "WANT_MOUSE")
      {
         encoding += "0,0,0,0,0,0,0,0,1,0,0";
      }
      else if (response == "WANT_STONE")
      {
         encoding += "0,0,0,0,0,0,0,0,0,1,0";
      }
      else if (response == "LAY_EGG")
      {
         encoding += "0,0,0,0,0,0,0,0,0,0,1";
      }
      else
      {
         encoding += "0,0,0,0,0,0,0,0,0,0,1";
      }
      return(encoding);
   }


   // Get length of response encoding.
   static int oneHotResponseLength()
   {
      return(11);
   }
};

// Behavior.
extern vector < vector < MaleSensoryResponse >> MaleTrainBehavior;
extern vector<MaleSensoryResponse> MaleTestBehavior;
extern vector < vector < FemaleSensoryResponse >> FemaleTrainBehavior;
extern vector<FemaleSensoryResponse> FemaleTestBehavior;

// Import sensors.
void importMaleSensors(string maleSensorFilename, vector<MaleSensoryResponse>& sensors);
void importFemaleSensors(string femaleSensorFilename, vector<FemaleSensoryResponse>& sensors);

// Install sensors.
void installMaleSensors(vector<MaleSensoryResponse>& sensors);
void installFemaleSensors(vector<FemaleSensoryResponse>& sensors);

#endif
