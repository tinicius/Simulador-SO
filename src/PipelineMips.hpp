#ifndef PIPELINE_MIPS_HPP
#define PIPELINE_MIPS_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ControlUnit.hpp"

using namespace std;

class PipelineMips {
 public:
  PipelineMips(ControlUnit* control_unit);

  ~PipelineMips() {}

  bool instruction_fetch(int pc, int program_address);
  void instruction_decode();
  void execute();
  void memory_access();
  void write_back();

  string active_instruction;

 private:
  InstructionType operation = NOP;

  ControlUnit* control_unit;

  vector<string> split_instruction(string instruction);

  map<string, InstructionType> instruction_map;
};

#endif