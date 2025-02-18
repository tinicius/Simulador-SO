#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <pthread.h>

#include <map>
#include <queue>

#include "Cache.hpp"
#include "entities.hpp"

using namespace std;

#define endl '\n'

extern int PROGRAMS_COUNT;
extern int CORES_COUNT;
extern int QUANTUM;  // Quantum em ciclos de clock
extern int POLICY;
extern int CACHE_TYPE;
extern int CACHE_SIZE;
extern int ENABLED_GROUPING;
extern int CACHE_HIT;
extern int CACHE_MISS;
extern bool LOGS_ENABLED;
extern int SWAP_COUNT;

extern vector<int> ready_process;

extern vector<Process> processes_map;

extern vector<pthread_mutex_t> core_mutex;
extern pthread_mutex_t cache_mutex;

extern vector<vector<int>> cpu_history;
extern vector<vector<int>> process_history;

#endif
