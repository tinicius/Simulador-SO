#include "OperatingSystem.hpp"

OperatingSystem::OperatingSystem(MemoryLogger *memory_logger,
                                 vector<Cpu> &cores)
    : memory_logger(memory_logger), cores(cores), scheduler(nullptr) {
  Policy *policy = this->get_policy();

  this->scheduler = new Scheduler(policy);

  CpuLogger::configure_logger();
}

OperatingSystem::~OperatingSystem() { delete this->scheduler; }

void OperatingSystem::boot(vector<int> &pids) {
  // Inicializando processos prontos
  for (auto pid : pids) scheduler->add_ready(pid);

  // Inicilizando as threads de CPU
  this->init_cores();

  // Inicializando thread do SO
  pthread_t t_os;
  pthread_create(&t_os, NULL, run_os, this);
  pthread_join(t_os, NULL);
}

void OperatingSystem::init_cores() {
  core_mutex.resize(CORES_COUNT);

  ready_process.resize(CORES_COUNT);

  for (int i = 0; i < CORES_COUNT; i++) {
    pthread_mutex_init(&core_mutex[i], NULL);
    ready_process[i] = -1;
  }
}

bool OperatingSystem::check_finished() {
  int end_processes = 0;

  for (auto &p : processes_map) {
    if (p.state == TERMINATED) end_processes++;
  }

  return end_processes == PROGRAMS_COUNT;
}

void OperatingSystem::log_processes_state() {
  ofstream data_file("./output/process.log", ios::app);
  ofstream cache_file("./output/cache.log", ios::app);

  data_file << endl << endl;

  for (int i = 0; i < PROGRAMS_COUNT; i++) {
    data_file << "Processo: " << i << endl;
    data_file << "Cpu time: " << processes_map[i].cpu_time << endl;
    data_file << "Waiting time: " << processes_map[i].waiting_time << endl;
    data_file << "Timestamp: "
              << processes_map[i].cpu_time + processes_map[i].waiting_time
              << endl;

    data_file << endl;

    cache_file << CACHE_HIT << " " << CACHE_MISS << endl;
  }

  data_file << endl;

  data_file.close();
  cache_file.close();
}

void OperatingSystem::log_final() {
  log_processes_state();

  this->memory_logger->print();

  CpuLogger::print();
}

Cpu *OperatingSystem::get_core(int core_id) { return &this->cores[core_id]; }

Policy *OperatingSystem::get_policy() {
  switch (POLICY) {
    case 1:
      return new PolicyFCFS();
    case 2:
      return new PolicySJF();
    case 3:
      return new PolicySRTN();
    case 4:
      return new PolicyRoundRobin();
  }

  return nullptr;
}

void *run_os(void *arg) {
  OperatingSystem *os = (OperatingSystem *)arg;

  while (true) {
    // Verifica se todos os processos terminaram
    if (os->check_finished()) {
      os->log_final();
      pthread_exit(NULL);
    }

    for (int i = 0; i < CORES_COUNT; i++) {
      if (pthread_mutex_trylock(&core_mutex[i]) != 0) continue;

      if (ready_process[i] != -1) {
        os->scheduler->add_ready(ready_process[i]);
        ready_process[i] = -1;
      }

      int next_pid = os->scheduler->get_next_process_pid();

      if (next_pid == -1) {
        pthread_mutex_unlock(&core_mutex[i]);
        continue;
      }

      CpuThreadArgs *args = (CpuThreadArgs *)malloc(sizeof(CpuThreadArgs));

      if (args == NULL) {
        perror("Erro ao alocar memória para args");
        exit(EXIT_FAILURE);
      }

      args->pid = next_pid;
      args->cpu = os->get_core(i);

      pthread_t t_core;
      pthread_create(&t_core, NULL, run_core, args);
    }
  }

  pthread_exit(NULL);
}