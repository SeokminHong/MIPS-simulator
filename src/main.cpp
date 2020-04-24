#include <cstdio>
#include <fstream>
#include <string>
#include "machine/machine.h"

int main(int argc, char* argv[])
{
    Machine machine;

    if (argc < 3)
    {
        puts("Not enough arguments to run. Please run with at least two arguments:\n");
        puts("./simulator (source_file) (cycles) [initial_registers]\n");
        return 1;
    }

    std::ifstream source;
    source.open(argv[1]);
    if (source.fail())
    {
        printf("Cannot open source file: %s\n", argv[1]);
        return 1;
    }

    std::string line;
    
    while (source >> line, !source.fail())
    {
        uint32_t inst = std::stoul(line, 0, 16);
        machine.AddInstruction(inst);
    }
    source.close();

    machine.SetMaxCycle(atoi(argv[2]));
    machine.SetMode(atoi(argv[3]));

    if (argc >= 5)
    {
        source.open(argv[4]);
        if (source.fail())
        {
            printf("Cannot open register file: %s\n", argv[3]);
            return 1;
        }
        for (ERegister reg = ERegister::zero; reg < ERegister::Max; ++reg)
        {
            if (source >> line, !source.fail())
            {
                machine.SetRegister(reg, std::stoul(line, 0, 16));
            }
        }
    }

    machine.Run();
}