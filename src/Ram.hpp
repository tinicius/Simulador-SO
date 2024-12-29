#ifndef RAM_HPP
#define RAM_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "entities.hpp"

using namespace std;

class Ram {
 private:
  vector<vector<string>> codes;      // Regiao com os programas
  vector<ProcessControlBlock> PCBs;  // Regiao com os PCBs

 public:
  Ram();

  void insert_code(vector<string> code);
  void insert_PCB(ProcessControlBlock PCB);

  string get_instruction(int code_address, int PC);

  ProcessControlBlock get_PCB(int pcb_address);
};

#endif
