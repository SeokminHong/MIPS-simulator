#include "machine.h"
#include <cstring>

#define JUMP_BITMASK (15u << 28)

Machine::Machine()
{
    registers.fill(0u);
    instructions.reserve(MAX_INSTRUCTION);
    memset(&if_id, 0, sizeof(if_id));
    memset(&id_ex, 0, sizeof(id_ex));
    memset(&ex_mem, 0, sizeof(ex_mem));
    memset(&mem_wb, 0, sizeof(mem_wb));
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

    if (!hazardDetector.IsHazardDetected())
    {
        if_id.curInst = curInst;
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
    
    std::tie(id_ex.ex, id_ex.m, id_ex.wb) = mux_ctrl.GetValue(hazardDetector.IsHazardDetected());

    id_ex.rd0 = inst.reg.rt;
    id_ex.rd1 = inst.reg.rd;
    id_ex.shamt = inst.reg.shamt;
    id_ex.address = inst.base.address;
    
    id_ex.rs_val = registers[inst.base.rs];
    id_ex.rt_val = registers[inst.base.rt];

    id_ex.rs = inst.base.rs;
    id_ex.rt = inst.base.rt;

    mux_jump.SetValue(1, (pc & JUMP_BITMASK) | (~JUMP_BITMASK & (inst.direct.address << 2)));
}

void Machine::EX()
{
    ex_mem.m = id_ex.m;
    ex_mem.wb = id_ex.wb;
    ex_mem.pc = ALU(EALU::ADD, id_ex.pc, id_ex.address << 2);

    mux_fwd0.SetValue(0, id_ex.rs_val);
    mux_fwd1.SetValue(0, id_ex.rt_val);

    forwarding.id_ex_rs = id_ex.rs;
    forwarding.id_ex_rt = id_ex.rt;
    
    UMultiplexer<int32_t> mux_aluSrc{ mux_fwd1.GetValue(forwarding.GetB()), id_ex.address };

    hazardDetector.ex_rt = id_ex.rt;
    hazardDetector.id_ex_memRead = id_ex.m.memRead;

    EALU control = GetALUControl(id_ex.ex.op, id_ex.ex.funct);
    // It is not shift operation.
    if (control != EALU::SLL && control != EALU::SRL && control != EALU::SRA)
    {
        ex_mem.aluResult = ALU(control, mux_fwd0.GetValue(forwarding.GetA()), mux_aluSrc.GetValue(id_ex.ex.aluSrc), ex_mem.zero);
    }
    else
    {
        ex_mem.aluResult = ALU(control, mux_aluSrc.GetValue(id_ex.ex.aluSrc), id_ex.shamt, ex_mem.zero);
    }
    
    ex_mem.rt_val = id_ex.rt_val;
    UMultiplexer<uint32_t> mux_rd{ id_ex.rd0, id_ex.rd1 };
    ex_mem.rd = mux_rd.GetValue(id_ex.ex.regDst);
    mux_branch.SetValue(1, ALU(EALU::ADD, id_ex.pc, id_ex.address << 2));
}

void Machine::MEM()
{
    if (!hazardDetector.IsHazardDetected())
    {
        pc_t tempPC = mux_branch.GetValue((ex_mem.zero ^ ex_mem.m.beq) && ex_mem.m.branch);
        mux_jump.SetValue(0, tempPC);
        pc = mux_jump.GetValue(ex_mem.m.jump);
        // If jump/branch occurs.
        if (pc != mux_branch.GetValue(0))
        {
            Flush();
        }
    }
    mem_wb.wb = ex_mem.wb;
    mem_wb.rd = ex_mem.rd;
    
    mux_fwd0.SetValue(2, ex_mem.aluResult);
    mux_fwd1.SetValue(2, ex_mem.aluResult);
    forwarding.ex_mem_rd = ex_mem.rd;

    int bytes = *ex_mem.m.numBytes;
    if (ex_mem.m.memWrite)
    {
        switch (ex_mem.m.numBytes)
        {
            case EMemoryRW::byte:
            {
                memory.Set<int8_t>(ex_mem.aluResult, ex_mem.rt_val);
                break;
            }
            case EMemoryRW::half:
            {
                memory.Set<int16_t>(ex_mem.aluResult, ex_mem.rt_val);
                break;
            }
            case EMemoryRW::word:
            {
                memory.Set<int32_t>(ex_mem.aluResult, ex_mem.rt_val);
                break;
            }
            default: {}
        }
        std::string formatStr = "W %d %04x %04x";
        formatStr[12] = bytes + '0';
        sprintf(outputBuffer, formatStr.c_str(), bytes, ex_mem.aluResult, ex_mem.rt_val);
    }
    else if (ex_mem.m.memRead)
    {
        switch (ex_mem.m.numBytes)
        {
            case EMemoryRW::byte:
            {
                *((int32_t*)&mem_wb.readData) = memory.Get<int8_t>(ex_mem.aluResult);
                break;
            }
            case EMemoryRW::ubyte:
            {
                mem_wb.readData = memory.Get<uint8_t>(ex_mem.aluResult);
                break;
            }
            case EMemoryRW::half:
            {
                *((int32_t*)&mem_wb.readData) = memory.Get<int16_t>(ex_mem.aluResult);
                break;
            }
            case EMemoryRW::uhalf:
            {
                mem_wb.readData = memory.Get<uint16_t>(ex_mem.aluResult);
                break;
            }
            case EMemoryRW::word:
            {
                mem_wb.readData = memory.Get<int32_t>(ex_mem.aluResult);
                break;
            }
        }
        std::string formatStr = "R %d %04x %04x";
        formatStr[12] = bytes + '0';
        sprintf(outputBuffer, formatStr.c_str(), bytes, ex_mem.aluResult, mem_wb.readData);
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
    
    mux_fwd0.SetValue(1, value);
    mux_fwd1.SetValue(1, value);
    forwarding.mem_wb_rd = mem_wb.rd;

    if (mem_wb.wb.regWrite)
    {
        registers[mem_wb.rd] = value;
    }
}

void Machine::Flush()
{
    memset(&if_id, 0, sizeof(if_id));
    memset(&id_ex, 0, sizeof(id_ex));
    memset(&ex_mem, 0, sizeof(ex_mem));
}
