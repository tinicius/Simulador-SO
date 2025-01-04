#include "Cpu.hpp"

map<string, InstructionType> instruction_map = {
    {"ILOAD", ILOAD}, {"ADD", ADD}, {"STORE", STORE}, {"BEQ", BEQ},
    {"J", J},         {"SUB", SUB}, {"MUL", MUL}};

Cpu::Cpu(int id, Ram* ram, Cache* cache)
    : id(id),
      cache(cache),
      ram(ram),
      logger(MemoryLogger::get_instance()),
      write_data(false) {}

int Cpu::get_register(int address) { return register_bank.get_value(address); }

void Cpu::set_register(int address, int value) {
  register_bank.set_value(address, value);
}

void* run_core(void* args) {
  CpuThreadArgs* cpu_thread_args = (CpuThreadArgs*)args;

  Cpu* cpu = cpu_thread_args->cpu;
  int pid = cpu_thread_args->pid;

  cpu_history[cpu->get_id()].push_back(pid);
  process_history[pid].push_back(cpu->get_id());

  auto process = processes_map[pid];

  auto start = chrono::high_resolution_clock::now();

  auto start_in_nano =
      chrono::duration_cast<chrono::microseconds>(start.time_since_epoch())
          .count();

  process.waiting_time += start_in_nano - process.start_time;

  // Inicializando quantum
  int quantum = 0;

  // Buscar PCB na RAM
  cpu->actual_pcb = cpu->get_ram()->get_PCB(process.pcb_address);

  // Atualizando registradores
  cpu->PC = cpu->actual_pcb.PC;
  cpu->set_registers(cpu->actual_pcb.registers);

  while (cpu->InstructionFetch()) {
    cpu->InstructionDecode();
    cpu->Execute();
    cpu->MemoryAccess();
    cpu->WriteBack();

    quantum += 5;

    cpu->actual_pcb.PC = cpu->PC;

    // Atualizando registradores do PCB
    for (int i = 0; i < REGISTERS_SIZE; i++) {
      cpu->actual_pcb.registers[i] = cpu->get_register(i);
    }

    // Verifica fim do processo
    if (cpu->actual_pcb.PC >= cpu->actual_pcb.program_size) {
      auto end = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

      // Salvando tempo de CPU
      process.cpu_time += duration.count();

      long end_in_nano =
          chrono::duration_cast<chrono::microseconds>(end.time_since_epoch())
              .count();

      process.start_time = end_in_nano;
      process.state = TERMINATED;

      processes_map[process.pid] = process;

      cpu->get_ram()->update_PCB(process.pcb_address, cpu->actual_pcb);

      cout << "Process " << process.pid << " finished!" << endl;
      break;
    }

    // Verifica se quantum expirou
    if (quantum >= QUANTUM) {
      auto end = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

      process.cpu_time += duration.count();

      process.start_time =
          chrono::duration_cast<chrono::microseconds>(end.time_since_epoch())
              .count();

      processes_map[process.pid] = process;

      cpu->get_ram()->update_PCB(process.pcb_address, cpu->actual_pcb);

      // Muda estado para READY
      process.state = READY;

      // Adiciona processo na fila de prontos
      ready_process[cpu->get_id()] = process.pid;

      break;
    }
  }

  pthread_mutex_unlock(&core_mutex[cpu->get_id()]);
  free(cpu_thread_args);
  pthread_exit(NULL);
}

bool Cpu::InstructionFetch() {
  if (this->PC >= actual_pcb.program_size) return false;

  this->active_instruction =
      ram->get_instruction(actual_pcb.program_address, this->PC);

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
      write_value =
          ula.add(get_register(get_register(2)), get_register(get_register(3)));

      write_data = true;
      break;
    }
    case SUB: {
      write_value =
          ula.sub(get_register(get_register(2)), get_register(get_register(3)));

      write_data = true;
      break;
    }
    case MUL: {
      write_value =
          ula.mul(get_register(get_register(2)), get_register(get_register(3)));

      write_data = true;
      break;
    }
    case DIV: {
      write_value =
          ula.div(get_register(get_register(2)), get_register(get_register(3)));

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
      // write_value = cache->read(address);
      write_value = ram->get_value(address);
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
      // cache->write(address, value);
      ram->set_value(address, value);
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