#include "machine.h"

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
        // Is PC indicating out of index.
        if (instructions.size() < pc / 4)
        {
            printf("PC indicating out of index.\n");
            exit(1);
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
}

void Machine::IF()
{
    // Increase PC.
    ALU(EALU_add, pc, 4u, if_id.pc);
    mux_pc.SetFalseValue(if_id.pc);

    // Read instruction from instruction memory.
    if_id.curInst = &instructions[pc / 4];
    printf("PC: %04X\nInstruction: %08x\n", pc, if_id.curInst->GetRawInst());
}

void Machine::ID()
{
    Instruction const* curInst = if_id.curInst;
    inst_t inst = curInst->GetInstruction();
    auto control = Control(inst);
    id_ex.ex = std::get<0>(control);
    id_ex.m = std::get<1>(control);
    id_ex.wb = std::get<2>(control);
    id_ex.rd0 = inst.reg.rt;
    id_ex.rd1 = inst.reg.rd;
    
    id_ex.rs = inst.base.rs;
    id_ex.rt = inst.base.rt;

    /*switch (curInst->GetOpCode())
    {
        // Jump
        // j
        case 2:
        {
            // Fill lower 28 bits into PC.
            pc = (pc & JUMP_BITMASK) | (~JUMP_BITMASK & (inst.direct.address << 2));
            break;
        }
        // jal
        case 3:
        {
            // Save PC into $ra
            ra.push(pc);
            // Fill lower 28 bits into PC.
            pc = (pc & JUMP_BITMASK) | (~JUMP_BITMASK & (inst.direct.address << 2));
            break;
        }

        // Branch
        // beq
        case 4:
        {
            pc += inst.base.address << 2;
            break;
        }
        // bne
        case 5:
        {
            pc += inst.base.address << 2;
            break;
        }

        // lw
        case 35:
        {
            id_ex.rs = inst.base.rs;
            id_ex.address = inst.base.address;
            break;
        }

        // R-format
        case 0:
        {
            switch (inst.reg.funct)
            {
                // jr
                case 8:
                {
                    // Load PC from $ra.
                    pc = ra.top();
                    ra.pop();
                    break;
                }
                // add
                case 16:
                {
                    id_ex.rs = inst.base.rs;
                    id_ex.rt = inst.base.rt;
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }

        default:
        {
            break;
        }
    }*/
}

void Machine::EX()
{
    ex_mem.m = id_ex.m;
    ex_mem.wb = id_ex.wb;
    ALU(EALU_add, (int32_t)id_ex.pc, id_ex.address << 2, (int32_t&)ex_mem.pc);
    Multiplexer<int32_t> mux_aluSrc{ (int32_t)id_ex.rt, id_ex.address };
    EALU control = GetALUControl(id_ex.ex.aluOP1, id_ex.ex.aluOP0, id_ex.address & 63);
    ex_mem.zero = ALU(control, (int32_t)id_ex.rs, mux_aluSrc.GetValue(id_ex.ex.aluSrc), ex_mem.aluResult);
    ex_mem.rt = id_ex.rt;
    Multiplexer<uint32_t> mux_rd{id_ex.rd0, id_ex.rd1};
    ex_mem.rd = mux_rd.GetValue(id_ex.ex.regDst);
}

void Machine::MEM()
{
    pc = mux_pc.GetValue(ex_mem.zero && ex_mem.m.branch);
    if (ex_mem.m.memWrite)
    {
        *(uint32_t*)(memory + ex_mem.aluResult) = ex_mem.rt;
    }
    if (ex_mem.m.memRead)
    {
        mem_wb.readData = *(uint32_t*)(memory + ex_mem.aluResult);
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
