#include <vector>

#include "Bootloader.hpp"
#include "CacheFIFO.hpp"
#include "CacheLRU.hpp"
#include "Cpu.hpp"
#include "CpuLogger.hpp"
#include "Disk.hpp"
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

  cout << "Digite o quantum (em número de instruções): ";
  cin >> QUANTUM;

  cout << "Digite o tamanho da cache (em número de instruções): ";
  cin >> CACHE_SIZE;

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

  cout << endl;

  cout << "[1] Nenhum" << endl;
  cout << "[2] FIFO" << endl;
  cout << "[3] LRU" << endl;

  cout << endl;

  cout << "Escolha o tipo da cache: ";
  cin >> CACHE_TYPE;

  if (CACHE_TYPE < 1 || CACHE_TYPE > 3) {
    cout << "Tipo inválido. Encerrando o programa." << endl;
    exit(1);
  }

  cout << endl;

  cout << "Agrupar jobs semelhantes? ([0] Não [1] Sim ): ";
  cin >> ENABLED_GROUPING;

  cout << endl;

  cout << "==========================" << endl;
  cout << endl;
}

CacheType* get_cache_type() {
  switch (CACHE_TYPE) {
    case 1:
      return nullptr;
    case 2:
      return new CacheFIFO();
    case 3:
      return new CacheLRU();

    default:
      return nullptr;
  }
}

int main(int argc, char* argv[]) {
  // Definindo valores padrão
  menu();

  cpu_history.resize(CORES_COUNT);
  process_history.resize(PROGRAMS_COUNT);

  // Inicializando o hardware
  Ram ram;
  Disk disk;

  CacheType* cache_type = get_cache_type();
  Cache cache(cache_type);

  vector<Cpu> cores;

  for (int i = 0; i < CORES_COUNT; i++) {
    if (CACHE_TYPE == 1) {
      cores.push_back(Cpu(i, &ram));
      continue;
    }

    cores.push_back(Cpu(i, &ram, &cache));
  }

  // Inicializando logger
  MemoryLogger memory_logger(&ram);
  CpuLogger::configure_logger();

  // Boot
  Bootloader bootloader;
  auto pids = bootloader.boot(&ram, &disk, "./dataset");

  // Inicializando SO
  OperatingSystem so(&memory_logger, &ram, &disk, cores);
  so.boot(pids);

  return 0;
}