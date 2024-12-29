
#include <vector>

#include "Cpu.hpp"
#include "OperatingSystem.hpp"
#include "Ram.hpp"
#include "entities.hpp"

int main() {
  // BOOT

  // Buscando programas do disco
  vector<vector<string>> programs = {{"LOAD 1 2", "LOAD 1 2", "LOAD 1 2"},
                                     {"LOAD 1 2", "LOAD 1 2", "LOAD 1 2"},
                                     {"LOAD 1 2", "LOAD 1 2", "LOAD 1 2"}};

  // Inicializando RAM
  Ram ram;

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
    pcb.PC = 0;

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