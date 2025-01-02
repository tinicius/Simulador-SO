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
  vector<Cpu> cores;
  Ram *ram;
  Cache *cache;

 public:
  OperatingSystem(Scheduler *scheduler, Ram *ram);

  Scheduler *scheduler;

  bool check_finished();
};

void *run_os(void *arg);

#endif