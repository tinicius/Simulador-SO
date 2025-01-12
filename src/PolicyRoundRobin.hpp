#ifndef ROUND_ROBIN_HPP
#define ROUND_ROBIN_HPP

#include <vector>

#include "Policy.hpp"
#include "globals.hpp"

using namespace std;

class PolicyRoundRobin : public Policy {
 private:
  vector<int> ready;
  int current;

 public:
  PolicyRoundRobin();
  ~PolicyRoundRobin();

  int get_next_process_pid();
  void add_ready(int pid);
  int get_ready_size();
};

#endif