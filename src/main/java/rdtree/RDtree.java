/*
 * Copyright (c) 2015 Tom Portegys (portegys@gmail.com). All rights reserved.
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

package rdtree;

import java.util.Random;

public class RDtree
{
   // Tree configuration parameter.
   public static final float DEFAULT_RADIUS = 0.9f;
   public float              RADIUS         = DEFAULT_RADIUS;

   // Tree root.
   public RDnode root;

   // Tree node.
   public class RDnode
   {
      public RDclient client;         /* client link */
      public RDnode   childlist;      /* child pattern list */
      public RDnode   childlast;      /* last child */
      public RDnode   sibnext;        /* next sibling */
      public RDnode   sibback;        /* previous sibling */
      public float    distance;       /* distance from child to parent */

      // Constructors.
      public RDnode(RDclient client)
      {
         this.client = client;
         childlist   = childlast = null;
         sibnext     = sibback = null;
         distance    = 0.0f;
      }


      public RDnode()
      {
         client    = null;
         childlist = childlast = null;
         sibnext   = sibback = null;
         distance  = 0.0f;
      }
   };

   // Constructors.
   public RDtree()
   {
      root = null;
   }


   public RDtree(float radius)
   {
      RADIUS = radius;
      root   = null;
   }


   // Insert pattern.
   public void insert(RDclient client)
   {
      RDnode node = new RDnode(client);

      insert(root, node);
   }


   // Insert node.
   public void insert(RDnode current, RDnode node)
   {
      RDnode p, p2, p3;
      float  dcn, dnn;

      /* clear node */
      node.childlist = null;
      node.childlast = null;
      node.sibnext   = null;
      node.sibback   = null;
      node.distance  = 0.0f;

      /* new root? */
      if (current == null)
      {
         root = node;
         return;
      }

      /* add pattern to first acceptable branch */
      dcn = node.client.dist(current.client);
      while (true)
      {
         for (p = current.childlist; p != null; p = p.sibnext)
         {
            /* check relative distances */
            dnn = node.client.dist(p.client);
            if (dnn <= (p.distance * RADIUS))
            {
               /* change current fragment */
               current = p;
               dcn     = dnn;
               break;
            }
         }
         if (p == null)
         {
            break;
         }
      }

      /* link new as child of current pattern */
      node.distance = dcn;
      node.sibnext  = null;
      node.sibback  = current.childlast;
      if (current.childlast != null)
      {
         current.childlast.sibnext = node;
      }
      else
      {
         current.childlist = node;
      }
      current.childlast = node;

      /*
       * check if previously added patterns should be unlinked from the
       * current pattern and linked as children of the new pattern.
       */
      for (p = current.childlist; p != node && p != null; )
      {
         dnn = node.client.dist(p.client);

         /* if should be linked to new pattern */
         if (dnn <= (node.distance * RADIUS))
         {
            /* re-link children */
            p2 = p.sibnext;
            if (p.sibback != null)
            {
               p.sibback.sibnext = p2;
            }
            else
            {
               current.childlist = p2;
            }
            if (p2 != null)
            {
               p2.sibback = p.sibback;
            }

            /* convert child sub-tree to list */
            for (p2 = p3 = p, p.sibnext = null; ; p3 = p3.sibnext)
            {
               for ( ; p2.sibnext != null; p2 = p2.sibnext)
               {
               }
               for ( ; p3 != null && p3.childlist == null; p3 = p3.sibnext)
               {
               }
               if (p3 == null)
               {
                  break;
               }
               p2.sibnext = p3.childlist;
            }

            /* add list to current pattern */
            for (p2 = p; p2 != null; p2 = p3)
            {
               p3 = p2.sibnext;
               insert(current, p2);
            }

            /* restart check (since child configuration may have changed) */
            for (p = current.childlist; p != node && p != null; p = p.sibnext)
            {
            }
            if (p == null)
            {
               break;
            }
            p = current.childlist;
         }
         else
         {
            p = p.sibnext;
         }
      }
   }


   // Remove node for client.
   public void remove(RDclient client)
   {
      RDnode current, node, p, p2, p3;
      float  d;

      if ((current = node = root) == null)
      {
         return;
      }
      d = current.client.dist(client);
      while (d > 0.0f)
      {
         for (node = current.childlist; node != null; node = node.sibnext)
         {
            d = node.client.dist(client);
            if (d <= (node.distance * RADIUS))
            {
               if (d > 0.0f)
               {
                  current = node;
               }
               break;
            }
         }
         if (node == null)
         {
            return;
         }
      }

      /* unlink pattern */
      if (node == root)
      {
         current = root = null;
      }
      else
      {
         if (node.sibback == null)
         {
            current.childlist = node.sibnext;
         }
         else
         {
            node.sibback.sibnext = node.sibnext;
         }
         if (node.sibnext == null)
         {
            current.childlast = node.sibback;
         }
         else
         {
            node.sibnext.sibback = node.sibback;
         }
      }
      node.sibnext = node.sibback = null;

      /* convert child sub-tree to list */
      p = node.childlist;
      for (p2 = p; p2 != null; p2 = p2.sibnext)
      {
         if (p2.childlist != null)
         {
            p3                 = p2.sibnext;
            p2.sibnext         = p2.childlist;
            p2.sibnext.sibback = p2;
            if (p3 != null)
            {
               p3.sibback = p2.childlast;
            }
            p2.childlast.sibnext = p3;
            p2.childlist         = p2.childlast = null;
         }
      }

      /* add list to parent pattern */
      for (p2 = p; p2 != null; p2 = p3)
      {
         p3 = p2.sibnext;
         insert(current, p2);
         if (current == null)
         {
            current = root;
         }
      }
   }


   // Search states.
   public static enum SearchState
   {
      DISTPENDING,                     /* distance pending */
      DISTDONE,                        /* distance computed */
      EXPANDED,                        /* pattern expanded */
      SRCHDONE                         /* pattern searched */
   };

   // Search element.
   public class RDsearch
   {
      public RDnode      node;             /* node */
      public float       distance;         /* comparison distance */
      public RDsearch    srchnext;         /* next on search return list */
      public float       workdist;         /* work distance */
      public SearchState state;            /* search state */
      public RDsearch    childlist;        /* children */
      public RDsearch    sibnext;          /* next sibling */
      public RDsearch    sibback;          /* previous sibling */

      // Constructor.
      public RDsearch()
      {
         node      = null;
         distance  = 0.0f;
         srchnext  = null;
         workdist  = 0.0f;
         state     = SearchState.DISTPENDING;
         childlist = sibnext = sibback = null;
      }
   };

   // Empty search element.
   RDsearch EMPTY = new RDsearch();

   // Search stack size.
   public static int SrchStkSz = 100;

   // Search stack.
   class SrchStk
   {
      RDsearch currsrch;
      RDsearch child;
      RDsearch childnext;

      SrchStk()
      {
         currsrch = child = childnext = null;
      }
   };

   // Search control.
   public class SrchCtl
   {
      RDsearch srchList;                    /* search results */
      RDsearch srchBest;                    /* best search result */
      int      maxFind;                     /* max number of results */
      int      maxSearch;                   /* max nodes to search (-1=unlimited) */
      int      searchCount;                 /* search counter */
      int      bestSearch;                  /* best result search counter */
      SrchStk[]             srchStk;        /* search stack */
      int srchStkIdx;                       /* stack index */
   };

   // Search bounds.
   class SrchBounds
   {
      int      numFound;
      RDsearch cutoff;

      SrchBounds()
      {
         numFound = 0;
         cutoff   = null;
      }
   };

   /* Search space for patterns closest to the given client pattern */
   /* Return best matches */
   public RDsearch search(RDclient client, int maxFind, int maxSearch)
   {
      RDsearch sw, sw2, sw3;
      RDsearch srchList = null;

      if (root == null)
      {
         return(srchList);
      }

      /* prepare for search */
      SrchCtl srchCtl = new SrchCtl();
      srchCtl.srchList    = null;
      srchCtl.srchBest    = null;
      srchCtl.maxFind     = maxFind;
      srchCtl.maxSearch   = maxSearch;
      srchCtl.searchCount = srchCtl.bestSearch = 0;
      srchCtl.srchStk     = new SrchStk[SrchStkSz];
      for (int i = 0; i < SrchStkSz; i++)
      {
         srchCtl.srchStk[i] = new SrchStk();
      }
      srchCtl.srchStkIdx = 0;
      RDnode node = new RDnode(client);

      /* search tree */
      search(srchCtl, node);

      /* extract search results */
      for (sw = srchCtl.srchList, sw3 = null; sw != null; sw = sw.srchnext)
      {
         sw2          = new RDsearch();
         sw2.node     = sw.node;
         sw2.distance = sw.distance;
         if (sw3 == null)
         {
            srchList = sw2;
         }
         else
         {
            sw3.srchnext = sw2;
         }
         sw3 = sw2;
      }
      return(srchList);
   }


   /* search space for patterns closest to the given pattern */
   /* put best matches on srchList */
   void search(SrchCtl srchCtl, RDnode srchNode)
   {
      int        numSearch, stkIdx, stkpIdx;
      RDnode     p;
      RDsearch   sw, sw2, bsw, bsw2;
      SrchBounds srchBounds;

      /* initialize */
      numSearch  = 0;
      srchBounds = new SrchBounds();
      if ((srchCtl.maxSearch >= 0) && (numSearch >= srchCtl.maxSearch))
      {
         return;
      }
      if (root == null)
      {
         return;
      }
      SrchStk stkp = srchCtl.srchStk[srchCtl.srchStkIdx];
      stkp.currsrch          = new RDsearch();
      stkp.currsrch.node     = root;
      stkp.currsrch.distance = stkp.currsrch.node.client.dist(srchNode.client);
      stkp.currsrch.state    = SearchState.DISTDONE;
      foundPatt(srchCtl, stkp.currsrch, srchBounds);
      numSearch++;
      if ((srchCtl.maxSearch >= 0) && (numSearch >= srchCtl.maxSearch))
      {
         return;
      }

      /* for each level of recursion */
      while (srchCtl.srchStkIdx >= 0)
      {
         /* find best and next best distances for current search branch */
         for (stkIdx = srchCtl.srchStkIdx; stkIdx >= 0; stkIdx--)
         {
            stkpIdx = stkIdx;
            stkp    = srchCtl.srchStk[stkpIdx];

            /* expand pattern? */
            if (stkp.currsrch.state == SearchState.DISTDONE)
            {
               for (p = stkp.currsrch.node.childlist, sw2 = null; p != null; p = p.sibnext)
               {
                  sw       = new RDsearch();
                  sw.node  = p;
                  sw.state = SearchState.DISTPENDING;
                  if (sw2 == null)
                  {
                     stkp.currsrch.childlist = sw;
                  }
                  else
                  {
                     sw2.sibnext = sw;
                     sw.sibback  = sw2;
                  }
                  sw2 = sw;
               }
               stkp.child          = stkp.childnext = null;
               stkp.currsrch.state = SearchState.EXPANDED;
            }

            /* best and next best distances must be (re)computed? */
            if ((stkp.child == null) ||
                ((stkp.child != null) &&
                 (stkp.child.workdist > 0.0f) &&
                 (stkp.childnext == EMPTY)) ||
                ((stkp.child != null) &&
                 (stkp.childnext != null) &&
                 (stkp.childnext != EMPTY) &&
                 (stkp.child.workdist > stkp.childnext.workdist)))
            {
               bsw = bsw2 = null;
               for (sw = stkp.currsrch.childlist; sw != null; sw = sw2)
               {
                  sw2 = sw.sibnext;

                  /* have best possible child? */
                  if ((bsw != null) && (bsw.workdist == 0.0f))
                  {
                     if (sw2 == null)
                     {
                        bsw2 = null;
                     }
                     else
                     {
                        bsw2 = EMPTY;
                     }
                     break;
                  }

                  /* compute distance? */
                  if (sw.state == SearchState.DISTPENDING)
                  {
                     sw.distance = sw.node.client.dist(srchNode.client);
                     if ((sw.workdist = sw.distance -
                                        (sw.node.distance * RADIUS)) < 0.0f)
                     {
                        sw.workdist = 0.0f;
                     }
                     sw.state = SearchState.DISTDONE;

                     /* save pattern on return list */
                     foundPatt(srchCtl, sw, srchBounds);
                     numSearch++;

                     /* check for termination of search */
                     if ((srchCtl.maxSearch >= 0) && (numSearch >= srchCtl.maxSearch))
                     {
                        return;
                     }
                  }

                  /* cut off infeasible or finished branch */
                  if ((sw.state == SearchState.SRCHDONE) ||
                      ((srchBounds.cutoff != null) && (sw.workdist >= srchBounds.cutoff.distance)))
                  {
                     /* cut off */
                     sw.state = SearchState.SRCHDONE;
                     if (sw.sibback == null)
                     {
                        stkp.currsrch.childlist = sw.sibnext;
                     }
                     else
                     {
                        sw.sibback.sibnext = sw.sibnext;
                     }
                     if (sw.sibnext != null)
                     {
                        sw.sibnext.sibback = sw.sibback;
                     }
                     continue;
                  }

                  /* find best and next best child branches */
                  if ((bsw == null) || (bsw.workdist > sw.workdist))
                  {
                     bsw2 = bsw;
                     bsw  = sw;
                     continue;
                  }
                  if ((bsw2 == null) || (bsw2.workdist > sw.workdist))
                  {
                     bsw2 = sw;
                     continue;
                  }
               }

               /* change to better branch level? */
               if (stkp.child != bsw)
               {
                  srchCtl.srchStkIdx = stkIdx;
               }
               stkp.child     = bsw;
               stkp.childnext = bsw2;
            }

            /* finished with this level? */
            if (stkp.child == null)
            {
               stkp.currsrch.state = SearchState.SRCHDONE;
               srchCtl.srchStkIdx  = stkIdx - 1;
               if (stkIdx > 0)
               {
                  srchCtl.srchStk[stkpIdx - 1].child = null;
               }
            }
            else
            {
               /* set new best distance for branch */
               for (int i = stkpIdx; i >= 0; i--)
               {
                  srchCtl.srchStk[i].currsrch.workdist = stkp.child.workdist;
               }
            }
         }

         /* expand search deeper */
         if (srchCtl.srchStkIdx >= 0)
         {
            srchCtl.srchStkIdx++;
            if (srchCtl.srchStkIdx == SrchStkSz)
            {
               System.err.println("Search stack overflow!");
               System.exit(1);
            }
            stkpIdx       = srchCtl.srchStkIdx;
            stkp          = srchCtl.srchStk[stkpIdx];
            stkp.currsrch = srchCtl.srchStk[stkpIdx - 1].child;
            stkp.child    = stkp.childnext = null;
         }
      }
   }


   /* add pattern to the found list */
   void foundPatt(SrchCtl srchCtl, RDsearch swfound, SrchBounds srchBounds)
   {
      RDsearch sw, sw2;

      for (sw = srchCtl.srchList, sw2 = null; sw != null; sw2 = sw, sw = sw.srchnext)
      {
         if (sw.distance <= swfound.distance)
         {
            break;
         }
      }
      srchCtl.searchCount++;
      if (sw == null)
      {
         srchCtl.bestSearch = srchCtl.searchCount;
      }
      if (sw2 == null)
      {
         if (srchBounds.numFound < srchCtl.maxFind)
         {
            srchBounds.numFound++;
            swfound.srchnext = srchCtl.srchList;
            srchCtl.srchList = swfound;
         }
      }
      else
      {
         swfound.srchnext = sw2.srchnext;
         sw2.srchnext     = swfound;
         if (srchBounds.numFound < srchCtl.maxFind)
         {
            srchBounds.numFound++;
         }
         else
         {
            srchCtl.srchList = (srchCtl.srchList).srchnext;
         }
      }
      if (srchBounds.numFound == srchCtl.maxFind)
      {
         /* set cut off */
         srchBounds.cutoff = srchCtl.srchList;
      }
   }


   /* test parameters */
   public static int NUM_VALS     = 50;
   public static int NUM_PATTERNS = 100000;
   public static int MAX_FIND     = 1;
   public static int NUM_SEARCH   = NUM_PATTERNS;

   public static class TestClient implements RDclient
   {
      public float dist(RDclient client)
      {
         TestClient testClient = (TestClient)client;
         float      dist       = 0.0f;

         for (int i = 0; i < NUM_VALS; i++)
         {
            float d = pattern[i] - testClient.pattern[i];
            if (d < 0.0f) { d = -d; }
            dist += d;
         }
         return(dist);
      }


      public float[] pattern;

      TestClient(float[] pattern)
      {
         this.pattern = pattern;
      }
   }

   /* test */
   public static void main(String[] args)
   {
      int i, j;

      float[] s, t, p;
      RDtree          tree;
      TestClient      client, searchClient, targetClient;
      RDtree.RDsearch searchList;

      /* initialize search pattern */
      Random randomizer = new Random();
      s = new float[NUM_VALS];
      for (i = 0; i < NUM_VALS; i++)
      {
         s[i] = (float)((randomizer.nextInt() % 27) + (int)'a');
      }
      searchClient = new TestClient(s);
      System.out.println("Search pattern:");
      PrintPattern(searchClient);

      /* add patterns to tree */
      tree = new RDtree();
      for (i = 0; i < NUM_PATTERNS; i++)
      {
         p = new float[NUM_VALS];
         for (j = 0; j < NUM_VALS; j++)
         {
            p[j] = (float)((randomizer.nextInt() % 27) + (int)'a');
         }
         client = new TestClient(p);
         tree.insert(client);
      }

      /* add a search target pattern */
      t = new float[NUM_VALS];
      for (i = 0; i < NUM_VALS; i++)
      {
         t[i] = s[i];
         if ((randomizer.nextInt() % 5) == 0)
         {
            if (randomizer.nextBoolean()) { t[i] += randomizer.nextInt() % 5; } else{ t[i] -= randomizer.nextInt() % 5; }
         }
      }
      targetClient = new TestClient(t);
      tree.insert(targetClient);
      System.out.println("Target:");
      PrintPattern(targetClient);

      /* search */
      System.out.println("Search:");
      searchList = tree.search(searchClient, MAX_FIND, NUM_SEARCH);

      /* print search results */
      System.out.println("Result:");
      PrintPattern((TestClient)searchList.node.client);
      System.out.println("Distance=" + searchList.distance);
   }


   /* print pattern */
   private static void PrintPattern(TestClient client)
   {
      float[] p = client.pattern;
      for (int i = 0; i < p.length; i++)
      {
         char c = (char)p[i];
         System.out.print(c);
      }
      System.out.println();
   }
};
