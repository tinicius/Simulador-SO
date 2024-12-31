#ifndef OPERATING_SYSTEM_HPP
#define OPERATING_SYSTEM_HPP

#include <pthread.h>

#include <vector>

#include "Cache.hpp"
#include "Cpu.hpp"
#include "Scheduler.hpp"
#include "globals.hpp"

using namespace std;

class OperatingSystem {
 private:
  vector<Cpu> cores;
  Ram *ram;
  Cache *cache;

 public:
  OperatingSystem(Scheduler *scheduler, Ram *ram);

  vector<Process> processes;
  Scheduler *scheduler;

  void insert_process(Process process);
};

void *run_os(void *arg);

#endif