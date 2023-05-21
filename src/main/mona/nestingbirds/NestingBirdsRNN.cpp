// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds RNN.

#include "world.hpp"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Usage.
const char* Usage =
"Usage:\n"
"    nestingbirds_rnn\n"
"      -steps <steps>\n"
"      -trainRandomSeeds <random seeds> (comma-separated list)\n"
"      [-testRandomSeed <random seed> [-dynamic (world generates sensory inputs from predicted responses)]]\n"
"      [-verbose <true | false> (default=true)]\n"
"Exit codes:\n"
"  0=success\n"
"  1=error\n";

// Steps.
int Steps;

// Random numbers.
vector<int> TrainRandomSeeds;
int         TestRandomSeed;
bool        Dynamic;

// File names.
#define BEHAVIOR_FILENAME                    (char *)"nestingbirds_rnn_behavior.json"
#define RNN_MALE_DATASET_FILENAME            (char *)"nestingbirds_rnn_male_dataset.csv"
#define RNN_FEMALE_DATASET_FILENAME          (char *)"nestingbirds_rnn_female_dataset.csv"
#define RNN_DATASET_FILENAME                 (char *)"nestingbirds_rnn_dataset.py"
#define TRAIN_RNN_FILENAME                   (char *)"nestingbirds_train_rnn.py"
#define TEST_RNN_FILENAME                    (char *)"nestingbirds_test_rnn.py"
#define RNN_MODEL_FILENAME                   (char *)"nestingbirds_rnn_model"
#define RNN_TRAIN_RESULTS_FILENAME           (char *)"nestingbirds_rnn_train_results.json"
#define RNN_TEST_RESULTS_FILENAME            (char *)"nestingbirds_rnn_test_results.json"
#define RNN_TEST_PREDICTIONS_FILENAME        (char *)"nestingbirds_rnn_test_predictions.txt"
#define RNN_MALE_TRAIN_RESULTS_FILENAME      (char *)"nestingbirds_rnn_male_train_results.json"
#define RNN_MALE_TEST_RESULTS_FILENAME       (char *)"nestingbirds_rnn_male_test_results.json"
#define RNN_FEMALE_TRAIN_RESULTS_FILENAME    (char *)"nestingbirds_rnn_female_train_results.json"
#define RNN_FEMALE_TEST_RESULTS_FILENAME     (char *)"nestingbirds_rnn_female_test_results.json"

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
   string oneHotSensory()
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


   // Get length of sensory encoding.
   static int oneHotSensoryLength()
   {
      return(28);
   }


   // Convert response to one-hot encoding.
   string oneHotResponse()
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
   string oneHotSensory()
   {
      string encoding = "";

      for (int i = 0; i < 9; i++)
      {
         if (cellSensors[i] == "NO_OBJECT")
         {
            encoding += "1,0,0";
         }
         else if (cellSensors[i] == "MOUSE")
         {
            encoding += "0,1,0";
         }
         else
         {
            encoding += "0,0,1";
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


   // Get length of sensory encoding.
   static int oneHotSensoryLength()
   {
      return(37);
   }


   // Convert response to one-hot encoding.
   string oneHotResponse()
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

// Imported behavior datasets.
vector < vector < MaleSensoryResponse >> MaleTrainDataset;
vector<MaleSensoryResponse> MaleTestDataset;
vector < vector < FemaleSensoryResponse >> FemaleTrainDataset;
vector<FemaleSensoryResponse> FemaleTestDataset;
void generateBehavior(int randomSeed, int steps);
void importBehaviorDataset(
   vector<MaleSensoryResponse>&   maleSequence,
   vector<FemaleSensoryResponse>& femaleSequence);
void createRNNdatasets();

// Write nest completion results.
void writeNestResults(char *filename);

// Trim string.
string trim(string& str);

// Main.
int main(int argc, char *args[])
{
   TestRandomSeed = -1;
   Dynamic        = false;
   bool gotSteps = false;

   for (int i = 1; i < argc; i++)
   {
      if (strcmp(args[i], "-steps") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid steps option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Steps = atoi(args[i]);
         if (Steps < 0)
         {
            fprintf(stderr, "Invalid steps option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         gotSteps = true;
         continue;
      }
      if (strcmp(args[i], "-trainRandomSeeds") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid trainRandomSeeds option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         stringstream s_stream(args[i]);
         while (s_stream.good())
         {
            string substr;
            getline(s_stream, substr, ',');
            int seed = atoi(trim(substr).c_str());
            if (seed <= 0)
            {
               fprintf(stderr, "Invalid trainRandomSeed %d\n", seed);
               fprintf(stderr, Usage);
               exit(1);
            }
            TrainRandomSeeds.push_back(seed);
         }
         continue;
      }
      if (strcmp(args[i], "-testRandomSeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid testRandomSeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         string arg(args[i]);
         if (arg == "-dynamic")
         {
            fprintf(stderr, Usage);
            exit(1);
         }
         TestRandomSeed = atoi(trim(arg).c_str());
         if (TestRandomSeed <= 0)
         {
            fprintf(stderr, "Invalid testRandomSeed %d\n", TestRandomSeed);
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-dynamic") == 0)
      {
         Dynamic = true;
         continue;
      }
      if (strcmp(args[i], "-verbose") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid verbose option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         if (strcmp(args[i], "true") == 0)
         {
            Verbose = true;
         }
         else if (strcmp(args[i], "false") == 0)
         {
            Verbose = false;
         }
         else
         {
            fprintf(stderr, "Invalid verbose option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if ((strcmp(args[i], "-help") == 0) || (strcmp(args[i], "-h") == 0) || (strcmp(args[i], "-?") == 0))
      {
         printf(Usage);
         exit(0);
      }
      fprintf(stderr, Usage);
      exit(1);
   }
   if (!gotSteps)
   {
      fprintf(stderr, Usage);
      exit(1);
   }
   if (TrainRandomSeeds.size() == 0)
   {
      fprintf(stderr, Usage);
      exit(1);
   }
   if ((TestRandomSeed == -1) && Dynamic)
   {
      fprintf(stderr, Usage);
      exit(1);
   }
   if (Steps == 0)
   {
      exit(0);
   }

   // Create behavior training datasets.
   for (int randomSeed : TrainRandomSeeds)
   {
      vector<MaleSensoryResponse>   maleSequence;
      vector<FemaleSensoryResponse> femaleSequence;
      generateBehavior(randomSeed, Steps);
      importBehaviorDataset(maleSequence, femaleSequence);
      MaleTrainDataset.push_back(maleSequence);
      FemaleTrainDataset.push_back(femaleSequence);
   }

   // Create behavior testing datasets.
   if (TestRandomSeed != -1)
   {
      generateBehavior(TestRandomSeed, Steps);
      importBehaviorDataset(MaleTestDataset, FemaleTestDataset);
   }

   // Convert to RNN datasets.
   createRNNdatasets();

   // Run RNN.
   if (Verbose)
   {
      printf("Running male dataset\n");
   }
   char buf[BUFSIZ];
   sprintf(buf, "cp %s %s", RNN_MALE_DATASET_FILENAME, RNN_DATASET_FILENAME);
   system(buf);
   sprintf(buf, "python %s", TRAIN_RNN_FILENAME);
   system(buf);
   sprintf(buf, "cp %s %s", RNN_TRAIN_RESULTS_FILENAME, RNN_MALE_TRAIN_RESULTS_FILENAME);
   system(buf);
   if (Verbose)
   {
      printf("Male train results in %s\n", RNN_MALE_TRAIN_RESULTS_FILENAME);
   }
   if (TestRandomSeed != -1)
   {
      if (Dynamic)
      {
         MaleTestDataset.clear();
         RANDOM_NUMBER_SEED           = TestRandomSeed;
         Male::RANDOMIZE_FOOD_LEVEL   = true;
         Female::RANDOMIZE_FOOD_LEVEL = true;
         init(true, false);
         int eggLaidStep = -1;
         int mouseTotal  = 0;
         int stoneTotal  = 0;
         for (int x = 0; x < WIDTH; x++)
         {
            for (int y = 0; y < HEIGHT; y++)
            {
               if ((World[x][y].locale == LOCALE::FOREST) &&
                   (World[x][y].object == OBJECT::MOUSE))
               {
                  mouseTotal++;
               }
               if ((World[x][y].locale == LOCALE::DESERT) &&
                   (World[x][y].object == OBJECT::STONE))
               {
                  stoneTotal++;
               }
            }
         }
         char buf[BUFSIZ];
         for (int i = 1; i <= Steps; i++)
         {
            stepMice();
            preCycleFemale();
            female->cycle();
            postCycleFemale();
            preCycleMale();
            MaleSensoryResponse sensoryResponse;
            int                 sensor = male->sensors[Male::LOCALE_SENSOR];
            sensoryResponse.locale = LOCALE::toString(sensor);
            sensor = male->sensors[Male::MOUSE_PROXIMITY_SENSOR];
            sensoryResponse.mouseProximity = Male::PROXIMITY::toString(sensor);
            sensor = male->sensors[Male::STONE_PROXIMITY_SENSOR];
            sensoryResponse.stoneProximity = Male::PROXIMITY::toString(sensor);
            sensor = male->sensors[Male::FEMALE_PROXIMITY_SENSOR];
            sensoryResponse.femaleProximity = Male::PROXIMITY::toString(sensor);
            sensor = male->sensors[Male::GOAL_SENSOR];
            sensoryResponse.goal = Male::GOAL::toString(sensor);
            sensor = male->sensors[Male::HAS_OBJECT_SENSOR];
            sensoryResponse.hasObject = OBJECT::toString(sensor);
            if (male->sensors[Male::FLYING_SENSOR] == 1)
            {
               sensoryResponse.flying = "true";
            }
            else
            {
               sensoryResponse.flying = "false";
            }
            if (male->sensors[Male::FEMALE_WANTS_MOUSE_SENSOR] == 1)
            {
               sensoryResponse.femaleWantsMouse = "true";
            }
            else
            {
               sensoryResponse.femaleWantsMouse = "false";
            }
            if (male->sensors[Male::FEMALE_WANTS_STONE_SENSOR] == 1)
            {
               sensoryResponse.femaleWantsStone = "true";
            }
            else
            {
               sensoryResponse.femaleWantsStone = "false";
            }
            sensoryResponse.response = Male::RESPONSE::toString(male->response);
            MaleTestDataset.push_back(sensoryResponse);
            FILE *fp = fopen(RNN_DATASET_FILENAME, "w");
            if (fp == NULL)
            {
               fprintf(stderr, "Cannot open male dataset file %s\n", RNN_DATASET_FILENAME);
               exit(1);
            }
            fprintf(fp, "X_test_shape = [1, %d, %d]\n", i, MaleSensoryResponse::oneHotSensoryLength());
            fprintf(fp, "X_test_seq = [\n");
            for (int j = 0; j < MaleTestDataset.size(); j++)
            {
               MaleSensoryResponse sensoryResponse = MaleTestDataset[j];
               fprintf(fp, "%s", sensoryResponse.oneHotSensory().c_str());
               if (j < MaleTestDataset.size() - 1)
               {
                  fprintf(fp, ",");
               }
               fprintf(fp, "\n");
            }
            fprintf(fp, "]\n");
            fprintf(fp, "y_test_shape = [1, %d, %d]\n", i, MaleSensoryResponse::oneHotResponseLength());
            fprintf(fp, "y_test_seq = [\n");
            for (int j = 0; j < MaleTestDataset.size(); j++)
            {
               MaleSensoryResponse sensoryResponse = MaleTestDataset[j];
               fprintf(fp, "%s", sensoryResponse.oneHotResponse().c_str());
               if (j < MaleTestDataset.size() - 1)
               {
                  fprintf(fp, ",");
               }
               fprintf(fp, "\n");
            }
            fprintf(fp, "]\n");
            fclose(fp);
            sprintf(buf, "python %s", TEST_RNN_FILENAME);
            system(buf);
            fp = fopen(RNN_TEST_PREDICTIONS_FILENAME, "r");
            if (fp == NULL)
            {
               fprintf(stderr, "Cannot open male test predictions file %s\n", RNN_TEST_PREDICTIONS_FILENAME);
               exit(1);
            }
            int response = -1;
            int n;
            while (fscanf(fp, "%d", &n) != EOF)
            {
               response = n;
            }
            fclose(fp);
            if (response == -1)
            {
               fprintf(stderr, "Invalid male test predictions file %s\n", RNN_TEST_PREDICTIONS_FILENAME);
               exit(1);
            }
            MaleTestDataset[MaleTestDataset.size() - 1].response =
               Male::RESPONSE::toString(response);
            male->response = response;
            male->setResponseOverride();
            male->cycle();
            postCycleMale();
            if ((eggLaidStep < 0) && (World[NEST_CENTER_X][NEST_CENTER_Y].object == OBJECT::EGG))
            {
               eggLaidStep = i;
            }
         }
         writeNestResults(RNN_MALE_TEST_RESULTS_FILENAME);
         if (Verbose)
         {
            printf("Run results: ");
            if (eggLaidStep >= 1)
            {
               printf("egg laid at step %d", eggLaidStep);
            }
            else
            {
               printf("no egg");
            }
            int mouseCount = 0;
            int stoneCount = 0;
            for (int x = 0; x < WIDTH; x++)
            {
               for (int y = 0; y < HEIGHT; y++)
               {
                  if ((World[x][y].locale == LOCALE::FOREST) &&
                      (World[x][y].object == OBJECT::MOUSE))
                  {
                     mouseCount++;
                  }
                  if ((World[x][y].locale == LOCALE::DESERT) &&
                      (World[x][y].object == OBJECT::STONE))
                  {
                     stoneCount++;
                  }
               }
            }
            printf(", remaining mice=%d/%d, remaining stones=%d/%d\n", mouseCount, mouseTotal, stoneCount, stoneTotal);
         }
      }
      else
      {
         sprintf(buf, "python %s", TEST_RNN_FILENAME);
         system(buf);
         sprintf(buf, "cp %s %s", RNN_TEST_RESULTS_FILENAME, RNN_MALE_TEST_RESULTS_FILENAME);
         system(buf);
      }
      if (Verbose)
      {
         printf("Male test results in %s\n", RNN_MALE_TEST_RESULTS_FILENAME);
      }
   }
   if (Verbose)
   {
      printf("Running female dataset\n");
   }
   sprintf(buf, "cp %s %s", RNN_FEMALE_DATASET_FILENAME, RNN_DATASET_FILENAME);
   system(buf);
   sprintf(buf, "python %s", TRAIN_RNN_FILENAME);
   system(buf);
   sprintf(buf, "cp %s %s", RNN_TRAIN_RESULTS_FILENAME, RNN_FEMALE_TRAIN_RESULTS_FILENAME);
   system(buf);
   if (Verbose)
   {
      printf("Female train results in %s\n", RNN_FEMALE_TRAIN_RESULTS_FILENAME);
   }
   if (TestRandomSeed != -1)
   {
      if (Dynamic)
      {
         FemaleTestDataset.clear();
         RANDOM_NUMBER_SEED           = TestRandomSeed;
         Male::RANDOMIZE_FOOD_LEVEL   = true;
         Female::RANDOMIZE_FOOD_LEVEL = true;
         init(false, true);
         int eggLaidStep = -1;
         int mouseTotal  = 0;
         int stoneTotal  = 0;
         for (int x = 0; x < WIDTH; x++)
         {
            for (int y = 0; y < HEIGHT; y++)
            {
               if ((World[x][y].locale == LOCALE::FOREST) &&
                   (World[x][y].object == OBJECT::MOUSE))
               {
                  mouseTotal++;
               }
               if ((World[x][y].locale == LOCALE::DESERT) &&
                   (World[x][y].object == OBJECT::STONE))
               {
                  stoneTotal++;
               }
            }
         }
         char buf[BUFSIZ];
         for (int i = 1; i <= Steps; i++)
         {
            stepMice();
            preCycleFemale();
            FemaleSensoryResponse sensoryResponse;
            int sensor = female->sensors[Female::CURRENT_OBJECT_SENSOR];
            sensoryResponse.cellSensors.push_back(OBJECT::toString(sensor));
            sensor = female->sensors[Female::LEFT_OBJECT_SENSOR];
            sensoryResponse.cellSensors.push_back(OBJECT::toString(sensor));
            sensor = female->sensors[Female::LEFT_FRONT_OBJECT_SENSOR];
            sensoryResponse.cellSensors.push_back(OBJECT::toString(sensor));
            sensor = female->sensors[Female::FRONT_OBJECT_SENSOR];
            sensoryResponse.cellSensors.push_back(OBJECT::toString(sensor));
            sensor = female->sensors[Female::RIGHT_FRONT_OBJECT_SENSOR];
            sensoryResponse.cellSensors.push_back(OBJECT::toString(sensor));
            sensor = female->sensors[Female::RIGHT_OBJECT_SENSOR];
            sensoryResponse.cellSensors.push_back(OBJECT::toString(sensor));
            sensor = female->sensors[Female::RIGHT_REAR_OBJECT_SENSOR];
            sensoryResponse.cellSensors.push_back(OBJECT::toString(sensor));
            sensor = female->sensors[Female::REAR_OBJECT_SENSOR];
            sensoryResponse.cellSensors.push_back(OBJECT::toString(sensor));
            sensor = female->sensors[Female::LEFT_REAR_OBJECT_SENSOR];
            sensoryResponse.cellSensors.push_back(OBJECT::toString(sensor));
            sensor = female->sensors[Female::ORIENTATION_SENSOR];
            sensoryResponse.orientation = ORIENTATION::toString(sensor);
            sensor = female->sensors[Female::GOAL_SENSOR];
            sensoryResponse.goal = Female::GOAL::toString(sensor);
            sensor = female->sensors[Female::HAS_OBJECT_SENSOR];
            sensoryResponse.hasObject = OBJECT::toString(sensor);
            sensoryResponse.response  = Female::RESPONSE::toString(female->response);
            FemaleTestDataset.push_back(sensoryResponse);
            FILE *fp = fopen(RNN_DATASET_FILENAME, "w");
            if (fp == NULL)
            {
               fprintf(stderr, "Cannot open female dataset file %s\n", RNN_DATASET_FILENAME);
               exit(1);
            }
            fprintf(fp, "X_test_shape = [1, %d, %d]\n", i, FemaleSensoryResponse::oneHotSensoryLength());
            fprintf(fp, "X_test_seq = [\n");
            for (int j = 0; j < FemaleTestDataset.size(); j++)
            {
               FemaleSensoryResponse sensoryResponse = FemaleTestDataset[j];
               fprintf(fp, "%s", sensoryResponse.oneHotSensory().c_str());
               if (j < FemaleTestDataset.size() - 1)
               {
                  fprintf(fp, ",");
               }
               fprintf(fp, "\n");
            }
            fprintf(fp, "]\n");
            fprintf(fp, "y_test_shape = [1, %d, %d]\n", i, FemaleSensoryResponse::oneHotResponseLength());
            fprintf(fp, "y_test_seq = [\n");
            for (int j = 0; j < FemaleTestDataset.size(); j++)
            {
               FemaleSensoryResponse sensoryResponse = FemaleTestDataset[j];
               fprintf(fp, "%s", sensoryResponse.oneHotResponse().c_str());
               if (j < FemaleTestDataset.size() - 1)
               {
                  fprintf(fp, ",");
               }
               fprintf(fp, "\n");
            }
            fprintf(fp, "]\n");
            fclose(fp);
            sprintf(buf, "python %s", TEST_RNN_FILENAME);
            system(buf);
            fp = fopen(RNN_TEST_PREDICTIONS_FILENAME, "r");
            if (fp == NULL)
            {
               fprintf(stderr, "Cannot open female test predictions file %s\n", RNN_TEST_PREDICTIONS_FILENAME);
               exit(1);
            }
            int response = -1;
            int n;
            while (fscanf(fp, "%d", &n) != EOF)
            {
               response = n;
            }
            fclose(fp);
            if (response == -1)
            {
               fprintf(stderr, "Invalid female test predictions file %s\n", RNN_TEST_PREDICTIONS_FILENAME);
               exit(1);
            }
            FemaleTestDataset[FemaleTestDataset.size() - 1].response =
               Female::RESPONSE::toString(response);
            female->response = response;
            female->setResponseOverride();
            female->cycle();
            postCycleFemale();
            preCycleMale();
            male->cycle();
            postCycleMale();
            if ((eggLaidStep < 0) && (World[NEST_CENTER_X][NEST_CENTER_Y].object == OBJECT::EGG))
            {
               eggLaidStep = i;
            }
         }
         writeNestResults(RNN_FEMALE_TEST_RESULTS_FILENAME);
         if (Verbose)
         {
            printf("Run results: ");
            if (eggLaidStep >= 1)
            {
               printf("egg laid at step %d", eggLaidStep);
            }
            else
            {
               printf("no egg");
            }
            int mouseCount = 0;
            int stoneCount = 0;
            for (int x = 0; x < WIDTH; x++)
            {
               for (int y = 0; y < HEIGHT; y++)
               {
                  if ((World[x][y].locale == LOCALE::FOREST) &&
                      (World[x][y].object == OBJECT::MOUSE))
                  {
                     mouseCount++;
                  }
                  if ((World[x][y].locale == LOCALE::DESERT) &&
                      (World[x][y].object == OBJECT::STONE))
                  {
                     stoneCount++;
                  }
               }
            }
            printf(", remaining mice=%d/%d, remaining stones=%d/%d\n", mouseCount, mouseTotal, stoneCount, stoneTotal);
         }
      }
      else
      {
         sprintf(buf, "python %s", TEST_RNN_FILENAME);
         system(buf);
         sprintf(buf, "cp %s %s", RNN_TEST_RESULTS_FILENAME, RNN_FEMALE_TEST_RESULTS_FILENAME);
         system(buf);
      }
      if (Verbose)
      {
         printf("Female test results in %s\n", RNN_FEMALE_TEST_RESULTS_FILENAME);
      }
   }

   // Clean up files.
   unlink(RNN_MALE_DATASET_FILENAME);
   unlink(RNN_FEMALE_DATASET_FILENAME);
   unlink(RNN_DATASET_FILENAME);
   unlink(RNN_TRAIN_RESULTS_FILENAME);
   unlink(RNN_TEST_RESULTS_FILENAME);
   unlink(RNN_TEST_PREDICTIONS_FILENAME);

   exit(0);
}


// Write nest completion results.
void writeNestResults(char *filename)
{
   int nestCount = 0;

   if (World[NEST_CENTER_X][NEST_CENTER_Y - 1].object == OBJECT::STONE)
   {
      nestCount++;
   }
   if (World[NEST_CENTER_X - 1][NEST_CENTER_Y - 1].object == OBJECT::STONE)
   {
      nestCount++;
   }
   if (World[NEST_CENTER_X - 1][NEST_CENTER_Y].object == OBJECT::STONE)
   {
      nestCount++;
   }
   if (World[NEST_CENTER_X - 1][NEST_CENTER_Y + 1].object == OBJECT::STONE)
   {
      nestCount++;
   }
   if (World[NEST_CENTER_X][NEST_CENTER_Y + 1].object == OBJECT::STONE)
   {
      nestCount++;
   }
   if (World[NEST_CENTER_X + 1][NEST_CENTER_Y + 1].object == OBJECT::STONE)
   {
      nestCount++;
   }
   if (World[NEST_CENTER_X + 1][NEST_CENTER_Y].object == OBJECT::STONE)
   {
      nestCount++;
   }
   if (World[NEST_CENTER_X + 1][NEST_CENTER_Y - 1].object == OBJECT::STONE)
   {
      nestCount++;
   }
   if (World[NEST_CENTER_X][NEST_CENTER_Y].object == OBJECT::EGG)
   {
      nestCount++;
   }
   FILE *fp;
   if ((fp = fopen(filename, "w")) == NULL)
   {
      fprintf(stderr, "Cannot write test results file %s\n", filename);
      exit(1);
   }
   fprintf(fp, "{\"nest_completed\":\"%d\", \"nest_total\":\"9\"}\n", nestCount);
   fclose(fp);
}


// Generate behavior.
void generateBehavior(int randomSeed, int steps)
{
   RANDOM_NUMBER_SEED           = randomSeed;
   Male::RANDOMIZE_FOOD_LEVEL   = true;
   Female::RANDOMIZE_FOOD_LEVEL = true;
   init(false, false);
   openBehaviorFile(BEHAVIOR_FILENAME);
   int  eggLaidStep = -1;
   char buf[BUFSIZ];
   if (Verbose)
   {
      printf("Generating behavior file %s\n", BEHAVIOR_FILENAME);
   }
   int mouseCount = 0;
   int stoneCount = 0;
   for (int x = 0; x < WIDTH; x++)
   {
       for (int y = 0; y < HEIGHT; y++)
       {
           if ((World[x][y].locale == LOCALE::FOREST) &&
               (World[x][y].object == OBJECT::MOUSE))
           {
               mouseCount++;
           }
           if ((World[x][y].locale == LOCALE::DESERT) &&
               (World[x][y].object == OBJECT::STONE))
           {
               stoneCount++;
           }
       }
   }
   for (int i = 1; i <= steps; i++)
   {
      if (Verbose)
      {
         printf("Step=%d\n", i);
      }
      sprintf(buf, "{ \"Step\": %d, \"Data\": {\n", i);
      writeBehaviorFile(buf);
      step();
      writeBehaviorFile((char *)"} }");
      if (i < Steps)
      {
         writeBehaviorFile((char *)",");
      }
      writeBehaviorFile((char *)"\n");
      if ((eggLaidStep < 0) && (World[NEST_CENTER_X][NEST_CENTER_Y].object == OBJECT::EGG))
      {
         eggLaidStep = i;
      }
   }
   if (Verbose)
   {
      printf("Run results: ");
      if (eggLaidStep >= 1)
      {
         printf("egg laid at step %d", eggLaidStep);
      }
      else
      {
         printf("no egg");
      }
      int mouseCount = 0;
      int stoneCount = 0;
      for (int x = 0; x < WIDTH; x++)
      {
         for (int y = 0; y < HEIGHT; y++)
         {
            if ((World[x][y].locale == LOCALE::FOREST) &&
                (World[x][y].object == OBJECT::MOUSE))
            {
               mouseCount++;
            }
            if ((World[x][y].locale == LOCALE::DESERT) &&
                (World[x][y].object == OBJECT::STONE))
            {
               stoneCount++;
            }
         }
      }
      printf(", remaining mice=%d/%d, remaining stones=%d/%d\n", mouseCount, mouseTotal, stoneCount, stoneTotal);
   }
   closeBehaviorFile();
}


// Import behavior dataset.
void importBehaviorDataset(
   vector<MaleSensoryResponse>&   maleSequence,
   vector<FemaleSensoryResponse>& femaleSequence)
{
   if (Verbose)
   {
      printf("Importing behavior dataset\n");
   }
   maleSequence.clear();
   femaleSequence.clear();
   ifstream file;
   file.open(BEHAVIOR_FILENAME);
   if (!file.is_open())
   {
      fprintf(stderr, "Cannot open behavior file %s\n", BEHAVIOR_FILENAME);
      exit(1);
   }
   string json;
   while (getline(file, json))
   {
      string gender = json.substr(1, 2);
      if (gender == "Fe")
      {
         FemaleSensoryResponse sensoryResponse;
         int end = json.find("\"");
         int i   = 0;
         while (end != -1)
         {
            switch (i)
            {
            case 15:
               sensoryResponse.cellSensors.push_back(json.substr(0, end));
               break;

            case 19:
               sensoryResponse.cellSensors.push_back(json.substr(0, end));
               break;

            case 23:
               sensoryResponse.cellSensors.push_back(json.substr(0, end));
               break;

            case 27:
               sensoryResponse.cellSensors.push_back(json.substr(0, end));
               break;

            case 31:
               sensoryResponse.cellSensors.push_back(json.substr(0, end));
               break;

            case 35:
               sensoryResponse.cellSensors.push_back(json.substr(0, end));
               break;

            case 39:
               sensoryResponse.cellSensors.push_back(json.substr(0, end));
               break;

            case 43:
               sensoryResponse.cellSensors.push_back(json.substr(0, end));
               break;

            case 47:
               sensoryResponse.cellSensors.push_back(json.substr(0, end));
               break;

            case 51:
               sensoryResponse.orientation = json.substr(0, end);
               break;

            case 55:
               sensoryResponse.goal = json.substr(0, end);
               break;

            case 59:
               sensoryResponse.hasObject = json.substr(0, end);
               break;

            case 73:
               sensoryResponse.response = json.substr(0, end);
               break;
            }
            json.erase(json.begin(), json.begin() + end + 1);
            end = json.find("\"");
            i++;
         }
         if (Verbose)
         {
            printf("Female data:\n");
            sensoryResponse.print();
         }
         femaleSequence.push_back(sensoryResponse);
         continue;
      }
      if (gender == "Ma")
      {
         MaleSensoryResponse sensoryResponse;
         int                 end = json.find("\"");
         int                 i   = 0;
         while (end != -1)
         {
            switch (i)
            {
            case 13:
               sensoryResponse.locale = json.substr(0, end);
               break;

            case 17:
               sensoryResponse.mouseProximity = json.substr(0, end);
               break;

            case 21:
               sensoryResponse.stoneProximity = json.substr(0, end);
               break;

            case 25:
               sensoryResponse.femaleProximity = json.substr(0, end);
               break;

            case 29:
               sensoryResponse.goal = json.substr(0, end);
               break;

            case 33:
               sensoryResponse.hasObject = json.substr(0, end);
               break;

            case 36:
               if (json.substr(0, end).find("true") != string::npos)
               {
                  sensoryResponse.flying = "true";
               }
               else
               {
                  sensoryResponse.flying = "false";
               }
               break;

            case 38:
               if (json.substr(0, end).find("true") != string::npos)
               {
                  sensoryResponse.femaleWantsMouse = "true";
               }
               else
               {
                  sensoryResponse.femaleWantsMouse = "false";
               }
               break;

            case 40:
               if (json.substr(0, end).find("true") != string::npos)
               {
                  sensoryResponse.femaleWantsStone = "true";
               }
               else
               {
                  sensoryResponse.femaleWantsStone = "false";
               }
               break;

            case 55:
               sensoryResponse.response = json.substr(0, end);
               break;
            }
            json.erase(json.begin(), json.begin() + end + 1);
            end = json.find("\"");
            i++;
         }
         if (Verbose)
         {
            printf("Male data:\n");
            sensoryResponse.print();
         }
         maleSequence.push_back(sensoryResponse);
         continue;
      }
   }
   file.close();
   unlink(BEHAVIOR_FILENAME);
}


// Convert to RNN datasets.
void createRNNdatasets()
{
   // Create male dataset.
   FILE *fp = fopen(RNN_MALE_DATASET_FILENAME, "w");

   if (fp == NULL)
   {
      fprintf(stderr, "Cannot open male dataset file %s\n", RNN_MALE_DATASET_FILENAME);
      exit(1);
   }
   fprintf(fp, "X_train_shape = [%d, %d, %d]\n", MaleTrainDataset.size(), Steps, MaleSensoryResponse::oneHotSensoryLength());
   fprintf(fp, "X_train_seq = [\n");
   for (int i = 0; i < MaleTrainDataset.size(); i++)
   {
      vector<MaleSensoryResponse> behaviorSequence = MaleTrainDataset[i];
      for (int j = 0; j < behaviorSequence.size(); j++)
      {
         MaleSensoryResponse sensoryResponse = behaviorSequence[j];
         fprintf(fp, "%s", sensoryResponse.oneHotSensory().c_str());
         if ((i < MaleTrainDataset.size() - 1) || (j < behaviorSequence.size() - 1))
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   fprintf(fp, "y_train_shape = [%d, %d, %d]\n", MaleTrainDataset.size(), Steps, MaleSensoryResponse::oneHotResponseLength());
   fprintf(fp, "y_train_seq = [\n");
   for (int i = 0; i < MaleTrainDataset.size(); i++)
   {
      vector<MaleSensoryResponse> behaviorSequence = MaleTrainDataset[i];
      for (int j = 0; j < behaviorSequence.size(); j++)
      {
         MaleSensoryResponse sensoryResponse = behaviorSequence[j];
         fprintf(fp, "%s", sensoryResponse.oneHotResponse().c_str());
         if ((i < MaleTrainDataset.size() - 1) || (j < behaviorSequence.size() - 1))
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   if (MaleTestDataset.size() == 0)
   {
      fprintf(fp, "X_test_shape = [0, %d, %d]\n", Steps, MaleSensoryResponse::oneHotSensoryLength());
      fprintf(fp, "X_test_seq = [\n");
   }
   else
   {
      fprintf(fp, "X_test_shape = [1, %d, %d]\n", Steps, MaleSensoryResponse::oneHotSensoryLength());
      fprintf(fp, "X_test_seq = [\n");
      for (int i = 0; i < MaleTestDataset.size(); i++)
      {
         MaleSensoryResponse sensoryResponse = MaleTestDataset[i];
         fprintf(fp, "%s", sensoryResponse.oneHotSensory().c_str());
         if (i < MaleTestDataset.size() - 1)
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   if (MaleTestDataset.size() == 0)
   {
      fprintf(fp, "y_test_shape = [0, %d, %d]\n", Steps, MaleSensoryResponse::oneHotResponseLength());
      fprintf(fp, "y_test_seq = [\n");
   }
   else
   {
      fprintf(fp, "y_test_shape = [1, %d, %d]\n", Steps, MaleSensoryResponse::oneHotResponseLength());
      fprintf(fp, "y_test_seq = [\n");
      for (int i = 0; i < MaleTestDataset.size(); i++)
      {
         MaleSensoryResponse sensoryResponse = MaleTestDataset[i];
         fprintf(fp, "%s", sensoryResponse.oneHotResponse().c_str());
         if (i < MaleTestDataset.size() - 1)
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   fclose(fp);

   // Create female dataset.
   fp = fopen(RNN_FEMALE_DATASET_FILENAME, "w");
   if (fp == NULL)
   {
      fprintf(stderr, "Cannot open female dataset file %s\n", RNN_FEMALE_DATASET_FILENAME);
      exit(1);
   }
   fprintf(fp, "X_train_shape = [%d, %d, %d]\n", FemaleTrainDataset.size(), Steps, FemaleSensoryResponse::oneHotSensoryLength());
   fprintf(fp, "X_train_seq = [\n");
   for (int i = 0; i < FemaleTrainDataset.size(); i++)
   {
      vector<FemaleSensoryResponse> behaviorSequence = FemaleTrainDataset[i];
      for (int j = 0; j < behaviorSequence.size(); j++)
      {
         FemaleSensoryResponse sensoryResponse = behaviorSequence[j];
         fprintf(fp, "%s", sensoryResponse.oneHotSensory().c_str());
         if ((i < FemaleTrainDataset.size() - 1) || (j < behaviorSequence.size() - 1))
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   fprintf(fp, "y_train_shape = [%d, %d, %d]\n", FemaleTrainDataset.size(), Steps, FemaleSensoryResponse::oneHotResponseLength());
   fprintf(fp, "y_train_seq = [\n");
   for (int i = 0; i < FemaleTrainDataset.size(); i++)
   {
      vector<FemaleSensoryResponse> behaviorSequence = FemaleTrainDataset[i];
      for (int j = 0; j < behaviorSequence.size(); j++)
      {
         FemaleSensoryResponse sensoryResponse = behaviorSequence[j];
         fprintf(fp, "%s", sensoryResponse.oneHotResponse().c_str());
         if ((i < FemaleTrainDataset.size() - 1) || (j < behaviorSequence.size() - 1))
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   if (FemaleTestDataset.size() == 0)
   {
      fprintf(fp, "X_test_shape = [0, %d, %d]\n", Steps, FemaleSensoryResponse::oneHotSensoryLength());
      fprintf(fp, "X_test_seq = [\n");
   }
   else
   {
      fprintf(fp, "X_test_shape = [1, %d, %d]\n", Steps, FemaleSensoryResponse::oneHotSensoryLength());
      fprintf(fp, "X_test_seq = [\n");
      for (int i = 0; i < FemaleTestDataset.size(); i++)
      {
         FemaleSensoryResponse sensoryResponse = FemaleTestDataset[i];
         fprintf(fp, "%s", sensoryResponse.oneHotSensory().c_str());
         if (i < FemaleTestDataset.size() - 1)
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   if (FemaleTestDataset.size() == 0)
   {
      fprintf(fp, "y_test_shape = [0, %d, %d]\n", Steps, FemaleSensoryResponse::oneHotResponseLength());
      fprintf(fp, "y_test_seq = [\n");
   }
   else
   {
      fprintf(fp, "y_test_shape = [1, %d, %d]\n", Steps, FemaleSensoryResponse::oneHotResponseLength());
      fprintf(fp, "y_test_seq = [\n");
      for (int i = 0; i < MaleTestDataset.size(); i++)
      {
         FemaleSensoryResponse sensoryResponse = FemaleTestDataset[i];
         fprintf(fp, "%s", sensoryResponse.oneHotResponse().c_str());
         if (i < FemaleTestDataset.size() - 1)
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   fclose(fp);
}


// Trim string.
string trim(string& str)
{
   str.erase(str.find_last_not_of(' ') + 1);
   str.erase(0, str.find_first_not_of(' '));
   return(str);
}
