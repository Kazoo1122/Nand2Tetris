#ifndef RAM_H
#define RAM_H

// ---------------------------------------------------------------------
// Included header
// ---------------------------------------------------------------------
#include <stdlib.h>

// ---------------------------------------------------------------------
// Macro definition
// ---------------------------------------------------------------------
#define MAX_ADDRESS (2047)

// ---------------------------------------------------------------------
// Type definition
// ---------------------------------------------------------------------
typedef enum
{
    CONSTANT = -1
    , SP
    , LCL
    , ARG
    , THIS
    , THAT
    , TEMP
    , R13
    , R14
    , R15
    , STATIC
    , STACK = 256
} Symbol;

static int *ram;

// ---------------------------------------------------------------------
// Prototype declaration
// ---------------------------------------------------------------------
void initialize_ram();
int get_ram(int *dest, unsigned int address);
int set_ram(int value, unsigned int address);
void free_ram();

#endif