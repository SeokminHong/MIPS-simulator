#pragma once

#include <cstdint>

enum class EALU : uint8_t
{
    NONE = 0,
    AND,
    OR,
    NOR,
    ADD, ADDU,
    SUB, SUBU,
    SLT, SLTU,
    SLL,
    SRL, SRA
};

EALU GetALUControl(uint8_t op, uint8_t funct);

static inline uint32_t ALU(EALU control, const uint32_t& arg1, const uint32_t& arg2)
{
    switch (control)
    {
        case EALU::AND:
        {
            return arg1 & arg2;
        }
        case EALU::OR:
        {
            return arg1 | arg2;
        }
        case EALU::NOR:
        {
            return ~(arg1 | arg2);
        }
        case EALU::ADD:
        {
            return arg1 + arg2;
        }
        case EALU::SUB:
        {
            return arg1 - arg2;
        }
        case EALU::SLT:
        {
            return arg1 < arg2;
        }
        case EALU::SLL:
        {
            return arg1 << arg2;
        }
        case EALU::SRL:
        {
            return arg1 >> arg2;
        }
        default:
        {}
    }
    return 0;
}

static inline uint32_t ALU(EALU control, const uint32_t& arg1, const uint32_t& arg2, bool& zero)
{
    uint32_t ret = ALU(control, arg1, arg2);
    zero = ret == 0;
    return ret;
}
