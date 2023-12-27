#include "machine.hpp"
#include "instructions.hpp"

machine::machine(memory* mem, program_info& prog) : mem(mem) {
    // regs.setReg(PC, prog.entryPoint);
    regs[PC] = prog.entryPoint;  // TODO: possible failure point
}

machine::~machine() {
    // TODO: stop machine, delete memory and registers
}

memory& machine::getMemory() {
    return *mem;
}

registers& machine::getRegisters() {
    return regs;
}

void machine::run() {
}

