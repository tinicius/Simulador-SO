#ifndef OPERATING_SYSTEM_HPP
#define OPERATING_SYSTEM_HPP

#include <pthread.h>

#include <vector>

#include "Cpu.hpp"
#include "Scheduler.hpp"
#include "globals.hpp"
#include "PoliticFCFS.hpp"
#include "PoliticSJF.hpp"

using namespace std;

class OperatingSystem {
 private:
  MemoryLogger* memory_logger;
  vector<Cpu> cores;

 public:
  OperatingSystem(MemoryLogger* memory_logger, vector<Cpu>& cores);
  ~OperatingSystem();

  Scheduler* scheduler;

  void boot(vector<int>& pids);

  void init_cores();

  bool check_finished();

  void log_processes_state();

  void log_final();

  Cpu* get_core(int core_id);
};

void* run_os(void* arg);

#endif