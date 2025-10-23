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

2025-10-19 16:51

I realized that in order to see that the logger is functional, the driver needs to communicate with it first, so I need to focus on getting the processes set up properly in the driver.

My plan for this session is to set up a parent and child process, allow a user input to be read through a pipe, and then output some messages stating whether or not it was successful.

I am still trying to get the core functionality of the driver down. I want the user to input the name of the logger file and have that be read through a pipe. I have some basic logic for this down, but it is currently causing a segmentation fault. I will need to refine this next session, and then I can move on to actually sending some arguments to the external logger.

2025-10-23 12:12

I am encountering difficulty implementing my driver.cpp program. I was struggling with understanding how the driver could communicate with a separate logger and encryption program via pipes, but upon looking at the process files provided with this project, I realized I could dedicate one pipe p1 to represent the driver-encryption pipeline, and pipe p2 to represent the encryption-logger pipeline.

I am going to push the provided process files (cpu.cpp and mem.cpp) to this project and determine a way to incorporate the cpu and memory logic into my driver program... It shouldn't be too difficult now that I have a better grasp of how communication over pipes works.

I have copied over the cpu logic to the driver program, and have commented out my prior logic. I have established three pipes, and have created forks and established the child process for the encryption program. The child process for the logger has now been established in a near-identical fashion. I suspect that I may need to rearrange the order of the close and dup2 calls, but I will see how it goes in the testing stage.

2025-10-23 14:33

I accomplished a lot during this session, and I implemented some of the core logic for the driver program (communication with the encryption and logger programs, and forking to create new processes). I established three pipes and I believe I have a clear idea of how each of them communicate between programs. For the next session, I plan to finish the driver program and test how it actually communicates between files. To do this, I need to establish the driver process. I plan to open/close the respective pipes and implement the menu program (in some form) using a while loop. I also need to create the logger.cpp and encryption.cpp files. 