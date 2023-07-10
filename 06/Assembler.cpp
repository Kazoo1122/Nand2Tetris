#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "include/Parser.h"
#include "include/Code.h"
#include "include/SymbolTable.h"

auto const FIRST_VAR_ADDRESS = 16;
auto const C_INSTRUCTION_OP_CODE = "111";

int main(int argc, char **argv)
{
    std::string asm_file = argv[1];
    auto file_name = asm_file.substr(0, asm_file.find(".asm")) + ".hack";
    std::ofstream ofs(file_name);
    if (!ofs)
    {
        std::cout << "The output file can't open." << std::endl;
        return EXIT_FAILURE;
    }

    Parser *parser = new Parser(asm_file);;
    Code *bin_code = new Code();
    SymbolTable *symbol_table = new SymbolTable();

    // First pass to read the program lines for adds the found labels to symbol table.
    while (parser->has_more_lines())
    {
        parser->advance();
        auto instruction_type = parser->instruction_type();
        if (instruction_type == Instruction::L_INSTRUCTION)
        {
            std::string symbol = parser->symbol();
            parser->remove_instruction();
            std::cout << "----------------------------------------------" << std::endl;
            std::cout << "Symbol : " << symbol << std::endl;
            std::cout << "Line no: " << std::to_string(parser->get_current_line_no()) << std::endl;
            if(!symbol_table->contains(symbol))
            {
                symbol_table->add_entry(symbol, parser->get_current_line_no());
            }
        }
    }

    auto latest_address = FIRST_VAR_ADDRESS;
    parser->init();

    std::cout << "==============================================" << std::endl;
    while (parser->has_more_lines())
    {
        parser->advance();
        auto instruction_type = parser->instruction_type();
        std::cout << "Line no: " << std::to_string(parser->get_current_line_no()) << std::endl;

        if (instruction_type == Instruction::A_INSTRUCTION)
        {
            std::string symbol = parser->symbol();
            std::cout << "Symbol: " + symbol << std::endl;
            if (symbol.empty())
            {
                auto decimal = parser->decimal();
                auto binary = parser->binary(decimal);
                std::cout << "A: " << binary << std::endl;
                ofs << binary << std::endl;
            }
            else
            {
                int address;
                std::string binary;
                if(symbol_table->contains(symbol))
                {
                    address = symbol_table->get_address(symbol);
                }
                else
                {
                    address = latest_address;
                    symbol_table->add_entry(symbol, latest_address++);
                }
                binary = parser->binary(std::to_string(address));
                ofs << binary << std::endl;
                std::cout << "A: " << binary << std::endl;
            }
        }
        else if (instruction_type == Instruction::C_INSTRUCTION)
        {
            std::string dest = "";
            std::string comp = "";
            std::string jump = "";
            std::cout << "Parser dest: " + parser->dest() << std::endl;
            std::cout << "Parser comp: " + parser->comp() << std::endl;
            std::cout << "Parser jump: " + parser->jump() << std::endl;
            dest = bin_code->dest(parser->dest());
            comp = bin_code->comp(parser->comp());
            jump = bin_code->jump(parser->jump());
            std::cout << "Code   dest: " + dest << std::endl;
            std::cout << "Code   comp: " + comp << std::endl;
            std::cout << "Code   jump: " + jump << std::endl;
            auto binary = C_INSTRUCTION_OP_CODE + comp + dest + jump;
            std::cout << "C: " << binary << std::endl;
            ofs << binary << std::endl;
        }
        else if (instruction_type == Instruction::L_INSTRUCTION)
        {
            std::cout << "L instruction is skipped when the second pass." << std::endl;
        }
        std::cout << "==============================================" << std::endl;
    }
    return EXIT_SUCCESS;
}
