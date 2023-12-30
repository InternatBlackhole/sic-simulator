#ifndef TK_MACHINE_HPP
#define TK_MACHINE_HPP
#include <cstdio>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <mutex>

#include "instructions.hpp"
#include "loader.hpp"
#include "memory.hpp"
#include "registers.hpp"

enum class machine_status {
    ready,
    running,
    paused,
    halted,
    // io_waiting,
    // io_done,
    // io_error,
};

class machine {
   public:
    machine(memory& mem, program_info& prog);
    ~machine();

    // think about passing a condition variable to run() so that it can be paused
    void run();
    void halt();
    void pause();
    void step();

    memory& getMemory();
    registers& getRegisters();
    bool ioAvailable();
    void setIOAvailable(bool ioAvailable);
    std::fstream& getDevice(int dev);
    void setFrequency(int freq);
    int getFrequency();

   private:
    memory& mem;
    registers regs;
    machine_status status;
    bool _ioAvailable;
    std::fstream* devices[256] = {0};
    int freq;

    instruction* fetch();
    bool precheck();
    inline void _step(bool print);
    inline void _status_report();
};

#endif  // MACHINE_HPP
