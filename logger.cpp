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
        if (line == "QUIT")
        {
            break;
        }

        std::string command;
        std::string message;

        line.erase(0, line.find_first_not_of(" \t"));
        size_t del = line.find(' ');

        if (del == std::string::npos)
        {
            command = line;
            message = "";
        }
        else
        {
            command = line.substr(0, del);
            message = line.substr(del+1);
        }

        time_t t = time(NULL);
        struct tm* timestamp = localtime(&t);

        out << (timestamp->tm_year + 1900) << "-"
            << std::setfill('0') << std::setw(2) << (timestamp->tm_mon + 1) << "-"
            << std::setfill('0') << std::setw(2) << timestamp->tm_mday << " "
            << std::setfill('0') << std::setw(2) << timestamp->tm_hour << ":"
            << std::setfill('0') << std::setw(2) << timestamp->tm_min 
            << " [" << command << "] " << message << std::endl;
    }

    out.close();

    return 0;
}
