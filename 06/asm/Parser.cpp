#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include "include/Parser.h"

Parser::Parser(std::string file_path):
    current_instruction("")
    , current_line_no(0)
    , lines({})
{
    std::ifstream ifs(file_path);
    if (ifs.fail())
    {
        std::cerr << "Failed to open the received file." << std::endl;
        return;
    };
    std::string line;
    while(getline(ifs, line))
    {
        Parser::lines.push_back(line);
    };
};

bool Parser::has_more_lines()
{
    return Parser::lines.size() > Parser::current_line_no;
};

void Parser::advance()
{
    while (Parser::has_more_lines())
    {
        auto tmp = Parser::lines[++Parser::current_line_no];
        tmp = std::regex_replace(tmp, std::regex("(//.*)"), "");
        tmp.erase(std::remove(tmp.begin(), tmp.end(), ' '), tmp.end());
        // if the current instruction has only whitespace, then skip.
        if (std::regex_match(tmp, std::regex(R"(^\s*?$(\r\n|\r|\n?)?)")))
        {
            continue;
        }
        std::cout << tmp << std::endl;
        Parser::current_instruction = tmp;
        break;
    }
};

Instruction Parser::instruction_type()
{
    if (std::regex_match(Parser::current_instruction, std::regex(R"(^@.*)")))
    {
        return Instruction::A_INSTRUCTION;
    } else if (std::regex_match(Parser::current_instruction, std::regex(R"(\(.?\))")))
    {
        return Instruction::L_INSTRUCTION;
    } else {
        return Instruction::C_INSTRUCTION;
    }
}
