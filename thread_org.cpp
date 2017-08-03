// Implementation file for socket_thread class

#include "thread_org.h"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <cstring>

#include "p2_helper.h"
#include "lof.h"

#define MAX_BUF_LENGTH 1000

pthread_cond_t has_new;
pthread_mutex_t lock;
list<vector<double>> shared_data;
int point_count = 0;

/* Function thread_org_func: initializes variables for the two thread functions. Create two thread functions.
 * Input: an integer window_size, two const char* host and port
 * Output: None
 */
void thread_org_func(int ws, const char* h, const char* p) {
  // Initialize the mutex before trying to use it
  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&has_new, NULL);

  // Push host and port to the arguments of socket_thread
  Socket_arg sa;
  sa.h = h;
  sa.p = p;

  // Initialize socket_thread
  pthread_t socket_thread;
  int socket_thread_res = pthread_create(&socket_thread, NULL, socket_thread_func, &sa);
  if (socket_thread_res != 0) {
    cout << "Socket_thread creation failed!" << endl;
    exit(1);
  }

  // Initialize lof_thread
  pthread_t lof_thread;
  int lof_thread_res = pthread_create(&lof_thread, NULL, lof_thread_func, &ws);
  if (lof_thread_res != 0) {
    cout << "lof_thread creation failed!" << endl;
    exit(1);
  }

  int socket_thread_end_res = pthread_join(socket_thread, NULL);
  int lof_thread_end_res = pthread_join(lof_thread, NULL);
  if (socket_thread_end_res != 0) {
    cout << "socket_thread_end failed!" << endl;
    exit(1);
  }
  if (lof_thread_end_res != 0) {
    cout << "lof_thread_end failed!" << endl;
    exit(1);
  }
  return;
}


/* Function socket_thread_func: creates a socket with given host and port, receives data as string, 
 * split the string into vector of doubles, and send the vector to shared data with lof_thread
 * Input: const char* host and port
 * Output: None
 */
void* socket_thread_func(void* argv) {
  Socket_arg* sa = (Socket_arg*)argv;
  const char* host = sa->h;
  const char* port = sa->p;

  // Get ready for connecting to server
  int sockfd, numbytes;
  struct addrinfo hints, *servinfo, *p;
  int rv;

  hints = {};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
    cout << "Error: getaddrinfor: " << gai_strerror(rv) << endl;
    exit(1);
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
    exit(2);
  }

  freeaddrinfo(servinfo);

  char recv_buffer[MAX_BUF_LENGTH];
  char buffer[MAX_BUF_LENGTH];
  string recv_str;
  string data_str;
  int buffer_start = 0;
  char* recv_start_pos = recv_buffer;
  char* recv_newline_pos;
  char* recv_end_pos;
  char* buffer_start_pos = buffer;
  char* buffer_end_pos;
  while ((numbytes = recv(sockfd, recv_buffer, MAX_BUF_LENGTH - 1, 0)) > 1) {
    recv_end_pos = recv_buffer + numbytes;
    *recv_end_pos = '\0';

    // Find '\n' in recv_buffer
    recv_start_pos = recv_buffer;
    recv_newline_pos = strchr(recv_buffer, '\n');
    while (recv_newline_pos != NULL) {
      strncpy(buffer_start_pos, recv_start_pos, recv_newline_pos - recv_start_pos);
      buffer_end_pos = buffer_start_pos + (recv_newline_pos - recv_start_pos);
      *buffer_end_pos = '\0';
      data_str = buffer;
      
      cout << "data_str = " << data_str << endl;
      
      // Split data_str into a vector of doubles, push the vector to shared_data list
      vector<double> read_point = splitCsv(data_str);
      if (read_point.size() == 0) {
        cout << "read invalid point! End the program!" << endl;
        exit(1);
      }
      pthread_mutex_lock(&lock);
      shared_data.push_back(read_point);
      point_count++;
      cout << "point_count = " << point_count << endl;
      pthread_mutex_unlock(&lock);

      // Signal the has_new condition
      pthread_cond_signal(&has_new);

      buffer_start_pos = buffer;
      recv_start_pos = recv_newline_pos + 1;

      // If the previous found recv_newline_pos is already the last char of recv_buffer
      // Jump out of the loop
      if (recv_start_pos == recv_end_pos) {
        break;
      }
      recv_newline_pos = strchr(recv_start_pos, '\n');
    }
    if (recv_start_pos == recv_end_pos) {
      continue;
    }
    strcpy(buffer_start_pos, recv_start_pos);
    buffer_start_pos = strchr(buffer, '\0');
  }
  if (numbytes == -1) {
    cout << "recv error\n";
    exit(1);
  }
  if (numbytes == 0) {
    pthread_mutex_lock(&lock);
    shared_data.push_back({{-1}});
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&has_new);
  }

  close(sockfd);
  return NULL;
}

/* Function lof_thread_func: the main function to call for lof_thread
 * Input: None
 * Output: None
 * Exit condition: when socket_thread signals there is no more data to come
 */
void* lof_thread_func(void* ws) {
  int ws_int = *(int*)ws;
  Lof lof_data(ws_int);
  
  vector<double> new_point_pos;

  while (true) {
    // Wait for new point come to shared data
    pthread_mutex_lock(&lock);
    if (shared_data.empty()) {
      pthread_cond_wait(&has_new, &lock);
    }
    new_point_pos = shared_data.front();
    shared_data.pop_front();
    point_count--;
    cout << "point_count = " << point_count << endl;
    pthread_mutex_unlock(&lock);
    
    // Check if the first element of new_point_pos is -1
    // If yes, all data is processed, return the thread
    if (new_point_pos[0] == -1) {
      break;
    }
    // Add the new point to all_points list
    lof_data.addPoint(new_point_pos);

    // If group_size is fixed, which means decision is needed on if the new_point is an outlier
    if (lof_data.getGroupSize() != 0) {
      /*
      if (lof_data.isOutlier()) {
        cout << "Newly added point is an outlier." << endl;
      }*/
      lof_data.isOutlier();
      lof_data.removeFrontPoint();
    }
    // If the group_size is not fixed, count if the new_point is outside the first window
    // If it is, then we need to decide group_size and threshold at this point
    // If not, no action or decision needed
    else {  
      if (lof_data.getPointCount() == ws_int + 1) {
        int best_group_size = lof_data.decideGroupSize();
        cout << "best_group_size = " << best_group_size << endl;
        lof_data.changeGroupSize(best_group_size);

        double th = lof_data.decideThreshold();
        cout << "picked threshold = " << th << endl;
        lof_data.setThreshold(th);

        // Decide if the newly added point is an outlier based on group_size and threshold
        lof_data.isOutlier();
        // if (lof_data.isOutlier()) {
          // cout << "Newly added point is an outlier." << endl;
        // }
        lof_data.removeFrontPoint();
      }
    }
  }
  return NULL;
}


















