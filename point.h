// Header file for Point class
#ifndef POINT_H
#define POINT_H

#include <set>
#include <utility>
#include <vector>

class Point {
  private:
    vector<double> pos;
    int dimension;
    int k;
    int num_neighbors;
    set<pair<point*, double>, cmpPair> neighbors;
    double kth_neighbor_distance;
  public:
    // Constructor
    point(vector<double> new_pos);

    addNeighbor(Point* new_point);
    removeNeighbor(Point* remove_point);




    static double distance(Point* p1, Point* p2);
}

static bool cmpPair(pair<point*, double> p1, pair<point*, double> p2) {
  if (p1.second < p2.second) {
    return true;
  }
  else if (p1.second == p2.second) {
    return p1.first < p2.first;
  }
  return false;
}

#endif
