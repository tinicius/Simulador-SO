#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <pthread.h>

#include <map>
#include <queue>

#include "Cache.hpp"
#include "entities.hpp"

using namespace std;

#define endl '\n'

#define QUANTUM 10  // Quantum em ciclos de clock

extern int PROGRAMS_COUNT;
extern int CORES_COUNT;

extern queue<int> next_process;
extern pthread_mutex_t next_process_mutex;

extern queue<int> ready_processes;
extern pthread_mutex_t ready_processes_mutex;

extern Cache* global_cache;
extern Ram* global_ram;

extern map<int, Process> processes_map;

#endif
