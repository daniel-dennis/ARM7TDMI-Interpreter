// (c) Daniel Desmond Dennis 2018

/*
    For simplicity, this currently uses big endian only
*/ 

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <cinttypes>
#include <iostream>
#include <iomanip>
#include <cmath>

#define MEM_LIM 32768 // 32KB of RAM
#define PRINT_WIDTH 12

class Memory
{
private:
    uint8_t ram[MEM_LIM];
protected:
    void store_byte(uint8_t input, uint32_t address);
    void store_halfword(uint16_t input, uint32_t address);
    void store_word(uint32_t input, uint32_t address);

    uint8_t load_byte(uint32_t address);
    uint16_t load_halfword(uint32_t address);
    uint32_t load_word(uint32_t address);

    void print_memory();
    void print_memory(uint32_t lower, uint32_t upper);
    Memory();
};

#endif