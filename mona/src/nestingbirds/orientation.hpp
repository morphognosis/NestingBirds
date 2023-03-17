// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __ORIENTATION__
#define __ORIENTATION__

#include <string>

// Orientation.
class ORIENTATION
{
public:
    static const int NORTH = 0;
    static const int SOUTH = 1;
    static const int EAST  = 2;
    static const int WEST  = 3;

    // Orientation to string.
    static const char *toString(int orientation)
    {
       switch (orientation)
       {
       case NORTH:
          return("NORTH");

       case SOUTH:
          return("SOUTH");

       case EAST:
          return("EAST");

       case WEST:
          return("WEST");

       default:
          return("Unknown orientation");
       }
    }
 };
#endif
