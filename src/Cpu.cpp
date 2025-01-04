#include "Cpu.hpp"

#include "ControlUnit.hpp"

Cpu::Cpu(int id, Ram* ram, Cache* cache)
    : id(id),
      cache(cache),
      ram(ram),
      control_unit(this),
      pipeline(&control_unit),
      logger(MemoryLogger::get_instance()),
      write_data(false) {}

int Cpu::get_id() { return id; }

Ram* Cpu::get_ram() { return ram; }

PipelineMips* Cpu::get_pipeline() { return &pipeline; }

ULA* Cpu::get_ula() { return &ula; }

string Cpu::get_instruction(int program_address, int PC) {
  return ram->get_instruction(program_address, PC);
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

    quantum += 5;

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
