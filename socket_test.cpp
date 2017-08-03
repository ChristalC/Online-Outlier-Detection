// Source file for project 2
#include <iostream>
#include <istream>
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

// #include "p2_helper.h"
// #include "thread_org.h"
// #include "lof.h"

using namespace std;


pthread_cond_t has_new;
pthread_mutex_t lock;
list<vector<double>> shared_data;

int main() {
  const unsigned int MAX_BUF_LENGTH = 1000;
  string port_str;
  const char* host = "localhost";
  cout << "Enter port#: ";
  getline(cin, port_str);
  const char* port = port_str.c_str();
  int ws = 365;

  // Get ready for connecting to server
  int sockfd, numbytes;
  char recv_buffer[MAX_BUF_LENGTH];
  string recv_str;
  struct addrinfo hints, *servinfo, *p;
  int rv;

  hints = {};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
    cout << "Error: getaddrinfor: " << gai_strerror(rv) << endl;
    return 1;
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      cout << "socket error\n";
      continue;
    }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      cout << "ai_addr = " << p->ai_addr << endl;
      cout << "client: connect error\n";
      continue;
    }

    break;
  }

  if (p == NULL) {
    cout << "Failed to connect\n";
    return 2;
  }

  freeaddrinfo(servinfo);

  while ((numbytes = recv(sockfd, recv_buffer, MAX_BUF_LENGTH - 1, 0)) > 1) {
    // cout << "numbytes = " << numbytes << endl;
    recv_buffer[numbytes] = '\0';
    recv_str = recv_buffer;
    // Split recv_str into a vector of doubles, push the vector to shared_data list
    /*
    vector<double> read_point = splitCsv(recv_str);
    if (read_point.size() == 0) {
      cout << "read invalid point! End the program!" << endl;
      exit(1);
    }
*/
    cout << recv_str;
  }
  if (numbytes == -1) {
    cout << "recv error\n";
    exit(1);
  }

  close(sockfd);
  /*
  int ws = 365
  Lof lof_data(ws);
  ifstream fpIn;
  fpIn.open("test_nums");
  while ()
*/
  return 0;
}

/*
  const unsigned int MAX_BUF_LENGTH = 100;
  string port_str;
  const char* host = "localhost";
  cout << "Enter port#: ";
  getline(cin, port_str);
  const char* port = port_str.c_str();
  int ws = 365;

  // Get ready for connecting to server
  int sockfd, numbytes;
  char recv_buffer[MAX_BUF_LENGTH];
  string recv_str;
  struct addrinfo hints, *servinfo, *p;
  int rv;

  hints = {};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
    cout << "Error: getaddrinfor: " << gai_strerror(rv) << endl;
    return 1;
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      cout << "socket error\n";
      continue;
    }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      cout << "ai_addr = " << p->ai_addr << endl;
      cout << "client: connect error\n";
      continue;
    }

    break;
  }

  if (p == NULL) {
    cout << "Failed to connect\n";
    return 2;
  }

  freeaddrinfo(servinfo);

  while ((numbytes = recv(sockfd, recv_buffer, MAX_BUF_LENGTH - 1, 0)) > 1) {
    // cout << "numbytes = " << numbytes << endl;
    recv_buffer[numbytes] = '\0';
    recv_str = recv_buffer;
    // Split recv_str into a vector of doubles, push the vector to shared_data list
    vector<double> read_point = splitCsv(recv_str);
    if (read_point.size() == 0) {
      cout << "read invalid point! End the program!" << endl;
      exit(1);
    }

    cout << recv_str;
  }
  if (numbytes == -1) {
    cout << "recv error\n";
    exit(1);
  }

  close(sockfd);
*/
