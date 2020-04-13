#include "machine.h"
#include "instruction.h"

Machine::Machine()
{

}

void Machine::AddInstruction(const std::string& instruction)
{
    uint32_t num = std::stoi(instruction, 0, 16);
    printf("%s\tDec: %d\tHex: %x\n", instruction.c_str(), num, num);

    IInstruction::MakeInstance(num);
}
