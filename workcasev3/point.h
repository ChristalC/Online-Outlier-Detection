// Header file for Point class
#ifndef POINT_H
#define POINT_H

#include <set>
#include <utility>
#include <vector>

class Point {
  private:
    typedef struct PointDistancePair {
      Point* p;
      double dist;
      // Constructor
      PointDistancePair(Point* pt, double dis) {p = pt; dist = dis;}
      // operator overloading
      bool operator<(const PointDistancePair& right) const {
        if (dist < right.dist) {
          return true;
        }
        else if (dist == right.dist) {
          return p < right.p;
        }
        return false;
      }
    } PointDistancePair;
    std::vector<double> pos;
    int dimension;
    int k;
    std::set<PointDistancePair> neighbors;
    int num_neighbors;  // always = total number of points - 1
    double kth_neighbor_distance;
    // double ave_k_neighbor_distance;
    double ave_reachability_distance;
    double lof_val;

  public:
    // Constructor
    Point(std::vector<double> new_pos);

    // Getter
    int getDimension() {return dimension;}
    int getK() {return k;}
    int getNumNeighbors() {return num_neighbors;}
    double getKthNeighborDistance() {return kth_neighbor_distance;}
    // double getAveKNeighborDistance() {return ave_k_neighbor_distance;}
    double getAveReachabilityDistance() {return ave_reachability_distance;}
    double getLofVal() {return lof_val;}

    // Setter
    void setDimension(int d) {dimension = d;}
    void setK(int k_val) {k = k_val;}
    void setNumNeighbors(int n) {num_neighbors = n;}
    void setKthNeighborDistance(double d) {kth_neighbor_distance = d;}
    // void setAveKNeighborDistance(double ad) {ave_k_neighbor_distance = ad;}
    void setAveReachabilityDistance(double ar) {ave_reachability_distance = ar;}
    void setLofVal(double l) {lof_val = l;}

    void insertNeighbor(Point* neighbor_point);
    void addNewNeighbor(Point* new_point);
    void removeNeighbor(Point* remove_point);
    void updateStats();

    void displayPoint();  // Print the point position to stdout

    double calcKthNeighborDistance() const;
    // double calcAveKDistance() const;
    double calcAveReachabilityDistance() const;
    double calcLof() const;
    
    double calcReachabilityDistance(const Point* p1, const Point* p2) const;
    double distance(const Point* p1, const Point* p2) const;
    
};

#endif
