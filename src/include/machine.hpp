#ifndef TK_MACHINE_HPP
#define TK_MACHINE_HPP
#include "loader.hpp"
#include "memory.hpp"
#include "registers.hpp"

class machine {
   public:
    machine(memory* mem, program_info& prog);
    ~machine();

    // think about passing a condition variable to run() so that it can be paused
    void run();

    memory& getMemory();
    registers& getRegisters();

   private:
    memory* mem;
    registers regs;
};

#endif  // MACHINE_HPP
