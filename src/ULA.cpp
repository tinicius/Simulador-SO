#include "ULA.hpp"

ULA::ULA() {}

int ULA::add(int a, int b) { return a + b; }

int ULA::sub(int a, int b) { return a - b; }

int ULA::mul(int a, int b) { return a * b; }

int ULA::div(int a, int b) { return a / b; }

int ULA::slt(int a, int b) { return a < b; }

int ULA::diff(int a, int b) { return a != b; }

int ULA::equal(int a, int b) { return a == b; }