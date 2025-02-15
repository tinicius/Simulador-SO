#ifndef RAM_HPP
#define RAM_HPP

#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "entities.hpp"

using namespace std;

class Ram {
 private:
  vector<vector<string>> programs;   // Região com os programas
  vector<ProcessControlBlock> PCBs;  // Região com os PCBs
  vector<int> free_space;            // Espacos livres

 public:
  Ram();

  void insert_program(vector<string> program);

  void insert_PCB(ProcessControlBlock PCB);
  void update_PCB(int pcb_address, ProcessControlBlock PCB);

  string get_instruction(int code_address, int PC);

  ProcessControlBlock get_PCB(int pcb_address);

  int get_value(int address);
  void set_value(int address, int value);
};

#endif
