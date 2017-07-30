// Source file for project 2
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "p2_helper.h"

using namespace std;

// bool readInput(int& window_size, string& host, string& port);

int main() {
  const unsigned int MAX_BUF_LENGTH = 100;
  const char* host = "localhost";
  const char* port = "45883";

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
    recv_buffer[numbytes] = '\0';
    recv_str = recv_buffer;
    cout << recv_str;
  }
  if (numbytes == -1) {
    cout << "recv error\n";
    exit(1);
  }

  close(sockfd);


  return 0;
}

/* Function readInput: read from stdin the window size and host:port
 * Input: reference parameter window_size, host and port
 * Output: true if the input read are correct; false otherwise
 */
/*
bool readInput(int& window_size, string& host, string& port) {
  string holder;
  vector<string> read_input;

  // Neglect all comment lines
  do {
    if (getline(cin, holder)) {
      read_input = splitString(holder);
    }
    else {
      return false;
    }
  } while (read_input.size() == 0);

  // Read the first line as window_size. 
  // If a number is read, update the window_size
  // If it is not a number, return false
  if (read_input.size() == 1 && isWindowSize(read_input)) {
    window_size = stoi(read_input[0], NULL, 10);
  }
  else {
    cout << "Window size is incorrect!\n";
    return false;
  }

  // Neglect comment lines
  do {
    if (getline(cin, holder)) {
      read_input = splitString(holder);
    }
    else {
      return false;
    }
  } while (read_input.size() == 0);

  // Read the second line as host and port, marked by a ':' in between
  // If correctly read, update host and port
  // If not, return false
  if (read_input.size() != 1) {
    cout << "Input host:port is invalid!\n";
    return false;
  }
  holder = read_input[0];
  int pos_colon = holder.find_first_of(":");
  if (pos_colon == string::npos) {
    return false;
  }
  host = holder.substr(0, pos_colon);
  port = holder.substr(pos_colon + 1);

  // Check if port is a number, if not, return false
  if (!isNum(port)) {
    cout << "Invalid port entered\n";
    return false;
  }
  
  return true;
}
*/
