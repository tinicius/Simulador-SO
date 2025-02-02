#include "PipelineMips.hpp"

PipelineMips::PipelineMips(ControlUnit* control_unit) {
  this->control_unit = control_unit;

  this->instruction_map = {{"ILOAD", ILOAD}, {"ADD", ADD}, {"STORE", STORE},
                           {"BEQ", BEQ},     {"J", J},     {"SUB", SUB},
                           {"MUL", MUL}};
}

bool PipelineMips::instruction_fetch(int pc, int program_address,
                                     int program_size) {
  if (pc >= program_size) return false;

  active_instruction = control_unit->get_instruction(program_address, pc);

  control_unit->increment_pc();

  return true;
}

void PipelineMips::instruction_decode() {
  vector<string> line = split_instruction(active_instruction);

  operation = instruction_map[line[0]];

  if (line[1] != "!") {
    control_unit->set_register(1, stoi(line[1]));
  }

  if (line[2] != "!") {
    control_unit->set_register(2, stoi(line[2]));
  }

  if (line[3] != "!") {
    control_unit->set_register(3, stoi(line[3]));
  }
}

void PipelineMips::execute() { control_unit->execute(operation); }

void PipelineMips::memory_access() { control_unit->memory_access(operation); }

void PipelineMips::write_back() { control_unit->write_back(); }

vector<string> PipelineMips::split_instruction(string instruction) {
  vector<string> result;
  istringstream iss(instruction);
  string word;

  while (iss >> word) {
    result.push_back(word);
  }

  while (result.size() < 4) {
    result.push_back("!");
  }

  return result;
}