#ifndef PROCESS_HPP
#define PROCESS_HPP

enum ProcessState { READY, RUNNING, BLOCKED, TERMINATED };

typedef struct ProcessControlBlock {
  int pid;
  int priority;
  ProcessState state;

  // Memory
  // - Base address
  // - Allocation size

  // Registers
  int PC;
} ProcessControlBlock;

class Process {
 private:
  ProcessControlBlock PCB;

 public:
  Process(int pid, int priority, ProcessState state);

  int get_pid() { return PCB.pid; }
  ProcessState get_state() { return PCB.state; }

  void set_state(ProcessState state) { PCB.state = state; }
};

#endif