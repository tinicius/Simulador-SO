#include "OperatingSystem.hpp"

OperatingSystem::OperatingSystem(Scheduler scheduler) {
  this->scheduler = scheduler;

  pthread_t t_core[CORES_COUNT];

  for (int i = 0; i < CORES_COUNT; i++) {
    Cpu core(i);
    cores.push_back(core);
  }

  for (int i = 0; i < CORES_COUNT; i++) {
    pthread_create(&t_core[i], NULL, run_core, &cores[i]);
  }
}

void OperatingSystem::boot() {
  for (int i = 0; i < 10; i++) {
    Process p(i, 0, READY);
    processes.push_back(p);
  }
}

void *run_os(void *arg) {
  OperatingSystem *os = (OperatingSystem *)arg;

  while (true) {
    if (!os->processes.size()) continue;

    Process *next = os->scheduler.get_next_process(os->processes);

    if (next == nullptr) continue;

    pthread_mutex_lock(&next_process_mutex);

    if (next_process == nullptr) {
      next_process = next;

      pthread_mutex_lock(&process_mutex);
      next->set_state(RUNNING);
      pthread_mutex_unlock(&process_mutex);

      pthread_cond_signal(&core_cond);
    }

    pthread_mutex_unlock(&next_process_mutex);
  }

  pthread_exit(NULL);
}