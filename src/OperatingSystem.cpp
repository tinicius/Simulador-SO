#include "OperatingSystem.hpp"

OperatingSystem::OperatingSystem(Scheduler *scheduler, Ram *ram) {
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

void OperatingSystem::insert_process(Process process) {
  this->processes.push_back(process);
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

  auto start = chrono::high_resolution_clock::now();
  auto start_in_nano =
      chrono::duration_cast<chrono::microseconds>(start.time_since_epoch())
          .count();

  cout << "Start: " << start_in_nano << endl << endl;

  for (auto &p : os->processes) {
    processes_map[p.pid].start_time = start_in_nano;
  }

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

    int next_pid = os->scheduler->get_next_process_pid();

    if (next_pid == -1) continue;

    next_process.push(next_pid);
  }

  pthread_exit(NULL);
}