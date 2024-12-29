#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <pthread.h>
#include <iostream>
#include <vector>

#include "entities.hpp"

using namespace std;

extern pthread_mutex_t running_mutex;

class Scheduler {
 private:
  vector<int> running;     // PIDs of running processes
  vector<int> ready;       // PIDs of ready processes
  vector<int> terminated;  // PIDs of terminated processes

 public:
  Scheduler();
  Scheduler(vector<Process>& processes);

  int get_next_process_pid();

  void add_running(int pid);
};

#endif