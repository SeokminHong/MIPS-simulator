#include <cstdio>
#include <string>
#include <fstream>
#include <cstring>

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

    printf ("Open %s...\n", argv[1]);
    
    std::string line;
    while (source >> line, !source.fail())
    {
        if (line[0] == '\\')
        {
            break;
        }
    }
    source.close();

    std::ofstream outfile;
    int len = strlen(argv[1]);
    char s[len + 3] = {};
    char outbuffer[20];
    strncpy(s, argv[1], len - 1);
    strncat(s, "txt", 3);
    outfile.open(s);

    if (!outfile.is_open())
    {
        printf ("Cannot open output file: %s\n", s);
        return 1;
    }
    
    printf ("Output file: %s, %d\n", s, outfile.is_open());

    printf ("Line: %s\n", line.c_str());
    int val, offset = 0;
    while (~sscanf(line.c_str() + offset, "\\x%x", &val))
    {
        sprintf(outbuffer + (offset % 16) / 2, "%02x", val);
        offset += 4;
        if (offset % 16 == 0)
        {
            strncat(outbuffer, "\n", 1);
            printf("%s", outbuffer);
            outfile.write(outbuffer, 9);
        }
    }
}