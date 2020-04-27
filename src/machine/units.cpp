#include "units.h"
#include "instruction.h"

void PC::TryWrite(pc_t newPC)
{
    if (pcWrite)
    {
        pc = newPC;
    }
}

std::tuple<ctrl_EX, ctrl_M, ctrl_WB> Control(inst_t inst)
{
    ctrl_EX ex{};
    ctrl_M m{};
    ctrl_WB wb{};
    
    if (inst.raw == 0)
    {
        return std::tie(ex, m, wb);
    }
    
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
            ex.regDst = 0;
            ex.aluOP1 = 0;
            ex.aluOP0 = 1;
            ex.aluSrc = 0;
            m.branch = 1;
            m.memRead = 0;
            m.memWrite = 0;
            wb.regWrite = 0;
            wb.memtoReg = 0;
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
            ex.regDst = 0;
            ex.aluOP1 = 0;
            ex.aluOP0 = 0;
            ex.aluSrc = 1;
            m.branch = 0;
            m.memRead = 0;
            m.memWrite = 1;
            wb.regWrite = 0;
            wb.memtoReg = 0;
            break;
        }
    }
    return std::tie(ex, m, wb);
}

int Forward::GetA() const
{
    if (id_ex_rs == ex_mem_rd)
    {
        return 2;
    }
    if (id_ex_rs == mem_wb_rd)
    {
        return 1;
    }
    return 0;
}

int Forward::GetB() const
{
    if (id_ex_rt == ex_mem_rd)
    {
        return 2;
    }
    if (id_ex_rt == mem_wb_rd)
    {
        return 1;
    }
    return 0;
}
