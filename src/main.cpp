
#include "Cpu.hpp"
#include "OperatingSystem.hpp"

int main() {
  Scheduler scheduler;

  for (int i = 0; i < 10; i++) scheduler.add_running(i);

  OperatingSystem so(scheduler);
  so.boot();

  pthread_t t_so;
  pthread_create(&t_so, NULL, run_os, &so);
  pthread_join(t_so, NULL);

  return 0;
}