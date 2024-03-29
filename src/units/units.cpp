#include "units.h"
#include "../machine/instruction.h"
#include "../machine/machine.h"

std::tuple<ctrl_EX, ctrl_M, ctrl_WB> Control(const inst_t& inst)
{
    ctrl_EX ex{};
    ctrl_M m{};
    ctrl_WB wb{};
    
    if (inst.raw == 0)
    {
        return std::make_tuple(std::move(ex), std::move(m), std::move(wb));
    }

    ex.op = inst.reg.op;
    ex.funct = inst.reg.funct;

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
        // JR
        if (inst.reg.funct == 8)
        {
            m.jump = 1;
        }
        else
        {
            ex.regDst = 1;
            wb.regWrite = 1;
            // JALR
            if (inst.reg.funct == 9)
            {
                m.jump = 1;
            }
        }        
    }
    // J
    else if (op.raw == 2)
    {
        m.jump = 1;
    }
    // JAL
    else if (op.raw == 3)
    {
        m.jump = 1;
        wb.regWrite = 1;
    }
    // BEQ
    else if (op.raw == 4)
    {
        m.branch = 1;
        m.beq = 1;
    }
    // BNE
    else if (op.raw == 5)
    {
        m.branch = 1;
        m.beq = 0;
    }
    // Immediates
    else if (op.v.hi == 1)
    {
        ex.aluSrc = 1;
        wb.regWrite = 1;
    }
    // Load
    else if (op.v.hi == 4)
    {
        ex.aluSrc = 1;
        m.memRead = 1;
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
        ex.aluSrc = 1;
        m.memWrite = 1;
        
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
    return std::make_tuple(std::move(ex), std::move(m), std::move(wb));
}

int UForward::GetA() const
{
    if (owner.GetMode() == 0)
    {
        return 0;
    }
    if (ex_rs == mem_rd)
    {
        return 2;
    }
    if (ex_rs == wb_rd)
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
    if (ex_rt == mem_rd)
    {
        return 2;
    }
    if (ex_rt == wb_rd)
    {
        return 1;
    }
    return 0;
}

int UForward::GetC() const
{
    if (owner.GetMode() == 0)
    {
        return 0;
    }
    if (mem_rd == wb_rd)
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
    if (ex_memRead && ex_rt > 0 && ex_rt < REG_MAX)
    {
        if (id_memWrite && ex_rt == id_rt)
        {
            return false;
        }
        return (id_rs == ex_rt) || (id_rt == ex_rt);
    }
    return false;
}
