#ifndef CODE_WRITER_H
#define CODE_WRITER_H

// ---------------------------------------------------------------------
// Included header
// ---------------------------------------------------------------------
#include "CommandType.h"

// ---------------------------------------------------------------------
// Macro definition
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// Type definition
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// Prototype declaration
// ---------------------------------------------------------------------
void writer_initialize(char *file_path);
void write_arithmetic(CommandType command);
void write_push_pop(CommandType command, char *segment, int index);
void close();

#endif