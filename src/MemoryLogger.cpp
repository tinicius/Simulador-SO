#include "MemoryLogger.hpp"

MemoryLogger* MemoryLogger::instance = nullptr;

MemoryLogger* MemoryLogger::getInstance(Ram* ram, Cache* cache) {
  if (instance == nullptr) {
    instance = new MemoryLogger();
    instance->ram = ram;
    instance->cache = cache;
    instance->open_log_file();
  }
  return instance;
}

void MemoryLogger::open_log_file() {
  log_file.open("./out/memory_operations.log", ios::trunc);
  log_file << "\n=== New Execution Session ===" << endl;
}

void MemoryLogger::log_memory_operation(const string& op, int address,
                                        int value) {
  log_file << "\n[Memory Operation] "
           << "Time: " << time(nullptr) << endl
           << "Operation: " << op << endl
           << "Address: " << address << endl
           << "Value: " << value << endl;
}

void MemoryLogger::log_quantum_expired(int pid, int remaining_time) {
  log_file << "\n[Quantum Expired] "
           << "Process: " << pid << endl
           << "Remaining Time: " << remaining_time << endl;
}

void MemoryLogger::log_process_finished(int pid, int result) {
  log_file << "\n[Process Finished] "
           << "Process: " << pid << endl
           << "Final Result: " << result << endl;
}

void MemoryLogger::close_log_file() {
  if (log_file.is_open()) {
    log_file.flush();
    log_file.close();
  }
}

void MemoryLogger::log_final_state() {
  if (!ram || !cache || !log_file.is_open()) {
    return;
  }

  log_file << "\n=== Final Memory State ===" << endl;
  log_file << "RAM Contents:" << endl;

  // Print RAM contents
  for (int i = 0; i < 32; i++) {
    log_file << "Address " << setw(2) << i << ": " << setw(5)
             << ram->get_value(i) << endl;
  }

  // Print Cache contents
  log_file << "\nCache Contents:" << endl;
  for (int i = 0; i < CACHE_SIZE; i++) {
    CacheBlock block = cache->get_block(i);
    if (cache && block.valid) {
      log_file << "Block " << setw(2) << i << ": Tag=" << block.tag
               << " Data=" << block.data << " Dirty=" << block.dirty << endl;
    }
  }

  log_file << "\n=== End of Memory State ===" << endl;
  log_file.flush();
}

void MemoryLogger::log_pcbs_state() {
  log_file << "\n=== PCBs State ===" << endl;

  for (int i = 0; i < 32; i++) {
    auto pcb = ram->get_PCB(i);

    log_file << "PID: " << pcb.pid << endl;
    log_file << "Priority: " << pcb.priority << endl;
    log_file << "Code Address: " << pcb.code_address << endl;
    log_file << "Code Size: " << pcb.code_size << endl;
    log_file << "PC: " << pcb.PC << endl;
    log_file << "State: " << pcb.state << endl;
    log_file << endl;
  }

  log_file << "\n=== End of PCBs State ===" << endl;
  log_file.flush();
}