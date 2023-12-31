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
    char file_name[64];
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
int initialize_writer(CodeWriter *writer, const char *file_path);
void set_file_name(CodeWriter *writer, char *file_name);
void write_arithmetic(CodeWriter *writer, char *command);
void write_push_pop(CodeWriter *writer, CommandType command, char *segment, int index);
void write_label(CodeWriter *writer, char *label);
void write_goto(CodeWriter *writer, char *label);
void write_if(CodeWriter *writer, char *label);
void write_function(CodeWriter *writer, char *function_name, int n_vars);
void write_call(CodeWriter *writer, char *function_name, int n_args);
void write_return(CodeWriter *writer);
void close(CodeWriter *writer);

#endif