#include "Scheduler.hpp"

using namespace std;

Scheduler::Scheduler() {}

int Scheduler::get_next_process_pid() {
  if (this->ready.size() == 0) return -1;

  int pid = this->ready.front();
  this->ready.pop_front();

  return pid;
}

void Scheduler::add_ready(int pid) { this->ready.push_back(pid); }

int Scheduler::get_ready_size() { return this->ready.size(); }
