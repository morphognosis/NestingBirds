// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "datasets.hpp"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Behavior data.
string BehaviorWorkFilename = "nestingbirds_behavior.json";
vector < vector < MaleSensoryResponse >> MaleTrainBehavior;
vector<MaleSensoryResponse> MaleTestBehavior;
vector < vector < FemaleSensoryResponse >> FemaleTrainBehavior;
vector<FemaleSensoryResponse> FemaleTestBehavior;
void generateBehavior(int randomSeed, int steps, bool verbose);
void importBehaviorDataset(
   vector<MaleSensoryResponse>& maleSequence,
   vector<FemaleSensoryResponse>& femaleSequence,
   bool discrimination, bool verbose);
void convertBehaviorToDatasets(int steps, string maleDatasetName, string femaleDatasetName);
void convertBehaviorToPatternDatasets(int steps, string maleDatasetName, string femaleDatasetName);

// Write datasets.
// If testRandomSeed == -1, do not write test dataset.
void writeDatasets(int steps, vector<int> trainRandomSeeds, int testRandomSeed,
                   string maleDatasetName, string femaleDatasetName, bool verbose)
{
   if (steps <= 0)
   {
      fprintf(stderr, "Steps must be positive number\n");
      exit(1);
   }
   if (trainRandomSeeds.size() == 0)
   {
      fprintf(stderr, "Must have at least one training random seed\n");
      exit(1);
   }

   MaleTrainBehavior.clear();
   MaleTestBehavior.clear();
   FemaleTrainBehavior.clear();
   FemaleTestBehavior.clear();

   // Create behavior training datasets.
   for (int randomSeed : trainRandomSeeds)
   {
      vector<MaleSensoryResponse>   maleSequence;
      vector<FemaleSensoryResponse> femaleSequence;
      generateBehavior(randomSeed, steps, verbose);
      importBehaviorDataset(maleSequence, femaleSequence, false, verbose);
      MaleTrainBehavior.push_back(maleSequence);
      FemaleTrainBehavior.push_back(femaleSequence);
   }

   // Create behavior testing datasets.
   if (testRandomSeed != -1)
   {
      generateBehavior(testRandomSeed, steps, verbose);
      importBehaviorDataset(MaleTestBehavior, FemaleTestBehavior, false, verbose);
   }

   // Convert behavior to datasets.
   convertBehaviorToDatasets(steps, maleDatasetName, femaleDatasetName);
}


// Write sensor discrimination datasets.
void writeSensorDiscriminationDatasets(int steps, vector<int> randomSeeds,
                                       string maleDatasetName, string femaleDatasetName, bool verbose)
{
   if (randomSeeds.size() == 0)
   {
      fprintf(stderr, "Must supply at least one random seed\n");
      exit(1);
   }
   if (steps == 0)
   {
      return;
   }

   // Create behavior datasets.
   for (int randomSeed : randomSeeds)
   {
      vector<MaleSensoryResponse>   maleSequence;
      vector<FemaleSensoryResponse> femaleSequence;
      generateBehavior(randomSeed, steps, verbose);
      importBehaviorDataset(maleSequence, femaleSequence, true, verbose);
      MaleTrainBehavior.push_back(maleSequence);
      FemaleTrainBehavior.push_back(femaleSequence);
   }

   // Convert behavior to pattern datasets.
   convertBehaviorToPatternDatasets(steps, maleDatasetName, femaleDatasetName);
}


// Generate behavior.
void generateBehavior(int randomSeed, int steps, bool verbose)
{
   RANDOM_NUMBER_SEED           = randomSeed;
   Male::RANDOMIZE_FOOD_LEVEL   = true;
   Female::RANDOMIZE_FOOD_LEVEL = true;
   init(false, false);
   openBehaviorFile((char *)BehaviorWorkFilename.c_str());
   int  eggLaidStep = -1;
   char buf[BUFSIZ];
   if (verbose)
   {
      printf("Generating behavior file %s\n", (char *)BehaviorWorkFilename.c_str());
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
      if (verbose)
      {
         printf("Step=%d\n", i);
      }
      sprintf(buf, "{ \"Step\": %d, \"Data\": {\n", i);
      writeBehaviorFile(buf);
      step();
      writeBehaviorFile((char *)"} }");
      if (i < steps)
      {
         writeBehaviorFile((char *)",");
      }
      writeBehaviorFile((char *)"\n");
      if ((eggLaidStep < 0) && (World[NEST_CENTER_X][NEST_CENTER_Y].object == OBJECT::EGG))
      {
         eggLaidStep = i;
      }
   }
   if (verbose)
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
   vector<MaleSensoryResponse>& maleSequence,
   vector<FemaleSensoryResponse>& femaleSequence,
   bool discrimination, bool verbose)
{
   if (verbose)
   {
      printf("Importing behavior dataset\n");
   }
   maleSequence.clear();
   femaleSequence.clear();
   ifstream file;
   file.open((char *)BehaviorWorkFilename.c_str());
   if (!file.is_open())
   {
      fprintf(stderr, "Cannot open behavior file %s\n", (char *)BehaviorWorkFilename.c_str());
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
         if (verbose)
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
         bool                flying = false;
         int                 end    = json.find("\"");
         int                 i      = 0;
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
                  flying = true;
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
         if (!discrimination || !flying)
         {
            if (verbose)
            {
               printf("Male data:\n");
               sensoryResponse.print();
            }
            maleSequence.push_back(sensoryResponse);
         }
         continue;
      }
   }
   file.close();
   unlink((char *)BehaviorWorkFilename.c_str());
}


// Convert behavior to datasets.
void convertBehaviorToDatasets(int steps, string maleDatasetName, string femaleDatasetName)
{
   // Create male dataset.
   FILE *fp = fopen(maleDatasetName.c_str(), "w");

   if (fp == NULL)
   {
      fprintf(stderr, "Cannot open male dataset file %s\n", maleDatasetName.c_str());
      exit(1);
   }
   fprintf(fp, "X_train_shape = [%d, %d, %d]\n", (int)MaleTrainBehavior.size(), (int)MaleTrainBehavior[0].size(), MaleSensoryResponse::oneHotSensoryLength());
   fprintf(fp, "X_train_seq = [\n");
   for (int i = 0; i < MaleTrainBehavior.size(); i++)
   {
      vector<MaleSensoryResponse> behaviorSequence = MaleTrainBehavior[i];
      for (int j = 0; j < behaviorSequence.size(); j++)
      {
         MaleSensoryResponse sensoryResponse = behaviorSequence[j];
         fprintf(fp, "%s", sensoryResponse.sensoryToOneHot().c_str());
         if ((i < MaleTrainBehavior.size() - 1) || (j < behaviorSequence.size() - 1))
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   fprintf(fp, "y_train_shape = [%d, %d, %d]\n", (int)MaleTrainBehavior.size(), (int)MaleTrainBehavior[0].size(), MaleSensoryResponse::oneHotResponseLength());
   fprintf(fp, "y_train_seq = [\n");
   for (int i = 0; i < MaleTrainBehavior.size(); i++)
   {
      vector<MaleSensoryResponse> behaviorSequence = MaleTrainBehavior[i];
      for (int j = 0; j < behaviorSequence.size(); j++)
      {
         MaleSensoryResponse sensoryResponse = behaviorSequence[j];
         fprintf(fp, "%s", sensoryResponse.responseToOneHot().c_str());
         if ((i < MaleTrainBehavior.size() - 1) || (j < behaviorSequence.size() - 1))
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   if (MaleTestBehavior.size() == 0)
   {
      fprintf(fp, "X_test_shape = [0, %d, %d]\n", steps, MaleSensoryResponse::oneHotSensoryLength());
      fprintf(fp, "X_test_seq = [\n");
   }
   else
   {
      fprintf(fp, "X_test_shape = [1, %d, %d]\n", (int)MaleTestBehavior.size(), (int)MaleSensoryResponse::oneHotSensoryLength());
      fprintf(fp, "X_test_seq = [\n");
      for (int i = 0; i < MaleTestBehavior.size(); i++)
      {
         MaleSensoryResponse sensoryResponse = MaleTestBehavior[i];
         fprintf(fp, "%s", sensoryResponse.sensoryToOneHot().c_str());
         if (i < MaleTestBehavior.size() - 1)
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   if (MaleTestBehavior.size() == 0)
   {
      fprintf(fp, "y_test_shape = [0, %d, %d]\n", steps, MaleSensoryResponse::oneHotResponseLength());
      fprintf(fp, "y_test_seq = [\n");
   }
   else
   {
      fprintf(fp, "y_test_shape = [1, %d, %d]\n", (int)MaleTestBehavior.size(), (int)MaleSensoryResponse::oneHotResponseLength());
      fprintf(fp, "y_test_seq = [\n");
      for (int i = 0; i < MaleTestBehavior.size(); i++)
      {
         MaleSensoryResponse sensoryResponse = MaleTestBehavior[i];
         fprintf(fp, "%s", sensoryResponse.responseToOneHot().c_str());
         if (i < MaleTestBehavior.size() - 1)
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   fclose(fp);

   // Create female dataset.
   fp = fopen(femaleDatasetName.c_str(), "w");
   if (fp == NULL)
   {
      fprintf(stderr, "Cannot open female dataset file %s\n", femaleDatasetName.c_str());
      exit(1);
   }
   fprintf(fp, "X_train_shape = [%d, %d, %d]\n", (int)FemaleTrainBehavior.size(), (int)FemaleTrainBehavior[0].size(), FemaleSensoryResponse::oneHotSensoryLength());
   fprintf(fp, "X_train_seq = [\n");
   for (int i = 0; i < FemaleTrainBehavior.size(); i++)
   {
      vector<FemaleSensoryResponse> behaviorSequence = FemaleTrainBehavior[i];
      for (int j = 0; j < behaviorSequence.size(); j++)
      {
         FemaleSensoryResponse sensoryResponse = behaviorSequence[j];
         fprintf(fp, "%s", sensoryResponse.sensoryToOneHot().c_str());
         if ((i < FemaleTrainBehavior.size() - 1) || (j < behaviorSequence.size() - 1))
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   fprintf(fp, "y_train_shape = [%d, %d, %d]\n", (int)FemaleTrainBehavior.size(), (int)FemaleTrainBehavior[0].size(), FemaleSensoryResponse::oneHotResponseLength());
   fprintf(fp, "y_train_seq = [\n");
   for (int i = 0; i < FemaleTrainBehavior.size(); i++)
   {
      vector<FemaleSensoryResponse> behaviorSequence = FemaleTrainBehavior[i];
      for (int j = 0; j < behaviorSequence.size(); j++)
      {
         FemaleSensoryResponse sensoryResponse = behaviorSequence[j];
         fprintf(fp, "%s", sensoryResponse.responseToOneHot().c_str());
         if ((i < FemaleTrainBehavior.size() - 1) || (j < behaviorSequence.size() - 1))
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   if (FemaleTestBehavior.size() == 0)
   {
      fprintf(fp, "X_test_shape = [0, %d, %d]\n", steps, FemaleSensoryResponse::oneHotSensoryLength());
      fprintf(fp, "X_test_seq = [\n");
   }
   else
   {
      fprintf(fp, "X_test_shape = [1, %d, %d]\n", (int)FemaleTestBehavior.size(), (int)FemaleSensoryResponse::oneHotSensoryLength());
      fprintf(fp, "X_test_seq = [\n");
      for (int i = 0; i < FemaleTestBehavior.size(); i++)
      {
         FemaleSensoryResponse sensoryResponse = FemaleTestBehavior[i];
         fprintf(fp, "%s", sensoryResponse.sensoryToOneHot().c_str());
         if (i < FemaleTestBehavior.size() - 1)
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   if (FemaleTestBehavior.size() == 0)
   {
      fprintf(fp, "y_test_shape = [0, %d, %d]\n", steps, FemaleSensoryResponse::oneHotResponseLength());
      fprintf(fp, "y_test_seq = [\n");
   }
   else
   {
      fprintf(fp, "y_test_shape = [1, %d, %d]\n", (int)FemaleTestBehavior.size(), (int)FemaleSensoryResponse::oneHotResponseLength());
      fprintf(fp, "y_test_seq = [\n");
      for (int i = 0; i < FemaleTestBehavior.size(); i++)
      {
         FemaleSensoryResponse sensoryResponse = FemaleTestBehavior[i];
         fprintf(fp, "%s", sensoryResponse.responseToOneHot().c_str());
         if (i < FemaleTestBehavior.size() - 1)
         {
            fprintf(fp, ",");
         }
         fprintf(fp, "\n");
      }
   }
   fprintf(fp, "]\n");
   fclose(fp);
}


// Convert behavior to pattern datasets.
void convertBehaviorToPatternDatasets(int steps, string maleDatasetName, string femaleDatasetName)
{
   // Create male dataset.
   FILE *fp = fopen(maleDatasetName.c_str(), "w");

   if (fp == NULL)
   {
      fprintf(stderr, "Cannot open male dataset file %s\n", maleDatasetName.c_str());
      exit(1);
   }
   for (int i = 0; i < MaleTrainBehavior.size(); i++)
   {
      vector<MaleSensoryResponse> behaviorSequence = MaleTrainBehavior[i];
      for (int j = 0; j < behaviorSequence.size(); j++)
      {
         MaleSensoryResponse sensoryResponse = behaviorSequence[j];
         fprintf(fp, "%s,", sensoryResponse.sensoryToOneHot().c_str());
         fprintf(fp, "%d\n", sensoryResponse.responseToNum());
      }
   }
   fclose(fp);

   // Create female dataset.
   fp = fopen(femaleDatasetName.c_str(), "w");
   if (fp == NULL)
   {
      fprintf(stderr, "Cannot open female dataset file %s\n", femaleDatasetName.c_str());
      exit(1);
   }
   for (int i = 0; i < FemaleTrainBehavior.size(); i++)
   {
      vector<FemaleSensoryResponse> behaviorSequence = FemaleTrainBehavior[i];
      for (int j = 0; j < behaviorSequence.size(); j++)
      {
         FemaleSensoryResponse sensoryResponse = behaviorSequence[j];
         fprintf(fp, "%s,", sensoryResponse.sensoryToOneHot().c_str());
         fprintf(fp, "%d\n", sensoryResponse.responseToNum());
      }
   }
   fclose(fp);
}


// Import male sensors.
void importMaleSensors(string maleSensorFilename, vector<MaleSensoryResponse>& sensors)
{
   ifstream file;

   file.open(maleSensorFilename);
   if (!file.is_open())
   {
      fprintf(stderr, "Cannot open male sensor file %s\n", maleSensorFilename.c_str());
      exit(1);
   }
   sensors.clear();
   string encoding;
   while (getline(file, encoding))
   {
      encoding.erase(std::remove(encoding.begin(), encoding.end(), '\n'), encoding.end());
      encoding.erase(std::remove(encoding.begin(), encoding.end(), '\r'), encoding.end());
      MaleSensoryResponse sensoryResponse;
      sensoryResponse.oneHotToSensory(encoding);
      if (Verbose)
      {
         printf("encoding: %s\n", encoding.c_str());
         printf("sensory:\n");
         sensoryResponse.print();
      }
      sensors.push_back(sensoryResponse);
   }
   file.close();
}


// Import female sensors.
void importFemaleSensors(string femaleSensorFilename, vector<FemaleSensoryResponse>& sensors)
{
   ifstream file;

   file.open(femaleSensorFilename);
   if (!file.is_open())
   {
      fprintf(stderr, "Cannot open female sensor file %s\n", femaleSensorFilename.c_str());
      exit(1);
   }
   sensors.clear();
   string encoding;
   while (getline(file, encoding))
   {
      encoding.erase(std::remove(encoding.begin(), encoding.end(), '\n'), encoding.end());
      encoding.erase(std::remove(encoding.begin(), encoding.end(), '\r'), encoding.end());
      FemaleSensoryResponse sensoryResponse;
      sensoryResponse.oneHotToSensory(encoding);
      if (Verbose)
      {
         printf("encoding: %s\n", encoding.c_str());
         printf("sensory:\n");
         sensoryResponse.print();
      }
      sensors.push_back(sensoryResponse);
   }
   file.close();
}


// Install male sensors.
void installMaleSensors(vector<MaleSensoryResponse>& sensors)
{
   int sInt[Male::NUM_SENSORS];

   vector<Mona::SENSOR> sFloat;
   vector<bool>         mask;
   Mona::SENSOR         distance;
   for (MaleSensoryResponse sensoryResponse : sensors)
   {
      sensoryResponse.toSensors(sInt);
      mask.clear();
      sFloat.clear();
      for (int i = 0; i < Male::NUM_SENSORS; i++)
      {
         if (sInt[i] == DONT_CARE)
         {
            mask.push_back(false);
         }
         else
         {
            mask.push_back(true);
         }
         sFloat.push_back((Mona::SENSOR)sInt[i]);
      }
      int            index     = male->brain->addSensorDiscriminator(mask);
      Mona::Receptor *receptor = male->brain->getSensorDiscriminatorReceptor(sFloat, index, distance);
      if ((receptor == NULL) || ((receptor != NULL) && (distance > male->brain->sensorDiscriminators[index]->resolution)))
      {
         male->newSensorDiscriminatorReceptor(sInt);
      }
   }
}


// Install female sensors.
void installFemaleSensors(vector<FemaleSensoryResponse>& sensors)
{
   int sInt[Female::NUM_SENSORS];

   vector<Mona::SENSOR> sFloat;
   vector<bool>         mask;
   Mona::SENSOR         distance;
   for (FemaleSensoryResponse sensoryResponse : sensors)
   {
      sensoryResponse.toSensors(sInt);
      mask.clear();
      sFloat.clear();
      for (int i = 0; i < Female::NUM_SENSORS; i++)
      {
         if (sInt[i] == DONT_CARE)
         {
            mask.push_back(false);
         }
         else
         {
            mask.push_back(true);
         }
         sFloat.push_back((Mona::SENSOR)sInt[i]);
      }
      int            index     = female->brain->addSensorDiscriminator(mask);
      Mona::Receptor *receptor = female->brain->getSensorDiscriminatorReceptor(sFloat, index, distance);
      if ((receptor == NULL) || ((receptor != NULL) && (distance > female->brain->sensorDiscriminators[index]->resolution)))
      {
         female->newSensorDiscriminatorReceptor(sInt);
      }
   }
}
