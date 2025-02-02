#ifndef CACHEFIFO_HPP
#define CACHEFIFO_HPP

#include <queue>
#include <string>
#include <unordered_map>

#include "CacheType.hpp"
#include "globals.hpp"

using namespace std;

class CacheFIFO : public CacheType {
 private:
  map<pair<int, int>, string> instructions;
  queue<pair<int, int>> fifo;

 public:
  string get_instruction(int program_address, int pc);

  void add_instruction(int program_address, int pc, string instruction);
};

#endif