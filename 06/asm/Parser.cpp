#include <string>
#include <iostream>
#include <fstream>
#include "include/Parser.h"

Parser::Parser(std::string file_path)
{
    std::ifstream ifs(file_path);
    if (ifs.fail())
    {
        std::cerr << "Failed to open the received file." << std::endl;
        Parser::file_content = "";
        return;
    };
    Parser::file_content = std::string(
        std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>()
    );
    std::cout << Parser::file_content << std::endl;
};
