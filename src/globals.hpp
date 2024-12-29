#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <pthread.h>

#include "Process.hpp"

#define endl '\n'

extern Process* next_process;
extern pthread_mutex_t next_process_mutex;
extern pthread_cond_t core_cond;

extern pthread_mutex_t process_mutex;

#endif
