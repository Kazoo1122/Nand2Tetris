#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "header/Parser.h"


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

    int cc = strlen(vm_file);
    char asm_file[cc + 1];
    strncpy(asm_file, vm_file, (cc - 3));
    strcat(asm_file, ".asm");
    printf("vm_file=\'%s\', asm_file=\'%s\'\n", vm_file, asm_file);
    Parser parser;
    initialize_parser(&parser, vm_file);
    CommandType type = command_type(&parser);
    printf("type=%d\n", type);
    char *arg1_var;
    void *result;
    result = arg1(&arg1_var, &parser, type);
    printf("arg1=%s\n", arg1_var);
    if (result != NULL) free(arg1_var);
    int *arg2_var;
    arg2(&arg2_var, &parser, type);
    printf("arg2=%d\n", *arg2_var);
    parser_finalize(&parser);
}
