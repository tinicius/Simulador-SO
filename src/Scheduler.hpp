#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <pthread.h>

#include <algorithm>
#include <iostream>
#include <vector>

#include "entities.hpp"

using namespace std;

extern pthread_mutex_t running_mutex;

class Scheduler {
 private:
  vector<int> ready;  // PIDs of ready processes

 public:
  Scheduler();

  int get_next_process_pid();

  void add_ready(int pid);

  int get_ready_size();
};

#endif