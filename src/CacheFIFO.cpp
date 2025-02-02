#include "CacheFIFO.hpp"

string CacheFIFO::get_instruction(int program_address, int pc) {
  pthread_mutex_lock(&cache_mutex);

  auto find = instructions.find(make_pair(program_address, pc));

  if (find == instructions.end()) {
    pthread_mutex_unlock(&cache_mutex);
    return "";
  }

  pthread_mutex_unlock(&cache_mutex);

  return find->second;
}

void CacheFIFO::add_instruction(int program_address, int pc,
                                string instruction) {
  pthread_mutex_lock(&cache_mutex);

  if ((int)instructions.size() == CACHE_SIZE) {
    auto oldest = fifo.front();
    fifo.pop();

    instructions.erase(oldest);
  }

  instructions[make_pair(program_address, pc)] = instruction;
  fifo.push(make_pair(program_address, pc));

  pthread_mutex_unlock(&cache_mutex);
}