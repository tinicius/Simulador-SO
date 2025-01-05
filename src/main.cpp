#include <vector>

#include "Bootloader.hpp"
#include "Cpu.hpp"
#include "CpuLogger.hpp"
#include "MemoryLogger.hpp"
#include "OperatingSystem.hpp"
#include "Ram.hpp"
#include "entities.hpp"
#include "globals.hpp"

void set_arguments(int program_count, int core_count, int quantum) {
  PROGRAMS_COUNT = program_count;
  CORES_COUNT = core_count;
  QUANTUM = quantum;
}

int main(int argc, char* argv[]) {
  // Definindo valores padrão
  switch (argc) {
    case 2:
      set_arguments(stoi(argv[1]), 1, 10);
      break;
    case 3:
      set_arguments(stoi(argv[1]), stoi(argv[2]), 10);
      break;
    case 4:
      set_arguments(stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));
      break;
    default:
      set_arguments(1, 1, 10);
      break;
  }

  cpu_history.resize(CORES_COUNT);
  process_history.resize(PROGRAMS_COUNT);

  // Inicializando o hardware
  Ram ram;
  Cache cache(&ram);

  vector<Cpu> cores;

  for (int i = 0; i < CORES_COUNT; i++) {
    cores.push_back(Cpu(i, &ram, &cache));
  }

  // Inicializando logger
  MemoryLogger memory_logger(&ram);
  CpuLogger::configure_logger();

  // Boot
  Bootloader bootloader;
  auto pids = bootloader.boot(&ram, "./dataset");

  // Inicializando SO
  OperatingSystem so(&memory_logger, cores);
  so.boot(pids);

  return 0;
}