#include "mmdr.h"





/*先使用mmdr_On4的算法，之后再修改*/


inline double getdist(const Point a, const Point b){
    double dist = hypot(a.first - b.first, a.second - b.second);
    return dist;
}



////////////  Begin implementation of getMinimalMaxEdgeInPerfectMatching  ////////////////

#if ALGORITHM == MMDR_N4 || ALGORITHM == MMD_MSD2

// We consider a bipartite graph with n nodes on the left and right.

// Global variables used in the floodfill.
// foo[0][i] corresponds to i'th node on left; foo[1][i] for the right.

std::vector<std::vector<int> > mmdr_out[2];  //adjacency list for each node
std::vector<bool> visited[2];           //reached in floodfill
std::vector<int> back[2];               //way to reach it
std::vector<int> used;                  //whether a left node is used
//right nodes are used if and only if mmdr_out[1][j].size() == 1.

// Floodfill from a node.
//  x in {0, 1}: left or right side
//  y in {0, ..., n-1}: node on side x
//  prev in {-1, 0, ..., n-1}: node on side 1-x that we came in on
//                             (-1 for unassigned node on left)
// Returns:
//  If it reaches an unassigned right node, the index of this node.
//  Otherwise, -1.
int flood(int x, int y, int prev){
  visited[x][y] = 1;
  back[x][y] = prev;
  if (x == 1 && mmdr_out[x][y].size() == 0) //reached an unassigned right node!
    return y;

  for(int j = 0; j < mmdr_out[x][y].size(); j++){
    if (!visited[1-x][mmdr_out[x][y][j]]){
      int tmp = flood(1-x, mmdr_out[x][y][j], y);
      if (tmp != -1) //Flood reached the end
        return tmp;
    }
  }
  return -1;
}

// starting at node (x, y), follow the back pointers and reverse each edge.
// Return the last node reached (i.e., the newly assigned left node)
inline int reverse(int x, int y){
  while (true) {
    int prev = back[x][y];
    if (prev == -1)       // Reached the unassigned node on the left
      break;
    mmdr_out[x][y].push_back(prev);
    VECREMOVE(mmdr_out[1-x][prev], y);
    x = 1-x; y = prev;
  }
  return y;
}

// Set visited to 0 and flood from unassigned left nodes.
inline void reset_flooding(int n){
  for(int i = 0; i < 2; i++)
    std::fill(visited[i].begin(), visited[i].end(), 0);

  for(int i = 0; i < n; i++)
    if(!used[i])
      flood(0, i, -1);
}

/*
  Add edges in order until k nodes can be matched.

  edges is a sorted vector of (dist, (left, right))

  Returns the index of the last edge added; this edge must appear.
 */
int getMinimalMaxEdgeInPerfectMatching(std::vector<Edge> edges, int n, int k){
  for(int i = 0; i < 2; i++) { //Clear the graph
    mmdr_out[i].clear();
    mmdr_out[i].resize(n);
  }
  std::fill(used.begin(), used.end(), 0);
  reset_flooding(n);

  int answer;
  for(answer = 0; answer < edges.size(); answer++){
    std::pair<int, int> e = edges[answer].second;
    mmdr_out[0][e.first].push_back(e.second);
    //printf("Added edge: %d %d\n", e.first, e.second);
    if(visited[0][e.first] && !visited[1][e.second]) {
      int ans = flood(1, e.second, e.first);
      if (ans != -1){  //We made it to the end!
        if (--k == 0)
          break;
        int start = reverse(1, ans);
        used[start] = 1;
        reset_flooding(n);
      }
    }
  }
  // We must use edges[answer] to push k flow with minimal max edge.
  return answer;
}

#endif // ALGORITHM == MMDR_N4 || ALGORITHM == MMD_MSD2

////////////  End implementation of getMinimalMaxEdgeInPerfectMatching  ////////////////


////////////  Begin implementation of mmdr_n4  ////////////////

#if ALGORITHM == MMDR_N4

// Begin implementation of hungarian with integer weights solution  ////////
//#define INF DBL_MAX    //just infinity

int cost_int[NN][NN];          //cost matrix

int n_int, max_match_int;        //n workers and n jobs

int lx_int[NN], ly_int[NN];        //labels of X and Y parts
int xy_int[NN];               //xy[x] - vertex that is matched with x,
int yx_int[NN];               //yx[y] - vertex that is matched with y
bool S_int[NN], T_int[NN];         //sets S and T in algorithm
int slack_int[NN];            //as in the algorithm description
int slackx_int[NN];           //slackx[y] such a vertex, that
                         // l(slackx[y]) + l(y) - w(slackx[y],y) = slack[y]
int prev_int[NN];             //array for memorizing alternating paths
// Code for O(n^3) Hungarian algorithm from http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=hungarianAlgorithm

//#define NN 55             //max number of vertices in one part


void init_labels_int()
{
    memset(lx_int, 0, sizeof(lx_int));
    memset(ly_int, 0, sizeof(ly_int));
    for (int x = 0; x < n_int; x++)
        for (int y = 0; y < n_int; y++)
      lx_int[x] = std::max(lx_int[x], cost_int[x][y]);
}

void update_labels_int()
{
  int x, y;
  int delta = INT_MAX;             //init delta as infinity
    for (y = 0; y < n_int; y++)            //calculate delta using slack
        if (!T_int[y])
      delta = std::min(delta, slack_int[y]);
    for (x = 0; x < n_int; x++)            //update X labels
        if (S_int[x]) lx_int[x] -= delta;
    for (y = 0; y < n_int; y++)            //update Y labels
        if (T_int[y]) ly_int[y] += delta;
    for (y = 0; y < n_int; y++)            //update slack_int array
        if (!T_int[y])
            slack_int[y] -= delta;
}

void add_to_tree_int(int x, int prevx)
//x - current vertex,prevx - vertex from X before x in the alternating path,
//so we add edges (prevx, xy[x]), (xy[x], x)
{
    S_int[x] = true;                    //add x to S
    prev_int[x] = prevx;                //we need this when augmenting
    for (int y = 0; y < n_int; y++)    //update slacks, because we add new vertex to S
        if (lx_int[x] + ly_int[y] - cost_int[x][y] < slack_int[y])
        {
            slack_int[y] = lx_int[x] + ly_int[y] - cost_int[x][y];
            slackx_int[y] = x;
        }
}

bool augment_int()                         //main function of the algorithm
{
    if (max_match_int == n_int) return true;        //check wether matching is already perfect
    int x, y, root;                    //just counters and root vertex
    int q[NN], wr = 0, rd = 0;          //q - queue for bfs, wr,rd - write and read
                                       //pos in queue
    memset(S_int, false, sizeof(S_int));       //init set S
    memset(T_int, false, sizeof(T_int));       //init set T
    memset(prev_int, -1, sizeof(prev_int));    //init set prev - for the alternating tree
    for (x = 0; x < n_int; x++)            //finding root of the tree
        if (xy_int[x] == -1)
        {
            q[wr++] = root = x;
            prev_int[x] = -2;
            S_int[x] = true;
            break;
        }

    for (y = 0; y < n_int; y++)            //initializing slack array
    {
        slack_int[y] = lx_int[root] + ly_int[y] - cost_int[root][y];
        slackx_int[y] = root;
    }

//second part of augment_int() function
    while (true)                                                        //main cycle
    {
        while (rd < wr)                                                 //building tree with bfs cycle
        {
            x = q[rd++];                                                //current vertex from X part
            for (y = 0; y < n_int; y++)                                     //iterate through all edges in equality graph
                if (cost_int[x][y] == lx_int[x] + ly_int[y] &&  !T_int[y])
                {
                    if (yx_int[y] == -1) break;                             //an exposed vertex in Y found, so
                                                                        //augmenting path exists!
                    T_int[y] = true;                                        //else just add y to T,
                    q[wr++] = yx_int[y];                                    //add vertex yx[y], which is matched
                                                                        //with y, to the queue
                    add_to_tree_int(yx_int[y], x);                              //add edges (x,y) and (y,yx[y]) to the tree
                }
            if (y < n_int) break;                                           //augmenting path found!
        }
        if (y < n_int) break;                                               //augmenting path found!

        update_labels_int();                                                //augmenting path not found, so improve labeling
        wr = rd = 0;
        for (y = 0; y < n_int; y++)
        //in this cycle we add edges that were added to the equality graph as a
        //result of improving the labeling, we add edge (slackx[y], y) to the tree if
        //and only if !T[y] &&  slack_int[y] == 0, also with this edge we add another one
        //(y, yx[y]) or augment the matching, if y was exposed
            if (!T_int[y] &&  slack_int[y] == 0)
            {
                if (yx_int[y] == -1)                                        //exposed vertex in Y found - augmenting path exists!
                {
                    x = slackx_int[y];
                    break;
                }
                else
                {
                    T_int[y] = true;                                        //else just add y to T,
                    if (!S_int[yx_int[y]])
                    {
                        q[wr++] = yx_int[y];                                //add vertex yx[y], which is matched with
                                                                        //y, to the queue
                        add_to_tree_int(yx_int[y], slackx_int[y]);                  //and add edges (x,y) and (y,
                                                                        //yx[y]) to the tree
                    }
                }
            }
        if (y < n_int) break;                                               //augmenting path found!
    }

    if (y < n_int)                                                          //we found augmenting path!
    {
        max_match_int++;                                                    //increment matching
        //in this cycle we inverse edges along augmenting path
        for (int cx = x, cy = y, ty; cx != -2; cx = prev_int[cx], cy = ty)
        {
            ty = xy_int[cx];
            yx_int[cy] = cx;
            xy_int[cx] = cy;
        }
        //augment3();                                                      //recall function, go to step 1 of the algorithm
    return false;
    }
    return true;
}//end of augment3() function

int hungarian_int()
{
  int ret = 0;                      //weight of the optimal matching
    n_int = NN;
    max_match_int = 0;                    //number of vertices in current matching
    memset(xy_int, -1, sizeof(xy_int));
    memset(yx_int, -1, sizeof(yx_int));

    init_labels_int();                    //step 0
    while (!augment_int()){;}                        //steps 1-3
    for (int x = 0; x < n_int; x++)       //forming answer there
        ret += cost_int[x][xy_int[x]];
    return ret;
}

std::vector<Edge> mmdr_n4(Test t){
  int n = t.starts.size();
  std::vector<Edge> edges, edges_tmp;
  std::vector<Edge> answer;

  // Create edges from all pairwise distances
  for(int i = 0; i < n; i++){

    for(int j = 0; j < n; j++) {
      edges.push_back(std::make_pair(getdist(t.starts[i], t.targets[j]),
                                     std::make_pair(i, j)));
      // printf("\t%f\t", getdist(t.starts[i], t.targets[j]));
    }
    // printf("\n");
  }

  // Make the global variables in the floodfill have the right sizes
  for(int i = 0; i < 2; i++) {
    visited[i].resize(n);
    back[i].resize(n);
  }
  used.resize(n);

  int k = n;

  // Initialize all costs to large value
  // Don't use memset as memory might not be contiguous
  //memset(cost_int, -(n+1), sizeof(cost_int));
  for (int i = 0; i < NN; i++) {
    for (int j = 0; j < NN ; j++) {
      cost_int[i][j] = -(n+1);
    }
  }

  while (k > 0) {

    std::sort(edges.begin(), edges.end());

    // Call getMinimalMaxEdgeInPerfectMatching to get minimum maximal edge in a perfect matching.
    int choice = getMinimalMaxEdgeInPerfectMatching(edges, n, n);
    double max_edge_value = edges[choice].first;

    // Now remove (make very large) all edges that are greater than max_edge_value
    // Set cost of all max_edge_values to 1 and set cost of smaller edges to 0
    for(__SIZE_TYPE__ i = 0; i < edges.size(); i++){
      if (edges[i].first < max_edge_value) {
    cost_int[edges[i].second.first][edges[i].second.second] = 0;
      } else if (edges[i].first == max_edge_value) {
    cost_int[edges[i].second.first][edges[i].second.second] = -1;
      } else {
    cost_int[edges[i].second.first][edges[i].second.second] = -(n+1);
      }
    }

    // Solve 0-1 sum minimization assignment problem
    int l = -hungarian_int();
    k -= l;

    if (k == 0) {
      // We are done!
      break;
    }

    // Now remove all edges that aren't tight edges and add max_edge_value
    // edges to candidate edges for solution
    edges_tmp.clear();
    for(__SIZE_TYPE__ i = 0; i < edges.size(); i++) {
      Edge e = edges[i];
      if (lx_int[e.second.first] + ly_int[e.second.second] == cost_int[e.second.first][e.second.second]) {
    // We have a tight edge
    if (e.first == max_edge_value) {
      // Set distance of edge to -1 so that we don't return it when finding the longest edge
      // in a perfect matching and also so that we set its cost to 0 when minimizing 0-1 sum.
      e.first = -1;
    }
    edges_tmp.push_back(e);
      } else {
    // We are removing this edge from consideration
    cost_int[e.second.first][e.second.second] = -(n+1);
      }
    }
    edges = edges_tmp;
  }

  for (int i = 0; i < n; i++) {      //forming answer there
    answer.push_back(std::make_pair(getdist(t.starts[i], t.targets[xy_int[i]]),
                                    std::make_pair(i, xy_int[i])));
  }
  // answer now contains the correct answer.

  return answer;
}

#endif // ALGORTHM == MMDR_N4

////////////  End implementation of mmdr_n4  ////////////////

////////////  Begin implementation of mmd_msd2 solution  ////////

#if ALGORITHM == MMD_MSD2

// Code for O(n^3) Hungarian algorithm from http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=hungarianAlgorithm

#define INF DBL_MAX    //just infinity

double cost[NN][NN];          //cost matrix
int n, max_match;        //n workers and n jobs
double lx[NN], ly[NN];        //labels of X and Y parts
int xy[NN];               //xy[x] - vertex that is matched with x,
int yx[NN];               //yx[y] - vertex that is matched with y
bool S[NN], T[NN];         //sets S and T in algorithm
double slack[NN];            //as in the algorithm description
int slackx[NN];           //slackx[y] such a vertex, that
                         // l(slackx[y]) + l(y) - w(slackx[y],y) = slack[y]
int prev[NN];             //array for memorizing alternating paths

void init_labels()
{
    memset(lx, 0, sizeof(lx));
    memset(ly, 0, sizeof(ly));
    for (int x = 0; x < n; x++)
        for (int y = 0; y < n; y++)
      lx[x] = std::max(lx[x], cost[x][y]);
}

void update_labels()
{
  int x, y;
  double delta = INF;             //init delta as infinity
    for (y = 0; y < n; y++)            //calculate delta using slack
        if (!T[y])
      delta = std::min(delta, slack[y]);
    for (x = 0; x < n; x++)            //update X labels
        if (S[x]) lx[x] -= delta;
    for (y = 0; y < n; y++)            //update Y labels
        if (T[y]) ly[y] += delta;
    for (y = 0; y < n; y++)            //update slack array
        if (!T[y])
            slack[y] -= delta;
}

void add_to_tree(int x, int prevx)
//x - current vertex,prevx - vertex from X before x in the alternating path,
//so we add edges (prevx, xy[x]), (xy[x], x)
{
    S[x] = true;                    //add x to S
    prev[x] = prevx;                //we need this when augmenting
    for (int y = 0; y < n; y++)    //update slacks, because we add new vertex to S
        if (lx[x] + ly[y] - cost[x][y] < slack[y])
        {
            slack[y] = lx[x] + ly[y] - cost[x][y];
            slackx[y] = x;
        }
}

bool augment()                         //main function of the algorithm
{
    if (max_match == n) return true;        //check wether matching is already perfect
    int x, y, root;                    //just counters and root vertex
    int q[NN], wr = 0, rd = 0;          //q - queue for bfs, wr,rd - write and read
                                       //pos in queue
    memset(S, false, sizeof(S));       //init set S
    memset(T, false, sizeof(T));       //init set T
    memset(prev, -1, sizeof(prev));    //init set prev - for the alternating tree
    for (x = 0; x < n; x++)            //finding root of the tree
        if (xy[x] == -1)
        {
            q[wr++] = root = x;
            prev[x] = -2;
            S[x] = true;
            break;
        }

    for (y = 0; y < n; y++)            //initializing slack array
    {
        slack[y] = lx[root] + ly[y] - cost[root][y];
        slackx[y] = root;
    }

//second part of augment() function
    while (true)                                                        //main cycle
    {
        while (rd < wr)                                                 //building tree with bfs cycle
        {
            x = q[rd++];                                                //current vertex from X part
            for (y = 0; y < n; y++)                                     //iterate through all edges in equality graph
                if (cost[x][y] == lx[x] + ly[y] &&  !T[y])
                {
                    if (yx[y] == -1) break;                             //an exposed vertex in Y found, so
                                                                        //augmenting path exists!
                    T[y] = true;                                        //else just add y to T,
                    q[wr++] = yx[y];                                    //add vertex yx[y], which is matched
                                                                        //with y, to the queue
                    add_to_tree(yx[y], x);                              //add edges (x,y) and (y,yx[y]) to the tree
                }
            if (y < n) break;                                           //augmenting path found!
        }
        if (y < n) break;                                               //augmenting path found!

        update_labels();                                                //augmenting path not found, so improve labeling
        wr = rd = 0;
        for (y = 0; y < n; y++)
        //in this cycle we add edges that were added to the equality graph as a
        //result of improving the labeling, we add edge (slackx[y], y) to the tree if
        //and only if !T[y] &&  slack[y] == 0, also with this edge we add another one
        //(y, yx[y]) or augment the matching, if y was exposed
            if (!T[y] &&  slack[y] == 0)
            {
                if (yx[y] == -1)                                        //exposed vertex in Y found - augmenting path exists!
                {
                    x = slackx[y];
                    break;
                }
                else
                {
                    T[y] = true;                                        //else just add y to T,
                    if (!S[yx[y]])
                    {
                        q[wr++] = yx[y];                                //add vertex yx[y], which is matched with
                                                                        //y, to the queue
                        add_to_tree(yx[y], slackx[y]);                  //and add edges (x,y) and (y,
                                                                        //yx[y]) to the tree
                    }
                }
            }
        if (y < n) break;                                               //augmenting path found!
    }

    if (y < n)                                                          //we found augmenting path!
    {
        max_match++;                                                    //increment matching
        //in this cycle we inverse edges along augmenting path
        for (int cx = x, cy = y, ty; cx != -2; cx = prev[cx], cy = ty)
        {
            ty = xy[cx];
            yx[cy] = cx;
            xy[cx] = cy;
        }
        //augment();                                                      //recall function, go to step 1 of the algorithm
    return false;
    }
    return true;
}//end of augment() function

void hungarian()
{
  //int ret = 0;                      //weight of the optimal matching
    n = NN;
    max_match = 0;                    //number of vertices in current matching
    memset(xy, -1, sizeof(xy));
    memset(yx, -1, sizeof(yx));
    for (int i = 0; i < NN; i++) {
      for (int j = 0; j < NN; j++) {
    cost[i][j] *= -1.0;
      }
    }
    init_labels();                    //step 0
    while (!augment()){;}                        //steps 1-3
    //for (int x = 0; x < n; x++)       //forming answer there
    //    ret += cost[x][xy[x]];
    //return ret;
}


std::vector<Edge> mmd_msd2(Test t){
  int n = t.starts.size();
  std::vector<Edge> edges;
  std::vector<Edge> answer;

  // Create edges from all pairwise distances squared
  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
      edges.push_back(std::make_pair(pow(getdist(t.starts[i], t.targets[j]),2),
                                     std::make_pair(i, j)));
  std::sort(edges.begin(), edges.end());

  // Make the global variables in the floodfill have the right sizes
  for(int i = 0; i < 2; i++) {
    visited[i].resize(n);
    back[i].resize(n);
  }
  used.resize(n);

  // Call getMinimalMaxEdgeInPerfectMatching to get minimum maximal edge in a perfect mathcing.

  int choice = getMinimalMaxEdgeInPerfectMatching(edges, n, n);
  double max_edge_value = edges[choice].first;

  // Now remove (make very large) all edges that are greater than max_edge_value
  for(int i = 0; i < edges.size(); i++){
    if (edges[i].first <= max_edge_value)
      cost[edges[i].second.first][edges[i].second.second] = edges[i].first;
    else
      cost[edges[i].second.first][edges[i].second.second] = max_edge_value*n+1;
  }

  hungarian();

  for(int i = 0; i < n; i++){
    //printf("Got: %d %d %lf\n", i, h2[i], getdist(t.starts[i], t.targets[h2[i]]));
    answer.push_back(std::make_pair(getdist(t.starts[i], t.targets[xy[i]]),
                                    std::make_pair(i, xy[i])));
  }

  // answer now contains the correct answer.

  return answer;
}

#endif // ALGORITHM == MMD_MSD2

////////////  End implementation of mmd_msd2  ////////////////
