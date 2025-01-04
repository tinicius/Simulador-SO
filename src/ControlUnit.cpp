#include "ControlUnit.hpp"

#include "Cpu.hpp"

ControlUnit::ControlUnit(Cpu* cpu) : cpu(cpu) {}

string ControlUnit::get_instruction(int program_address, int pc) {
  return cpu->get_instruction(program_address, pc);
}

void ControlUnit::increment_pc() { cpu->set_pc(cpu->get_pc() + 1); }

void ControlUnit::set_register(int address, int value) {
  cpu->set_register(address, value);
}

void ControlUnit::execute(InstructionType operation) {
  if (operation == LOAD || operation == ILOAD || operation == STORE) {
    return;
  }

  auto ula = cpu->get_ula();

  if (operation == BNE) {
    int value1 = cpu->get_register(cpu->get_register(2));
    int value2 = cpu->get_register(cpu->get_register(1));

    if (ula->diff(value1, value2)) cpu->set_pc(cpu->get_register(3) - 1);
    return;
  }

  if (operation == BEQ) {
    int value1 = cpu->get_register(cpu->get_register(2));
    int value2 = cpu->get_register(cpu->get_register(1));

    if (ula->equal(value1, value2)) cpu->set_pc(cpu->get_register(3) - 1);
    return;
  }

  if (operation == J) {
    cpu->set_pc(cpu->get_register(1) - 1);
    return;
  }

  int value1 = cpu->get_register(cpu->get_register(2));
  int value2 = cpu->get_register(cpu->get_register(3));

  int write_value;

  switch (operation) {
    case ADD:
      write_value = ula->add(value1, value2);
      break;

    case SUB:
      write_value = ula->sub(value1, value2);
      break;

    case MUL:
      write_value = ula->mul(value1, value2);
      break;

    case DIV:
      write_value = ula->div(value1, value2);
      break;

    case SLT:
      write_value = ula->slt(value1, value2);
      break;
    default:
      return;
  }

  cpu->set_write_value(write_value);
  cpu->set_write_data(true);
}

void ControlUnit::memory_access(InstructionType operation) {
  switch (operation) {
    case LOAD: {
      int address = cpu->get_register(2);
      cpu->set_write_value(cpu->get_ram()->get_value(address));

      cpu->set_write_data(true);
      break;
    }
    case ILOAD: {
      cpu->set_write_value(cpu->get_register(2));
      cpu->set_write_data(true);
      break;
    }
    case STORE: {
      int address = cpu->get_register(2);
      int value = cpu->get_register(cpu->get_register(1));

      cpu->get_ram()->set_value(address, value);
      break;
    }
    default:
      return;
  }
}

void ControlUnit::write_back() {
  if (!cpu->get_write_data()) return;

  cpu->set_register(cpu->get_register(1), cpu->get_write_value());
  cpu->set_write_data(false);
}