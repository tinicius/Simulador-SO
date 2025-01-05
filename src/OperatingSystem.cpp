#include "OperatingSystem.hpp"

OperatingSystem::OperatingSystem() : ram(Ram()), scheduler(Scheduler()) {
  this->cache = new Cache(&ram);

  MemoryLogger::create_instance(&ram, cache);
  CpuLogger::configure_logger();
}

OperatingSystem::~OperatingSystem() { delete cache; }

void OperatingSystem::boot() {
  processes_map.resize(PROGRAMS_COUNT);

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
  core_mutex.resize(CORES_COUNT);

  ready_process.resize(CORES_COUNT);

  for (int i = 0; i < CORES_COUNT; i++) {
    pthread_mutex_init(&core_mutex[i], NULL);
    ready_process[i] = -1;
  }

  for (int i = 0; i < CORES_COUNT; i++) {
    Cpu core(i, &ram, cache);
    cores.push_back(core);
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
  ofstream data_file("./output/process.txt", ios::app);

  data_file << endl << endl;

  for (int i = 0; i < PROGRAMS_COUNT; i++) {
    data_file << "Processo: " << i << endl;
    data_file << "Cpu time: " << processes_map[i].cpu_time << " μm" << endl;
    data_file << "Waiting time: " << processes_map[i].waiting_time << " μm"
              << endl;
    data_file << "Timestamp: "
              << processes_map[i].cpu_time + processes_map[i].waiting_time
              << " μm" << endl;
    data_file << "State: " << processes_map[i].state << endl;

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

      CpuLogger::print();

      pthread_exit(NULL);
    }

    for (int i = 0; i < CORES_COUNT; i++) {
      if (pthread_mutex_trylock(&core_mutex[i]) != 0) continue;

      if (ready_process[i] != -1) {
        os->scheduler.add_ready(ready_process[i]);
        ready_process[i] = -1;
      }

      int next_pid = os->scheduler.get_next_process_pid();

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
      args->cpu = &os->cores[i];

      pthread_t t_core;
      pthread_create(&t_core, NULL, run_core, args);
    }
  }

  pthread_exit(NULL);
}