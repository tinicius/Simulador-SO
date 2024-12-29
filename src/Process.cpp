#include "Process.hpp"

Process::Process(int pid, int priority, ProcessState state) {
 this->PCB = ProcessControlBlock{pid, priority, state, 0};
}