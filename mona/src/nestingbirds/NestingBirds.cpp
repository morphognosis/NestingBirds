// For conditions of distribution and use, see copyright notice in LICENSE.txt

// The nesting birds.

#include "male.hpp"
#include "female.hpp"

// Version.
const char* VERSION = "2.0";

// Random numbers.
int    RANDOM_NUMBER_SEED = 4517;

// Usage.
#define STRING(X) #X
const char* Usage =
"Usage:\n"
"    nestingbirds\n"
"      -train -save <save file name>] | -test -load <load file name>\n"
"      [-maleInitialFood <amount> (default=" STRING(MALE_DEFAULT_INITIAL_FOOD) ")]\n"
"      [-maleFoodDuration <amount> (default=" STRING(MALE_DEFAULT_FOOD_DURATION) ")]\n"
"      [-maleRandomizeFoodLevel (food level probabilistically increases 0-" STRING(MALE_DEFAULT_FOOD_DURATION) " upon eating food)]\n"
"      [-maleMouseNeed <amount> (default=" STRING(MALE_DEFAULT_MOUSE_NEED) ")]\n"
"      [-maleFemaleMouseNeed <amount> (default=" STRING(MALE_DEFAULT_Female_MOUSE_NEED) ")]\n"
"      [-maleStoneNeed <amount> (default=" STRING(MALE_DEFAULT_STONE_NEED) ")]\n"
"      [-maleFemaleStoneNeed <amount> (default=" STRING(MALE_DEFAULT_Female_STONE_NEED) ")]\n"
"      [-FemaleInitialFood <amount> (default=" STRING(Female_DEFAULT_INITIAL_FOOD) ")]\n"
"      [-FemaleFoodDuration <amount> (default=" STRING(Female_DEFAULT_FOOD_DURATION) ")]\n"
"      [-FemaleRandomizeFoodLevel (food level probabilistically increases 0-" STRING(Female_DEFAULT_FOOD_DURATION) " upon eating food)]\n"
"      [-FemaleMouseNeed <amount> (default=" STRING(Female_DEFAULT_MOUSE_NEED) ")]\n"
"      [-FemaleStoneNeed <amount> (default=" STRING(Female_DEFAULT_STONE_NEED) ")]\n"
"      [-FemaleEggNeed <amount> (default=" STRING(Female_DEFAULT_EGG_NEED) ")]\n"
"      [-verbose <true | false> (default=true)]\n"
"      [-randomSeed <seed> (default=" STRING(RANDOM_NUMBER_SEED) ")]\n"
"      [-version]\n"
"Exit codes:\n"
"  0=success\n"
"  1=error\n";

// Testing vs. training.
bool Testing;

// Training controls.
bool FemaleWantMouse;
bool FemaleWantStone;
int     FemaleNestSequence;

// Save/load file name.
char *Filename;

// World map: plain=0, forest=1, mouse=2, desert=3, stone=4
const int  WIDTH      = 21, HEIGHT = 21;
const char WorldMap[WIDTH][HEIGHT] =
{
    { 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 2, 1, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 2, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 2, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 3, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 3, 3, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 3, 4, 4, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 3, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 3, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 4, 4, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 3, 3, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 3, 0, 0, 0 }
};

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
Cell World[WIDTH][HEIGHT];

// Birds.
Male   *male;
Female *female;

// Probability of mouse movement.
static double MOUSE_MOVE_PROBABILITY = 0.1;

// Verbosity.
bool Verbose = true;

// Initialize.
int init()
{
    // Random numbers.
    srand(RANDOM_NUMBER_SEED);

    // Initialize world.
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            switch (WorldMap[x][y])
            {
            case 0:
                World[x][y].locale = LOCALE::PLAIN;
                World[x][y].object = OBJECT::NO_OBJECT;
                break;

            case 1:
                World[x][y].locale = LOCALE::FOREST;
                World[x][y].object = OBJECT::NO_OBJECT;
                break;

            case 2:
                World[x][y].locale = LOCALE::FOREST;
                World[x][y].object = OBJECT::MOUSE;
                break;

            case 3:
                World[x][y].locale = LOCALE::DESERT;
                World[x][y].object = OBJECT::NO_OBJECT;
                break;

            case 4:
                World[x][y].locale = LOCALE::DESERT;
                World[x][y].object = OBJECT::STONE;
                break;
            }
        }
    }

    // Create birds.
    female             = new Female();
    female->x           = WIDTH / 2;
    female->y           = HEIGHT / 2;
    female->food        = Female::INITIAL_FOOD;
    female->response    = Bird::RESPONSE::DO_NOTHING;
    male               = new Male();
    male->x             = WIDTH / 2;
    male->y             = HEIGHT / 2;
    male->food          = Male::INITIAL_FOOD;
    male->response      = Bird::RESPONSE::DO_NOTHING;
    FemaleWantMouse     = false;
    FemaleWantStone    = false;
    FemaleNestSequence = 0;
    setSensors(Bird::FEMALE);
    setSensors(Bird::MALE);
}


// Step.
void step()
{
    // Do responses in world.
    doResponse(Bird::FEMALE);
    doResponse(Bird::MALE);

    // Step mice.
    stepMice();

    // Set Female sensors.
    setSensors(Bird::FEMALE);

    // Produce Female response.
    if (Testing)
    {
        female->cycle();
    } else {
        cycleAutopilot(Bird::FEMALE);
    }
    if (Verbose)
    {
        printf("Female: Location: [%d,%d], ", female->x, female->y);
        female->print();
    }

    // Set male sensors.
    setSensors(Bird::MALE);

    // Produce male response.
    if (Testing)
    {
        male->cycle();
    } else {
        cycleAutopilot(Bird::MALE);
    }
    if (Verbose)
    {
        printf("Male: Location: [%d,%d], ", male->x, male->y);
        male->print();
    }
}


// Cycle on autopilot.
void cycleAutopilot(int gender)
{
    if (gender == Bird::MALE)
    {
        maleAutopilot();
    }
    else
    {
        femaleAutopilot();
    }
}


// Male autopilot.
void maleAutopilot()
{
    if (World[male->x][male->y].object == OBJECT::EGG)
    {
        male->response = Bird::RESPONSE::DO_NOTHING;
    }
    else
    {
        if (!getMouse() && !getStone())
        {
            returnToFemale();
        }
    }
}


// Get mouse.
bool getMouse()
{
    if ((male->x == female->x) && (male->y == female->y))
    {
        if (female->response == Female::RESPONSE::WANT_MOUSE)
        {
            FemaleWantMouse = true;
        }
    }
    if ((male->food > 0) && !FemaleWantMouse)
    {
        return(false);
    }
    if (male->hasObject == OBJECT::MOUSE)
    {
        if (male->food < 0) { male->food = 0; }
        if (male->food == 0)
        {
            male->response = Bird::RESPONSE::EAT;
            return(true);
        }
        if ((male->x == female->x) && (male->y == female->y))
        {
            male->response  = Male::RESPONSE::GIVE_MOUSE;
            FemaleWantMouse = false;
            return(true);
        }
        else
        {
            return(false);
        }
    }
    if (male->hasObject != OBJECT::NO_OBJECT)
    {
        male->response = Bird::RESPONSE::TOSS;
        return(true);
    }
    if (World[male->x][male->y].object == OBJECT::MOUSE)
    {
        male->response = Bird::RESPONSE::GET;
        return(true);
    }
    if (male->sensors[Bird::LEFT_OBJECT_SENSOR] == OBJECT::MOUSE)
    {
        male->response = Bird::RESPONSE::TURN_LEFT;
        return(true);
    }
    if (male->sensors[Bird::FORWARD_OBJECT_SENSOR] == OBJECT::MOUSE)
    {
        male->response = Bird::RESPONSE::MOVE;
        return(true);
    }
    if (male->sensors[Bird::RIGHT_OBJECT_SENSOR] == OBJECT::MOUSE)
    {
        male->response = Bird::RESPONSE::TURN_RIGHT;
        return(true);
    }
    if ((male->sensors[Bird::CURRENT_LOCALE_SENSOR] == LOCALE::FOREST) ||
        (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::FOREST) ||
        (male->sensors[Bird::FORWARD_LOCALE_SENSOR] == LOCALE::FOREST) ||
        (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::FOREST))
    {
        if (male->sensors[Bird::CURRENT_LOCALE_SENSOR] == LOCALE::FOREST)
        {
            if ((male->sensors[Bird::LEFT_LOCALE_SENSOR] != LOCALE::FOREST) &&
                (male->sensors[Bird::FORWARD_LOCALE_SENSOR] != LOCALE::FOREST) &&
                (male->sensors[Bird::RIGHT_LOCALE_SENSOR] != LOCALE::FOREST))
            {
                male->response = Bird::RESPONSE::TURN_RIGHT;
            }
            else
            {
                vector<int> responses;
                if (male->sensors[Bird::FORWARD_LOCALE_SENSOR] == LOCALE::FOREST)
                {
                    responses.push_back(Bird::RESPONSE::MOVE);
                    responses.push_back(Bird::RESPONSE::MOVE);
                }
                if (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::FOREST)
                {
                    responses.push_back(Bird::RESPONSE::TURN_LEFT);
                }
                if (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::FOREST)
                {
                    responses.push_back(Bird::RESPONSE::TURN_RIGHT);
                }
                male->response = responses[rand() % responses.size()];
            }
        }
        else
        {
            vector<int> responses;
            if (male->sensors[Bird::FORWARD_LOCALE_SENSOR] == LOCALE::FOREST)
            {
                responses.push_back(Bird::RESPONSE::MOVE);
                responses.push_back(Bird::RESPONSE::MOVE);
            }
            if (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::FOREST)
            {
                responses.push_back(Bird::RESPONSE::TURN_LEFT);
            }
            if (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::FOREST)
            {
                responses.push_back(Bird::RESPONSE::TURN_RIGHT);
            }
            male->response = responses[rand() % responses.size()];
        }
        return(true);
    }
    int mousex = 3;
    int mousey = 5;
    if (male->x > mousex)
    {
        if (male->orientation == Bird::ORIENTATION::WEST)
        {
        male->response = Bird::RESPONSE::MOVE;
        }
        else
        {
        if (male->orientation == Bird::ORIENTATION::SOUTH)
        {
            male->response = Bird::RESPONSE::TURN_RIGHT;
        }
        else
        {
            male->response = Bird::RESPONSE::TURN_LEFT;
        }
        }
        return(true);
    }
    if (male->x < mousex)
    {
        if (male->orientation == Bird::ORIENTATION::EAST)
        {
        male->response = Bird::RESPONSE::MOVE;
        }
        else
        {
        if (male->orientation == Bird::ORIENTATION::SOUTH)
        {
            male->response = Bird::RESPONSE::TURN_LEFT;
        }
        else
        {
            male->response = Bird::RESPONSE::TURN_RIGHT;
        }
        }
        return(true);
    }
    if (male->y > mousey)
    {
        if (male->orientation == Bird::ORIENTATION::NORTH)
        {
        male->response = Bird::RESPONSE::MOVE;
        }
        else
        {
        if (male->orientation == Bird::ORIENTATION::EAST)
        {
            male->response = Bird::RESPONSE::TURN_LEFT;
        }
        else
        {
            male->response = Bird::RESPONSE::TURN_RIGHT;
        }
        }
        return(true);
    }
    if (male->y < mousey)
    {
        if (male->orientation == Bird::ORIENTATION::SOUTH)
        {
        male->response = Bird::RESPONSE::MOVE;
        }
        else
        {
        if (male->orientation == Bird::ORIENTATION::EAST)
        {
            male->response = Bird::RESPONSE::TURN_RIGHT;
        }
        else
        {
            male->response = Bird::RESPONSE::TURN_LEFT;
        }
        }
    }
    return(true);
}


// Get stone.
bool getStone()
{
    if ((male->x == female->x) && (male->y == female->y))
    {
        if (female->response == Female::RESPONSE::WANT_STONE)
        {
        FemaleWantStone = true;
        }
    }
    if (FemaleWantMouse || !FemaleWantStone)
    {
        return(false);
    }
    if (male->hasObject == OBJECT::STONE)
    {
        if ((male->x == female->x) && (male->y == female->y))
        {
        male->response   = Male::RESPONSE::GIVE_STONE;
        FemaleWantStone = false;
        return(true);
        }
        else
        {
        return(false);
        }
    }
    if ((World[male->x][male->y].object == OBJECT::STONE) &&
        (World[male->x][male->y].locale == LOCALE::DESERT))
    {
        male->response = Bird::RESPONSE::GET;
        return(true);
    }
    if ((male->sensors[Bird::LEFT_OBJECT_SENSOR] == OBJECT::STONE) &&
        (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::DESERT))
    {
        male->response = Bird::RESPONSE::TURN_LEFT;
        return(true);
    }
    if ((male->sensors[Bird::FORWARD_OBJECT_SENSOR] == OBJECT::STONE) &&
        (male->sensors[Bird::FORWARD_LOCALE_SENSOR] == LOCALE::DESERT))
    {
        male->response = Bird::RESPONSE::MOVE;
        return(true);
    }
    if ((male->sensors[Bird::RIGHT_OBJECT_SENSOR] == OBJECT::STONE) &&
        (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::DESERT))
    {
        male->response = Bird::RESPONSE::TURN_RIGHT;
        return(true);
    }
    if ((male->sensors[Bird::CURRENT_LOCALE_SENSOR] == LOCALE::DESERT) ||
        (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::DESERT) ||
        (male->sensors[Bird::FORWARD_LOCALE_SENSOR] == LOCALE::DESERT) ||
        (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::DESERT))
    {
        if (male->sensors[Bird::CURRENT_LOCALE_SENSOR] == LOCALE::DESERT)
        {
            if ((male->sensors[Bird::LEFT_LOCALE_SENSOR] != LOCALE::DESERT) &&
                (male->sensors[Bird::FORWARD_LOCALE_SENSOR] != LOCALE::DESERT) &&
                (male->sensors[Bird::RIGHT_LOCALE_SENSOR] != LOCALE::DESERT))
            {
                male->response = Bird::RESPONSE::TURN_RIGHT;
            }
            else
            {
                vector<int> responses;
                if (male->sensors[Bird::FORWARD_LOCALE_SENSOR] == LOCALE::DESERT)
                {
                    responses.push_back(Bird::RESPONSE::MOVE);
                    responses.push_back(Bird::RESPONSE::MOVE);
                }
                if (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::DESERT)
                {
                    responses.push_back(Bird::RESPONSE::TURN_LEFT);
                }
                if (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::DESERT)
                {
                    responses.push_back(Bird::RESPONSE::TURN_RIGHT);
                }
                male->response = responses[rand() % responses.size()];
            }
        }
        else
        {
            vector<int> responses;
            if (male->sensors[Bird::FORWARD_LOCALE_SENSOR] == LOCALE::DESERT)
            {
                responses.push_back(Bird::RESPONSE::MOVE);
                responses.push_back(Bird::RESPONSE::MOVE);
            }
            if (male->sensors[Bird::LEFT_LOCALE_SENSOR] == LOCALE::DESERT)
            {
                responses.push_back(Bird::RESPONSE::TURN_LEFT);
            }
            if (male->sensors[Bird::RIGHT_LOCALE_SENSOR] == LOCALE::DESERT)
            {
                responses.push_back(Bird::RESPONSE::TURN_RIGHT);
            }
            male->response = responses[rand() % responses.size()];
        }
        return(true);
    }
    int stonex = 14;
    int stoney = 16;
    if (male->x > stonex)
    {
        if (male->orientation == Bird::ORIENTATION::WEST)
        {
        male->response = Bird::RESPONSE::MOVE;
        }
        else
        {
        if (male->orientation == Bird::ORIENTATION::SOUTH)
        {
            male->response = Bird::RESPONSE::TURN_RIGHT;
        }
        else
        {
            male->response = Bird::RESPONSE::TURN_LEFT;
        }
        }
        return(true);
    }
    if (male->x < stonex)
    {
        if (male->orientation == Bird::ORIENTATION::EAST)
        {
        male->response = Bird::RESPONSE::MOVE;
        }
        else
        {
        if (male->orientation == Bird::ORIENTATION::SOUTH)
        {
            male->response = Bird::RESPONSE::TURN_LEFT;
        }
        else
        {
            male->response = Bird::RESPONSE::TURN_RIGHT;
        }
        }
        return(true);
    }
    if (male->y > stoney)
    {
        if (male->orientation == Bird::ORIENTATION::NORTH)
        {
        male->response = Bird::RESPONSE::MOVE;
        }
        else
        {
        if (male->orientation == Bird::ORIENTATION::EAST)
        {
            male->response = Bird::RESPONSE::TURN_LEFT;
        }
        else
        {
            male->response = Bird::RESPONSE::TURN_RIGHT;
        }
        }
        return(true);
    }
    if (male->y < stoney)
    {
        if (male->orientation == Bird::ORIENTATION::SOUTH)
        {
        male->response = Bird::RESPONSE::MOVE;
        }
        else
        {
        if (male->orientation == Bird::ORIENTATION::EAST)
        {
            male->response = Bird::RESPONSE::TURN_RIGHT;
        }
        else
        {
            male->response = Bird::RESPONSE::TURN_LEFT;
        }
        }
    }
    return(true);
}


// Male returns to female.
bool returnToFemale()
{
    switch (male->sensors[Bird::MATE_PROXIMITY_SENSOR])
    {
    case Bird::MATE_PROXIMITY_PRESENT:
        male->response = Bird::RESPONSE::DO_NOTHING;
        return(true);

    case Bird::MATE_PROXIMITY_LEFT:
        male->response = Bird::RESPONSE::TURN_LEFT;
        break;

    case Bird::MATE_PROXIMITY_FORWARD:
        male->response = Bird::RESPONSE::MOVE;
        break;

    case Bird::MATE_PROXIMITY_RIGHT:
        male->response = Bird::RESPONSE::TURN_RIGHT;
        break;

    case Bird::MATE_PROXIMITY_UNKNOWN:
        int centerx = WIDTH / 2;
        int centery = HEIGHT / 2;
        if (male->x < centerx)
        {
        if (male->orientation == Bird::ORIENTATION::EAST)
        {
            male->response = Bird::RESPONSE::MOVE;
        }
        else
        {
            if (male->orientation == Bird::ORIENTATION::SOUTH)
            {
                male->response = Bird::RESPONSE::TURN_LEFT;
            }
            else
            {
                male->response = Bird::RESPONSE::TURN_RIGHT;
            }
        }
        }
        else if (male->x > centerx)
        {
        if (male->orientation == Bird::ORIENTATION::WEST)
        {
            male->response = Bird::RESPONSE::MOVE;
        }
        else
        {
            if (male->orientation == Bird::ORIENTATION::SOUTH)
            {
                male->response = Bird::RESPONSE::TURN_RIGHT;
            }
            else
            {
                male->response = Bird::RESPONSE::TURN_LEFT;
            }
        }
        }
        else if (male->y < centery)
        {
            if (male->orientation == Bird::ORIENTATION::SOUTH)
            {
                male->response = Bird::RESPONSE::MOVE;
            }
            else
            {
                if (male->orientation == Bird::ORIENTATION::EAST)
                {
                    male->response = Bird::RESPONSE::TURN_RIGHT;
                }
                else
                {
                    male->response = Bird::RESPONSE::TURN_LEFT;
                }
            }
        }
        else if (male->y > centery)
        {
            if (male->orientation == Bird::ORIENTATION::NORTH)
            {
                male->response = Bird::RESPONSE::MOVE;
            }
            else
            {
                if (male->orientation == Bird::ORIENTATION::WEST)
                {
                    male->response = Bird::RESPONSE::TURN_RIGHT;
                }
                else
                {
                    male->response = Bird::RESPONSE::TURN_LEFT;
                }
            }
        }
        else
        {
            male->response = Bird::RESPONSE::MOVE;
        }
        break;
    }
    return(false);
}


// Female autopilot.
void femaleAutopilot()
{
    if (World[female->x][female->y].object == OBJECT::EGG)
    {
        female->response = Bird::RESPONSE::DO_NOTHING;
        return;
    }
    if (female->food < 0) { female->food = 0; }
    if (female->food == 0)
    {
        if (female->hasObject == OBJECT::MOUSE)
        {
        female->response = Bird::RESPONSE::EAT;
        }
        else
        {
        if (female->hasObject == OBJECT::NO_OBJECT)
        {
            female->response = Female::RESPONSE::WANT_MOUSE;
        }
        else
        {
            female->response = Bird::RESPONSE::TOSS;
        }
        }
        return;
    }
    switch (FemaleNestSequence)
    {
    case 0:
        if (World[female->x][female->y].object == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::GET;
        }
        else if (female->hasObject == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::TOSS;
        }
        else if (female->orientation == Bird::ORIENTATION::NORTH)
        {
            female->response = Bird::RESPONSE::MOVE;
            FemaleNestSequence++;
        }
        else
        {
            female->response = Bird::RESPONSE::TURN_RIGHT;
        }
        break;

    case 1:
        if (World[female->x][female->y].object == OBJECT::STONE)
        {
            if (female->orientation == Bird::ORIENTATION::WEST)
            {
                female->response = Bird::RESPONSE::MOVE;
                FemaleNestSequence++;
            }
            else
            {
                female->response = Bird::RESPONSE::TURN_LEFT;
            }
        }
        else if (female->hasObject == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::PUT;
        }
        else
        {
            female->response = Female::RESPONSE::WANT_STONE;
        }
        break;

    case 2:
        if (World[female->x][female->y].object == OBJECT::STONE)
        {
            if (female->orientation == Bird::ORIENTATION::SOUTH)
            {
                female->response = Bird::RESPONSE::MOVE;
                FemaleNestSequence++;
            }
            else
            {
                female->response = Bird::RESPONSE::TURN_LEFT;
            }
        }
        else if (female->hasObject == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::PUT;
        }
        else
        {
            female->response = Female::RESPONSE::WANT_STONE;
        }
        break;

    case 3:
        if (World[female->x][female->y].object == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::MOVE;
            FemaleNestSequence++;
        }
        else if (female->hasObject == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::PUT;
        }
        else
        {
            female->response = Female::RESPONSE::WANT_STONE;
        }
        break;

    case 4:
        if (World[female->x][female->y].object == OBJECT::STONE)
        {
            if (female->orientation == Bird::ORIENTATION::EAST)
            {
                female->response = Bird::RESPONSE::MOVE;
                FemaleNestSequence++;
            }
            else
            {
                female->response = Bird::RESPONSE::TURN_LEFT;
            }
        }
        else if (female->hasObject == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::PUT;
        }
        else
        {
            female->response = Female::RESPONSE::WANT_STONE;
        }
        break;

    case 5:
        if (World[female->x][female->y].object == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::MOVE;
            FemaleNestSequence++;
        }
        else if (female->hasObject == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::PUT;
        }
        else
        {
            female->response = Female::RESPONSE::WANT_STONE;
        }
        break;

    case 6:
        if (World[female->x][female->y].object == OBJECT::STONE)
        {
            if (female->orientation == Bird::ORIENTATION::NORTH)
            {
                female->response = Bird::RESPONSE::MOVE;
                FemaleNestSequence++;
            }
            else
            {
                female->response = Bird::RESPONSE::TURN_LEFT;
            }
        }
        else if (female->hasObject == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::PUT;
        }
        else
        {
            female->response = Female::RESPONSE::WANT_STONE;
        }
        break;

    case 7:
        if (World[female->x][female->y].object == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::MOVE;
            FemaleNestSequence++;
        }
        else if (female->hasObject == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::PUT;
        }
        else
        {
            female->response = Female::RESPONSE::WANT_STONE;
        }
        break;

    case 8:
        if (World[female->x][female->y].object == OBJECT::STONE)
        {
            if (female->orientation == Bird::ORIENTATION::WEST)
            {
                female->response = Bird::RESPONSE::MOVE;
                FemaleNestSequence++;
            }
            else
            {
                female->response = Bird::RESPONSE::TURN_LEFT;
            }
        }
        else if (female->hasObject == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::PUT;
        }
        else
        {
            female->response = Female::RESPONSE::WANT_STONE;
        }
        break;

    case 9:
        if (World[female->x][female->y].object == OBJECT::STONE)
        {
            if (female->orientation == Bird::ORIENTATION::SOUTH)
            {
                female->response = Bird::RESPONSE::MOVE;
                FemaleNestSequence++;
            }
            else
            {
                female->response = Bird::RESPONSE::TURN_LEFT;
            }
        }
        else if (female->hasObject == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::PUT;
        }
        else
        {
            female->response = Female::RESPONSE::WANT_STONE;
        }
        break;

    case 10:
        if (World[female->x][female->y].object == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::GET;
        }
        else if (female->hasObject == OBJECT::STONE)
        {
            female->response = Bird::RESPONSE::TOSS;
        }
        else if (World[female->x][female->y].object == OBJECT::NO_OBJECT)
        {
            female->response = Female::RESPONSE::LAY_EGG;
            FemaleNestSequence++;
        }
        else
        {
            female->response    = Bird::RESPONSE::TURN_RIGHT;
            FemaleNestSequence = 0;
        }
        break;

    case 11:
        break;
    }
}


// Do bird response.
void doResponse(int gender)
{
    Bird* bird = (Bird*)male;
    if (gender == Bird::FEMALE)
    {
        bird = (Bird*)female;
    }
    Cell cell = World[bird->x][bird->y];
    int response = male->response;
    if (gender == Bird::FEMALE)
    {
        female->response;
    }

    if (response < Bird::RESPONSE::NUM_RESPONSES)
    {
        switch (response)
        {
        case Bird::RESPONSE::DO_NOTHING:
        break;

        case Bird::RESPONSE::EAT:
        if (bird->hasObject == OBJECT::MOUSE)
        {
            if (gender == Bird::MALE)
            {
                if (Male::RANDOMIZE_FOOD_LEVEL && (Male::FOOD_DURATION > 0))
                {
                    male->food = rand() % (Male::FOOD_DURATION + 1);
                }
                else
                {
                    male->food = Male::FOOD_DURATION;
                }
            }
            else
            {
                if (Female::RANDOMIZE_FOOD_LEVEL && (Female::FOOD_DURATION > 0))
                {
                    female->food = rand() % (Female::FOOD_DURATION + 1);
                }
                else
                {
                    female->food = Female::FOOD_DURATION;
                }
            }
            bird->hasObject = OBJECT::NO_OBJECT;
        }
        break;

        case Bird::RESPONSE::GET:
        if (bird->hasObject == OBJECT::NO_OBJECT)
        {
            bird->hasObject = cell.object;
            cell.object    = OBJECT::NO_OBJECT;
        }
        break;

        case Bird::RESPONSE::PUT:
        if (cell.object == OBJECT::NO_OBJECT)
        {
            cell.object    = bird->hasObject;
            bird->hasObject = OBJECT::NO_OBJECT;
        }
        break;

        case Bird::RESPONSE::TOSS:
        if (bird->hasObject == OBJECT::NO_OBJECT) { break; }

        // Object lands randomly nearby.
        int i = HEIGHT / 2;
        if (i > WIDTH / 2) { i = WIDTH / 2; }
        for (int j = 5; j < i; j++)
        {
            int x = (int)(rand() % (long)j);
            int y = (int)(rand() % (long)j);
            if ((x == 0) && (y == 0)) { continue; }
            if ((rand() % 2) == 0) { x = -x; }
            if ((rand() % 2) == 0) { y = -y; }
            x += bird->x;
            if ((x < 0) || (x >= WIDTH)) { continue; }
            y += bird->y;
            if ((y < 0) || (y >= HEIGHT)) { continue; }
            if (World[x][y].object == OBJECT::NO_OBJECT)
            {
                World[x][y].object = bird->hasObject;
                bird->hasObject     = OBJECT::NO_OBJECT;
                break;
            }
        }
        if (bird->hasObject != OBJECT::NO_OBJECT)
        {
            // Vaporize object.
            bird->hasObject = OBJECT::NO_OBJECT;
        }
        break;

        case Bird::RESPONSE::MOVE:
        switch (bird->orientation)
        {
        case Bird::ORIENTATION::NORTH:
            if (bird->y > 0)
            {
                bird->y--;
            }
            break;

        case Bird::ORIENTATION::SOUTH:
            if (bird->y < HEIGHT - 1)
            {
                bird->y++;
            }
            break;


        case Bird::ORIENTATION::EAST:
            if (bird->x < WIDTH - 1)
            {
                bird->x++;
            }
            break;

        case Bird::ORIENTATION::WEST:
            if (bird->x > 0)
            {
                bird->x--;
            }
            break;
        }
        break;

        case Bird::RESPONSE::TURN_RIGHT:
        switch (bird->orientation)
        {
        case Bird::ORIENTATION::NORTH:
            bird->orientation = Bird::ORIENTATION::EAST;
            break;

        case Bird::ORIENTATION::SOUTH:
            bird->orientation = Bird::ORIENTATION::WEST;
            break;

        case Bird::ORIENTATION::EAST:
            bird->orientation = Bird::ORIENTATION::SOUTH;
            break;

        case Bird::ORIENTATION::WEST:
            bird->orientation = Bird::ORIENTATION::NORTH;
            break;
        }
        break;

        case Bird::RESPONSE::TURN_LEFT:
        switch (bird->orientation)
        {
        case Bird::ORIENTATION::NORTH:
            bird->orientation = Bird::ORIENTATION::WEST;
            break;

        case Bird::ORIENTATION::SOUTH:
            bird->orientation = Bird::ORIENTATION::EAST;
            break;

        case Bird::ORIENTATION::EAST:
            bird->orientation = Bird::ORIENTATION::NORTH;
            break;

        case Bird::ORIENTATION::WEST:
            bird->orientation = Bird::ORIENTATION::SOUTH;
            break;
        }
        break;
        }
    }
    else
    {
        // Gender-specific response.
        if (gender == Bird::MALE)
        {
            switch (male->response)
            {
            case Male::RESPONSE::GIVE_MOUSE:
                if (gender == Bird::MALE)
                {
                    if ((bird->hasObject == OBJECT::MOUSE) && (female->hasObject == OBJECT::NO_OBJECT))
                    {
                        if ((bird->x == female->x) && (bird->y == female->y))
                        {
                            female->hasObject = bird->hasObject;
                            bird->hasObject   = OBJECT::NO_OBJECT;
                        }
                    }
                }
                break;

            case Male::RESPONSE::GIVE_STONE:
                if (gender == Bird::MALE)
                {
                    if ((bird->hasObject == OBJECT::STONE) && (female->hasObject == OBJECT::NO_OBJECT))
                    {
                        if ((bird->x == female->x) && (bird->y == female->y))
                        {
                            female->hasObject = bird->hasObject;
                            bird->hasObject   = OBJECT::NO_OBJECT;
                        }
                    }
                }
                break;
            }
        }
        else
        {
        // Female response.
        switch (female->response)
        {
        case Female::RESPONSE::WANT_MOUSE:
            break;

        case Female::RESPONSE::WANT_STONE:
            break;

        case Female::RESPONSE::LAY_EGG:
            if (gender == Bird::FEMALE)
            {
                if (cell.object == OBJECT::NO_OBJECT)
                {
                    cell.object = OBJECT::EGG;
                }
            }
            break;
        }
        }
    }

    // Digest food.
    bird->digest();
}


// Set bird sensors.
void setSensors(int gender)
{
    Bird *bird;
    int *sensors;

    if (gender == Bird::FEMALE)
    {
        bird    = female;
        sensors = new int[Female::NUM_SENSORS];
        female->setSensors(sensors);
    }
    else
    {
        bird    = male;
        sensors = new int[Male::NUM_SENSORS];
        male->setSensors(sensors);
    }

    // Set locale, object, and mate proximity sensors.
    sensors[Bird::MATE_PROXIMITY_SENSOR] = Bird::MATE_PROXIMITY_UNKNOWN;
    int x = -1;
    int y = -1;
    for (int i = 0; i < Bird::NUM_CELL_SENSORS; i++)
    {
        switch (i)
        {
        // Current.
        case 0:
        sensors[Bird::CURRENT_LOCALE_SENSOR] = World[bird->x][bird->y].locale;
        sensors[Bird::CURRENT_OBJECT_SENSOR] = World[bird->x][bird->y].object;
        if ((male->x == female->x) && (male->y == female->y))
        {
            sensors[Bird::MATE_PROXIMITY_SENSOR] = Bird::MATE_PROXIMITY_PRESENT;
        }
        break;

        // Left.
        case 1:
        x = -1;
        y = -1;
        switch (bird->orientation)
        {
        case Bird::ORIENTATION::NORTH:
            if (bird->x > 0)
            {
                sensors[Bird::LEFT_LOCALE_SENSOR] = World[bird->x - 1][bird->y].locale;
                sensors[Bird::LEFT_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
                x = bird->x - 1;
                y = bird->y;
            }
            break;

        case Bird::ORIENTATION::SOUTH:
            if (bird->x < WIDTH - 1)
            {
                sensors[Bird::LEFT_LOCALE_SENSOR] = World[bird->x + 1][bird->y].locale;
                sensors[Bird::LEFT_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
                x = bird->x + 1;
                y = bird->y;
            }
            break;

        case Bird::ORIENTATION::EAST:
            if (bird->y > 0)
            {
                sensors[Bird::LEFT_LOCALE_SENSOR] = World[bird->x][bird->y - 1].locale;
                sensors[Bird::LEFT_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
                x = bird->x;
                y = bird->y - 1;
            }
            break;

        case Bird::ORIENTATION::WEST:
            if (bird->y < HEIGHT - 1)
            {
                sensors[Bird::LEFT_LOCALE_SENSOR] = World[bird->x][bird->y + 1].locale;
                sensors[Bird::LEFT_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
                x = bird->x;
                y = bird->y + 1;
            }
            break;
        }
        if (x >= 0)
        {
            if (gender == Bird::MALE)
            {
                if ((female->x == x) && (female->y == y))
                {
                    sensors[Bird::MATE_PROXIMITY_SENSOR] = Bird::MATE_PROXIMITY_LEFT;
                }
            }
            else
            {
                if ((male->x == x) && (male->y == y))
                {
                    sensors[Bird::MATE_PROXIMITY_SENSOR] = Bird::MATE_PROXIMITY_LEFT;
                }
            }
        }
        break;

        // Forward.
        case 2:
        x = -1;
        y = -1;
        switch (bird->orientation)
        {
        case Bird::ORIENTATION::NORTH:
            if (bird->y > 0)
            {
                sensors[Bird::FORWARD_LOCALE_SENSOR] = World[bird->x][bird->y - 1].locale;
                sensors[Bird::FORWARD_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
                x = bird->x;
                y = bird->y - 1;
            }
            break;

        case Bird::ORIENTATION::SOUTH:
            if (bird->y < HEIGHT - 1)
            {
                sensors[Bird::FORWARD_LOCALE_SENSOR] = World[bird->x][bird->y + 1].locale;
                sensors[Bird::FORWARD_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
                x = bird->x;
                y = bird->y + 1;
            }
            break;

        case Bird::ORIENTATION::EAST:
            if (bird->x < WIDTH - 1)
            {
                sensors[Bird::FORWARD_LOCALE_SENSOR] = World[bird->x + 1][bird->y].locale;
                sensors[Bird::FORWARD_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
                x = bird->x + 1;
                y = bird->y;
            }
            break;

        case Bird::ORIENTATION::WEST:
            if (bird->x > 0)
            {
                sensors[Bird::FORWARD_LOCALE_SENSOR] = World[bird->x - 1][bird->y].locale;
                sensors[Bird::FORWARD_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
                x = bird->x - 1;
                y = bird->y;
            }
            break;
        }
        if (x >= 0)
        {
            if (gender == Bird::MALE)
            {
                if ((female->x == x) && (female->y == y))
                {
                    sensors[Bird::MATE_PROXIMITY_SENSOR] = Bird::MATE_PROXIMITY_FORWARD;
                }
            }
            else
            {
                if ((male->x == x) && (male->y == y))
                {
                    sensors[Bird::MATE_PROXIMITY_SENSOR] = Bird::MATE_PROXIMITY_FORWARD;
                }
            }
        }
        break;

        // Right.
        case 3:
        x = -1;
        y = -1;
        switch (bird->orientation)
        {
        case Bird::ORIENTATION::NORTH:
            if (bird->x < WIDTH - 1)
            {
                sensors[Bird::RIGHT_LOCALE_SENSOR] = World[bird->x + 1][bird->y].locale;
                sensors[Bird::RIGHT_OBJECT_SENSOR] = World[bird->x + 1][bird->y].object;
                x = bird->x + 1;
                y = bird->y;
            }
            break;

        case Bird::ORIENTATION::SOUTH:
            if (bird->x > 0)
            {
                sensors[Bird::RIGHT_LOCALE_SENSOR] = World[bird->x - 1][bird->y].locale;
                sensors[Bird::RIGHT_OBJECT_SENSOR] = World[bird->x - 1][bird->y].object;
                x = bird->x - 1;
                y = bird->y;
            }
            break;

        case Bird::ORIENTATION::EAST:
            if (bird->y < HEIGHT - 1)
            {
                sensors[Bird::RIGHT_LOCALE_SENSOR] = World[bird->x][bird->y + 1].locale;
                sensors[Bird::RIGHT_OBJECT_SENSOR] = World[bird->x][bird->y + 1].object;
                x = bird->x;
                y = bird->y + 1;
            }
            break;

        case Bird::ORIENTATION::WEST:
            if (bird->y > 0)
            {
                sensors[Bird::RIGHT_LOCALE_SENSOR] = World[bird->x][bird->y - 1].locale;
                sensors[Bird::RIGHT_OBJECT_SENSOR] = World[bird->x][bird->y - 1].object;
                x = bird->x;
                y = bird->y - 1;
            }
            break;
        }
        if (x >= 0)
        {
            if (gender == Bird::MALE)
            {
                if ((female->x == x) && (female->y == y))
                {
                    sensors[Bird::MATE_PROXIMITY_SENSOR] = Bird::MATE_PROXIMITY_RIGHT;
                }
            }
            else
            {
                if ((male->x == x) && (male->y == y))
                {
                    sensors[Bird::MATE_PROXIMITY_SENSOR] = Bird::MATE_PROXIMITY_RIGHT;
                }
            }
        }
        break;
        }
    }

    // Male senses female want?
    if (gender == Bird::MALE)
    {
        sensors[Male::WANT_MOUSE_SENSOR]  = 0;
        sensors[Male::WANT_STONE_SENSOR] = 0;
        if ((bird->x == female->x) && (bird->y == female->y))
        {
        switch (female->response)
        {
        case Female::RESPONSE::WANT_MOUSE:
            sensors[Male::WANT_MOUSE_SENSOR] = 1;
            break;

        case Female::RESPONSE::WANT_STONE:
            sensors[Male::WANT_STONE_SENSOR] = 1;
            break;
        }
        }
    }
}

// Euclidean distance.
static double distance(double x1, double y1, double x2, double y2)
{
    double xd = x1 - x2;
    double yd = y1 - y2;

    return(sqrt((xd * xd) + (yd * yd)));
}


// Move mice in forest.
void stepMice()
{
    for (int x = (rand() % WIDTH), x2 = 0; x2 < WIDTH; x = (x + 1) % WIDTH, x2++)
    {
        for (int y = (rand() % HEIGHT), y2 = 0; y2 < HEIGHT; y = (y + 1) % HEIGHT, y2++)
        {
        if ((World[x][y].object == OBJECT::MOUSE) &&
            ((male->x != x) || (male->y != y) || (male->response != Bird::RESPONSE::GET)) &&
            (((double)rand() / (double)(RAND_MAX + 1))) < MOUSE_MOVE_PROBABILITY)
        {
            boolean move = false;
            for (int i = (rand() % 4), i2 = 0; i2 < 4 && !move; i = (i + 1) % 4, i2++)
            {
                switch (i)
                {
                case 0:
                    if ((y > 0) && (World[x][y - 1].locale == LOCALE::FOREST) &&
                        (World[x][y - 1].object == OBJECT::NO_OBJECT))
                    {
                    World[x][y].object     = OBJECT::NO_OBJECT;
                    World[x][y - 1].object = -OBJECT::MOUSE;
                    move = true;
                    }
                    break;

                case 1:
                    if ((y < HEIGHT - 1) && (World[x][y + 1].locale == LOCALE::FOREST) &&
                        (World[x][y + 1].object == OBJECT::NO_OBJECT))
                    {
                    World[x][y].object     = OBJECT::NO_OBJECT;
                    World[x][y + 1].object = -OBJECT::MOUSE;
                    move = true;
                    }
                    break;

                case 2:
                    if ((x > 0) && (World[x - 1][y].locale == LOCALE::FOREST) &&
                        (World[x - 1][y].object == OBJECT::NO_OBJECT))
                    {
                    World[x][y].object     = OBJECT::NO_OBJECT;
                    World[x - 1][y].object = -OBJECT::MOUSE;
                    move = true;
                    }
                    break;

                case 3:
                    if ((x < WIDTH - 1) && (World[x + 1][y].locale == LOCALE::FOREST) &&
                        (World[x + 1][y].object == OBJECT::NO_OBJECT))
                    {
                    World[x][y].object     = OBJECT::NO_OBJECT;
                    World[x + 1][y].object = -OBJECT::MOUSE;
                    move = true;
                    }
                    break;
                }
            }
        }
        }
    }
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
        if (World[x][y].object == -OBJECT::MOUSE)
        {
            World[x][y].object = OBJECT::MOUSE;
        }
        }
    }
}

// Main.
void main(int argc, char* args[])
{
    bool gotTrainTest = false;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(args[i], "-train") == 0)
        {
            Testing = false;
            if (gotTrainTest)
            {
                fprintf(stderr,"Duplicate train/test option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            gotTrainTest = true;
            continue;
        }
        if (strcmp(args[i], "-save") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid save option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            Filename = args[i];
            continue;
        }
        if (strcmp(args[i], "-test") == 0)
        {
            Testing = true;
            if (gotTrainTest)
            {
                fprintf(stderr, "Duplicate train/test option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            gotTrainTest = true;
            continue;
        }
        if (strcmp(args[i], "-load") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "Invalid load option\n");
                fprintf(stderr, Usage);
                exit(1);
            }
            Filename = args[i];
            continue;
        }
        if (args[i].equals("-runs"))
        {
        i++;
        if (i >= args.length)
        {
            System.err.println("Invalid runs option");
            System.err.println(Usage);
            System.exit(1);
        }
        try
        {
            runs = Integer.parseInt(args[i]);
        }
        catch (NumberFormatException e) {
            System.err.println("Invalid runs option");
            System.err.println(Usage);
            System.exit(1);
        }
        if (runs < 0)
        {
            System.err.println("Invalid runs option");
            System.err.println(Usage);
            System.exit(1);
        }
        continue;
        }
        if (args[i].equals("-responseDriver"))
        {
        i++;
        if (i >= args.length)
        {
            System.err.println("Invalid driver option");
            System.err.println(Usage);
            System.exit(1);
        }
        if (args[i].equals("autopilot"))
        {
            ResponseDriver = RESPONSE_DRIVER.AUTOPILOT;
        }
        else if (args[i].equals("bird"))
        {
            ResponseDriver = RESPONSE_DRIVER.BIRD;
        }
        else
        {
            System.err.println("Invalid responseDriver option");
            System.err.println(Usage);
            System.exit(1);
        }
        continue;
        }
        if (args[i].equals("-maleInitialFood"))
        {
        i++;
        if (i >= args.length)
        {
            System.err.println("Invalid maleInitialFood option");
            System.err.println(Usage);
            System.exit(1);
        }
        try
        {
            Male::INITIAL_FOOD = Integer.parseInt(args[i]);
        }
        catch (NumberFormatException e) {
            System.err.println("Invalid maleInitialFood option");
            System.err.println(Usage);
            System.exit(1);
        }
        if (Male::INITIAL_FOOD < 0)
        {
            System.err.println("Invalid maleInitialFood option");
            System.err.println(Usage);
            System.exit(1);
        }
        continue;
        }
        if (args[i].equals("-FemaleInitialFood"))
        {
        i++;
        if (i >= args.length)
        {
            System.err.println("Invalid FemaleInitialFood option");
            System.err.println(Usage);
            System.exit(1);
        }
        try
        {
            Female::INITIAL_FOOD = Integer.parseInt(args[i]);
        }
        catch (NumberFormatException e) {
            System.err.println("Invalid FemaleInitialFood option");
            System.err.println(Usage);
            System.exit(1);
        }
        if (Female::INITIAL_FOOD < 0)
        {
            System.err.println("Invalid FemaleInitialFood option");
            System.err.println(Usage);
            System.exit(1);
        }
        continue;
        }
        if (args[i].equals("-maleFoodDuration"))
        {
        i++;
        if (i >= args.length)
        {
            System.err.println("Invalid maleFoodDuration option");
            System.err.println(Usage);
            System.exit(1);
        }
        try
        {
            Male::FOOD_DURATION = Integer.parseInt(args[i]);
        }
        catch (NumberFormatException e) {
            System.err.println("Invalid maleFoodDuration option");
            System.err.println(Usage);
            System.exit(1);
        }
        if (Male::FOOD_DURATION < 0)
        {
            System.err.println("Invalid maleFoodDuration option");
            System.err.println(Usage);
            System.exit(1);
        }
        continue;
        }
        if (args[i].equals("-FemaleFoodDuration"))
        {
        i++;
        if (i >= args.length)
        {
            System.err.println("Invalid FemaleFoodDuration option");
            System.err.println(Usage);
            System.exit(1);
        }
        try
        {
            Female::FOOD_DURATION = Integer.parseInt(args[i]);
        }
        catch (NumberFormatException e) {
            System.err.println("Invalid FemaleFoodDuration option");
            System.err.println(Usage);
            System.exit(1);
        }
        if (Female::FOOD_DURATION < 0)
        {
            System.err.println("Invalid FemaleFoodDuration option");
            System.err.println(Usage);
            System.exit(1);
        }
        continue;
        }
        if (args[i].equals("-randomizeMaleFoodLevel"))
        {
        Male::RANDOMIZE_FOOD_LEVEL = true;
        continue;
        }
        if (args[i].equals("-randomizeFemaleFoodLevel"))
        {
        Female::RANDOMIZE_FOOD_LEVEL = true;
        continue;
        }
        if (args[i].equals("-randomSeed"))
        {
        i++;
        if (i >= args.length)
        {
            System.err.println("Invalid randomSeed option");
            System.err.println(Usage);
            System.exit(1);
        }
        try
        {
            RANDOM_NUMBER_SEED = Integer.parseInt(args[i]);
        }
        catch (NumberFormatException e) {
            System.err.println("Invalid randomSeed option");
            System.err.println(Usage);
            System.exit(1);
        }
        if (RANDOM_NUMBER_SEED <= 0)
        {
            System.err.println("Invalid randomSeed option");
            System.err.println(Usage);
            System.exit(1);
        }
        continue;
        }
        if (args[i].equals("-writeMaleDataset"))
        {
        writeMaleDataset = true;
        continue;
        }
        if (args[i].equals("-writeFemaleDataset"))
        {
        writeFemaleDataset = true;
        continue;
        }
        if (args[i].equals("-verbose"))
        {
        i++;
        if (i >= args.length)
        {
            System.err.println("Invalid verbose option");
            System.err.println(Usage);
            System.exit(1);
        }
        if (args[i].equals("true"))
        {
            Verbose = true;
        }
        else if (args[i].equals("false"))
        {
            Verbose = false;
        }
        else
        {
            System.err.println("Invalid verbose option");
            System.err.println(Usage);
            System.exit(1);
        }
        continue;
        }
        if (args[i].equals("-version"))
        {
        System.out.println("Nesting birds version = " + VERSION);
        System.exit(0);
        }
        if (args[i].equals("-help") || args[i].equals("-h") || args[i].equals("-?"))
        {
        System.out.println(Usage);
        System.exit(0);
        }
        System.err.println(Usage);
        System.exit(1);
    }
    if (steps == -1)
    {
        System.err.println(Usage);
        System.exit(1);
    }
    if (Male::INITIAL_FOOD > Male::FOOD_DURATION)
    {
        System.err.println("Male initial food cannot be greater than food duration");
        System.err.println(Usage);
        System.exit(1);
    }
    if (Female::INITIAL_FOOD > Female::FOOD_DURATION)
    {
        System.err.println("Female initial food cannot be greater than food duration");
        System.err.println(Usage);
        System.exit(1);
    }

    // Run birds.
    for (int run = 0; run < runs; run++)
    {
        if (Verbose) { printf("Run=%d\n", run); }
        int          eggLaidStep  = -1;
        for (int step = 0; step < steps; step++)
        {
        if (Verbose) { System.out.println("Step=" + step); }
        nestingbirds.step();
        if ((eggLaidStep < 0) && (nestingbirds.World[WIDTH / 2][HEIGHT / 2].object == OBJECT::EGG))
        {
            eggLaidStep = step;
        }
        }

        if (Verbose)
        {
        System.out.print("Run results: ");
        if (eggLaidStep >= 0)
        {
            System.out.print("egg laid at step " + eggLaidStep);
        }
        else
        {
            System.out.print("no egg");
        }
        int mouseCount = 0;
        int stoneCount = 0;
        for (int x = 0; x < WIDTH; x++)
        {
            for (int y = 0; y < HEIGHT; y++)
            {
                if ((nestingbirds.World[x][y].locale == LOCALE::FOREST) &&
                    (nestingbirds.World[x][y].object == OBJECT::MOUSE))
                {
                    mouseCount++;
                }
                if ((nestingbirds.World[x][y].locale == LOCALE::DESERT) &&
                    (nestingbirds.World[x][y].object == OBJECT::STONE))
                {
                    stoneCount++;
                }
            }
        }
        System.out.println(", remaining mice=" + mouseCount + ", remaining stones=" + stoneCount);
        }
    }
}

