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
#include <cassert>

DynamicBitset::DynamicBitset(const std::size_t t_size, const std::size_t t_num) {
  build(t_size);
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
  for (std::size_t i = 0; i < t_copy.m_blocks; ++i) {
    const std::size_t blockToCopy = t_toCopy.m_bits[i];
    t_copy.m_bits[i] = blockToCopy;
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
  t_toMove.build(1);
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

std::pair<std::size_t, std::size_t> DynamicBitset::getPosition(std::size_t t_position) const {
  if (t_position >= m_size) throw(DynamicBitsetUtils::DynamicBitsetOutOfRange());
  std::size_t blockPosition = 0;
  // Get in what block is allocated
  while (t_position >= BLOCK_SIZE) {
    t_position -= BLOCK_SIZE; // get internal block position
    ++blockPosition;
  }
  return std::make_pair(blockPosition, getMaskPosition(t_position));
}

std::size_t DynamicBitset::getMaskPosition(const std::size_t t_position) {
  assert(t_position <= BLOCK_SIZE);
  constexpr std::size_t auxMask = 1;
  return (auxMask << t_position);
}

DynamicBitset& DynamicBitset::set() noexcept {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    m_bits[i] = ALL_BITS_ONE;
  }
  return *this;
}

DynamicBitset& DynamicBitset::set(const std::size_t t_position) {
  const std::pair<std::size_t, std::size_t> position(getPosition(t_position));
  const std::size_t blockPosition = position.first;
  const std::size_t positionMask = position.second;
  m_bits[blockPosition] |= positionMask;

  return *this;
}

DynamicBitset& DynamicBitset::reset() noexcept {
  clean();
  return *this;
}

DynamicBitset& DynamicBitset::reset(const std::size_t t_position) {
  const std::pair<std::size_t, std::size_t> position(getPosition(t_position));
  const std::size_t blockPosition = position.first;
  const std::size_t positionMask = ~position.second; // Reversed position mask
  m_bits[blockPosition] &= positionMask;

  return *this;
}

DynamicBitset& DynamicBitset::flip() noexcept {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    m_bits[i] = ~m_bits[i];
  }
  return *this;
}

DynamicBitset& DynamicBitset::flip(const std::size_t t_position) {
  const std::pair<std::size_t, std::size_t> position(getPosition(t_position));
  const std::size_t blockPosition = position.first;
  const std::size_t positionMask = position.second;

  // flip the bit in the position
  const std::size_t positionValueReversed = ~m_bits[blockPosition] & positionMask;
  // original block with the position bit = 0
  const std::size_t allExceptPositionValue = m_bits[blockPosition] & ~positionMask;
  m_bits[blockPosition] = allExceptPositionValue | positionValueReversed;

  return *this;
}

void DynamicBitset::printDebug() const noexcept {
  std::cout << "size: " << m_size << std::endl << "blocks: " << m_blocks << std::endl;
}

std::size_t DynamicBitset::count() const noexcept {
  std::size_t numberOfActive = 0;
  for (std::size_t i = 0; i < m_blocks; ++i) {
    std::size_t block = m_bits[i] & m_mask[i]; // remove no significant bits
    for (std::size_t j = 0; j < BLOCK_SIZE; ++j) {
      if (block % 2 != 0) ++numberOfActive;
      block >>= 1;
    }
  }
  return numberOfActive;
}

bool DynamicBitset::operator[](std::size_t t_position) const {
  return getValueInPosition(t_position);
}

bool DynamicBitset::test(std::size_t t_position) const {
  return getValueInPosition(t_position);
}

bool DynamicBitset::getValueInPosition(std::size_t t_position) const {
  const std::pair<std::size_t, std::size_t> position(getPosition(t_position));
  const std::size_t blockPosition = position.first;
  const std::size_t positionMask = position.second;

  const std::size_t auxBlock = m_bits[blockPosition] & positionMask;
  if ((auxBlock | 0) == 0) return false;
  return true;
}

DynamicBitset operator&(const DynamicBitset& t_1, const DynamicBitset& t_2) {
  if (t_1.size() != t_2.size()) throw(DynamicBitsetUtils::DynamicBitsetSizeDismatch());
  DynamicBitset aux(t_1.size());
  for (std::size_t i = 0; i < aux.m_blocks; ++i) {
    aux.m_bits[i] = t_1.m_bits[i] & t_2.m_bits[i];
  }
  return aux;
}

DynamicBitset operator|(const DynamicBitset& t_1, const DynamicBitset& t_2) {
  if (t_1.size() != t_2.size()) throw(DynamicBitsetUtils::DynamicBitsetSizeDismatch());
  DynamicBitset aux(t_1.size());
  for (std::size_t i = 0; i < aux.m_blocks; ++i) {
    aux.m_bits[i] = t_1.m_bits[i] | t_2.m_bits[i];
  }
  return aux;
}

DynamicBitset operator^(const DynamicBitset& t_1, const DynamicBitset& t_2) {
  if (t_1.size() != t_2.size()) throw(DynamicBitsetUtils::DynamicBitsetSizeDismatch());
  DynamicBitset aux(t_1.size());
  for (std::size_t i = 0; i < aux.m_blocks; ++i) {
    aux.m_bits[i] = t_1.m_bits[i] ^ t_2.m_bits[i];
  }
  return aux;
}

DynamicBitset& DynamicBitset::operator&=(const DynamicBitset& t_other) {
  *this = *this & t_other;
  return *this;
}

DynamicBitset& DynamicBitset::operator|=(const DynamicBitset& t_other) {
  *this = *this | t_other;
  return *this;
}

DynamicBitset& DynamicBitset::operator^=(const DynamicBitset& t_other) {
  *this = *this ^ t_other;
  return *this;
}

DynamicBitset DynamicBitset::operator~() {
  for (std::size_t i = 0; i < this->m_blocks; ++i) {
    m_bits[i] = ~m_bits[i];
  }
  return *this;
}

DynamicBitset DynamicBitset::operator<<(std::size_t t_pos) const {
  DynamicBitset aux = *this;
  aux.bitwiseLeft(t_pos);
  return aux;
}

DynamicBitset DynamicBitset::operator>>(std::size_t t_pos) const {
  DynamicBitset aux = *this;
  aux.bitwiseRight(t_pos);
  return aux;
}

DynamicBitset& DynamicBitset::operator<<=(std::size_t t_pos) {
  this->bitwiseLeft(t_pos);
  return *this;
}

DynamicBitset& DynamicBitset::operator>>=(std::size_t t_pos) {
  this->bitwiseRight(t_pos);
  return *this;
}

void DynamicBitset::bitwiseLeft(std::size_t t_pos) {
  std::size_t blockWise = 0;
  while (t_pos >= BLOCK_SIZE) {
    ++blockWise;
    t_pos -= BLOCK_SIZE;
  }
  if (blockWise > 0) this->shiftBlocksLeft(blockWise);
  
  for (long long i = this->m_blocks - 1; i >= 0; --i) {
    const std::size_t remain = (this->m_bits[i] & this->m_mask[i]) >> (BLOCK_SIZE - t_pos);
    this->m_bits[i] = (this->m_bits[i] & this->m_mask[i]) << t_pos;
    if ((i + 1) < static_cast<long long>(this->m_blocks)) this->m_bits[i + 1] = this->m_bits[i + 1] | remain;
  }
}

void DynamicBitset::bitwiseRight(std::size_t t_pos) {
  std::size_t blockWise = 0;
  while (t_pos >= BLOCK_SIZE) {
    ++blockWise;
    t_pos -= BLOCK_SIZE;
  }
  if (blockWise > 0) this->shiftBlocksRight(blockWise);
  for (long long i = 0; i < static_cast<long long>(this->m_blocks); ++i) {
    const std::size_t remain = (this->m_bits[i] & this->m_mask[i]) << (BLOCK_SIZE - t_pos);
    this->m_bits[i] = (this->m_bits[i] & this->m_mask[i]) >> t_pos;
    if ((i - 1) >= static_cast<long long>(0)) this->m_bits[i - 1] = this->m_bits[i - 1] | remain;
  }
}

void DynamicBitset::shiftBlocksLeft(std::size_t t_pos) {
  for (long long i = m_blocks - 1; i >= 0; --i) {
    const long long newPos = i + t_pos;
    if (newPos < static_cast<long long>(m_blocks)) {
      m_bits[newPos] = m_bits[i];
    }
    m_bits[i] = 0;
  }
}

void DynamicBitset::shiftBlocksRight(std::size_t t_pos) {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    const long long newPos = i - t_pos;
    if (newPos >= 0) {
      m_bits[newPos] = m_bits[i];
    }
    m_bits[i] = 0;
  }
}