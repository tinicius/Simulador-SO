#include "Bootloader.hpp"

vector<int> Bootloader::boot(Ram* ram, string directory) {
  cout << "========== Bootloader ==========" << endl;

  this->validate_directory(directory);
  this->load_programs(directory);

  processes_map.resize(PROGRAMS_COUNT);

  cout << "Processos" << endl;
  cout << endl;

  for (int i = 0; i < PROGRAMS_COUNT; i++) {
    ram->insert_program(this->programs[i]);

    auto pcb = this->get_pcb(i, i, this->programs[i].size());
    ram->insert_PCB(pcb);

    auto process = this->get_process(i, i);
    process.size = pcb.program_size;
    process.remaining_instructions = pcb.program_size;
    processes_map[i] = process;

    cout << "PID: " << i << " carregado." << endl;
    cout << "Tamanho: " << pcb.program_size << endl;
    cout << endl;
  }

  cout << "==============================" << endl;
  cout << endl;

  return get_process_pid();
};

vector<int> Bootloader::get_process_pid() {
  vector<int> pids;

  if (ENABLED_GROUPING) {
    JobGrouping jobGrouping(0.5);

    vector<vector<int>> clusters = jobGrouping.clusterPrograms(this->programs);

    for (int i = 0; i < (int)clusters.size(); i++) {
      for (int j = 0; j < (int)clusters[i].size(); j++) {
        pids.push_back(clusters[i][j]);
      }
    }

    return pids;
  }

  for (int i = 0; i < PROGRAMS_COUNT; i++) pids.push_back(i);

  return pids;
}

void Bootloader::validate_directory(string directory) {
  if (!fs::exists(directory) || !fs::is_directory(directory)) {
    cerr << "O diretório não existe ou não é válido." << endl;
    exit(1);
  }

  if (distance(fs::directory_iterator(directory), fs::directory_iterator{}) <
      PROGRAMS_COUNT) {
    cerr << "Número de programas inválido!" << endl;
    exit(1);
  }
}

void Bootloader::load_programs(string directory) {
  cout << "Arquivos e PID" << endl;
  cout << endl;

  int i = 0;

  for (const auto& entry : fs::directory_iterator(directory)) {
    if (!fs::is_regular_file(entry)) {
      cerr << "Apenas arquivos regulares são suportados. Arquivo: " << entry
           << endl;
      exit(1);
    }

    cout << "Arquivo: " << entry.path().string() << endl;
    cout << "PID: " << i << endl;
    cout << endl;

    i++;

    this->load_program(entry.path().string());
  }

  cout << endl;
}

void Bootloader::load_program(string path) {
  ifstream codigo(path);

  vector<string> program;
  string temp;

  while (getline(codigo, temp)) program.push_back(temp);

  if (program.size() == 0) {
    cout << "Invalid program:" << path << endl;
    exit(1);
  }

  this->programs.push_back(program);
}

Process Bootloader::get_process(int pid, int pcb_address) {
  auto now = chrono::duration_cast<chrono::microseconds>(
                 chrono::system_clock::now().time_since_epoch())
                 .count();

  Process process;

  process.pid = pid;
  process.pcb_address = pcb_address;
  process.state = READY;
  process.cpu_time = 0;
  process.waiting_time = 0;
  process.timestamp = 0;
  process.start_time = now;

  processes_map[pid] = process;

  return process;
}

ProcessControlBlock Bootloader::get_pcb(int pid, int program_address,
                                        int program_size) {
  ProcessControlBlock pcb;

  pcb.pid = pid;
  pcb.priority = 1;
  pcb.program_address = program_address;
  pcb.program_size = program_size;
  pcb.PC = 0;
  pcb.state = READY;

  return pcb;
}
