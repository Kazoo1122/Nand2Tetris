#include <string>
#include <fstream>
#include <vector>

enum class Instruction
{
    A_INSTRUCTION
    , C_INSTRUCTION
    , L_INSTRUCTION
};

/**
 * @brief Reads and parses an instruction
 * 
 */
class Parser
{
public:
    Parser(std::string file_path);
    // Checks if there is more work to do.
    bool has_more_lines();
    // Gets the next instruction and makes it the current instruction.
    void advance();
    // Returns the current instruction type, as a constant
    Instruction instruction_type();
    // Returns the instruction's symbol
    std::string symbol();
    // Returns the instruction's dest field
    std::string dest();
    // Returns the instruction's comp field
    std::string comp();
    // Returns the instruction's jump field
    std::string jump();

private:
    std::vector<std::string> lines;
    int current_line_no;
    std::string current_instruction;
};