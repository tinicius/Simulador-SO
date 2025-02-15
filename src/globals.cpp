#include "globals.hpp"

int PROGRAMS_COUNT;
int CORES_COUNT;
int QUANTUM;
int POLICY;
int CACHE_TYPE;
int CACHE_SIZE;
int ENABLED_GROUPING;
int CACHE_HIT;
int CACHE_MISS;
bool LOGS_ENABLED = true;

vector<int> ready_process;

vector<Process> processes_map;

vector<pthread_mutex_t> core_mutex;
pthread_mutex_t cache_mutex;

vector<vector<int>> cpu_history;
vector<vector<int>> process_history;