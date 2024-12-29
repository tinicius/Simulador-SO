#ifndef CPU_HPP
#define CPU_HPP

#include <iostream>

#include "Ram.hpp"
#include "globals.hpp"

using namespace std;

class Cpu {
 private:
  Ram* ram;

  string active_instruction;

  // Registradores
  int PC;

 public:
  Cpu(int id, Ram* ram) {
    this->id = id;
    this->ram = ram;
  }

  int id;

  Process get_process();

  // TODO: Criar em uma classe separada
  // Pipelne MIPS

  void InstructionFetch();
};

void* run_core(void* arg);

#endif