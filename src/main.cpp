#include <vector>

#include "Cpu.hpp"
#include "MemoryLogger.hpp"
#include "OperatingSystem.hpp"
#include "Ram.hpp"
#include "entities.hpp"
#include "globals.hpp"

int main(int argc, char* argv[]) {
  if (argc >= 2) {
    PROGRAMS_COUNT = stoi(argv[1]);
    CORES_COUNT = stoi(argv[2]);
  } else {
    PROGRAMS_COUNT = 1;
    CORES_COUNT = 1;
  }

  cpu_history.resize(CORES_COUNT);
  process_history.resize(PROGRAMS_COUNT);

  // Inicializando SO
  OperatingSystem so;
  so.boot();

  return 0;
}