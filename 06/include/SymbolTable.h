#include <string>
#include <map>

/**
 * @brief handles symbols
 * 
 */
class SymbolTable
{
public:
    SymbolTable();
    // Add <symbol, address> to the table.
    void add_entry(std::string symbol, int address);
    // Checks if symbol exists in the table.
    bool contains(std::string symbol);
    // Returns the address associated with symbol
    int get_address(std::string symbol);
    std::map<std::string, int> table;

private:
};