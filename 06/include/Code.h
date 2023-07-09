#include <string>
#include <map>

/**
 * @brief Generates binary code
 * 
 */
class Code
{
public:
    Code();
    // Returns the binary representation of the parsed dest field
    std::string dest(std::string);
    // Returns the binary representation of the parsed comp field
    std::string comp(std::string);
    // Returns the binary representation of the parsed jump field
    std::string jump(std::string);
private:
    std::map<char, int> dest_map;
    std::map<std::string, int> comp_map;
    std::map<char, int> jump_map;
};