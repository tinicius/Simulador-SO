#ifndef ENTITIES_HPP
#define ENTITIES_HPP

enum ProcessState { READY, RUNNING, BLOCKED, TERMINATED };

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
} ProcessControlBlock;

#endif