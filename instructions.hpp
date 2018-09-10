// (c) Daniel Desmond Dennis 2018

#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <vector>
#include <cinttypes>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "errorhandler.hpp"

#define FILE_NAME "program.txt"
#define LINE_LEN 50
#define NUM_REG 16

class Instructions: protected Error_Handler
{
private:
    uint16_t instruction_type(int start, char* argv, unsigned int curr_line);   // Read instruction type
    uint64_t val_type(int start, char* argv, unsigned int curr_line);           // Read Rd, Rn, Rm values
    bool readline(char* str, int lim, FILE* file);
    void preprocess(char* str);

protected:
    struct instruction
    {
        uint8_t type; // Instruction type, see bottom list
        uint8_t dest_reg; // 0x1F = destination register -> Rd
                          // 0xE0 = Shift operation (See table below)
        uint32_t val0; // Rn
        bool val0reg;
        uint32_t val1; // Rm
        bool val1reg;

        bool flags; // Whether code flags should be updated or not
        bool address; // Whether instruction contains address
        bool postindex; // Whether register is incremented before or after memory op

        // Used for finding labels
        std::string label;
        unsigned int line;
    };

    struct label_struct
    {
        std::string label;
        unsigned int instruction;
    };

    std::vector<struct instruction> program;
    std::vector<struct label_struct> labels;

    int start();
    const char* inst_type(uint8_t type);
};

#endif

/*
    List of instruction types (decimal)

    N/A.........0
    ADD.........1
    SUB.........2
    MUL.........3
    AND.........4
    ORR.........5
    MOV.........6
    EOR.........7
    MVN.........8

    LDR.........9
    LDRH.......10
    LDRB.......11
    STR........12
    STRH.......13
    STRB.......14

    CMP........15

    B / BAL....16
    BEQ........17
    BNE........18
    BCS / BCS..19
    BCC / BLO..20
    BMI........21
    BPL........22
    BVS........23
    BVC........24
    BHI........25
    BLS........26
    BGE........27
    BLT........28
    BGT........29
    BLE........30
    BAL........31

    LDMIA......32
    LDMIB......33
    LDMDA......34
    LDMDB......35
    STMIA......36
    STMIB......37
    STMDA......38
    STMDB......39
    LDM........40
    STM........41

*/

/*
    List of bitwise shift operations (binary)

    000 No Shift
    001 LSR
    010 ASR
    011 ROR
    100 LSL
    101 ASL
    110 ROL
    111 [Unused]
*/