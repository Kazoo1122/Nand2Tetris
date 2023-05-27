#include "include/SymbolTable.h"

SymbolTable::SymbolTable()
{
    // Set predefined symbols
    SymbolTable::table = {
        std::pair{"R0", 0},
        std::pair{"R1", 1},
        std::pair{"R2", 2},
        std::pair{"R3", 3},
        std::pair{"R4", 4},
        std::pair{"R5", 5},
        std::pair{"R6", 6},
        std::pair{"R7", 7},
        std::pair{"R8", 8},
        std::pair{"R9", 9},
        std::pair{"R10", 10},
        std::pair{"R11", 11},
        std::pair{"R12", 12},
        std::pair{"R13", 13},
        std::pair{"R14", 14},
        std::pair{"R15", 15},
        std::pair{"SP", 0},
        std::pair{"LCL", 1},
        std::pair{"ARG", 2},
        std::pair{"THIS", 3},
        std::pair{"THAT", 4},
        std::pair{"SCREEN", 16384},
        std::pair{"KBD", 24576},
    };
};