#include "PoliticSJF.hpp"

int PoliticSJF::get_next_process_pid() {
  if (ready.empty()) return -1;

  int min_size = 1e9;
  int pid = -1;

  for (int pid_r : ready) {
    Process process = processes_map[pid_r];

    if (process.size < min_size) {
      min_size = process.size;
      pid = process.pid;
    }
  }

  ready.remove(pid);

  return pid;
}

void PoliticSJF::add_ready(int pid) { ready.push_back(pid); }

int PoliticSJF::get_ready_size() { return ready.size(); }