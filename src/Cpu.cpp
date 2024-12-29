#include "Cpu.hpp"

void* run_core(void* arg) {
  Cpu* cpu = (Cpu*)arg;

  if (cpu == nullptr) {
    cout << "Cpu is null" << endl;
    pthread_exit(NULL);
  }

  int id = cpu->id;

  while (true) {
    auto process = cpu->get_process();

    if (process.pid == -1) continue;

    cout << "Core: " << id << endl;
    cout << "Process: " << process.pid << endl;
    cout << endl;
  }

  pthread_exit(NULL);
}

Process Cpu::get_process() {
  pthread_mutex_lock(&next_process_mutex);

  if (next_process.empty()) {
    pthread_mutex_unlock(&next_process_mutex);

    Process p_empty;
    p_empty.pid = -1;
    return p_empty;
  }

  Process process = next_process.front();
  next_process.pop();

  pthread_mutex_unlock(&next_process_mutex);

  return process;
}

void Cpu::InstructionFetch() {}