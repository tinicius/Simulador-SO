#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include <iostream>

#include "Process.hpp"
#include "globals.hpp"

using namespace std;

class Scheduler {
 public:
  Scheduler();

  Process* get_next_process(vector<Process>& processes);
};

#endif