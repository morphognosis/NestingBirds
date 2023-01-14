// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef __LOCALE__
#define __LOCALE__

// Locale types.
class LOCALE
{
public:
   static const int DESERT = 0;
   static const int FOREST = 1;
   static const int PLAIN  = 2;

   // Locale to string.
   static char *toString(int locale)
   {
      switch (locale)
      {
      case DESERT:
         return("DESERT");

      case FOREST:
         return("FOREST");

      case PLAIN:
         return("PLAIN");

      default:
         return("Unknown locale");
      }
   }
};
#endif
