
#include <vector>

#include "Cpu.hpp"
#include "OperatingSystem.hpp"
#include "Ram.hpp"
#include "entities.hpp"

#define PROGRAMS_COUNT 2
#define CORES_COUNT 2

int main() {
  // BOOT

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