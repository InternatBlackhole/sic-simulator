#ifndef TK_MACHINE_HPP
#define TK_MACHINE_HPP
#include <cstdio>
#include <fstream>

#include "instructions.hpp"
#include "loader.hpp"
#include "memory.hpp"
#include "registers.hpp"

class machine {
   public:
    machine(memory* mem, program_info& prog);
    ~machine();

    // think about passing a condition variable to run() so that it can be paused
    void run();
    void stop();

    memory& getMemory();
    registers& getRegisters();
    bool ioAvailable();
    void setIOAvailable(bool ioAvailable);
    std::fstream& getDevice(int dev);

   private:
    memory* mem;
    registers regs;
    bool running;
    bool _ioAvailable;
    std::fstream* devices[256] = {nullptr};

    instruction* fetch();
};

#endif  // MACHINE_HPP
