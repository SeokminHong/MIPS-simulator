#include "units.h"
#include "instruction.h"
#include "machine.h"

std::tuple<ctrl_EX, ctrl_M, ctrl_WB> Control(inst_t inst)
{
    ctrl_EX ex{};
    ctrl_M m{};
    ctrl_WB wb{};
    
    if (inst.raw == 0)
    {
        return std::tie(ex, m, wb);
    }

    union {
        uint8_t raw;
        struct {
            uint8_t lo : 3;
            uint8_t hi : 3;
        } v;
    } op;
    op.raw = inst.base.op;

    // R-format
    if (op.raw == 0)
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
    }
    // Load
    else if (op.v.hi == 4)
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

        switch (op.v.lo)
        {
            case 0:
            {
                m.numBytes = EMemoryRW::byte;
                break;
            }
            case 1:
            {
                m.numBytes = EMemoryRW::half;
                break;
            }
            case 3:
            {
                m.numBytes = EMemoryRW::word;
                break;
            }
            case 4:
            {
                m.numBytes = EMemoryRW::ubyte;
                break;
            }
            case 5:
            {
                m.numBytes = EMemoryRW::uhalf;
                break;
            }
        }
    }
    // Save
    else if (op.v.hi == 5)
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
        
        switch (op.v.lo)
        {
            case 0:
            {
                m.numBytes = EMemoryRW::byte;
                break;
            }
            case 1:
            {
                m.numBytes = EMemoryRW::half;
                break;
            }
            case 3:
            {
                m.numBytes = EMemoryRW::word;
                break;
            }
        }
    }
    
    /*switch (inst.base.op)
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
    }*/
    return std::make_tuple(std::move(ex), std::move(m), std::move(wb));
}

int UForward::GetA() const
{
    if (owner.GetMode() == 0)
    {
        return 0;
    }
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

int UForward::GetB() const
{
    if (owner.GetMode() == 0)
    {
        return 0;
    }
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

bool UHazardDetector::IsHazardDetected() const
{
    if (owner.GetMode() == 0)
    {
        return false;
    }
    if (id_ex_memRead && ex_rt > 0 && ex_rt < REG_MAX)
    {
        return (id_rs == ex_rt) || (id_rt == ex_rt);
    }
    return false;
}
