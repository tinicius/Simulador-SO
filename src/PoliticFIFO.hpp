#ifndef POLITIC_FIFO_HPP
#define POLITIC_FIFO_HPP

#include <queue>

#include "Politic.hpp"

using namespace std;

class PoliticFIFO : public Politic {
 private:
  queue<int> ready;

 public:
  int get_next_process_pid() override;
  void add_ready(int pid) override;
  int get_ready_size() override;
};

#endif