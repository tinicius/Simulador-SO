#include "globals.hpp"

int PROGRAMS_COUNT;
int CORES_COUNT;

queue<int> next_process;
pthread_mutex_t next_process_mutex = PTHREAD_MUTEX_INITIALIZER;

queue<int> ready_processes;
pthread_mutex_t ready_processes_mutex = PTHREAD_MUTEX_INITIALIZER;

Cache* global_cache = nullptr;
Ram* global_ram = nullptr;

map<int, Process> processes_map;
