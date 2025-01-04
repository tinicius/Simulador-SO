#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <pthread.h>

#include <map>
#include <queue>

#include "Cache.hpp"
#include "entities.hpp"

using namespace std;

#define endl '\n'

#define QUANTUM 5  // Quantum em ciclos de clock

extern int PROGRAMS_COUNT;
extern int CORES_COUNT;

extern vector<int> ready_process;

extern vector<Process> processes_map;

extern vector<pthread_mutex_t> core_mutex;

#endif
