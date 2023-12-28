#ifndef TK_INSTRUCTION_HANDLERS_HPP
#define TK_INSTRUCTION_HANDLERS_HPP

#include "instructions.hpp"
#include "machine.hpp"
#include "memory.hpp"
#include "registers.hpp"

int exec_instruction(machine& m, instruction instr);
#endif  // INSTRUCTION_HANDLERS_HPP