#include <cstdio>
#include <string>
#include <fstream>
#include <cstring>
#include <map>

static std::map<std::string, int> regMap
{
    {"zero", 0},
    {"at", 1},
    {"v0", 2}, {"v1", 3},
    {"a0", 4}, {"a1", 5}, {"a2", 6}, {"a3", 7},
    {"t0", 8}, {"t1", 9}, {"t2", 10}, {"t3", 11}, {"t4", 12}, {"t5", 13}, {"t6", 14}, {"t7", 15},
    {"s0", 16}, {"s1", 17}, {"s2", 18}, {"s3", 19}, {"s4", 20}, {"s5", 21}, {"s6", 22}, {"s7", 23},
    {"t8", 24}, {"t9", 25}, 
    {"k0", 26}, {"k1", 27}, 
    {"gp", 28},
    {"sp", 29},
    {"fp", 30},
    {"ra", 31},
};

static uint32_t regArr[32] = {};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        puts("Not enough arguments to run. Please run with at least one arguments.\n");
        return 1;
    }

    std::ifstream source;
    source.open(argv[1]);
    if (source.fail())
    {
        printf("Cannot open source file: %s\n", argv[1]);
        return 1;
    }
    source.close();

    std::ofstream outfile;
    int len = strlen(argv[1]);
    char s[len + 12] = {};
    strncpy(s, argv[1], len - 2);
    strncat(s, "-register.txt", 13);
    outfile.open(s);

    if (!outfile.is_open())
    {
        printf ("Cannot open output file: %s\n", s);
        return 1;
    }

    std::string outbuffer;

    for (int i = 2; i < argc; ++i)
    {
        std::string line(argv[i]);
        int l = line.find('=');
        std::string regName;
        std::string numStr;
        uint32_t regVal;

        if (l >= 0)
        {
            regName = line.substr(0, l);
            numStr = line.substr(l + 1);
            if (numStr == "")
            {
                numStr = argv[++i];
            }
        }
        else
        {
            regName = std::move(line);
            line = argv[++i];
            numStr = line.substr(1);
            if (numStr == "")
            {
                numStr = argv[++i];
            }
        }
        regVal = std::stoul(numStr, 0, 16);
        auto it = regMap.find(regName);
        if (it != regMap.end())
        {
            printf("regName: %s, regIndex: %d, regVal: %x\n", regName.c_str(), it->second, regVal);
            regArr[it->second] = regVal;
        }
        else if ((regName[0] | 32) == 'r')
        {
            int regIndex = stoi(regName.substr(1));
            if (regIndex < 32)
            {
                printf("regName: %s, regIndex: %d, regVal: %x\n", regName.c_str(), regIndex, regVal);
                regArr[regIndex] = regVal;
            }
            else
            {
                printf("Cannot find register %s.\n", regName.c_str());
                return 1;
            }
        }
        else
        {
            printf("Cannot find register %s.\n", regName.c_str());
            return 1;
        }
    }
    
    for (int i = 0; i < 32; ++i)
    {
        char buffer[10];
        sprintf(buffer, "%08x\n", regArr[i]);
        outbuffer += buffer;
    }
    outfile << outbuffer;
}