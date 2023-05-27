#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

int main(int argc, char **argv)
{
    auto asm_file = argv[1];
    std::ifstream ifs(asm_file);
    if (ifs.fail())
    {
        std::cerr << "Failed to open the received file." << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<std::string> lines;
    std::string line;
    std::map<std::string, int> symbol_table = {
        {}
    };
    while (getline(ifs, line))
    {
    }
    std::cout << "end" << std::endl;
    return EXIT_SUCCESS;
}