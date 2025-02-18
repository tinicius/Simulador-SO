#include "Ram.hpp"

Ram::Ram() {
  this->free_space.resize(32, 0);
  this->pages.resize(MAX_PAGES);
}

void Ram::insert_program(vector<string> program) {
  this->programs.push_back(program);
}

void Ram::insert_PCB(ProcessControlBlock PCB) { this->PCBs.push_back(PCB); }

void Ram::update_PCB(int pcb_address, ProcessControlBlock PCB) {
  if (pcb_address >= (int)PCBs.size()) {
    cout << "Invalid PCB address" << endl;
    exit(1);
  }

  PCBs[pcb_address] = PCB;
}

string Ram::get_instruction(int code_address, int PC) {
  if (code_address >= (int)programs.size()) {
    cout << "Invalid code address" << endl;
    exit(1);
  }

  if (PC >= (int)programs[code_address].size()) {
    cout << "Invalid PC" << endl;
    exit(1);
  }

  usleep(1000);

  return programs[code_address][PC];
}

ProcessControlBlock Ram::get_PCB(int pcb_address) {
  if (pcb_address >= (int)PCBs.size()) {
    cout << "Invalid PCB address" << endl;
    exit(1);
  }

  return PCBs[pcb_address];
}

int Ram::get_value(int address) {
  if (address < 0 || address >= 32) {
    cout << "Invalid address in free space " << address << endl;
    exit(1);
  }

  return free_space[address];
}

void Ram::set_value(int address, int value) {
  if (address < 0 || address >= 32) {
    cout << "Invalid address in free space" << endl;
    exit(1);
  }

  this->free_space[address] = value;
}

int Ram::insert_page(Page page) {
  this->pages[idx] = page;

  int page_address = idx;

  idx = (idx + 1) % MAX_PAGES;

  items++;

  return page_address;
}

Page Ram::get_page(int page_address) {
  if (page_address >= (int)pages.size()) {
    cout << "Invalid page address on ram" << endl;
    exit(1);
  }

  return pages[page_address];
}

bool Ram::is_full() { return items >= MAX_PAGES; }

void Ram::dirty_page() { items--; }