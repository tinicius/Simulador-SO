#ifndef REGISTER_BANK
#define REGISTER_BANK

#include <vector>
#include <iostream>

#include "globals.hpp"

using namespace std;

typedef struct Register {
  int value;
  bool dirty;
} Register;

class RegisterBank {
 private:
  Register registers[REGISTERS_SIZE];

 public:
  RegisterBank();

  int get_value(int address);
  void set_value(int address, int value);
  void set_dirty(int address);
  void set_clean(int address);

  void set_registers(int registers[REGISTERS_SIZE]);

  void print();
};

#endif