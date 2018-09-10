// (c) Daniel Desmond Dennis 2018

#include "instructions.hpp"

int Instructions::start()
{
    FILE* file;
    uint16_t inst;
    uint64_t val64;
    unsigned int offset = 0;
    unsigned int curr_instruction = 0; // Used for labels
    unsigned int curr_line = 1; // Used for error reporting
    char str[LINE_LEN], token[LINE_LEN];
    bool found_label;
    struct label_struct temp_label;
    struct instruction temp_line;

    file = fopen(FILE_NAME, "r");
    if(!readline(str, LINE_LEN, file)) return 0;

    while(!feof(file))
    {
        offset = 0;
        if(str[offset] <= ' ') // If not a label
        {
            // Read instruction type
            while(str[++offset] <= ' ');
            inst = instruction_type(offset, str, curr_line);
            offset = (inst & 0x0F00) >> 8;
            temp_line.flags = inst & 0x1000 ? true : false;
            temp_line.type = inst & 0x00FF;

            if(inst & 0x2000) // Branch instruction
            {
                strcpy(token, str + offset);
                for(int i = 0; i < LINE_LEN; i++)
                {
                    if(token[i] == ' ')
                    {
                        temp_line.label = strtok(token, " ");
                        break;
                    }
                    else if(token[i] == '\n')
                    {
                        temp_line.label = strtok(token, "\n");
                        break;
                    }
                }
                temp_line.line = curr_line;
            }
            else // Not a branch instruction
            {
                // Read Rd value
                val64               =  val_type(offset, str, curr_line);
                offset             += (val64 & 0x000000F000000000) >> 36;
                temp_line.dest_reg  =  val64 & 0x00000000FFFFFFFF;
                temp_line.postindex =  val64 & 0x0000020000000000  ? true : false;

                if(*(str + offset))
                {
                    // Read Rm value
                    val64                =  val_type(offset, str, curr_line);
                    offset              += (val64 & 0x000000F000000000) >> 36;
                    temp_line.val0reg    =  val64 & 0x0000000100000000  ? true : false;
                    temp_line.val0       =  val64 & 0x00000000FFFFFFFF;
                    temp_line.address    =  val64 & 0x0000010000000000  ? true : false;

                    if(*(str + offset) && (val64 & 0x000000F000000000))
                    {
                        // Read Rn value
                        val64                =  val_type(offset, str, curr_line);
                        temp_line.val1reg    =  val64 & 0x0000000100000000  ? true : false;
                        temp_line.val1       =  val64 & 0x00000000FFFFFFFF;
                        temp_line.dest_reg  |= (val64 & 0x0000000E00000000) >> 28; // Add shift operation if specified
                    }
                }
            }
            program.push_back(temp_line);
            curr_instruction++;
            if(!readline(str, LINE_LEN, file)) return fclose(file);
        }
        else // A label
        {
            strcpy(token, str);
            for(int i = 0; i < LINE_LEN; i++)
            {
                if(str[i] == ' ')
                {
                    temp_label.label = strtok(token, " ");
                    offset = i;
                    break;
                }
                else if(str[i] == '\n')
                {
                    temp_label.label = strtok(token, "\n");
                    offset = i;
                    break;
                }
            }
            // while(str[offset + 1])
            // {
            //     if(str[offset]) <= ' ')
            //         offset++;
            //     else
            //         break;
            // }
            temp_label.instruction = curr_instruction;
            labels.push_back(temp_label);

            if(!readline(str, LINE_LEN, file)) return fclose(file);
        }
        curr_line++;
    }

    // Populate labels
    for(int i = 0; i < program.size(); i++)
    {
        if((program[i].type & 0x0010) && !(program[i].type & 0xFFE0))
        {
            found_label = false;
            for(int j = 0; j < labels.size(); j++)
            {
                if(labels[j].label == program[i].label)
                {
                    program[i].dest_reg = labels[j].instruction;
                    found_label = true;
                }
            }
            if(!found_label)
            {
                report(program[i].line, 3); // Cannot find label
            }
        }
    }

    labels.clear();

    return fclose(file);
}

uint16_t Instructions::instruction_type(int start, char* argv, unsigned int curr_line)
{
    /*
        Return format is as follows (16 bit)
        0x00FF 8 bit instruction type
        0x0F00 4 bit offset
        0x1000 Boolean to set flags
        0x2000 Boolean for branch instruction
        0xC000 [Unused]
    */
    
    uint16_t val = 0; // Value returned by function
    
    switch(argv[start++])
    {
        case 'A':
            switch(argv[start++])
            {
                case 'D':
                    switch(argv[start++])
                    {
                        case 'D':
                            val = 1;
                        break;
                    }
                break;
                case 'N':
                    switch(argv[start++])
                    {
                        case 'D':
                            val = 4;
                        break;
                    }
                break;
            }
        break;
        case 'B':
            switch(argv[start++])
            {
                case 'A':
                    switch(argv[start++])
                    {
                        case 'L':
                            val = 0x2010;
                        break;
                    }
                break;
                case 'C':
                    switch(argv[start++])
                    {
                        case 'C':
                            val = 0x2014;
                        break;
                        case 'S':
                            val = 0x2013;
                        break;
                    }
                break;
                case 'E':
                    switch(argv[start++])
                    {
                        case 'Q':
                            val = 0x2011;
                        break;
                    }
                break;
                case 'G':
                    switch(argv[start++])
                    {
                        case 'E':
                            val = 0x201B;
                        break;
                        case 'T':
                            val = 0x201D;
                        break;
                    }
                break;
                case 'H':
                    switch(argv[start++])
                    {
                        case 'I':
                            val = 0x2019;
                        break;
                        case 'S':
                            val = 0x2013;
                        break;
                    }
                break;
                case 'L':
                    switch(argv[start++])
                    {
                        case 'E':
                            val = 0x201E;
                        break;
                        case 'S':
                            val = 0x201A;
                        break;
                        case 'L':
                            val = 0x201C;
                        break;
                        case 'O':
                            val = 0x2014;
                        break;
                    }
                break;
                case 'N':
                    switch(argv[start++])
                    {
                        case 'E':
                            val = 0x2012;
                        break;
                    }
                break;
                case 'M':
                    switch(argv[start++])
                    {
                        case 'I':
                            val = 0x2015;
                        break;
                    }
                break;
                case 'P':
                    switch(argv[start++])
                    {
                        case 'L':
                            val = 0x2016;
                        break;
                    }
                break;
                case 'V':
                    switch(argv[start++])
                    {
                        case 'C':
                            val = 0x2018;
                        break;
                        case 'S':
                            val = 0x2017;
                        break;
                    }
                break;
                default:
                    val = 0x2010;
                    start--;
                break;
            }
        break;
        case 'C':
            switch(argv[start++])
            {
                case 'M':
                    switch(argv[start++])
                    {
                        case 'P':
                            val = 15;
                        break;
                    }
                break;
            }
        break;
        case 'E':
            switch(argv[start++])
            {
                case 'O':
                    switch(argv[start++])
                    {
                        case 'R':
                            val = 7;
                        break;
                    }
                break;
            }
        break;
        case 'L':
            switch(argv[start++])
            {
                case 'D':
                    switch(argv[start++])
                    {
                        case 'M':
                            switch(argv[start++])
                            {
                                case 'D':
                                    switch(argv[start++])
                                    {
                                        case 'A':
                                            val = 34;
                                        break;
                                        case 'B':
                                            val = 35;
                                        break;
                                    }
                                break;
                                case 'E':
                                    switch(argv[start++])
                                    {
                                        case 'A':
                                            val = 35;
                                        break;
                                        case 'D':
                                            val = 33;
                                        break;
                                    }
                                break;
                                case 'F':
                                    switch(argv[start++])
                                    {
                                        case 'A':
                                            val = 34;
                                        break;
                                        case 'D':
                                            val = 32;
                                        break;
                                    }
                                break;
                                case 'I':
                                    switch(argv[start++])
                                    {
                                        case 'A':
                                            val = 32;
                                        break;
                                        case 'B':
                                            val = 33;
                                        break;
                                    }
                                break;
                                default:
                                    val = 40;
                                    start--;
                                break;
                            }
                        break;
                        case 'R':
                            switch(argv[start++])
                            {
                                case 'B':
                                    val = 11;
                                break;
                                case 'H':
                                    val = 10;
                                break;
                                default:
                                    start--;
                                    val = 9;
                                break;
                            }
                        break;
                    }
                break;
            }
        break;
        case 'M':
            switch(argv[start++])
            {
                case 'O':
                    switch(argv[start++])
                    {
                        case 'V':
                            val = 6;
                        break;
                    }
                break;
                case 'U':
                    switch(argv[start++])
                    {
                        case 'L':
                            val = 3;
                        break;
                    }
                break;
                case 'V':
                    switch(argv[start++])
                    {
                        case 'N':
                            val = 8;
                        break;
                    }
                break;
            }
        break;
        case 'O':
            switch(argv[start++])
            {
                switch(argv[start++])
                {
                    case 'R':
                        switch(argv[start++])
                        {
                            case 'R':
                                val = 5;
                            break;
                        }
                    break;
                }
            }
        break;
        case 'S':
            switch(argv[start++])
            {
                case 'T':
                    switch(argv[start++])
                    {
                        case 'M':
                            switch(argv[start++])
                            {
                                case 'D':
                                    switch(argv[start++])
                                    {
                                        case 'A':
                                            val = 38;
                                        break;
                                        case 'B':
                                            val = 39;
                                        break;
                                    }
                                break;
                                case 'E':
                                    switch(argv[start++])
                                    {
                                        case 'A':
                                            val = 36;
                                        break;
                                        case 'D':
                                            val = 38;
                                        break;
                                    }
                                break;
                                case 'F':
                                    switch(argv[start++])
                                    {
                                        case 'A':
                                            val = 37;
                                        break;
                                        case 'D':
                                            val = 39;
                                        break;
                                    }
                                break;
                                case 'I':
                                    switch(argv[start++])
                                    {
                                        case 'A':
                                            val = 36;
                                        break;
                                        case 'B':
                                            val = 37;
                                        break;
                                    }
                                break;
                                default:
                                    val = 41;
                                    start--;
                                break;
                            }
                        break;
                        case 'R':
                            switch(argv[start++])
                            {
                                case 'B':
                                    val = 14;
                                break;
                                case 'H':
                                    val = 13;
                                break;
                                default:
                                    start--;
                                    val = 12;
                                break;
                            }
                        break;
                    }
                break;
                case 'U':
                    switch(argv[start++])
                    {
                        case 'B':
                            val = 2;
                        break;
                    }
                break;
            }
        break;
    }
    
    if(argv[start] == 'S')
    {
        val |= 0x1000;
        start++;
    }

    if(!val) report(curr_line, 1); // Unknown Instruction
    while(argv[++start] <= ' '); // Calculate offset
    return start < 0x10 ? start << 8 | val : 0;
}

uint64_t Instructions::val_type(int start, char* argv, unsigned int curr_line)
{
    /*
        Return format is as follows (64 bit)
        0x00000000FFFFFFFF 32 bit number (register, or hard-coded value)
        0x0000000100000000 Boolean to say if register or not
        0x0000000E00000000 Type of bitwise shift operation, if any
            000 No Shift
            001 LSR
            010 ASR
            011 ROR
            100 LSL
            101 ASL
            110 ROL
            111 [Unused]
        0x000000F000000000 4 bit offset for next number, 0 for end
        0x0000010000000000 Boolean to say if address or not
        0x0000020000000000 Boolean to say if postindex or not / update sp pointer
        0xFFFFFC0000000000 [Unused]
    */

    char* space;
    char* comma;
    int64_t offset;
    uint64_t val= 0;
    int num0, num1;
    char str[LINE_LEN];
    strcpy(str, argv);
    char* s = &str[start]; // Make this better

    if((str[start] == 'R') && (str[start + 1] >= '0') && (str[start + 1] <= '9'))
    // Handling register value
    {
        val = atoi(strtok(&str[start + 1], ","));
        val = val < NUM_REG ? val | 0x0000000100000000 : 0;

        if(!val)
        {
            report(curr_line, 2); // Invalid register
        }
    }
    else if(!memcmp(str + start, "SP", 2))
    // Handling stack pointer
    {
        val = 0x000000010000000D;
        if(str[start + 2] == '!')
        {
            val |= 0x0000020000000000;
        }
    }
    else if((str[start] >= 'A') && (str[start] <= 'R'))
    // Handling shift operation, see table above
    {
        switch(str[start++])
        {
            case 'A':
                switch(str[start++])
                {
                    case 'S':
                        switch(str[start++])
                        {
                            case 'L':
                                val = str[++start] == '#' ? 0xA00000000 | (atoi(strtok(&str[++start], " ")) & 0x1F) : 0;
                            break;
                            case 'R':
                                val = str[++start] == '#' ? 0x400000000 | (atoi(strtok(&str[++start], " ")) & 0x1F) : 0;
                            break;
                        }
                    break;
                } 
            break;
            case 'L':
                switch(str[start++])
                {
                    case 'S':
                        switch(str[start++])
                        {
                            case 'L':
                                val = str[++start] == '#' ? 0x800000000 | (atoi(strtok(&str[++start], " ")) & 0x1F) : 0;
                            break;
                            case 'R':
                                val = str[++start] == '#' ? 0x200000000 | (atoi(strtok(&str[++start], " ")) & 0x1F) : 0;
                            break;
                        }
                    break;
                }
            break;
            case 'R':
                switch(str[start++])
                {
                    case 'O':
                        switch(str[start++])
                        {
                            case 'L':
                                val = str[++start] == '#' ? 0xC00000000 | (atoi(strtok(&str[++start], " ")) & 0x1F) : 0;
                            break;
                            case 'R':
                                val = str[++start] == '#' ? 0x600000000 | (atoi(strtok(&str[++start], " ")) & 0x1F) : 0;
                            break;
                        }
                    break;
                }
            break;
        }
    }
    else if((str[start] == '[') && (str[start + 1] == 'R')) // Use memcmp instead
    // Handle address value []
    {
        for(int i = start; i < LINE_LEN - start + 2; i++)
        {
            if(str[i] == ',')
            // LDR Rd, [Rm, #x]
            {
                val = 0;
                val |= (atoi(strtok(&str[start + 2], ",")) & 0x00000000FFFFFFFF) | 0x0000010100000000;
                break;
            }
            else if(str[i] == ']')
            // LDR Rd, [Rm], Rn
            {
                val = 0x0000020000000000; // postindex = true
                val |= (atoi(strtok(&str[start + 2], "]")) & 0x00000000FFFFFFFF) | 0x0000010100000000;
                break;
            }
        }
    }
    else if(str[start] == '{')
    // Stack operations {}
    {
        num0 = num1 = -1;
        while(*s)
        {
           if((*s >= '0') && (*s <= '9'))
           {
               if(num0 == -1)
               {
                   num0 = atoi(s);
               }
               else if(num1 == -1)
               {
                   num1 = atoi(s);
               }
               else
               {
                   puts("Imporve this error message!");
               }
               while((*s >= '0') && (*s <= '9'))
               {
                   s++;
               }
           }
           else if(*s == ',')
           {
               do
               {
                   val |= 1 << num0; // Remove function
               }while(++num0 <= num1);
               num0 = num1 = -1;
               s++;
           }
           else if(*s == '}')
           {
               do
               {
                   val |= 1 << num0; // Remove function
                   num0++;
               }while(num0 <= num1);
               break;
           }
           else
           {
               s++;
           }
        }

        // use argv instead to remove this return requirement
        return val;
    }
    else if(str[start] == '#')
    // Handling hardcoded value
    {
        val = strtol(strtok(&str[start + 1], " "), NULL, 0) & 0x00000000FFFFFFFF;
    }

    // Calculate offset
    space = strchr(&argv[start], ' ');
    comma = strchr(&argv[start], ',');

    if(space && comma)
    {
        if(space > comma)
        {
            if(comma[1] == ' ')
            {
                offset = -1;
                while(space[++offset] == ' ');
                space += offset;
                offset = space - &argv[start];
            }
            else
            {
                offset = ++comma - &argv[start];
            }
        }
        else if(comma > space)
        {
            offset = 0;
            while(comma[++offset] == ' ');
            comma += --offset;
            offset = ++comma - &argv[start];
        }
    }
    else if(comma)
    {
        offset = ++comma - &argv[start];
    }
    else
    {
        offset = 0;                
    }
    
    val = offset < 0x10 ? val | (offset << 36) : 0;

    return val;
}

void Instructions::preprocess(char* str)
{
    do // Ensure everything is upperccase
    {
        if((*str > 0x60) && (*str < 0x7B))
        {
            *str -= 0x20;
        }
    }while(*++str);
}

bool Instructions::readline(char* str, int lim, FILE* file)
{
    if(fgets(str, lim, file))
    {
        preprocess(str);
        return true;
    }
    else return false;
}

const char* Instructions::inst_type(uint8_t type)
{
    switch(type)
    {
        case 0:   return "N/A";
        case 1:   return "ADD";
        case 2:   return "SUB";
        case 3:   return "MUL";
        case 4:   return "AND";
        case 5:   return "ORR";
        case 6:   return "MOV";
        case 7:   return "EOR";
        case 8:   return "MVN";
        case 9:   return "LDR";
        case 10:  return "LDRH";
        case 11:  return "LDRB";
        case 12:  return "STR";
        case 13:  return "STRH";
        case 14:  return "STRB";
        case 15:  return "CMP";
        case 16:  return "B";
        case 17:  return "BEG";
        case 18:  return "BNE";
        case 19:  return "BCS";
        case 20:  return "BCC";
        case 21:  return "BMI";
        case 22:  return "BPL";
        case 23:  return "BVS";
        case 24:  return "BVC";
        case 25:  return "BHI";
        case 26:  return "BLS";
        case 27:  return "BGE";
        case 28:  return "BLT";
        case 29:  return "BGT";
        case 30:  return "BLE";
        case 31:  return "BAL";
        case 32:  return "LDMIA";
        case 33:  return "LDMIB";   
        case 34:  return "LDMDA";   
        case 35:  return "LDMDB"; 
        case 36:  return "STMIA";  
        case 37:  return "STMIB";  
        case 38:  return "STMDA";
        case 39:  return "STMDB";
        case 40:  return "LDM";
        case 41:  return "STM";
        default:  return "Error";
    }
}