#include <iostream>
#include <stdlib.h>
#include <unistd.h>               
#include <string>

std::string encrypt(const std::string &argument, const std::string &passkey)
{
    std::string cipher;

    for (int i = 0; i < argument.size(); i++)
    {
        char letter = ((argument[i] - 'A') + (passkey[i % passkey.size()] - 'A')) % 26;
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
        char letter = ((cipher[i] - 'A') - ((passkey[i % passkey.size()]) - 'A') + 26) % 26;
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
        line.erase(line.find_last_not_of(" \t")+1);

        // delimeter position for whitespace
        size_t del = line.find(' ');

        // split string
        if (del == std::string::npos)
        {   
            command = line;        
            argument = "";
        }
        else
        {
            command = line.substr(0, del);  // command = first half
            argument = line.substr(del+1);  // argument = second half
        }
        

        if (command == "PASS")
        {
            if (argument.empty())
            {
                std::cerr << "ERROR passkey not found\n";
                std::cerr.flush();
                std::cout << "RESULT error, passkey not found\n";
                std::cout.flush();
            }
            else
            {
                passkey = argument;
                std::cerr << "PASS passkey set\n";
                std::cerr.flush();
                std::cout << "RESULT passkey set\n";
                std::cout.flush();
            }
        }
        else if (command == "ENCRYPT")
        {
            if (passkey.empty())
            {
                std::cerr << "ERROR passkey not set\n";
                std::cerr.flush();
                std::cout << "RESULT error, passkey not set\n";
                std::cout.flush();
            }
            else
            {
            std::string encrypted = encrypt(argument, passkey);
            std::cerr << "ENCRYPT " << argument << "\n";
            std::cerr.flush();
            std::cout << "RESULT " << encrypted << "\n";
            std::cout.flush();
            }
        }

        else if (command == "DECRYPT")
        {
            if (passkey.empty())
            {
                std::cerr << "ERROR passkey not set\n";
                std::cerr.flush();
                std::cout << "RESULT error, passkey not set\n";
                std::cout.flush();
            }
            else
            {
            std::string decrypted = decrypt(argument, passkey);
            std::cerr << "DECRYPT argument\n";
            std::cerr.flush();
            std::cout << "RESULT " << decrypted << "\n";
            std::cout.flush();
            }
        }

        else if (command == "QUIT")
        {
            std::cerr << "QUIT program exited\n";
            std::cerr.flush();
            std::cout << "RESULT quit program\n";
            std::cout.flush();
            break;
        }
        else
        {
            std::cerr << "ERROR command not found\n";
            std::cerr.flush();
            std::cout << "RESULT error, command not found\n";
            std::cout.flush();
        }
    }

    return 0;
}