#include <string>
#include "include/SymbolTable.h"

SymbolTable::SymbolTable()
{
    // Set predefined symbols
    SymbolTable::table = {
        {"R0", 0},
        {"R1", 1},
        {"R2", 2},
        {"R3", 3},
        {"R4", 4},
        {"R5", 5},
        {"R6", 6},
        {"R7", 7},
        {"R8", 8},
        {"R9", 9},
        {"R10", 10},
        {"R11", 11},
        {"R12", 12},
        {"R13", 13},
        {"R14", 14},
        {"R15", 15},
        {"SP", 0},
        {"LCL", 1},
        {"ARG", 2},
        {"THIS", 3},
        {"THAT", 4},
        {"SCREEN", 16384},
        {"KBD", 24576},
    };
}

void SymbolTable::add_entry(std::string symbol, int address)
{
    SymbolTable::table[symbol] = address;
}

bool SymbolTable::contains(std::string symbol)
{
    return SymbolTable::table.count(symbol) > 0;
}

int SymbolTable::get_address(std::string symbol)
{
    return SymbolTable::table[symbol];
}
