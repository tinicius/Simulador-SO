#include <vector>

#include "Bootloader.hpp"
#include "Cpu.hpp"
#include "CpuLogger.hpp"
#include "MemoryLogger.hpp"
#include "OperatingSystem.hpp"
#include "Ram.hpp"
#include "entities.hpp"
#include "globals.hpp"

void menu() {
  cout << "========== Menu ==========" << endl;

  cout << "Digite o número de programas: ";
  cin >> PROGRAMS_COUNT;

  cout << "Digite o número de núcleos: ";
  cin >> CORES_COUNT;

  cout << "Digte o quantum (em número de instruções): ";
  cin >> QUANTUM;

  QUANTUM *= 5;

  cout << endl;

  cout << "[1] FCFS (First Come First Service)" << endl;
  cout << "[2] SJF (Shortest Job First)" << endl;
  cout << "[3] SRTN (Shortest Remaining Time Next)" << endl;
  cout << "[4] Round Robin" << endl;

  cout << endl;

  cout << "Escolha uma política de escalonamento: ";
  cin >> POLICY;

  if (POLICY < 1 || POLICY > 4) {
    cout << "Política inválida. Encerrando o programa." << endl;
    exit(1);
  }

  cout << "==========================" << endl;
  cout << endl;
}

int main(int argc, char* argv[]) {
  // Definindo valores padrão
  menu();

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