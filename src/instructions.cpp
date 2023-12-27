#include "instructions.hpp"

#include <stdexcept>

typedef void (*instruction_handler)(machine&, memory&, registers&, instruction&);

// the ones that don't exist are NULL
instruction_handler handlers[63] = {};

instruction::instruction(int firstByte) : op1(-1), op2(-1) {
    this->opcode = (firstByte & 0xFC) >> 2;
    flags = (firstByte & 0x3) << 4;
}

format instruction::getFormat() {
    switch (opcode) {
        case 48:
        case 49:
        case 50:
        case 60:
        case 61:
        case 62:
            return format::F1;
    }

    if (opcode >= 36 && opcode <= 46)
        return format::F2;

    if (getNI() == NI_state::SIC) {
        return format::SIC;
    }
    return format::F3F4;
}

int instruction::getFlags() {
    return flags;
}

bool instruction::isExtended() {
    return flags & 0x1;
}

bool instruction::isIndexed() {
    return flags & 0x8;
}

NI_state instruction::getNI() {
    return static_cast<NI_state>((flags >> 4) & 0x3);
}

BP_state instruction::getBP() {
    return static_cast<BP_state>((flags >> 1) & 0x3);
}

int handle_opcode(machine& m, instruction& instr) {
}
