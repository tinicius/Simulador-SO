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
#include "entities.hpp"
#include "globals.hpp"

using namespace std;

class Cpu {
 private:
 private:
  int id;  // Move id to private and first
  Cache* cache;
  Ram* ram;
  MemoryLogger* logger;
  string active_instruction;
  RegisterBank register_bank;
  bool write_data;
  int write_value;
  int op;

  vector<string> split_instruction(string instruction);

  void set_register(int address, int value);

  int ula(int op1, int op2, char oper);

 public:
  ProcessControlBlock actual_pcb;
  // Program Counter - Registrador especial
  int PC;
  Cpu(int id, Ram* ram, Cache* cache)
      : id(id),
        cache(cache),
        ram(ram),
        logger(MemoryLogger::get_instance()),
        write_data(false) {}

  // Add getter for id
  int get_id() const { return id; }
  Ram* get_ram() { return ram; }

  int get_register(int address);

  void set_registers(int registers[REGISTERS_SIZE]) {
    register_bank.set_registers(registers);
  }

  Process get_process();

  // Pipeline MIPS
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