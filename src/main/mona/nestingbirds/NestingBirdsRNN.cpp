// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds RNN.

#include "male.hpp"
#include "female.hpp"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Random numbers.
#define DEFAULT_RANDOM_NUMBER_SEED    "4517"
int RANDOM_NUMBER_SEED = 4517;
int TEST_RANDOM_NUMBER_SEED = -1;

// Usage.
const char* Usage =
"Usage:\n"
"    nestingbirds_rnn\n"
"      -behaviorTrainFiles <behavior files> (comma-separated list)\n"
"      [-testRandomSeed <random seed>]\n"
"Exit codes:\n"
"  0=success\n"
"  1=error\n";

// Train file names.
vector<string> BehaviorTrainFilenames;

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
void importBehaviorFileDataset(string filename, 
    vector<MaleSensoryResponse> &MaleDataset, vector<FemaleSensoryResponse>& FemaleDataset);

// Trim string.
string trim(string& str);

// Main.
int main(int argc, char *args[])
{
   for (int i = 1; i < argc; i++)
   {
      if (strcmp(args[i], "-behaviorTrainFiles") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid behaviorTrainFiles option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         stringstream s_stream(args[i]);
         while (s_stream.good()) 
         {
             string substr;
             getline(s_stream, substr, ',');
             BehaviorTrainFilenames.push_back(trim(substr));
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
         TEST_RANDOM_NUMBER_SEED = atoi(args[i]);
         if (TEST_RANDOM_NUMBER_SEED < 0)
         {
             fprintf(stderr, "Invalid testRandomSeed option\n");
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
   if (BehaviorTrainFilenames.size() == 0)
   {
       fprintf(stderr, Usage);
       exit(1);
   }

   // Import behavior datasets.
   for (string filename : BehaviorTrainFilenames)
   {
       vector<MaleSensoryResponse> maleDataset;
       vector<FemaleSensoryResponse> femaleDataset;
       importBehaviorFileDataset(filename, maleDataset, femaleDataset);
       MaleTrainDataset.push_back(maleDataset);
       FemaleTrainDataset.push_back(femaleDataset);
   }

   exit(0);
}

// Import behavior file dataset.
void importBehaviorFileDataset(string filename, 
    vector<MaleSensoryResponse>& maleDataset, vector<FemaleSensoryResponse>& femaleDataset)
{
    maleDataset.clear();
    femaleDataset.clear();
    ifstream file;
    file.open(filename);
    if (!file.is_open())
    {
        fprintf(stderr, "Cannot open behavior file %s\n", filename);
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
            femaleDataset.push_back(sensoryResponse);
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
            maleDataset.push_back(sensoryResponse);
            continue;
        }
    }
    file.close();
}

// Trim string.
string trim(string& str)
{
    str.erase(str.find_last_not_of(' ') + 1);
    str.erase(0, str.find_first_not_of(' '));
    return str;
}

