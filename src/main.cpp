#include <vector>

#include "Cpu.hpp"
#include "MemoryLogger.hpp"
#include "OperatingSystem.hpp"
#include "Ram.hpp"
#include "entities.hpp"

// Variáveis globais para o handler
extern Ram* global_ram;
extern Cache* global_cache;

void signal_handler(int signum) {
  if (global_ram && global_cache) {
    auto logger = MemoryLogger::getInstance(global_ram, global_cache);
    logger->log_final_state();

    logger->close_log_file();
  }

  // Print final state
  ofstream data_file("/out/process.log", ios::app);

  data_file << endl << endl;

  for (auto [pid, process] : processes_map) {
    data_file << "Processo: " << pid << endl;
    data_file << "Cpu time: " << process.cpu_time << endl;
    data_file << "Waiting time: " << process.waiting_time << endl;
    data_file << "Timestamp: " << process.cpu_time + process.waiting_time
              << endl;
    data_file << "State: " << process.state << endl;

    data_file << endl;
  }

  data_file << endl;

  data_file.close();

  exit(signum);
}

int main(int argc, char* argv[]) {
  if (argc >= 2) {
    PROGRAMS_COUNT = stoi(argv[1]);
    CORES_COUNT = stoi(argv[2]);
  } else {
    PROGRAMS_COUNT = 1;
    CORES_COUNT = 1;
  }

  signal(SIGINT, signal_handler);

  // Inicializando RAM
  Ram ram;
  global_ram = &ram;  // Salvar referência global

  Cache cache(&ram);
  global_cache = &cache;  // Salvar referência global

  auto logger = MemoryLogger::getInstance(&ram, &cache);
  logger->log_memory_operation("INIT", 0, 0);

  // Inicializando o escalonador
  Scheduler scheduler;

  // Inicializando SO
  OperatingSystem so(&scheduler, &ram);

  pthread_t t_so;
  pthread_create(&t_so, NULL, run_os, &so);
  pthread_join(t_so, NULL);

  return 0;
}