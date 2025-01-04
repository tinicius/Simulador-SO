#ifndef CONTROL_UNIT_HPP
#define CONTROL_UNIT_HPP

#include <string>

#include "entities.hpp"

class Cpu;

using namespace std;

class ControlUnit {
 private:
  Cpu* cpu;

 public:
  ControlUnit(Cpu* cpu);

  string get_instruction(int program_address, int PC);

  void increment_pc();
  void set_register(int address, int value);

  void execute(InstructionType operation);

  void memory_access(InstructionType operation);

  void write_back();
};

#endif