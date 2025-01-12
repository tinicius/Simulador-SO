#include "PoliticFCFS.hpp"

int PoliticFCFS::get_next_process_pid() {
  if (ready.empty()) return -1;

  int pid = ready.front();
  ready.pop();

  return pid;
}

void PoliticFCFS::add_ready(int pid) { ready.push(pid); }

int PoliticFCFS::get_ready_size() { return ready.size(); }