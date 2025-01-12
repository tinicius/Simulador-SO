#ifndef POLICY_HPP
#define POLICY_HPP

class Policy {
 public:
  virtual ~Policy() {};

  virtual int get_next_process_pid() = 0;
  virtual void add_ready(int pid) = 0;
  virtual int get_ready_size() = 0;
};

#endif