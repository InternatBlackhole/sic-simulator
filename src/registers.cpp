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

int& registers::operator[](int reg) {
    return regs[reg];
}

int registers::SW_set_CC(int comp1, int comp2) {
    int cc = 0;
    cc |= (comp1 < comp2) << 1;
    cc |= (comp1 == comp2);
    regs[SW] &= 0xFCFFFF;
    regs[SW] |= cc << 16;
    return regs[SW];
}

bool registers::SW_get_LT() {
    return regs[SW] & 0x020000;
}

bool registers::SW_get_EQ() {
    return regs[SW] & 0x010000;
}