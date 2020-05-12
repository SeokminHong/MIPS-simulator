#include <cstdio>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <algorithm>
#include <cstring>
#include <unordered_map>

static constexpr int REG_MAX = 32;
static constexpr int BUFFER_SIZE = 16384;

static int count = -1;

static std::vector<std::string> commands 
{"code:", "count:", "register:", "output 0:", "output 1:"};

static std::string fileName{};
static int parsePos = 0;

static std::ifstream setting;
static char buffer[BUFFER_SIZE];
std::string line;

// trim from start
static inline std::string& ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string& rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string& trim(std::string& s) {
    return ltrim(rtrim(s));
}

// Update buffer.
static inline bool ReadLine(bool readParam = false)
{
    static bool skip = false;
    if (!readParam && skip)
    {
        // printf("Skip readline. Current cmd: %s\n", line.c_str());
        skip = false;
        return true;
    }
    skip = false;
    while (!setting.getline(buffer, BUFFER_SIZE).fail())
    {
        line = std::string{buffer};
        trim(line);
        // Empty line.
        if (line == "")
        {
            // puts("Empty line");
            continue;
        }

        auto it0 = std::find(commands.begin(), commands.end(), line);
        // Not keywords
        if (it0 == commands.end())
        {
            if (readParam)
            {
                // printf("Param: %s\n", line.c_str());
                return true;
            }
            printf("Unexpected token: %s\n", line.c_str());
            return false;
        }
        // Encounter keywords
        if (readParam)
        {
            // printf("Param encountered cmd %s\n", line.c_str());
            skip = true;
            return false;
        }
        // printf("Command: %s\n", line.c_str());
        return true;
    }
    return false;
}

static std::array<uint32_t, REG_MAX> regArr{};

static std::unordered_map<std::string, int> regMap
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

int Assemble();
int MakeRegister();

// Return value is a number of count.
int main(int argc, char* argv[])
{
    fileName = argv[1];
    setting.open(fileName);
    if (!setting.is_open())
    {
        printf("Cannot open file: %s\n", fileName.c_str());
        return -1;
    }
    parsePos = fileName.find("-setting");

    while (ReadLine())
    {
        // code:
        if (line == commands[0])
        {
            if (int ret = Assemble())
            {
                return ret;
            }
        }
        // count:
        else if (line == commands[1])
        {
            if (ReadLine(true))
            {
                count = std::stoi(line);
            }
            else
            {
                return -1;
            }
        }
        // register:
        else if (line == commands[2])
        {
            if (int ret = MakeRegister())
            {
                return ret;
            }
        }        
    }

    return count;
}

int Assemble()
{
    std::string outFileName = fileName.substr(0, parsePos) + "-code.txt";
    
    std::ofstream outfile;
    outfile.open(outFileName);

    if (!outfile.is_open())
    {
        printf ("Cannot open output file: %s\n", outFileName.c_str());
        return -1;
    }
    
    printf ("Assembled output: %s\n", outFileName.c_str());

    if (!ReadLine(true))
    {
        return -1;
    }
    if (line[0] == '\\')
    {
        int val, offset = 0;
        char outbuffer[20];
        while (~sscanf(line.c_str() + offset, "\\x%x", &val))
        {
            sprintf(outbuffer + (offset % 16) / 2, "%02x", val);
            offset += 4;
            if (offset % 16 == 0)
            {
                strncat(outbuffer, "\n", 1);
                outfile.write(outbuffer, 9);
            }
        }
    }
    else
    {
        do
        {
            outfile.write(line.c_str(), 8);
            outfile.write("\n", 1);
        } while (ReadLine(true));
    }

    return 0;
}

int MakeRegister()
{    
    std::string outFileName = fileName.substr(0, parsePos) + "-reg.txt";
    
    std::ofstream outfile;
    outfile.open(outFileName);

    if (!outfile.is_open())
    {
        printf("Cannot open output file: %s\n", outFileName.c_str());
        return -1;
    }
    
    printf("Register output: %s\n", outFileName.c_str());

    while (ReadLine(true))
    {
        std::string& token = line;
        int eqPos = token.find('=');
        std::string regName;
        std::string numStr;
        uint32_t regVal;
        if (eqPos >= 0)
        {
            regName = token.substr(0, eqPos);
            numStr = token.substr(eqPos + 1);
            if (numStr == "")
            {
                if (!ReadLine(true))
                {
                    return -1;
                }
                numStr = line;
            }
        }
        else
        {
            regName = std::move(token);
            token = line;
            numStr = token.substr(1);
            if (numStr == "")
            {
                if (!ReadLine(true))
                {
                    return -1;
                }
                numStr = line;
            }
        }
        regVal = std::stoul(numStr, 0, 16);
        auto it = regMap.find(regName);
        if (it != regMap.end())
        {
            regArr[it->second] = regVal;
        }
        else if ((regName[0] | 32) == '$')
        {
            int regIndex = stoi(regName.substr(1));
            if (regIndex < 32)
            {
                regArr[regIndex] = regVal;
            }
            else
            {
                printf("Cannot find register %s.\n", regName.c_str());
                return -1;
            }
        }
        else
        {
            printf("Cannot find register %s.\n", regName.c_str());
            return -1;
        }
    }

    for (int i = 0; i < REG_MAX; ++i)
    {
        char buffer[10];
        sprintf(buffer, "%08x\n", regArr[i]);
        outfile << buffer;
    }
    return 0;
}
