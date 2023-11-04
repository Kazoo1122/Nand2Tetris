#ifndef CODE_WRITER_H
#define CODE_WRITER_H

// ---------------------------------------------------------------------
// Included header
// ---------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CommandType.h"

// ---------------------------------------------------------------------
// Macro definition
// ---------------------------------------------------------------------
#define MAX_ADDRESS (2047)

// ---------------------------------------------------------------------
// Type definition
// ---------------------------------------------------------------------
typedef struct
{
    FILE *output;
    char *file_name;
    int cond_i;
} CodeWriter;

typedef enum
{
    NONE = -1
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

// ---------------------------------------------------------------------
// Prototype declaration
// ---------------------------------------------------------------------
int initialize_writer(CodeWriter *writer, char *file_path);
void write_arithmetic(CodeWriter *writer, char *command);
void write_push_pop(CodeWriter *writer, CommandType command, char *segment, int index);
void close(CodeWriter *writer);

#endif