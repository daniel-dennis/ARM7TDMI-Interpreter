// (c) Daniel Desmond Dennis 2018

#ifndef ERRORHANDLER_HPP
#define ERRORHANDLER_HPP

#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

class Error_Handler
{
private:
    struct errors
    {
        std::string message;
        int line;
        int error_num;
    };

    std::vector<struct errors> error_list;
    const char* error_string(int error_num);

protected:
    void report(int line, int error_num);
    bool print_errors();
};

#endif