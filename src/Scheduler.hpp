#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <pthread.h>

#include <algorithm>
#include <iostream>
#include <list>

#include "Policy.hpp"
#include "entities.hpp"
#include "globals.hpp"

using namespace std;

extern pthread_mutex_t running_mutex;

class Scheduler {
 private:
  Policy *policy;

 public:
  Scheduler(Policy *policy);
  ~Scheduler() { delete policy; }

  int get_next_process_pid();

  void add_ready(int pid);

  int get_ready_size();
};

#endif