#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "include/Parser.h"

int main(int argc, char **argv)
{
    auto asm_file = argv[1];
    Parser parser(argv[1]);
    while (parser.has_more_lines())
    {
        parser.advance();
        if (parser.instruction_type() == Instruction::C_INSTRUCTION) {
            std::cout << "dest: " + parser.dest() << std::endl;
            std::cout << "comp: " + parser.comp() << std::endl;
            std::cout << "jump: " + parser.jump() << std::endl;
            std::cout << "------------------" << std::endl;
        }
    }
    // std::ifstream ifs(asm_file);
    // if (ifs.fail())
    // {
    //     std::cerr << "Failed to open the received file." << std::endl;
    //     return EXIT_FAILURE;
    // }
    // std::vector<std::string> lines;
    // std::string line;
    // std::map<std::string, int> symbol_table = {
    //     {}
    // };
    // while (getline(ifs, line))
    // {
    // }
    // std::cout << "end" << std::endl;
    return EXIT_SUCCESS;
}