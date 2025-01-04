#ifndef CPU_HPP
#define CPU_HPP

#include <chrono>
#include <iostream>
#include <map>
#include <sstream>

#include "Cache.hpp"
#include "MemoryLogger.hpp"
#include "PipelineMips.hpp"
#include "Ram.hpp"
#include "RegisterBank.hpp"
#include "ULA.hpp"
#include "entities.hpp"
#include "globals.hpp"

using namespace std;

class ControlUnit;

class Cpu {
 private:
  int id;

  Cache* cache;
  Ram* ram;
  ULA ula;
  ControlUnit control_unit;
  PipelineMips pipeline;

  MemoryLogger* logger;

  int PC;
  RegisterBank register_bank;
  bool write_data;
  int write_value;
  int op;

  chrono::_V2::system_clock::time_point start;

  vector<string> split_instruction(string instruction);

 public:
  ProcessControlBlock actual_pcb;

  Cpu(int id, Ram* ram, Cache* cache);

  Cpu(const Cpu& cpu) : control_unit(this), pipeline(&control_unit) {
    this->pipeline = PipelineMips(&this->control_unit);

    this->id = cpu.id;
    this->cache = cpu.cache;
    this->ram = cpu.ram;

    this->logger = cpu.logger;
    this->write_data = false;
  };

  int get_id();
  Ram* get_ram();

  PipelineMips* get_pipeline();

  ULA* get_ula();

  string get_instruction(int program_address, int PC);

  int get_pc();
  void set_pc(int pc);

  int get_register(int address);
  void set_register(int address, int value);

  void set_registers(int registers[REGISTERS_SIZE]);

  int get_write_value();
  void set_write_value(int value);

  bool get_write_data();
  void set_write_data(bool value);
};

typedef struct CpuThreadArgs {
  Cpu* cpu;
  int pid;
} CpuThreadArgs;

void* run_core(void* args);

#endif