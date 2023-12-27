#ifndef TK_LOADER_HPP
#define TK_LOADER_HPP
#include "memory.hpp"
#include <string>

struct program_info {
    std::string name;
    int entryPoint;  // Address of the first instruction to execute
    int loadAddress;  // Address of the first byte of the program
    int codeSize;  // Size of the program in bytes
};

void loadProgram(std::string filename, memory& mem, program_info& prog);

#endif  // TK_LOADER_HPP