#include "memory.hpp"

memory::memory(int size) {
    mem = new unsigned char[size];
    memSize = size;
}

memory::~memory() {
    delete[] mem;
}

void memory::setByte(int address, int value) {
    mem[address] = value & 0xFF;  // yes i know this is done in java but it has an optimization in c
}

int memory::getByte(int address) {
    return mem[address] & 0xFF;
}

void memory::setWord(int address, int value) {
    mem[address] = (value >> 16) & 0xFF;
    mem[address + 1] = (value >> 8) & 0xFF;
    mem[address + 2] = value & 0xFF;
}

int memory::getWord(int address) {
    int tmp = (mem[address] << 16) | (mem[address + 1] << 8) | mem[address + 2];
    tmp = tmp & 0x00800000 ? tmp | 0xFF000000 : tmp;
    return tmp;
}

int memory::getMemSize() {
    return memSize;
}

char* memory::getMem() {
    return (char*)mem;
}