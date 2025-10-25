Files:
1) driver.cpp - the driver program, gives the user a menu of commands to choose from and serves as the communication baseline for the rest of the program. all encrypted/decrypted strings are stored in a temporary history that terminates at the end of each session. for a permanent string history, check the txt file that the logger logs to.

2) encryption.cpp - contains the actual encryption logic. arguments from the driver are sent here and are encrypted/decrypted as desired.

3) logger.cpp - logs everything to an external txt file contained within the same folder as all program files.

4) devlog.md - a journal containing all of my thoughts and timestamped work sessions throughout the course of this project

5) readme.txt - you are here. tells you everything you need to know (I hope)

For compilation:
driver.cpp, encryption.cpp, and logger.cpp must be compiled as separate files using g++ and -std=c++11 (or higher)

For execution:
create a txt file that you want the log to be written to
simply run ./driver <filename.txt>
	(replace <filename.txt> with the txt file you want the log stored in)

Tools used:
files were compiled, run, and tested on the cs1 utdallas server using MobaXterm, but any UNIX terminal should work
all code was written (but not tested) in VS code with c++ extensions

**NOTE:
Professor gave me an extension on this project due to extenuating circumstance, hence the late submission