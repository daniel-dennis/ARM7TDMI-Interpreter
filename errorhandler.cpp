// (c) Daniel Desmond Dennis 2018

#include "errorhandler.hpp"

const char* Error_Handler::error_string(int error_num)
{
    switch(error_num)
    {
        case 1:
            return "Unknown instruction";
        case 2:
            return "Invalid register";
        case 3:
            return "Cannot find label";
        default:
            return "Unknown error";
    }
}

void Error_Handler::report(int line, int error_num)
{
    struct errors temp;
    temp.message = error_string(error_num);
    temp.line = line;
    temp.error_num = error_num;
    error_list.push_back(temp);
}

bool Error_Handler::print_errors()
{
    if(error_list.size())
    {
        for(int i = 0; i < error_list.size(); i++)
        {
            std::cout << "Error: " << std::setw(25) << std::left << error_list[i].message << "Line " << error_list[i].line << std::endl;
        }
        return true;
    }
    else
    {
        return false;
    }
}