// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __OBJECT__
#define __OBJECT__

#include <string>

// Object types.
class OBJECT
{
public:
   static const int NO_OBJECT = 0;
   static const int EGG       = 1;
   static const int MOUSE     = 2;
   static const int STONE     = 3;

   // Object to string.
   static char *toString(int object)
   {
      switch (object)
      {
      case NO_OBJECT:
         return("NO_OBJECT");

      case EGG:
         return("EGG");

      case MOUSE:
         return("MOUSE");

      case STONE:
         return("STONE");

      default:
         return("Unknown object");
      }
   }
};
#endif
