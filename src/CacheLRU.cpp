#include "CacheLRU.hpp"

#include <pthread.h>

string CacheLRU::get_instruction(int program_address, int pc) {
  pthread_mutex_lock(&cache_mutex);

  usleep(100);

  auto find = cache_map.find(make_pair(program_address, pc));

  if (find == cache_map.end()) {
    CACHE_MISS++;
    pthread_mutex_unlock(&cache_mutex);
    return "";
  }

  cache_order.splice(cache_order.begin(), cache_order, find->second);

  CACHE_HIT++;
  pthread_mutex_unlock(&cache_mutex);

  return find->second->second;
}

void CacheLRU::add_instruction(int program_address, int pc,
                               string instruction) {
  pthread_mutex_lock(&cache_mutex);

  auto find = cache_map.find(make_pair(program_address, pc));

  if (find != cache_map.end()) {
    find->second->second = instruction;
    cache_order.splice(cache_order.begin(), cache_order, find->second);
  } else {
    if ((int)cache_order.size() == CACHE_SIZE) {
      auto last = cache_order.back();
      cache_map.erase(last.first);
      cache_order.pop_back();
    }

    cache_order.push_front(
        make_pair(make_pair(program_address, pc), instruction));
    cache_map[make_pair(program_address, pc)] = cache_order.begin();
  }

  pthread_mutex_unlock(&cache_mutex);
}
