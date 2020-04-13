#pragma once

#include <string>
#include <vector>
#include <array>
#include <memory>
#include "register.h"

#define MAX_INSTRUCTION 4096

class Machine
{
    using inst_ptr = std::shared_ptr<class IInstructionBase>;

public:
    Machine();

    virtual ~Machine() {}

    void AddInstruction(const std::string& instruction);

    inline void SetMaxCycle(int newMaxCycle)
    {
        maxCycle = newMaxCycle;
    }

    void SetRegister(ERegister reg, uint32_t regVal);

    void Run();

private:
    void Cycle();

    int pc = 0;

    std::array<uint32_t, REG_MAX> registers;

    std::vector<inst_ptr> instructions;

    int maxCycle = 0;
};