// (c) Daniel Desmond Dennis 2018

#include "machine.hpp"

Machine::Machine()
{
    curr_instruction = 0;
    n_flag = z_flag = v_flag = c_flag = false;
    for(int i = 0; i < NUM_REG; i++) reg[i] = 0;
}

void Machine::run_machine()
{
    std::cout << ANSI_RED << "1. Reading instructions          \n" << COLOUR_END;
    Instructions::start();
    // Machine::print_instructions();
    if(!Error_Handler::print_errors())
    {
        std::cout << ANSI_RED << "2. Executing instructions    \n" << COLOUR_END;
        Machine::execute();
        std::cout << ANSI_RED << "3. Finished\n"                   << COLOUR_END;
        std::cout << ANSI_RED << "4. Current register values   \n" << COLOUR_END;
        Machine::print_registers();
        std::cout << ANSI_RED << "5. Memory                    \n" << COLOUR_END;
        Machine::print_memory(0, 0x15B);
    }
}

void Machine::execute()
{
    uint32_t temp;
    for(int i = 0; i < program.size(); i++)
    {
        // Execute instruction
        std::cout << "Instruction " << i << " / " << program.size() - 1 << " ~ " << inst_type(program[i].type) << std::endl;
        switch(program[i].type)
        {
            case 0:
                std::cout << "Invalid instruction!\n";
            break;
            case 1: // ADD
                ex_ADD(i);
            break;
            case 2: // SUB
                ex_SUB(i);
            break;
            case 3: // MUL
                ex_MUL(i);
            break;
            case 4: // AND
                ex_AND(i);
            break;
            case 5: // ORR
                ex_ORR(i);
            break;
            case 6: // MOV
                ex_MOV(i);
            break;
            case 7: // EOR
                ex_EOR(i);
            break;
            case 9: // LDR
                ex_LDR(i);
            break;
            case 10: // LDRH
                ex_LDRH(i);
            break;
            case 11: // LDRB
                ex_LDRB(i);
            break;
            case 12: // STR
                ex_STR(i);
            break;
            case 13: // STRH
                ex_STRH(i);
            break;
            case 14: // STRB
                ex_STRB(i);
            break;
            case 15: // CMP
                temp = reg[program[i].val0] - reg[program[i].val1];
                z_flag = temp == 0          ? true : false;
                n_flag = temp >= 0x80000000 ? true : false;
                // C and V flags need to be implemented here somehow
            break;
            case 16: // B
                i = program[i].dest_reg - 1;
            break;
            case 17: // BEQ
                if(z_flag)
                {
                    i = program[i].dest_reg - 1;
                }
            break;
            case 18: // BNE
                if(!z_flag)
                {
                    i = program[i].dest_reg - 1;
                }
            break; 
            case 19: // BCS
                if(c_flag)
                {
                    i = program[i].dest_reg - 1;
                }
            break;   
            case 20: // BCC
                if(!c_flag)
                {
                    i = program[i].dest_reg - 1;
                }
            break;  
            case 21: // BMI 
                if(n_flag)
                {
                    i = program[i].dest_reg - 1;
                }
            break;
            case 22: // BPL
                if(!n_flag)
                {
                    i = program[i].dest_reg - 1;
                }
            break;   
            case 23: // BVS
                if(v_flag)
                {
                    i = program[i].dest_reg - 1;
                }
            break;   
            case 24: // BVC
                if(!v_flag)
                {
                    i = program[i].dest_reg - 1;
                }
            break;   
            case 25: // BHI
                if(c_flag && !z_flag)
                {
                    i = program[i].dest_reg - 1;
                }
            break;  
            case 26: // BLS 
                if(!c_flag | z_flag)
                {
                    i = program[i].dest_reg - 1;
                }
            break;  
            case 27: // BGE 
                if((n_flag && v_flag) || (!n_flag && !v_flag))
                {
                    i = program[i].dest_reg - 1;
                }
            break;   
            case 28: // BLT
                if((n_flag && !v_flag) || (!n_flag && v_flag))
                {
                    i = program[i].dest_reg - 1;
                }
            break;   
            case 29: // BGT
                if(z_flag && ((n_flag && v_flag) || (!n_flag && !v_flag)))
                {
                    i = program[i].dest_reg - 1;
                }
            break;  
            case 30: // BLE 
                if(z_flag || (n_flag && v_flag) || (!n_flag && !v_flag))
                {
                    i = program[i].dest_reg - 1;
                }
            break;
            case 31:
                i = program[i].dest_reg - 1;
            break;
            case 32: // LDMIA
                if(program[i].postindex)
                {
                    reg[program[i].dest_reg] = ex_LDM(program[i].val0, reg[program[i].dest_reg], true,true);
                }
                else
                {
                    ex_LDM(program[i].val0, reg[program[i].dest_reg], true, true);
                }
            break;
            case 33: // LDMIB
                if(program[i].postindex)
                {
                    reg[program[i].dest_reg] = ex_LDM(program[i].val0, reg[program[i].dest_reg], true, false);
                }
                else
                {
                    ex_LDM(program[i].val0, reg[program[i].dest_reg], true, false);
                }
            break;
            case 34: // LDMDA
                if(program[i].postindex)
                {
                    reg[program[i].dest_reg] = ex_LDM(program[i].val0, reg[program[i].dest_reg], false, true);
                }
                else
                {
                    ex_LDM(program[i].val0, reg[program[i].dest_reg], false, true);
                }
            break;
            case 35: // LDMDB
                if(program[i].postindex)
                {
                    reg[program[i].dest_reg] = ex_LDM(program[i].val0, reg[program[i].dest_reg], false, false);
                }
                else
                {
                    ex_LDM(program[i].val0, reg[program[i].dest_reg], false, false);
                }
            break;
            case 36: // STMIA
                if(program[i].postindex)
                {
                    reg[program[i].dest_reg] = ex_STM(program[i].val0, reg[program[i].dest_reg], true, true);
                }
                else
                {
                    ex_STM(program[i].val0, reg[program[i].dest_reg], true, true);
                }
            break;
            case 37: // STMIB
                if(program[i].postindex)
                {
                    reg[program[i].dest_reg] = ex_STM(program[i].val0, reg[program[i].dest_reg], true, false);
                }
                else
                {
                    ex_STM(program[i].val0, reg[program[i].dest_reg], true, false);
                }
            break;
            case 38: // STMDA
                if(program[i].postindex)
                {
                    reg[program[i].dest_reg] = ex_STM(program[i].val0, reg[program[i].dest_reg], false, true);
                }
                else
                {
                    ex_STM(program[i].val0, reg[program[i].dest_reg], false, true);
                }
            break;
            case 39: // STMDB
                if(program[i].postindex)
                {
                    reg[program[i].dest_reg] = ex_STM(program[i].val0, reg[program[i].dest_reg], false, false);
                }
                else
                {
                    ex_STM(program[i].val0, reg[program[i].dest_reg], false, false);
                }
            break;
        }

        // Update flags if requested
        if(program[i].flags)
        {
            z_flag = reg[program[i].dest_reg] == 0          ? true : false;
            n_flag = reg[program[i].dest_reg] >= 0x80000000 ? true : false;
            // C and V flags need to be implemented here somehow
        }
    }
}

void Machine::print_registers()
{
    for(int i = 0; i < NUM_REG; i++)
    {
        std::cout << ANSI_GREEN << "R" << std::setw(2) << std::right << std::setfill('0') << std::dec << i << COLOUR_END << std::hex << std::uppercase << " -> 0x" << std::setw(8) << reg[i] << std::endl;
    }
    std::cout << "N=" << n_flag << " Z=" << z_flag << " C=" << c_flag << " V=" << v_flag << std::endl;
}

uint32_t fetch_reg_val(uint8_t reg)
{
    return 0;
}

void Machine::ex_ADD(int inst)
{
    int val;

    if(program[inst].val0reg)
    {
        val = reg[program[inst].val0];
    }
    else
    {
        val = program[inst].val0;
    }

    if(program[inst].val1reg)
    {
        val += reg[program[inst].val1];
    }
    else
    {
        val += program[inst].val1;
    }

    reg[program[inst].dest_reg] = val;
}

void Machine::ex_SUB(int inst)
{
    int val;

    if(program[inst].val0reg)
    {
        val = reg[program[inst].val0];
    }
    else
    {
        val = program[inst].val0;
    }

    if(program[inst].val1reg)
    {
        val -= reg[program[inst].val1];
    }
    else
    {
        val -= program[inst].val1;
    }

    reg[program[inst].dest_reg] = val;
}

void Machine::ex_MUL(int inst)
{
    int val;

    if(program[inst].val0reg)
    {
        val = reg[program[inst].val0];
    }
    else
    {
        val = program[inst].val0;
    }

    if(program[inst].val1reg)
    {
        val *= reg[program[inst].val1];
    }
    else
    {
        val *= program[inst].val1;
    }

    reg[program[inst].dest_reg] = val;
}

void Machine::ex_AND(int inst)
{
    int val;

    if(program[inst].val0reg)
    {
        val = reg[program[inst].val0];
    }
    else
    {
        val = program[inst].val0;
    }

    if(program[inst].val1reg)
    {
        val &= reg[program[inst].val1];
    }
    else
    {
        val &= program[inst].val1;
    }

    reg[program[inst].dest_reg] = val;
}

void Machine::ex_ORR(int inst)
{
    int val;

    if(program[inst].val0reg)
    {
        val = reg[program[inst].val0];
    }
    else
    {
        val = program[inst].val0;
    }

    if(program[inst].val1reg)
    {
        val |= reg[program[inst].val1];
    }
    else
    {
        val |= program[inst].val1;
    }

    reg[program[inst].dest_reg] = val;
}

void Machine::ex_MOV(int inst)
{
    if(program[inst].val0reg)
    {
        reg[program[inst].dest_reg & 0x1F] = shift(reg[program[inst].val0], program[inst].val1, program[inst].dest_reg);
    }
    else
    {
        reg[program[inst].dest_reg & 0x1F] = shift(program[inst].val0, program[inst].val1, program[inst].dest_reg);
    }
}

void Machine::ex_EOR(int inst)
{
    int val;

    if(program[inst].val0reg)
    {
        val = reg[program[inst].val0];
    }
    else
    {
        val = program[inst].val0;
    }

    if(program[inst].val1reg)
    {
        val ^= reg[program[inst].val1];
    }
    else
    {
        val ^= program[inst].val1;
    }

    reg[program[inst].dest_reg] = val;
}

void Machine::ex_MVN(int inst)
{
    if(program[inst].val0reg)
    {
        reg[program[inst].dest_reg] = ~reg[program[inst].val0];
    }
    else
    {
        reg[program[inst].dest_reg] = ~program[inst].val0;
    }
}

void Machine::ex_LDR(int inst)
{
    if(program[inst].postindex)
    {
        reg[program[inst].dest_reg] = load_word(reg[program[inst].val0]);
        reg[program[inst].val0] += program[inst].address ? reg[program[inst].val1] : program[inst].val1;
    }
    else
    {
        if(program[inst].address)
        {
            reg[program[inst].dest_reg] = load_word(reg[program[inst].val0] + reg[program[inst].val1]);
        }
        else
        {
            reg[program[inst].dest_reg] = load_word(reg[program[inst].val0] + program[inst].val1);
        }
    }
}

void Machine::ex_LDRH(int inst)
{
    if(program[inst].postindex)
    {
        reg[program[inst].dest_reg] = load_halfword(reg[program[inst].val0]);
        reg[program[inst].val0] += program[inst].address ? reg[program[inst].val1] : program[inst].val1;
    }
    else
    {
        if(program[inst].address)
        {
            reg[program[inst].dest_reg] = load_halfword(reg[program[inst].val0] + reg[program[inst].val1]);
        }
        else
        {
            reg[program[inst].dest_reg] = load_halfword(reg[program[inst].val0] + program[inst].val1);
        }
    }
}

void Machine::ex_LDRB(int inst)
{
    if(program[inst].postindex)
    {
        reg[program[inst].dest_reg] = load_byte(reg[program[inst].val0]);
        reg[program[inst].val0] += program[inst].address ? reg[program[inst].val1] : program[inst].val1;
    }
    else
    {
        if(program[inst].address)
        {
            reg[program[inst].dest_reg] = load_byte(reg[program[inst].val0] + reg[program[inst].val1]);
        }
        else
        {
            reg[program[inst].dest_reg] = load_byte(reg[program[inst].val0] + program[inst].val1);
        }
    }
}

void Machine::ex_STR(int inst)
{
    if(program[inst].postindex)
    {
        store_word(reg[program[inst].dest_reg], reg[program[inst].val0]);
        reg[program[inst].val0] += program[inst].val1reg ? reg[program[inst].val1] : program[inst].val1;
    }
    else
    {
        if(program[inst].address)
        {
            store_word(reg[program[inst].dest_reg], reg[program[inst].val0] + reg[program[inst].val1]);
        }
        { 
            store_word(reg[program[inst].dest_reg], reg[program[inst].val0] + program[inst].val1);
        }
    }
}

void Machine::ex_STRH(int inst)
{
    if(program[inst].postindex)
    {
        store_halfword(reg[program[inst].dest_reg], reg[program[inst].val0]);
        reg[program[inst].val0] += program[inst].val1reg ? reg[program[inst].val1] : program[inst].val1;
    }
    else
    {
        if(program[inst].address)
        {
            store_halfword(reg[program[inst].dest_reg], reg[program[inst].val0] + reg[program[inst].val1]);
        }
        { 
            store_halfword(reg[program[inst].dest_reg], reg[program[inst].val0] + program[inst].val1);
        }
    }
}

void Machine::ex_STRB(int inst)
{
    if(program[inst].postindex)
    {
        store_byte(reg[program[inst].dest_reg], reg[program[inst].val0]);
        reg[program[inst].val0] += program[inst].val1reg ? reg[program[inst].val1] : program[inst].val1;
    }
    else
    {
        if(program[inst].address)
        {
            store_byte(reg[program[inst].dest_reg], reg[program[inst].val0] + reg[program[inst].val1]);
        }
        { 
            store_byte(reg[program[inst].dest_reg], reg[program[inst].val0] + program[inst].val1);
        }
    }
}

uint8_t Machine::ex_LDM(uint32_t registers, uint8_t address, bool increment, bool after)
{
    for(int i = NUM_REG - 1; i >= 0; i--)
    {
        if((registers >> i) & 0x00000001)
        {
            !after ? increment ? address += 4 : address -= 4 : 0;
            reg[i] = load_word(address);
            after ? increment ? address += 4 : address -= 4 : 0;
        }
    }
    return address;
}

uint8_t Machine::ex_STM(uint32_t registers, uint8_t address, bool increment, bool after)
{
    for(int i = 0; i < NUM_REG; i++)
    {
        if((registers >> i) & 0x00000001)
        {
            !after ? increment ? address += 4 : address -= 4 : 0;
            store_word(reg[i], address);
            after ? increment ? address += 4 : address -= 4 : 0;
        }
    }
    return address;
}

inline uint32_t Machine::shift(uint32_t val0, uint32_t val1, uint8_t type)
{
    uint32_t temp;
    switch(type & 0xE0)
    {
        case 0x00: // No Shift
            return val0;
        break;
        case 0x20: // LSR
            return val0 >> val1;
        break;
        case 0x40: // ASR
            // This needs to be fixed
            return val0 >> val1;
        break;
        case 0x60: // ROR
            for(int i = 0; i < val1; i++)
            {
                temp = val0 & 0x00000001;
                temp <<= 31;
                val0 >>= 1;
                val0 |= temp;
            }
        break;
        case 0x80: // LSL
            return val0 << val1;
        break;
        case 0xA0: // ASL
            // This needs to be fixed
            return val0 << val1;
        break;
        case 0xC0: // ROL
            for(int i = 0; i < val1; i++)
            {
                temp = val0 & 0x80000000;
                temp >>= 31;
                val0 <<= 1;
                val0 |= temp;
            }
        break;
        case 0xE0: // No Shift
            return val0;
        break;
    }

    return val0;
}

int Machine::return_label(int inst)
{
    return 0;
}

void Machine::print_instructions()
{
    for(int i = 0; i < program.size(); i++)
    {
        std::cout << std::hex << std::uppercase << ANSI_BLUE << inst_type(program[i].type) 
            << COLOUR_END << ": Rd = 0x" << (int)program[i].dest_reg << ", Rn = 0x" 
            << program[i].val0 << ", Rm = 0x" << program[i].val1 << "\nval0reg = " 
            << program[i].val0reg << ", val1reg = " << program[i].val1reg << ", flags = "
            << program[i].flags << ", address = " << program[i].address << ", postindex = " 
            << program[i].postindex << "\n\n";
    }
}