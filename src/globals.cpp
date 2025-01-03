#include "globals.hpp"

int SIGNAL;

int PROGRAMS_COUNT;
int CORES_COUNT;

queue<int> next_process;
pthread_mutex_t next_process_mutex = PTHREAD_MUTEX_INITIALIZER;

queue<int> ready_processes;

map<int, Process> processes_map;

long get_now_in_milliseconds() {
  return chrono::duration_cast<chrono::microseconds>(
             chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}

vector<int> core_process_map;