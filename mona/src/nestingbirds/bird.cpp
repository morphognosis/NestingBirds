// For conditions of distribution and use, see copyright notice in LICENSE.txt

// Nesting bird.

#include "bird.hpp"

// Constructor.
Bird::Bird(int gender)
{
    this->gender = gender;
    x           = y = 0;
    orientation = ORIENTATION::NORTH;
    food        = 0;
    hasObject   = OBJECT::NO_OBJECT;
}

// Digest food.
void Bird::digest()
{
    if (food > 0) { food--; } else{ food = 0; }
}

// Print state.
void Bird::printState()
{
    printf(stateToString().c_str());
    printf("\n");
}

// State to string.
string Bird::stateToString()
{
    string s = "Orientation: ";
    s += ORIENTATION::toString(orientation);
    s += ", Food: " + food;
    s += ", Has_object: ";
    s += OBJECT::toString(hasObject);
}
