#include "units.h"
#include "instruction.h"

std::tuple<ctrl_EX, ctrl_M, ctrl_WB> Control(inst_t inst)
{
    ctrl_EX ex;
    ctrl_M m;
    ctrl_WB wb;
    
    switch (inst.base.op)
    {
        // R-format
        case 0:
        {
            ex.regDst = 1;
            ex.aluOP1 = 1;
            ex.aluOP0 = 0;
            ex.aluSrc = 0;
            m.branch = 0;
            m.memRead = 0;
            m.memWrite = 0;
            wb.regWrite = 1;
            wb.memtoReg = 0;
            break;
        }
        // beq
        case 4:
        {
            ex.aluOP1 = 0;
            ex.aluOP0 = 1;
            ex.aluSrc = 0;
            m.branch = 1;
            m.memRead = 0;
            m.memWrite = 0;
            wb.regWrite = 0;
            break;
        }
        // lw
        case 35:
        {
            ex.regDst = 0;
            ex.aluOP1 = 0;
            ex.aluOP0 = 0;
            ex.aluSrc = 1;
            m.branch = 0;
            m.memRead = 1;
            m.memWrite = 0;
            wb.regWrite = 1;
            wb.memtoReg = 1;
            break;
        }
        // sw
        case 43:
        {
            ex.aluOP1 = 0;
            ex.aluOP0 = 0;
            ex.aluSrc = 1;
            m.branch = 0;
            m.memRead = 0;
            m.memWrite = 1;
            wb.regWrite = 0;
            break;
        }
    }
    return std::tie(ex, m, wb);
}