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
    registers[static_cast<int>(reg)] = regVal;
}

void Machine::Run()
{
    pc = 0;
    for (int cycle = 0; cycle < maxCycle; ++cycle)
    {
        printf("Cycle %d\n", cycle + 1);
        Cycle();
    }
}

void Machine::Cycle()
{
    // Read instruction from instruction memory.
    const Instruction& curInst = instructions[pc / 4];
    inst_t inst = curInst.GetInstruction();
    printf("PC: %04X\nInstruction: %08X\n\n", pc, curInst.GetRawInst());

    // Increase PC.
    pc += 4;

    switch (curInst.GetOpCode())
    {
        // Jump
        // j
        case 2:
        {
            // Fill 28 bit into PC.
            pc = (pc & JUMP_BITMASK) | (inst.direct.address << 2);
            break;
        }
        // jal
        case 3:
        {
            // Save PC into $ra
            SetRegister(ERegister::ra, pc);
            // Fill 28 bit into PC.
            pc = (pc & JUMP_BITMASK) | (inst.direct.address << 2);
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
                    pc = registers[static_cast<int>(ERegister::ra)];
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
