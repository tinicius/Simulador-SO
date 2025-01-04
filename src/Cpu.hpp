#ifndef CPU_HPP
#define CPU_HPP

#include <chrono>
#include <iostream>
#include <map>
#include <sstream>

#include "Cache.hpp"
#include "MemoryLogger.hpp"
#include "Ram.hpp"
#include "RegisterBank.hpp"
#include "ULA.hpp"
#include "entities.hpp"
#include "globals.hpp"

using namespace std;

class Cpu {
 private:
  int id;
  Cache* cache;
  Ram* ram;
  ULA ula;

  MemoryLogger* logger;
  string active_instruction;
  RegisterBank register_bank;
  bool write_data;
  int write_value;
  int op;

  chrono::_V2::system_clock::time_point start;

  vector<string> split_instruction(string instruction);

  void set_register(int address, int value);

 public:
  ProcessControlBlock actual_pcb;

  int PC;

  Cpu(int id, Ram* ram, Cache* cache);

  int get_id() const { return id; }
  Ram* get_ram() { return ram; }

  int get_register(int address);

  void set_registers(int registers[REGISTERS_SIZE]) {
    register_bank.set_registers(registers);
  }

  // Pipeline MIPS
  bool InstructionFetch();
  void InstructionDecode();
  void Execute();
  void MemoryAccess();
  void WriteBack();
};

typedef struct CpuThreadArgs {
  Cpu* cpu;
  int pid;
} CpuThreadArgs;

void* run_core(void* args);

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