#include "DynamicBitset/DynamicBitset.hpp"
#include <iostream>
#include <bitset>

int main() {
  DynamicBitset dynamicbitset(66, 0);
  dynamicbitset.flip(64).flip(60).flip(14);
  
  std::cout << dynamicbitset.count() << std::endl;
  return 0;
}