#ifndef ENTITIES_HPP
#define ENTITIES_HPP

using namespace std;

#include <vector>

#define REGISTERS_SIZE 32

enum ProcessState {
  NEW,        // Processo acabou de ser criado
  READY,      // Processo pronto para executar
  RUNNING,    // Processo em execução
  BLOCKED,    // Processo bloqueado
  TERMINATED  // Processo terminado
};

typedef struct Process {
  int pid;
  int pcb_address;

  long start_time = 0;

  long timestamp = 0;
  long waiting_time = 0;
  long cpu_time = 0;

  ProcessState state;
} Process;

typedef struct ProcessControlBlock {
  int pid;
  int priority;

  // Memory
  int code_address;
  int code_size;

  // Registers
  int PC;
  int registers[32];

  ProcessState state = READY;

} ProcessControlBlock;

#endif