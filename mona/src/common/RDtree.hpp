/*
 * Copyright (c) 2011 Tom Portegys (portegys@gmail.com). All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY TOM PORTEGYS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * Relative distance tree for storing and searching multidimensional patterns.
 *
 * Reference:
 * A Search Technique for Pattern Recognition Using Relative Distances
 * Thomas E. Portegys
 * IEEE TRANSACTIONS ON PATTERN ANALYSIS AND MACHINE INTELLIGENCE,
 * VOL. 17, NO. 9, SEPTEMBER 1995
 *
 * Abstract:
 * A technique for creating and searching a tree of patterns
 * using relative distances is presented. The search is conducted to find patterns
 * which are nearest neighbors of a given test pattern. The structure
 * of the tree is such that the search time is proportional to the distance between
 * the test pattern and its nearest neighbor, since a larger tree, which can be 
 * expected on average to contain closer neighbors, can be searched faster than a
 * smaller tree.
 */

#ifndef __PATTREE__
#define __PATTREE__

#ifdef _WIN32
#ifndef WIN32
#define WIN32
#endif
#endif

#include <stdio.h>
#include <vector>
using namespace std;

#ifndef PUBLIC_API
#ifdef WIN32
#define PUBLIC_API    __declspec(dllexport)
#pragma warning( disable: 4251 )
#else
#define PUBLIC_API
#endif
#endif

// Mona: sensory/response, neural network, and needs.
class PUBLIC_API RDtree
{
public:

   // Tree configuration parameter.
   static const float DEFAULT_RADIUS;
   float              RADIUS;

   // Tree node.
   class RDnode
   {
public:
      void *pattern;           /* pattern value */
      void *client;            /* client link */
private:
      RDnode *childlist;       /* child pattern list */
      RDnode *childlast;       /* last child */
      RDnode *sibnext;         /* next sibling */
      RDnode *sibback;         /* previous sibling */
      float  distance;         /* distance from child to parent */
public:
      RDnode(void *pattern, void *client);
      RDnode();
      friend class RDtree;
   };

   // Search element.
   class RDsearch
   {
public:
      RDnode   *node;           /* node */
      float    distance;        /* comparison distance */
      RDsearch *srchnext;       /* next on search return list */
private:
      float    workdist;        /* work distance */
      int      state;           /* search state */
      RDsearch *childlist;      /* children */
      RDsearch *sibnext;        /* next sibling */
      RDsearch *sibback;        /* previous sibling */
public:
      RDsearch();
      friend class RDtree;
   };

   // Constructors.
   RDtree(float (*distFunc)(void *, void *), void (*delFunc)(void *) = NULL);
   RDtree(float radius, float (*distFunc)(void *, void *), void (*delFunc)(void *) = NULL);

   // Destructor.
   ~RDtree();
   void deleteSubtree(RDnode *);

   // Insert, remove, and search.
   void insert(void *pattern, void *client);
   void remove(void *pattern);
   RDsearch *search(void *pattern, int maxFind = 1, int maxSearch = (-1));

   // Load and save tree.
   bool load(char *filename, void *(*loadPatt)(FILE * fp),
             void *(*loadClient)(FILE * fp) = NULL);
   void load(FILE * fp, void *(*loadPatt)(FILE * fp),
             void *(*loadClient)(FILE * fp) = NULL);
   bool load(char *filename, void *helper,
             void *(*loadPatt)(void *helper, FILE * fp),
             void *(*loadClient)(void *helper, FILE * fp) = NULL);
   void load(FILE * fp, void *helper,
             void *(*loadPatt)(void *helper, FILE * fp),
             void *(*loadClient)(void *helper, FILE * fp) = NULL);
   bool save(char *filename, void (*savePatt)(void *pattern, FILE *fp),
             void (*saveClient)(void *client, FILE *fp) = NULL);
   void save(FILE * fp, void (*savePatt)(void *pattern, FILE *fp),
             void (*saveClient)(void *client, FILE *fp) = NULL);

   // Print.
   bool print(char *filename, void (*printPatt)(void *pattern, FILE *fp));
   void print(void (*printPatt)(void *pattern, FILE *fp), FILE * fp = stdout);

private:

   // Tree root.
   RDnode *root;

   // Pattern distance function.
   float (*distFunc)(void *pattern0, void *pattern1);

   // Pattern delete function.
   void (*delFunc)(void *pattern);

   // Search states.
   enum
   {
      DISTPENDING=0,                        /* distance pending */
      DISTDONE   =1,                        /* distance computed */
      EXPANDED   =2,                        /* pattern expanded */
      SRCHDONE   =3                         /* pattern searched */
   };

   static RDsearch *EMPTY;

   // Search stack.
   enum { STKMEM_QUANTUM=32 };              /* stack malloc increment */
   int stkMem;
   struct SrchStk
   {
      RDsearch *currsrch;
      RDsearch *child;
      RDsearch *childnext;
   };

   // Search work memory.
   enum { SRCHWORKMEM_QUANTUM=128 };         /* search work memory size */

   // Search control.
   struct SrchCtl
   {
      RDsearch           *srchList;          /* search results */
      RDsearch           *srchBest;          /* best search result */
      int                maxFind;            /* max number of results */
      int                maxSearch;          /* max nodes to search (-1=unlimited) */
      int                searchCount;        /* search counter */
      int                bestSearch;         /* best result search counter */
      struct SrchStk     *srchStk;           /* search stack */
      int                srchStkIdx;         /* stack index */
      int                srchStkSz;          /* current stack size */
      vector<RDsearch *> srchWork;           /* search work space */
      int                srchWorkIdx;        /* search work index */
      int                srchWorkUse;        /* used search work */
   };

   // Internal functions.
   void insert(RDnode *current, RDnode *node);
   void search(struct SrchCtl *srchCtl, RDnode *srchNode);
   void foundPatt(struct SrchCtl *srchCtl, RDsearch *swfound,
                  int *numFind, RDsearch **swcut);
   RDsearch *getSrchWork(struct SrchCtl *srchCtl);

   void loadChildren(FILE * fp, RDnode * parent,
                     void *(*loadPatt)(FILE * fp),
                     void *(*loadClient)(FILE * fp));
   void loadChildren(FILE * fp, void *helper, RDnode * parent,
                     void *(*loadPatt)(void *helper, FILE * fp),
                     void *(*loadClient)(void *helper, FILE * fp));
   void saveChildren(FILE * fp, RDnode * parent,
                     void (*savePatt)(void *pattern, FILE *fp),
                     void (*saveClient)(void *client, FILE *fp));
   void printNode(FILE * fp, RDnode * node, int level,
                  void (*printPatt)(void *pattern, FILE *fp));
};
#endif
