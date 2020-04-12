#include <cstdio>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
    std::ifstream source;
    source.open(argv[1]);

    std::string line;
    
    while (source >> line, !source.fail())
    {
        int num = std::stoi(line, 0, 16);
        printf("%s\tDec: %d\tHex: %x\n", line.c_str(), num, num);
    }
}