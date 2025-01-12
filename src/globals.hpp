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
extern bool LOGS_ENABLED;

extern vector<int> ready_process;

extern vector<Process> processes_map;

extern vector<pthread_mutex_t> core_mutex;

extern vector<vector<int>> cpu_history;
extern vector<vector<int>> process_history;

#endif
