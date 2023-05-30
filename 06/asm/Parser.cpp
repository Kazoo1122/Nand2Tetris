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
    return Parser::lines.size() - 1 > Parser::current_line_no;
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
        Parser::current_instruction = tmp;
        break;
    }
};

Instruction Parser::instruction_type()
{
    if (Parser::is_A_instruction(false))
    {
        return Instruction::A_INSTRUCTION;
    } else if (Parser::is_L_instruction())
    {
        return Instruction::L_INSTRUCTION;
    } else {
        return Instruction::C_INSTRUCTION;
    }
}

std::string Parser::symbol()
{
    if (Parser::is_A_instruction(true))
    {
        return Parser::current_instruction.substr(1);
    } else if (Parser::is_L_instruction())
    {
        return Parser::current_instruction.substr(1, Parser::current_instruction.length() - 2);
    }
    return "";
}

bool Parser::is_A_instruction(bool is_symbol_only)
{
    auto re = std::regex(is_symbol_only ? R"(^@\D+)" : R"(^@.*)");
    return std::regex_search(Parser::current_instruction, re);
}

bool Parser::is_L_instruction()
{
    return std::regex_search(Parser::current_instruction, std::regex(R"(^\(.*\)$)"));
}

std::string Parser::dest()
{

}

std::string Parser::comp()
{

}

std::string Parser::jump()
{

}
