#pragma once

#include <string>
#include "register.h"

union SInstruction
{
    uint32_t raw;
    struct {
        uint32_t funct : 6;
        uint32_t shamt : 5;
        uint32_t rd : 5;
        uint32_t rt : 5;
        uint32_t rs : 5;
        uint32_t op : 6;
    } field;
};

class IInstruction
{
public:
    virtual void Execute() = 0;

    static IInstruction* MakeInstance(uint32_t rawInst);

protected:
    IInstruction(uint32_t raw) {}

    SInstruction inst;
};
