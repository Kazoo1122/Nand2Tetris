#include "header/RAM.h"

void initialize_ram()
{
    ram = (int *) malloc(MAX_ADDRESS * sizeof(int));
    ram[SP] = STACK;
}

int get_from_ram(int *dest, unsigned int address)
{
    if (address <= MAX_ADDRESS)
    {
        int tmp = ram[address];
        dest = &tmp;
        return 0;
    }
    return 1;
}

int set_to_ram(int value, unsigned int address)
{
    if (address <= MAX_ADDRESS)
    {
        ram[address] = value;
        return 0;
    }
    return 1;
}

void free_ram()
{
    free(ram);
}
