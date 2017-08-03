// Header file for thread_org class

#include <pthread.h>
#include <unistd.h>

void thread_org_func(int ws, const char* h, const char* p);

void* socket_thread_func(void* argv);

void* lof_thread_func(void* ws);


