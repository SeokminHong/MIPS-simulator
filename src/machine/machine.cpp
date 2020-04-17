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
            printf("\n\n");
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
    // Read instruction from instruction memory.
    const Instruction& curInst = instructions[pc / 4];
    inst_t inst = curInst.GetInstruction();
    printf("PC: %04X\nInstruction: %08x", pc, curInst.GetRawInst());

    // Increase PC.
    pc += 4;

    switch (curInst.GetOpCode())
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
    }
}
