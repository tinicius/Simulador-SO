#include "Scheduler.hpp"

using namespace std;

pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;

Scheduler::Scheduler() {}

int Scheduler::get_next_process_pid() {
  pthread_mutex_lock(&running_mutex);

  if (this->ready.size() == 0) {
    pthread_mutex_unlock(&running_mutex);
    return -1;
  }

  int pid = this->ready[0];
  this->ready.erase(this->ready.begin());

  pthread_mutex_unlock(&running_mutex);
  return pid;
}

void Scheduler::add_ready(int pid) {
  pthread_mutex_lock(&running_mutex);
  this->ready.push_back(pid);
  pthread_mutex_unlock(&running_mutex);
}

int Scheduler::get_ready_size() { return this->ready.size(); }
