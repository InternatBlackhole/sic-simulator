#include "machine.hpp"

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "instruction-handlers.hpp"
#include "instructions.hpp"

machine::machine(memory& mem, program_info& prog) : mem(mem), _ioAvailable(true), freq(100) {
    regs[PC] = prog.entryPoint;  // TODO: possible failure point
    devices[0] = new std::fstream("/proc/self/fd/0", std::ios::in);
    devices[1] = new std::fstream("/proc/self/fd/1", std::ios::out);
    devices[2] = new std::fstream("/proc/self/fd/2", std::ios::out);
    status = machine_status::ready;
}

machine::~machine() {
    halt();
    // mem is not in my control
    // delete mem;

    for (unsigned long i = 0; i < (sizeof(devices) / sizeof(devices[0])); i++) {
        if (devices[i] != nullptr) {
            devices[i]->close();
            delete devices[i];
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
    uint op1 = 0, op2 = 0;
    instruction* instr = new instruction(newByte);
    switch (instr->getFormat()) {
        case format::F1:
            // done
            break;
        case format::F2:
            newByte = mem.getByte(regs[PC]++);
            // these two should never be negative
            instr->op1 = (newByte & 0xF0) >> 4;
            instr->op2 = newByte & 0xF;
            break;
        case format::SIC:
            newByte = mem.getByte(regs[PC]++);
            instr->flags |= (newByte & 0x80) >> 4;
            // shift these 4 bits all the way up so that they will be correctly expanded
            instr->op1 = (newByte & 0x7F) << 28;
            // 17 for sic so that it is on 15 bit
            instr->op1 >>= 17;
            newByte = mem.getByte(regs[PC]++);
            instr->op1 |= newByte;
            break;
        case format::F3F4:
            newByte = mem.getByte(regs[PC]++);
            instr->flags |= (newByte & 0xF0) >> 4;
            if (instr->isExtended() && newByte & 0x8) {
                // extended negative number
                instr->op1 = (newByte & 0xF) << 8;
            } else {
                instr->op1 = (newByte & 0xF) << 28;
                instr->op1 >>= 20;
                instr->op1 = (newByte & 0xF) << 8;
            }
            newByte = mem.getByte(regs[PC]++);
            instr->op1 |= newByte;
            if (instr->isExtended()) {
                newByte = mem.getByte(regs[PC]++);
                instr->op1 = (instr->op1 << 8) | newByte;
            }
            break;
        default:
            throw std::runtime_error("Invalid instruction format");
    }
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
#ifdef DEBUG
    if (print)
#endif
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
    std::cout << "Machine running" << std::endl;
    while (status == machine_status::running) {
        _step(false);
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
    if (freq <= 0) {
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
