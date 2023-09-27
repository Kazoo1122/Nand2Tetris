#include "header/CodeWriter.h"

int initialize_writer(CodeWriter *writer, char *file_path)
{
    printf("[writer_initialize] start\n");
    writer->output = NULL;
    writer->output = fopen(file_path, "a");

    // set file name for static
    int cc = strlen(file_path);
    char file_name[cc - 3];
    strncpy(file_name, file_path, (cc - 3));
    writer->file_name = file_name;

    if (writer->output == NULL)
    {
        printf("[writer_initialize] Can't open the asm file.\n");
        return 1;
    }

    fprintf(writer->output, "// Stack is initialized.\n");
    fprintf(writer->output, "@%d\n", STACK);
    fprintf(writer->output, "D=A\n");
    fprintf(writer->output, "@SP\n");
    fprintf(writer->output, "M=D\n");

    return 0;
}

void write_arithmetic(CodeWriter *writer, char *command)
{
    int *sp = NULL;
    if (strcmp(command, "add") == 0 || strcmp(command, "sub") == 0)
    {
        fprintf(writer->output, "@SP\n");
        fprintf(writer->output, "A=M\n");
        fprintf(writer->output, "D=M\n");
        fprintf(writer->output, "@SP\n");
        fprintf(writer->output, "M=M-1\n");
        fprintf(writer->output, "A=M\n");

        char operator = strcmp(command, "add") == 0 ? '+' : '-';
        fprintf(writer->output, "M=M%cD\n", operator);

        // get_ram(sp, SP);
        // printf("[write_arithmetic] sp=%d", *sp);
        // set_ram(--(*sp), SP);

    }
    else if (strcmp(command, "neg") == 0)
    {
        fprintf(writer->output, "@SP\n");
        fprintf(writer->output, "A=M\n");
        fprintf(writer->output, "M=-M\n");

    }
    else if (strcmp(command, "eq") == 0)
    {

    }
    else if (strcmp(command, "gt") == 0)
    {

    }
    else if (strcmp(command, "lt") == 0)
    {

    }
    else if (strcmp(command, "and") == 0)
    {

    }
    else if (strcmp(command, "or") == 0)
    {

    }
    else if (strcmp(command, "not") == 0)
    {

    }


}

void write_push_pop(
    CodeWriter *writer,
    CommandType command,
    char *segment,
    int index
)
{
    Symbol symbol;

    if (strcmp(segment, "local") == 0)
    {
        symbol = LCL;
    }

    else if (strcmp(segment, "argument") == 0)
    {
        symbol = ARG;
    }

    else if (strcmp(segment, "static") == 0)
    {
        symbol = STATIC;
    }

    else if (strcmp(segment, "constant") == 0)
    {
        symbol = CONSTANT;
    }
    else if (strcmp(segment, "this") == 0)
    {
        symbol = THIS;
    }

    else if (strcmp(segment, "that") == 0)
    {
        symbol = THAT;
    }

    else if (strcmp(segment, "pointer") == 0)
    {
        if (index == 0)
        {
            symbol = THIS;
        }

        else
        {
            symbol = THAT;
        }
    }

    else if (strcmp(segment, "temp") == 0)
    {
        symbol = TEMP;
    }

    printf("[write_push_pop] symbol=%d\n", symbol);

    int *sp = NULL;
    int *base = NULL;

    if (command == C_PUSH)
    {
        int pushed_value = 0;
        unsigned int seg_addr = 0;

        switch (symbol)
        {
        case LCL:
        case ARG:
        case THIS:
        case THAT:
            // get_ram(base, symbol);
            // printf("[write_push_pop] base=%d", *base);
            // unsigned int seg_addr = 0;
            // seg_addr = *base + index;
            // int *seg_value = NULL;
            // get_ram(seg_value, seg_addr);
            // pushed_value = *seg_value;

            switch (symbol)
            {
            case LCL:
                fprintf(writer->output, "@LCL\n");
                break;

            case ARG:
                fprintf(writer->output, "@ARG\n");
                break;

            case THIS:
                fprintf(writer->output, "@THIS\n");
                break;

            case THAT:
                fprintf(writer->output, "@THAT\n");
                break;

            default:
                break;

            }
            fprintf(writer->output, "D=M\n");
            break;

        case STATIC:
        case TEMP:
            seg_addr = symbol + index;
            // get_ram(seg_value, seg_addr);
            // pushed_value = *seg_value;

            switch (symbol)
            {
                case STATIC:
                    fprintf(writer->output, "@%s.%d\n", writer->file_name, index);
                    break;

                case TEMP:
                    fprintf(writer->output, "@%d\n", seg_addr);
                    break;

                default:
                    break;
            }

            fprintf(writer->output, "D=M\n");

        // constant
        default:
            fprintf(writer->output, "@%d\n", index);
            fprintf(writer->output, "D=A\n");
            // pushed_value = index;
        }

        fprintf(writer->output, "@SP\n");
        // get_ram(sp, SP);
        // printf("[write_push_pop] sp=%d\n", *sp);

        fprintf(writer->output, "A=M\n");
        fprintf(writer->output, "M=D\n");
        // set_ram(pushed_value, *sp);

        fprintf(writer->output, "@SP\n");
        fprintf(writer->output, "M=M+1\n");
        // set_ram(++(*sp), SP);
    }

    else if (command == C_POP)
    {
        unsigned int seg_addr = 0;
        switch (symbol)
        {
        case LCL:
        case ARG:
        case THIS:
        case THAT:
            switch (symbol)
            {
            case LCL:
                fprintf(writer->output, "@LCL\n");
                break;

            case ARG:
                fprintf(writer->output, "@ARG\n");
                break;

            case THIS:
                fprintf(writer->output, "@THIS\n");
                break;

            case THAT:
                fprintf(writer->output, "@THAT\n");
                break;

            default:
                break;

            }

            fprintf(writer->output, "D=M\n");
            fprintf(writer->output, "D=D+%d\n", index);

            // get_ram(base, symbol);
            // printf("[write_push_pop] base=%d", *base);
            // seg_addr = *base + index;
            break;

        case STATIC:
        case TEMP:
            // get_ram(base, symbol);
            // printf("[write_push_pop] base=%d", *base);

            switch (symbol)
            {
                case STATIC:
                    fprintf(writer->output, "@%s.%d\n", writer->file_name, index);
                    break;

                case TEMP:
                    seg_addr = symbol + index;
                    fprintf(writer->output, "@%d\n", seg_addr);
                    break;

                default:
                    break;
            }

            fprintf(writer->output, "D=A\n");
            break;

        // constant
        default:
            printf("[write_push_pop] cannot use pop to constant.\n");
            return;
        }

        fprintf(writer->output, "@SP\n");
        fprintf(writer->output, "M=M-1\n");
        // get_ram(sp, SP);
        // printf("[write_push_pop] sp=%d", *sp);
        // set_ram(--(*sp), SP);

        fprintf(writer->output, "A=M\n");
        fprintf(writer->output, "M=D\n");
        // int *popped_value = NULL;
        // get_ram(popped_value, *sp);
        // printf("[write_push_pop] popped_value=%d\n", *popped_value);
        // set_ram(*popped_value, seg_addr);
    }
}

void close(CodeWriter *writer)
{
    free_ram();
}