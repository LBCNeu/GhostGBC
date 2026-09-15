#include "cpu.h"

void cpu::reset()
{
    A = 0x01;
    F = 0xB0; // Register Values of real GB DMG Harware on Power-up
    B = 0x00;
    C = 0x13; // TODO... Via Pan Docs: these are the Power-up values for the DMG version but the CGB (GBC) versions are very diffrent... for testing leave I'll keep DMG Values
    D = 0x00;
    E = 0xD8;
    H = 0x01;
    L = 0x4D;

    SP = 0xFFFE;
    PC = 0x0100; // GB Rom Start is at 0x0100

    lastCycles = 0;
}

void cpu::step(std::array<uint8_t, 65536> &memory) // using std::array instead of uint8_t memory[65536] is better in this case baecause it knows its own size (eg. memory.size())
{                                                  // & signals a Refference, the memory Array is not coppied on step() but is refferenced and the original one gets edited

    uint8_t opcode = memory[PC]; // Fetching Opcode Instruction
    PC++;                        // Jump to next Byte fso it can be used in Operation (eg. "LDA n" PC points to LDA before PC++, PC points to n after PC++)

    // Opcode Instructions

    switch (opcode)
    {

    case 0x00: // NOP... Nothing... Absolutely Nothing
        lastCycles = 4;
        break;

        // ------------- LD Reg, u8 ----------------

    case 0x06: // LD B, u8... Load unsigned 8-bit int into B-Reg
        B = memory[PC];
        PC++;
        lastCycles = 8;
        break;

    case 0x0E:
        C = memory[PC];
        PC++;
        lastCycles = 8;
        break;

    case 0x16:
        D = memory[PC];
        PC++;
        lastCycles = 8;
        break;

    case 0x1E:
        E = memory[PC];
        PC++;
        lastCycles = 8;
        break;

    case 0x26:
        H = memory[PC];
        PC++;
        lastCycles = 8;
        break;

    case 0x2E:
        L = memory[PC];
        PC++;
        lastCycles = 8;
        break;

    case 0x3E:
        A = memory[PC];
        PC++;
        lastCycles = 8;
        break;

        // ------------- LD Reg, u16 ----------------

    case 0xFA:
    { // LD A, u16
        // TODO
        uint16_t addr = memory[PC];
        PC++;
        addr = addr | (memory[PC] << 8);
        A = memory[addr];
        PC++;
        lastCycles = 16;
        break;
    }
        // ------------- LD Reg -> Reg ----------------

    case 0x78: // LD A, B... Load unsigned B-Reg data int into A-Reg
        A = B;
        lastCycles = 4;
        break;

    case 0x79:
        A = C;
        lastCycles = 4;
        break;

    case 0x7A:
        A = D;
        lastCycles = 4;
        break;

    case 0x7B:
        A = E;
        lastCycles = 4;
        break;

    case 0x7C:
        A = H;
        lastCycles = 4;
        break;

    case 0x7D:
        A = L;
        lastCycles = 4;
        break;

    case 0x7E: // LD A, (HL) <- Brackets in the name mean that is has to load the Value into A and not the adress
        A = memory[getHL()];
        lastCycles = 8;
        break;

    case 0x7F:
        A = A;
        lastCycles = 4;
        break;

    case 0x0A:
        A = memory[getBC()];
        lastCycles = 8;
        break;

    case 0x1A:
        A = memory[getDE()];
        lastCycles = 8;
        break;

    case 0x2A: // LD A, (HL+) HL gets incremented after Operation
        A = memory[getHL()];
        setHL(getHL() + 1); // Adress in HL Reg gets inremented by 1 so it points to the next Byte
        lastCycles = 8;
        break;

    case 0x3A: // LD A, (HL-) HL gets decremented after Operation
        A = memory[getHL()];
        setHL(getHL() - 1); // Adress in HL Reg gets decremented by 1 so it points to the Byte before
        lastCycles = 8;
        break;

    case 0xF0: // LD A, (FF00+u8) Load Value in Adress FF00+u8 into A Reg
    {
        uint16_t addr = 0xFF00 + memory[PC]; // Adress is composed of 0xFF00 and the Byte next to the Opcode
        A = memory[addr];
        PC++;
        lastCycles = 8;
        break;
    }

    case 0xF2: // LD A, (FF00+C) Load Value in Adress FF00 + C into A Reg
    {
        uint16_t addr = 0xFF00 + C; // Adress is composed of 0xFF00 and C Reg and no following Byte -> no PC++ needed
        A = memory[addr];
        lastCycles = 8;
        break;
    }

        // ------------- INC ----------------

    case 0x3C: // INC A Increase Value of A-Reg by 1
        A++;
        setZeroFlag(A == 0); // Only set ZeroFlag if Result is Zero
        setSubFlag(false);
        setHalfFlag((A & 0x0F) == 0x00); // Half Carry checks i there was a carry from the lower 4 bits to the higher. A gets put through a mask (0x0F, 1111) with & to check the low 4 bits, then it gets checked if they are all Zero, that would mean that a carry took place
        // INC does not touch the Carry Flag!!!
        lastCycles = 4;
        break;

        // ------------- DEC ----------------

    case 0x3D: // DEC A Decrease Value of A-Reg by 1
        A--;
        setZeroFlag(A == 0); // Only set ZeroFlag if Result is Zero
        setSubFlag(true);
        setHalfFlag((A & 0x0F) == 0x0F); // Half Borrow checks i there was a "reverse" carry from the higher 4 bits to the lower. A gets put through a mask (0x0F, 1111) with & to check the low 4 bits, then it gets checked if they are all 1, that would mean that a "reverse" carry/borrow took place
        // DEC does not touch the Carry Flag!!!
        lastCycles = 4;
        break;

    default:
        lastCycles = 4; // TODO... just for testing, not legit GB behaviour
        break;
    }
}