// Implementation for Point class
#include "point.h"

#include <iostream>
#include <cstdlib>
#include <cmath>

// Constructors
Point::Point(vector<double> new_pos) {
  pos = new_pos;
  dimention = new_pos.size();
}

/* Function addNeighbor: update neighbors and kth_neighbor_distance of a point with given new point
 * Input: Pointer to the new neighbor point
 */
void Point::addNeighbor(Point* new_point) {
  double new_dist = distance(this, new_point);
  neighbors.insert(make_pair(new_point, new_dist));
  num_neighbors++;

  // If the new point is within k nearest neighbor of it, then the kth_neighbor_distance needs to be updated
  if (new_dist < kth_neighbor_distance) {
    if (num_neighbors <= k) {
      kth_neighbor_distance = (neighbors.end() - 1)->second;
    }
    else {
      auto it = neighbors.begin();
      advance(it, k - 1);
      kth_neighbor_distance = it->second;
    }
  }
}

/* Function removeNeighbor: update neighbors, num_neighbors and kth_neighbor_distance
 * Input: Pointer to the neighbor point to be removed
 */
void Point::removeNeighbor(Point* remove_point) {
  double dist = distance(this, remove_point);
  pair<Point*, double> temp_pair = make_pair(remove_point, dist);
  neighbors.erase(neighbors.find(temp_pair));
  num_neighbors--;
  
  // If the removed point is within k nearest neighbors of it, update the kth_neighbor_distance field
  if (dist <= kth_neighbor_distance) {
    if (num_neighbors <= k) {
      kth_neighbor_distance = (neighbors.end() - 1)->second;
    }
    else {
      auto it = neighbors.begin();
      advance(it, k - 1);
      kth_neighbor_distance = it->second;
    }
  }
}

/* Static function distance
 * Input: two pointers to points
 * Output: distance between two given points
 */
static double Point::distance(Point* p1, Point* p2) {
  int d = p1->dimension;
  double sum = 0;
  for (int i = 0; i < d; i++) {
    sum += pow((p1->pos[i] - p2->pos[i]), 2);
  }
  return sqrt(sum);
}
