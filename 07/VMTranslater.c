#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "header/Parser.h"
#include "header/CodeWriter.h"


int is_vm_file(const char *file_name)
{
    printf("[is_vm_file] start\n");
    printf("file_name=%s\n", file_name);
    const char *ext = strrchr(file_name, '.');
    if (ext == NULL) return 0;

    if (strcmp(ext, ".vm") == 0)
    {
        return 'A' <= file_name[0] && file_name[0] <= 'Z' ? 1 : 0;
    }
    else
    {
        return 0;
    }
}

int main(int argc, char **argv)
{
    printf("[main] start\n");
    char *vm_file = NULL;

    if (argc > 1)
    {
        if (is_vm_file(argv[1]) == 1)
        {
            vm_file = argv[1];
        }
        else
        {
            printf("The passed argument is not vm file.\n");
            return 1;
        }
    }
    else
    {
        DIR *dir;
        struct dirent *entry;
        dir = opendir(".");
        if (dir == NULL)
        {
            printf("Cannot open current directory.\n");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type != DT_DIR)
            {
                char *tmp_file = entry->d_name;
                if (is_vm_file(tmp_file) == 1)
                {
                    vm_file = tmp_file;
                    break;
                }
            }
        }
    }
    if (vm_file == NULL)
    {
        printf(".vm file is not found.\n");
        return 1;
    }

    // initialize
    Parser parser;
    initialize_parser(&parser, vm_file);

    CodeWriter writer;
    initialize_writer(&writer, vm_file);

    while (true)
    {
        CommandType type = command_type(&parser);
        printf("type=%d\n", type);

        char *arg1_val = NULL;
        void *result = NULL;
        int *arg2_val = NULL;

        switch (type)
        {
            case C_ARITHMETIC:
                result = arg1(&arg1_val, &parser, type);
                printf("arg1=%s\n", arg1_val);

                write_arithmetic(&writer, arg1_val);

                if (result != NULL) free(arg1_val);
                break;

            case C_PUSH:
            case C_POP:
                result = arg1(&arg1_val, &parser, type);
                printf("arg1=%s\n", arg1_val);
                arg2(&arg2_val, &parser, type);
                printf("arg2=%d\n", *arg2_val);

                write_push_pop(&writer, type, arg1_val, *arg2_val);

                if (result != NULL) free(arg1_val);
                break;

            default:
                break;
        }

        if (has_more_lines(&parser) == false) break;
        advance(&parser);
    }

    // close
    parser_finalize(&parser);
    close(&writer);
}
