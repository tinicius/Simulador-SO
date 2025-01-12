#include "Scheduler.hpp"

using namespace std;

Scheduler::Scheduler(Politic* politic) { this->politic = politic; }

int Scheduler::get_next_process_pid() {
  return this->politic->get_next_process_pid();
}

void Scheduler::add_ready(int pid) { this->politic->add_ready(pid); }

int Scheduler::get_ready_size() { return this->politic->get_ready_size(); }
