// (c) Daniel Desmond Dennis 2018

#include "memory.hpp"

void Memory::store_byte(uint8_t input, uint32_t address)
{
    if(address > MEM_LIM)
    {
        return;
    }
    else
    {
        ram[address] = input;
    }
}

void Memory::store_halfword(uint16_t input, uint32_t address)
{
    if(address > MEM_LIM)
    {
        return;
    }
    else
    {
        ram[address++] = (input & 0xFF00) >> 8;
        ram[address]   = (input & 0x00FF) >> 0;
    }
}

void Memory::store_word(uint32_t input, uint32_t address)
{
    if(address > MEM_LIM)
    {
        return;
    }
    else
    {
        ram[address++] = (input & 0xFF000000) >> 24;
        ram[address++] = (input & 0x00FF0000) >> 16;
        ram[address++] = (input & 0x0000FF00) >> 8;
        ram[address]   = (input & 0x000000FF) >> 0;
    }
}

uint8_t Memory::load_byte(uint32_t address)
{
    return ram[address];
}

uint16_t Memory::load_halfword(uint32_t address)
{
    uint16_t val;
    val  = ram[address++] << 8;
    val += ram[address];
    return val;
}

uint32_t Memory::load_word(uint32_t address)
{
    uint32_t val;
    val  = ram[address++] << 24;
    val += ram[address++] << 16;
    val += ram[address++] << 8;
    val += ram[address];
    return val;
}

void Memory::print_memory()
{
    print_memory(0, MEM_LIM - 1);
}

void Memory::print_memory(uint32_t lower, uint32_t upper)
{
    uint8_t nextline = PRINT_WIDTH;
    
    std::cout << "0x" << std::setw(8) << std::right << std::setfill('0')
        << std::hex << std::uppercase << std::setprecision(8) << 0;

    for(int i = (int)lower; i <= (int)upper; i++)
    {
        if(!nextline)
        {
            std::cout << std::endl << "0x" << std::setw(8) << std::right << std::hex << std::uppercase << std::setprecision(8) << i;
            std::cout << " 0x" << std::uppercase << std::hex << std::setw(2) << std::left << (int)ram[i];
            nextline = PRINT_WIDTH;
        }
        else
        {
            std::cout << " 0x" << std::uppercase << std::setfill('0') << std::hex << std::setw(2) << std::left << (int)ram[i];
        }
        nextline--;
    }
    std::cout << std::endl;
}

Memory::Memory()
{
    memset(ram, 0, MEM_LIM);
}