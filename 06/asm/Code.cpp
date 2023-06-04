#include <iostream>
#include <string>
#include <bitset>
#include <regex>
#include "include/Code.h"

Code::Code()
{
    Code::dest_map = {
        std::pair{'M', 0b1},
        std::pair{'D', 0b10},
        std::pair{'A', 0b100},
    };
    Code::comp_map = {
        std::pair{"a",  0b1000000},
        std::pair{"zx", 0b0100000},
        std::pair{"nx", 0b0010000},
        std::pair{"zy", 0b0001000},
        std::pair{"ny", 0b0000100},
        std::pair{"f",  0b0000010},
        std::pair{"no", 0b0000001},
    };
    Code::jump_map = {
        std::pair{'G', 0b1},
        std::pair{'E', 0b10},
        std::pair{'L', 0b100},
    };
};
std::string Code::dest(std::string dest)
{
    auto bin = 0b0;
    bin = dest.find('M') != std::string::npos ? bin + Code::dest_map['M'] : bin;
    bin = dest.find('D') != std::string::npos ? bin + Code::dest_map['D'] : bin;
    bin = dest.find('A') != std::string::npos ? bin + Code::dest_map['A'] : bin;
    return std::bitset<3>(bin).to_string();
}

std::string Code::comp(std::string comp)
{
    auto bin = 0b0;
    // The most left bit is 1 or 0(M or others)?
    if (comp.find('M') != std::string::npos)
    {
        bin += Code::comp_map["a"];
    }

    if (std::regex_search(comp, std::regex(R"(D\&(A|M))")))
    {
    }
    else if (std::regex_search(comp, std::regex(R"(D\|(A|M))")))
    {
        bin += Code::comp_map["nx"] + Code::comp_map["ny"] + Code::comp_map["no"];
    }
    else if (std::regex_search(comp, std::regex(R"(D\+(A|M))")))
    {
        bin += Code::comp_map["f"];
    }
    else if (std::regex_search(comp, std::regex(R"(D\-(A|M))")))
    {
        bin += Code::comp_map["ny"] + Code::comp_map["f"] + Code::comp_map["no"];
    }
    else
    {
        // skip return if any check wasn't passed.
        goto CONSTANTS;
    }
    return std::bitset<7>(bin).to_string();

    CONSTANTS:
    // Is comp a constant value?
    try {
        switch (stoi(comp))
        {
            case 0:
                bin = Code::comp_map["zx"] + Code::comp_map["zy"]
                    + Code::comp_map["f"];
                break;
            case 1:
                bin = Code::comp_map["zx"] + Code::comp_map["zy"]
                    + Code::comp_map["nx"] + Code::comp_map["ny"]
                    + Code::comp_map["f"]  + Code::comp_map["no"];
                break;
            case -1:
                bin = Code::comp_map["zx"] + Code::comp_map["zy"]
                    + Code::comp_map["nx"] + Code::comp_map["f"];
            default:
                return "";
        }
        return std::bitset<7>(bin).to_string(); 
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << "comp was not int." << std::endl;
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "comp value was out of range." << std::endl;
        return "";
    }
}

std::string Code::jump(std::string jump)
{
    auto bin = 0b0;
    bin = jump.find('G') != std::string::npos ? bin + Code::jump_map['G'] : bin;
    bin = jump.find('L') != std::string::npos ? bin + Code::jump_map['L'] : bin;
    if (jump.find('E') != std::string::npos)
    {
        if (jump.find('N') != std::string::npos)
        {
            return std::bitset<3>(Code::jump_map['G'] + Code::jump_map['L']).to_string();
        }
        return std::bitset<3>(bin + Code::jump_map['E']).to_string();
    }
    bin = jump == "JMP" ? Code::jump_map['G'] + Code::jump_map['L'] + Code::jump_map['E'] : bin;
    return std::bitset<3>(bin).to_string();
}