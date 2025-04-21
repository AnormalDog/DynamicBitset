/**
 * Creator: AnormalDog
 * License: MIT
 * 
 * source file, implementation of the class DynamicBitset, represents
 *   a bitset without template
 */

#include "DynamicBitset/DynamicBitset.hpp"
#include <iostream>
#include <bitset>

DynamicBitset::DynamicBitset() {
  m_bits = nullptr;
  m_mask = nullptr;
  m_size = 0;
  m_blocks = 0;
}

DynamicBitset::~DynamicBitset() {
  if (m_bits != nullptr) {
    delete[] m_bits;
  }
  if (m_mask != nullptr) {
    delete[] m_mask;
  }
  m_size = 0;
  m_blocks = 0;
}

DynamicBitset::DynamicBitset(const std::size_t t_size) {
  m_size = t_size;
  m_blocks = getNumberBlocks(t_size);
  buildBits();
  buildMask();
  clean();
}

std::size_t DynamicBitset::getNumberBlocks(const std::size_t t_size) noexcept {
  std::size_t numberOfBlocks = 0;
  for (std::size_t i = 0; i < t_size; i+=BLOCK_SIZE) {
    ++numberOfBlocks;
  }
  return numberOfBlocks;
}

void DynamicBitset::buildBits() {
  if (m_bits != nullptr) {
    delete[] m_bits;
  }
  if (m_mask != nullptr) {
    delete[] m_mask;
  }
  m_bits = new std::size_t[m_blocks];
  m_mask = new std::size_t[m_blocks];
}

void DynamicBitset::buildMask() {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    m_mask[i] = ALL_BITS_ONE;
  }
  std::size_t lastMask = m_size - ((m_blocks -1) * BLOCK_SIZE);
  m_mask[m_blocks - 1] = getLastMask(lastMask);
}

void DynamicBitset::clean() {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    m_bits[i] = ALL_BITS_ONE;
  }
}

std::size_t DynamicBitset::getLastMask(const std::size_t t_number_bits) {
  std::size_t lastMask = ALL_BITS_ONE;
  lastMask >>= (BLOCK_SIZE - t_number_bits);
  return lastMask;
}

void DynamicBitset::print() const noexcept {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    std::size_t block = m_bits[i] & m_mask[i];
    std::size_t mask = m_mask[i];
    for (std::size_t j = 0; j < BLOCK_SIZE; ++j) {
      if (mask % 2 != 0) {
        if (block % 2 != 0) {
          std::cout << '1';
        }
        else {
          std::cout << '0';
        }
      }
      block >>= 1;
      mask >>= 1;
    }
  }
  std::cout << std::endl;
}