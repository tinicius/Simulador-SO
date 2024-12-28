#include "Ram.hpp"

Ram::Ram() {
  for (int i = 0; i < MAX_SIZE; i++) {
    values[i] = 0;
  }
}

int Ram::get_value(int address) {
  if (!(address < 0 || address >= MAX_SIZE)) {
    return values[address];
  }
  return 0;
}

void Ram::set_value(int address, int value) {
  if (!(address < 0 || address >= MAX_SIZE)) {
    this->values[address] = value;
  }
}

