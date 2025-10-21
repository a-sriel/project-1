#include <cstdlib>
#include <cstdio>
#include <array>
#include <iostream>
#include <string>
#include <unistd.h>

int main(int argc, char* argv[])
{
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
    return 0;
}