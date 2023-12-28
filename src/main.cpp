#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "loader.hpp"
#include "machine.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    program_info prog;
    // 1MB memory
    memory mem(1 << 10);
    loadProgram(argv[1], mem, prog);
#ifdef DEBUG
    printf("Loaded program %s\n", prog.name.c_str());
    printf("Entry point: %d\n", prog.entryPoint);
    printf("Load address: %d\n", prog.loadAddress);
    printf("Code size: %d\n", prog.codeSize);
#endif
    machine mach(&mem, prog);
    printf("Press enter to start the machine\n");
    //scanf("%*c");  // wait for user to press enter
    mach.run();
    return 0;
}