#include "OperatingSystem.hpp"

OperatingSystem::OperatingSystem(Scheduler* scheduler, Ram *ram) {
  this->scheduler = scheduler;
  this->ram = ram;

  pthread_t t_core[CORES_COUNT];

  for (int i = 0; i < CORES_COUNT; i++) {
    Cpu core(i, ram);
    cores.push_back(core);
  }

  for (int i = 0; i < CORES_COUNT; i++) {
    pthread_create(&t_core[i], NULL, run_core, &cores[i]);
  }
}

void OperatingSystem::insert_process(Process process) {
  this->processes.push_back(process);
}

void *run_os(void *arg) {
  OperatingSystem *os = (OperatingSystem *)arg;

  while (true) {
    if (!os->processes.size()) continue;

    pthread_mutex_lock(&next_process_mutex);

    int next_pid = os->scheduler->get_next_process_pid();

    if (next_pid == -1) {
      pthread_mutex_unlock(&next_process_mutex);
      continue;
    }

    Process *next_p = nullptr;

    for (auto &p : os->processes) {
      if (p.pid != next_pid) continue;

      next_p = &p;
      break;
    }

    next_process.push(*next_p);

    pthread_mutex_unlock(&next_process_mutex);
  }

  pthread_exit(NULL);
}