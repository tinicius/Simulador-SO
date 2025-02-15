#ifndef CACHELRU_HPP
#define CACHELRU_HPP

#include <pthread.h>
#include <unistd.h>

#include <list>
#include <string>
#include <unordered_map>

#include "CacheType.hpp"
#include "globals.hpp"

using namespace std;

struct pair_hash {
  template <typename T1, typename T2>
  std::size_t operator()(const std::pair<T1, T2>& p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);

    return h1 ^ (h2 << 1);
  }
};

class CacheLRU : public CacheType {
 private:
  list<pair<pair<int, int>, string>> cache_order;

  unordered_map<pair<int, int>, list<pair<pair<int, int>, string>>::iterator,
                pair_hash>
      cache_map;

 public:
  string get_instruction(int program_address, int pc) override;

  void add_instruction(int program_address, int pc,
                       string instruction) override;
};

#endif
