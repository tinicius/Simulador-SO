#include "Ram.hpp"

Ram::Ram() {}

void Ram::insert_code(vector<string> code) { this->codes.push_back(code); }

void Ram::insert_PCB(ProcessControlBlock PCB) { this->PCBs.push_back(PCB); }

string Ram::get_instruction(int code_address, int PC) {
  if (code_address >= (int)codes.size()) {
    cout << "Invalid code address" << endl;
    exit(1);
  }

  if (PC >= (int)codes[code_address].size()) {
    cout << "Invalid PC" << endl;
    exit(1);
  }

  return codes[code_address][PC];
}

ProcessControlBlock Ram::get_PCB(int pcb_address) {
  if (pcb_address >= (int)PCBs.size()) {
    cout << "Invalid PCB address" << endl;
    exit(1);
  }

  return PCBs[pcb_address];
}