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

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "RDtree.hpp"
#include "fileio.h"

// Tree configuration parameter.
const float RDtree::DEFAULT_RADIUS = 100.0f;

RDtree::RDsearch *RDtree::EMPTY = (RDsearch *)(-1);

// Node constructors.
RDtree::RDnode::RDnode(void *pattern, void *client)
{
   this->pattern = pattern;
   this->client  = client;
   childlist     = childlast = NULL;
   sibnext       = sibback = NULL;
   distance      = 0.0f;
}


RDtree::RDnode::RDnode()
{
   pattern   = NULL;
   client    = NULL;
   childlist = childlast = NULL;
   sibnext   = sibback = NULL;
   distance  = 0.0f;
}


// Search element constructor.
RDtree::RDsearch::RDsearch()
{
   node      = NULL;
   distance  = 0.0f;
   srchnext  = NULL;
   workdist  = 0.0f;
   state     = DISTPENDING;
   childlist = sibnext = sibback = NULL;
}


// Constructors.
RDtree::RDtree(float(*distFunc)(void *, void *), void(*delFunc)(void *))
{
   RADIUS         = DEFAULT_RADIUS;
   this->distFunc = distFunc;
   this->delFunc  = delFunc;
   root           = NULL;
   stkMem         = STKMEM_QUANTUM;
}


RDtree::RDtree(float radius, float(*distFunc)(void *, void *),
               void(*delFunc)(void *))
{
   RADIUS         = radius;
   this->distFunc = distFunc;
   this->delFunc  = delFunc;
   root           = NULL;
   stkMem         = STKMEM_QUANTUM;
}


// Destructor.
RDtree::~RDtree()
{
   deleteSubtree(root);
}


// Delete subtree.
void RDtree::deleteSubtree(RDnode *node)
{
   RDnode *p, *p2, *p3;

   /* convert tree to list */
   if ((p = node) == NULL)
   {
      return;
   }
   for (p2 = p; p2 != NULL; p2 = p2->sibnext)
   {
      if (p2->childlist != NULL)
      {
         p3                   = p2->sibnext;
         p2->sibnext          = p2->childlist;
         p2->sibnext->sibback = p2;
         if (p3 != NULL)
         {
            p3->sibback = p2->childlast;
         }
         p2->childlast->sibnext = p3;
         p2->childlist          = p2->childlast = NULL;
      }
   }

   /* delete list */
   for (p2 = p; p2 != NULL; p2 = p3)
   {
      p3 = p2->sibnext;
      if ((p2->pattern != NULL) && (delFunc != NULL))
      {
         delFunc(p2->pattern);
      }
      delete p2;
   }
}


// Insert pattern.
void RDtree::insert(void *pattern, void *client)
{
   RDnode *node = new RDnode(pattern, client);

   assert(node != NULL);
   insert(root, node);
}


void RDtree::insert(RDnode *current, RDnode *node)
{
   RDnode *p, *p2, *p3;
   float  dcn, dnn;

   /* clear node */
   node->childlist = NULL;
   node->childlast = NULL;
   node->sibnext   = NULL;
   node->sibback   = NULL;
   node->distance  = 0.0f;

   /* new root? */
   if (current == NULL)
   {
      root = node;
      return;
   }

   /* add pattern to first acceptable branch */
   dcn = distFunc(current->pattern, node->pattern);
   while (1)
   {
      for (p = current->childlist; p != NULL; p = p->sibnext)
      {
         /* check relative distances */
         dnn = distFunc(p->pattern, node->pattern);
         if (dnn <= (p->distance * RADIUS))
         {
            /* change current fragment */
            current = p;
            dcn     = dnn;
            break;
         }
      }
      if (p == NULL)
      {
         break;
      }
   }

   /* link new as child of current pattern */
   node->distance = dcn;
   node->sibnext  = NULL;
   node->sibback  = current->childlast;
   if (current->childlast != NULL)
   {
      current->childlast->sibnext = node;
   }
   else
   {
      current->childlist = node;
   }
   current->childlast = node;

   /*
    * check if previously added patterns should be un-linked from the
    * current pattern and linked as children of the new pattern.
    */
   for (p = current->childlist; p != node && p != NULL; )
   {
      dnn = distFunc(p->pattern, node->pattern);

      /* if should be linked to new pattern */
      if (dnn <= (node->distance * RADIUS))
      {
         /* re-link children */
         p2 = p->sibnext;
         if (p->sibback != NULL)
         {
            p->sibback->sibnext = p2;
         }
         else
         {
            current->childlist = p2;
         }
         if (p2 != NULL)
         {
            p2->sibback = p->sibback;
         }

         /* convert child sub-tree to list */
         for (p2 = p3 = p, p->sibnext = NULL; ; p3 = p3->sibnext)
         {
            for ( ; p2->sibnext != NULL; p2 = p2->sibnext)
            {
            }
            for ( ; p3 != NULL && p3->childlist == NULL; p3 = p3->sibnext)
            {
            }
            if (p3 == NULL)
            {
               break;
            }
            p2->sibnext = p3->childlist;
         }

         /* add list to current pattern */
         for (p2 = p; p2 != NULL; p2 = p3)
         {
            p3 = p2->sibnext;
            insert(current, p2);
         }

         /* restart check (since child configuration may have changed) */
         for (p = current->childlist; p != node && p != NULL; p = p->sibnext)
         {
         }
         if (p == NULL)
         {
            break;
         }
         p = current->childlist;
      }
      else
      {
         p = p->sibnext;
      }
   }
}


// Remove pattern.
void RDtree::remove(void *pattern)
{
   RDnode *current, *node, *p, *p2, *p3;
   float  d;

   if ((current = node = root) == NULL)
   {
      return;
   }
   d = distFunc(current->pattern, pattern);
   while (d > 0.0f)
   {
      for (node = current->childlist; node != NULL; node = node->sibnext)
      {
         d = distFunc(node->pattern, pattern);
         if (d <= (node->distance * RADIUS))
         {
            if (d > 0.0f)
            {
               current = node;
            }
            break;
         }
      }
      if (node == NULL)
      {
         return;
      }
   }

   /* unlink pattern */
   if (node == root)
   {
      current = root = NULL;
   }
   else
   {
      if (node->sibback == NULL)
      {
         current->childlist = node->sibnext;
      }
      else
      {
         node->sibback->sibnext = node->sibnext;
      }
      if (node->sibnext == NULL)
      {
         current->childlast = node->sibback;
      }
      else
      {
         node->sibnext->sibback = node->sibback;
      }
   }
   node->sibnext = node->sibback = NULL;

   /* convert child sub-tree to list */
   p = node->childlist;
   for (p2 = p; p2 != NULL; p2 = p2->sibnext)
   {
      if (p2->childlist != NULL)
      {
         p3                   = p2->sibnext;
         p2->sibnext          = p2->childlist;
         p2->sibnext->sibback = p2;
         if (p3 != NULL)
         {
            p3->sibback = p2->childlast;
         }
         p2->childlast->sibnext = p3;
         p2->childlist          = p2->childlast = NULL;
      }
   }

   /* add list to parent pattern */
   for (p2 = p; p2 != NULL; p2 = p3)
   {
      p3 = p2->sibnext;
      insert(current, p2);
      if (current == NULL)
      {
         current = root;
      }
   }

   // delete node.
   delete node;
}


/* search space for patterns closest to the given pattern */
/* return best matches */
RDtree::RDsearch *RDtree::search(void *pattern, int maxFind, int maxSearch)
{
   RDsearch *sw, *sw2, *sw3;
   RDsearch *srchList = NULL;

   if (root == NULL)
   {
      return(srchList);
   }

   /* prepare for search */
   struct SrchCtl srchCtl;
   srchCtl.srchList    = NULL;
   srchCtl.srchBest    = NULL;
   srchCtl.maxFind     = maxFind;
   srchCtl.maxSearch   = maxSearch;
   srchCtl.searchCount = srchCtl.bestSearch = 0;
   srchCtl.srchStkSz   = stkMem;
   srchCtl.srchStk     = (struct SrchStk *)malloc(srchCtl.srchStkSz * sizeof(struct SrchStk));
   assert(srchCtl.srchStk != NULL);
   srchCtl.srchStkIdx = 0;
   srchCtl.srchWork.push_back((RDsearch *)malloc(SRCHWORKMEM_QUANTUM * sizeof(RDsearch)));
   assert(srchCtl.srchWork[0] != NULL);
   memset(srchCtl.srchWork[0], 0, SRCHWORKMEM_QUANTUM * sizeof(RDsearch));
   srchCtl.srchWorkIdx = 0;
   srchCtl.srchWorkUse = 0;
   RDnode node(pattern, NULL);

   /* search tree */
   search(&srchCtl, &node);

   /* extract search results */
   for (sw = srchCtl.srchList, sw3 = NULL; sw != NULL; sw = sw->srchnext)
   {
      sw2 = new RDsearch();
      assert(sw2 != NULL);
      sw2->node     = sw->node;
      sw2->distance = sw->distance;
      if (sw3 == NULL)
      {
         srchList = sw2;
      }
      else
      {
         sw3->srchnext = sw2;
      }
      sw3 = sw2;
   }

   /* free search memory */
   free(srchCtl.srchStk);
   for (int i = 0, j = (int)srchCtl.srchWork.size(); i < j; i++)
   {
      free(srchCtl.srchWork[i]);
   }

   return(srchList);
}


/* search space for patterns closest to the given pattern */
/* put best matches on srchList */
void RDtree::search(struct SrchCtl *srchCtl, RDtree::RDnode *srchNode)
{
   int            numSearch, stkIdx;
   struct SrchStk *stkp, *stkp2;
   RDnode         *p;
   RDsearch       *sw, *sw2, *bsw, *bsw2;
   RDsearch       *swcut;
   int            numFind;

   /* initialize */
   swcut     = NULL;
   numSearch = numFind = 0;
   if ((srchCtl->maxSearch >= 0) && (numSearch >= srchCtl->maxSearch))
   {
      return;
   }
   if (root == NULL)
   {
      return;
   }
   stkp                     = &(srchCtl->srchStk[srchCtl->srchStkIdx]);
   stkp->currsrch           = getSrchWork(srchCtl);
   stkp->currsrch->node     = root;
   stkp->currsrch->distance = distFunc(stkp->currsrch->node->pattern, srchNode->pattern);
   stkp->currsrch->state    = DISTDONE;
   foundPatt(srchCtl, stkp->currsrch, &numFind, &swcut);
   numSearch++;
   if ((srchCtl->maxSearch >= 0) && (numSearch >= srchCtl->maxSearch))
   {
      return;
   }

   /* for each level of recursion */
   while (srchCtl->srchStkIdx >= 0)
   {
      /* find best and next best distances for current search branch */
      for (stkIdx = srchCtl->srchStkIdx; stkIdx >= 0; stkIdx--)
      {
         stkp = &(srchCtl->srchStk[stkIdx]);

         /* expand pattern? */
         if (stkp->currsrch->state == DISTDONE)
         {
            for (p = stkp->currsrch->node->childlist, sw2 = NULL; p != NULL; p = p->sibnext)
            {
               sw        = getSrchWork(srchCtl);
               sw->node  = p;
               sw->state = DISTPENDING;
               if (sw2 == NULL)
               {
                  stkp->currsrch->childlist = sw;
               }
               else
               {
                  sw2->sibnext = sw;
                  sw->sibback  = sw2;
               }
               sw2 = sw;
            }
            stkp->child           = stkp->childnext = NULL;
            stkp->currsrch->state = EXPANDED;
         }

         /* best and next best distances must be (re)computed? */
         if ((stkp->child == NULL) ||
             ((stkp->child != NULL) &&
              (stkp->child->workdist > 0.0f) &&
              (stkp->childnext == EMPTY)) ||
             ((stkp->child != NULL) &&
              (stkp->childnext != NULL) &&
              (stkp->childnext != EMPTY) &&
              (stkp->child->workdist > stkp->childnext->workdist)))
         {
            bsw = bsw2 = NULL;
            for (sw = stkp->currsrch->childlist; sw != NULL; sw = sw2)
            {
               sw2 = sw->sibnext;

               /* have best possible child? */
               if ((bsw != NULL) && (bsw->workdist == 0.0f))
               {
                  if (sw2 == NULL)
                  {
                     bsw2 = NULL;
                  }
                  else
                  {
                     bsw2 = EMPTY;
                  }
                  break;
               }

               /* compute distance? */
               if (sw->state == DISTPENDING)
               {
                  sw->distance = distFunc(sw->node->pattern, srchNode->pattern);
                  if ((sw->workdist = sw->distance -
                                      (sw->node->distance * RADIUS)) < 0.0f)
                  {
                     sw->workdist = 0.0f;
                  }
                  sw->state = DISTDONE;

                  /* save pattern on return list */
                  foundPatt(srchCtl, sw, &numFind, &swcut);
                  numSearch++;

                  /* check for termination of search */
                  if ((srchCtl->maxSearch >= 0) && (numSearch >= srchCtl->maxSearch))
                  {
                     return;
                  }
               }

               /* cut off infeasible or finished branch */
               if ((sw->state == SRCHDONE) ||
                   ((swcut != NULL) && (sw->workdist >= swcut->distance)))
               {
                  /* cut off */
                  sw->state = SRCHDONE;
                  if (sw->sibback == NULL)
                  {
                     stkp->currsrch->childlist = sw->sibnext;
                  }
                  else
                  {
                     sw->sibback->sibnext = sw->sibnext;
                  }
                  if (sw->sibnext != NULL)
                  {
                     sw->sibnext->sibback = sw->sibback;
                  }
                  continue;
               }

               /* find best and next best child branches */
               if ((bsw == NULL) || (bsw->workdist > sw->workdist))
               {
                  bsw2 = bsw;
                  bsw  = sw;
                  continue;
               }
               if ((bsw2 == NULL) || (bsw2->workdist > sw->workdist))
               {
                  bsw2 = sw;
                  continue;
               }
            }

            /* change to better branch level? */
            if (stkp->child != bsw)
            {
               srchCtl->srchStkIdx = stkIdx;
            }
            stkp->child     = bsw;
            stkp->childnext = bsw2;
         }

         /* finished with this level? */
         if (stkp->child == NULL)
         {
            stkp->currsrch->state = SRCHDONE;
            srchCtl->srchStkIdx   = stkIdx - 1;
            if (stkIdx > 0)
            {
               (stkp - 1)->child = NULL;
            }
         }
         else
         {
            /* set new best distance for branch */
            for (stkp2 = stkp; stkp2 >= srchCtl->srchStk; stkp2--)
            {
               stkp2->currsrch->workdist = stkp->child->workdist;
            }
         }
      }

      /* expand search deeper */
      if (srchCtl->srchStkIdx >= 0)
      {
         srchCtl->srchStkIdx++;
         if (srchCtl->srchStkIdx == srchCtl->srchStkSz)
         {
            stkMem            += STKMEM_QUANTUM;
            srchCtl->srchStkSz = stkMem;
            srchCtl->srchStk   = (struct SrchStk *)realloc((void *)(srchCtl->srchStk), srchCtl->srchStkSz * sizeof(struct SrchStk));
            assert(srchCtl->srchStk != NULL);
         }
         stkp           = &(srchCtl->srchStk[srchCtl->srchStkIdx]);
         stkp->currsrch = (stkp - 1)->child;
         stkp->child    = stkp->childnext = NULL;
      }
   }
}


/* add pattern to the found list */
void RDtree::foundPatt(struct SrchCtl *srchCtl, RDsearch *swfound,
                       int *numFind, RDsearch **swcut)
{
   RDsearch *sw, *sw2;

   for (sw = srchCtl->srchList, sw2 = NULL; sw != NULL; sw2 = sw, sw = sw->srchnext)
   {
      if (sw->distance <= swfound->distance)
      {
         break;
      }
   }
   srchCtl->searchCount++;
   if (sw == NULL)
   {
      srchCtl->bestSearch = srchCtl->searchCount;
   }
   if (sw2 == NULL)
   {
      if (*numFind < srchCtl->maxFind)
      {
         (*numFind)++;
         swfound->srchnext = srchCtl->srchList;
         srchCtl->srchList = swfound;
      }
   }
   else
   {
      swfound->srchnext = sw2->srchnext;
      sw2->srchnext     = swfound;
      if (*numFind < srchCtl->maxFind)
      {
         (*numFind)++;
      }
      else
      {
         srchCtl->srchList = (srchCtl->srchList)->srchnext;
      }
   }
   if (*numFind == srchCtl->maxFind)
   {
      /* set cut off */
      *swcut = srchCtl->srchList;
   }
}


/* get a pattern search element */
RDtree::RDsearch *RDtree::getSrchWork(struct SrchCtl *srchCtl)
{
   RDsearch *sw;

   if (srchCtl->srchWorkUse == SRCHWORKMEM_QUANTUM)
   {
      srchCtl->srchWorkIdx++;
      srchCtl->srchWork.push_back((RDsearch *)malloc(SRCHWORKMEM_QUANTUM * sizeof(RDsearch)));
      assert(srchCtl->srchWork[srchCtl->srchWorkIdx] != NULL);
      memset(srchCtl->srchWork[srchCtl->srchWorkIdx], 0, SRCHWORKMEM_QUANTUM * sizeof(RDsearch));
      srchCtl->srchWorkUse = 0;
   }
   sw = &(srchCtl->srchWork[srchCtl->srchWorkIdx][srchCtl->srchWorkUse]);
   srchCtl->srchWorkUse++;

   return(sw);
}


/* load tree */
bool RDtree::load(char *filename, void *(*loadPatt)(FILE * fp),
                  void *(*loadClient)(FILE * fp))
{
   FILE *fp = FOPEN_READ(filename);

   if (fp == NULL)
   {
      return(false);
   }
   load(fp, loadPatt, loadClient);
   FCLOSE(fp);
   return(true);
}


/* load tree */
void RDtree::load(FILE *fp, void *(*loadPatt)(FILE * fp),
                  void *(*loadClient)(FILE * fp))
{
   int   n;
   float d;

   deleteSubtree(root);
   root = NULL;
   FREAD_INT(&n, fp);
   if (n == 1)
   {
      root = new RDnode();
      assert(root != NULL);
      root->pattern = loadPatt(fp);
      if (loadClient != NULL)
      {
         root->client = loadClient(fp);
      }
      FREAD_FLOAT(&d, fp);
      root->distance = d;
      loadChildren(fp, root, loadPatt, loadClient);
   }
}


/* load children */
void RDtree::loadChildren(FILE *fp, RDnode *parent,
                          void *(*loadPatt)(FILE * fp),
                          void *(*loadClient)(FILE * fp))
{
   int    n;
   float  d;
   RDnode *p, *p2;

   FREAD_INT(&n, fp);
   p2 = NULL;
   for (int i = 0; i < n; i++)
   {
      p = new RDnode();
      assert(p != NULL);
      p->pattern = loadPatt(fp);
      if (loadClient != NULL)
      {
         p->client = loadClient(fp);
      }
      FREAD_FLOAT(&d, fp);
      p->distance = d;
      if (i == 0)
      {
         parent->childlist = p;
      }
      parent->childlast = p;
      if (p2 != NULL)
      {
         p2->sibnext = p;
         p->sibback  = p2;
      }
      p2 = p;
      loadChildren(fp, p, loadPatt, loadClient);
   }
}


/* load tree with address resolver */
bool RDtree::load(char *filename, void *helper,
                  void *(*loadPatt)(void *helper, FILE * fp),
                  void *(*loadClient)(void *helper, FILE * fp))
{
   FILE *fp = FOPEN_READ(filename);

   if (fp == NULL)
   {
      return(false);
   }
   load(fp, helper, loadPatt, loadClient);
   FCLOSE(fp);
   return(true);
}


/* load tree with address resolver */
void RDtree::load(FILE *fp, void *helper,
                  void *(*loadPatt)(void *helper, FILE * fp),
                  void *(*loadClient)(void *helper, FILE * fp))
{
   int   n;
   float d;

   root = NULL;
   FREAD_INT(&n, fp);
   if (n == 1)
   {
      root = new RDnode();
      assert(root != NULL);
      root->pattern = loadPatt(helper, fp);
      if (loadClient != NULL)
      {
         root->client = loadClient(helper, fp);
      }
      FREAD_FLOAT(&d, fp);
      root->distance = d;
      loadChildren(fp, helper, root, loadPatt, loadClient);
   }
}


/* load children with address resolver */
void RDtree::loadChildren(FILE *fp, void *helper,
                          RDnode *parent,
                          void *(*loadPatt)(void *helper, FILE * fp),
                          void *(*loadClient)(void *helper, FILE * fp))
{
   int    n;
   float  d;
   RDnode *p, *p2;

   FREAD_INT(&n, fp);
   p2 = NULL;
   for (int i = 0; i < n; i++)
   {
      p = new RDnode();
      assert(p != NULL);
      p->pattern = loadPatt(helper, fp);
      if (loadClient != NULL)
      {
         p->client = loadClient(helper, fp);
      }
      FREAD_FLOAT(&d, fp);
      p->distance = d;
      if (i == 0)
      {
         parent->childlist = p;
      }
      parent->childlast = p;
      if (p2 != NULL)
      {
         p2->sibnext = p;
         p->sibback  = p2;
      }
      p2 = p;
      loadChildren(fp, helper, p, loadPatt, loadClient);
   }
}


/* save tree */
bool RDtree::save(char *filename, void (*savePatt)(void *pattern, FILE *fp),
                  void (*saveClient)(void *client, FILE *fp))
{
   FILE *fp = FOPEN_WRITE(filename);

   if (fp == NULL)
   {
      return(false);
   }
   save(fp, savePatt, saveClient);
   FCLOSE(fp);
   return(true);
}


/* save tree */
void RDtree::save(FILE *fp, void (*savePatt)(void *pattern, FILE *fp),
                  void (*saveClient)(void *client, FILE *fp))
{
   int   n;
   float d;

   if (root == NULL)
   {
      n = 0;
      FWRITE_INT(&n, fp);
   }
   else
   {
      n = 1;
      FWRITE_INT(&n, fp);
      savePatt(root->pattern, fp);
      if (saveClient != NULL)
      {
         saveClient(root->client, fp);
      }
      d = root->distance;
      FWRITE_FLOAT(&d, fp);
      saveChildren(fp, root, savePatt, saveClient);
   }
}


/* save children */
void RDtree::saveChildren(FILE *fp, RDnode *parent,
                          void (*savePatt)(void *pattern, FILE *fp),
                          void (*saveClient)(void *client, FILE *fp))
{
   int    n;
   float  d;
   RDnode *p;

   for (p = parent->childlist, n = 0; p != NULL; p = p->sibnext, n++)
   {
   }
   FWRITE_INT(&n, fp);
   for (p = parent->childlist; p != NULL; p = p->sibnext)
   {
      savePatt(p->pattern, fp);
      if (saveClient != NULL)
      {
         saveClient(p->client, fp);
      }
      d = p->distance;
      FWRITE_FLOAT(&d, fp);
      saveChildren(fp, p, savePatt, saveClient);
   }
}


// Print tree.
bool RDtree::print(char *filename, void (*printPatt)(void *pattern, FILE *fp))
{
   if (filename == NULL)
   {
      print(printPatt);
   }
   else
   {
      FILE *fp = fopen(filename, "w");
      if (fp == NULL)
      {
         return(false);
      }
      print(printPatt, fp);
      fclose(fp);
   }
   return(true);
}


void RDtree::print(void (*printPatt)(void *pattern, FILE *fp), FILE *fp)
{
   fprintf(fp, "<RDtree>\n");
   printNode(fp, root, 0, printPatt);
   fprintf(fp, "</RDtree>\n");
}


/* print node */
void RDtree::printNode(FILE *fp, RDnode *node, int level,
                       void (*printPatt)(void *pattern, FILE *fp))
{
   int    i;
   RDnode *p;

   if (node != NULL)
   {
      for (i = 0; i < level; i++)
      {
         fprintf(fp, "  ");
      }
      fprintf(fp, "<node>\n");
      for (i = 0; i < level; i++)
      {
         fprintf(fp, "  ");
      }
      fprintf(fp, "  <pattern>");
      printPatt(node->pattern, fp);
      fprintf(fp, "</pattern>\n");
      for (i = 0; i < level; i++)
      {
         fprintf(fp, "  ");
      }
      fprintf(fp, "  <distance>%f</distance>\n", node->distance);
      for (i = 0; i < level; i++)
      {
         fprintf(fp, "  ");
      }
      fprintf(fp, "  <children>\n");
      for (p = node->childlist; p != NULL; p = p->sibnext)
      {
         printNode(fp, p, level + 1, printPatt);
      }
      for (i = 0; i < level; i++)
      {
         fprintf(fp, "  ");
      }
      fprintf(fp, "  </children>\n");
      for (i = 0; i < level; i++)
      {
         fprintf(fp, "  ");
      }
      fprintf(fp, "</node>\n");
   }
}
