// Implementation for Point class
#include "point.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace std;

// Constructors
Point::Point(vector<double> new_pos) {
  pos = new_pos;
  dimension = new_pos.size();
  num_neighbors = 0;
  kth_neighbor_distance = 0;
  // ave_k_neighbor_distance = 0;
  ave_reachability_distance = 0;
  lof_val = 0;
}

/* Function insertNeighbor: insert a point to the neighbors set only. 
 * Input: Pointer to an existing point
 * Output: None
 * The function is used on new added point for adding all existing points to it's neighbors set. It will be called by Lof's member function together with other functions to update other fields like kth_neighbor_distance, etc.
 */
void Point::insertNeighbor(Point* neighbor_point) {
  double new_dist = distance(this, neighbor_point);
  PointDistancePair new_pair(neighbor_point, new_dist);
  neighbors.insert(new_pair);
  num_neighbors++;
}

/* Function addNewNeighbor: update existing points of their new neighbor and kth_neighbor_distance of a point with given new point
 * Input: Pointer to the new neighbor point
 */
void Point::addNewNeighbor(Point* new_point) {
  double new_dist = distance(this, new_point);
  PointDistancePair new_pair(new_point, new_dist);
  neighbors.insert(new_pair);
  num_neighbors++;

  // If the new point is within k nearest neighbor of it, then the kth_neighbor_distance needs to be updated
  if (new_dist <= kth_neighbor_distance) {
    updateStats();
  }
}

/* Function removeNeighbor: update neighbors, num_neighbors and kth_neighbor_distance
 * Input: Pointer to the neighbor point to be removed
 */
void Point::removeNeighbor(Point* remove_point) {
  double dist = distance(this, remove_point);
  PointDistancePair temp_pair(remove_point, dist);
  neighbors.erase(neighbors.find(temp_pair));
  num_neighbors--;
  
  // If the removed point is within k nearest neighbors of it, update stats field
  if (dist <= kth_neighbor_distance) {
    updateStats();
  }
}

/* Function udateStats: updates the following four fields of a point
 * kth_neighbor_distance, ave_k_neighbor_distance, ave_reachability_distance, lof_val
 * Input: None
 * Output: None
 */
void Point::updateStats() {
  if (num_neighbors >= k) {
    kth_neighbor_distance = calcKthNeighborDistance(); 
    // ave_k_neighbor_distance = calcAveKDistance();
    ave_reachability_distance = calcAveReachabilityDistance();
    lof_val = calcLof();
  }
}

/* Function calcKthNeighborDistance: return the distance of a point with its kth nearest neighbor
 * Input: none
 * Output: a double
 */
double Point::calcKthNeighborDistance() const {
  auto it = neighbors.begin();
  advance(it, k - 1);
  return it->dist;
}

/* Function calcAveKDistance: computes the average distance to a point's k nearest neighbors (v_k(x))
 * Input: None
 * Output: a double
 */
/*
double Point::calcAveKDistance() const {
  double sum = 0;
  auto it = neighbors.begin();
  for (int i = 0; i < k; i++) {
    sum += distance(this, it->p);
  }
  return sum / k;
}
*/

/* Function calcAveReachablityDistance: computes the average reachability distance of point
 * Input: None
 * Output: a double
 * ar_k(x) = MEAN_y in L_k(x)(r_k(x, y))
 */
double Point::calcAveReachabilityDistance() const {
  double sum = 0;
  auto it = neighbors.begin();
  int counter = 0;
  while (it->dist <= kth_neighbor_distance) {
    sum += calcReachabilityDistance(this, it->p);
    it++;
    counter++;
  }
  return sum / counter;
}


/* Function calcLof: LOF_k(x) = MEAN y in L_k(x)(ar_k(x) / ar_k(y))
 * Input: None
 * Output: a double
 */
double Point::calcLof() const {
  double dist;
  double sum = 0;
  int counter = 0;
  auto it = neighbors.begin();
  while (it->dist <= kth_neighbor_distance) {
    sum += ave_reachability_distance / it->p->ave_reachability_distance;
    it++;
    counter++;
  }
  return sum / counter;
}

/* Function displayPoint: print the point position to stdout
 */
void Point::displayPoint() {
  for (int i = 0; i < dimension; i++) {
    cout << pos[i] << ", ";
  }
  cout << endl;
}

/* Function calcReachabilityDistance: computes the reachability distance of p1 with respect to p2
 * Input: two pointers to points p1 and p2
 * Output: reachability distance of p1 with respect to p2 = max{distance(p1, p2), ave_k_neighbor_distance of p2}
 */
double Point::calcReachabilityDistance(const Point* p1, const Point* p2) const {
  return max(distance(p1, p2), p2->kth_neighbor_distance);
}

/* Static function distance
 * Input: two pointers to points
 * Output: distance between two given points
 */
double Point::distance(const Point* p1, const Point* p2) const {
  int d = p1->dimension;
  double sum = 0;
  for (int i = 0; i < d; i++) {
    sum += pow((p1->pos[i] - p2->pos[i]), 2);
  }
  return sqrt(sum);
}

