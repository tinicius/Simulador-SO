#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <pthread.h>

#include <queue>

#include "Cache.hpp"
#include "entities.hpp"

using namespace std;

#define endl '\n'

#define QUANTUM 5  // Quantum em ciclos de clock

#define PROGRAMS_COUNT 1
#define CORES_COUNT 2

extern queue<Process> next_process;
extern pthread_mutex_t next_process_mutex;

extern queue<Process> ready_processes;
extern pthread_mutex_t ready_processes_mutex;
extern Cache* global_cache;
extern Ram* global_ram;
#endif
