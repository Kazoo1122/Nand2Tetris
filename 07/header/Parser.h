#ifndef PARSER_H
#define PARSER_H

// ---------------------------------------------------------------------
// Included header
// ---------------------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "CommandType.h"

// ---------------------------------------------------------------------
// Macro definition
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// Type definition
// ---------------------------------------------------------------------
typedef struct
{
    char **lines;
    unsigned int current_line_no;
    unsigned int line_total;
    char *current_instruction;
    char *command;
} Parser;

// ---------------------------------------------------------------------
// Prototype declaration
// ---------------------------------------------------------------------
void initialize_parser(Parser *parser, char *file_path);

// Checks if there is more work to do.
bool has_more_lines(Parser *parser);

// Gets the next instruction and makes it the current instruction.
void advance(Parser *parser);

CommandType command_type(Parser *parser);

void *arg1(char **dest, Parser *parser, CommandType type);

void arg2(int **dest, Parser *parser, CommandType type);

static unsigned int parse_file_contents(
    FILE *fp,
    unsigned int *columns,
    char *column_contents
);

void parser_finalize(Parser *parser);

#endif

