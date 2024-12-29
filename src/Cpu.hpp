#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>
#include <map>
#include <sstream>

#include "Ram.hpp"
#include "RegisterBank.hpp"
#include "entities.hpp"
#include "globals.hpp"

using namespace std;

class Cpu {
 private:
  string active_instruction;

  RegisterBank register_bank;

  // Registradores especiais
  bool write_data;
  int write_value;
  int op;

    vector<string> split_instruction(string instruction);
  int get_register(int address);
  void set_register(int address, int value);

  int ula(int op1, int op2, char oper);

 public:
  Cpu(int id, Ram* ram) {
    this->id = id;
    this->ram = ram;

    this->write_data = false;
  }

  int id;

  Ram* ram;

  ProcessControlBlock actual_pcb;

  Process get_process();

  // Registradores especiais
  int PC;

  // TODO: Criar em uma classe separada
  // Pipelne MIPS
  bool InstructionFetch();
  void InstructionDecode();
  void Execute();
  void MemoryAccess();
  void WriteBack();
};

void* run_core(void* arg);

enum InstructionType {
  LOAD,
  ILOAD,
  ADD,
  STORE,
  BEQ,
  J,
  SUB,
  MUL,
  DIV,
  SLT,
  BNE,
};

#endif