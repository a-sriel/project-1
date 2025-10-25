#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <string>

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        std::cerr << "ERROR filename not found" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    std::string line;

    std::ofstream out(filename, std::ios::app);
    if (!out)
    {
        std::cerr << "ERROR file could not be opened" << std::endl;
        return 1;
    }
    while(std::getline(std::cin, line))
    {
        time_t timestamp = time(NULL);
        out << line << std::endl;
    }

    out.close();

    return 0;
}
