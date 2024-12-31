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

  //
  ProcessState state = READY;
  int waiting_time = 0;
  int cpu_time = 0;

  int timestamp = 0;

} ProcessControlBlock;

#endif