#include "machine.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>

#include "instruction-handlers.hpp"
#include "instructions.hpp"

machine::machine(memory* mem, program_info& prog) : mem(mem) {
    // regs.setReg(PC, prog.entryPoint);
    regs[PC] = prog.entryPoint;  // TODO: possible failure point
    devices[0] = new std::fstream("/dev/stdin", std::ios::in);
    devices[1] = new std::fstream("/dev/stdout", std::ios::out);
    devices[2] = new std::fstream("/dev/stderr", std::ios::out);
}

machine::~machine() {
    stop();
    delete mem;
    for (unsigned long i = 0; i < sizeof(devices); i++) {
        delete devices[i];
    }
}

memory& machine::getMemory() {
    return *mem;
}

registers& machine::getRegisters() {
    return regs;
}

instruction* machine::fetch() {
    int newByte = mem->getByte(regs[PC]++);
    instruction* instr = new instruction(newByte);
    switch (instr->getFormat()) {
        case format::F1:
            // done
            break;
        case format::F2:
            newByte = mem->getByte(regs[PC]++);
            instr->op1 = (newByte & 0xF0) >> 4;
            instr->op2 = newByte & 0xF;
            break;
        case format::SIC:
            newByte = mem->getByte(regs[PC]++);
            instr->flags |= (newByte & 0x80) >> 4;
            instr->op1 = (newByte & 0x7F) << 8;
            newByte = mem->getByte(regs[PC]++);
            instr->op1 |= newByte;
            break;
        case format::F3F4:
            newByte = mem->getByte(regs[PC]++);
            instr->flags |= (newByte & 0xF0) >> 4;
            instr->op1 = (newByte & 0xF) << 8;
            newByte = mem->getByte(regs[PC]++);
            instr->op1 |= newByte;
            if (instr->isExtended()) {
                newByte = mem->getByte(regs[PC]++);
                instr->op1 = (instr->op1 << 8) | newByte;
            }
            /*if (instr.isIndexed()) {
                newByte = mem->getByte(regs[PC]++);
                instr.op2 = newByte;
            }*/
            break;
        default:
            throw std::runtime_error("Invalid instruction format");
    }
    return instr;
}

void machine::run() {
}

void machine::stop() {
    running = false;
}

bool machine::ioAvailable() {
    return _ioAvailable;
}

void machine::setIOAvailable(bool ioAvailable) {
    this->_ioAvailable = ioAvailable;
}

std::fstream& machine::getDevice(int dev) {
    std::fstream* device = devices[dev];
    if (device == nullptr) {
        // lazy init device
        std::stringstream ss;
        ss << std::hex << dev << ".dev";
        device = new std::fstream(std::string(ss.str()), std::ios::out | std::ios::in);
        devices[dev] = device;
    }
    return *device;
}
