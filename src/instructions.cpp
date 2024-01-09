
#include "instructions.hpp"

#include <sstream>
#include <stdexcept>

#include "machine.hpp"
#include "memory.hpp"
#include "registers.hpp"

static const char* names[63] = {
    "LDA", "LDX", "LDL", "STA", "STX", "STL", "ADD", "SUB", "MUL", "DIV",
    "COMP", "TIX", "JEQ", "JGT", "JLT", "J", "AND", "OR", "JSUB", "RSUB",
    "LDCH", "STCH", "ADDF", "SUBF", "MULF", "DIVF", "LDB", "LDS", "LDF", "LDT",
    "STB", "STS", "STF", "STT", "COMPF", "null", "ADDR", "SUBR", "MULR", "DIVR",
    "COMPR", "SHIFTL", "SHIFTR", "RMO", "SVC", "CLEAR", "TIXR", "null", "FLOAT", "FIX",
    "NORM", "null", "LPS", "STI", "RD", "WD", "TD", "null", "STSW", "SSK",
    "SIO", "HIO", "TIO"};

instruction::instruction(int firstByte) : op1(-1), op2(-1) {
    this->opcode = ((firstByte & 0xFC) >> 2);
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

std::string instruction::toString() {
    std::stringstream str;
    switch (getFormat()) {
        case format::F1:
            str << "F1: ";
            break;
        case format::F2:
            str << "F2: ";
            break;
        case format::SIC:
            str << "SIC: ";
            break;
        case format::F3F4:
            str << (isExtended() ? "F4: " : "F3: ");
            break;
    }
    str << names[opcode]
        << "|"
        << op1 << "_0x" << std::hex << op1
        << "|";
    if (op2 != -1) {
        str << ((uint)op2 & 0xFFFFFF) << "_0x" << std::hex << ((uint)op2 & 0xFFFFFF);
    }
    return str.str();
}
