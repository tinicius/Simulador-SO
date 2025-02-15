#include "Cpu.hpp"

#include "ControlUnit.hpp"

Cpu::Cpu(int id, Ram* ram, Cache* cache)
    : id(id),
      cache(cache),
      ram(ram),
      control_unit(this),
      pipeline(&control_unit),
      write_data(false) {}

Cpu::Cpu(int id, Ram* ram)
    : id(id),
      cache(nullptr),
      ram(ram),
      control_unit(this),
      pipeline(&control_unit),
      write_data(false) {}

int Cpu::get_id() { return id; }

Ram* Cpu::get_ram() { return ram; }

PipelineMips* Cpu::get_pipeline() { return &pipeline; }

ULA* Cpu::get_ula() { return &ula; }

string Cpu::get_instruction(int program_address, int PC) {
  if (cache == nullptr) return ram->get_instruction(program_address, PC);

  string cached_instruction = cache->get_instruction(program_address, PC);

  if (cached_instruction != "") return cached_instruction;

  string instruction = ram->get_instruction(program_address, PC);
  cache->add_instruction(program_address, PC, instruction);

  return instruction;
}

int Cpu::get_pc() { return PC; }

void Cpu::set_pc(int pc) { PC = pc; }

int Cpu::get_register(int address) { return register_bank.get_value(address); }

void Cpu::set_register(int address, int value) {
  register_bank.set_value(address, value);
}

void Cpu::set_registers(int registers[REGISTERS_SIZE]) {
  for (int i = 0; i < REGISTERS_SIZE; i++) {
    register_bank.set_value(i, registers[i]);
  }
}

int Cpu::get_write_value() { return write_value; }

void Cpu::set_write_value(int value) { write_value = value; }

bool Cpu::get_write_data() { return write_data; }

void Cpu::set_write_data(bool value) { write_data = value; }

void Cpu::log(const string& message) {
  if (LOGS_ENABLED) CpuLogger::log(id, message);
}

void Cpu::log_all(const string& message) {
  if (LOGS_ENABLED) CpuLogger::log_all(message);
}

void* run_core(void* args) {
  CpuThreadArgs* cpu_thread_args = (CpuThreadArgs*)args;

  Cpu* cpu = cpu_thread_args->cpu;
  int pid = cpu_thread_args->pid;

  cpu->log("Process " + to_string(pid) + " started");
  cpu->log_all("Process " + to_string(pid) + " started at core " +
               to_string(cpu->get_id()));

  cpu_history[cpu->get_id()].push_back(pid);
  process_history[pid].push_back(cpu->get_id());

  auto process = processes_map[pid];

  auto start = chrono::high_resolution_clock::now();

  auto start_in_nano =
      chrono::duration_cast<chrono::microseconds>(start.time_since_epoch())
          .count();

  process.waiting_time += start_in_nano - process.start_time;

  // Inicializando quantum
  int quantum = process.quantum;

  // Buscar PCB na RAM
  cpu->actual_pcb = cpu->get_ram()->get_PCB(process.pcb_address);

  // Atualizando registradores
  cpu->set_pc(cpu->actual_pcb.PC);
  cpu->set_registers(cpu->actual_pcb.registers);

  auto pipeline = cpu->get_pipeline();

  while (pipeline->instruction_fetch(cpu->get_pc(),
                                     cpu->actual_pcb.program_address,
                                     cpu->actual_pcb.program_size)) {
    pipeline->instruction_decode();
    pipeline->execute();
    pipeline->memory_access();
    pipeline->write_back();

    quantum -= 5;

    process.remaining_instructions--;

    cpu->actual_pcb.PC = cpu->get_pc();

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

      cpu->log("Process " + to_string(pid) + " finished");
      cpu->log_all("Process " + to_string(pid) + " finished at time " +
                   to_string(end_in_nano));
      break;
    }

    // Verifica se quantum expirou
    if (quantum == 0) {
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

      cpu->log("Quantum expired for process " + to_string(pid));
      cpu->log_all("Quantum expired for process " + to_string(pid));
      break;
    }
  }

  pthread_mutex_unlock(&core_mutex[cpu->get_id()]);
  free(cpu_thread_args);
  pthread_exit(NULL);
}
