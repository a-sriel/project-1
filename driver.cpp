#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>               /* UNIX and POSIX constants and functions (fork, pipe) */

int main(int argc, const char** argv)
{
    // To store pipe identifiers
	// p1[0] - read from pipe
	// p1[1] - write to pipe
    int p1[2],p2[2];
	pid_t child; // To store the process id

	if(pipe(p1) == -1)
	{
		std::cerr << "Unable to create first pipe" << std::endl;
		exit(-1);
	}

	if(pipe(p2) == -1)
	{
		std::cerr << "Unable to create second pipe" << std::endl;
		exit(-1);
	}

	child = fork();

    if(child == -1)
	{
		std::cerr << "Error forking" << std::endl;
		exit(-1);
	}

	if(child == 0)
	{
		//I am the child process
		close(p1[1]); // p1 is for reading
		close(p2[0]); // p2 is for writing
		dup2(p1[0], 0); //map pipe to stdin
		dup2(p2[1], 1); //map pipe to stdout

		char *args[] = {"./mem",NULL};
		execvp(args[0],args); //replace this program with the mem program
	}
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