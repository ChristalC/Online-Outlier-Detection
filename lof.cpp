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
 * Output: True if the point is added successfully; False if otherwise
 * The function calculates distance between new point and existing points, updates affected points
 * establishes neighbors of the new point. 
 * If new point is after window size, it will decide if the new point is an outlier.
 */
bool Lof::addPoint(vector<double> new_pos) {
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
    return false;
  }
  auto it = all_points.begin();
  for (; it != prev(all_points.end()); it++) {
    (*it)->addNewNeighbor(new_point);
    new_point->insertNeighbor(*it);
  }
  bool is_outlier = false;
  // If group_size is set (!= 0), update stats of the new point
  // If group_size is fixed, which means decision is needed on if the new_point is an outlier
  if (group_size != 0) {
    new_point->updateStats();
    double lof_val = new_point->getLofVal();
    if (lof_val > threshold) {
      cout << "Found an outlier: ";
      new_point->displayPoint();
    }
    removeFrontPoint();
  }
  // If the group_size is not fixed, count if the new_point is outside the first window
  // If it is, then we need to decide group_size and threshold at this point
  // If not, no action or decision needed
  else {  
    if (getPointCount() == window_size + 1) {
      int best_group_size = decideGroupSize();
      changeGroupSize(best_group_size);

      double th = decideThreshold();
      setThreshold(th);

      // Decide if the newly added point is an outlier based on group_size and threshold
      double lof_val = new_point->getLofVal();
      if (lof_val > threshold) {
        cout << "Found an outlier: ";
        new_point->displayPoint();
      }
      removeFrontPoint();
    }
  }
  return true;
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
    // (*it)->setAveKNeighborDistance((*it)->calcAveKDistance());
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
  int group_num = 2;
  while (group_num <= 10) {
    test_k = window_size / group_num;
    changeGroupSize(test_k);
    lof = p->calcLof();
    if (lof > max_lof) {
      max_lof = lof;
      max_lof_k = test_k;
    }
    // cout << "test_k = " << test_k << ", lof = " << lof <<endl;
    group_num++;
  }
  // cout << "best k = " << max_lof_k << endl;
  return max_lof_k;
}

/* Function decideThreshold: the function decides the threshold value of the lof algorithm
 * Input: None
 * Output: a double threshold value
 * Set threshold as higher than 90% of lof_val of all existing points
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
  
  double t = lof_vals[window_size * 0.9];
  return t;
}

