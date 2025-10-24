#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>               
#include <string>

std::string encrypt(const std::string &argument, const std::string &passkey)
{
    std::string cipher;

    for (int i = 0; i < argument.size(); i++)
    {
        char letter = (argument[i] + passkey[i]) % 26;

        letter += 'A';

        cipher.push_back(letter);
    }

    return cipher;
}

std::string decrypt(const std::string &cipher, const std::string &passkey)
{
    std::string argument;

    for (int i = 0; i < cipher.size(); i++)
    {
        char letter = (cipher[i] - passkey[i] + 26) % 26;

        letter += 'A';

        argument.push_back(letter);
    }

    return argument;
}

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

        if (command == "encrypt")
        {
            encrypt(argument, passkey);
        }

        if (command == "decrypt")
        {

        }

        if (command == "quit")
        {
            break;
        }


    }
    return 0;
}