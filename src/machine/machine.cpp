#include "machine.h"
#include <cstring>

#define JUMP_BITMASK (15u << 28)

Machine::Machine()
{
    registers.fill(0u);
    instructions.reserve(MAX_INSTRUCTION);
}

void Machine::AddInstruction(uint32_t instruction)
{
    instructions.emplace_back(instruction);
}

void Machine::SetRegister(ERegister reg, uint32_t regVal)
{
    registers[*reg] = regVal;
}

void Machine::Run()
{
    pc = 0;
    for (int cycle = 0; cycle < maxCycle; ++cycle)
    {
        if (cycle > 0)
        {
            printf("\n");
        }
        printf("Cycle %d\n", cycle + 1);
        Cycle();
    }
}

void Machine::Cycle()
{
    WB();
    MEM();
    EX();
    ID();
    IF();

    if (branchResolvedFlag)
    {
        branchUnresolved = false;
        pc = pendingPC;
    }

    // Print registers' states.
    puts("Registers:");
    for (int i = 0; i < REG_MAX; ++i)
    {
        printf("[%d] %08x\n", i, registers[i]);
    }
    // Print memory I/O.
    printf("Memory I/O: %s\n", outputBuffer);
}

void Machine::IF()
{
    // Increase PC.
    if_id.pc = ALU(EALU::ADD, pc, 4);
    mux_branch.SetValue(0, if_id.pc);

    uint32_t rawInst = 0;
    Instruction const* curInst = nullptr;
    // Read instruction from instruction memory.
    if ((int32_t)instructions.size() >= pc / 4)
    {
        curInst = &instructions[pc / 4];
        rawInst = curInst->GetRawInst();
    }

    if (!NeedsStall())
    {
        if_id.curInst = curInst;
        if (curInst && curInst->IsBranchOrJump())
        {
            branchUnresolved = true;
        }
    }
    else
    {
        if_id.curInst = nullptr;
    }
    
    printf("PC: %04X\nInstruction: %08x\n", pc, rawInst);
}

void Machine::ID()
{
    Instruction const* curInst = if_id.curInst;
    inst_t inst = curInst ? curInst->GetInstruction() : inst_t{0};
    auto controlValue = Control(inst);
    using tupleType = decltype(controlValue);
    UMultiplexer<tupleType> mux_ctrl{ controlValue, tupleType{} };

    hazardDetector.id_rs = inst.reg.rs;
    hazardDetector.id_rt = inst.reg.rt;
    hazardDetector.id_memWrite = std::get<1>(controlValue).memWrite;

    id_ex.pc = if_id.pc;
    
    std::tie(id_ex.ex, id_ex.m, id_ex.wb) = mux_ctrl.GetValue(hazardDetector.IsHazardDetected());

    id_ex.rd0 = inst.reg.rt;
    id_ex.rd1 = inst.reg.rd;
    id_ex.shamt = inst.reg.shamt;
    id_ex.address = inst.base.address;
    
    id_ex.rs_val = registers[inst.base.rs];
    id_ex.rt_val = registers[inst.base.rt];

    id_ex.rs = inst.base.rs;
    id_ex.rt = inst.base.rt;

    id_ex.jmp_address = (pc & JUMP_BITMASK) | (~JUMP_BITMASK & (inst.direct.address << 2));
}

void Machine::EX()
{
    ex_mem.m = std::move(id_ex.m);
    ex_mem.wb = std::move(id_ex.wb);
    ex_mem.offsettedPC = ALU(EALU::ADD, id_ex.pc, id_ex.address << 2);

    mux_fwd_a.SetValue(0, id_ex.rs_val);
    mux_fwd_b.SetValue(0, id_ex.rt_val);

    forwarding.ex_rs = id_ex.rs;
    forwarding.ex_rt = id_ex.rt;
    
    UMultiplexer<int32_t> mux_aluSrc{ mux_fwd_b.GetValue(forwarding.GetB()), id_ex.address };

    hazardDetector.ex_rt = id_ex.rt;
    hazardDetector.ex_memRead = id_ex.m.memRead;

    EALU control = GetALUControl(id_ex.ex.op, id_ex.ex.funct);
    pc_t jump_addr = id_ex.jmp_address;
    // JAL
    if (id_ex.ex.op == 3)
    {
        ex_mem.aluResult = id_ex.pc;
    }
    // JR
    else if (id_ex.ex.op == 0 && id_ex.ex.funct == 8)
    {
        jump_addr =  mux_fwd_a.GetValue(forwarding.GetA());
    }
    // JALR
    else if (id_ex.ex.op == 0 && id_ex.ex.funct == 9)
    {
        ex_mem.aluResult = id_ex.pc;
        jump_addr =  mux_fwd_a.GetValue(forwarding.GetA());
    }
    // ANDI, ORI
    else if (id_ex.ex.op == 12 || id_ex.ex.op == 13)
    {
        // Remove sign-extends of bit operation for immediates.
        ex_mem.aluResult = ALU(control, mux_fwd_a.GetValue(forwarding.GetA()), 0xffff & mux_aluSrc.GetValue(id_ex.ex.aluSrc), ex_mem.zero);
    }
    // LUI
    else if (id_ex.ex.op == 15)
    {
        ex_mem.aluResult = ALU(control, mux_aluSrc.GetValue(id_ex.ex.aluSrc), 16, ex_mem.zero);
    }
    // It is not shift operation.
    else if (control != EALU::SLL && control != EALU::SRL && control != EALU::SRA)
    {
        ex_mem.aluResult = ALU(control, mux_fwd_a.GetValue(forwarding.GetA()), mux_aluSrc.GetValue(id_ex.ex.aluSrc), ex_mem.zero);
    }
    else
    {
        ex_mem.aluResult = ALU(control, mux_aluSrc.GetValue(id_ex.ex.aluSrc), id_ex.shamt, ex_mem.zero);
    }
    
    ex_mem.rt_val = id_ex.rt_val;
    // JAL
    if (id_ex.ex.op == 3)
    {
        ex_mem.rd = *ERegister::ra;
    }
    else
    {
        UMultiplexer<uint32_t> mux_rd{ id_ex.rd0, id_ex.rd1 };
        ex_mem.rd = mux_rd.GetValue(id_ex.ex.regDst);
    }    

    mux_jump.SetValue(1, jump_addr);
}

void Machine::MEM()
{
    if (!hazardDetector.IsHazardDetected())
    {
        mux_branch.SetValue(1, ex_mem.offsettedPC);
        pc_t newPC = mux_branch.GetValue((ex_mem.zero == ex_mem.m.beq) && ex_mem.m.branch);
        mux_jump.SetValue(0, newPC);
        newPC = mux_jump.GetValue(ex_mem.m.jump);
        if (ex_mem.m.jump)
        {
            printf("Jump!! %d\n", newPC);
        }
        if (branchUnresolved)
        {
            // If jump/branch occurs.
            if (ex_mem.m.branch | ex_mem.m.jump)
            {
                branchResolvedFlag = true;
                pendingPC = newPC;
            }
        }
        else
        {
            if (branchResolvedFlag)
            {
                branchResolvedFlag = false;
            }
            else
            {
                pc = newPC;
            }
        }        
    }
    mem_wb.wb = std::move(ex_mem.wb);
    mem_wb.rd = ex_mem.rd;
    
    mux_fwd_a.SetValue(2, ex_mem.aluResult);
    mux_fwd_b.SetValue(2, ex_mem.aluResult);
    forwarding.mem_rd = ex_mem.rd;

    int bytes = *ex_mem.m.numBytes;
    if (ex_mem.m.memWrite)
    {
        mux_fwd_c.SetValue(0, ex_mem.rt_val);
        int32_t writeValue = mux_fwd_c.GetValue(forwarding.GetC());
        uint32_t printValue = 0;
        switch (ex_mem.m.numBytes)
        {
            case EMemoryRW::byte:
            {
                printValue = writeValue & 255;
                memory.Set<int8_t>(ex_mem.aluResult, writeValue);
                break;
            }
            case EMemoryRW::half:
            {
                printValue = writeValue & 65535;
                memory.Set<int16_t>(ex_mem.aluResult, writeValue);
                break;
            }
            case EMemoryRW::word:
            {
                printValue = *(uint32_t*)&writeValue;
                memory.Set<int32_t>(ex_mem.aluResult, writeValue);
                break;
            }
            default: {}
        }
        std::string formatStr = "W %d %04x %04x";
        formatStr[12] = bytes * 2 + '0';
        sprintf(outputBuffer, formatStr.c_str(), bytes, ex_mem.aluResult, printValue);
    }
    else if (ex_mem.m.memRead)
    {
        uint32_t printValue = 0;
        switch (ex_mem.m.numBytes)
        {
            case EMemoryRW::byte:
            {
                *((int32_t*)&mem_wb.readData) = memory.Get<int8_t>(ex_mem.aluResult);
                printValue = mem_wb.readData & 255;
                break;
            }
            case EMemoryRW::ubyte:
            {
                mem_wb.readData = memory.Get<uint8_t>(ex_mem.aluResult);
                printValue = mem_wb.readData;
                break;
            }
            case EMemoryRW::half:
            {
                *((int32_t*)&mem_wb.readData) = memory.Get<int16_t>(ex_mem.aluResult);
                printValue = mem_wb.readData & 65535;
                break;
            }
            case EMemoryRW::uhalf:
            {
                mem_wb.readData = memory.Get<uint16_t>(ex_mem.aluResult);
                printValue = mem_wb.readData;
                break;
            }
            case EMemoryRW::word:
            {
                mem_wb.readData = memory.Get<int32_t>(ex_mem.aluResult);
                printValue = mem_wb.readData;
                break;
            }
        }
        std::string formatStr = "R %d %04x %04x";
        formatStr[12] = bytes * 2 + '0';
        sprintf(outputBuffer, formatStr.c_str(), bytes, ex_mem.aluResult, printValue);
    }
    else
    {
        outputBuffer[0] = '\0';
    }
    mem_wb.aluResult = ex_mem.aluResult;
}

void Machine::WB()
{
    UMultiplexer<uint32_t> mux_memtoReg{ mem_wb.aluResult, mem_wb.readData };
    uint32_t value = mux_memtoReg.GetValue(mem_wb.wb.memtoReg);
    
    mux_fwd_a.SetValue(1, value);
    mux_fwd_b.SetValue(1, value);
    mux_fwd_c.SetValue(1, value);
    forwarding.wb_rd = mem_wb.rd;

    if (mem_wb.wb.regWrite)
    {
        registers[mem_wb.rd] = value;
    }
}

bool Machine::NeedsStall() const
{
    if (hazardDetector.IsHazardDetected())
    {
        return true;
    }
    return branchUnresolved;
}
