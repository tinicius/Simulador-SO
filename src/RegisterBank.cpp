#include "RegisterBank.hpp"

RegisterBank::RegisterBank() { this->registers.resize(32); }

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
  for (int i = 0; i < this->registers.size(); i++) {
    cout << "0x" << i << " " << this->get_value(i) << endl;
  }
}