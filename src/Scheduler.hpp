#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <iostream>
#include <vector>
#include <pthread.h>

#include "Process.hpp"

using namespace std;

extern pthread_mutex_t running_mutex;

class Scheduler {
 private:
  vector<int> running;
  vector<int> ready;
  vector<int> terminated;

 public:
  Scheduler();
  Scheduler(vector<Process>& processes);

  int get_next_process_pid();

  void add_running(int pid);
};

#endif