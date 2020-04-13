#include "instruction.h"
#include <cstdio>

#define DECLARE_INSTRUCTION(INSTNAME) \
class I##INSTNAME : public IInstruction \
{ \
public: \
    virtual void Execute() override \
    {} \
    virtual const std::string& GetName() const override { \
        static std::string s{ #INSTNAME }; \
        return s; \
    } \
    I##INSTNAME(uint32_t rawInst) : IInstruction(rawInst) {} \
};

DECLARE_INSTRUCTION(Add);
DECLARE_INSTRUCTION(Subtract);
DECLARE_INSTRUCTION(AddImmediate);

DECLARE_INSTRUCTION(LoadWord);
DECLARE_INSTRUCTION(StoreWord);
DECLARE_INSTRUCTION(LoadHalf);
DECLARE_INSTRUCTION(LoadHalfUnsigned);
DECLARE_INSTRUCTION(StoreHalf);
DECLARE_INSTRUCTION(LoadByte);
DECLARE_INSTRUCTION(LoadByteUnsigned);
DECLARE_INSTRUCTION(StoreByte);
DECLARE_INSTRUCTION(LoadUpperImmediate);

DECLARE_INSTRUCTION(And);
DECLARE_INSTRUCTION(Or);
DECLARE_INSTRUCTION(Nor);
DECLARE_INSTRUCTION(AndImmediate);
DECLARE_INSTRUCTION(OrImmediate);
DECLARE_INSTRUCTION(ShiftLeftLogical);
DECLARE_INSTRUCTION(ShiftRightLogical);

DECLARE_INSTRUCTION(BranchOnEqual);
DECLARE_INSTRUCTION(BranchOnNotEqual);
DECLARE_INSTRUCTION(SetOnLessThan);
DECLARE_INSTRUCTION(SetOnLessThanUnsigned);
DECLARE_INSTRUCTION(SetLessThanImmediate);
DECLARE_INSTRUCTION(SetLessThanImmediateUnsigned);

DECLARE_INSTRUCTION(Jump);
DECLARE_INSTRUCTION(JumpRegister);
DECLARE_INSTRUCTION(JumpAndLink);

DECLARE_INSTRUCTION(Dummy);

IInstruction::IInstruction(uint32_t raw)
{
    inst.raw = raw;
}

IInstruction::inst_ptr IInstruction::MakeInstance(uint32_t rawInst)
{
    inst_t inst;
    inst.raw = rawInst;
    printf("op: %2d, rs: %2d, rt: %2d, rd: %2d, shamt: %2d, funct: %2d\n", 
           inst.field.op, inst.field.rs, inst.field.rt, inst.field.rd, inst.field.shamt, inst.field.funct);

    if (inst.field.op == 0)
    {
        return std::make_shared<IAdd>(rawInst);
    }
    if (inst.field.op == 12)
    {
        return std::make_shared<IAndImmediate>(rawInst);
    }

    /*switch (inst.field.op)
    {
        case 0:
        {

        } break;
        default:
        { break; }
    }*/

    return std::make_shared<IDummy>(rawInst);
}