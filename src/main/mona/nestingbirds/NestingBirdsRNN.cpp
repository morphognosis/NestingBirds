// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds RNN.

#include "male.hpp"
#include "female.hpp"
#include <stdlib.h>
#include<iostream>
#include<vector>
#include<sstream>
using namespace std;

// Random numbers.
#define DEFAULT_RANDOM_NUMBER_SEED    "4517"
int RANDOM_NUMBER_SEED = 4517;

// Usage.
const char* Usage =
"Usage:\n"
"    nestingbirds_rnn\n"
"      -behaviorTrainFiles <behavior files> (comma-separated list)\n"
"      [-behaviorTestFiles <behavior files> (comma-separated list)]\n"
"Exit codes:\n"
"  0=success\n"
"  1=error\n";

// Train/test file names.
vector<string> BehaviorTrainFilenames;
vector<string> BehaviorTestFilenames;

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
      if (strcmp(args[i], "-behaviorTestFiles") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid behaviorTestFiles file\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         stringstream s_stream(args[i]);
         while (s_stream.good())
         {
             string substr;
             getline(s_stream, substr, ',');
             BehaviorTestFilenames.push_back(trim(substr));
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
   
   exit(0);
}

// Trim string.
string trim(string& str)
{
    str.erase(str.find_last_not_of(' ') + 1);
    str.erase(0, str.find_first_not_of(' '));
    return str;
}

