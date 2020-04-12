#include "instruction.h"

class IAdd : public Instruction
{
public:
    virtual void Execute()
    {

    }
};

Instruction* Instruction::MakeInstance(uint32_t rawInst)
{
    return nullptr;
}