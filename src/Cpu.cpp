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

  int id = cpu->id;

  while (true) {
    auto process = cpu->get_process();

    if (process.pid == -1) continue;

    int quantum = 0;

    // Buscar PCB na RAM
    cpu->actual_pcb = cpu->ram->get_PCB(process.pcb_address);

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
    cpu->actual_pcb.quantum_remaining -= 5;
    cpu->actual_pcb.cpu_time += 5;

    // Verifica se quantum expirou
    if (cpu->actual_pcb.quantum_remaining <= 0) {
        // Salva contexto atual no PCB
        cpu->actual_pcb.PC = cpu->PC;
        cpu->ram->update_PCB(process.pcb_address, cpu->actual_pcb);

        // Muda estado para READY
        process.state = READY;
        
        // Adiciona processo na fila de prontos
        pthread_mutex_lock(&ready_processes_mutex);
        ready_processes.push(process);
        pthread_mutex_unlock(&ready_processes_mutex);

        // Força troca de contexto
        break;
    }

    // Verifica fim do processo
    if (cpu->actual_pcb.PC >= cpu->actual_pcb.code_size) {
        cout << "Process " << process.pid << " finished" << endl;
        cout << cpu->ram->get_value(16) << endl;
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
  if (actual_pcb.PC >= actual_pcb.code_size) return false;

  this->active_instruction =
      ram->get_instruction(actual_pcb.code_address, actual_pcb.PC);

  actual_pcb.PC++;

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
        this->actual_pcb.PC = PC;
      }
      break;
    }
    case BEQ: {
      if (get_register(get_register(2)) == get_register(get_register(1))) {
        PC = get_register(3) - 1;
        this->actual_pcb.PC = PC;
      }
      break;
    }
    case J: {
      PC = get_register(1) - 1;
      this->actual_pcb.PC = PC;
      break;
    }
  }
}

void Cpu::MemoryAccess() {
  switch (op) {
    case LOAD: {
      write_value = ram->get_value(get_register(2));
      write_data = true;

      break;
    }
    case ILOAD: {
      write_value = get_register(2);
      write_data = true;

      break;
    }
    case STORE: {
      ram->set_value(get_register(2), get_register(get_register(1)));

      // ram->print(active_instruction);
      break;
    }
  }
}

void Cpu::WriteBack() {
  if (!write_data) return;

  set_register(get_register(1), write_value);
  write_data = false;
}