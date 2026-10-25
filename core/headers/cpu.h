#pragma once
#include <cstdint>
#include <array>

// TODO -> Not finished, Bug etc.
// TODOC -> Needs Documentaion / Documenation not finished
// TOCHECK -> Finished but uncertain if it is correct
// Sub and Negative (N-Flag) are the same

struct cpu
{
    bool ime; // Interrupt Master Enable TODOC

    // 8-bit Registers
    uint8_t A, F; // why in this Layout
    uint8_t B, C;
    uint8_t D, E;
    uint8_t H, L;

    // 16-bit Registers
    uint16_t SP;
    uint16_t PC;

    // Cycle Counter
    uint8_t lastCycles;

    // ------------- Helper Functions ----------------

    // Join Registers
    uint16_t getBC() { return (B << 8) | C; } // Cobine B and C into a 16-bit Register, B gets shifted left to "high byte", result gets combined with C
    uint16_t getDE() { return (D << 8) | E; }
    uint16_t getHL() { return (H << 8) | L; }

    // Reverse Join Registers
    void setBC(uint16_t value)
    {
        B = (value >> 8) & 0xFF;
        C = value & 0xFF;
    }
    void setDE(uint16_t value)
    {
        D = (value >> 8) & 0xFF;
        E = value & 0xFF;
    }
    void setHL(uint16_t value)
    {
        H = (value >> 8) & 0xFF;
        L = value & 0xFF;
    }

    // Flags (Bits of F Register)
    // Z = 7
    // N = 6
    // H = 5
    // C = 4
    // Bits 0 to 3 of F Register are unused in Original GB Hardware

    // ------------- Flag Helpers ----------------

    bool getZeroFlag() { return (F >> 7) & 1; } // Shifts the 7th (from right, last bit bacause we start counting at 0 to 7) Bit to the Right 1011 0000 >> 7... Now it is 0000 0001... Now we see that the 7th bis was 1. Now compare (&) that with 1 and you get 1 (true/Zero-Flag set)
    bool getSubFlag() { return (F >> 6) & 1; }
    bool getHalfFlag() { return (F >> 5) & 1; }
    bool getCarryFlag() { return (F >> 4) & 1; }

    void setZeroFlag(bool set)
    {
        uint8_t mask = 1 << 7; // Creates mask where only Bit 7 is set to 1 (1000 0000)

        if (set) // if set == 1 then it sets the Bit of the Flag to 1
        {
            F = F | mask; // turn only Bit 7 to 1, all other bits stay the same
                          // F (eg. 0110 1000) gets linked with | (OR) to the Mask (1000 0000) -> Resulting F is 1110 1000 (The 7th Bit, the one on the far right, got set to 1)
        }
        else // else it sets the Bit of the Flag to 0
        {
            F = F & (~mask); // turn only Bit 7 to 0, all other bits stay the same
                             // ~ is the Operator for NOT (Inversion)
                             // F (eg. 1110 1000) gets linked with & (AND) to the inverted Mask (0111 1111) -> Resulting F is 0110 1000 (The 7th Bit, the one on the far right, got set to 0)
        }
    }

    void setSubFlag(bool set)
    {
        uint8_t mask = 1 << 6;
        if (set)
        {
            F = F | mask;
        }
        else
        {
            F = F & (~mask);
        }
    }

    void setHalfFlag(bool set)
    {
        uint8_t mask = 1 << 5;
        if (set)
        {
            F = F | mask;
        }
        else
        {
            F = F & (~mask);
        }
    }

    void setCarryFlag(bool set)
    {
        uint8_t mask = 1 << 4;
        if (set)
        {
            F = F | mask;
        }
        else
        {
            F = F & (~mask);
        }
    }

    // ------------- CB Prefix Helpers ---------------- TODOC

    uint8_t readReg(uint8_t reg, std::array<uint8_t, 65536> &memory)
    {
        switch (reg)
        {
        case 0:
            return B;
        case 1:
            return C;
        case 2:
            return D;
        case 3:
            return E;
        case 4:
            return H;
        case 5:
            return L;
        case 6:
            return memory[getHL()];
        case 7:
            return A;
        }
        return A; // just so the complier is happy, never reached
    }

    void writeReg(uint8_t reg, uint8_t value, std::array<uint8_t, 65536> &memory)
    {
        switch (reg)
        {
        case 0:
            B = value;
            break;
        case 1:
            C = value;
            break;
        case 2:
            D = value;
            break;
        case 3:
            E = value;
            break;
        case 4:
            H = value;
            break;
        case 5:
            L = value;
            break;
        case 6:
            memory[getHL()] = value;
            break;
        case 7:
            A = value;
            break;
        }
    }

    void reset();
    void step(std::array<uint8_t, 65536> &memory); // Descripion in cpu.cpp
    void stepCB(std::array<uint8_t, 65536> &memory);
};