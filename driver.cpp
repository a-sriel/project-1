#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>               /* UNIX and POSIX constants and functions (fork, pipe) */
#include <string>

std::string history;

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
    /*
	else
	{
		//I am the parent
		close(p1[0]);
		close(p2[1]);

		char tmp;

		for(int i=10; i>0; i--)
		{
            tmp = 1; //write command
			write(p1[1], &tmp, 1);
			write(p1[1], (char*)&i, sizeof(int));
			std::cout << "Set to ";
			tmp = 0; //Read Command
			write(p1[1], &tmp, 1);
			int val;
			read(p2[0], (char*)&val, sizeof(int));
			std::cout << val << std::endl;

		}

        tmp = 2; // Halt Mem Command
		write(p1[1], &tmp, 1);
        wait(NULL);
	}
    */

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

    // menu
    std::string command, passkey;

    while (command != "quit")
    {
        std::cout << "\nEnter command:";
        std::getline(std::cin, command);

        // menu options
        if (command == "quit")
        {
            write(p1[1], "quit\n", 5);
        }
        if (command == "password")
        {

        }
        if (command == "encrypt")
        {
            std::cout << "\nType '1' to use a past string"
                            "\nOr type '2' to create a new one";
            std::string selection;
            std::getline(std::cin, selection);
            if (std::stoi(selection) == 1)
            {

            }
            else if (std::stoi(selection) == 2)
            {
                std::cout << "\nEnter string: ";
                std::getline(std::cin, passkey);

                // send to encryption program
                std::string line = "[ENCRYPT]" + passkey + "\n";
                write(p1[1], line.c_str(), line.size());
                history += command + "\n";

            } else {
                std::cout<< "\nInvalid input. Please try again.";
            }

            write(p1[1], passkey.c_str(), passkey.size());
        }
        if (command == "decrypt")
        {

        }
        if (command == "history")
        {
            std::cout << history;
        }
    }

    close(p1[1]);
    close(p2[0]);

    wait(NULL);

    /*
    int fd[2];
    // fd[0] = read
    // fd[1] = write
    if (pipe(fd) == -1)
    {
        printf("Failed to open pipe\n");
        return 1;
    }

    int pid = fork();
    // parent process
    if (pid == 0)
    {
        close(fd[0]);
        
        std::string str;
        printf("Input name of logger file: ");
        scanf("%s", &str);
        write(fd[1], &str, sizeof(std::string));
        close(fd[1]);
    }
    // child process
    else
    {
        close(fd[1]);
        std::string str_2;
        read(fd[0], &str_2, sizeof(std::string));
        close(fd[0]);
        printf("Obtained from child process %s\n", &str_2);
    }
    */
    return 0;
}