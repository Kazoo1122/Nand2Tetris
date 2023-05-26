#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <iomanip>

int main(int argc, char **argv)
{
    std::string asm_file = argv[1];
    std::ifstream ifs(asm_file);
    if (ifs.fail())
    {
        std::cerr << "Failed to open the received file." << std::endl;
        return -1;
    }
    std::string line;
    int i = 0;
    std::string row_num;
    std::stringstream ss;
    while (getline(ifs, line))
    {
        row_num = std::to_string(++i);
        ss << std::setw(2) << std::setfill('0') << row_num;
        std::cout << ss.str() + ": " << line << std::endl;
        ss.str("");
        ss.clear(std::stringstream::goodbit);
    }
    std::cout << "end" << std::endl;
    return 0;
}