#ifndef TK_MEMORY_HPP
#define TK_MEMORY_HPP

class memory {
   public:
    memory(int size);
    ~memory();
    void setByte(int address, int value);
    int getByte(int address);
    void setWord(int address, int value);
    int getWord(int address);
    int getMemSize();
    char* getMem();

   private:
    char* mem;
    int memSize;
};

#endif  // MEMORY_HPP
