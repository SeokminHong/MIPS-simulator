#include "instruction.h"
#include <cstdio>

#define DECLARE_INSTRUCTION(INSTNAME) \
class INSTNAME : public IInstruction \
{ \
public: \
    virtual void Execute() override \
    {} \
};

DECLARE_INSTRUCTION(IAdd);
DECLARE_INSTRUCTION(ISubtract);
DECLARE_INSTRUCTION(IAddImmediate);

DECLARE_INSTRUCTION(ILoadWord);
DECLARE_INSTRUCTION(IStoreWord);
DECLARE_INSTRUCTION(ILoadHalf);
DECLARE_INSTRUCTION(ILoadHalfUnsigned);
DECLARE_INSTRUCTION(IStoreHalf);
DECLARE_INSTRUCTION(ILoadByte);
DECLARE_INSTRUCTION(ILoadByteUnsigned);
DECLARE_INSTRUCTION(IStoreByte);
DECLARE_INSTRUCTION(ILoadUpperImmediate);

DECLARE_INSTRUCTION(IAnd);
DECLARE_INSTRUCTION(IOr);
DECLARE_INSTRUCTION(INor);
DECLARE_INSTRUCTION(IAndImmediate);
DECLARE_INSTRUCTION(IOrImmediate);
DECLARE_INSTRUCTION(IShiftLeftLogical);
DECLARE_INSTRUCTION(IShiftRightLogical);

DECLARE_INSTRUCTION(IBranchOnEqual);
DECLARE_INSTRUCTION(IBranchOnNotEqual);
DECLARE_INSTRUCTION(ISetOnLessThan);
DECLARE_INSTRUCTION(ISetOnLessThanUnsigned);
DECLARE_INSTRUCTION(ISetLessThanImmediate);
DECLARE_INSTRUCTION(ISetLessThanImmediateUnsigned);

DECLARE_INSTRUCTION(IJump);
DECLARE_INSTRUCTION(IJumpRegister);
DECLARE_INSTRUCTION(IJumpAndLink);

IInstruction* IInstruction::MakeInstance(uint32_t rawInst)
{
    SInstruction inst;
    inst.raw = rawInst;
    printf("op: %d, rs: %d, rt: %d, rd: %d, shamt: %d, funct: %d\n", 
           inst.field.op, inst.field.rs, inst.field.rt, inst.field.rd, inst.field.shamt, inst.field.funct);

    /*switch (inst.field.op)
    {
        case 0:
        {

        } break;
        default:
        { break; }
    }*/

    return nullptr;
}