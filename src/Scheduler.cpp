#include "Scheduler.hpp"

Scheduler::Scheduler() {}

Process* Scheduler::get_next_process(vector<Process>& processes) {
  pthread_mutex_lock(&process_mutex);

  for (auto& p : processes) {
    if (p.get_state() == READY) {
      pthread_mutex_unlock(&process_mutex);
      return &p;
    }
  }

  pthread_mutex_unlock(&process_mutex);
  return nullptr;
}