#include "Scheduler.hpp"

using namespace std;

pthread_mutex_t running_mutex = PTHREAD_MUTEX_INITIALIZER;

Scheduler::Scheduler() {}

Scheduler::Scheduler(vector<Process>& processes) {
  for (auto p : processes) this->running.push_back(p.pid);
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

void Scheduler::block_process(int pid) {
    pthread_mutex_lock(&running_mutex);
    
    // Remove from running
    auto it = find(running.begin(), running.end(), pid);
    if (it != running.end()) {
        running.erase(it);
        blocked.push_back(pid);
    }
    
    pthread_mutex_unlock(&running_mutex);
}

void Scheduler::unblock_process(int pid) {
    pthread_mutex_lock(&running_mutex);
    
    // Move from blocked to ready
    auto it = find(blocked.begin(), blocked.end(), pid);
    if (it != blocked.end()) {
        blocked.erase(it);
        ready.push_back(pid);
    }
    
    pthread_mutex_unlock(&running_mutex);
}

void Scheduler::terminate_process(int pid) {
    pthread_mutex_lock(&running_mutex);
    
    // Move to terminated
    auto it = find(running.begin(), running.end(), pid);
    if (it != running.end()) {
        running.erase(it);
        terminated.push_back(pid);
    }
    
    pthread_mutex_unlock(&running_mutex);
}