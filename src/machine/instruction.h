#pragma once

#include <string>
#include "register.h"

// Instruction union for parsing the instruction into bit fields.
union inst_t
{
    // Raw instruction (32bit unsigned)
    uint32_t raw;

    // Immdediate addressing model for unsigned int.
    struct
    {
        uint32_t immediate : 16;
        uint32_t rt : 5;
        uint32_t rs : 5;
        uint32_t op : 6;
    } imm_u;

    // Immdediate addressing model for signed int.
    struct
    {
        int32_t immediate : 16;
        uint32_t rt : 5;
        uint32_t rs : 5;
        uint32_t op : 6;
    } imm;
    
    // Register addressing model.
    struct
    {
        uint32_t funct : 6;
        uint32_t shamt : 5;
        uint32_t rd : 5;
        uint32_t rt : 5;
        uint32_t rs : 5;
        uint32_t op : 6;
    } reg;
    
    // Base/PC-relative addressing model (signed int for address).
    struct
    {
        int32_t address : 16;
        uint32_t rt : 5;
        uint32_t rs : 5;
        uint32_t op : 6;
    } base;
    
    // Pseudodirect addressing model.
    struct
    {
        uint32_t address : 26;
        uint32_t op : 6;
    } direct;
};

class Instruction
{
public:
    Instruction(uint32_t inst);

    inline uint32_t GetRawInst() const
    {
        return instruction.raw;
    }

    inline uint8_t GetOpCode() const
    {
        return instruction.direct.op;
    }

    inline inst_t GetInstruction() const
    {
        return instruction;
    }

private:
    inst_t instruction;
};
