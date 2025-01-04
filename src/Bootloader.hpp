#ifndef BOOT_LOADER_HPP
#define BOOT_LOADER_HPP

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

#include "entities.hpp"
#include "globals.hpp"

class Bootloader {
 private:
  vector<vector<string>> programs;

  Process get_process(int pid, int pcb_address);
  ProcessControlBlock get_pcb(int pid, int program_address, int program_size);

 public:
  Bootloader();

  vector<Process> get_processes();
  vector<ProcessControlBlock> get_pcbs();
  vector<vector<string>> get_programs();
};

#endif