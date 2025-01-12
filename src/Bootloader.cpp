#include "Bootloader.hpp"

vector<int> Bootloader::boot(Ram* ram, string directory) {
  this->validate_directory(directory);
  this->load_programs(directory);

  processes_map.resize(PROGRAMS_COUNT);

  vector<int> processes_pids;

  cout << "========== Bootloader ==========" << endl;

  for (int i = 0; i < PROGRAMS_COUNT; i++) {
    ram->insert_program(this->programs[i]);

    auto pcb = this->get_pcb(i, i, this->programs[i].size());
    ram->insert_PCB(pcb);

    auto process = this->get_process(i, i);
    process.size = pcb.program_size;
    process.remaining_instructions = pcb.program_size;
    processes_map[i] = process;

    processes_pids.push_back(i);

    cout << "Programa " << i << " carregado." << endl;
    cout << "Tamanho: " << pcb.program_size << endl;
    cout << endl;
  }

  cout << "==============================" << endl;
  cout << endl;

  return processes_pids;
};

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
  for (const auto& entry : fs::directory_iterator(directory)) {
    if (!fs::is_regular_file(entry)) {
      cerr << "Apenas arquivos regulares são suportados. Arquivo: " << entry
           << endl;
      exit(1);
    }

    this->load_program(entry.path().string());
  }
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
