#ifndef TK_INSTRUCTIONS_HPP
#define TK_INSTRUCTIONS_HPP
#include "machine.hpp"
#include "memory.hpp"
#include "registers.hpp"

enum format {
    F1,
    F2,
    SIC,
    F3F4,
};

enum NI_state {
    indirect = 1,
    direct = 2,
    simple = 3,
    SIC = 4,
};

enum BP_state {
    indirect = 0,
    pc = 1,
    base = 2,
};

//utility class for instructions
struct instruction {
    int flags;
    int opcode;
    int op1;  // if address/offset then this one is set
    int op2;

    instruction(int firstByte);
    format getFormat();
    int getFlags();
    bool isExtended();
    bool isIndexed();
    NI_state getNI();
    BP_state getBP();
};

int handle_opcode(machine& m, instruction& instr);

#endif  // TK_INSTRUCTIONS_HPP