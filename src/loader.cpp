#include "loader.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <iostream>

void handleH(std::string, memory&, program_info&);
void handleE(std::string, memory&, program_info&);
void handleD(std::string, memory&, program_info&);
void handleR(std::string, memory&, program_info&);
void handleT(std::string, memory&, program_info&);
void handleM(std::string, memory&, program_info&);

void loadProgram(std::string filename, memory& mem, program_info& prog) {
    std::ifstream file(filename, std::ios::in);
    char buffer[1 << 16];
    memset(buffer, 0, sizeof(buffer));
    while (!file.eof()) {
        file.getline(buffer, sizeof(buffer), L'\n');
        std::string line(buffer);
        switch (line[0]) {
            case 'H':
                handleH(line, mem, prog);
                break;
            case 'D':
                handleD(line, mem, prog);
                break;
            case 'R':
                handleR(line, mem, prog);
                break;
            case 'T':
                handleT(line, mem, prog);
                break;
            case 'E':
                handleE(line, mem, prog);
                break;
            case 'M':
                handleM(line, mem, prog);
                break;
            default:
                std::cout << "Invalid line: " << line << std::endl;
                break;
        }
    }
    file.close();
}

void handleH(std::string line, memory& mem, program_info& prog) {
    prog.name = line.substr(2, 6);
    prog.loadAddress = std::stoi(line.substr(8, 6), nullptr, 16);
    prog.codeSize = std::stoi(line.substr(14, 6), nullptr, 16);
}

void handleD(std::string line, memory& mem, program_info& prog) {
    // not implemented
    throw std::runtime_error("D record not implemented");
}

void handleR(std::string line, memory& mem, program_info& prog) {
    // not implemented
    throw std::runtime_error("R record not implemented");
}

void handleT(std::string line, memory& mem, program_info& prog) {
    int address = std::stoi(line.substr(2, 6), nullptr, 16);
    int length = std::stoi(line.substr(8, 2), nullptr, 16);
    for (int i = 0; i < length; i++) {
        mem.setByte(address + i, std::stoi(line.substr(10 + i * 2, 2), nullptr, 16));
    }
}

void handleE(std::string line, memory& mem, program_info& prog) {
    prog.entryPoint = std::stoi(line.substr(2, 6), nullptr, 16);
}

void handleM(std::string line, memory& mem, program_info& prog) {
    // not implemented
    // the ones that use just reloacation without imported symbols could be done
    throw std::runtime_error("M record not implemented");
}
