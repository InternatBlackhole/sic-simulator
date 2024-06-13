#include <curses.h>
#include <errno.h>
#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <iostream>

#include "loader.hpp"
#include "machine.hpp"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RESET "\033[0m"

static machine* mach;
static memory* mem;
static program_info* prog;

static time_t lastPress = 0;

struct proc_info {
    pid_t pid;
    int readPoint;
    int writePoint;
};

void print_usage();
void set_signals();
void sigint_handler(int sig);
void dump_memory(int start, int length, struct proc_info* dump_pid);

int main(int argc, char* argv[]) {
    // initscr();
    // clear();
    // cbreak();
    // noecho();
    // nodelay(stdscr, FALSE);

    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct stat st;
    if (stat(argv[1], &st) != 0) {
        perror("Error opening file");
        return 1;
    }

    set_signals();

    prog = new program_info();
    // 1MB memory
    mem = new memory(1 << 20);
    loadProgram(argv[1], *mem, *prog);

    std::cout
        << GREEN
        << "Loaded program: " << prog->name << '\n'
        << "Entry point: " << prog->entryPoint << '\n'
        << "Load address: " << prog->loadAddress << '\n'
        << "Code size: " << prog->codeSize << '\n'
        << RESET;

    mach = new machine(*mem, *prog);

    print_usage();
    std::cout
        << GREEN
        << "Program loaded and machine ready to start!\n"
        << "Enter running parameters for the machine\n"
        << RESET;

    char buffer[256];
    int scanned = 0;
    if (scanned == EOF) {
        // let the OS clean :)
        std::cout << "\n" RESET;
        return 0;
    }
    // getch();

    int dump_cont = 0;
    struct proc_info dump_pid = {0};

    while (true) {
        std::cout
            << GREEN
            << "tk-sic> "
            << CYAN;
        scanned = scanf("%s", buffer);
        std::cout << RESET;
        if (scanned == EOF) {
            std::cout << '\n';
            break;
        }
        std::cout.flush();
        if (strcmp("r", buffer) == 0) {
            mach->run();
        } else if (strcmp("s", buffer) == 0) {
            mach->step();
        } else if (strcmp("p", buffer) == 0) {
            mach->pause();
        } else if (strcmp("h", buffer) == 0) {
            mach->halt();
            std::cout
                << YELLOW
                << "Machine halted\n"
                << RESET;
        } else if (strcmp("q", buffer) == 0) {
            break;
        } else if (strcmp("f", buffer) == 0) {
            std::cout
                << YELLOW
                << "Current frequency: "
                << CYAN
                << mach->getFrequency()
                << YELLOW
                << "\nEnter new frequency: "
                << CYAN;
            int freq;
            scanned = scanf("%d", &freq);
            try {
                mach->setFrequency(freq);
                std::cout << "Frequency set to: " << freq << RESET << '\n';
            } catch (std::runtime_error& e) {
                std::cout << RED << e.what() << "\n" RESET << '\n';
            }

        } else if (strcmp("d", buffer) == 0) {
            std::cout << '\n';
            dump_memory(dump_cont * 512, 512, &dump_pid);
            usleep(10000);  // 10ms
            std::cout
                << YELLOW
                << "Dump next 512B of memory? press 'd' again\n"
                << RESET;
            dump_cont++;
            continue;
        } else if (strcmp("reg", buffer) == 0) {
            std::cout << mach->getRegisters().toString();
        } else if (strcmp("?", buffer) == 0) {
            print_usage();
        } else if (strcmp("dump", buffer) == 0) {
            std::cout
                << YELLOW
                << "Dumping memory to file memory.bin ...\n"
                << RESET;
            std::ofstream dumpFile("memory.bin", std::ios::binary);
            dumpFile.write(mem->getMem(), mem->getMemSize());
            dumpFile.close();
            std::cout
                << YELLOW
                << "Dumped memory to file memory.bin\n"
                << RESET;
        } else {
            std::cout
                << RED
                << "Unknown command: "
                << CYAN
                << buffer
                << "\n" RESET;
        }

        // d wasn't pressed

        if (dump_pid.pid != 0) {
            // ignore errors
            kill(dump_pid.pid, SIGTERM);
            waitpid(dump_pid.pid, NULL, 0);
            close(dump_pid.writePoint);

            dump_pid.pid = 0;
            dump_pid.writePoint = -1;
            dump_pid.readPoint = -1;
        }
        dump_cont = 0;
    }

    delete mach;
    delete mem;
    delete prog;

    return 0;
}

void print_usage() {
    const char* usage =
        YELLOW
        "Usage:\n"
        "  r: run until done or SIGINT received\n"
        "  s: step\n"
        "  p: pause\n"
        "  h: halt\n"
        "  q: quit\n"
        "  f: set frequency\n"
        "  d: dump 512B of memory\n"
        "  reg: print registers\n"
        "  dump: dump memory to file memory.bin\n"
        "  ?: print this message\n" RESET;
    write(STDOUT_FILENO, usage, strlen(usage));
}

void set_signals() {
    struct sigaction sa = {0};
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) != 0) {
        perror("Error setting SIGINT handler");
        exit(1);
    }
}

void sigint_handler(int sig) {
    std::cout
        << GREEN
        << "\nSIGINT received, press again within 1s to exit simulator\n";
    int current = time(NULL);
    int prev = lastPress;
    // std::cout << "current: " << current << " prev: " << prev << '\n';
    if (current - prev <= 1) {
        std::cout << RED << "Exiting simulator\n"
                  << RESET;
        exit(0);
    }
    mach->pause();
    std::cout
        << "Machine paused\n"
        << RESET;
    lastPress = current;
}

void dump_memory(int start, int length, struct proc_info* dump_pid) {
    if (dump_pid->pid == 0) {
        int pipefd[2];
        if (pipe(pipefd) != 0) {
            perror("Error creating pipe");
            return;
        }

        fflush(stdout);
        std::cout.flush();
        fflush(stderr);
        std::cerr.flush();
        pid_t pid = fork();
        if (pid == 0) {
            // child
            if (prctl(PR_SET_PDEATHSIG, SIGTERM) == -1) {
                perror("Error setting PR_SET_PDEATHSIG");
                exit(1);
            }
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);

            char startStr[16];
            char lengthStr[16];
            sprintf(startStr, "%d", start);
            sprintf(lengthStr, "%d", length);
            execlp("xxd", "xxd", "-g", "1", "-c", "16", "-s", startStr /*, "-l", lengthStr*/, NULL);
            perror("Error executing xxd");
            exit(1);
        } else if (pid > 0) {
            // parent
            close(pipefd[0]);
            dump_pid->pid = pid;
            dump_pid->writePoint = pipefd[1];
            dump_pid->readPoint = -1;  // cuz it is closed
        } else {
            perror("Error forking");
        }
    }

    if (dump_pid->pid > 0) {
        // ignore write errors
        struct sigaction old = {0};
        struct sigaction ign = {0};
        ign.sa_handler = SIG_IGN;
        if (sigaction(SIGPIPE, &ign, &old) != 0) {
            perror("Error getting old SIGPIPE handler");
            return;
        }
        char* memory = mem->getMem();
        write(dump_pid->writePoint, &memory[start], length);
        if (sigaction(SIGPIPE, &old, NULL) != 0) {
            perror("Error setting old SIGPIPE handler");
            return;
        }
    }
}