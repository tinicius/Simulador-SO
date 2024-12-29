#ifndef OPERATING_SYSTEM_HPP
#define OPERATING_SYSTEM_HPP

#include <pthread.h>

#include <vector>

#include "Cpu.hpp"
#include "Process.hpp"
#include "Scheduler.hpp"

using namespace std;

#define CORES_COUNT 4

class OperatingSystem {
 private:
    vector<Cpu> cores;

 public:
  OperatingSystem(Scheduler scheduler);

  vector<Process> processes;
  Scheduler scheduler;

  void boot();
};

void *run_os(void *arg);

#endif