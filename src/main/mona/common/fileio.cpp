// Load and save in binary or readable format.

#include "fileio.h"

// Get path to resource file.
// Application responsible for freeing malloc'd memory.
char *getResourcePath(char *file)
{
   return(getPath((char *)"resource", file));
}


// Get path to data file.
// Application responsible for freeing malloc'd memory.
char *getDataPath(char *file)
{
   return(getPath((char *)"data", file));
}


// Get path to file within directory.
// Application responsible for freeing malloc'd memory.
char *getPath(char *dir, char *file)
{
   char *home, *path, *path2;

   // Sanity check.
   if ((file == NULL) || (*file == '\0'))
   {
      return(NULL);
   }

   // Fixed path?
   if ((file[0] == '/') || (file[0] == '.'))
   {
      if ((path = (char *)malloc(strlen(file) + 1)) == NULL)
      {
         fprintf(stderr, "getPath: cannot malloc path memory\n");
         exit(1);
      }
      strcpy(path, file);
      return(path);
   }

   // Check HOME directory path environment variable.
   if ((home = getenv(ENV_HOME)) != NULL)
   {
#ifdef WIN32
      // Replace Cygwin path with Windows path.
      if ((strncmp(home, "/cygdrive/", 10) == 0) && (strlen(home) >= 11))
      {
         if ((path = (char *)malloc(strlen(home) + strlen(dir) + 2)) == NULL)
         {
            fprintf(stderr, "getPath: cannot malloc path memory\n");
            exit(1);
         }
         sprintf(path, "c:%s/%s", &home[11], dir);
      }
      else
      {
         if ((path = (char *)malloc(strlen(home) + strlen(dir) + 2)) == NULL)
         {
            fprintf(stderr, "getPath: cannot malloc path memory\n");
            exit(1);
         }
         sprintf(path, "%s/%s", home, dir);
      }
      if (GetFileAttributes(path) != 0xffffffff)
#else
      if ((path = (char *)malloc(strlen(home) + strlen(dir) + 2)) == NULL)
      {
         fprintf(stderr, "getPath: cannot malloc path memory\n");
         exit(1);
      }
      sprintf(path, "%s/%s", home, dir);
      if (access(path, F_OK) != -1)
#endif
      {
         // Add the file.
         if ((path2 = (char *)malloc(strlen(path) + strlen(file) + 2)) == NULL)
         {
            fprintf(stderr, "getPath: cannot malloc path memory\n");
            exit(1);
         }
         sprintf(path2, "%s/%s", path, file);
         free(path);
         return(path2);
      }
      else
      {
         free(path);
      }
   }

   // Try relative paths.
   if ((path = (char *)malloc(strlen(dir) + 1)) == NULL)
   {
      fprintf(stderr, "getPath: cannot malloc path memory\n");
      exit(1);
   }
   sprintf(path, "%s", dir);
#ifdef WIN32
   if (GetFileAttributes(path) != 0xffffffff)
#else
   if (access(path, F_OK) != -1)
#endif
   {
      free(path);
      if ((path = (char *)malloc(strlen(dir) + strlen(file) + 2)) == NULL)
      {
         fprintf(stderr, "getPath: cannot malloc path memory\n");
         exit(1);
      }
      sprintf(path, "%s/%s", dir, file);
      return(path);
   }
   else
   {
      free(path);
   }
   if ((path = (char *)malloc(strlen(dir) + 4)) == NULL)
   {
      fprintf(stderr, "getPath: cannot malloc path memory\n");
      exit(1);
   }
   sprintf(path, "../%s", dir);
#ifdef WIN32
   if (GetFileAttributes(path) != 0xffffffff)
#else
   if (access(path, F_OK) != -1)
#endif
   {
      free(path);
      if ((path = (char *)malloc(strlen(dir) + strlen(file) + 5)) == NULL)
      {
         fprintf(stderr, "getPath: cannot malloc path memory\n");
         exit(1);
      }
      sprintf(path, "../%s/%s", dir, file);
      return(path);
   }
   else
   {
      free(path);
   }
   if ((path = (char *)malloc(strlen(dir) + 7)) == NULL)
   {
      fprintf(stderr, "getPath: cannot malloc path memory\n");
      exit(1);
   }
   sprintf(path, "../../%s", dir);
#ifdef WIN32
   if (GetFileAttributes(path) != 0xffffffff)
#else
   if (access(path, F_OK) != -1)
#endif
   {
      free(path);
      if ((path = (char *)malloc(strlen(dir) + strlen(file) + 8)) == NULL)
      {
         fprintf(stderr, "getPath: cannot malloc path memory\n");
         exit(1);
      }
      sprintf(path, "../../%s/%s", dir, file);
      return(path);
   }
   else
   {
      free(path);
   }

   // Default to input file.
   if ((path = (char *)malloc(strlen(file) + 1)) == NULL)
   {
      fprintf(stderr, "getPath: cannot malloc path memory\n");
      exit(1);
   }
   strcpy(path, file);
   return(path);
}


FILE *myfopenRead(char *filename)
{
   return(fopen(filename, "rb"));
}


FILE *myfopenWrite(char *filename)
{
   return(fopen(filename, "wb"));
}


int myfclose(FILE *fp)
{
   return(fclose(fp));
}


int myfreadInt(int *i, FILE *fp)
{
   return(fscanf(fp, "%d", i));
}


int myfwriteInt(int *i, FILE *fp)
{
   return(fprintf(fp, "%d\n", *i));
}


int myfreadShort(short *s, FILE *fp)
{
   short v;
   int   ret = fscanf(fp, "%hd", &v);

   *s = v;
   return(ret);
}


int myfwriteShort(short *s, FILE *fp)
{
   short v;

   v = *s;
   return(fprintf(fp, "%hd\n", v));
}


int myfreadLong(unsigned long *l, FILE *fp)
{
   return(fscanf(fp, "%lu", l));
}


int myfwriteLong(unsigned long *l, FILE *fp)
{
   return(fprintf(fp, "%lu\n", *l));
}


int myfreadLongLong(unsigned long long *l, FILE *fp)
{
   return(fscanf(fp, "%llu", l));
}


int myfwriteLongLong(unsigned long long *l, FILE *fp)
{
   return(fprintf(fp, "%llu\n", *l));
}


int myfreadFloat(float *f, FILE *fp)
{
   char buf[100];
   int  ret = fscanf(fp, "%s", buf);

   *f = (float)atof(buf);
   return(ret);
}


int myfwriteFloat(float *f, FILE *fp)
{
   return(fprintf(fp, "%f\n", *f));
}


int myfreadDouble(double *d, FILE *fp)
{
   char buf[100];
   int  ret = fscanf(fp, "%s", buf);

   *d = strtod(buf, NULL);
   return(ret);
}


int myfwriteDouble(double *d, FILE *fp)
{
   return(fprintf(fp, "%f\n", *d));
}


int myfreadBool(bool *b, FILE *fp)
{
   int v;
   int ret = fscanf(fp, "%d", &v);

   if (v == 1)
   {
      *b = true;
   }
   else
   {
      *b = false;
   }
   return(ret);
}


int myfwriteBool(bool *b, FILE *fp)
{
   if (*b)
   {
      return(fprintf(fp, "1\n"));
   }
   else
   {
      return(fprintf(fp, "0\n"));
   }
}


int myfreadChar(unsigned char *c, FILE *fp)
{
   char buf[10];
   int  ret = fscanf(fp, "%s", buf);

   *c = buf[0];
   return(ret);
}


int myfwriteChar(unsigned char *c, FILE *fp)
{
   return(fprintf(fp, "%c\n", *c));
}


int myfreadBytes(unsigned char *bytes, int size, FILE *fp)
{
   int           len  = (2 * size) + 1;
   unsigned char *buf = new unsigned char[len];

   assert(buf != NULL);
   int ret = fscanf(fp, "%s", buf);
   int i, j, d1, d2;
   for (i = 0; i < size; i++)
   {
      j = 2 * i;
      if ((buf[j] >= '0') && (buf[j] <= '9'))
      {
         d1 = buf[j] - '0';
      }
      else
      {
         d1 = buf[j] - 'a' + 10;
      }
      j++;
      if ((buf[j] >= '0') && (buf[j] <= '9'))
      {
         d2 = buf[j] - '0';
      }
      else
      {
         d2 = buf[j] - 'a' + 10;
      }
      bytes[i] = (d1 * 16) + d2;
   }
   delete [] buf;
   return(ret);
}


int myfwriteBytes(unsigned char *bytes, int size, FILE *fp)
{
   int  len  = (2 * size) + 1;
   char *buf = new char[len];

   assert(buf != NULL);
   for (int i = 0; i < size; i++)
   {
      sprintf(&buf[2 * i], "%02x", bytes[i]);
   }
   buf[len - 1] = '\0';
   int ret = fprintf(fp, "%s\n", buf);
   delete [] buf;
   return(ret);
}


int myfreadString(char *str, int size, FILE *fp)
{
   // String is delimited by double quotes.
   char c;

   while (true)
   {
      c = fgetc(fp);
      if (c == EOF)
      {
         return(0);
      }
      if (c == '"')
      {
         break;
      }
   }
   int i = 0;
   for ( ; i < size; i++)
   {
      c = fgetc(fp);
      if (c == EOF)
      {
         return(i);
      }
      if (c == '"')
      {
         str[i] = '\0';
         return(i);
      }
      else
      {
         str[i] = c;
      }
   }
   fgetc(fp);
   return(i);
}


int myfwriteString(char *str, int size, FILE *fp)
{
   // Delimit string with double quotes.
   fputc('"', fp);
   int ret = 0;
   for (int i = 0; i < size && str[i] != '\0'; i++)
   {
      if (str[i] != '"')
      {
         fputc(str[i], fp);
         ret++;
      }
   }
   fputc('"', fp);
   fputc('\n', fp);
   return(ret);
}
