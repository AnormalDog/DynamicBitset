#include "DynamicBitset/DynamicBitset.hpp"
#include <iostream>
#include <bitset>

int main() {
  DynamicBitset dynamicbitset(1, 1);
  std::bitset<0> perro;
  std::cout << dynamicbitset.to_string() << std::endl;
  return 0;
}