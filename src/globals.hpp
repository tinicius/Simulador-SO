#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <pthread.h>
#include <queue>

#include "entities.hpp"

using namespace std;

#define endl '\n'

extern queue<Process> next_process;
extern pthread_mutex_t next_process_mutex;

#endif
