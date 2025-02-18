#ifndef BOOT_LOADER_HPP
#define BOOT_LOADER_HPP

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Disk.hpp"
#include "JobGrouping.hpp"
#include "Ram.hpp"
#include "entities.hpp"
#include "globals.hpp"

using namespace std;

namespace fs = filesystem;

class Bootloader {
 private:
  vector<vector<string>> programs;

  Process get_process(int pid, int pcb_address);
  ProcessControlBlock get_pcb(int pid, int program_address, int program_size);

  vector<int> get_process_pid();

  void validate_directory(string directory);

  void load_programs(string directory);
  void load_program(string path);

 public:
  vector<int> boot(Ram* ram, Disk* disk, string directory);
};

#endif