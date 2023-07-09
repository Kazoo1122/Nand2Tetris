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

    Parser *first_parser = new Parser(asm_file);;
    Code *bin_code = new Code();
    SymbolTable *symbol_table = new SymbolTable();

    std::cout << "----------------------------------------------" << std::endl;
    // First pass to read the program lines for adds the found labels to symbol table.
    while (first_parser->has_more_lines())
    {
        first_parser->advance();
        auto instruction_type = first_parser->instruction_type();
        if (instruction_type == Instruction::L_INSTRUCTION)
        {
            std::string symbol = first_parser->symbol();
            std::cout << first_parser->get_current_line_no() << std::endl;
            std::cout << "Symbol : " << symbol << std::endl;
            std::cout << "Line no: " << std::to_string(first_parser->get_current_line_no()) << std::endl;
            if(!symbol_table->contains(symbol))
            {
                symbol_table->add_entry(symbol, first_parser->get_current_line_no());
            }
        }
        std::cout << "----------------------------------------------" << std::endl;
    }

    Parser *second_parser = new Parser(asm_file);;
    auto latest_address = FIRST_VAR_ADDRESS;

    std::cout << "==============================================" << std::endl;
    while (second_parser->has_more_lines())
    {
        second_parser->advance();
        auto instruction_type = second_parser->instruction_type();
        std::cout << "Line no: " << std::to_string(second_parser->get_current_line_no()) << std::endl;

        if (instruction_type == Instruction::A_INSTRUCTION)
        {
            std::string symbol = second_parser->symbol();
            std::cout << "symbol: " + symbol << std::endl;
            if (symbol.empty())
            {
                auto decimal = second_parser->decimal();
                auto binary = second_parser->binary(decimal);
                std::cout << "A: " << binary << std::endl;
                ofs << binary << std::endl;
            }
            else
            {
                if(symbol_table->contains(symbol))
                {
                    auto address = symbol_table->get_address(symbol);
                    auto binary = second_parser->binary(std::to_string(address));
                    std::cout << "A: " << binary << std::endl;
                    ofs << binary << std::endl;
                }
                else
                {
                    symbol_table->add_entry(symbol, latest_address++);
                }
            }
        }
        else if (instruction_type == Instruction::C_INSTRUCTION)
        {
            std::string dest = "";
            std::string comp = "";
            std::string jump = "";
            std::cout << "Parser dest: " + second_parser->dest() << std::endl;
            std::cout << "Parser comp: " + second_parser->comp() << std::endl;
            std::cout << "Parser jump: " + second_parser->jump() << std::endl;
            dest = bin_code->dest(second_parser->dest());
            comp = bin_code->comp(second_parser->comp());
            jump = bin_code->jump(second_parser->jump());
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
