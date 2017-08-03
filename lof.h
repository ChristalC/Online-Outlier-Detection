// Header file for Lof class

#include <list>
#include <vector>
#include "point.h"


class Lof {
  private:
    int dimension;
    int window_size;
    int group_size;
    int point_count;
    double threshold;
    std::list<Point*> all_points;

  public:
    // Constructor
    Lof(int ws);

    // Destructor
    ~Lof();

    // Getters
    int getDimension() {return dimension;}
    int getWindowSize() {return window_size;}
    int getGroupSize() {return group_size;}
    int getPointCount() {return point_count;}
    double getThreshold() {return threshold;}

    // Setters
    void setDimension(int d) {dimension = d;}
    void setWindowSize(int ws) {window_size = ws;}
    void setGroupSize(int gs) {group_size = gs;}
    void setThreshold(double t) {threshold = t;}

    bool addPoint(std::vector<double> new_pos);
    void removeFrontPoint();


    void changeGroupSize(int k_val);
    int decideGroupSize();
    double decideThreshold();
};
