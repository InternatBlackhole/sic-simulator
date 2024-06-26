#include "registers.hpp"

#include <sstream>

// turns the value into a 3byte signed integer in 4 byte format
int canonize(int value) {
    if (value > 0x7FFFFF) {
        //number is negative
        //put ones into the upper 8 bits
        value |= 0xFF000000;
    }
    return value;
}

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

std::string registers::toString() {
    std::stringstream str;
    str << "A: " << regs[A] << "\n";
    str << "X: " << regs[X] << "\n";
    str << "L: " << regs[L] << "\n";
    str << "B: " << regs[B] << "\n";
    str << "S: " << regs[S] << "\n";
    str << "T: " << regs[T] << "\n";
    str << "F: " << regs[F] << "\n";
    str << "PC: " << regs[PC] << "\n";
    str << "SW: " << regs[SW] << "\n";
    return str.str();
}