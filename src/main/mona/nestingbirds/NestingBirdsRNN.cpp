// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds RNN.

#include "world.hpp"
#include "datasets.hpp"
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
            fprintf(stderr, "%s", Usage);
            exit(1);
         }
         Steps = atoi(args[i]);
         if (Steps <= 0)
         {
            fprintf(stderr, "Steps must be positive number\n");
            fprintf(stderr, "%s", Usage);
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
            fprintf(stderr, "%s", Usage);
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
               fprintf(stderr, "%s", Usage);
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
            fprintf(stderr, "%s", Usage);
            exit(1);
         }
         string arg(args[i]);
         if (arg == "-dynamic")
         {
            fprintf(stderr, "%s", Usage);
            exit(1);
         }
         TestRandomSeed = atoi(trim(arg).c_str());
         if (TestRandomSeed <= 0)
         {
            fprintf(stderr, "Invalid testRandomSeed %d\n", TestRandomSeed);
            fprintf(stderr, "%s", Usage);
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
            fprintf(stderr, "%s", Usage);
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
            fprintf(stderr, "%s", Usage);
            exit(1);
         }
         continue;
      }
      if ((strcmp(args[i], "-help") == 0) || (strcmp(args[i], "-h") == 0) || (strcmp(args[i], "-?") == 0))
      {
         printf("%s", Usage);
         exit(0);
      }
      fprintf(stderr, "%s", Usage);
      exit(1);
   }
   if (!gotSteps)
   {
      fprintf(stderr, "%s", Usage);
      exit(1);
   }
   if (TrainRandomSeeds.size() == 0)
   {
      fprintf(stderr, "%s", Usage);
      exit(1);
   }
   if ((TestRandomSeed == -1) && Dynamic)
   {
      fprintf(stderr, "%s", Usage);
      exit(1);
   }
   if (Steps == 0)
   {
      exit(0);
   }

   // Write datasets.
   writeDatasets(Steps, TrainRandomSeeds, TestRandomSeed,
                 RNN_MALE_DATASET_FILENAME, RNN_FEMALE_DATASET_FILENAME, Verbose);

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
         MaleTestBehavior.clear();
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
            MaleTestBehavior.push_back(sensoryResponse);
            FILE *fp = fopen(RNN_DATASET_FILENAME, "w");
            if (fp == NULL)
            {
               fprintf(stderr, "Cannot open male dataset file %s\n", RNN_DATASET_FILENAME);
               exit(1);
            }
            fprintf(fp, "X_test_shape = [1, %d, %d]\n", i, MaleSensoryResponse::oneHotSensoryLength());
            fprintf(fp, "X_test_seq = [\n");
            for (int j = 0; j < MaleTestBehavior.size(); j++)
            {
               MaleSensoryResponse sensoryResponse = MaleTestBehavior[j];
               fprintf(fp, "%s", sensoryResponse.sensoryToOneHot().c_str());
               if (j < MaleTestBehavior.size() - 1)
               {
                  fprintf(fp, ",");
               }
               fprintf(fp, "\n");
            }
            fprintf(fp, "]\n");
            fprintf(fp, "y_test_shape = [1, %d, %d]\n", i, MaleSensoryResponse::oneHotResponseLength());
            fprintf(fp, "y_test_seq = [\n");
            for (int j = 0; j < MaleTestBehavior.size(); j++)
            {
               MaleSensoryResponse sensoryResponse = MaleTestBehavior[j];
               fprintf(fp, "%s", sensoryResponse.responseToOneHot().c_str());
               if (j < MaleTestBehavior.size() - 1)
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
            MaleTestBehavior[MaleTestBehavior.size() - 1].response =
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
         FemaleTestBehavior.clear();
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
            FemaleTestBehavior.push_back(sensoryResponse);
            FILE *fp = fopen(RNN_DATASET_FILENAME, "w");
            if (fp == NULL)
            {
               fprintf(stderr, "Cannot open female dataset file %s\n", RNN_DATASET_FILENAME);
               exit(1);
            }
            fprintf(fp, "X_test_shape = [1, %d, %d]\n", i, FemaleSensoryResponse::oneHotSensoryLength());
            fprintf(fp, "X_test_seq = [\n");
            for (int j = 0; j < FemaleTestBehavior.size(); j++)
            {
               FemaleSensoryResponse sensoryResponse = FemaleTestBehavior[j];
               fprintf(fp, "%s", sensoryResponse.sensoryToOneHot().c_str());
               if (j < FemaleTestBehavior.size() - 1)
               {
                  fprintf(fp, ",");
               }
               fprintf(fp, "\n");
            }
            fprintf(fp, "]\n");
            fprintf(fp, "y_test_shape = [1, %d, %d]\n", i, FemaleSensoryResponse::oneHotResponseLength());
            fprintf(fp, "y_test_seq = [\n");
            for (int j = 0; j < FemaleTestBehavior.size(); j++)
            {
               FemaleSensoryResponse sensoryResponse = FemaleTestBehavior[j];
               fprintf(fp, "%s", sensoryResponse.responseToOneHot().c_str());
               if (j < FemaleTestBehavior.size() - 1)
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
            FemaleTestBehavior[FemaleTestBehavior.size() - 1].response =
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
   int mouseTotal = 0;
   int stoneTotal = 0;
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


// Trim string.
string trim(string& str)
{
   str.erase(str.find_last_not_of(' ') + 1);
   str.erase(0, str.find_first_not_of(' '));
   return(str);
}
