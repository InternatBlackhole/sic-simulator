#include "registers.hpp"

registers::registers() {
    for (int i = 0; i < 10; i++) {
        regs[i] = 0;
    }
}

registers::~registers() {}

void registers::setReg(reg reg, int value) {
    regs[reg] = value;
}

int registers::getReg(reg reg) {
    return regs[reg];
}

void registers::setF(float value) {
    regs[6] = (int)value;
}

float registers::getF() {
    return (float)regs[6];
}

int& registers::operator[](reg reg) {
    return regs[reg];
}