#include "PoliticFIFO.hpp"

int PoliticFIFO::get_next_process_pid() {
  if (ready.empty()) return -1;

  int pid = ready.front();
  ready.pop();

  return pid;
}

void PoliticFIFO::add_ready(int pid) { ready.push(pid); }

int PoliticFIFO::get_ready_size() { return ready.size(); }