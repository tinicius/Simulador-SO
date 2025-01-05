#include "CpuLogger.hpp"

vector<vector<string>> core_logs;
vector<string> all_cores_logs;

pthread_mutex_t log_mutex;

void CpuLogger::configure_logger() {
  core_logs.resize(CORES_COUNT);
  pthread_mutex_init(&log_mutex, NULL);
}

void CpuLogger::log(int id, string message) {
  core_logs[id].push_back(message);
}

void CpuLogger::log_all(string message) {
  pthread_mutex_lock(&log_mutex);
  all_cores_logs.push_back(message);
  pthread_mutex_unlock(&log_mutex);
}

void CpuLogger::print() {
  for (int i = 0; i < CORES_COUNT; i++) {
    ofstream core_file("./output/cores/core_" + to_string(i) + ".log");

    for (string log : core_logs[i]) {
      core_file << log << endl;
    }

    core_file.close();
  }

  ofstream all_cores_file("./output/all_cores.log");

  for (string log : all_cores_logs) {
    all_cores_file << log << endl;
  }

  all_cores_file.close();
}