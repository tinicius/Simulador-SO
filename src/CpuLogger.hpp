#ifndef CPU_LOGGER_HPP
#define CPU_LOGGER_HPP

#include <fstream>
#include <string>
#include <vector>

#include "globals.hpp"

using namespace std;

extern vector<vector<string>> core_logs;
extern vector<string> all_cores_logs;

class CpuLogger {
 public:
  static void configure_logger();

  static void log(int id, string message);
  static void log_all(string message);

  static void print();
};

#endif