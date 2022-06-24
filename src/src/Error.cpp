#pragma once

#include "../include/Error.hpp"
#include <fstream>

std::ofstream Error::__ErrFile("ErrorLog.txt");

void Error::ThrowError(std::string err)
{
    Error::__ErrFile.write(err.c_str(), err.size()) ;
    abort();
}
