#ifndef CODE_WRITER_H
#define CODE_WRITER_H

// ---------------------------------------------------------------------
// Included header
// ---------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "CommandType.h"
#include "RAM.h"

// ---------------------------------------------------------------------
// Macro definition
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// Type definition
// ---------------------------------------------------------------------
typedef struct
{
    FILE *output;
    char *file_name;
    int cond_i;
} CodeWriter;

// ---------------------------------------------------------------------
// Prototype declaration
// ---------------------------------------------------------------------
int initialize_writer(CodeWriter *writer, char *file_path);
void write_arithmetic(CodeWriter *writer, char *command);
void write_push_pop(CodeWriter *writer, CommandType command, char *segment, int index);
void close(CodeWriter *writer);

#endif