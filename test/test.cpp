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
  DynamicBitset::Reference ref = one[15];
  ref.flip();
  std::cout << ref << std::endl;
  one[10] = false;
  std::cout << one << std::endl;

  return 0;
}