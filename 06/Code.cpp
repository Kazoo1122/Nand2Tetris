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

    // is logical and?
    if (std::regex_search(comp, std::regex(R"(D&(A|M))")))
    {
        // then remain the binary is 0.
    }
    else if (std::regex_search(comp, std::regex(R"(D\|(A|M))")))
    {
        bin += Code::comp_map["nx"];
        bin += Code::comp_map["ny"];
        bin += Code::comp_map["no"];
    }
    // is addition?
    else if (std::regex_search(comp, std::regex(R"(D\+(A|M))")))
    {
        bin += Code::comp_map["f"];
    }
    // is D subtracted?
    else if (std::regex_search(comp, std::regex(R"(D-(A|M))")))
    {
        bin += Code::comp_map["nx"];
        bin += Code::comp_map["f"];
        bin += Code::comp_map["no"];
    }
    // is M or A subtracted?
    else if (std::regex_search(comp, std::regex(R"((A|M)-D)")))
    {
        bin += Code::comp_map["ny"];
        bin += Code::comp_map["f"];
        bin += Code::comp_map["no"];
    }
    else
    {
        if (comp.find('D') != std::string::npos)
        {
            bin += Code::comp_map["zy"];
            bin += Code::comp_map["ny"];
        }
        else if (comp.find('A') != std::string::npos || comp.find('M') != std::string::npos)
        {
            bin += Code::comp_map["zx"];
            bin += Code::comp_map["nx"];
        }
        else
        {
            // Is comp a constant value?
            try {
                switch (stoi(comp))
                {
                    case -1:
                        bin += Code::comp_map["zx"];
                        bin += Code::comp_map["zy"];
                        bin += Code::comp_map["nx"];
                        bin += Code::comp_map["f"];
                        break;
                    case 0:
                        bin = Code::comp_map["zx"];
                        bin += Code::comp_map["zy"];
                        bin += Code::comp_map["f"];
                        break;
                    case 1:
                        bin += Code::comp_map["zx"];
                        bin += Code::comp_map["zy"];
                        bin += Code::comp_map["nx"];
                        bin += Code::comp_map["ny"];
                        bin += Code::comp_map["f"];
                        bin += Code::comp_map["no"];
                        break;
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

        if (comp.find("-1") != std::string::npos)
        {
            bin += Code::comp_map["f"];
        }
        else if (comp.find("+1") != std::string::npos)
        {
            bin += Code::comp_map["f"];
            bin += Code::comp_map["no"];
            bin += Code::comp_map[comp[0] == 'D' ? "nx" : "ny"];
        }
        else if (comp[0] == '!')
        {
            bin += Code::comp_map["no"];
        }
        else if (comp[0] == '-')
        {
            bin += Code::comp_map["f"];
            bin += Code::comp_map["no"];
        }
    }
    return std::bitset<7>(bin).to_string();
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
    if(jump == "JMP")
    {
        bin += Code::jump_map['G'];
        bin += Code::jump_map['L'];
        bin += Code::jump_map['E'];
    }
    return std::bitset<3>(bin).to_string();
}
