#include "memory.hpp"

memory::memory(int size) {
    mem = new char[size];
    memSize = size;
}

memory::~memory() {
    delete[] mem;
}

void memory::setByte(int address, int value) {
    mem[address] = value & 0xFF;  // yes i know this is done in java but it has an optimization in c
}

int memory::getByte(int address) {
    return mem[address];
}

void memory::setWord(int address, int value) {
    mem[address] = (value >> 16) & 0xFF;
    mem[address + 1] = (value >> 8) & 0xFF;
    mem[address + 2] = value & 0xFF;
}

int memory::getWord(int address) {
    return (mem[address] << 16) | (mem[address + 1] << 8) | mem[address + 2];
}

int memory::getMemSize() {
    return memSize;
}

char* memory::getMem() {
    return mem;
}