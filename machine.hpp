// (c) Daniel Desmond Dennis 2018

#ifndef MACHINE_HPP
#define MACHINE_HPP

#include <cinttypes>
#include <iostream>

#include "instructions.hpp"
#include "memory.hpp"

#define ANSI_BLUE ""
#define ANSI_GREEN ""
#define ANSI_RED "\e[38;5;196m"
#define COLOUR_END "\e[0m"

class Machine : protected Instructions, protected Memory
{
private:
    int curr_instruction; // Program Counter
    uint32_t reg[NUM_REG]; // Registers
    bool n_flag, z_flag, c_flag, v_flag; // Code flags

    void execute();
    void print_registers();
    uint32_t fetch_reg_val(uint8_t reg);

    void ex_ADD(int inst);
    void ex_SUB(int inst);
    void ex_MUL(int inst);
    void ex_AND(int inst);
    void ex_ORR(int inst);
    void ex_MOV(int inst);
    void ex_EOR(int inst);
    void ex_MVN(int inst);

    void ex_LDR(int inst);
    void ex_LDRH(int inst);
    void ex_LDRB(int inst);
    void ex_STR(int inst);
    void ex_STRH(int inst);
    void ex_STRB(int inst);

    uint8_t ex_LDM(uint32_t registers, uint8_t address, bool increment, bool after);
    uint8_t ex_STM(uint32_t registers, uint8_t address, bool increment, bool after);

    inline uint32_t shift(uint32_t val0, uint32_t val1, uint8_t type);
    int return_label(int inst);

    void print_instructions();
public:
    Machine();
    void run_machine();
};

#endif