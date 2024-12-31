#include "RegisterBank.hpp"

RegisterBank::RegisterBank() {}

int RegisterBank::get_value(int address) {
  return this->registers[address].value;
}

void RegisterBank::set_value(int address, int value) {
  this->registers[address].value = value;
}

void RegisterBank::set_dirty(int address) {
  this->registers[address].dirty = true;
}

void RegisterBank::set_clean(int address) {
  this->registers[address].dirty = false;
}

void RegisterBank::print() {
  for (int i = 0; i < REGISTERS_SIZE; i++) {
    cout << "0x" << i << " " << this->get_value(i) << endl;
  }
}

void RegisterBank::set_registers(int registers[REGISTERS_SIZE]) {
  for (int i = 0; i < REGISTERS_SIZE; i++) {
    this->registers[i].value = registers[i];
  }
}