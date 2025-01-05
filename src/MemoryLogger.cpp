#include "MemoryLogger.hpp"

MemoryLogger::MemoryLogger(Ram* ram) : ram(ram) {}

void MemoryLogger::print() {
  ofstream log_file("./output/memory_operations.log", ios::trunc);

  log_file << "\n=== Final Memory State ===" << endl;
  log_file << "RAM Contents:" << endl;

  for (int i = 0; i < 32; i++) {
    log_file << "Address " << setw(2) << i << ": " << setw(5)
             << ram->get_value(i) << endl;
  }

  log_file << "\n=== End of Memory State ===" << endl;
  log_file.flush();

  log_file.close();
}
