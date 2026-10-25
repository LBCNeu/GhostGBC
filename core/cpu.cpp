#include "cpu.h"
#include <cstdio> // for testing TODO

// TODO -> Not finished, Bug etc.
// TODOC -> Needs Documentaion / Documenation not finished
// TOCHECK -> Finished but uncertain if it is correct
// Sub and Negative (N-Flag) are the same

void cpu::reset()
{
    ime = false;

    A = 0x01;
    F = 0xB0; // Register Values of real GB DMG Harware on Power-up
    B = 0x00;
    C = 0x13; // TODO... Via Pan Docs: these are the Power-up values for the DMG version but the CGB (GBC) versions are very diffrent... for testing I'll keep DMG Values
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

    case 0x00: // NOP... Nothing... Absolutely Nothing TODO
        lastCycles = 4;
        break;

    case 0x10: // STOP (second Byte (00) gets skipped) TODO
        PC++;
        lastCycles = 4;
        break;

    case 0x76: // HALT TODO
        lastCycles = 4;
        break;

    case 0xF3: // DI Disable Interrupts
        ime = false;
        lastCycles = 4;
        break;

    case 0xFB: // EI Enable Interrupts
        ime = true;
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
        lastCycles = 12;
        break;
    }

    case 0xF2: // LD A, (FF00+C) Load Value in Adress FF00 + C into A Reg
    {
        uint16_t addr = 0xFF00 + C; // Adress is composed of 0xFF00 and C Reg and no following Byte -> no PC++ needed
        A = memory[addr];
        lastCycles = 8;
        break;
    }

        // ------------- LD rr,A ----------------

    case 0xE0: // LD (FF00+u8),A Load Value in A Reg into Memory at Adress FF00+u8
    {
        uint16_t addr = 0xFF00 + memory[PC]; // Adress is composed of 0xFF00 and the Byte next to the Opcode
        memory[addr] = A;
        PC++;
        lastCycles = 12;
        break;
    }

    case 0xE2: // LD (FF00+C),A Load Value in A Reg into Memory at  Adress FF00+C
    {
        uint16_t addr = 0xFF00 + C; // Adress is composed of 0xFF00 and C Reg and no following Byte -> no PC++ needed
        memory[addr] = A;
        lastCycles = 8;
        break;
    }

        // ------------- LD (HL),r ----------------

    case 0x70: // LD (HL),B <- Brackets in the name mean that is has to load the Value from B Reg into Memory at addr HL
        memory[getHL()] = B;
        lastCycles = 8;
        break;

    case 0x71:
        memory[getHL()] = C;
        lastCycles = 8;
        break;

    case 0x72:
        memory[getHL()] = D;
        lastCycles = 8;
        break;

    case 0x73:
        memory[getHL()] = E;
        lastCycles = 8;
        break;

    case 0x74:
        memory[getHL()] = H;
        lastCycles = 8;
        break;

    case 0x75:
        memory[getHL()] = L;
        lastCycles = 8;
        break;

    case 0x77:
        memory[getHL()] = A;
        lastCycles = 8;
        break;

    case 0x36: // LD (HL),u8 Load u8 into Memory at addr HL
        memory[getHL()] = memory[PC];
        lastCycles = 12;
        PC++;
        break;

        // ------------- LD (rr),A ----------------

    case 0x02: // LD (BC),A <- Brackets in the name mean that is has to load the Value from A Reg into Memory at addr in (BC)
        memory[getBC()] = A;
        lastCycles = 8;
        break;

    case 0x12: // LD (DE),A
        memory[getDE()] = A;
        lastCycles = 8;
        break;

    case 0x22: // LD (HL+),A Writes Value from A Reg into memory at addr in HL joint Reg, and then increases HL joint Reg by 1
        memory[getHL()] = A;
        setHL(getHL() + 1);
        lastCycles = 8;
        break;

    case 0x32: // LD (HL-),A Writes Value from A Reg into memory at addr in HL joint Reg, and then decreases HL joint Reg by 1
        memory[getHL()] = A;
        setHL(getHL() - 1);
        lastCycles = 8;
        break;

        // ------------- LD rr,u16 ----------------

    case 0x01: // LD BC,u16 u16 Value after Opcode gets written into BC joint Reg
    {
        uint16_t val = memory[PC];
        PC++;
        val = val | (memory[PC] << 8);
        PC++;
        setBC(val);
        lastCycles = 12;
        break;
    }

    case 0x11: // LD DE,u16 u16 Value after Opcode gets written into DE joint Reg
    {
        uint16_t val = memory[PC];
        PC++;
        val = val | (memory[PC] << 8);
        PC++;
        setDE(val);
        lastCycles = 12;
        break;
    }

    case 0x21: // LD HL,u16 u16 Value after Opcode gets written into HL joint Reg
    {
        uint16_t val = memory[PC];
        PC++;
        val = val | (memory[PC] << 8);
        PC++;
        setHL(val);
        lastCycles = 12;
        break;
    }

    case 0x31: // LD SP,u16 u16 Value after Opcode gets written into SP
    {
        uint16_t val = memory[PC];
        PC++;
        val = val | (memory[PC] << 8);
        PC++;
        SP = val;
        lastCycles = 12;
        break;
    }

        // ------------- LD (u16),r ----------------

    case 0xEA:                           // LD (u16),A Value at A Reg gets loaded to memory at 16bit Adress
    {                                    // In Memory: [Opcode 0xEA] [Low Byte] [High Byte]
        uint16_t addr = memory[PC];      // Low Byte -> addr
        PC++;                            // PC shifted to High Byte
        addr = addr | (memory[PC] << 8); // High byte gets shifted left and then merged with Low Byte (in addr)
        PC++;
        // Before: [Low Byte] [High Byte] -> After shift + merge -> 0x[High Byte] [Low Byte]
        memory[addr] = A; // Value at A Reg into addr
        lastCycles = 16;
        break;
    }

        // ------------- LD (u16),SP ----------------

    case 0x08:                           // LD (u16),SP Value at SP gets loaded to memory at 16bit Adress
    {                                    // In Memory: [Opcode 0x08] [Low Byte] [High Byte]
        uint16_t addr = memory[PC];      // Low Byte -> addr
        PC++;                            // PC shifted to High Byte
        addr = addr | (memory[PC] << 8); // High byte gets shifted left and then merged with Low Byte (in addr)
        PC++;
        memory[addr] = SP & 0xFF;                        // Low Byte of SP
        memory[(uint16_t)(addr + 1)] = (SP >> 8) & 0xFF; // High Byte of SP
        // Edge case: When adding 1 to addr and addr is 0xFFFF the operation will give 0x10000 which is false, it should wrap around back to 0x0000 like real hardware, to chop off the 1 from 0x10000 (uint16_t) is used to typecast the result back into 16bit
        lastCycles = 20;
        break;
    }

        // ------------- LD SP,HL ----------------

    case 0xF9: // LD SP,HL Value of HL joint Reg gets written into the Stack-Pointer
        SP = getHL();
        lastCycles = 8;
        break;

        // ------------- INC ----------------

    case 0x3C: // INC A Increase Value of A Reg by 1
        A++;
        setZeroFlag(A == 0); // Only set ZeroFlag if Result is Zero
        setSubFlag(false);
        setHalfFlag((A & 0x0F) == 0x00); // Half Carry checks i there was a carry from the lower 4 bits to the higher. A gets put through a mask (0x0F, 1111) with & to check the low 4 bits, then it gets checked if they are all Zero, that would mean that a carry took place
        // INC does not touch the Carry Flag!!!
        lastCycles = 4;
        break;

    case 0x2C: // INC L Increase Value of L Reg by 1
        L++;
        setZeroFlag(L == 0);
        setSubFlag(false);
        setHalfFlag((L & 0x0F) == 0x00);
        lastCycles = 4;
        break;

    case 0x1C: // INC E Increase Value of E Reg by 1
        E++;
        setZeroFlag(E == 0);
        setSubFlag(false);
        setHalfFlag((E & 0x0F) == 0x00);
        lastCycles = 4;
        break;

    case 0x0C: // INC C Increase Value of C Reg by 1
        C++;
        setZeroFlag(C == 0);
        setSubFlag(false);
        setHalfFlag((C & 0x0F) == 0x00);
        lastCycles = 4;
        break;

    case 0x04: // INC B Increase Value of B Reg by 1
        B++;
        setZeroFlag(B == 0);
        setSubFlag(false);
        setHalfFlag((B & 0x0F) == 0x00);
        lastCycles = 4;
        break;

    case 0x14: // INC D Increase Value of D Reg by 1
        D++;
        setZeroFlag(D == 0);
        setSubFlag(false);
        setHalfFlag((D & 0x0F) == 0x00);
        lastCycles = 4;
        break;

    case 0x24: // INC H Increase Value of H Reg by 1
        H++;
        setZeroFlag(H == 0);
        setSubFlag(false);
        setHalfFlag((H & 0x0F) == 0x00);
        lastCycles = 4;
        break;

    case 0x34: // INC (HL) Increase Value at addr in (HL) joint Reg by 1
        memory[getHL()] = memory[getHL()] + 1;
        setZeroFlag(memory[getHL()] == 0);
        setSubFlag(false);
        setHalfFlag((memory[getHL()] & 0x0F) == 0x00);
        lastCycles = 12;
        break;

    case 0x03: // INC BC Increase Value of BC joint Reg by 1
        setBC(getBC() + 1);
        // INC BC, DE, HL and SP do not touch the Flags
        lastCycles = 8;
        break;

    case 0x13: // INC DE Increase Value of DE joint Reg by 1
        setDE(getDE() + 1);
        lastCycles = 8;
        break;

    case 0x23: // INC HL Increase Value of HL joint Reg by 1
        setHL(getHL() + 1);
        lastCycles = 8;
        break;

    case 0x33: // INC SP Increase Value of SP joint Reg by 1
        SP++;
        lastCycles = 8;
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

    case 0x2D: // DEC L Decrease Value of L Reg by 1
        L--;
        setZeroFlag(L == 0);
        setSubFlag(true);
        setHalfFlag((L & 0x0F) == 0x0F);
        lastCycles = 4;
        break;

    case 0x1D: // DEC E Decrease Value of E Reg by 1
        E--;
        setZeroFlag(E == 0);
        setSubFlag(true);
        setHalfFlag((E & 0x0F) == 0x0F);
        lastCycles = 4;
        break;

    case 0x0D: // DEC C Decrease Value of C Reg by 1
        C--;
        setZeroFlag(C == 0);
        setSubFlag(true);
        setHalfFlag((C & 0x0F) == 0x0F);
        lastCycles = 4;
        break;

    case 0x05: // DEC B Decrease Value of B Reg by 1
        B--;
        setZeroFlag(B == 0);
        setSubFlag(true);
        setHalfFlag((B & 0x0F) == 0x0F);
        lastCycles = 4;
        break;

    case 0x15: // DEC D Decrease Value of D Reg by 1
        D--;
        setZeroFlag(D == 0);
        setSubFlag(true);
        setHalfFlag((D & 0x0F) == 0x0F);
        lastCycles = 4;
        break;

    case 0x25: // DEC H Decrease Value of H Reg by 1
        H--;
        setZeroFlag(H == 0);
        setSubFlag(true);
        setHalfFlag((H & 0x0F) == 0x0F);
        lastCycles = 4;
        break;

    case 0x35: // DEC (HL) Decrease Value at addr in (HL) joint Reg
        memory[getHL()] = memory[getHL()] - 1;
        setZeroFlag(memory[getHL()] == 0);
        setSubFlag(true);
        setHalfFlag((memory[getHL()] & 0x0F) == 0x0F);
        lastCycles = 12;
        break;

    case 0x0B: // DEC BC Decrease Value of BC joint Reg by 1
        setBC(getBC() - 1);
        lastCycles = 8;
        break;

    case 0x1B: // DEC DE Decrease Value of DE joint Reg by 1
        setDE(getDE() - 1);
        lastCycles = 8;
        break;

    case 0x2B: // DEC HL Decrease Value of HL joint Reg by 1
        setHL(getHL() - 1);
        lastCycles = 8;
        break;

    case 0x3B: // DEC SP Decrease Value of SP by 1
        SP = SP - 1;
        lastCycles = 8;
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

        // ------------- ADD r,r ----------------

    case 0x80: // ADD A,B Adds B Reg Val to A Reg
    {
        setHalfFlag(((A & 0x0F) + (B & 0x0F)) > 0x0F);
        setCarryFlag((A + B) > 0xFF);
        A = A + B;
        setZeroFlag(A == 0);
        setSubFlag(false);
        lastCycles = 4;
        break;
    }

    case 0x81: // ADD A,C
    {
        setHalfFlag(((A & 0x0F) + (C & 0x0F)) > 0x0F);
        setCarryFlag((A + C) > 0xFF);
        A = A + C;
        setZeroFlag(A == 0);
        setSubFlag(false);
        lastCycles = 4;
        break;
    }

    case 0x82: // ADD A,D
    {
        setHalfFlag(((A & 0x0F) + (D & 0x0F)) > 0x0F);
        setCarryFlag((A + D) > 0xFF);
        A = A + D;
        setZeroFlag(A == 0);
        setSubFlag(false);
        lastCycles = 4;
        break;
    }

    case 0x83: // ADD A,E
    {
        setHalfFlag(((A & 0x0F) + (E & 0x0F)) > 0x0F);
        setCarryFlag((A + E) > 0xFF);
        A = A + E;
        setZeroFlag(A == 0);
        setSubFlag(false);
        lastCycles = 4;
        break;
    }

    case 0x84: // ADD A,H
    {
        setHalfFlag(((A & 0x0F) + (H & 0x0F)) > 0x0F);
        setCarryFlag((A + H) > 0xFF);
        A = A + H;
        setZeroFlag(A == 0);
        setSubFlag(false);
        lastCycles = 4;
        break;
    }

    case 0x85: // ADD A,L
    {
        setHalfFlag(((A & 0x0F) + (L & 0x0F)) > 0x0F);
        setCarryFlag((A + L) > 0xFF);
        A = A + L;
        setZeroFlag(A == 0);
        setSubFlag(false);
        lastCycles = 4;
        break;
    }

    case 0x86: // ADD A,(HL) <- Brackets in this case mean that the Op takes the value stored at addr of HL into A Reg
    {
        uint8_t val = memory[getHL()];
        setHalfFlag(((A & 0x0F) + (val & 0x0F)) > 0x0F);
        setCarryFlag((A + val) > 0xFF);
        A = A + val;
        setZeroFlag(A == 0);
        setSubFlag(false);
        lastCycles = 8;
        break;
    }

    case 0x87: // ADD A,A
    {
        setHalfFlag(((A & 0x0F) + (A & 0x0F)) > 0x0F);
        setCarryFlag((A + A) > 0xFF);
        A = A + A;
        setZeroFlag(A == 0);
        setSubFlag(false);
        lastCycles = 4;
        break;
    }

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

    case 0x19: // ADD HL,DE Adds DE to HL
    {
        setHalfFlag(((getHL() & 0x0FFF) + (getDE() & 0x0FFF)) > 0x0FFF);
        setCarryFlag((getHL() + getDE()) > 0xFFFF);
        setHL(getHL() + getDE());
        setSubFlag(false);
        lastCycles = 8;
        break;
    }

    case 0x29: // ADD HL,HL Adds HL to HL
    {
        setHalfFlag(((getHL() & 0x0FFF) + (getHL() & 0x0FFF)) > 0x0FFF);
        setCarryFlag((getHL() + getHL()) > 0xFFFF);
        setHL(getHL() + getHL());
        setSubFlag(false);
        lastCycles = 8;
        break;
    }

    case 0x39: // ADD HL,SP Adds SP to HL
    {
        setHalfFlag(((getHL() & 0x0FFF) + (SP & 0x0FFF)) > 0x0FFF);
        setCarryFlag((getHL() + SP) > 0xFFFF);
        setHL(getHL() + SP);
        setSubFlag(false);
        lastCycles = 8;
        break;
    }

        // ---------- ADD SP,i8 ----------------

    case 0xE8: // ADD SP,i8 / ADD SP,e8, ADD SP,r8 Adds a signed 8 bit integer to SP
    {
        uint8_t unsigned_val = memory[PC];        // unsigned_val reads the Byte as unsigned int
        int8_t signed_val = (int8_t)unsigned_val; // unsigned_val gets reinterpreted as a signed int, this CPU Function needs both

        setHalfFlag(((SP & 0x0F) + (unsigned_val & 0x0F)) > 0x0F); // GB CPU uses the unsigned variant of the following Byte to calculate the flags
        setCarryFlag(((SP & 0xFF) + unsigned_val) > 0xFF);
        setZeroFlag(false);
        setSubFlag(false);

        SP = SP + signed_val;

        // Signed ints:
        // 0x00-0x7F means 0 to 127
        // 0x80-0xFF means -128 to -1 (0xFF = -1, 0xFE = -2, 0x80 = -128)

        PC++;
        lastCycles = 16;
        break;
    }

        // ------------- SUB ----------------

    case 0xD6: // SUB A,u8 Subtracts u8 Val from A Reg
    {
        setHalfFlag((A & 0x0F) < (memory[PC] & 0x0F));
        setCarryFlag(A < memory[PC]);
        A = A - memory[PC];
        setZeroFlag(A == 0);
        setSubFlag(true);
        PC++;
        lastCycles = 8;
        break;
    }

        // ------------- SUB r,r ----------------

    case 0x90: // SUB A,B
    {
        setHalfFlag((A & 0x0F) < (B & 0x0F)); // Half Borrow checks if the lower Nibble (4 bit) of B is bigger than A, if that is true you cannto do lower - lower without "borrowing" 1 form the higher nibble -> Half Borrow is true
        setCarryFlag(A < B);                  // Borrow funcions like Half Borrow but for the whole Byte -> if B > A then A goes under Zero and loops back around (0 -> 255)
        A = A - B;
        setZeroFlag(A == 0);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

    case 0x91: // SUB A,C
    {
        setHalfFlag((A & 0x0F) < (C & 0x0F));
        setCarryFlag(A < C);
        A = A - C;
        setZeroFlag(A == 0);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

    case 0x92: // SUB A,D
    {
        setHalfFlag((A & 0x0F) < (D & 0x0F));
        setCarryFlag(A < D);
        A = A - D;
        setZeroFlag(A == 0);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

    case 0x93: // SUB A,E
    {
        setHalfFlag((A & 0x0F) < (E & 0x0F));
        setCarryFlag(A < E);
        A = A - E;
        setZeroFlag(A == 0);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

    case 0x94: // SUB A,H
    {
        setHalfFlag((A & 0x0F) < (H & 0x0F));
        setCarryFlag(A < H);
        A = A - H;
        setZeroFlag(A == 0);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

    case 0x95: // SUB A,L
    {
        setHalfFlag((A & 0x0F) < (L & 0x0F));
        setCarryFlag(A < L);
        A = A - L;
        setZeroFlag(A == 0);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

    case 0x96: // SUB A,(HL) <- Brackets in this case mean that the Op takes the value stored at addr of HL (A - Value at HL addr)
    {
        uint8_t val = memory[getHL()];
        setHalfFlag((A & 0x0F) < (val & 0x0F));
        setCarryFlag(A < val);
        A = A - val;
        setZeroFlag(A == 0);
        setSubFlag(true);
        lastCycles = 8;
        break;
    }

    case 0x97: // SUB A,A
    {
        setHalfFlag((A & 0x0F) < (A & 0x0F));
        setCarryFlag(A < A);
        A = A - A;
        setZeroFlag(A == 0);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

        // ---------- LD HL,SP+i8 ----------------

    case 0xF8: // LD HL,SP+i8 / LD HL,SP+e8, LD HL,SP+r8 Adds a signed 8 bit integer to SP and then saves that Value into HL
    {

        // IMPORTANT: SP does not get changed in this Operation, the addition only happens internally to store the Value in HL!!!

        uint8_t unsigned_val = memory[PC];        // unsigned_val reads the Byte as unsigned int
        int8_t signed_val = (int8_t)unsigned_val; // unsigned_val gets reinterpreted as a signed int, this CPU Function needs both

        setHalfFlag(((SP & 0x0F) + (unsigned_val & 0x0F)) > 0x0F); // GB CPU uses the unsigned variant of the following Byte to calculate the flags
        setCarryFlag(((SP & 0xFF) + unsigned_val) > 0xFF);
        setZeroFlag(false);
        setSubFlag(false);

        uint16_t sum = SP + signed_val;
        setHL(sum);

        // Signed ints:
        // 0x00-0x7F means 0 to 127
        // 0x80-0xFF means -128 to -1 (0xFF = -1, 0xFE = -2, 0x80 = -128)

        PC++;
        lastCycles = 12;
        break;
    }

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

        // ------------- PUSH ----------------

    case 0xC5: // PUSH BC Pushes BC Joint Reg into Stack
    {
        SP--;
        memory[SP] = B;
        SP--;
        memory[SP] = C;
        lastCycles = 16;
        break;
    }

    case 0xD5: // PUSH DE Pushes DE Joint Reg into Stack
    {
        SP--;
        memory[SP] = D;
        SP--;
        memory[SP] = E;
        lastCycles = 16;
        break;
    }

    case 0xE5: // PUSH HL Pushes HL Joint Reg into Stack
    {
        SP--;
        memory[SP] = H;
        SP--;
        memory[SP] = L;
        lastCycles = 16;
        break;
    }

    case 0xF5: // PUSH AF Pushes AF Joint Reg into Stack
    {
        SP--;
        memory[SP] = A;
        SP--;
        memory[SP] = F;
        lastCycles = 16;
        break;
    }

        // ------------- POP ----------------

    case 0xC1: // POP BC POPs from Stack into Joint Reg BC
    {
        C = memory[SP];
        SP++;
        B = memory[SP];
        SP++;
        lastCycles = 12;
        break;
    }

    case 0xD1: // POP DE POPs from Stack into Joint Reg DE
    {
        E = memory[SP];
        SP++;
        D = memory[SP];
        SP++;
        lastCycles = 12;
        break;
    }

    case 0xE1: // POP HL POPs from Stack into Joint Reg HL
    {
        L = memory[SP];
        SP++;
        H = memory[SP];
        SP++;
        lastCycles = 12;
        break;
    }

    case 0xF1: // POP AF POPs from Stack into Joint Reg AF
    {
        F = memory[SP] & 0xF0; // Lower Nibble (4 bits) of F Reg is always 0
        SP++;
        A = memory[SP];
        SP++;
        lastCycles = 12;
        break;
    }

        // ------------- CALL ----------------

    case 0xCD: // CALL u16 Jumps Program to 16bit addr, saves the jumpoff location into Stack
    {
        uint16_t addr = 0;
        addr = memory[PC];
        PC++;
        addr = addr | (memory[PC] << 8);
        PC++;

        SP--;
        memory[SP] = (PC & 0xFF00) >> 8;
        SP--;
        memory[SP] = (PC & 0x00FF);

        PC = addr;

        lastCycles = 24;
        break;
    }

    case 0xCC: // CALL Z,u16 Jumps Program to 16bit addr only if Z-Flag is set, saves the jumpoff location into Stack
    {
        uint16_t addr = 0;
        addr = memory[PC];
        PC++;
        addr = addr | (memory[PC] << 8);
        PC++;

        if (getZeroFlag() == true)
        {
            SP--;
            memory[SP] = (PC & 0xFF00) >> 8;
            SP--;
            memory[SP] = (PC & 0x00FF);

            PC = addr;
            lastCycles = 24;
        }
        else
        {
            lastCycles = 12;
        }

        break;
    }

    case 0xDC: // CALL C,u16 Jumps Program to 16bit addr only if C-Flag is set, saves the jumpoff location into Stack
    {
        uint16_t addr = 0;
        addr = memory[PC];
        PC++;
        addr = addr | (memory[PC] << 8);
        PC++;

        if (getCarryFlag() == true)
        {
            SP--;
            memory[SP] = (PC & 0xFF00) >> 8;
            SP--;
            memory[SP] = (PC & 0x00FF);

            PC = addr;
            lastCycles = 24;
        }
        else
        {
            lastCycles = 12;
        }

        break;
    }

    case 0xC4: // CALL NZ,u16 Jumps Program to 16bit addr only if Z-Flag is not set, saves the jumpoff location into Stack
    {
        uint16_t addr = 0;
        addr = memory[PC];
        PC++;
        addr = addr | (memory[PC] << 8);
        PC++;

        if (getZeroFlag() == false)
        {
            SP--;
            memory[SP] = (PC & 0xFF00) >> 8;
            SP--;
            memory[SP] = (PC & 0x00FF);

            PC = addr;
            lastCycles = 24;
        }
        else
        {
            lastCycles = 12;
        }

        break;
    }

    case 0xD4: // CALL NC,u16 Jumps Program to 16bit addr only if C-Flag is not set, saves the jumpoff location into Stack
    {
        uint16_t addr = 0;
        addr = memory[PC];
        PC++;
        addr = addr | (memory[PC] << 8);
        PC++;

        if (getCarryFlag() == false)
        {
            SP--;
            memory[SP] = (PC & 0xFF00) >> 8;
            SP--;
            memory[SP] = (PC & 0x00FF);

            PC = addr;
            lastCycles = 24;
        }
        else
        {
            lastCycles = 12;
        }

        break;
    }

        // ------------- RST ----------------

    case 0xC7: // RST 00h Jumps Program to Hex addr 0x0000, saves PC into Stack
    {
        SP--;
        memory[SP] = (PC & 0xFF00) >> 8;
        SP--;
        memory[SP] = (PC & 0x00FF);

        PC = 0x0000;

        lastCycles = 16;
        break;
    }

    case 0xD7: // RST 10h
    {
        SP--;
        memory[SP] = (PC & 0xFF00) >> 8;
        SP--;
        memory[SP] = (PC & 0x00FF);

        PC = 0x0010;

        lastCycles = 16;
        break;
    }

    case 0xE7: // RST 20h
    {
        SP--;
        memory[SP] = (PC & 0xFF00) >> 8;
        SP--;
        memory[SP] = (PC & 0x00FF);

        PC = 0x0020;

        lastCycles = 16;
        break;
    }

    case 0xF7: // RST 30h
    {
        SP--;
        memory[SP] = (PC & 0xFF00) >> 8;
        SP--;
        memory[SP] = (PC & 0x00FF);

        PC = 0x0030;

        lastCycles = 16;
        break;
    }

    case 0xCF: // RST 08h
    {
        SP--;
        memory[SP] = (PC & 0xFF00) >> 8;
        SP--;
        memory[SP] = (PC & 0x00FF);

        PC = 0x0008;

        lastCycles = 16;
        break;
    }

    case 0xDF: // RST 18h
    {
        SP--;
        memory[SP] = (PC & 0xFF00) >> 8;
        SP--;
        memory[SP] = (PC & 0x00FF);

        PC = 0x0018;

        lastCycles = 16;
        break;
    }

    case 0xEF: // RST 28h
    {
        SP--;
        memory[SP] = (PC & 0xFF00) >> 8;
        SP--;
        memory[SP] = (PC & 0x00FF);

        PC = 0x0028;

        lastCycles = 16;
        break;
    }

    case 0xFF: // RST 38h
    {
        SP--;
        memory[SP] = (PC & 0xFF00) >> 8;
        SP--;
        memory[SP] = (PC & 0x00FF);

        PC = 0x0038;

        lastCycles = 16;
        break;
    }

        // ------------- RET ----------------

    case 0xC9: // RET ... POPs Return Adress from Stack and jumps back
    {
        uint16_t addr = 0;

        addr = memory[SP];
        SP++;
        addr = addr | (memory[SP] << 8);
        SP++;

        PC = addr;

        lastCycles = 16;
        break;
    }

    case 0xC8: // RET Z ... POPs Return Adress from Stack and jumps back only if Z-Flag true
    {
        if (getZeroFlag() == true)
        {
            uint16_t addr = 0;

            addr = memory[SP];
            SP++;
            addr = addr | (memory[SP] << 8);
            SP++;

            PC = addr;
            lastCycles = 20;
        }
        else
        {
            lastCycles = 8;
        }
        break;
    }

    case 0xD8: // RET C ... POPs Return Adress from Stack and jumps back only if C-Flag true
    {
        if (getCarryFlag() == true)
        {
            uint16_t addr = 0;

            addr = memory[SP];
            SP++;
            addr = addr | (memory[SP] << 8);
            SP++;

            PC = addr;
            lastCycles = 20;
        }
        else
        {
            lastCycles = 8;
        }
        break;
    }

    case 0xC0: // RET NZ ... POPs Return Adress from Stack and jumps back only if Z-Flag false
    {
        if (getZeroFlag() == false)
        {
            uint16_t addr = 0;

            addr = memory[SP];
            SP++;
            addr = addr | (memory[SP] << 8);
            SP++;

            PC = addr;
            lastCycles = 20;
        }
        else
        {
            lastCycles = 8;
        }
        break;
    }

    case 0xD0: // RET NC ... POPs Return Adress from Stack and jumps back only if C-Flag false
    {
        if (getCarryFlag() == false)
        {
            uint16_t addr = 0;

            addr = memory[SP];
            SP++;
            addr = addr | (memory[SP] << 8);
            SP++;

            PC = addr;
            lastCycles = 20;
        }
        else
        {
            lastCycles = 8;
        }
        break;
    }

    case 0xD9: // RETI ... Return from interrupt handler POPs Return Adress from Stack and jumps back and enables interrupts by setting IME to true
    {
        uint16_t addr = 0;

        addr = memory[SP];
        SP++;
        addr = addr | (memory[SP] << 8);
        SP++;

        PC = addr;
        ime = true;

        lastCycles = 16;
        break;
    }

        // ------------- JP ----------------

    case 0xC3: // JP u16 Jumps PC unconditionally to 16bit Address
    {
        uint16_t addr = 0;
        addr = memory[PC];
        PC++;
        addr = addr | (memory[PC] << 8);
        PC++;
        PC = addr;

        lastCycles = 16;
        break;
    }

    case 0xCA: // JP Z,u16 Jumps PC to 16bit Address only if Zero Flag is True
    {
        uint16_t addr = 0;
        addr = memory[PC];
        PC++;
        addr = addr | (memory[PC] << 8);
        PC++;

        if (getZeroFlag() == true)
        {
            PC = addr;
            lastCycles = 16;
        }
        else
        {
            lastCycles = 12;
        }

        break;
    }

    case 0xDA: // JP C,u16 Jumps PC to 16bit Address only if Carry Flag is True
    {
        uint16_t addr = 0;
        addr = memory[PC];
        PC++;
        addr = addr | (memory[PC] << 8);
        PC++;

        if (getCarryFlag() == true)
        {
            PC = addr;
            lastCycles = 16;
        }
        else
        {
            lastCycles = 12;
        }

        break;
    }

    case 0xC2: // JP NZ,u16 Jumps PC to 16bit Address only if Zero Flag is False
    {
        uint16_t addr = 0;
        addr = memory[PC];
        PC++;
        addr = addr | (memory[PC] << 8);
        PC++;

        if (getZeroFlag() == false)
        {
            PC = addr;
            lastCycles = 16;
        }
        else
        {
            lastCycles = 12;
        }

        break;
    }

    case 0xD2: // JP NC,u16 Jumps PC to 16bit Address only if Carry Flag is False
    {
        uint16_t addr = 0;
        addr = memory[PC];
        PC++;
        addr = addr | (memory[PC] << 8);
        PC++;

        if (getCarryFlag() == false)
        {
            PC = addr;
            lastCycles = 16;
        }
        else
        {
            lastCycles = 12;
        }

        break;
    }

    case 0xE9: // JP HL Jumps PC to 16bit Address in HL
    {
        PC = getHL();
        lastCycles = 4;
        break;
    }

        // ------------- JR ----------------

    case 0x18: // JR i8 Jumps PC unconditionally relatively based on signed 8bit Val
    {
        int8_t val = memory[PC];
        PC++; // The Op only jumps after PC has been increased normally
        PC = PC + val;
        lastCycles = 12;
        break;
    }

    case 0x28: // JR Z,i8 Jumps PC only if Z-Flag is true relatively based on signed 8bit Val
    {
        int8_t val = memory[PC];
        PC++;

        if (getZeroFlag() == true)
        {
            PC = PC + val;
            lastCycles = 12;
        }
        else
        {
            lastCycles = 8;
        }

        break;
    }

    case 0x38: // JR C,i8
    {
        int8_t val = memory[PC];
        PC++;

        if (getCarryFlag() == true)
        {
            PC = PC + val;
            lastCycles = 12;
        }
        else
        {
            lastCycles = 8;
        }

        break;
    }

    case 0x20: // JR NZ,i8
    {
        int8_t val = memory[PC];
        PC++;

        if (getZeroFlag() == false)
        {
            PC = PC + val;
            lastCycles = 12;
        }
        else
        {
            lastCycles = 8;
        }

        break;
    }

    case 0x30: // JR NC,i8
    {
        int8_t val = memory[PC];
        PC++;

        if (getCarryFlag() == false)
        {
            PC = PC + val;
            lastCycles = 12;
        }
        else
        {
            lastCycles = 8;
        }

        break;
    }

        // ------------- RLCA ----------------

    case 0x07: // RLCA Rotate Left Circular Accumulator (A Reg)... Shifts A Reg left by 1 and then takes the bit that falls out on the left and put is back on the right
    {
        uint8_t carry = (A & 0x80) >> 7;
        A = (A << 1) | carry;

        setZeroFlag(false);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(carry);

        lastCycles = 4;
        break;
    }

        // ------------- RLA ----------------

    case 0x17: // RLA Rotate Left Accumulator (A Reg)... Shifts A Reg left by 1 and then takes the Carry-Flag bit and put is back on the right, the bit on the left that falls out gets put into the carry flag
    {
        uint8_t carry = (A & 0x80) >> 7;
        A = (A << 1) | getCarryFlag();

        setZeroFlag(false);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(carry);

        lastCycles = 4;
        break;
    }

        // ------------- RRCA ----------------

    case 0x0F: // RRCA Rotate Right Circular Accumulator (A Reg)... Shifts A Reg right by 1 and then takes the bit that falls out on the right and put is back on the left
    {
        uint8_t carry = (A & 0x01);
        A = (A >> 1) | (carry << 7); // carry only gets shifted for this Operation not in general, so the setCarryFlag(carry) gets a clear 0 or 1

        setZeroFlag(false);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(carry);

        lastCycles = 4;
        break;
    }

        // ------------- RRA ----------------

    case 0x1F: // RRA Rotate Right Accumulator (A Reg)... Shifts A Reg right by 1 and then takes the Carry-Flag bit and puts it back on the left, the bit on the right that falls out gets put into the carry flag
    {
        uint8_t carry = (A & 0x01);
        A = (A >> 1) | (getCarryFlag() << 7); // Unlike RRCA carry is only needed for setCarryFlag() and not merged with A again, instead getCarryFlag()'s Value needs to be shifted to the left (just in this Operation not in general) to be merged correctly

        setZeroFlag(false);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(carry);

        lastCycles = 4;
        break;
    }

        // ------------- AND ----------------

    case 0xE6: // AND A,u8 AND Operation with A Reg and u8 Val
    {
        A = A & memory[PC];

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(true);
        setCarryFlag(false);
        PC++;
        lastCycles = 8;
        break;
    }

        // ------------- AND r,r ----------------

    case 0xA0: // AND A,B
    {
        A = A & B;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(true);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xA1: // AND A,C
    {
        A = A & C;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(true);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xA2: // AND A,D
    {
        A = A & D;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(true);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xA3: // AND A,E
    {
        A = A & E;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(true);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xA4: // AND A,H
    {
        A = A & H;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(true);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xA5: // AND A,L
    {
        A = A & L;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(true);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xA6: // AND A,(HL) <- Brackets mean that the Op uses the Val at the addr inside HL joint Reg
    {
        A = A & memory[getHL()];

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(true);
        setCarryFlag(false);

        lastCycles = 8;
        break;
    }

    case 0xA7: // AND A,A
    {
        A = A & A;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(true);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

        // ------------- OR ----------------

    case 0xF6: // OR A,u8 OR Operation with A Reg and u8 Val
    {
        A = A | memory[PC];

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);
        PC++;
        lastCycles = 8;
        break;
    }

        // ------------- OR r,r ----------------

    case 0xB0: // OR A,B
    {
        A = A | B;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xB1: // OR A,C
    {
        A = A | C;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xB2: // OR A,D
    {
        A = A | D;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xB3: // OR A,E
    {
        A = A | E;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xB4: // OR A,H
    {
        A = A | H;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xB5: // OR A,L
    {
        A = A | L;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xB6: // OR A,(HL) <- Brackets mean that the Op uses the Val at the addr inside HL joint Reg
    {
        A = A | memory[getHL()];

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 8;
        break;
    }

    case 0xB7: // OR A,A
    {
        A = A | A;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

        // ------------- ADC ----------------

    case 0xCE: // ADC A,u8 Add with Carry A Reg + u8 Val
    {
        uint16_t result = A + memory[PC] + getCarryFlag();

        setHalfFlag(((A & 0x0F) + (memory[PC] & 0x0F) + getCarryFlag()) > 0x0F);

        A = (uint8_t)result; // casting to 8bit for A Reg

        setCarryFlag(result > 0xFF);
        setZeroFlag(A == 0);
        setSubFlag(false);

        PC++;
        lastCycles = 8;
        break;
    }

        // ------------- ADC r,r ----------------

    case 0x88: // ADC A,B Add with Carry
    {
        uint16_t result = A + B + getCarryFlag();

        setHalfFlag(((A & 0x0F) + (B & 0x0F) + getCarryFlag()) > 0x0F);

        A = (uint8_t)result;

        setCarryFlag(result > 0xFF);
        setZeroFlag(A == 0);
        setSubFlag(false);

        lastCycles = 4;
        break;
    }

    case 0x89: // ADC A,C
    {
        uint16_t result = A + C + getCarryFlag();

        setHalfFlag(((A & 0x0F) + (C & 0x0F) + getCarryFlag()) > 0x0F);

        A = (uint8_t)result;

        setCarryFlag(result > 0xFF);
        setZeroFlag(A == 0);
        setSubFlag(false);

        lastCycles = 4;
        break;
    }

    case 0x8A: // ADC A,D
    {
        uint16_t result = A + D + getCarryFlag();

        setHalfFlag(((A & 0x0F) + (D & 0x0F) + getCarryFlag()) > 0x0F);

        A = (uint8_t)result;

        setCarryFlag(result > 0xFF);
        setZeroFlag(A == 0);
        setSubFlag(false);

        lastCycles = 4;
        break;
    }

    case 0x8B: // ADC A,E
    {
        uint16_t result = A + E + getCarryFlag();

        setHalfFlag(((A & 0x0F) + (E & 0x0F) + getCarryFlag()) > 0x0F);

        A = (uint8_t)result;

        setCarryFlag(result > 0xFF);
        setZeroFlag(A == 0);
        setSubFlag(false);

        lastCycles = 4;
        break;
    }

    case 0x8C: // ADC A,H
    {
        uint16_t result = A + H + getCarryFlag();

        setHalfFlag(((A & 0x0F) + (H & 0x0F) + getCarryFlag()) > 0x0F);

        A = (uint8_t)result;

        setCarryFlag(result > 0xFF);
        setZeroFlag(A == 0);
        setSubFlag(false);

        lastCycles = 4;
        break;
    }

    case 0x8D: // ADC A,L
    {
        uint16_t result = A + L + getCarryFlag();

        setHalfFlag(((A & 0x0F) + (L & 0x0F) + getCarryFlag()) > 0x0F);

        A = (uint8_t)result;

        setCarryFlag(result > 0xFF);
        setZeroFlag(A == 0);
        setSubFlag(false);

        lastCycles = 4;
        break;
    }

    case 0x8E: // ADC A,(HL) <- Brackets mean that Val at addr in HL is used by the Op
    {
        uint16_t result = A + memory[getHL()] + getCarryFlag();

        setHalfFlag(((A & 0x0F) + (memory[getHL()] & 0x0F) + getCarryFlag()) > 0x0F);

        A = (uint8_t)result;

        setCarryFlag(result > 0xFF);
        setZeroFlag(A == 0);
        setSubFlag(false);

        lastCycles = 8;
        break;
    }

    case 0x8F: // ADC A,A
    {
        uint16_t result = A + A + getCarryFlag();

        setHalfFlag(((A & 0x0F) + (A & 0x0F) + getCarryFlag()) > 0x0F);

        A = (uint8_t)result;

        setCarryFlag(result > 0xFF);
        setZeroFlag(A == 0);
        setSubFlag(false);

        lastCycles = 4;
        break;
    }

        // ------------- SBC ----------------

    case 0xDE: // SBC A,u8 Subtract with Carry (Borrow) A Reg - u8 Val
    {
        uint8_t old_carry = getCarryFlag();
        setHalfFlag((A & 0x0F) < ((memory[PC] & 0x0F) + old_carry));
        setCarryFlag(A < (memory[PC] + old_carry));

        A = A - memory[PC] - old_carry;

        setZeroFlag(A == 0);
        setSubFlag(true);

        PC++;
        lastCycles = 8;
        break;
    }

        // ------------- SBC r,r ----------------

    case 0x98: // SBC A,B Subtract with Carry (Borrow)
    {
        uint8_t old_carry = getCarryFlag();
        setHalfFlag((A & 0x0F) < ((B & 0x0F) + old_carry));
        setCarryFlag(A < (B + old_carry));

        A = A - B - old_carry;

        setZeroFlag(A == 0);
        setSubFlag(true);

        lastCycles = 4;
        break;
    }

    case 0x99: // SBC A,C
    {
        uint8_t old_carry = getCarryFlag();
        setHalfFlag((A & 0x0F) < ((C & 0x0F) + old_carry));
        setCarryFlag(A < (C + old_carry));

        A = A - C - old_carry;

        setZeroFlag(A == 0);
        setSubFlag(true);

        lastCycles = 4;
        break;
    }

    case 0x9A: // SBC A,D
    {
        uint8_t old_carry = getCarryFlag();
        setHalfFlag((A & 0x0F) < ((D & 0x0F) + old_carry));
        setCarryFlag(A < (D + old_carry));

        A = A - D - old_carry;

        setZeroFlag(A == 0);
        setSubFlag(true);

        lastCycles = 4;
        break;
    }

    case 0x9B: // SBC A,E
    {
        uint8_t old_carry = getCarryFlag();
        setHalfFlag((A & 0x0F) < ((E & 0x0F) + old_carry));
        setCarryFlag(A < (E + old_carry));

        A = A - E - old_carry;

        setZeroFlag(A == 0);
        setSubFlag(true);

        lastCycles = 4;
        break;
    }

    case 0x9C: // SBC A,H
    {
        uint8_t old_carry = getCarryFlag();
        setHalfFlag((A & 0x0F) < ((H & 0x0F) + old_carry));
        setCarryFlag(A < (H + old_carry));

        A = A - H - old_carry;

        setZeroFlag(A == 0);
        setSubFlag(true);

        lastCycles = 4;
        break;
    }

    case 0x9D: // SBC A,L
    {
        uint8_t old_carry = getCarryFlag();
        setHalfFlag((A & 0x0F) < ((L & 0x0F) + old_carry));
        setCarryFlag(A < (L + old_carry));

        A = A - L - old_carry;

        setZeroFlag(A == 0);
        setSubFlag(true);

        lastCycles = 4;
        break;
    }

    case 0x9E: // SBC A,(HL) <- Brackets: Val at addr used
    {
        uint8_t old_carry = getCarryFlag();
        setHalfFlag((A & 0x0F) < ((memory[getHL()] & 0x0F) + old_carry));
        setCarryFlag(A < (memory[getHL()] + old_carry));

        A = A - memory[getHL()] - old_carry;

        setZeroFlag(A == 0);
        setSubFlag(true);

        lastCycles = 8;
        break;
    }

    case 0x9F: // SBC A,A
    {
        uint8_t old_carry = getCarryFlag();
        setHalfFlag((A & 0x0F) < ((A & 0x0F) + old_carry));
        setCarryFlag(A < (A + old_carry));

        A = A - A - old_carry;

        setZeroFlag(A == 0);
        setSubFlag(true);

        lastCycles = 4;
        break;
    }

        // ------------- XOR ----------------

    case 0xEE: // XOR A,u8 XOR Op with A Reg and u8 Val
    {
        A = A ^ memory[PC];

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        PC++;
        lastCycles = 8;
        break;
    }

        // ------------- XOR r,r ----------------

    case 0xA8: // XOR A,B
    {
        A = A ^ B;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xA9: // XOR A,C
    {
        A = A ^ C;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xAA: // XOR A,D
    {
        A = A ^ D;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xAB: // XOR A,E
    {
        A = A ^ E;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xAC: // XOR A,H
    {
        A = A ^ H;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xAD: // XOR A,L
    {
        A = A ^ L;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

    case 0xAE: // XOR A,(HL) <- Brackets: Val at addr used
    {
        A = A ^ memory[getHL()];

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 8;
        break;
    }

    case 0xAF: // XOR A,A
    {
        A = A ^ A;

        setZeroFlag(A == 0);
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(false);

        lastCycles = 4;
        break;
    }

        // ------------- CP ----------------

    case 0xFE: // CP A,u8 Does a A - u8 Operation but discardes the Result and just sets the Flags
    {
        setZeroFlag((A - memory[PC]) == 0);
        setHalfFlag((A & 0x0F) < (memory[PC] & 0x0F));
        setCarryFlag(A < memory[PC]);
        setSubFlag(true);

        PC++;
        lastCycles = 8;
        break;
    }

        // ------------- CP r,r ----------------

    case 0xB8: // CP A,B Compare... Does a A - B Operation but discardes the Result and just sets the Flags
    {
        setZeroFlag((A - B) == 0);
        setHalfFlag((A & 0x0F) < (B & 0x0F));
        setCarryFlag(A < B);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

    case 0xB9: // CP A,C
    {
        setZeroFlag((A - C) == 0);
        setHalfFlag((A & 0x0F) < (C & 0x0F));
        setCarryFlag(A < C);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

    case 0xBA: // CP A,D
    {
        setZeroFlag((A - D) == 0);
        setHalfFlag((A & 0x0F) < (D & 0x0F));
        setCarryFlag(A < D);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

    case 0xBB: // CP A,E
    {
        setZeroFlag((A - E) == 0);
        setHalfFlag((A & 0x0F) < (E & 0x0F));
        setCarryFlag(A < E);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

    case 0xBC: // CP A,H
    {
        setZeroFlag((A - H) == 0);
        setHalfFlag((A & 0x0F) < (H & 0x0F));
        setCarryFlag(A < H);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

    case 0xBD: // CP A,L
    {
        setZeroFlag((A - L) == 0);
        setHalfFlag((A & 0x0F) < (L & 0x0F));
        setCarryFlag(A < L);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

    case 0xBE: // CP A,(HL)
    {
        setZeroFlag((A - memory[getHL()]) == 0);
        setHalfFlag((A & 0x0F) < (memory[getHL()] & 0x0F));
        setCarryFlag(A < memory[getHL()]);
        setSubFlag(true);
        lastCycles = 8;
        break;
    }

    case 0xBF: // CP A,A
    {
        setZeroFlag((A - A) == 0);
        setHalfFlag((A & 0x0F) < (A & 0x0F));
        setCarryFlag(A < A);
        setSubFlag(true);
        lastCycles = 4;
        break;
    }

        // ------------- CPL ----------------

    case 0x2F: // CPL Complement accumulator Flips all Bits in A Reg and sets Sub (N) and Half Flags
    {
        A = ~A; // ~ flips all bits 0 -> 1, 1 -> 0
        setSubFlag(true);
        setHalfFlag(true);
        lastCycles = 4;
        break;
    }

        // ------------- CCF ----------------

    case 0x3F: // CCF Complement carry flag Flips the carry flag, and clears the Sub (N) and H Flags
    {
        setCarryFlag(!getCarryFlag());
        setSubFlag(false);
        setHalfFlag(false);
        lastCycles = 4;
        break;
    }

        // ------------- SCF ----------------

    case 0x37: // SCF Set carry flag Sets the carry flag, and clears the N and H Flags
    {
        setCarryFlag(true);
        setSubFlag(false);
        setHalfFlag(false);
        lastCycles = 4;
        break;
    }

        // ------------- CB ----------------

    case 0xCB: // CB PREFIX
    {
        stepCB(memory);
        break;
    }

    default:
        printf("Unimplemented opcode 0x%02X at PC=0x%04X\n", opcode, PC - 1);
        lastCycles = 4; // TODO... just for testing, not real GB behaviour
        break;
    }
}

void cpu::stepCB(std::array<uint8_t, 65536> &memory) // TODOC
{
    // This is how the Byte after CB looks: xx yyy zzz
    // xx (group) selects the Operation Group (0 (00) = rotate/shift, 1 (01) = BIT, 2 (10) = RES, 3 (11) = SET)
    // yyy (selection) selects Operation in Op Group 0 (RLC, RRC, RL, RR, SLA, SRA, SWAP, SRL) or selects the wanted bit of the Register during the BIT, RES or SET Operations
    // zzz (reg) selects the Register

    uint8_t cb_opcode = memory[PC];
    PC++; // unlike shorter Operation I am increasing PC now so I don't forget it later

    uint8_t reg = cb_opcode & 0x07;              // keep only the lowest 3 bits
    uint8_t selection = (cb_opcode >> 3) & 0x07; // shift the middle bits down, then keep 3 bits
    uint8_t group = cb_opcode >> 6;              // shift down to only keep the 2 highest bits

    uint8_t value = readReg(reg, memory);

    switch (group)
    {
    case 0: // rotate/shift
    {
        uint8_t result = 0;
        uint8_t carry = 0;

        switch (selection)
        {
        case 0: // RLC (like RLCA 0x07)
            carry = (value & 0x80) >> 7;
            result = (value << 1) | carry;
            break;

        case 1: // RRC (like RRCA 0x0F)
            carry = (value & 0x01);
            result = (value >> 1) | (carry << 7);
            break;

        case 2: // RL (like RLA 0x17)
            carry = (value & 0x80) >> 7;
            result = (value << 1) | getCarryFlag();
            break;

        case 3: // RR (like RRA 0x1F)
            carry = (value & 0x01);
            result = (value >> 1) | (getCarryFlag() << 7);
            break;

        case 4:                     // SLA (Shift Left Arithmetic) Shift all bits in Reg left, bit 7 (leftmost) that falls out goes into Carry Flag and bit 0 (rightmost) gets filled with 0
            carry = (value & 0x80) >> 7; // 7th bit gets masked (10000000), shifted to the right (00000001) and put into carry
            result = (value << 1); // By shifting value 1 to the left, the rightmost bit (bit 0) automatically turns 0 (reset)
            break;

        case 5:                     // SRA (Shift Right Arithmetic) Shift all bits in Reg right, bit 0 (rightmost) that falls out goes into Carry Flag
            carry = (value & 0x01); // bit 0 gets masked (00000001) and put into carry
            result = (value >> 1);
            result = result | (value & 0x80); // IMPORTANT: unlike SLA here the last bit in the chain, here bit 7 (from the other side of the bit that falls away), keeps it's original value from before the shift and does not get reset to 0
            break;

        case 6: // SWAP Lower and Higher Nibble (4 bits) get switched
            result = ((value & 0x0F) << 4) | ((value & 0xF0) >> 4);
            carry = 0; // Per definition during SWAP carry must be cleared
            break;

        case 7:                             // SRL  (Shift Right Logical) Reverse verion or SLA
            carry = (value & 0x01);         // 0 bit gets masked (00000001) and put into carry
            result = (value >> 1); // IMPORTANT: unlike SRA here the 7th bit (left) gets reset to 0
            break;

            // Logically you would think that SRA is the reverse version of SLA but that is not true, SRL is the reverse version or SLA
            // SRA has a slight diffrence in how it handles the leftmost bit
        }

        setZeroFlag(result == 0); // unlike RLCA, Z is calculated normally here
        setSubFlag(false);
        setHalfFlag(false);
        setCarryFlag(carry);

        writeReg(reg, result, memory);

        if (reg == 6)
        {
            lastCycles = 16;
        }
        else
        {
            lastCycles = 8;
        }

        break;
    }

    case 1:                                          // BIT selection,reg Looks at the selected bit (indicated by selection val) in selected Register, negates it (complement) and writes that into the zero flag
        setZeroFlag(!((value >> selection) & 0x01)); // This line isolates the selected bit and moves it all the way to the rightmost bit
        setSubFlag(false);
        setHalfFlag(true);
        // Carry is unmodiefied

        if (reg == 6)
        {
            lastCycles = 12;
        }
        else
        {
            lastCycles = 8;
        }

        break;

    case 2:                                                  // RES selection,reg Clears selected bit in selected Register
        writeReg(reg, value & ~(0x01 << selection), memory); // This line creates a (00000001) mask , moves the 1 by the amout of the selction (eg. 3) to the left to select the correct bit
        // then inverts the mask with ~ -> (11110111) and then uses the & (AND) Operator to clear the selected bit 3 (11011001) & (11110111) -> (11010001)
        // (11011001) is an example Value inside the selected Register

        if (reg == 6)
        {
            lastCycles = 16;
        }
        else
        {
            lastCycles = 8;
        }

        break;

    case 3:                                                 // SET selection,reg Sets selected bit in selected Register
        writeReg(reg, value | (0x01 << selection), memory); // This line creates a (00000001) mask , moves the 1 by the amout of the selction (eg. 3) to the left to select the correct bit
        // unlike RES, SET does not invert and use & but uses the | (OR) Operator to merge the selected bit 3 with the Value in the Register (11010001) | (00001000) -> (11011001)
        // (11011001) is an example Value inside the selected Register

        if (reg == 6)
        {
            lastCycles = 16;
        }
        else
        {
            lastCycles = 8;
        }

        break;

    default:
        printf("Unimplemented opcode 0xCB%02X at PC=0x%04X\n", cb_opcode, PC - 2);
        lastCycles = 4; // TODO... just for testing, not real GB behaviour
        break;
    }
}