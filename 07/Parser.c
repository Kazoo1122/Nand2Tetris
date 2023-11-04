#include "header/Parser.h"

int initialize_parser(Parser *parser, char *file_path)
{
    printf("[Parser(%s): %d] INFO: start\n", __func__, __LINE__);
    printf("[Parser(%s): %d] INFO: file_path=%s\n", __func__, __LINE__, file_path);
    FILE *fp = NULL;
    fp = fopen(file_path, "r");

    if (fp == NULL)
    {
        printf("[Parser(%s): %d] ERROR: Can't open the vm file.", __func__, __LINE__);
        return 1;
    }

    unsigned int line_total = parse_file_contents(fp, NULL, NULL);
    rewind(fp);
    unsigned int column_counts[line_total];
    parse_file_contents(fp, column_counts, NULL);
    rewind(fp);

    printf("[Parser(%s): %d] INFO: line=%d\n", __func__, __LINE__, line_total);

    parser->lines = (char **) malloc(line_total * sizeof(char *));

    for (int i = 0; i < line_total; i++)
    {
        parser->lines[i] = (char *) malloc((column_counts[i] + 1) * sizeof(char));
        parse_file_contents(fp, NULL, parser->lines[i]);
        printf("[Parser(%s): %d] INFO: column=%d\n", __func__, __LINE__, column_counts[i]);
    }

    fclose(fp);

    for (int i = 0; i < line_total; i++)
    {
        printf("[Parser(%s): %d] INFO: ", __func__, __LINE__);
        for (int j = 0; j < column_counts[i]; j++)
        {
            printf("%c", parser->lines[i][j]);
        }
        printf("\n");
    }

    // Parser initialized.
    parser->current_line_no = 0;
    parser->line_total = line_total;
    parser->current_instruction = parser->lines[0];

    printf("[Parser(%s): %d] INFO: end\n", __func__, __LINE__);
    return 0;
}

static unsigned int parse_file_contents(
    FILE *fp,
    unsigned int *column_counts,
    char *column_contents
)
{
    printf("[Parser(%s): %d] INFO: start\n", __func__, __LINE__);
    char c = '\0';
    unsigned int line = 0;
    unsigned int col = 1;
    unsigned int slash_count = 0;
    bool comment_flg = false;

    while ((c = fgetc(fp)) != EOF)
    {
        if (comment_flg == false)
        {
            if (c == '/')
            {
                if (slash_count == 0)
                {
                    slash_count++;
                }
                else
                {
                    comment_flg = true;
                    slash_count = 0;
                }
                col++;
                continue;
            }
            else if (c == '\n')
            {
                if (col > 1)
                {
                    if (column_counts != NULL)
                    {
                        column_counts[line] = col;
                    }
                    else if (column_contents != NULL)
                    {
                        column_contents[col - 1] = '\0';
                        printf("[Parser(%s): %d] INFO: end line=%d\n", __func__, __LINE__, line);
                        return line;
                    }
                    col = 1;
                    line++;
                }
            }
            else if(c != '\r')
            {
                if (column_contents != NULL)
                {
                    column_contents[col - 1] = c;
                }
                col++;
            }
        }
        else
        {
            if (c == '\n')
            {
                comment_flg = false;
                col = 1;
            }
            continue;
        }
    }
    if (col > 1)
    {
        if (column_counts != NULL)
        {
            column_counts[line] = col;
        }
        line++;
    }

    printf("[Parser(%s): %d] INFO: end line=%d\n", __func__, __LINE__, line);
    return line;
}

bool has_more_lines(Parser *parser)
{
    printf("[Parser(%s): %d] INFO: start\n", __func__, __LINE__);
    bool result = parser->current_line_no < (parser->line_total - 1);
    printf("[Parser(%s): %d] INFO: end result=%d\n", __func__, __LINE__, result);
    return result;
}

void advance(Parser *parser)
{
    printf("[Parser(%s): %d] INFO: start\n", __func__, __LINE__);
    parser->current_instruction = parser->lines[++parser->current_line_no];
    printf("[Parser(%s): %d] INFO: end current_instruction=%s\n", __func__, __LINE__, parser->current_instruction);
}

CommandType command_type(Parser *parser)
{
    printf("[Parser(%s): %d] INFO: start\n", __func__, __LINE__);
    printf("[Parser(%s): %d] INFO: current_instruction=%s\n", __func__, __LINE__, parser->current_instruction);
    char *tail = strchr(parser->current_instruction, ' ');
    printf("[Parser(%s): %d] INFO: tail=%s\n", __func__, __LINE__, tail);
    int char_count = tail != NULL ? tail - parser->current_instruction : strlen(parser->current_instruction);
    printf("[Parser(%s): %d] INFO: char_count=%d\n", __func__, __LINE__, char_count);
    parser->command = (char *) malloc((char_count + 1) * sizeof(char *));
    strncpy(parser->command, parser->current_instruction, char_count);
    parser->command[char_count] = '\0';
    printf("[Parser(%s): %d] INFO: command=%s\n", __func__, __LINE__, parser->command);

    CommandType type;
    if (
        strcmp(parser->command, "add") == 0
        || strcmp(parser->command, "sub") == 0
        || strcmp(parser->command, "neg") == 0
        || strcmp(parser->command, "eq") == 0
        || strcmp(parser->command, "gt") == 0
        || strcmp(parser->command, "lt") == 0
        || strcmp(parser->command, "and") == 0
        || strcmp(parser->command, "or") == 0
        || strcmp(parser->command, "not") == 0
    )
    {
        type = C_ARITHMETIC;
    }
    else if (strcmp(parser->command, "push") == 0)
    {
        type = C_PUSH;
    }
    else if (strcmp(parser->command, "pop") == 0)
    {
        type = C_POP;
    }
    else if (strcmp(parser->command, "label") == 0)
    {
        type = C_LABEL;
    }
    else if (strcmp(parser->command, "goto") == 0)
    {
        type = C_GOTO;
    }
    else if (strcmp(parser->command, "if-goto") == 0)
    {
        type = C_IF;
    }
    else if (strcmp(parser->command, "Function") == 0)
    {
        type = C_FUNCTION;
    }
    else if (strcmp(parser->command, "return") == 0)
    {
        type = C_RETURN;
    }
    else if (strcmp(parser->command, "Call") == 0)
    {
        type = C_CALL;
    }
    else
    {
        printf("[Parser(%s): %d] ERROR: The passed command is invalid.\n", __func__, __LINE__);
        exit(1);
    }
    printf("[Parser(%s): %d] INFO: end\n", __func__, __LINE__);
    return type;
}

void *arg1(char **dest, Parser *parser, CommandType type)
{
    printf("[Parser(%s): %d] INFO: start\n", __func__, __LINE__);
    if (type == C_ARITHMETIC)
    {
        *dest = parser->command;
        printf("[Parser(%s): %d] INFO: end\n", __func__, __LINE__);
        return NULL;
    }
    else if (type == C_RETURN)
    {
        *dest = NULL;
        printf("[Parser(%s): %d] INFO: end\n", __func__, __LINE__);
        return NULL;
    }
    else
    {
        printf("[Parser(%s): %d] INFO: current pointer=%p\n", __func__, __LINE__, parser->current_instruction);
        char *first_blank = strchr(parser->current_instruction, ' ');
        printf("[Parser(%s): %d] INFO: first pointer=%p\n", __func__, __LINE__, first_blank);
        printf("[Parser(%s): %d] INFO: first string=%s\n", __func__, __LINE__, first_blank);
        char *second_blank = strchr(first_blank + 1, ' ');
        printf("[Parser(%s): %d] INFO: second pointer=%p\n", __func__, __LINE__, second_blank);
        printf("[Parser(%s): %d] INFO: second string=%s\n", __func__, __LINE__, second_blank);
        int length = second_blank - first_blank;
        printf("[Parser(%s): %d] INFO: length=%d\n", __func__, __LINE__, length);
        char *arg = (char *) malloc(length * sizeof(char *));
        strncpy(arg, first_blank + 1, length - 1);
        arg[length] = '\0';
        printf("[Parser(%s): %d] INFO: arg=%s\n", __func__, __LINE__, arg);
        *dest = arg;
        printf("[Parser(%s): %d] INFO: end\n", __func__, __LINE__);
        return arg;
    }
}

void arg2(int *dest, Parser *parser, CommandType type)
{
    printf("[Parser(%s): %d] INFO: start\n", __func__, __LINE__);
    if (
        type == C_PUSH
        || type == C_POP
        || type == C_FUNCTION
        || type == C_CALL
    )
    {
        char *arg = strchr(parser->current_instruction, ' ');
        arg = strchr(++arg, ' ');
        printf("[Parser(%s): %d] INFO: arg=%s\n", __func__, __LINE__, arg);

        int num = atoi(++arg);
        printf("[Parser(%s): %d] INFO: num=%d\n", __func__, __LINE__, num);
        *dest = num;
    }
    printf("[Parser(%s): %d] INFO: end\n", __func__, __LINE__);
}

void parser_finalize(Parser *parser)
{
    printf("[Parser(%s): %d] INFO: start\n", __func__, __LINE__);
    for (int i = 0; i < parser->line_total; i++)
    {
        free(parser->lines[i]);
    }
    free(parser->lines);
    printf("[Parser(%s): %d] INFO: end\n", __func__, __LINE__);
}