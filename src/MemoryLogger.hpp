#ifndef MEMORY_LOGGER_HPP
#define MEMORY_LOGGER_HPP

#include <signal.h>

#include <iomanip>
#include <iostream>

#include "Cache.hpp"
#include "Ram.hpp"

class MemoryLogger {
 private:
  Ram* ram;

 public:
  MemoryLogger(Ram* ram);

  void print();
};

#endif