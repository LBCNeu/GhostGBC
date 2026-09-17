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

    case 0x06: // LD B, u8... Load unsigned 8-bit int into B Reg
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

    case 0x3E: // LD A,u8
        A = memory[PC];
        PC++;
        lastCycles = 8;
        break;

        // ------------- LD Reg,(u16) ----------------

    case 0xFA:                           // LD A,(u16) Value at the 16bit Adress gets loaded into A Reg
    {                                    // In Memory: [Opcode 0xFA] [Low Byte] [High Byte]
        uint16_t addr = memory[PC];      // Low Byte -> addr
        PC++;                            // PC shifted to High Byte
        addr = addr | (memory[PC] << 8); // High byte gets shifted left and then merged with Low Byte (in addr)
        // Before: [Low Byte] [High Byte] -> After shift + merge -> 0x[High Byte] [Low Byte]
        A = memory[addr]; // Value at addr into A Reg
        PC++;
        lastCycles = 16;
        break;
    }
        // ------------- LD Reg -> Reg ----------------

    case 0x78: // LD A, B... Load unsigned B Reg data int into A Reg
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

    case 0x40: // LD B, B... Load unsigned B Reg data int into B Reg
        B = B;
        lastCycles = 4;
        break;

    case 0x41:
        B = C;
        lastCycles = 4;
        break;

    case 0x42:
        B = D;
        lastCycles = 4;
        break;

    case 0x43:
        B = E;
        lastCycles = 4;
        break;

    case 0x44:
        B = H;
        lastCycles = 4;
        break;

    case 0x45:
        B = L;
        lastCycles = 4;
        break;

    case 0x46: // LD B, (HL) <- Brackets in the name mean that is has to load the Value into B and not the adress
        B = memory[getHL()];
        lastCycles = 8;
        break;

    case 0x47:
        B = A;
        lastCycles = 4;
        break;

    case 0x50: // LD D, B... Load unsigned B Reg data int into D Reg
        D = B;
        lastCycles = 4;
        break;

    case 0x51:
        D = C;
        lastCycles = 4;
        break;

    case 0x52:
        D = D;
        lastCycles = 4;
        break;

    case 0x53:
        D = E;
        lastCycles = 4;
        break;

    case 0x54:
        D = H;
        lastCycles = 4;
        break;

    case 0x55:
        D = L;
        lastCycles = 4;
        break;

    case 0x56: // LD D, (HL) <- Brackets in the name mean that is has to load the Value into D and not the adress
        D = memory[getHL()];
        lastCycles = 8;
        break;

    case 0x57:
        D = A;
        lastCycles = 4;
        break;

    case 0x60: // LD H, B... Load unsigned B Reg data int into H Reg
        H = B;
        lastCycles = 4;
        break;

    case 0x61:
        H = C;
        lastCycles = 4;
        break;

    case 0x62:
        H = D;
        lastCycles = 4;
        break;

    case 0x63:
        H = E;
        lastCycles = 4;
        break;

    case 0x64:
        H = H;
        lastCycles = 4;
        break;

    case 0x65:
        H = L;
        lastCycles = 4;
        break;

    case 0x66: // LD H, (HL) <- Brackets in the name mean that is has to load the Value into H and not the adress
        H = memory[getHL()];
        lastCycles = 8;
        break;

    case 0x67:
        H = A;
        lastCycles = 4;
        break;

    case 0x48: // LD C, B... Load unsigned B Reg data int into C Reg
        C = B;
        lastCycles = 4;
        break;

    case 0x49:
        C = C;
        lastCycles = 4;
        break;

    case 0x4A:
        C = D;
        lastCycles = 4;
        break;

    case 0x4B:
        C = E;
        lastCycles = 4;
        break;

    case 0x4C:
        C = H;
        lastCycles = 4;
        break;

    case 0x4D:
        C = L;
        lastCycles = 4;
        break;

    case 0x4E: // LD C, (HL) <- Brackets in the name mean that is has to load the Value into C and not the adress
        C = memory[getHL()];
        lastCycles = 8;
        break;

    case 0x4F:
        C = A;
        lastCycles = 4;
        break;

    case 0x58: // LD E, B... Load unsigned B Reg data int into E Reg
        E = B;
        lastCycles = 4;
        break;

    case 0x59:
        E = C;
        lastCycles = 4;
        break;

    case 0x5A:
        E = D;
        lastCycles = 4;
        break;

    case 0x5B:
        E = E;
        lastCycles = 4;
        break;

    case 0x5C:
        E = H;
        lastCycles = 4;
        break;

    case 0x5D:
        E = L;
        lastCycles = 4;
        break;

    case 0x5E: // LD E, (HL) <- Brackets in the name mean that is has to load the Value into E and not the adress
        E = memory[getHL()];
        lastCycles = 8;
        break;

    case 0x5F:
        E = A;
        lastCycles = 4;
        break;

    case 0x68: // LD L, B... Load unsigned B Reg data int into L Reg
        L = B;
        lastCycles = 4;
        break;

    case 0x69:
        L = C;
        lastCycles = 4;
        break;

    case 0x6A:
        L = D;
        lastCycles = 4;
        break;

    case 0x6B:
        L = E;
        lastCycles = 4;
        break;

    case 0x6C:
        L = H;
        lastCycles = 4;
        break;

    case 0x6D:
        L = L;
        lastCycles = 4;
        break;

    case 0x6E: // LD L, (HL) <- Brackets in the name mean that is has to load the Value into L and not the adress
        L = memory[getHL()];
        lastCycles = 8;
        break;

    case 0x6F:
        L = A;
        lastCycles = 4;
        break;

        // ------------- LD A,rr ----------------

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

    case 0x3C: // INC A Increase Value of A Reg by 1
        A++;
        setZeroFlag(A == 0); // Only set ZeroFlag if Result is Zero
        setSubFlag(false);
        setHalfFlag((A & 0x0F) == 0x00); // Half Carry checks i there was a carry from the lower 4 bits to the higher. A gets put through a mask (0x0F, 1111) with & to check the low 4 bits, then it gets checked if they are all Zero, that would mean that a carry took place
        // INC does not touch the Carry Flag!!!
        lastCycles = 4;
        break;

        // ------------- DEC ----------------

    case 0x3D: // DEC A Decrease Value of A Reg by 1
        A--;
        setZeroFlag(A == 0); // Only set ZeroFlag if Result is Zero
        setSubFlag(true);
        setHalfFlag((A & 0x0F) == 0x0F); // Half Borrow checks i there was a "reverse" carry from the higher 4 bits to the lower. A gets put through a mask (0x0F, 1111) with & to check the low 4 bits, then it gets checked if they are all 1, that would mean that a "reverse" carry/borrow took place
        // DEC does not touch the Carry Flag!!!
        lastCycles = 4;
        break;

        // ------------- ADD ----------------

    case 0xC6: // ADD A,u8 Adds u8 to A Reg
        setHalfFlag(((A & 0x0F) + (memory[PC] & 0x0F)) > 0x0F);
        setCarryFlag((A + memory[PC]) > 0xFF);
        A = A + memory[PC];
        setZeroFlag(A == 0);
        setSubFlag(false);
        PC++;
        lastCycles = 8;
        break;

        // ---------- ADD rr,rr ----------------

    case 0x09: // ADD HL,BC Adds BC to HL
    {
        setHalfFlag(((getHL() & 0x0FFF) + (getBC() & 0x0FFF)) > 0x0FFF);
        setCarryFlag((getHL() + getBC()) > 0xFFFF);
        setHL(getHL() + getBC());
        setSubFlag(false);
        lastCycles = 8;
        break;
    }

        // ---------- ADD rr,i8 ---------------- TODO

        // case 0xE8: // ADD SP,i8 / ADD SP,e8, ADD SP,r8
        // {
        //     setHalfFlag(((getHL() & 0x0FFF) + (getBC() & 0x0FFF)) > 0x0FFF);
        //     setCarryFlag((getHL() + getBC()) > 0xFFFF);
        //     setHL(getHL() + getBC());
        //     setSubFlag(false);
        //     setZeroFlag(false);
        //     PC++;
        //     lastCycles = 16;
        //     break;
        // }

        // ------------- DAA ----------------

    case 0x27: // DAA Corrects Additions to be correct in BCD Format instead of Binary / Hex. Unlinke the Intel 8080 the Sharp LR35 (GB CPU) can Correct both additions and subtractions with DAA
    {          // Based on documentation on https://blog.ollien.com/posts/gb-daa/
        uint8_t correction = 0;

        if (!getSubFlag())
        {
            if (((A & 0x0F) > 0x09) || getHalfFlag())
            {
                correction = correction | 0x06;
            }
            if ((A > 0x99) || getCarryFlag())
            {
                correction = correction | 0x60;
                setCarryFlag(true);
            }

            A = A + correction;
        }
        else
        {
            if (getHalfFlag())
            {
                correction = correction | 0x06;
            }
            if (getCarryFlag())
            {
                correction = correction | 0x60;
            }

            A = A - correction;
        }

        setHalfFlag(false);
        setZeroFlag(A == 0);
        lastCycles = 4;
        break;
    }

    default:
        lastCycles = 4; // TODO... just for testing, not legit GB behaviour
        break;
    }
}