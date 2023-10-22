#include "header/CodeWriter.h"

int initialize_writer(CodeWriter *writer, char *file_path)
{
    printf("[CodeWriter(%s): %d] INFO: start\n", __func__, __LINE__);
    int cc = strlen(file_path) - 3;
    char file_name[cc];
    strncpy(file_name, file_path, cc);
    file_name[cc] = '\0';

    char asm_file[cc];
    strcpy(asm_file, file_name);

    char *ext = strrchr(file_path, '.');
    char *slash = strrchr(file_path, '/');
    if (slash != NULL)
    {
        int name_count = ext - ++slash;
        printf("[CodeWriter(%s): %d] INFO: name_count=%d\n", __func__, __LINE__, name_count);
        strncpy(file_name, slash, name_count);
        file_name[name_count] = '\0';
    }
    printf("[CodeWriter(%s): %d] INFO: file_name=%s\n", __func__, __LINE__, file_name);

    // set file name for static
    writer->file_name = file_name;

    strcat(asm_file, ".asm");
    printf("[CodeWriter(%s): %d] INFO: asm_file=%s\n", __func__, __LINE__, asm_file);

    writer->output = NULL;
    writer->output = fopen(asm_file, "w");

    writer->cond_i = 0;

    if (writer->output == NULL)
    {
        printf("[CodeWriter(%s): %d] ERROR: Can't open the asm file.\n", __func__, __LINE__);
        return 1;
    }

    printf("[CodeWriter(%s): %d] INFO: Initial assembly code is written.\n", __func__, __LINE__);
    fprintf(writer->output, "// Stack is initialized.\n");
    fprintf(writer->output, "@%d\n", STACK);
    fprintf(writer->output, "D=A\n");
    fprintf(writer->output, "@SP\n");
    fprintf(writer->output, "M=D\n");

    printf("[CodeWriter(%s): %d] INFO: end\n", __func__, __LINE__);
    return 0;
}

void write_arithmetic(CodeWriter *writer, char *command)
{
    printf("[CodeWriter(%s): %d] INFO: start\n", __func__, __LINE__);
    fprintf(writer->output, "@SP\n");
    fprintf(writer->output, "AM=M-1\n");
    fprintf(writer->output, "D=M\n");
    printf("[CodeWriter(%s): %d] INFO: command=%s\n", __func__, __LINE__, command);

    if (strcmp(command, "neg") == 0)
    {
        fprintf(writer->output, "M=-M\n");

    }
    else if (strcmp(command, "not") == 0)
    {
        fprintf(writer->output, "M=!M\n");

    }
    else
    {
        fprintf(writer->output, "@SP\n");
        fprintf(writer->output, "AM=M-1\n");

        if (strcmp(command, "add") == 0 || strcmp(command, "sub") == 0)
        {
            char operator = strcmp(command, "add") == 0 ? '+' : '-';
            fprintf(writer->output, "M=M%cD\n", operator);
        }

        else
        {
            if (
                strcmp(command, "eq") == 0
                || strcmp(command, "gt") == 0
                || strcmp(command, "lt") == 0
            )
            {
                fprintf(writer->output, "D=D-M\n");
                fprintf(writer->output, "M=0\n");

                if (strcmp(command, "eq") == 0)
                {
                    fprintf(writer->output, "@END_EQ.%d\n", writer->cond_i);
                    fprintf(writer->output, "D;JNE\n");
                    fprintf(writer->output, "@SP\n");
                    fprintf(writer->output, "A=M\n");
                    fprintf(writer->output, "M=-1\n");
                    fprintf(writer->output, "(END_EQ.%d)\n", writer->cond_i);

                }
                else if (strcmp(command, "gt") == 0)
                {
                    fprintf(writer->output, "@END_GT.%d\n", writer->cond_i);
                    fprintf(writer->output, "D;JGT\n");
                    fprintf(writer->output, "@SP\n");
                    fprintf(writer->output, "A=M\n");
                    fprintf(writer->output, "M=-1\n");
                    fprintf(writer->output, "(END_GT.%d)\n", writer->cond_i);

                }
                else if (strcmp(command, "lt") == 0)
                {
                    fprintf(writer->output, "@END_LT.%d\n", writer->cond_i);
                    fprintf(writer->output, "D;JLT\n");
                    fprintf(writer->output, "@SP\n");
                    fprintf(writer->output, "A=M\n");
                    fprintf(writer->output, "M=-1\n");
                    fprintf(writer->output, "(END_LT.%d)\n", writer->cond_i);

                }

            }
            else if (strcmp(command, "and") == 0)
            {
                fprintf(writer->output, "D=D&M\n");
                fprintf(writer->output, "D=D+1\n");
                fprintf(writer->output, "M=0\n");
                fprintf(writer->output, "@END_AND.%d\n", writer->cond_i);
                fprintf(writer->output, "D;JNE\n");
                fprintf(writer->output, "@SP\n");
                fprintf(writer->output, "A=M\n");
                fprintf(writer->output, "M=-1\n");
                fprintf(writer->output, "(END_AND.%d)\n", writer->cond_i);

            }

            else if (strcmp(command, "or") == 0)
            {
                fprintf(writer->output, "D=D|M\n");
                fprintf(writer->output, "D=D+1\n");
                fprintf(writer->output, "M=0\n");
                fprintf(writer->output, "@END_OR.%d\n", writer->cond_i);
                fprintf(writer->output, "D;JNE\n");
                fprintf(writer->output, "@SP\n");
                fprintf(writer->output, "A=M\n");
                fprintf(writer->output, "M=-1\n");
                fprintf(writer->output, "(END_OR.%d)\n", writer->cond_i);

            }

        }
        writer->cond_i++;

    }
    fprintf(writer->output, "@SP\n");
    fprintf(writer->output, "M=M+1\n");
    printf("[CodeWriter(%s): %d] INFO: end\n", __func__, __LINE__);

}

void write_push_pop(
    CodeWriter *writer,
    CommandType command,
    char *segment,
    int index
)
{
    printf("[CodeWriter(%s): %d] INFO: start\n", __func__, __LINE__);
    Symbol symbol;

    printf("[CodeWriter(%s): %d] INFO: segment=%s\n", __func__, __LINE__, segment);
    if (strcmp(segment, "constant") == 0)
    {
        symbol = CONSTANT;
    }

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
        symbol = index == 0 ? THIS : THAT;
    }

    else if (strcmp(segment, "temp") == 0)
    {
        symbol = TEMP;
    }

    printf("[CodeWriter(%s): %d] symbol=%d\n", __func__, __LINE__, symbol);

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

            fprintf(writer->output, "D=M\n");

        // constant
        default:
            fprintf(writer->output, "@%d\n", index);
            fprintf(writer->output, "D=A\n");

        }

        fprintf(writer->output, "@SP\n");

        fprintf(writer->output, "A=M\n");
        fprintf(writer->output, "M=D\n");

        fprintf(writer->output, "@SP\n");
        fprintf(writer->output, "M=M+1\n");
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

            break;

        case STATIC:
        case TEMP:
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
            printf("[CodeWriter(%s): %d] WARN: cannot use pop to constant.\n", __func__, __LINE__);
            return;
        }

        fprintf(writer->output, "@SP\n");
        fprintf(writer->output, "M=M-1\n");

        fprintf(writer->output, "A=M\n");
        fprintf(writer->output, "M=D\n");

        printf("[CodeWriter(%s): %d] INFO: end\n", __func__, __LINE__);
    }
}

void close(CodeWriter *writer)
{
    printf("[CodeWriter(%s): %d] INFO: start\n", __func__, __LINE__);
    // fprintf(writer->output, "// Add infinite loop at the end.\n");
    // fprintf(writer->output, "(INFINITE_LOOP)\n");
    // fprintf(writer->output, "@INFINITE_LOOP\n");
    // fprintf(writer->output, "0;JMP\n");
    fclose(writer->output);
    printf("[CodeWriter(%s): %d] INFO: end\n", __func__, __LINE__);
}