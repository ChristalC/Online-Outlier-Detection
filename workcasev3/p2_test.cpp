// Source file for project 2
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <list>
#include <vector>

#include "p2_helper.h"
#include "thread_org.h"
#include "lof.h"

using namespace std;

int main() {
  int ws = 365;
  Lof lof_data(ws);

  ifstream fpIn;
  fpIn.open("test_nums");
  
  string buffer;
  vector<double> new_point_pos;
  int counter = 0;
  
  // read the first point and set the lof_data dimension as the dimension of first point
  if (!getline(fpIn, buffer)) {
    cout << "File is empty." << endl;
    exit(1);
  }
  new_point_pos = splitCsv(buffer);
  lof_data.setDimension(new_point_pos.size());
  lof_data.addPoint(new_point_pos);
  counter++;
  cout << "Point#" << counter << ": ";
  displayPos(new_point_pos);
  while (getline(fpIn, buffer)) {
    new_point_pos = splitCsv(buffer);
    // Check if dimension of new point is consistent with lof_data, if not, report error and exit
    if (new_point_pos.size() != lof_data.getDimension()) {
      cout << "New point dimension is incorrect!" << endl;
      exit(1);
    }

    lof_data.addPoint(new_point_pos);
    counter++;
    cout << "Point#" << counter << ": ";
    displayPos(new_point_pos);
    
    // If point_count == window_size + 1 and group_size is not fixed, 
    // need to decide group size and threshold value
    if (lof_data.getPointCount() == ws + 1 && lof_data.getGroupSize() == 0) {
      cout << "Choose best group size..." << endl;
      int best_group_size = lof_data.decideGroupSize();
      cout << "best_group_size = " << best_group_size << endl;
      lof_data.changeGroupSize(best_group_size);
      double t = lof_data.decideThreshold();
      cout << "picked threshold = " << t << endl;
      lof_data.setThreshold(t);
    }
    
    // If group_size and threshold are fixed, decide if the last point in lof_data is outlier
    // After the decision, remove the front point from lof_data
    if (lof_data.getGroupSize() != 0) {
      if (lof_data.isOutlier()) {
        cout << "newly added point is an outlier" << endl;
      }
      lof_data.removeFrontPoint();
    }
  }

  return 0;
}
