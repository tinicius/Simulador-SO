#ifndef POLICY_SRTN_HPP
#define POLICY_SRTN_HPP

#include <list>

#include "Policy.hpp"
#include "globals.hpp"

using namespace std;

class PolicySRTN : public Policy {
 private:
  list<int> ready;

 public:
  int get_next_process_pid() override;
  void add_ready(int pid) override;
  int get_ready_size() override;
};

#endif