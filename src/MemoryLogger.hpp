#ifndef MEMORY_LOGGER_HPP
#define MEMORY_LOGGER_HPP

#include <signal.h>

#include <iomanip>
#include <iostream>

#include "Cache.hpp"
#include "Ram.hpp"

class MemoryLogger {
 private:
  static MemoryLogger* instance;
  Ram* ram;
  Cache* cache;
  ofstream log_file;

 public:
  static MemoryLogger* getInstance(Ram* ram, Cache* cache);
  ~MemoryLogger();  // Adicionar destrutor

  void log_memory_operation(const string& op, int address, int value);
  void log_state_change(int pid, ProcessState old_state,
                        ProcessState new_state);
  void log_final_state();
  void log_quantum_expired(int pid, int remaining_time);
  void log_process_finished(int pid, int result);
  void open_log_file();
  void close_log_file();
}

;

#endif