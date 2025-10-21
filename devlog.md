2025-10-18 19:55

For this first project I need to develop three key components:
1. A logger that tracks all activity as lines of tex. Each command and the result of each command must be logged, as well as the start and exit of the driver.
2. An encryption program that handles encrypting and decrypting an argument using a Vigenere cipher
    The cipher requires:
        1. Plaintext argument
        2. A key
    Each letter of the argument is encoded with a different Caesar cipher, with the increment being determined by the key (each letter's position in the alphabet is the shift amount applied to the plaintext argument)
3. A driver program, which accepts the name of the log file, and creates two new processes, executing the logger and the encryption program. It will also display a commands menu to the user.

I am planning to get the logger operational first. I'm not worried about getting the commands in the driver operational for awhile, I simply want to ensure that each argument provided in the driver is being kept track of in a separate file by the logger. Later on, I can work on the command logic, and finally implement the Vigenere cipher logic.

I am rusty on pipes so I will need to brush up on using read and write to communicate over pipes, as this project will be completed in C++, which necessitates the read and write commands.

2025-10-29 16:51

I realized that in order to see that the logger is functional, the driver needs to communicate with it first, so I need to focus on getting the processes set up properly in the driver.

My plan for this session is to set up a parent and child process, allow a user input to be read through a pipe, and then output some messages stating whether or not it was successful.

I am still trying to get the core functionality of the driver down. I want the user to input the name of the logger file and have that be read through a pipe. I have some basic logic for this down, but it is currently causing a segmentation fault. I will need to refine this next session, and then I can move on to actually sending some arguments to the external logger.

