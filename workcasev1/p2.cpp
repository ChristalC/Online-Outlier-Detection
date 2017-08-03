// Source file for project 2
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <pthread.h>
#include <list>
#include <vector>

#include "p2_helper.h"
#include "thread_org.h"

using namespace std;


int main() {
  // Read input from stdin to get window size, host and port
  int ws;
  string host_str, port_str;

  if (!readInput(ws, host_str, port_str)) {
    cout << "Invalid Input. The program is ended!" << endl;
    exit(1);
  }

  const char* host = host_str.c_str();
  const char* port = port_str.c_str();

  thread_org_func(ws, host, port);

  return 0;
}

