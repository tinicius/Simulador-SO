#ifndef CACHE_HPP
#define CACHE_HPP

#include <pthread.h>

#include <map>
#include <queue>
#include <string>
#include <vector>

#include "CacheType.hpp"
#include "globals.hpp"

using namespace std;

class Cache {
 private:
  CacheType *cache;

 public:
  Cache(CacheType *cache);
  ~Cache();

  string get_instruction(int program_address, int pc);
  void add_instruction(int program_address, int pc, string instruction);
};

#endif