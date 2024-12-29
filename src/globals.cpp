#include "globals.hpp"

Process* next_process = nullptr;
pthread_mutex_t next_process_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t core_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t process_mutex = PTHREAD_MUTEX_INITIALIZER;
