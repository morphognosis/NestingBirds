// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __DATASETS__
#define __DATASETS__

#include <string>
#include <vector>
using namespace std;

/*
Create training and testing dataset files.
Format:
X_train_shape = [<number of sequences>, <sequence length>, <input dimension>]
X_train_seq = [ ... ]
y_train_shape = [<number of sequences>, <sequence length>, <output dimension>]
y_train_seq = [ ... ]
X_test_shape = [<number of sequences>, <sequence length>, <input dimension>]
X_test_seq = [ ... ]
y_test_shape = [<number of sequences>, <sequence length>, <output dimension>]
y_test_seq = [ ... ]
*/

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


    // Convert response string to number.
    int responseToInt()
    {
        if (response == "DO_NOTHING")
        {
            return 0;
        }
        else if (response == "MOVE_FORWARD")
        {
            return 1;
        }
        else if (response == "TURN_RIGHT")
        {
            return 2;
        }
        else if (response == "TURN_LEFT")
        {
            return 3;
        }
        else if (response == "EAT_MOUSE")
        {
            return 4;
        }
        else if (response == "GET_OBJECT")
        {
            return 5;
        }
        else if (response == "PUT_OBJECT")
        {
            return 6;
        }
        else if (response == "TOSS_OBJECT")
        {
            return 7;
        }
        else if (response == "GIVE_MOUSE")
        {
            return 8;
        }
        else if (response == "GIVE_STONE")
        {
            return 9;
        }
        else if (response == "FLY")
        {
            return 10;
        }
        else if (response == "ALIGHT")
        {
            return 11;
        }
        return(-1);
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

    // Convert response string to number.
    int responseToInt()
    {
        if (response == "DO_NOTHING")
        {
            return 0;
        }
        else if (response == "MOVE_FORWARD")
        {
            return 1;
        }
        else if (response == "TURN_RIGHT")
        {
            return 2;
        }
        else if (response == "TURN_LEFT")
        {
            return 3;
        }
        else if (response == "EAT_MOUSE")
        {
            return 4;
        }
        else if (response == "GET_OBJECT")
        {
            return 5;
        }
        else if (response == "PUT_OBJECT")
        {
            return 6;
        }
        else if (response == "TOSS_OBJECT")
        {
            return 7;
        }
        else if (response == "WANT_MOUSE")
        {
            return 8;
        }
        else if (response == "WANT_STONE")
        {
            return 9;
        }
        else if (response == "LAY_EGG")
        {
           return 10;
        }
        return(-1);
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

// Behavior.
extern vector < vector < MaleSensoryResponse >> MaleTrainBehavior;
extern vector<MaleSensoryResponse> MaleTestBehavior;
extern vector < vector < FemaleSensoryResponse >> FemaleTrainBehavior;
extern vector<FemaleSensoryResponse> FemaleTestBehavior;

// Write datasets.
// If testRandomSeed == -1, do not write test dataset.
void writeDatasets(int steps, vector<int> trainRandomSeeds, int testRandomSeed, 
    string maleDatasetName, string femaleDatasetName, bool verbose);

// Write pattern datasets.
void writePatternDatasets(int steps, vector<int> randomSeeds,
    string maleDatasetName, string femaleDatasetName, bool verbose);

#endif
