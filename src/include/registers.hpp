#ifndef TK_REGISTERS_HPP
#define TK_REGISTERS_HPP

enum reg {
    A = 0,
    X = 1,
    L = 2,
    B = 3,
    S = 4,
    T = 5,
    F = 6,
    PC = 8,
    // bit 6: 1 LT, 0 GT; bit 7: 1 EQ, 0 NE
    SW = 9
};

class registers {
   public:
    registers();
    ~registers();

    void setReg(reg reg, int value);
    int getReg(reg reg);
    void setF(float value);
    float getF();

    // int operator[](reg reg) const;
    int& operator[](reg reg);
    int& operator[](int reg);

    int SW_set_CC(int comp1, int comp2);
    bool SW_get_LT();
    bool SW_get_EQ();

   private:
    int regs[10];
};

/*struct registers {
    // int* A; // 0
    // int* X; // 1
    // int* L; // 2
    // int* B; // 3
    // int* S; // 4
    // int* T; // 5
    // int* PC; // 8
    // int* SW; // 9

    int regs[10];
};*/

#endif  // TK_REGISTERS_HPP