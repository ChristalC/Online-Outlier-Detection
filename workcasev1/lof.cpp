// Implementation file for Lof class
#include "lof.h"

#include <iostream>
#include <algorithm>
#include "point.h"

using namespace std;

// Constructor
Lof::Lof(int ws) {
  window_size = ws;
  point_count = 0;
  dimension = 0;
  group_size = 0;
  threshold = 0;
}

// Destructor
Lof::~Lof() {
  auto it = all_points.begin();
  while (it != all_points.end()) {
    delete *it;
    it++;
  }
}

/* Function addPoint
 * Input: vector of double indicating the position of new point
 * Output: None
 * The function calculates distance between new point and existing points, updates affected points
 * establishes neighbors of the new point.
 */
void Lof::addPoint(vector<double> new_pos) {
  Point* new_point = new Point(new_pos);
  all_points.push_back(new_point);
  point_count++;
  // If lof_data.dimension is not fixed (== 0), set it to dimension of new_point.
  if (dimension == 0) {
    dimension = new_point->getDimension();
  }
  // If lof_data.dimension is fixed, compare with new_point dimension to confirm consistency
  else if (dimension != new_point->getDimension()) {
    cout << "New point dimension is inconsistent. End the program!" << endl;
    exit(3);
  }
  auto it = all_points.begin();
  for (; it != prev(all_points.end()); it++) {
    (*it)->addNewNeighbor(new_point);
    new_point->insertNeighbor(*it);
  }
  // If group_size is set (!= 0), update stats of the new point
  if (group_size != 0) {
    new_point->updateStats();
  }
}

/* Function removeFrontPoint
 * Input: None
 * Output: None
 * The function removes the first point from the all_points list, free the allocated memory
 * and update neighbors of all remaining points
 */
void Lof::removeFrontPoint() {
  Point* front_point = all_points.front();
  for (auto it = next(all_points.begin()); it != all_points.end(); it++) {
    (*it)->removeNeighbor(front_point);
  }
  delete front_point;
  all_points.pop_front();
  point_count--;
}

/* Function changeGroupSize: the function sets value of k to given integer. k is the value of group size. 
 * Input: an integer k_val
 * Output: None
 * The function will adjust other fields of point related to k, including kth_neighbor_distance, ave_k_neighbor_distance, 
 * ave_reachability_distance
 */
void Lof::changeGroupSize(int k_val) {
  group_size = k_val;
  
  // In the first iteration, update fields k, kth_neighbor_distance and ave_k_neighbor_distance of all existing points
  auto it = all_points.begin();
  while (it != all_points.end()) {
    (*it)->setK(k_val);
    (*it)->setKthNeighborDistance((*it)->calcKthNeighborDistance());
    (*it)->setAveKNeighborDistance((*it)->calcAveKDistance());
    it++;
  }
  
  // In the second iteration, update ave_reachability_distance field of all points
  it = all_points.begin();
  while (it != all_points.end()) {
    double ar = (*it)->calcAveReachabilityDistance();
    (*it)->setAveReachabilityDistance((*it)->calcAveReachabilityDistance());
    (*it)->setLofVal((*it)->calcLof());
    it++;
  }

}

/* Function decideGroupSize: the function decides the group_size which maximize the Lof score of the last point
 * Input: None
 * Output: an integer of best group_size;
 * The range of group_size the function will test is [2, 32]
 */
int Lof::decideGroupSize() {
  Point* p = all_points.back();
  double max_lof = 0;
  double lof = 0;
  int max_lof_k = 0;
  int test_k = 2;
  for (test_k = 2; test_k <= window_size / 2; test_k += window_size / 32) {
    changeGroupSize(test_k);
    lof = p->calcLof();
    if (lof > max_lof) {
      max_lof = lof;
      max_lof_k = test_k;
    }
    cout << "test_k = " << test_k << ", lof = " << lof <<endl;
  }
  cout << "best k = " << max_lof_k << endl;
  return max_lof_k;
}

/* Function decideThreshold: the function decides the threshold value of the lof algorithm
 * Input: None
 * Output: a double threshold value
 * Set threshold as 100% higher than the median of lof_val of all existing points
 */
double Lof::decideThreshold() {
  // Collect all lof_vals in a vector lof_vals, sort the vector and find the median
  vector<double> lof_vals;
  auto it = all_points.begin();
  while (*it != all_points.back()) {
    lof_vals.push_back((*it)->getLofVal());
    it++;
  }
  sort(lof_vals.begin(), lof_vals.end());
  int mid_idx = window_size / 2;
  
  double t = 2 * lof_vals[mid_idx];
  return t;
}

/* Function isOutlier: decides if the last point of all_points is an outlier
 * Input: None
 * Output: True if the lof_val of the last point is higher than threshold, indicating the last point is an outlier
 * False if otherwise
 */
bool Lof::isOutlier() {
  Point* last_point = all_points.back();
  double lof_val = last_point->getLofVal();
  if (lof_val > threshold) {
    last_point->displayPoint();
    cout << "lof_val = " << lof_val << ", it is an outlier" << endl;
    return true;
  }
  return false;
}










