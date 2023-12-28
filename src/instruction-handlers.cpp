#include <stdexcept>

#include "instructions.hpp"
#include "machine.hpp"
#include "memory.hpp"
#include "registers.hpp"

typedef void (*instruction_handler)(machine&, memory&, registers&, instruction&, int, int);
int actual_address(machine&, instruction&);
int actual_param(machine&, instruction&, int);

// done
void add(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[A] += actual_param;
}

// TODO: implement
void addf(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    throw std::runtime_error("Not implemented");
}

// done
void addr(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[instr.op2] += regs[instr.op1];
}

// done
void and_(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[A] &= actual_param;
}

// done
void clear(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[instr.op1] = 0;
}

// done
void comp(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs.SW_set_CC(regs[A], actual_param);
}

// TODO: implement
void compf(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    throw std::runtime_error("Not implemented");
}

// done
void compr(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs.SW_set_CC(regs[instr.op1], regs[instr.op2]);
}

// done
void div(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[A] /= actual_param;
}

// TODO: implement
void divf(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    throw std::runtime_error("Not implemented");
}

// done
void divr(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[instr.op2] /= regs[instr.op1];
}

// done
void fix(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[A] = (int)regs.getF();
}

// done
void flot(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs.setF((float)regs[A]);
}

// done
void hio(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    m.setIOAvailable(false);
}

void j(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    int offset = 3;
    if (instr.getFormat() == format::F3F4 && instr.isExtended()) {
        offset = 4;
    }
    if (instr.op1 == regs[PC] - offset)
        m.stop();
    regs[PC] = instr.op1;
}

void jeq(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    if (regs.SW_get_EQ()) {
        j(m, mem, regs, instr, actual_addr, actual_param);
    }
}

void jgt(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    if (!regs.SW_get_LT()) {
        j(m, mem, regs, instr, actual_addr, actual_param);
    }
}

void jlt(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    if (regs.SW_get_LT()) {
        j(m, mem, regs, instr, actual_addr, actual_param);
    }
}

void jsub(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[L] = regs[PC];
    regs[PC] = instr.op1;
}

void lda(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[A] = actual_param;
}

void ldb(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[B] = actual_param;
}

void ldf(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs.setF(actual_param);
}

void ldl(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[L] = actual_param;
}

void lds(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[S] = actual_param;
}

void ldt(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[T] = actual_param;
}

void ldx(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[X] = actual_param;
}

void lps(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    // regs[PC] = actual_param;
    throw std::runtime_error("Not implemented");
}

void mul(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[A] *= actual_param;
}

void mulf(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    throw std::runtime_error("Not implemented");
}

void mulr(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[instr.op2] *= regs[instr.op1];
}

void norm(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    throw std::runtime_error("Not implemented");
}

void or_(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[A] |= actual_param;
}

void rd(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    std::fstream& dev = m.getDevice(actual_param);
    char c = 0;
    dev >> c;
    regs[A] = c & 0xFF;
}

void rmo(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[instr.op2] = regs[instr.op1];
}

void rsub(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[PC] = regs[L];
}

void shiftl(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[instr.op1] <<= instr.op2;
}

void shiftr(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[instr.op1] >>= instr.op2;
}

void sio(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    throw std::runtime_error("Not implemented");
}

void ssk(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    throw std::runtime_error("Not implemented");
}

void sta(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    mem.setWord(actual_param, regs[A]);
}

void stb(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    mem.setWord(actual_param, regs[B]);
}

void stch(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    mem.setByte(actual_param, regs[A]);
}

void stf(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    throw std::runtime_error("Not implemented");
}

void sti(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    throw std::runtime_error("Not implemented");
}

void stl(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    mem.setWord(actual_param, regs[L]);
}

void sts(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    mem.setWord(actual_param, regs[S]);
}

void stsw(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    mem.setWord(actual_param, regs[SW]);
}

void stt(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    mem.setWord(actual_param, regs[T]);
}

void stx(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    mem.setWord(actual_param, regs[X]);
}

void sub(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[A] -= actual_param;
}

void subf(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    throw std::runtime_error("Not implemented");
}

void subr(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[instr.op2] -= regs[instr.op1];
}

void svc(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    throw std::runtime_error("Not implemented");
}

void td(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    // all devices always available
    regs.SW_set_CC(0, 0);  // 0 == 0 = true
}

void tio(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    throw std::runtime_error("Not implemented");
}

void tix(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[X]++;
    regs.SW_set_CC(regs[X], actual_param);
}

void tixr(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[X]++;
    regs.SW_set_CC(regs[X], regs[instr.op1]);
}

void wd(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    std::fstream& dev = m.getDevice(actual_param);
    dev << (char)(regs[A] & 0xFF);
}

void ldch(machine& m, memory& mem, registers& regs, instruction& instr, int actual_addr, int actual_param) {
    regs[A] = actual_param & 0xFF;
}

// leave these two here or define all function declarations on top
// the ones that don't exist are NULL
static const instruction_handler handlers[63] = {
    lda, ldx, ldl, sta, stx, stl, add, sub, mul, div,
    comp, tix, jeq, jgt, jlt, j, and_, or_, jsub, rsub,
    ldch, stch, addf, subf, mulf, divf, ldb, lds, ldf, ldt,
    stb, sts, stf, stt, compf, nullptr, addr, subr, mulr, divr,
    compr, shiftl, shiftr, rmo, svc, clear, tixr, nullptr, flot, fix,
    norm, nullptr, lps, sti, rd, wd, td, nullptr, stsw, ssk,
    sio, hio, tio};

int actual_address(machine& m, instruction& instr) {
    registers& regs = m.getRegisters();
    int addr = instr.op1;

    if (instr.getFormat() == format::SIC) return addr;

    switch (instr.getBP()) {
        case BP_state::base:
            addr += regs[B];
            break;
        case BP_state::pc:
            addr += regs[PC];
            break;
        case BP_state::indirect:
            break;
        default:
            throw std::runtime_error("Invalid BP state");
    }

    if (instr.isIndexed()) {
        addr += regs[X];
    }

    return addr;
}

int actual_param(machine& m, instruction& instr, int address) {
    memory& mem = m.getMemory();
    int param = address;

    switch (instr.getNI()) {
        case NI_state::indirect:
            // derefrence twice
            param = mem.getWord(param);
        case NI_state::SIC:
            // SIC also derefrences once
        case NI_state::simple:
            // derefrence once
            param = mem.getWord(param);
            break;
        case NI_state::immediate:
            break;
        default:
            throw std::runtime_error("Invalid NI state");
    }

    return param;
}

int exec_instruction(machine& m, instruction instr) {
    instruction_handler handler = handlers[instr.opcode];
    if (handler == nullptr) {
        throw std::runtime_error("Instruction does not exist");
    }

    if (instr.getFormat() == format::F2 || instr.getFormat() == format::F1) {
        // optimization for F1 and F2 instructions
        handler(m, m.getMemory(), m.getRegisters(), instr, 0, 0);
        return 0;
    }

    int addr = actual_address(m, instr);
    int param = actual_param(m, instr, addr);
    handler(m, m.getMemory(), m.getRegisters(), instr, addr, param);
    return 0;
}