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
"      [-testRandomSeeds <random seeds>]\n"
"      [-verbose <true | false> (default=true)]\n"
"Exit codes:\n"
"  0=success\n"
"  1=error\n";

// Steps.
int Steps;

// Random numbers.
vector<int> trainRandomSeeds;
vector<int> testRandomSeeds;

// File names.
#define BEHAVIOR_FILENAME (char *)"nestingbirds_rnn_behavior.json"
#define RNN_MALE_DATASET_FILENAME (char *)"nestingbirds_rnn_male_dataset.csv"
#define RNN_FEMALE_DATASET_FILENAME (char *)"nestingbirds_rnn_female_dataset.csv"
#define RNN_FILENAME (char *)"nestingbirds_rnn.py"

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

    // Convert to one-hot encoding.
    string oneHot()
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
        else {
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
        else {
            encoding += "0,0,0,0,1";
        }
        return encoding;
    }

    // Get length of encoding.
    int oneHotLength()
    {
        return 0;
    }
};

class FemaleSensoryResponse
{
public:
    vector<string> cellSensors;
    string orientation;
    string goal;
    string hasObject;
    string response;

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
};

// Imported behavior datasets.
vector<vector<MaleSensoryResponse>> MaleTrainDataset;
vector<vector<MaleSensoryResponse>> MaleTestDataset;
vector<vector<FemaleSensoryResponse>> FemaleTrainDataset;
vector<vector<FemaleSensoryResponse>> FemaleTestDataset;
void createBehaviorFileSequences(int randomSeed, int steps, bool maleTest, bool femaleTest, 
    vector<MaleSensoryResponse> &maleSequence, vector<FemaleSensoryResponse>& femaleSequence);
void createRNNdataset();

// Trim string.
string trim(string& str);

// Main.
int main(int argc, char *args[])
{
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
             trainRandomSeeds.push_back(atoi(trim(substr).c_str()));
         }
         continue;
      }
      if (strcmp(args[i], "-testRandomSeeds") == 0)
      {
          i++;
          if (i >= argc)
          {
              fprintf(stderr, "Invalid testRandomSeeds option\n");
              fprintf(stderr, Usage);
              exit(1);
          }
          stringstream s_stream(args[i]);
          while (s_stream.good())
          {
              string substr;
              getline(s_stream, substr, ',');
              testRandomSeeds.push_back(atoi(trim(substr).c_str()));
          }
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
   if (trainRandomSeeds.size() == 0)
   {
       fprintf(stderr, Usage);
       exit(1);
   }

   // Create behavior training datasets.
   for (int seed : trainRandomSeeds)
   {
       vector<MaleSensoryResponse> maleSequence;
       vector<FemaleSensoryResponse> femaleSequence;
       createBehaviorFileSequences(seed, Steps, false, false, maleSequence, femaleSequence);
       MaleTrainDataset.push_back(maleSequence);
       FemaleTrainDataset.push_back(femaleSequence);
   }

   // Convert to RNN dataset.
   createRNNdataset();

   // Train RNN.

   exit(0);
}

// Create behavior sequences.
void createBehaviorFileSequences(int randomSeed, int steps, bool maleTest, bool femaleTest, 
    vector<MaleSensoryResponse>& maleSequence, vector<FemaleSensoryResponse>& femaleSequence)
{
    // Generate behavior file.
    RANDOM_NUMBER_SEED = randomSeed;
    Male::RANDOMIZE_FOOD_LEVEL = true;
    Female::RANDOMIZE_FOOD_LEVEL = true;
    init(maleTest, femaleTest);
    openBehaviorFile(BEHAVIOR_FILENAME);
    int eggLaidStep = -1;
    char buf[100];
    if (Verbose)
    {
        printf("Generating behavior file %s\n", BEHAVIOR_FILENAME);
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
        writeBehaviorFile((char*)"} }");
        if (i < Steps)
        {
            writeBehaviorFile((char*)",");
        }
        writeBehaviorFile((char*)"\n");
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
        printf(", remaining mice=%d, remaining stones=%d\n", mouseCount, stoneCount);
    }
    closeBehaviorFile();

    // Import behavior dataset.
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
            int i = 0;
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
                printf("Female data:\n"); sensoryResponse.print();
            }
            femaleSequence.push_back(sensoryResponse);
            continue;
        }
        if (gender == "Ma")
        {
            MaleSensoryResponse sensoryResponse;
            int end = json.find("\"");
            int i = 0;
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
                    else {
                        sensoryResponse.flying = "false";
                    }
                    break;
                case 38:
                    if (json.substr(0, end).find("true") != string::npos)
                    {
                        sensoryResponse.femaleWantsMouse = "true";
                    }
                    else {
                        sensoryResponse.femaleWantsMouse = "false";
                    }
                    break;
                case 40:
                    if (json.substr(0, end).find("true") != string::npos)
                    {
                        sensoryResponse.femaleWantsStone = "true";
                    }
                    else {
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
                printf("Male data:\n"); sensoryResponse.print();
            }
            maleSequence.push_back(sensoryResponse);
            continue;
        }
    }
    file.close();
    unlink(BEHAVIOR_FILENAME);
}

// Convert to RNN dataset.
void createRNNdataset()
{
    ofstream file;
    FILE *fp = fopen(RNN_MALE_DATASET_FILENAME, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Cannot open male dataset file %s\n", RNN_MALE_DATASET_FILENAME);
        exit(1);
    }
    for (int i = 0; i < MaleTrainDataset.size(); i++)
    {
        vector<MaleSensoryResponse> behaviorSequence = MaleTrainDataset[i];
        for (int j = 0; j < behaviorSequence.size(); j++)
        {
            MaleSensoryResponse sensoryResponse = behaviorSequence[j];
            fprintf(fp, "%s", sensoryResponse.oneHot().c_str());
            if (i < MaleTrainDataset.size() - 1 || j < behaviorSequence.size() - 1)
            {
                fprintf(fp, ",");
            }
            fprintf(fp, "\n");
        }
    }
    fclose(fp);
}

// Trim string.
string trim(string& str)
{
    str.erase(str.find_last_not_of(' ') + 1);
    str.erase(0, str.find_first_not_of(' '));
    return str;
}

