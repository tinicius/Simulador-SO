#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>

#include "Process.hpp"
#include "globals.hpp"

using namespace std;

class Cpu {
  //  private:
 public:
  Cpu(int id);

  int id;

  Process get_process();
};

void *run_core(void *arg);

#endif