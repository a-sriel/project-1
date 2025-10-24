#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>
#include <fstream>

int main(int argc, const char** argv)
{
    std::string filename = argv[1];

    std::string line;
    std::ofstream out(filename, std::ios::app);

    while(std::getline(std::cin, line))
    {
        time_t timestamp = time(NULL);
        out << line << std::endl;
    }

    out.close();

    return 0;
}
