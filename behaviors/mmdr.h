#ifndef MMDR_H
#define MMDR_H

#define MMDR_DYNA 1
#define MMDR_N5 2
#define MMDR_N4 3
#define MMD_MSD2 4

// options for simulation

#define REPETITIONS 5
#define ALGORITHM MMDR_N4
// #define ALGORITHM MMD_MSD2
#define NN 11
#define MAXNN NN




#if ALGORITHM == MMDR_DYNA
#define SOLVER mmdr_dyna
#elif ALGORITHM == MMDR_N5
#define SOLVER mmdr_n5
#elif ALGORITHM == MMDR_N4
#define SOLVER mmdr_n4
#elif ALGORITHM == MMD_MSD2
#define SOLVER mmd_msd2
#endif // ALGORITHM

// #define INTERNAL 0.5
#include<iostream>
#include<fstream>
#include<cstdio>
#include<cmath>
#include<string>
#include<algorithm>
#include<vector>
#include<climits>
#include<float.h>
#include<string.h>

#include <boost/cstdint.hpp>

const double D = 3.0f;
const double INTERNAL = 0.5f;
const int MAXP = 100;

typedef uint64_t uint64;


// remove an element from a vector by value.
#define VECREMOVE(vec, v) (vec).erase(  \
              std::remove((vec).begin(), (vec).end(), (v)), (vec).end())


typedef std::pair<double, double> Point;

// Edge looks like (dist, (left node, right node))
typedef std::pair<double, std::pair<int, int> > Edge;




/*struct PriorityPoint{
   Point pt;
   int level;
   PriorityPoint(){}
   PriorityPoint(Point _p, int _level): pt(_p), level(_level){}
};*/

// Edge looks like (dist, (left node, right node))
typedef std::pair<double, std::pair<int, int> > Edge;

struct Test{
 std::vector<Point> starts;
 std::vector<Point> targets;
};


//inline double getdist(const PriorityPoint &a, const PriorityPoint &b);
//inline double getRealdist(const PriorityPoint &a, const PriorityPoint &b);
inline bool edgeVectorLessThan(const std::vector<Edge> &a, const std::vector<Edge> &b);




////////////  Begin implementation of getMinimalMaxEdgeInPerfectMatching  ////////////////

#if ALGORITHM == MMDR_N4 || ALGORITHM == MMD_MSD2

// We consider a bipartite graph with n nodes on the left and right.

// Global variables used in the floodfill.
// foo[0][i] corresponds to i'th node on left; foo[1][i] for the right.

//right nodes are used if and only if mmdr_out[1][j].size() == 1.

// Floodfill from a node.
//  x in {0, 1}: left or right side
//  y in {0, ..., n-1}: node on side x
//  prev in {-1, 0, ..., n-1}: node on side 1-x that we came in on
//                             (-1 for unassigned node on left)
// Returns:
//  If it reaches an unassigned right node, the index of this node.
//  Otherwise, -1.
int flood(int x, int y, int prev);

// starting at node (x, y), follow the back pointers and reverse each edge.
// Return the last node reached (i.e., the newly assigned left node)
inline int reverse(int x, int y);

// Set visited to 0 and flood from unassigned left nodes.
inline void reset_flooding(int n);
/*
  Add edges in order until k nodes can be matched.

  edges is a sorted vector of (dist, (left, right))

  Returns the index of the last edge added; this edge must appear.
 */
int getMinimalMaxEdgeInPerfectMatching(std::vector<Edge> edges, int n, int k);

#endif // ALGORITHM == MMDR_N4 || ALGORITHM == MMD_MSD2


inline double getdist(const Point a, const Point b);






#if ALGORITHM == MMDR_N4

// Begin implementation of hungarian with integer weights solution  ////////

// Code for O(n^3) Hungarian algorithm from http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=hungarianAlgorithm

//#define NN 55             //max number of vertices in one part


void init_labels_int();

void update_labels_int();

void add_to_tree_int(int x, int prevx);

bool augment_int();

int hungarian_int();

std::vector<Edge> mmdr_n4(Test t);

#endif // ALGORTHM == MMDR_N4









#if ALGORITHM == MMD_MSD2

// Code for O(n^3) Hungarian algorithm from http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=hungarianAlgorithm

void init_labels();

void update_labels();

void add_to_tree(int x, int prevx);

bool augment();

void hungarian();


std::vector<Edge> mmd_msd2(Test t);

#endif // ALGORITHM == MMD_MSD2

////////////  End implementation of mmd_msd2  ////////////////


#endif
