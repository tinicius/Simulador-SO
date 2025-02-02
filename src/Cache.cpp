#include "Cache.hpp"

Cache::Cache(CacheType* cache) : cache(cache) {
  pthread_mutex_init(&cache_mutex, NULL);
}

Cache::~Cache() {
  pthread_mutex_destroy(&cache_mutex);
  delete cache;
}

void Cache::add_instruction(int program_address, int pc, string instruction) {
  return cache->add_instruction(program_address, pc, instruction);
}

string Cache::get_instruction(int program_address, int pc) {
  return cache->get_instruction(program_address, pc);
}