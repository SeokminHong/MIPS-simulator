#include "machine.h"
#include "instruction.h"

Machine::Machine()
{
    registers.fill(0u);
    instructions.reserve(MAX_INSTRUCTION);
}

void Machine::AddInstruction(const std::string& instruction)
{
    uint32_t num = std::stoul(instruction, 0, 16);
    printf("%s\tDec: %10u\tHex: %08x\n", instruction.c_str(), num, num);

    instructions.emplace_back(IInstructionBase::MakeInstance(num));
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
    inst_ptr curInst = instructions[pc / 4];
    printf("PC: %04X\nInstruction: %08X\n\n", pc, curInst->GetRawInst());

    pc += 4;
    curInst->UpdatePc(pc);
}
