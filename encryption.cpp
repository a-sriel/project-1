#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>               
#include <string>

int main(int argc, const char** argv)
{
    std::string line, passkey, command, argument;

    while(std::getline(std::cin, line))
    {
        
        line.erase(0, line.find_first_not_of(" \t"));

        // delimeter position for whitespace
        size_t del = line.find(' ');

        // split string
        command = (del == std::string::npos) ? line : line.substr(0, del);  // command = first half
        argument = (del == std::string::npos) ? line: line.substr(del+1);   // argument = second half

        if (line == "encrypt")
        {

        }

        if (line == "decrypt")
        {

        }

        if (line == "quit")
        {
            break;
        }

        if (line == "result")
        {

        }

        if (line == "error")
        {

        }

    }
    return 0;
}