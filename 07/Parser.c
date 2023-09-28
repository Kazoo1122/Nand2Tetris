#include "header/Parser.h"

void initialize_parser(Parser *parser, char *file_path)
{
    printf("[parser_initialize] start\n");
    FILE *fp = NULL;
    fp = fopen(file_path, "r");

    if (fp == NULL)
    {
        printf("[parser_initialize] Can't open the vm file.");
        return;
    }

    unsigned int line_total = parse_file_contents(fp, NULL, NULL);
    rewind(fp);
    unsigned int column_counts[line_total];
    parse_file_contents(fp, column_counts, NULL);
    rewind(fp);

    printf("[parser_initialize] line=%d\n", line_total);

    parser->lines = (char **) malloc(line_total * sizeof(char *));

    for (int i = 0; i < line_total; i++)
    {
        parser->lines[i] = (char *) malloc((column_counts[i] + 1) * sizeof(char));
        parse_file_contents(fp, NULL, parser->lines[i]);
        printf("[parser_initialize] column=%d\n", column_counts[i]);
    }

    fclose(fp);

    for (int i = 0; i < line_total; i++)
    {
        printf("[parser_initialize] ");
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

    return;
}

static unsigned int parse_file_contents(
    FILE *fp,
    unsigned int *column_counts,
    char *column_contents
)
{
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
    return line;
}

bool has_more_lines(Parser *parser)
{
    printf("[has_more_line] start\n");
    bool result = parser->current_line_no < (parser->line_total - 1);
    printf("[has_more_line] result=%d\n", result);
    return result;
}

void advance(Parser *parser)
{
    printf("[advance] start\n");
    parser->current_instruction = parser->lines[++parser->line_total];
}

CommandType command_type(Parser *parser)
{
    printf("[command_type] start\n");
    printf("[command_type] current_instruction=%s\n", parser->current_instruction);
    char *tail = strchr(parser->current_instruction, ' ');
    printf("[command_type] tail=%s\n", tail);
    int char_count = tail - parser->current_instruction;
    printf("[command_type] char_count=%d\n", char_count);
    parser->command = (char *) malloc((char_count + 1) * sizeof(char *));
    // char command[char_count + 1];
    strncpy(parser->command, parser->current_instruction, char_count);
    parser->command[char_count] = '\0';
    printf("[command_type] command=%s\n", parser->command);

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
        return C_ARITHMETIC;
    }
    else if (strcmp(parser->command, "push") == 0)
    {
        return C_PUSH;
    }
    else if (strcmp(parser->command, "pop") == 0)
    {
        return C_POP;
    }
    else if (strcmp(parser->command, "label") == 0)
    {
        return C_LABEL;
    }
    else if (strcmp(parser->command, "goto") == 0)
    {
        return C_GOTO;
    }
    else if (strcmp(parser->command, "if-goto") == 0)
    {
        return C_IF;
    }
    else if (strcmp(parser->command, "Function") == 0)
    {
        return C_FUNCTION;
    }
    else if (strcmp(parser->command, "return") == 0)
    {
        return C_RETURN;
    }
    else if (strcmp(parser->command, "Call") == 0)
    {
        return C_CALL;
    }
    else
    {
        printf("[command_type] The passed command is invalid.\n");
        exit(1);
    }
}

void *arg1(char **dest, Parser *parser, CommandType type)
{
    if (type == C_ARITHMETIC)
    {
        *dest = parser->command;
        return NULL;
    }
    else if (type == C_RETURN)
    {
        *dest = NULL;
        return NULL;
    }
    else
    {
        printf("current=%p\n", parser->current_instruction);
        char *first_blank = strchr(parser->current_instruction, ' ');
        printf("first=%p\n", first_blank);
        printf("first=%s\n", first_blank);
        char *second_blank = strchr(first_blank + 1, ' ');
        printf("second=%p\n", second_blank);
        printf("second=%s\n", second_blank);
        int length = second_blank - first_blank;
        printf("length=%d\n", length);
        char *arg = (char *) malloc(length * sizeof(char *));
        strncpy(arg, first_blank + 1, length - 1);
        arg[length] = '\0';
        printf("arg=%s\n", arg);
        *dest = arg;
        return arg;
    }
}

void arg2(int **dest, Parser *parser, CommandType type)
{
    if (
        type == C_PUSH
        || type == C_POP
        || type == C_FUNCTION
        || type == C_CALL
    )
    {
        char *arg = strrchr(parser->current_instruction, ' ');
        int num = *++arg - '0';
        int *num_p = &num;
        *dest = num_p;
    }
    else
    {
        *dest = NULL;
    }
}

void parser_finalize(Parser *parser)
{
    for (int i = 0; i < parser->line_total; i++)
    {
        free(parser->lines[i]);
    }
    free(parser->lines);
}