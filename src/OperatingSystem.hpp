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

 public:
  OperatingSystem(Scheduler *scheduler, Ram *ram);

  Scheduler *scheduler;

  void insert_process(Process &process);

  Process get_process_by_pid(int pid);

  Ram *ram;
  Cache *cache;
};

void *run_os(void *arg);
void *run_clock_os(void *arg);

#endif