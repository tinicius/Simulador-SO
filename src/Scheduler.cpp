#include "Scheduler.hpp"

pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;

Scheduler::Scheduler() {}

Scheduler::Scheduler(vector<Process>& processes) {
  for (auto p : processes) this->running.push_back(p.get_pid());
}

int Scheduler::get_next_process_pid() {
  pthread_mutex_lock(&running_mutex);

  if (this->running.size() == 0) {
    pthread_mutex_unlock(&running_mutex);
    return -1;
  }

  int pid = this->running[0];
  this->running.erase(this->running.begin());

  pthread_mutex_unlock(&running_mutex);
  return pid;
}

void Scheduler::add_running(int pid) {
  pthread_mutex_lock(&running_mutex);
  this->running.push_back(pid);
  pthread_mutex_unlock(&running_mutex);
}