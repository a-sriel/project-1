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