#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>
#include <map>
#include <sstream>
#include "Cache.hpp"
#include "Ram.hpp"
#include "RegisterBank.hpp"
#include "entities.hpp"
#include "globals.hpp"

#include "MemoryLogger.hpp"

using namespace std;



class Cpu {
 private:
  private:
    int id;          // Move id to private and first
    Cache* cache;
    Ram* ram;
    MemoryLogger* logger;
    string active_instruction;
    RegisterBank register_bank;
    bool write_data;
    int write_value;
    int op;

  vector<string> split_instruction(string instruction);
  int get_register(int address);
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
    
    // Add getter for id
    int get_id() const { return id; }
    Ram* get_ram() { return ram; }

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