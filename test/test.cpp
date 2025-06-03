/**
 * Creator: AnormalDog
 * License: MIT
 * 
 * test file used for testing RuntimeBitset
 */


// g++ -Wall -Wextra -Werror -I lib/ -g lib/RuntimeBitset/RuntimeBitset.cpp test/test.cpp

#include "RuntimeBitset/RuntimeBitset.hpp"
#include <iostream>
#include <bitset>

using namespace DynBitset;

int main() {
  RuntimeBitset one(70, ~0);
  RuntimeBitset::Reference ref = one[15];
  ref.flip();
  std::cout << ref << std::endl;
  one[10] = false;
  std::cout << one << std::endl;

  return 0;
}