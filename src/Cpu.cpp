#include "Cpu.hpp"

map<string, InstructionType> instruction_map = {
    {"ILOAD", ILOAD}, {"ADD", ADD}, {"STORE", STORE}, {"BEQ", BEQ},
    {"J", J},         {"SUB", SUB}, {"MUL", MUL}};

int Cpu::ula(int op1, int op2, char oper) {
  if (oper == '+')
    return op1 + op2;
  else if (oper == '-')
    return op1 - op2;
  else if (oper == '*')
    return op1 * op2;
  else if (oper == '/')
    return op1 / op2;
  return 0;
}

int Cpu::get_register(int address) { return register_bank.get_value(address); }

void Cpu::set_register(int address, int value) {
  register_bank.set_value(address, value);
}

void* run_core(void* arg) {
  Cpu* cpu = (Cpu*)arg;

  if (cpu == nullptr) {
    cout << "Cpu is null" << endl;
    pthread_exit(NULL);
  }

  int id = cpu->get_id();

  while (true) {
    auto process = cpu->get_process();

    if (process.pid == -1) continue;

    int quantum = 0;

    // Buscar PCB na RAM
    cpu->actual_pcb = cpu->get_ram()->get_PCB(process.pcb_address);

    // Atualizando registradores
    cpu->PC = cpu->actual_pcb.PC;

    cout << "Core: " << id << endl;
    cout << "Process: " << process.pid << endl;
    cout << endl;

    while (cpu->InstructionFetch()) {
      cpu->InstructionDecode();
      cpu->Execute();
      cpu->MemoryAccess();
      cpu->WriteBack();

      quantum += 5;
      cpu->actual_pcb.timestamp += 5;
      cpu->actual_pcb.cpu_time += 5;

      cpu->actual_pcb.PC = cpu->PC;

      // Verifica fim do processo
      if (cpu->actual_pcb.PC >= cpu->actual_pcb.code_size) {
        cout << "Process " << process.pid << " finished" << endl;
        cpu->get_ram()->update_PCB(process.pcb_address, cpu->actual_pcb);
        break;
      }

      // Verifica se quantum expirou
      if (quantum >= QUANTUM) {
        cpu->get_ram()->update_PCB(process.pcb_address, cpu->actual_pcb);

        // Muda estado para READY
        process.state = READY;

        // Adiciona processo na fila de prontos
        pthread_mutex_lock(&ready_processes_mutex);
        ready_processes.push(process);
        pthread_mutex_unlock(&ready_processes_mutex);

        break;
      }
    }
  }

  pthread_exit(NULL);
}

Process Cpu::get_process() {
  pthread_mutex_lock(&next_process_mutex);

  if (next_process.empty()) {
    pthread_mutex_unlock(&next_process_mutex);

    Process p_empty;
    p_empty.pid = -1;
    return p_empty;
  }

  Process process = next_process.front();
  next_process.pop();

  pthread_mutex_unlock(&next_process_mutex);

  return process;
}

bool Cpu::InstructionFetch() {
  if (this->PC >= actual_pcb.code_size) return false;

  this->active_instruction =
      ram->get_instruction(actual_pcb.code_address, this->PC);

  this->PC++;

  return true;
}

vector<string> Cpu::split_instruction(string instruction) {
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

void Cpu::InstructionDecode() {
  vector<string> linha = this->split_instruction(active_instruction);

  op = instruction_map[linha[0]];

  if (linha[1] != "!") {
    set_register(1, stoi(linha[1]));
    register_bank.set_dirty(1);
  }

  if (linha[2] != "!") {
    set_register(2, stoi(linha[2]));
    register_bank.set_dirty(2);
  }

  if (linha[3] != "!") {
    set_register(3, stoi(linha[3]));
    register_bank.set_dirty(3);
  }
}

void Cpu::Execute()  // Unidade de controle
{
  switch (op) {
    case ADD: {
      write_value = ula(get_register(get_register(2)),
                        get_register(get_register(3)), '+');

      write_data = true;
      break;
    }
    case SUB: {
      write_value = ula(get_register(get_register(2)),
                        get_register(get_register(3)), '-');

      write_data = true;
      break;
    }
    case MUL: {
      write_value = ula(get_register(get_register(2)),
                        get_register(get_register(3)), '*');

      write_data = true;
      break;
    }
    case DIV: {
      write_value = ula(get_register(get_register(2)),
                        get_register(get_register(3)), '/');

      write_data = true;
      break;
    }
    case SLT: {
      write_value =
          get_register(get_register(2)) < get_register(get_register(3));

      write_data = true;
      break;
    }
    case BNE: {
      if (get_register(get_register(2)) != get_register(get_register(1))) {
        PC = get_register(3) - 1;
      }
      break;
    }
    case BEQ: {
      if (get_register(get_register(2)) == get_register(get_register(1))) {
        PC = get_register(3) - 1;
      }
      break;
    }
    case J: {
      PC = get_register(1) - 1;
      break;
    }
  }
}

void Cpu::MemoryAccess() {
  switch (op) {
    case LOAD: {
      int address = get_register(2);
      write_value = cache->read(address);
      logger->log_memory_operation("LOAD", address, write_value);
      write_data = true;
      break;
    }
    case ILOAD: {
      write_value = get_register(2);
      write_data = true;

      break;
    }
    case STORE: {
      int address = get_register(2);
      int value = get_register(get_register(1));
      cache->write(address, value);
      logger->log_memory_operation("STORE", address, value);
      break;
    }
  }
}

void Cpu::WriteBack() {
  if (!write_data) return;

  set_register(get_register(1), write_value);
  write_data = false;
}