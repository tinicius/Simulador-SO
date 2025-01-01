#include "globals.hpp"

queue<int> next_process;
pthread_mutex_t next_process_mutex = PTHREAD_MUTEX_INITIALIZER;

queue<int> ready_processes;
pthread_mutex_t ready_processes_mutex = PTHREAD_MUTEX_INITIALIZER;

Cache* global_cache = nullptr;
Ram* global_ram = nullptr;

map<int, Process> processes_map;
// pthread_mutex_t processes_map_mutex = PTHREAD_MUTEX_INITIALIZER;

string print_state(ProcessState state) {
  switch (state) {
    case NEW:
      return "NEW";
    case READY:
      return "READY";
    case RUNNING:
      return "RUNNING";
    case BLOCKED:
      return "BLOCKED";
    case TERMINATED:
      return "TERMINATED";
    default:
      return "UNKNOWN";
  }
}

void print_process_states() {
  cout << endl;

  for (auto& process : processes_map) {
    cout << "Process " << process.first
         << " state: " << print_state(process.second.state) << endl;
  }

  cout << endl;
}