#ifndef CACHE_TYPE_HPP
#define CACHE_TYPE_HPP

#include <pthread.h>

#include <string>

using namespace std;

class CacheType {
 private:
 public:
  virtual ~CacheType() {};
  virtual string get_instruction(int program_address, int pc) = 0;
  virtual void add_instruction(int program_address, int pc,
                               string instruction) = 0;
};

#endif