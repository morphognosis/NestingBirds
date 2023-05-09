// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds world.

#include "male.hpp"
#include "female.hpp"

// Random numbers.
#define DEFAULT_RANDOM_NUMBER_SEED    "4517"
extern int RANDOM_NUMBER_SEED;

// Probability of mouse movement.
#define DEFAULT_MOUSE_MOVE_PROBABILITY    "0.1"
extern double MOUSE_MOVE_PROBABILITY;

// Cell.
class Cell
{
public:
   int locale;
   int object;
   Cell()
   {
      locale = LOCALE::DESERT;
      object = OBJECT::NO_OBJECT;
   }
};

// World.
#define WIDTH     21
#define HEIGHT    21
extern Cell World[WIDTH][HEIGHT];
extern int  NEST_CENTER_X;
extern int  NEST_CENTER_Y;

// Birds.
extern Male   *male;
extern Female *female;

// Initialize.
void init(bool maleTest, bool femaleTest);

// Step.
void step();

// Save/load.
void saveMale(char *filename);
void loadMale(char *filename);
void saveFemale(char *filename);
void loadFemale(char *filename);

// Behavior.
void openBehaviorFile(char *filename);
void writeBehaviorFile(char *text);
void closeBehaviorFile();

// Verbosity.
extern bool Verbose;
