#include "PolicyRoundRobin.hpp"

PolicyRoundRobin::PolicyRoundRobin() { current = 0; }

PolicyRoundRobin::~PolicyRoundRobin() {}

int PolicyRoundRobin::get_next_process_pid() {
  if (ready.empty()) return -1;

  int pid = ready[current];
  auto process = processes_map[pid];
  current = (current + 1) % ready.size();

  int count = 0;

  while (process.state != READY) {
    pid = ready[current];
    process = processes_map[pid];
    count++;

    if (count == (int)ready.size()) return -1;

    current = (current + 1) % ready.size();
  }

  return pid;
}

void PolicyRoundRobin::add_ready(int pid) { ready.push_back(pid); }

int PolicyRoundRobin::get_ready_size() { return ready.size(); }