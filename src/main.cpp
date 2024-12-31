#include <vector>

#include "Cpu.hpp"
#include "MemoryLogger.hpp"
#include "OperatingSystem.hpp"
#include "Ram.hpp"
#include "entities.hpp"

#define PROGRAMS_COUNT 1
#define CORES_COUNT 2

// Variáveis globais para o handler
extern Ram* global_ram;
extern Cache* global_cache;

void signal_handler(int signum) {
  if (global_ram && global_cache) {
    auto logger = MemoryLogger::getInstance(global_ram, global_cache);
    logger->log_final_state();

    logger->log_pcbs_state();

    logger->close_log_file();
  }
  exit(signum);
}

int main() {
  signal(SIGINT, signal_handler);
  // Buscando programas do disco
  vector<vector<string>> programs;

  for (int i = 0; i < PROGRAMS_COUNT; i++) {
    ifstream codigo("./dataset/codigo" + to_string(i + 1) + ".txt");

    vector<string> program;
    string temp;

    while (getline(codigo, temp)) program.push_back(temp);

    programs.push_back(program);
  }

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

  for (int i = 0; i < (int)programs.size(); i++) {
    vector<string> program = programs[i];

    // Inserindo programa na RAM
    ram.insert_code(program);

    // Criando PCB
    ProcessControlBlock pcb;
    pcb.pid = i;
    pcb.priority = 1;
    pcb.code_address = i;
    pcb.code_size = program.size();
    pcb.PC = 0;
    pcb.state = READY;
    pcb.waiting_time = 0;
    pcb.cpu_time = 0;
    pcb.timestamp = 0;

    // Inserindo PCB na RAM
    ram.insert_PCB(pcb);

    // Criando Processos
    Process p;
    p.pid = i;
    p.pcb_address = i;
    p.state = READY;

    // Inserindo processo ao escalonador
    scheduler.add_running(p.pid);

    // Inserindo processo no SO
    so.insert_process(p);
  }

  pthread_t t_so;
  pthread_create(&t_so, NULL, run_os, &so);
  pthread_join(t_so, NULL);

  return 0;
}