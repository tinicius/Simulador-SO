#include "PolicyFCFS.hpp"

int PolicyFCFS::get_next_process_pid() {
  if (ready.empty()) return -1;

  int pid = ready.front();
  ready.pop();

  return pid;
}

void PolicyFCFS::add_ready(int pid) { ready.push(pid); }

int PolicyFCFS::get_ready_size() { return ready.size(); }