#include "PolicySRTN.hpp"

int PolicySRTN::get_next_process_pid() {
  if (ready.empty()) return -1;

  int min_remaning = 1e9;
  int pid = -1;

  for (int pid_r : ready) {
    Process process = processes_map[pid_r];

    if (process.remaining_instructions < min_remaning) {
      min_remaning = process.size;
      pid = process.pid;
    }
  }

  ready.remove(pid);

  return pid;
}

void PolicySRTN::add_ready(int pid) { ready.push_back(pid); }

int PolicySRTN::get_ready_size() { return ready.size(); }