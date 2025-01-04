#ifndef OPERATING_SYSTEM_HPP
#define OPERATING_SYSTEM_HPP

#include <pthread.h>

#include <vector>

#include "Bootloader.hpp"
#include "Cache.hpp"
#include "Cpu.hpp"
#include "Scheduler.hpp"
#include "globals.hpp"

using namespace std;

class OperatingSystem {
 private:
  
  Ram ram;
  Cache *cache;

 public:
  OperatingSystem();
  ~OperatingSystem();

  Scheduler scheduler;

  vector<Cpu> cores;

  void boot();

  void init_cores();

  bool check_finished();

  void log_processes_state();
};

void *run_os(void *arg);

#endif