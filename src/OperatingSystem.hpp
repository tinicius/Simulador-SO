#ifndef OPERATING_SYSTEM_HPP
#define OPERATING_SYSTEM_HPP

#include <pthread.h>

#include <vector>

#include "Cpu.hpp"
#include "Scheduler.hpp"

using namespace std;

#define CORES_COUNT 2

class OperatingSystem {
 private:
  vector<Cpu> cores;
  Ram *ram;
  Cache* cache;

 public:
  OperatingSystem(Scheduler* scheduler, Ram *ram);

  vector<Process> processes;
  Scheduler* scheduler;

  void insert_process(Process process);

};

void *run_os(void *arg);

#endif