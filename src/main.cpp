#include <vector>

#include "Cpu.hpp"
#include "MemoryLogger.hpp"
#include "OperatingSystem.hpp"
#include "Ram.hpp"
#include "entities.hpp"
#include "globals.hpp"

int main(int argc, char* argv[]) {
  // Definindo valores padrão
  PROGRAMS_COUNT = 1;
  CORES_COUNT = 1;
  QUANTUM = 10;

  // Definindo o número de programas
  if (argc == 2) {
    PROGRAMS_COUNT = stoi(argv[1]);
  }

  // Definindo o número de programas e de núcleos
  if (argc == 3) {
    PROGRAMS_COUNT = stoi(argv[1]);
    CORES_COUNT = stoi(argv[2]);
  }

  // Definindo o número de programas, de núcleos e o quantum
  if (argc == 4) {
    PROGRAMS_COUNT = stoi(argv[1]);
    CORES_COUNT = stoi(argv[2]);
    QUANTUM = stoi(argv[3]);
  }

  cpu_history.resize(CORES_COUNT);
  process_history.resize(PROGRAMS_COUNT);

  // Inicializando SO
  OperatingSystem so;
  so.boot();

  return 0;
}