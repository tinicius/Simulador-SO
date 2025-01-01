#ifndef CPU_HPP
#define CPU_HPP

#include <fstream>
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
  int id;  // Move id to private and first
  Cache* cache;
  Ram* ram;
  MemoryLogger* logger;
  string active_instruction;
  RegisterBank register_bank;
  bool write_data;
  int write_value;
  int op;

  ofstream cpu_file;

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
        logger(MemoryLogger::getInstance(ram, cache)),
        write_data(false) {}

  void log(const std::string& message) { (cpu_file) << message << endl; }

  // Add getter for id
  int get_id() const { return id; }
  Ram* get_ram() { return ram; }

  int get_register(int address);

  void set_registers(int registers[REGISTERS_SIZE]) {
    register_bank.set_registers(registers);
  }

  void open_log_file() {
    cpu_file.open("cpu" + to_string(id) + ".log");
    if (!cpu_file.is_open()) {
      cout << "Error opening file" << endl;
      exit(1);
    }
  }

  void close_log_file() { cpu_file.close(); }

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