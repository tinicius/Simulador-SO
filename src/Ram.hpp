#ifndef RAM_HPP
#define RAM_HPP

#include <fstream>
#include <iostream>

using namespace std;

#define MAX_SIZE 32

class Ram {
 private:
  int values[MAX_SIZE];

 public:
  Ram();
  int get_value(int address);
  void set_value(int address, int value);
};

#endif
