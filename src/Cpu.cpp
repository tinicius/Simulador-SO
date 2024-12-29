#include "Cpu.hpp"

#include <unistd.h>

Cpu::Cpu(int id) { this->id = id; }

void* run_core(void* arg) {
  Cpu* cpu = (Cpu*)arg;

  if (cpu == nullptr) {
    cout << "Cpu is null" << endl;
    pthread_exit(NULL);
  }

  int id = cpu->id;

  while (true) {
    auto process = cpu->get_process();

    // cout << "Core: " << id << endl;
    cout << "Process: " << process->get_pid() << endl;
    // cout << "State: " << process->get_state() << endl;
    cout << endl;

    pthread_mutex_lock(&process_mutex);
    process->set_state(TERMINATED);
    pthread_mutex_unlock(&process_mutex);
  }

  pthread_exit(NULL);
}

Process* Cpu::get_process() {
  pthread_mutex_lock(&next_process_mutex);
  pthread_cond_wait(&core_cond, &next_process_mutex);

  Process* process = next_process;

  next_process = nullptr;

  cout << next_process << endl;

  pthread_mutex_unlock(&next_process_mutex);

  return process;
}