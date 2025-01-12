#include "globals.hpp"

int PROGRAMS_COUNT;
int CORES_COUNT;
int QUANTUM = 20;  // Quantum em ciclos de clock
int POLICY = 0;
bool LOGS_ENABLED = true;

vector<int> ready_process;

vector<Process> processes_map;

vector<pthread_mutex_t> core_mutex;

vector<vector<int>> cpu_history;
vector<vector<int>> process_history;