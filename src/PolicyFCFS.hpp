#ifndef POLICY_FCFS_HPP
#define POLICY_FCFS_HPP

#include <queue>

#include "Policy.hpp"

using namespace std;

class PolicyFCFS : public Policy {
 private:
  queue<int> ready;

 public:
  int get_next_process_pid() override;
  void add_ready(int pid) override;
  int get_ready_size() override;
};

#endif