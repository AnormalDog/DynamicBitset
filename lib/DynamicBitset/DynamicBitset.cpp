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
#include <sstream>
#include <algorithm>

DynamicBitset::DynamicBitset(const std::size_t t_size, const std::size_t t_num) {
  build(t_size);
  std::cout << "size: " << m_size << std::endl;
  m_bits[0] = t_num;
}

DynamicBitset::DynamicBitset(const std::size_t t_size) {
  build(t_size);
  clean();
}

DynamicBitset::DynamicBitset() {
  build(BLOCK_SIZE);
  clean();
}

DynamicBitset::~DynamicBitset() {
  destroy();
}

DynamicBitset::DynamicBitset(const DynamicBitset& t_dynamicBitset) {
  copy(*this, t_dynamicBitset);
}

DynamicBitset& DynamicBitset::operator=(const DynamicBitset& t_dynamicBitset) {
  copy(*this, t_dynamicBitset);
  return *this;
}

DynamicBitset::DynamicBitset(DynamicBitset&& t_dynamicBitset) {
  move(*this, t_dynamicBitset);
}

DynamicBitset& DynamicBitset::operator=(DynamicBitset&& t_dynamicBitset) {
  move(*this, t_dynamicBitset);
  return *this;
}

std::string DynamicBitset::to_string() const noexcept {
  std::string toReturn;
  for (long long i = m_blocks - 1; i >= 0; --i) {
    std::stringstream buffer;
    std::size_t block = m_bits[i] & m_mask[i];
    std::size_t mask = m_mask[i];
    for (std::size_t j = 0; j < BLOCK_SIZE; ++j) {
      if (mask % 2 != 0) {
        if (block % 2 != 0) {
          buffer << '1';
        }
        else {
          buffer << '0';
        }
      }
      block >>= 1;
      mask >>= 1;
    }  
    std::string aux = buffer.str();
    std::reverse(aux.begin(), aux.end());
    toReturn += aux;
  }
  return toReturn;
}

void DynamicBitset::build(const std::size_t t_size) {
  if (t_size == 0) throw (DynamicBitsetUtils::DynamicBitsetExceptionInvalidSize());
  destroy();
  m_size = t_size;
  m_blocks = getNumberBlocks(t_size);
  std::cout << "size: " << m_size << std::endl << "blocks: " << m_blocks << std::endl;
  buildBits();
  buildMask();
}

std::size_t DynamicBitset::getNumberBlocks(const std::size_t t_size) noexcept {
  std::size_t numberOfBlocks = 0;
  for (std::size_t i = 0; i < t_size; i+=BLOCK_SIZE) {
    ++numberOfBlocks;
  }
  return numberOfBlocks;
}

void DynamicBitset::buildBits() {
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
    m_bits[i] = 0;
  }
}

std::size_t DynamicBitset::getLastMask(const std::size_t t_number_bits) {
  std::size_t lastMask = ALL_BITS_ONE;
  lastMask >>= (BLOCK_SIZE - t_number_bits);
  return lastMask;
}

void DynamicBitset::destroy() {
  if (m_bits != nullptr) {
    delete[] m_bits;
    m_bits = nullptr;
  }
  if (m_mask != nullptr) {
    delete[] m_mask;
    m_mask = nullptr;
  }
  m_size = 0;
  m_blocks = 0;
}

void DynamicBitset::copy(DynamicBitset& t_copy, const DynamicBitset& t_toCopy) {
  t_copy.destroy();
  t_copy.build(t_toCopy.size());
  for (std::size_t i = 0; i < t_copy.size(); ++i) {
    t_copy.m_bits[i] = t_toCopy.m_bits[i];
  }
}

void DynamicBitset::move(DynamicBitset& t_move, DynamicBitset& t_toMove) {
  t_move.destroy();
  // MOVE
  t_move.m_bits = t_toMove.m_bits;
  t_move.m_mask = t_toMove.m_mask;
  t_move.m_size = t_toMove.m_size;
  t_move.m_blocks = t_toMove.m_blocks;
  // CLEAN
  t_toMove.m_bits = nullptr;
  t_toMove.m_mask = nullptr;
  t_toMove.m_size = 0;
  t_toMove.m_blocks = 0;
}

unsigned long long DynamicBitset::to_ullong() const noexcept {
  std::size_t bits = m_bits[0] & m_mask[0];
  return static_cast<unsigned long long>(bits);

}

unsigned long DynamicBitset::to_ulong() const noexcept {
  std::size_t bits = m_bits[0] & m_mask[0];
  return static_cast<unsigned long>(bits);
}

bool DynamicBitset::all() const noexcept {
  for ( std::size_t i = 0; i < m_blocks; ++i) {
    if ((m_bits[i] & m_mask[i]) != m_mask[i]) return false;
  }
  return true;
}

bool DynamicBitset::any() const noexcept {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    if ((m_bits[i] & m_mask[i]) != 0) return true;
  }
  return false;
}

bool DynamicBitset::none() const noexcept {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    if ((m_bits[i] & m_mask[i]) != 0) return false;
  }
  return true;
}