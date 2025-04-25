/**
 * Creator: AnormalDog
 * License: MIT
 * 
 * test file used for testing DynamicBitset
 */


// g++ -Wall -Wextra -Werror -I lib/ -g lib/DynamicBitset/DynamicBitset.cpp test/test.cpp

#include "DynamicBitset/DynamicBitset.hpp"
#include <iostream>
#include <bitset>

using namespace DynBitset;

int main() {
  DynamicBitset one(70, ~0);
  std::cout << one.to_string() << std::endl;
  //DynamicBitset two(70, 24);
  //std::cout << one.to_string() << std::endl << two.to_string() << std::endl;
  //std::cout << (one ^ two).to_string() << std::endl;
  DynamicBitset three;
  three = one << 65;
  std::cout << one.to_string() << std::endl;
  std::cout << three.to_string() << std::endl;

  return 0;
}