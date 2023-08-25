// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds.

#include "world.hpp"
#include "datasets.hpp"
#include <stdlib.h>

// Version.
const char *VERSION = "2.0";

// Usage.
const char* Usage =
"Usage:\n"
"    nestingbirds\n"
"      -trainMale | -testMale\n"
"      -trainFemale | -testFemale\n"
"      -steps <steps>\n"
"      [-saveMaleFile <save file name>]\n"
"      [-loadMaleFile <load file name>]\n"
"      [-saveFemaleFile <save file name>]\n"
"      [-loadFemaleFile <load file name>]\n"
"      [-maleInitialFood <amount> (default=" MALE_DEFAULT_INITIAL_FOOD "]\n"
"      [-maleFoodDuration <amount> (default=" MALE_DEFAULT_FOOD_DURATION ")]\n"
"      [-maleRandomizeFoodLevel (food level probabilistically increases 0-" MALE_DEFAULT_FOOD_DURATION " upon eating mouse)]\n"
"      [-maleMouseNeed <amount> (default=" MALE_DEFAULT_MOUSE_NEED ")]\n"
"      [-maleFemaleMouseNeed <amount> (default=" MALE_DEFAULT_FEMALE_MOUSE_NEED ")]\n"
"      [-maleFemaleStoneNeed <amount> (default=" MALE_DEFAULT_FEMALE_STONE_NEED ")]\n"
"      [-maleAttendFemaleNeed <amount> (default=" MALE_DEFAULT_ATTEND_FEMALE_NEED ")]\n"
"      [-femaleInitialFood <amount> (default=" FEMALE_DEFAULT_INITIAL_FOOD ")]\n"
"      [-femaleFoodDuration <amount> (default=" FEMALE_DEFAULT_FOOD_DURATION ")]\n"
"      [-femaleRandomizeFoodLevel (food level probabilistically increases 0-" FEMALE_DEFAULT_FOOD_DURATION " upon eating mouse)]\n"
"      [-femaleMouseNeed <amount> (default=" FEMALE_DEFAULT_MOUSE_NEED ")]\n"
"      [-femaleLayEggNeed <amount> (default=" FEMALE_DEFAULT_LAY_EGG_NEED ")]\n"
"      [-femaleBroodEggNeed <amount> (default=" FEMALE_DEFAULT_BROOD_EGG_NEED ")]\n"
"      [-randomSeed <seed> (default=" DEFAULT_RANDOM_NUMBER_SEED ")]\n"
"      [-mouseMoveProbability <probability> (default=" DEFAULT_MOUSE_MOVE_PROBABILITY ")]\n"
"      [-discriminateSensors (discriminate sensors by detecting features that are important to responses)]\n"
"      [-writeBehaviorFile <behavior file name> (json)]\n"
"      [-verbose <true | false> (default=true)]\n"
"      [-version]\n"
"Exit codes:\n"
"  0=success\n"
"  1=error\n";

// Steps.
int Steps;

// Save/load file names.
char *MaleSaveFilename;
char *MaleLoadFilename;
char *FemaleSaveFilename;
char *FemaleLoadFilename;

// Main.
int main(int argc, char *args[])
{
   bool maleTest               = false;
   bool femaleTest             = false;
   bool gotMaleTrainTest       = false;
   bool gotFemaleTrainTest     = false;
   bool gotSteps               = false;
   bool gotDiscriminateSensors = false;
   char *behaviorFilename      = NULL;
   char buf[BUFSIZ];

   for (int i = 1; i < argc; i++)
   {
      if (strcmp(args[i], "-trainMale") == 0)
      {
         maleTest = false;
         if (gotMaleTrainTest)
         {
            fprintf(stderr, "Duplicate male train/test option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         gotMaleTrainTest = true;
         continue;
      }
      if (strcmp(args[i], "-testMale") == 0)
      {
         maleTest = true;
         if (gotMaleTrainTest)
         {
            fprintf(stderr, "Duplicate male train/test option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         gotMaleTrainTest = true;
         continue;
      }
      if (strcmp(args[i], "-trainFemale") == 0)
      {
         femaleTest = false;
         if (gotFemaleTrainTest)
         {
            fprintf(stderr, "Duplicate female train/test option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         gotFemaleTrainTest = true;
         continue;
      }
      if (strcmp(args[i], "-testFemale") == 0)
      {
         femaleTest = true;
         if (gotFemaleTrainTest)
         {
            fprintf(stderr, "Duplicate female train/test option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         gotFemaleTrainTest = true;
         continue;
      }
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
      if (strcmp(args[i], "-saveMaleFile") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid male save file\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         MaleSaveFilename = args[i];
         continue;
      }
      if (strcmp(args[i], "-loadMaleFile") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid male load file\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         MaleLoadFilename = args[i];
         continue;
      }
      if (strcmp(args[i], "-saveFemaleFile") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid female save file\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         FemaleSaveFilename = args[i];
         continue;
      }
      if (strcmp(args[i], "-loadFemaleFile") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid female load file\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         FemaleLoadFilename = args[i];
         continue;
      }
      if (strcmp(args[i], "-maleInitialFood") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleInitialFood option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::INITIAL_FOOD = atoi(args[i]);
         if (Male::INITIAL_FOOD < 0)
         {
            fprintf(stderr, "Invalid maleInitialFood option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-maleFoodDuration") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleFoodDuration option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::FOOD_DURATION = atoi(args[i]);
         if (Male::FOOD_DURATION < 0)
         {
            fprintf(stderr, "Invalid maleFoodDuration option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-maleRandomizeFoodLevel") == 0)
      {
         Male::RANDOMIZE_FOOD_LEVEL = true;
         continue;
      }
      if (strcmp(args[i], "-maleMouseNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleMouseNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::MOUSE_NEED = strtod(args[i], 0);
         if (Male::MOUSE_NEED < 0.0)
         {
            fprintf(stderr, "Invalid maleMouseNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-maleFemaleMouseNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleFemaleMouseNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::FEMALE_MOUSE_NEED = strtod(args[i], 0);
         if (Male::FEMALE_MOUSE_NEED < 0.0)
         {
            fprintf(stderr, "Invalid maleFemaleMouseNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-maleAttendFemaleNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleAttendFemaleNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::ATTEND_FEMALE_NEED = strtod(args[i], 0);
         if (Male::ATTEND_FEMALE_NEED < 0.0)
         {
            fprintf(stderr, "Invalid maleAttendFemaleNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-maleFemaleStoneNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid maleFemaleStoneNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Male::FEMALE_STONE_NEED = strtod(args[i], 0);
         if (Male::FEMALE_STONE_NEED < 0.0)
         {
            fprintf(stderr, "Invalid maleFemaleStoneNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-femaleInitialFood") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid femaleInitialFood option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Female::INITIAL_FOOD = atoi(args[i]);
         if (Female::INITIAL_FOOD < 0)
         {
            fprintf(stderr, "Invalid femaleInitialFood option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-femaleFoodDuration") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid femaleFoodDuration option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Female::FOOD_DURATION = atoi(args[i]);
         if (Female::FOOD_DURATION < 0)
         {
            fprintf(stderr, "Invalid femaleFoodDuration option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-femaleRandomizeFoodLevel") == 0)
      {
         Female::RANDOMIZE_FOOD_LEVEL = true;
         continue;
      }
      if (strcmp(args[i], "-femaleMouseNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid femaleMouseNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Female::MOUSE_NEED = 1.0;
         Female::MOUSE_NEED = strtod(args[i], 0);
         if (Female::MOUSE_NEED < 0.0)
         {
            fprintf(stderr, "Invalid femaleMouseNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-femaleLayEggNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid femaleLayEggNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Female::LAY_EGG_NEED = strtod(args[i], 0);
         if (Female::LAY_EGG_NEED < 0.0)
         {
            fprintf(stderr, "Invalid femaleLayEggNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-femaleBroodEggNeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid femaleBroodEggNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         Female::BROOD_EGG_NEED = strtod(args[i], 0);
         if (Female::BROOD_EGG_NEED < 0.0)
         {
            fprintf(stderr, "Invalid femaleBroodEggNeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-randomSeed") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid randomSeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         RANDOM_NUMBER_SEED = atoi(args[i]);
         if (RANDOM_NUMBER_SEED <= 0)
         {
            fprintf(stderr, "Invalid randomSeed option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         continue;
      }
      if (strcmp(args[i], "-mouseMoveProbability") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid mouseMoveProbability option\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         MOUSE_MOVE_PROBABILITY = atof(args[i]);
         if ((MOUSE_MOVE_PROBABILITY < 0.0) || (MOUSE_MOVE_PROBABILITY > 1.0))
         {
            fprintf(stderr, "Invalid mouseMoveProbability option\n");
            fprintf(stderr, Usage);
            exit(1);
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
      if (strcmp(args[i], "-discriminateSensors") == 0)
      {
         gotDiscriminateSensors = true;
         continue;
      }
      if (strcmp(args[i], "-writeBehaviorFile") == 0)
      {
         i++;
         if (i >= argc)
         {
            fprintf(stderr, "Invalid behavior file\n");
            fprintf(stderr, Usage);
            exit(1);
         }
         behaviorFilename = args[i];
         continue;
      }
      if (strcmp(args[i], "-version") == 0)
      {
         printf("Nesting birds version %s\n", VERSION);
         Mona::printVersion(stdout);
         exit(0);
      }
      if ((strcmp(args[i], "-help") == 0) || (strcmp(args[i], "-h") == 0) || (strcmp(args[i], "-?") == 0))
      {
         printf(Usage);
         exit(0);
      }
      fprintf(stderr, Usage);
      exit(1);
   }
   if (!gotMaleTrainTest)
   {
      fprintf(stderr, Usage);
      exit(1);
   }
   if (!gotFemaleTrainTest)
   {
      fprintf(stderr, Usage);
      exit(1);
   }
   if (!gotSteps)
   {
      fprintf(stderr, Usage);
      exit(1);
   }
   if (Male::INITIAL_FOOD > Male::FOOD_DURATION)
   {
      fprintf(stderr, "Male initial food cannot be greater than food duration\n");
      fprintf(stderr, Usage);
      exit(1);
   }
   if (Female::INITIAL_FOOD > Female::FOOD_DURATION)
   {
      fprintf(stderr, "Female initial food cannot be greater than food duration");
      fprintf(stderr, Usage);
      exit(1);
   }

   // Initialize.
   init(maleTest, femaleTest);

   // Load?
   if (MaleLoadFilename != NULL)
   {
      loadMale(MaleLoadFilename);
   }
   if (FemaleLoadFilename != NULL)
   {
      loadFemale(FemaleLoadFilename);
   }

   // Discriminate sensors?
   if (gotDiscriminateSensors)
   {
      discriminateSensors(Steps, maleTest, femaleTest);
   }

   // Write behavior file?
   if (behaviorFilename != NULL)
   {
      openBehaviorFile(behaviorFilename);
   }

   // Run birds.
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
   for (int i = 1; i <= Steps; i++)
   {
      if (Verbose)
      {
         printf("Step=%d\n", i);
      }

      if (behaviorFilename != NULL)
      {
         sprintf(buf, "{ \"Step\": %d, \"Data\": {\n", i);
         writeBehaviorFile(buf);
      }

      step();

      if (behaviorFilename != NULL)
      {
         writeBehaviorFile((char *)"} }");
         if (i < Steps)
         {
            writeBehaviorFile((char *)",");
         }
         writeBehaviorFile((char *)"\n");
      }

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

   // Save?
   if (MaleSaveFilename != NULL)
   {
      saveMale(MaleSaveFilename);
   }
   if (FemaleSaveFilename != NULL)
   {
      saveFemale(FemaleSaveFilename);
   }

   // Write behavior file?
   if (behaviorFilename != NULL)
   {
      closeBehaviorFile();
   }

   exit(0);
}
