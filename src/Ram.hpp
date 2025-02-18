#ifndef RAM_HPP
#define RAM_HPP

#include <unistd.h>

#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "entities.hpp"

#define MAX_PAGES 1

using namespace std;

class Ram {
 private:
  vector<vector<string>> programs;   // Região com os programas
  vector<ProcessControlBlock> PCBs;  // Região com os PCBs
  vector<int> free_space;            // Espacos livres

  int items = 0;
  vector<Page> pages;

  int idx = 0;

 public:
  Ram();

  void insert_program(vector<string> program);

  void insert_PCB(ProcessControlBlock PCB);
  void update_PCB(int pcb_address, ProcessControlBlock PCB);

  string get_instruction(int code_address, int PC);

  ProcessControlBlock get_PCB(int pcb_address);

  int get_value(int address);
  void set_value(int address, int value);

  int insert_page(Page page);
  Page get_page(int page_address);

  bool is_full();
  void dirty_page();
};

#endif
