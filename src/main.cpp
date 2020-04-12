#include <cstdio>
#include <fstream>
#include <string>
#include "machine/machine.h"

int main(int argc, char* argv[])
{
    Machine machine;

    std::ifstream source;
    source.open(argv[1]);

    std::string line;
    
    while (source >> line, !source.fail())
    {
        machine.AddInstruction(line);
    }
}