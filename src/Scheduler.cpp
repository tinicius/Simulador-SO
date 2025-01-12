#include "Scheduler.hpp"

using namespace std;

Scheduler::Scheduler(Policy* policy) { this->policy = policy; }

int Scheduler::get_next_process_pid() {
  int next_pid = this->policy->get_next_process_pid();

  if (next_pid == -1) return next_pid;

  processes_map[next_pid].quantum = 200;

  return next_pid;
}

void Scheduler::add_ready(int pid) { this->policy->add_ready(pid); }

int Scheduler::get_ready_size() { return this->policy->get_ready_size(); }
