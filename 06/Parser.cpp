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
        line = std::regex_replace(line, std::regex("(//.*)"), "");
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        // if the current instruction has only whitespace, then skip.
        if (std::regex_match(line, std::regex(R"(^\s*?$(\r\n|\r|\n?)?)"))) continue;
        // if is linefeed code crlf, then trim cr.
        if (*(line.end() - 1) == 0x0d) line.pop_back();
        Parser::lines.push_back(line);
    };
};


bool Parser::has_more_lines()
{
    return Parser::lines.size() > Parser::current_line_no;
}

void Parser::advance()
{
    if (Parser::has_more_lines())
    {
        Parser::current_instruction = Parser::lines[++Parser::current_line_no - 1];
    }
}

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
    }
    else if (Parser::is_L_instruction())
    {
        return Parser::current_instruction.substr(
            Parser::current_instruction.find('(') + 1,
            Parser::current_instruction.find(')') - 1
        );
    }
    return "";
}

std::string Parser::decimal()
{
    if (Parser::is_A_instruction(false) && !Parser::is_A_instruction(true))
    {
        return Parser::current_instruction.substr(1);
    }
    return "";
}

std::string Parser::binary(std::string decimal)
{
    auto const A_INSTRUCTION_OP_CODE = '0';
    return A_INSTRUCTION_OP_CODE + std::bitset<15>(stoi(decimal)).to_string();
}

bool Parser::is_A_instruction(bool is_symbol_only)
{
    auto re = std::regex(is_symbol_only ? R"(^@\D+)" : R"(^@.*)");
    return std::regex_search(Parser::current_instruction, re);
}

bool Parser::is_L_instruction()
{
    return std::regex_search(Parser::current_instruction, std::regex(R"(\(.*\))"));
}

std::string Parser::dest()
{
    auto assignment_offset = Parser::current_instruction.find('=');
    return assignment_offset == std::string::npos ? ""
        : Parser::current_instruction.substr(0, assignment_offset);
}

std::string Parser::comp()
{
    auto assignment_offset = Parser::current_instruction.find('=');
    auto delimiter_offset = Parser::current_instruction.find(';');
    auto start_pos = assignment_offset == std::string::npos ? 0
        : ++assignment_offset;
    auto end_pos = delimiter_offset == std::string::npos ? Parser::current_instruction.length()
        : delimiter_offset;
    return Parser::current_instruction.substr(start_pos, end_pos);
}

std::string Parser::jump()
{
    auto delimiter_offset = Parser::current_instruction.find(';');
    return delimiter_offset == std::string::npos ? ""
        : Parser::current_instruction.substr(++delimiter_offset, Parser::current_instruction.length());
}

int Parser::get_current_line_no()
{
    return Parser::current_line_no;
}

void Parser::remove_instruction()
{
    Parser::lines.erase(std::cbegin(Parser::lines) + --Parser::current_line_no);
}

void Parser::init()
{
    Parser::current_instruction = "";
    Parser::current_line_no = 0;
}
