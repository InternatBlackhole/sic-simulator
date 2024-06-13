#include "machine.hpp"

#include <unistd.h>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "instruction-handlers.hpp"
#include "instructions.hpp"

machine::machine(memory& mem, program_info& prog) : mem(mem), _ioAvailable(true), freq(100) {
    regs[PC] = prog.entryPoint;  // TODO: possible failure point
    devices[0] = std::fopen("/proc/self/fd/0", "r");
    devices[1] = std::fopen("/proc/self/fd/1", "w");
    devices[2] = std::fopen("/proc/self/fd/2", "w");
    status = machine_status::ready;
}

machine::~machine() {
    halt();
    // mem is not in my control
    // delete mem;

    for (unsigned long i = 0; i < (sizeof(devices) / sizeof(devices[0])); i++) {
        if (devices[i] != nullptr) {
            std::fflush(devices[i]);
            std::fclose(devices[i]);
        }
    }
}

memory& machine::getMemory() {
    return mem;
}

registers& machine::getRegisters() {
    return regs;
}

instruction* machine::fetch() {
    int newByte = mem.getByte(regs[PC]++);
    instruction* instr = new instruction(newByte);
    uint op1 = 0, op2 = 0;
    switch (instr->getFormat()) {
        case format::F1:
            // done
            break;
        case format::F2:
            newByte = mem.getByte(regs[PC]++);
            // these two should never be negative
            op1 = (newByte & 0xF0) >> 4;
            op2 = newByte & 0xF;
            break;
        case format::SIC:
            newByte = mem.getByte(regs[PC]++);
            instr->flags |= (newByte & 0x80) >> 4;
            // shift these 4 bits all the way up so that they will be correctly expanded
            op1 = (newByte & 0x7F) << 8;
            newByte = mem.getByte(regs[PC]++);
            op1 |= newByte;
            op1 = op1 & 0x4000 ? op1 | 0xffff8000 : op1;
            break;
        case format::F3F4:
            newByte = mem.getByte(regs[PC]++);
            instr->flags |= (newByte & 0xF0) >> 4;
            op1 = (newByte & 0xF) << 8;
            newByte = mem.getByte(regs[PC]++);
            op1 |= newByte;
            if (instr->isExtended()) {
                newByte = mem.getByte(regs[PC]++);
                op1 = (op1 << 8) | newByte;
                op1 = op1 & 0x800000 ? op1 | 0xff000000 : op1;
            } else {
                op1 = op1 & 0x800 ? op1 | 0xfffff000 : op1;
            }
            break;
        default:
            throw std::runtime_error("Invalid instruction format");
    }

    instr->op1 = op1;
    instr->op2 = op2;
    return instr;
}

bool machine::precheck() {
    /*if (status != machine_status::ready) {
        throw std::runtime_error("Machine not ready!");
    }*/

    if (status == machine_status::halted) {
        throw std::runtime_error("Machine halted!");
    }

    return true;
}

inline void machine::_step(bool print) {
    instruction* instr = fetch();
    if (print)
        std::cout << "Fetched instruction: " << instr->toString() << std::endl;
    exec_instruction(*this, *instr);
    delete instr;
}

inline void machine::_status_report() {
    switch (status) {
        case machine_status::halted:
            std::cout << "Machine halted" << std::endl;
            break;
        case machine_status::paused:
            std::cout << "Machine paused" << std::endl;
            break;
        case machine_status::running:
            std::cout << "Machine running" << std::endl;
            break;
        case machine_status::ready:
            std::cout << "Machine ready" << std::endl;
            break;
        default:
            std::cout << "Unknown status" << std::endl;
            break;
    }
}

void machine::run() {
    precheck();

    status = machine_status::running;
    std::cout << "Machine running with frequency " << this->freq << "Hz" << std::endl;
    if (freq == 0) {
        while (status == machine_status::running) {
            _step(false);
        }
    } else {
        while (status == machine_status::running) {
            _step(true);
            usleep(1000000 / freq);
        }
    }

    _status_report();
}

void machine::step() {
    precheck();

    status = machine_status::running;

    _step(true);
    _status_report();
}

void machine::setFrequency(int freq) {
    if (freq < 0) {
        throw std::runtime_error("Frequency must be bigger than 0");
    }
    this->freq = freq;
}

int machine::getFrequency() {
    return freq;
}

void machine::halt() {
    status = machine_status::halted;
}

void machine::pause() {
    if (status == machine_status::running) {
        status = machine_status::paused;
    } else {
        throw std::runtime_error("Cannot pause machine when not running");
    }
}

bool machine::ioAvailable() {
    return _ioAvailable;
}

void machine::setIOAvailable(bool ioAvailable) {
    this->_ioAvailable = ioAvailable;
}

FILE* machine::getDevice(int dev) {
    FILE* device = devices[dev];
    if (device == nullptr) {
        // lazy init device
        std::stringstream ss;
        ss << std::hex << dev << ".dev";
        device = std::fopen(ss.str().c_str(), "w+");
        devices[dev] = device;
    }
    return device;
}
