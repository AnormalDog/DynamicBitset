/**
 * Creator: AnormalDog
 * License: MIT
 * 
 * header file, interface of the class DynamicBitset, represents
 *   a bitset without template
 */

#pragma once

#include <exception>
#include <string>
#include <cstddef>

class DynamicBitset {
  public:
    DynamicBitset(const std::size_t t_size, const std::size_t t_num);
    DynamicBitset(const std::size_t t_size);
    // SPECIAL MEMBERS
    DynamicBitset(); // Default constructor
    ~DynamicBitset(); // Destructor
    DynamicBitset(const DynamicBitset& t_DynamicBitset); // Copy constructor
    DynamicBitset& operator=(const DynamicBitset& t_DynamicBitset); // Copy assignment
    DynamicBitset(DynamicBitset&& t_DynamicBitset); // Move constructor
    DynamicBitset& operator=(DynamicBitset&& t_DynamicBitset); // Move assignment

    void print() const noexcept;
    
  private:
    std::size_t* m_bits = nullptr; // little endian
    std::size_t* m_mask = nullptr; // little endian
    std::size_t  m_size;
    std::size_t  m_blocks;

    static constexpr std::size_t BLOCK_SIZE = sizeof(std::size_t) * 8; // Number of bits of each block
    static constexpr std::size_t ALL_BITS_ONE = ~(0);

    // PRIVATE METHODS
    void buildBits();
    void buildMask();
    void clean();
    static std::size_t getNumberBlocks(const std::size_t t_size) noexcept;
    static std::size_t getLastMask(const std::size_t t_number_bits);
};

namespace DynamicBitsetUtils {

class DynamicBitsetException : public std::exception {
  public:
    DynamicBitsetException(const std::string& t_message) : m_errorMessage(t_message) {}
    const char* what() const noexcept override {return m_errorMessage.c_str();}
  private:
    std::string m_errorMessage;
};

}