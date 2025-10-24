#include <iostream>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>

int main(int argc, const char** argv, std::string filename)
{
    time_t timestamp = time(NULL);
    std::string 

    std::cout << 
    timestamp.tm_year + "-" + timestamp.tm_month + "-" timestamp.tm_day +
    " " + timestamp.tm_hour + ":" + timestamp.tm_min;

    std::string line;
    std::ofstream out(filename);

    while(std::getline(std::cin, line))
    {
        out << line << std::endl;
    }

    out.close();

    return 0;
}
