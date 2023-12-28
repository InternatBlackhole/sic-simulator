
#include "instructions.hpp"

#include <stdexcept>

#include "machine.hpp"
#include "memory.hpp"
#include "registers.hpp"

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

// checks the e flag
bool instruction::isExtended() {
    return flags & 0x1;
}

// checks the x flag
bool instruction::isIndexed() {
    return flags & 0x8;
}

NI_state instruction::getNI() {
    return static_cast<NI_state>((flags >> 4) & 0x3);
}

BP_state instruction::getBP() {
    return static_cast<BP_state>((flags >> 1) & 0x3);
}
