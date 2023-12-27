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
    //1MB memory
    memory mem(1<<10);
    loadProgram(argv[1], mem, prog);
    machine mach(&mem, prog);
    mach.run();
    return 0;
}