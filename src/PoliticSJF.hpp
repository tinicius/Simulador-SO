#ifndef POLITIC_SJF_HPP
#define POLITIC_SJF_HPP

#include <list>

#include "Politic.hpp"
#include "globals.hpp"

using namespace std;

class PoliticSJF : public Politic {
 private:
  list<int> ready;

 public:
  int get_next_process_pid() override;
  void add_ready(int pid) override;
  int get_ready_size() override;
};

#endif