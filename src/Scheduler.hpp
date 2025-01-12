#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <pthread.h>

#include <algorithm>
#include <iostream>
#include <list>

#include "Politic.hpp"
#include "entities.hpp"
#include "globals.hpp"

using namespace std;

extern pthread_mutex_t running_mutex;

class Scheduler {
 private:
  Politic *politic;

 public:
  Scheduler(Politic *politic);
  ~Scheduler() { delete politic; }

  int get_next_process_pid();

  void add_ready(int pid);

  int get_ready_size();
};

#endif