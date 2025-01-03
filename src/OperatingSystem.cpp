#include "OperatingSystem.hpp"

OperatingSystem::OperatingSystem() : ram(Ram()), scheduler(Scheduler()) {
  this->cache = new Cache(&ram);

  MemoryLogger::create_instance(&ram, cache);
}

OperatingSystem::~OperatingSystem() { delete cache; }

void OperatingSystem::boot() {
  // Carregando programas
  Bootloader bootloader;

  auto programs = bootloader.get_programs();
  auto processes = bootloader.get_processes();
  auto pcbs = bootloader.get_pcbs();

  for (int i = 0; i < PROGRAMS_COUNT; i++) {
    ram.insert_program(programs[i]);
    ram.insert_PCB(pcbs[i]);

    processes_map[i] = processes[i];

    scheduler.add_ready(processes[i].pid);
  }

  // Inicilizando as threads de CPU
  this->init_cores();

  // Inicializando thread do SO
  pthread_t t_os;
  pthread_create(&t_os, NULL, run_os, this);
  pthread_join(t_os, NULL);
}

void OperatingSystem::init_cores() {
  pthread_t t_core[CORES_COUNT];

  for (int i = 0; i < CORES_COUNT; i++) {
    Cpu core(i, &ram, cache);
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

void OperatingSystem::log_processes_state() {
  ofstream data_file("/out/process.log", ios::app);

  data_file << endl << endl;

  for (auto [pid, process] : processes_map) {
    data_file << "Processo: " << pid << endl;
    data_file << "Cpu time: " << process.cpu_time << endl;
    data_file << "Waiting time: " << process.waiting_time << endl;
    data_file << "Timestamp: " << process.cpu_time + process.waiting_time
              << endl;
    data_file << "State: " << process.state << endl;

    data_file << endl;
  }

  data_file << endl;

  data_file.close();
}

void *run_os(void *arg) {
  OperatingSystem *os = (OperatingSystem *)arg;

  while (true) {
    // Verifica se todos os processos terminaram
    if (os->check_finished()) {
      auto logger = MemoryLogger::get_instance();
      logger->log_final_state();
      logger->close_log_file();

      MemoryLogger::delete_instance();

      os->log_processes_state();

      pthread_exit(NULL);
    }

    // Adicionando novos processos prontos ao escalonador
    pthread_mutex_lock(&ready_processes_mutex);

    while (ready_processes.size()) {
      auto pid = ready_processes.front();
      ready_processes.pop();
      os->scheduler.add_ready(pid);
    }

    pthread_mutex_unlock(&ready_processes_mutex);

    int next_pid = os->scheduler.get_next_process_pid();
    if (next_pid == -1) continue;

    next_process.push(next_pid);
  }

  pthread_exit(NULL);
}