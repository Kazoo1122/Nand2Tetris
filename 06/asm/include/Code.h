#include <string>

/**
 * @brief Generates binary code
 * 
 */
class Code
{
public:
    // Returns the binary representation of the parsed dest field
    std::string dest();
    // Returns the binary representation of the parsed comp field
    std::string comp();
    // Returns the binary representation of the parsed jump field
    std::string jump();
};