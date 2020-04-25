#include "machine.h"
#include <cstring>

#define JUMP_BITMASK (15u << 28)

Machine::Machine()
{
    registers.fill(0u);
    instructions.reserve(MAX_INSTRUCTION);
    memset(memory, 0, MAX_MEMORY);
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
    ALU(EALU_add, pc, 4u, if_id.pc);
    mux_pc.SetFalseValue(if_id.pc);

    uint32_t rawInst = 0;
    // Read instruction from instruction memory.
    if (instructions.size() < pc / 4)
    {
        if_id.curInst = nullptr;
    }
    else
    {
        if_id.curInst = &instructions[pc / 4];
        rawInst = if_id.curInst->GetRawInst();
    }
    
    printf("PC: %04X\nInstruction: %08x\n", pc, rawInst);
}

void Machine::ID()
{
    Instruction const* curInst = if_id.curInst;
    inst_t inst = curInst ? curInst->GetInstruction() : inst_t{0};
    auto control = Control(inst);
    id_ex.ex = std::get<0>(control);
    id_ex.m = std::get<1>(control);
    id_ex.wb = std::get<2>(control);
    id_ex.rd0 = inst.reg.rt;
    id_ex.rd1 = inst.reg.rd;
    id_ex.address = inst.base.address;
    
    id_ex.rs = registers[inst.base.rs];
    id_ex.rt = registers[inst.base.rt];
}

void Machine::EX()
{
    ex_mem.m = id_ex.m;
    ex_mem.wb = id_ex.wb;
    ALU(EALU_add, (int32_t)id_ex.pc, id_ex.address << 2, (int32_t&)ex_mem.pc);
    Multiplexer<int32_t> mux_aluSrc{ id_ex.address, (int32_t)id_ex.rt };
    EALU control = GetALUControl(id_ex.ex.aluOP1, id_ex.ex.aluOP0, id_ex.address & 63);
    ex_mem.zero = ALU(control, (int32_t)id_ex.rs, mux_aluSrc.GetValue(id_ex.ex.aluSrc), ex_mem.aluResult);
    ex_mem.rt = id_ex.rt;
    Multiplexer<uint32_t> mux_rd{id_ex.rd1, id_ex.rd0};
    ex_mem.rd = mux_rd.GetValue(id_ex.ex.regDst);
}

void Machine::MEM()
{
    pc = mux_pc.GetValue(ex_mem.zero && ex_mem.m.branch);
    mem_wb.wb = ex_mem.wb;
    mem_wb.rd = ex_mem.rd;

    int bytes = 4;
    if (ex_mem.m.memWrite)
    {
        *(uint32_t*)(memory + ex_mem.aluResult) = ex_mem.rt;
        std::string formatStr = "W %d %04x %04d";
        formatStr[12] = bytes + '\0';
        sprintf(outputBuffer, formatStr.c_str(), bytes, ex_mem.aluResult, ex_mem.rt);
    }
    else if (ex_mem.m.memRead)
    {
        mem_wb.readData = *(uint32_t*)(memory + ex_mem.aluResult);
        std::string formatStr = "R %d %04x %04d";
        formatStr[12] = bytes + '\0';
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
    Multiplexer<uint32_t> mux_memtoReg{ mem_wb.readData, mem_wb.aluResult };
    uint32_t value = mux_memtoReg.GetValue(mem_wb.wb.memtoReg);
    if (mem_wb.wb.regWrite)
    {
        registers[mem_wb.rd] = value;
    }
}
