#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>               /* UNIX and POSIX constants and functions (fork, pipe) */
#include <string>
#include <stdio.h>
#include <map>

std::map<int, std::string> history;
int history_idx = 0;

std::string command_menu = "password - takes a string to be used as a password /n"
                            "encrypt - takes a string that will be encrypted using the Vigenere cipher based on a provided password /n"
                            "decrypt - takes a string to be decrypted /n"
                            "history - shows all strings from this session /n"
                            "quit - exits the program /n";
                           
FILE* from_encryption;
char buffer[256];

int main(int argc, const char** argv)
{
    // To store pipe identifiers
	// p1[0] - read from pipe
	// p1[1] - write to pipe
    int p1[2], p2[2], p3[2];
	pid_t encryption, logger; // To store the process id

    std::string filename;

    std::cout << "Input name of logger file: ";
    std::getline(std::cin, filename);

    // p1 = outgoing to encryption
	if(pipe(p1) == -1)
	{
		std::cerr << "Unable to create first pipe" << std::endl;
		exit(-1);
	}

    // p2 = incoming from encryption
	if(pipe(p2) == -1)
	{
		std::cerr << "Unable to create second pipe" << std::endl;
		exit(-1);
	}

    // p3 = outgoing to logger
    if(pipe(p3) == -1)
	{
		std::cerr << "Unable to create third pipe" << std::endl;
		exit(-1);
	}

    // create child process for encryption 
	encryption = fork();

    if(encryption == -1)
	{
		std::cerr << "Error forking" << std::endl;
		exit(-1);
	}

	if(encryption == 0)
	{
		//I am the child process
		close(p1[1]); // p1 is for reading
        close(p2[0]); // p2 is for writing
		close(p3[0]); // p2 is for writing

		dup2(p1[0], 0); // map pipe to stdin
		dup2(p2[1], 1); // map pipe to stdout
        dup2(p3[1], 2); // map pipe to stdout (stderr)

		char *args[] = {"./encryption",NULL};
		execvp(args[0],args); // replace this program with the encryption program

        exit(1);
	}

    // create child process for logger
	logger = fork();

    if(logger == -1)
	{
		std::cerr << "Error forking" << std::endl;
		exit(-1);
	}

	if(logger == 0)
	{
		//I am the child process
		close(p3[1]); 
        close(p1[0]); 
        close(p1[1]);
		close(p2[0]); 
        close(p2[1]);

        dup2(p3[0], 0); // map pipe to stdin

		char *args[] = {"./logger", (char*)filename.c_str(), NULL};
		execvp(args[0],args); // replace this program with the logger program

        exit(1);
	}

    // establish driver process
    close(p1[0]);
    close(p2[1]);
    close(p3[0]);
    close(p3[1]);

    // open read end of p2
    from_encryption = fdopen(p2[0], "r");
    if (from_encryption == NULL)
    {
        std::cerr << "Failed to open pipe" << std::endl;
        exit(-1);
    }

    // menu
    std::string command, argument, password;

    while (command != "quit")
    {
        std::cout << "\nEnter command:";
        std::getline(std::cin, command);

        for (auto &c : command) = std::to_lower((unsigned char)c);

        // menu options
        if (command == "quit")
        {
            write(p1[1], "QUIT\n", 5);
        }
        else if (command == "password")
        {
            std::cout << "\nType '1' to use a past string"
                            "\nOr type '2' to create a new one: ";
            std::string selection;
            std::getline(std::cin, selection);
            if (std::stoi(selection) == 1)
            {
                if (password.empty())
                {
                    std::cout << "Password not found\n";
                } else if !( for (auto &c : command) = std::is_alpha((unsigned char)c) );{
                    std::cout << "Password can only consist of letters from the alphabet.\n";
                } else {
                    int idx = 0;
                    std::cout << "Enter the digit for the string you want to use for the password.\n";
                }
            }
            else if (std::stoi(selection) == 2)
            {
                std::cout << "\nEnter string: ";
                std::getline(std::cin, password);

                // send to encryption program
                std::string line = "PASS " + password + "\n";
                write(p1[1], line.c_str(), line.size());

                if (fgets(buffer, sizeof(buffer), from_encryption) != NULL)
                {
                    std::cout << buffer;
                }

            } else {
                std::cout<< "\nInvalid input. Please try again.";
            }
        }
        else if (command == "encrypt")
        {
            std::cout << "\nType '1' to use a past string"
                            "\nOr type '2' to create a new one: ";
            std::string selection;
            std::getline(std::cin, selection);
            if (std::stoi(selection) == 1)
            {
                for (const auto &pair: history)
                {
                    std::cout << pair.first << " " << pair.second << "/n";
                }
            }
            else if (std::stoi(selection) == 2)
            {
                std::cout << "\nEnter string: ";
                std::getline(std::cin, argument);

                // send to encryption program
                std::string line = "ENCRYPT " + argument + "\n";
                write(p1[1], line.c_str(), line.size());

                // add entered string to history
                history_idx++;
                history.insert({history_idx, argument});

                if (fgets(buffer, sizeof(buffer), from_encryption) != NULL)
                    std::cout << buffer;

            } else {
                std::cout<< "\nInvalid input. Please try again.";
            }

        }
        else if (command == "decrypt")
        {
            std::cout << "\nType '1' to use a past string"
                            "\nOr type '2' to create a new one";
            std::string selection;
            std::getline(std::cin, selection);
            if (std::stoi(selection) == 1)
            {
                for (const auto &pair: history)
                {
                    std::cout << pair.first << " " << pair.second << "/n";
                }
            }
            else if (std::stoi(selection) == 2)
            {
                std::cout << "\nEnter string: ";
                std::getline(std::cin, argument);

                // send to encryption program
                std::string line = "DECRYPT " + argument + "\n";
                write(p1[1], line.c_str(), line.size());

                // add entered string to history
                history_idx++;
                history.insert({history_idx, argument});

                if (fgets(buffer, sizeof(buffer), from_encryption) != NULL)
                    std::cout << buffer;

            } else {
                std::cout<< "\nInvalid input. Please try again.";
            }

        }
        else if (command == "history")
        {
            for (const auto &pair: history)
            {
                std::cout << pair.first << " " << pair.second << "/n";
            }
        }
    }

    fclose(from_encryption);
    close(p1[1]);

    wait(NULL);
    wait(NULL);


    return 0;
}