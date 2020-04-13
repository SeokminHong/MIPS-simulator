#include "machine.h"
#include "instruction.h"

Machine::Machine()
{
    registers.fill(0u);
    instructions.reserve(MAX_INSTRUCTION);
}

void Machine::AddInstruction(const std::string& instruction)
{
    uint32_t num = std::stoi(instruction, 0, 16);
    printf("%s\tDec: %10d\tHex: %08x\n", instruction.c_str(), num, num);

    instructions.emplace_back(IInstruction::MakeInstance(num));
}

void Machine::SetRegister(ERegister regNum, uint32_t regVal)
{
    registers[static_cast<int>(regNum)] = regVal;
}

void Machine::Run()
{
    pc = 0;
    for (int cycle = 0; cycle < maxCycle; ++cycle)
    {
        Cycle();
    }
}

void Machine::Cycle()
{
    inst_ptr curInst = instructions[pc / 4];
    int nextPc = pc + 4;

    printf("Inst: %s\n", curInst->GetName().c_str());
    pc = nextPc;
}
