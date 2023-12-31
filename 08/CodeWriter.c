#include "header/CodeWriter.h"

int initialize_writer(CodeWriter *writer, const char *file_path)
{
    printf("[CodeWriter(%s): %d] INFO: start\n", __func__, __LINE__);
    int path_count = strlen(file_path);
    char asm_file[path_count + 4];
    strcpy(asm_file, file_path);

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

void set_file_name(CodeWriter *writer, char *file_name)
{
    printf("[CodeWriter(%s): %d] INFO: start\n", __func__, __LINE__);

    // set file name for static
    strcpy(writer->file_name, file_name);

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

        else if (strcmp(command, "and") == 0)
        {
            fprintf(writer->output, "M=D&M\n");
        }

        else if (strcmp(command, "or") == 0)
        {
            fprintf(writer->output, "M=D|M\n");
        }

        else if (
            strcmp(command, "eq") == 0
            || strcmp(command, "gt") == 0
            || strcmp(command, "lt") == 0
        )
        {
            fprintf(writer->output, "D=M-D\n");
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
                fprintf(writer->output, "D;JLE\n");
                fprintf(writer->output, "@SP\n");
                fprintf(writer->output, "A=M\n");
                fprintf(writer->output, "M=-1\n");
                fprintf(writer->output, "(END_GT.%d)\n", writer->cond_i);

            }
            else if (strcmp(command, "lt") == 0)
            {
                fprintf(writer->output, "@END_LT.%d\n", writer->cond_i);
                fprintf(writer->output, "D;JGE\n");
                fprintf(writer->output, "@SP\n");
                fprintf(writer->output, "A=M\n");
                fprintf(writer->output, "M=-1\n");
                fprintf(writer->output, "(END_LT.%d)\n", writer->cond_i);

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
    Symbol symbol = NONE;

    printf("[CodeWriter(%s): %d] INFO: segment=%s\n", __func__, __LINE__, segment);
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

    else if (strcmp(segment, "temp") == 0)
    {
        symbol = TEMP;
    }

    printf("[CodeWriter(%s): %d] symbol=%d\n", __func__, __LINE__, symbol);

    if (command == C_PUSH)
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
            fprintf(writer->output, "@%d\n", index);
            fprintf(writer->output, "D=D+A\n");
            fprintf(writer->output, "A=D\n");
            fprintf(writer->output, "D=M\n");
            break;

        case STATIC:
        case TEMP:
            if (symbol == STATIC)
            {
                fprintf(writer->output, "@%s.%d\n", writer->file_name, index);
            }
            else
            {
                seg_addr = symbol + index;
                fprintf(writer->output, "@%d\n", seg_addr);
                fprintf(writer->output, "A=A\n");
            }

            fprintf(writer->output, "D=M\n");
            break;

        default:
            if (strcmp(segment, "pointer") == 0)
            {
                fprintf(writer->output, "@%s\n", index == 0 ? "THIS" : "THAT");
                fprintf(writer->output, "D=M\n");
            }
            // constant
            else
            {
                fprintf(writer->output, "@%d\n", index);
                fprintf(writer->output, "D=A\n");
            }
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

            // Store the sum of the base address and index in R13
            fprintf(writer->output, "D=M\n");
            fprintf(writer->output, "@%d\n", index);
            fprintf(writer->output, "D=D+A\n");
            fprintf(writer->output, "@R13\n");
            fprintf(writer->output, "M=D\n");

            // SP-- and insert popped value to address in R13
            fprintf(writer->output, "@SP\n");
            fprintf(writer->output, "AM=M-1\n");
            fprintf(writer->output, "D=M\n");
            fprintf(writer->output, "@R13\n");
            fprintf(writer->output, "A=M\n");
            fprintf(writer->output, "M=D\n");

            break;

        case STATIC:
        case TEMP:
        default:
            fprintf(writer->output, "@SP\n");
            fprintf(writer->output, "AM=M-1\n");
            fprintf(writer->output, "D=M\n");

            if (symbol == STATIC)
            {
                fprintf(writer->output, "@%s.%d\n", writer->file_name, index);
            }
            else if (symbol == TEMP)
            {
                seg_addr = symbol + index;
                fprintf(writer->output, "@%d\n", seg_addr);
            }
            // pointer
            else
            {
                fprintf(writer->output, "@%d\n", index == 0 ? THIS : THAT);
                fprintf(writer->output, "A=A\n");
            }

            fprintf(writer->output, "M=D\n");
            break;

        }

        printf("[CodeWriter(%s): %d] INFO: end\n", __func__, __LINE__);
    }
}

void close(CodeWriter *writer)
{
    printf("[CodeWriter(%s): %d] INFO: start\n", __func__, __LINE__);
    fprintf(writer->output, "// Add infinite loop at the end.\n");
    fprintf(writer->output, "(INFINITE_LOOP)\n");
    fprintf(writer->output, "@INFINITE_LOOP\n");
    fprintf(writer->output, "0;JMP\n");
    fclose(writer->output);
    free(writer->file_name);
    printf("[CodeWriter(%s): %d] INFO: end\n", __func__, __LINE__);
}