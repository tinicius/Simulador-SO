#include "OperatingSystem.hpp"

OperatingSystem::OperatingSystem(Scheduler *scheduler, Ram *ram) {
  this->scheduler = scheduler;
  this->ram = ram;
  this->cache = new Cache(ram);
  global_cache = this->cache;

  // pthread_t t_core[CORES_COUNT];

  // for (int i = 0; i < CORES_COUNT; i++) {
  //   Cpu core(i, ram, cache);
  //   cores.push_back(core);
  // }

  // for (int i = 0; i < CORES_COUNT; i++) {
  //   pthread_create(&t_core[i], NULL, run_core, &cores[i]);
  // }
}

void OperatingSystem::insert_process(Process &process) {
  processes_map[process.pid] = process;
}

void *run_clock_os(void *arg) {
  OperatingSystem *os = (OperatingSystem *)arg;

  while (true) {
    // Cada execução do loop é um ciclo de clock

    // Adicionando novos processos prontos
    pthread_mutex_lock(&ready_processes_mutex);

    while (ready_processes.size()) {
      auto pid = ready_processes.front();
      ready_processes.pop();
      os->scheduler->add_running(pid);
    }

    pthread_mutex_unlock(&ready_processes_mutex);

    // Verificando se há processos prontos
    pthread_mutex_lock(&running_mutex);
    if (os->scheduler->get_running_size() == 0) {
      pthread_mutex_unlock(&running_mutex);
      continue;
    }
    pthread_mutex_unlock(&running_mutex);

    auto next_pid = os->scheduler->get_next_process_pid();

    while (next_pid != -1) {
      pthread_mutex_lock(&next_process_mutex);
      next_process.push(next_pid);
      pthread_mutex_unlock(&next_process_mutex);

      next_pid = os->scheduler->get_next_process_pid();
    }

    // Iniciando cpus
    pthread_t t_core[CORES_COUNT];

    vector<Cpu> cores;

    for (int i = 0; i < CORES_COUNT; i++) {
      cores.push_back(Cpu(i, os->ram, os->cache));
    }

    for (int i = 0; i < CORES_COUNT; i++) {
      pthread_create(&t_core[i], NULL, run_core, &cores[i]);
    }

    for (int i = 0; i < CORES_COUNT; i++) {
      pthread_join(t_core[i], NULL);
    }

    cout << endl;
  }

  pthread_exit(NULL);
}

void *run_os(void *arg) {
  OperatingSystem *os = (OperatingSystem *)arg;

  while (true) {
    // Adicionando novos processos prontos
    pthread_mutex_lock(&ready_processes_mutex);

    while (ready_processes.size()) {
      auto pid = ready_processes.front();
      ready_processes.pop();
      os->scheduler->add_running(pid);
    }

    pthread_mutex_unlock(&ready_processes_mutex);

    // Verificando se há processos prontos
    pthread_mutex_lock(&running_mutex);
    if (os->scheduler->get_running_size() == 0) {
      pthread_mutex_unlock(&running_mutex);
      continue;
    }
    pthread_mutex_unlock(&running_mutex);

    pthread_mutex_lock(&next_process_mutex);

    int next_pid = os->scheduler->get_next_process_pid();

    if (next_pid == -1) {
      pthread_mutex_unlock(&next_process_mutex);
      continue;
    }

    next_process.push(next_pid);

    pthread_mutex_unlock(&next_process_mutex);
  }

  pthread_exit(NULL);
}