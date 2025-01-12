#ifndef POLITIC_HPP
#define POLITIC_HPP

class Politic {
 public:
  virtual ~Politic() {};

  virtual int get_next_process_pid() = 0;
  virtual void add_ready(int pid) = 0;
  virtual int get_ready_size() = 0;
};

#endif