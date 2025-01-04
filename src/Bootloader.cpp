#include "Bootloader.hpp"

Bootloader::Bootloader() {
  for (int i = 0; i < PROGRAMS_COUNT; i++) {
    ifstream codigo("./dataset/codigo" + to_string(i + 1) + ".txt");

    vector<string> program;
    string temp;

    while (getline(codigo, temp)) program.push_back(temp);

    if (program.size() == 0) {
      cout << "Invalid program" << endl;
      exit(1);
    }

    this->programs.push_back(program);
  }
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

vector<Process> Bootloader::get_processes() {
  vector<Process> processes;

  for (int i = 0; i < (int)programs.size(); i++) {
    Process process = get_process(i, i);
    processes.push_back(process);
  }

  return processes;
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

vector<ProcessControlBlock> Bootloader::get_pcbs() {
  vector<ProcessControlBlock> pcbs;

  for (int i = 0; i < (int)programs.size(); i++) {
    ProcessControlBlock pcb = get_pcb(i, i, programs[i].size());
    pcbs.push_back(pcb);
  }

  return pcbs;
}

vector<vector<string>> Bootloader::get_programs() { return this->programs; }