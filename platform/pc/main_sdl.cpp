#include <SDL2/SDL.h>
#include <cstdio>
#include <fstream>
#include "cpu.h"

// TODO -> Not finished, Bug etc.
// TODOC -> Needs Documentaion / Documenation not finished
// TOCHECK -> Finished but uncertain if it is correct
// Sub and Negative (N-Flag) are the same

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("How to use: ./GhostGBC.exe \"your_rom.gb\"\n"); // Explains how to use load function in Terminal, eg. ./GhostGBC.exe <your_rom.gb>
        return 1;                                               // Returns Error Code 1 and quits the Software
    }

    std::array<uint8_t, 65536> memory{}; // {} Inits every uint8_t as 0

    std::ifstream rom(argv[1], std::ios::binary); // argv[1] ist the second sent Argument, it should be the filename of the rom (eg. Metal Gear Solid (USA).gbc)

    if (!rom) // If rom object could not be created (eg. a loading error / file does not exist)
    {
        printf("Could not read %s\n", argv[1]);
        // argv[1] returns the second sent argument in the Terminal from the User (eg. Metal Gear Solid (USA).gbc)
        return 1;
    }

    rom.read(reinterpret_cast<char *>(memory.data()), 0x8000); // Loads first 32 KB of rom file TODOC

    cpu cpu;
    cpu.reset();

    while (true) // Infinite Loop
    {
        cpu.step(memory);

        // TODOC
        // Blargg's Testroms use the Link-Cable Registers of the Gameboy to Send the Passed or Failed Message
        // Here the Emulator checks for testing reasons if there was something sent and then prints that
        if (memory[0xFF02] == 0x81) // Check the Serial Transfer Control Register if it is activated
        {
            printf("%c", memory[0xFF01]); // Read and print the Character written to the Serial Transfer Data Register
            fflush(stdout);               // Force the Character to print instantly instead of whaiting for a line break
            memory[0xFF02] = 0;           // Clear the Serial Transfer Control Register so another Character can be sent
        }
    }

    return 0;
}