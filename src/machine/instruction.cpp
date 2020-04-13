#include "instruction.h"
#include <cstdio>

#define DECLARE_INSTRUCTION(INSTNAME, INSTTYPE) \
class I##INSTNAME : public IInstruction<INSTTYPE> \
{ \
public: \
    virtual const std::string& GetName() const override { \
        static std::string s{ #INSTNAME }; \
        return s; \
    } \
    I##INSTNAME(uint32_t rawInst) : IInstruction(rawInst) {} \
};

DECLARE_INSTRUCTION(Add, inst_reg);
DECLARE_INSTRUCTION(Subtract, inst_reg);
DECLARE_INSTRUCTION(AddImmediate, inst_imm);

//DECLARE_INSTRUCTION(LoadWord);
//DECLARE_INSTRUCTION(StoreWord);
//DECLARE_INSTRUCTION(LoadHalf);
//DECLARE_INSTRUCTION(LoadHalfUnsigned);
//DECLARE_INSTRUCTION(StoreHalf);
//DECLARE_INSTRUCTION(LoadByte);
//DECLARE_INSTRUCTION(LoadByteUnsigned);
//DECLARE_INSTRUCTION(StoreByte);
//DECLARE_INSTRUCTION(LoadUpperImmediate);

DECLARE_INSTRUCTION(And, inst_reg);
DECLARE_INSTRUCTION(Or, inst_reg);
DECLARE_INSTRUCTION(Nor, inst_reg);
DECLARE_INSTRUCTION(AndImmediate, inst_imm);
DECLARE_INSTRUCTION(OrImmediate, inst_imm);
DECLARE_INSTRUCTION(ShiftLeftLogical, inst_reg);
DECLARE_INSTRUCTION(ShiftRightLogical, inst_reg);

DECLARE_INSTRUCTION(BranchOnEqual, inst_base);
DECLARE_INSTRUCTION(BranchOnNotEqual, inst_base);
//DECLARE_INSTRUCTION(SetOnLessThan);
//DECLARE_INSTRUCTION(SetOnLessThanUnsigned);
//DECLARE_INSTRUCTION(SetLessThanImmediate);
//DECLARE_INSTRUCTION(SetLessThanImmediateUnsigned);

DECLARE_INSTRUCTION(Jump, inst_direct);
//DECLARE_INSTRUCTION(JumpRegister);
//DECLARE_INSTRUCTION(JumpAndLink);

DECLARE_INSTRUCTION(Dummy, inst_reg);

IInstructionBase::IInstructionBase(uint32_t raw)
{
    rawInst = raw;
}

inst_ptr IInstructionBase::MakeInstance(uint32_t rawInst)
{
    inst_reg inst;
    inst.raw = rawInst;
    printf("op: %2d, rs: %2d, rt: %2d, rd: %2d, shamt: %2d, funct: %2d\n", 
           inst.field.op, inst.field.rs, inst.field.rt, inst.field.rd, inst.field.shamt, inst.field.funct);

    switch (inst.field.op)
    {
        case 0:
        {
            return std::make_shared<IAdd>(rawInst);
        }
        case 2:
        {
            return std::make_shared<IJump>(rawInst);
        }
        case 4:
        {
            return std::make_shared<IBranchOnEqual>(rawInst);
        }
        case 5:
        {
            return std::make_shared<IBranchOnNotEqual>(rawInst);
        }
        case 12:
        {
            return std::make_shared<IAndImmediate>(rawInst);
        }
        default:
        {
            return std::make_shared<IDummy>(rawInst);
        }
    }
}

template <>
void IInstruction<inst_base>::UpdatePc(int& pc) const
{
    printf("PC: %X -> %X\n", pc, pc + (GetInst().field.address << 2));
    pc += GetInst().field.address << 2;
}

template <>
void IInstruction<inst_direct>::UpdatePc(int& pc) const
{
    printf("PC: %X -> %X\n", pc, GetInst().field.address << 2);
    pc = GetInst().field.address << 2;
}

template <typename inst_t>
void IInstruction<inst_t>::UpdatePc(int& pc) const
{}
