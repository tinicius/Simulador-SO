#include "OperatingSystem.hpp"

OperatingSystem::OperatingSystem(Scheduler *scheduler, Ram *ram) {
  Bootloader bootloader;

  auto programs = bootloader.get_programs();
  auto processes = bootloader.get_processes();
  auto pcbs = bootloader.get_pcbs();

  for (int i = 0; i < PROGRAMS_COUNT; i++) {
    ram->insert_program(programs[i]);
    ram->insert_PCB(pcbs[i]);

    processes_map[i] = processes[i];

    scheduler->add_ready(processes[i].pid);
  }

  this->scheduler = scheduler;
  this->ram = ram;
  this->cache = new Cache(ram);
  global_cache = this->cache;
  pthread_t t_core[CORES_COUNT];

  for (int i = 0; i < CORES_COUNT; i++) {
    Cpu core(i, ram, cache);
    cores.push_back(core);
  }

  for (int i = 0; i < CORES_COUNT; i++) {
    pthread_create(&t_core[i], NULL, run_core, &cores[i]);
  }
}

bool OperatingSystem::check_finished() {
  int valid_processes = 0;

  for (auto [a, b] : processes_map) {
    if (b.state != TERMINATED) valid_processes++;
  }

  return valid_processes == 0;
}

void *run_os(void *arg) {
  OperatingSystem *os = (OperatingSystem *)arg;

  while (true) {
    if (os->check_finished()) {
      kill(getpid(), SIGINT);
      return NULL;
    }

    // Adicionando novos processos prontos
    pthread_mutex_lock(&ready_processes_mutex);

    while (ready_processes.size()) {
      auto pid = ready_processes.front();
      ready_processes.pop();
      os->scheduler->add_ready(pid);
    }

    pthread_mutex_unlock(&ready_processes_mutex);

    // Verificando se há processos prontos
    pthread_mutex_lock(&running_mutex);
    if (os->scheduler->get_ready_size() == 0) {
      pthread_mutex_unlock(&running_mutex);
      continue;
    }
    pthread_mutex_unlock(&running_mutex);

    int next_pid = os->scheduler->get_next_process_pid();

    if (next_pid == -1) continue;

    next_process.push(next_pid);
  }

  pthread_exit(NULL);
}