#include "globals.hpp"

queue<Process> next_process;
pthread_mutex_t next_process_mutex = PTHREAD_MUTEX_INITIALIZER;

queue<Process> ready_processes;
pthread_mutex_t ready_processes_mutex = PTHREAD_MUTEX_INITIALIZER;
