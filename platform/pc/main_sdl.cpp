#include <SDL2/SDL.h>
#include <cstdio>
#include "cpu.h"


int main(int argc, char *argv[])
{
    std::array<uint8_t, 65536> memory{}; // {} inits every uint8_t as 0
    memory[0x0100] = 0x00; // Write a NOP at Boot Adress

    cpu cpu;
    cpu.reset();

    printf("Before step: PC=0x%04X\n", cpu.PC); // What is 0x%04X?
    cpu.step(memory);
    printf("After step: PC=0x%04X, cycles=%d\n", cpu.PC, cpu.lastCycles);

    return 0;
}