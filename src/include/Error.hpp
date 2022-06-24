#pragma once

#include <fstream>

class Error
{
private:
    static std::ofstream __ErrFile; 
public:
    static void ThrowError(std::string err);
};
