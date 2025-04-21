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
    DynamicBitset(const DynamicBitset& t_dynamicBitset); // Copy constructor
    DynamicBitset& operator=(const DynamicBitset& t_dynamicBitset); // Copy assignment
    DynamicBitset(DynamicBitset&& t_dynamicBitset); // Move constructor
    DynamicBitset& operator=(DynamicBitset&& t_dynamicBitset); // Move assignment

    std::string to_string() const noexcept;
    unsigned long long to_ullong() const noexcept;
    unsigned long to_ulong() const noexcept;

    bool all() const noexcept;
    bool any() const noexcept;
    bool none() const noexcept;

    inline std::size_t size() const noexcept {return m_size;}
    
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
    void build(const std::size_t t_size); // Call buildBits, buildMask
    void clean(); // Put all bits to 0
    void destroy(); // Destroy the object
    static void copy(DynamicBitset& t_copy, const DynamicBitset& t_toCopy);
    static void move(DynamicBitset& t_copy, DynamicBitset& t_toMove);
    static std::size_t getNumberBlocks(const std::size_t t_size) noexcept; // Method to calculate the number of needed blocks
    static std::size_t getLastMask(const std::size_t t_number_bits); // Method to calculate the mask of the last block
};

namespace DynamicBitsetUtils {

class DynamicBitsetException : public std::exception {
  public:
    DynamicBitsetException(const std::string& t_message) : m_errorMessage(t_message) {}
    const char* what() const noexcept override {return m_errorMessage.c_str();}
  private:
    std::string m_errorMessage;
};

class DynamicBitsetExceptionInvalidSize : public DynamicBitsetException {
  public:
    DynamicBitsetExceptionInvalidSize() : DynamicBitsetException("Invalid size creating the DynamicBitset.") {}
};

}