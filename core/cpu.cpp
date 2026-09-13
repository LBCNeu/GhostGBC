#include "cpu.h"

void cpu::reset()
{
    A = 0x01; F = 0xB0; // Register Values of real GB DMG Harware on Power-up
    B = 0x00; C = 0x13; // TODO... Via Pan Docs: these are the Power-up values for the DMG version but the CGB (GBC) versions are very diffrent... for testing leave I'll keep DMG Values
    D = 0x00; E = 0xD8;
    H = 0x01; L = 0x4D;

    SP = 0xFFFE;
    PC = 0x0100; // GB Rom Start is at 0x0100

    lastCycles = 0;
}

void cpu::step(std::array<uint8_t, 65536>& memory) // using std::array instead of uint8_t memory[65536] is better in this case baecause it knows its own size (eg. memory.size())
{                                                  // & signals a Refference, the memory Array is not coppied on step() but is refferenced and the original one gets edited


    uint8_t opcode = memory[PC]; // Fetching Opcode Instruction
    PC++; // Jump to next Byte fso it can be used in Operation (eg. "LDA n" PC points to LDA before PC++, PC points to n after PC++)


    // Opcode Instructions

    switch (opcode)
    {
    case 0x00: // NOP... Nothing... Absolutely Nothing
        lastCycles = 4;
        break;
    
    default:
        lastCycles = 4; // TODO... just for testing, not legit GB behaviour
        break;
    }
}