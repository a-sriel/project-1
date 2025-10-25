#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>               /* UNIX and POSIX constants and functions (fork, pipe) */
#include <string>
#include <stdio.h>
#include <map>

#include <cctype>      
#include <cstring>

std::map<int, std::string> history;
int history_idx = 0;

// file handling
FILE* from_encryption;
FILE* to_logger;
char buffer[256];

std::string command_menu =  "Command menu:\n"
                            "\tpassword - takes a string to be used as a password\n"
                            "\tencrypt - takes a string that will be encrypted using the Vigenere cipher based on a provided password\n"
                            "\tdecrypt - takes a string to be decrypted\n"
                            "\thistory - shows all strings from this session\n"
                            "\tquit - exits the program\n";

// input validation methods
bool is_alpha(const std::string &argument)
{
    bool alphabetical = true;
    if (argument.empty())
    {
        alphabetical = false;
    }
    else
    {
        for (char c : argument)
        {
            if (!std::isalpha(static_cast<unsigned char>(c))){
                alphabetical = false;
            }
        }
    }

    return alphabetical;
}

std::string to_upper(std::string argument) {
    for (size_t i = 0; i < argument.length(); ++i) {
        argument[i] = std::toupper(static_cast<unsigned char>(argument[i]));
    }
    return argument;
}

std::string get_string(const std::string &message)
{
    std::string argument;

    while(true)
    {
        std::cout << message;
        std::getline(std::cin, argument);

        if (argument.empty())
        {
            std::cout << "Input cannot be empty\n";
        } 
        else if (!is_alpha(argument))
        {
            std::cout << "Input must be alphabetical, with no spaces or numbers\n";
        }
        else
        {
            return to_upper(argument);
        }
    }
}

std::string pull_from_history()
{
    if (history.empty())
    {
        std::cout << "History is empty\n";
        return "";
    }

    std::cout << "\n\nHistory: \n";
    for (const auto &pair: history)
    {
        std::cout << "\t" << pair.first << " " << pair.second << "\n";
    }

    std::cout << "\n\nEnter the digit for the string you want to use: ";

    std::string argument;
    if (!std::getline(std::cin, argument))
    {
        return "";
    }

    // get string with desired key (index)
    try {
        int index = std::stoi(argument);
        auto itr = history.find(index);
        if (itr != history.end()) 
        {
            return itr->second;
        } 
        else 
        {
            std::cout << "Invalid index: " << argument << "\n";
            return "";
        }
    } 
    catch (const std::exception& e) 
    {
        std::cout << "Index must be a number\n";
        return "";
    }
}

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename.txt>" << std::endl;
        exit(-1);
    }
    
    std::string filename = argv[1];

    // To store pipe identifiers
	// p1[0] - read from pipe
	// p1[1] - write to pipe
    int p1[2], p2[2], p3[2];
	pid_t encryption, logger; // To store the process id

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
		close(p3[0]);
        close(p3[1]); // do not need to write to logger directly

		dup2(p1[0], 0); // map pipe to stdin
		dup2(p2[1], 1); // map pipe to stdout
        // dup2(p2[1], 2); // map pipe to stdout (stderr)

        close(p1[0]);
        close(p2[1]);

		char *args[] = {"./encryption",NULL};
		execvp(args[0],args); // replace this program with the encryption program
        perror("Encryption failed");
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
        close(p3[0]);

		char *args[] = {"./logger", (char*)filename.c_str(), NULL};
		execvp(args[0],args); // replace this program with the logger program
        perror("Logger failed");
        exit(1);
	}

    // establish driver process
    close(p1[0]);
    close(p2[1]);
    close(p3[0]);

    // open read end of p2
    from_encryption = fdopen(p2[0], "r");
    if (from_encryption == NULL)
    {
        std::cerr << "Failed to open pipe from encryption" << std::endl;
        exit(-1);
    }

    // open write end of p3
    to_logger = fdopen(p3[1], "w");
    if (to_logger == NULL)
    {
        std::cerr << "Failed to open pipe to logger" << std::endl;
        exit(-1);
    }

    setlinebuf(to_logger);

    // menu
    std::string command, argument, password;
    std::cout << "\n\n" << command_menu << "\n\n";

    // write program start
    fprintf(to_logger, "START Driver program started\n");
    fflush(to_logger);
    while (true)
    {
        std::cout << "\nEnter command:";
        if (!std::getline(std::cin, command))
        {
            std::cout << "End of file reached" << std::endl;
            break;
        }

        for (char &c : command) 
        {
            c = std::tolower(static_cast<unsigned char>(c));
        }

        fprintf(to_logger, "COMMAND %s\n", command.c_str());

        // menu options
        if (command == "quit")
        {
            if (write(p1[1], "QUIT\n", 5) == -1)
            {
                std::cerr << "Failed to write to pipe" << std::endl;
            } 
            fprintf(to_logger, "QUIT Driver program ended\n");
            fflush(to_logger);
            fclose(to_logger);
            break;
        }
        else if (command == "password")
        {
            std::cout << "\nType '1' to use a past string"
                            "\nOr type '2' to create a new one: ";
            std::string selection;
            std::getline(std::cin, selection);
            if (selection == "1")
            {
                argument = pull_from_history();
                if (argument.empty())
                {
                    std::cout << "Password not found\n";
                    continue;
                } 

                password = argument;
            }
            else if (selection == "2")
            {
                password = get_string("\nEnter password string: ");
                if (password.empty()) continue;
            } 
            else 
            {
                std::cout<< "\nInvalid input. Please try again.";
                continue;
            }

            // send to encryption program
            std::string line = "PASS " + password + "\n";
            write(p1[1], line.c_str(), line.size());

            if (fgets(buffer, sizeof(buffer), from_encryption) != NULL)
            {
                buffer[strcspn(buffer, "\n")] = 0; 
                std::cout << buffer << std::endl;
                fprintf(to_logger, "RESULT %s\n", buffer);
            }

        }
        else if (command == "encrypt")
        {
            std::cout << "\nType '1' to use a past string"
                            "\nOr type '2' to create a new one: ";
            std::string selection;
            if (!std::getline(std::cin, selection)) continue;

            if (selection == "1")
            {
                argument = pull_from_history();
                if (argument.empty())
                {
                    std::cout << "Argument is empty\n";
                    continue;
                }
            }
            else if (selection == "2")
            {
                argument = get_string("Enter string to encrypt: ");
                if (argument.empty()) continue;

                // add entered string to history
                history_idx++;
                history.insert({history_idx, argument});
            }
            else 
            {
                std::cout<< "\nInvalid input. Please try again.";
            }

            // send to encryption program
            std::string line = "ENCRYPT " + argument + "\n";
            write(p1[1], line.c_str(), line.size());

            if (fgets(buffer, sizeof(buffer), from_encryption) != NULL)
            {
                buffer[strcspn(buffer, "\n")] = 0;
                std::cout << buffer << std::endl;
                fprintf(to_logger, "RESULT %s\n", buffer);

                if (strncmp(buffer, "RESULT", 6) == 0)
                {
                    std::string result = buffer;
                    history_idx++;
                    history.insert({history_idx, result.substr(7)});
                }
            }
        } 
        else if (command == "decrypt")
        {
            std::cout << "\nType '1' to use a past string"
                            "\nOr type '2' to create a new one: ";
            std::string selection;
            if (!std::getline(std::cin, selection)) continue;

            if (selection == "1")
            {
                argument = pull_from_history();
                if (argument.empty())
                {
                    std::cout << "Argument is empty\n";
                    continue;
                }
            }
            else if (selection == "2")
            {
                argument = get_string("Enter string to decrypt: ");
                if (argument.empty()) continue;

                // add entered string to history
                history_idx++;
                history.insert({history_idx, argument});

            } else {
                std::cout<< "\nInvalid input. Please try again.";
                continue;
            }

            // send to encryption program
            std::string line = "DECRYPT " + argument + "\n";
            write(p1[1], line.c_str(), line.size());

            if (fgets(buffer, sizeof(buffer), from_encryption) != NULL)
            {
                buffer[strcspn(buffer, "\n")] = 0;
                std::cout << buffer << std::endl;
                fprintf(to_logger, "RESULT %s\n", buffer);

                if (strncmp(buffer, "RESULT", 6) == 0)
                {
                    std::string result = buffer;
                    history_idx++;
                    history.insert({history_idx, result.substr(7)});
                }
            }
        }
        else if (command == "history")
        {
            if (history.empty())
            {
                std::cout << "History is empty\n";
            }
            else
            {
                std::cout << "\n\nHistory: \n";
                for (const auto &pair: history)
                {
                    std::cout << pair.first << " " << pair.second << "\n";
                }
                std::cout << "\n";
            }
        }
        else
        {
            std::cout << "Invalid command\n";
            fprintf(to_logger, "ERROR Invalid command inputted\n");
        }
    }

    fclose(from_encryption);
    close(p1[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}