#include "DynamicBitset/DynamicBitset.hpp"
#include <iostream>
#include <bitset>

int main() {
  DynamicBitset one(70, ~0);
  //DynamicBitset two(70, 24);
  //std::cout << one.to_string() << std::endl << two.to_string() << std::endl;
  //std::cout << (one ^ two).to_string() << std::endl;
  DynamicBitset three;
  three = one << 65;
  std::cout << one.to_string() << std::endl;
  std::cout << three.to_string() << std::endl;

  return 0;
}