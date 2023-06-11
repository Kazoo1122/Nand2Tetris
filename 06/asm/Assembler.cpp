#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "include/Parser.h"
#include "include/Code.h"
#include "include/SymbolTable.h"

auto const FIRST_VAR_ADDRESS = 16;
auto const C_INSTRUCTION_OP_CODE = "000";

int main(int argc, char **argv)
{
    auto asm_file = argv[1];

    Parser *parser;
    Code *bin_code;
    SymbolTable *symbol_table;

    parser = new Parser(argv[1]);
    bin_code = new Code();
    symbol_table = new SymbolTable();

    // First pass to read the program lines for adds the found labels to symbol table.
    while (parser->has_more_lines())
    {
        parser->advance();
        auto instruction_type = parser->instruction_type();
        if (instruction_type == Instruction::L_INSTRUCTION)
        {
            std::string symbol = parser->symbol();
            if(!symbol_table->contains(symbol))
            {
                symbol_table->add_entry(symbol, parser->get_current_line_no());
            }
        }
    }

    // std::vector<std::string> keys;
    // for (const auto& [k, v]: symbol_table->table)
    // {
    //     keys.push_back(k);
    // }
    // for (const auto& key: keys)
    // {
    //     auto address = symbol_table->get_address(key);
    //     std::cout << key + ": " << address << std::endl;
    // }

    auto latest_address = FIRST_VAR_ADDRESS;

    while (parser->has_more_lines())
    {
        parser->advance();
        auto instruction_type = parser->instruction_type();
        std::cout << "line no: " << parser->get_current_line_no() << std::endl;
        std::cout << "instruction: " << (int) instruction_type << std::endl;

        if (instruction_type == Instruction::A_INSTRUCTION)
        {
            std::string symbol = parser->symbol();
            if (!symbol.empty())
            {
                if(!symbol_table->contains(symbol))
                {
                    symbol_table->add_entry(symbol, ++latest_address);
                }
                else
                {
                    std::cout << "A: " << parser->decimal_number() << std::endl;
                }
            }
            else
            {
                std::cout << "A: " << symbol_table->get_address(symbol) << std::endl;
            }
        } else if (instruction_type == Instruction::C_INSTRUCTION)
        {
            std::string dest = "";
            std::string comp = "";
            std::string jump = "";
            dest = bin_code->dest(parser->dest());
            comp = bin_code->comp(parser->comp());
            jump = bin_code->jump(parser->jump());
            std::cout << "C: " << C_INSTRUCTION_OP_CODE + dest + comp + jump << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
