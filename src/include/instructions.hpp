#ifndef TK_INSTRUCTIONS_HPP
#define TK_INSTRUCTIONS_HPP

#include "registers.hpp"
#include <string>

enum class format {
    F1,
    F2,
    SIC,
    F3F4,
};

enum class NI_state {
    immediate = 1,
    indirect = 2,
    simple = 3,
    SIC = 0,
};

enum class BP_state {
    indirect = 0,
    pc = 1,
    base = 2,
};

// utility class for instructions
struct instruction {
    int flags;
    int opcode;
    int op1;  // if address/offset then this one is set
    int op2;

    instruction(int firstByte);
    format getFormat();
    bool isExtended();
    bool isIndexed();
    NI_state getNI();
    BP_state getBP();

    std::string toString();
};

#endif  // TK_INSTRUCTIONS_HPP