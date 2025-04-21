#include "DynamicBitset/DynamicBitset.hpp"
#include <iostream>
#include <bitset>

int main() {
  DynamicBitset dynamicbitset(70, ~0);
  std::cout << dynamicbitset.to_string() << std::endl;
  std::cout << dynamicbitset.all() << std::endl;
  std::cout << dynamicbitset.any() << std::endl;
  std::cout << dynamicbitset.none() << std::endl;
  return 0;
}