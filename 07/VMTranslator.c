#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "header/VMTranslator.h"
#include "header/Parser.h"
#include "header/CodeWriter.h"

int is_vm_file(char *file_path)
{
    printf("[VMTranslator(%s): %d] INFO: start\n", __func__, __LINE__);
    printf("[VMTranslator(%s): %d] INFO: file_path=%s\n", __func__, __LINE__, file_path);
    char *ext = strrchr(file_path, '.');
    if (ext == NULL) return 0;
    char *slash = strrchr(file_path, '/');
    char *file_name = NULL;
    if (slash != NULL)
    {
        int name_count = ext - ++slash;
        printf("[%s: %d] INFO: name_count=%d\n", __func__, __LINE__, name_count);
        file_name = (char *) malloc(name_count * sizeof(char *));
        strncpy(file_name, slash, name_count);
        file_name[name_count] = '\0';
        printf("[%s: %d] INFO: &slash=%p\n", __func__, __LINE__, slash);
        printf("[%s: %d] INFO: &ext=%p\n", __func__, __LINE__, ext);
        printf("[%s: %d] INFO: &file_path=%p\n", __func__, __LINE__, file_path);
        printf("[%s: %d] INFO: &file_name=%p\n", __func__, __LINE__, file_name);
    }
    else
    {
        int name_count = strlen(file_path);
        printf("[%s: %d] INFO: name_count=%d\n", __func__, __LINE__, name_count);
        file_name = (char *) malloc(name_count * sizeof(char *));
        strncpy(file_name, file_path, name_count);
    }
    printf("[%s: %d] INFO: file_name=%s\n", __func__, __LINE__, file_name);

    if (strcmp(ext, ".vm") == 0)
    {
        int result = 'A' <= file_name[0] && file_name[0] <= 'Z' ? 1 : 0;
        free(file_name);
        printf("[%s: %d] INFO: end result=%d\n", __func__, __LINE__, result);
        return result;
    }
    else
    {
        free(file_name);
        printf("[%s: %d] INFO: end result=%d\n", __func__, __LINE__, 0);
        return 0;
    }
}

int main(int argc, char **argv)
{
    printf("[%s: %d] INFO: start\n", __func__, __LINE__);
    char *vm_file = NULL;

    if (argc > 1)
    {
        if (is_vm_file(argv[1]) == 1)
        {
            vm_file = argv[1];
        }
        else
        {
            printf("[%s: %d] ERROR: The passed argument is not vm file.\n", __func__, __LINE__);
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
            printf("[%s: %d] ERROR: Cannot open current directory.\n", __func__, __LINE__);
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
        printf("[%s: %d] ERROR: .vm file is not found.\n", __func__, __LINE__);
        return 1;
    }

    int init_result = 0;

    // initialize
    Parser parser;
    init_result = initialize_parser(&parser, vm_file);
    if (init_result == 1) exit(init_result);

    CodeWriter writer;
    init_result = initialize_writer(&writer, vm_file);
    if (init_result == 1) exit(init_result);

    while (true)
    {
        fprintf(writer.output, "// %s\n", parser.current_instruction);
        CommandType type = command_type(&parser);
        printf("[%s: %d] INFO: type=%d\n", __func__, __LINE__, type);

        char *arg1_val = NULL;
        void *result = NULL;
        int arg2_val = 0;

        switch (type)
        {
            case C_ARITHMETIC:
                result = arg1(&arg1_val, &parser, type);
                printf("[%s: %d] INFO: arg1=%s\n", __func__, __LINE__, arg1_val);

                write_arithmetic(&writer, arg1_val);

                if (result != NULL) free(arg1_val);
                break;

            case C_PUSH:
            case C_POP:
                result = arg1(&arg1_val, &parser, type);
                printf("[%s: %d] INFO: arg1=%s\n", __func__, __LINE__, arg1_val);
                arg2(&arg2_val, &parser, type);
                printf("[%s: %d] INFO: arg2=%d\n", __func__, __LINE__, arg2_val);

                write_push_pop(&writer, type, arg1_val, arg2_val);

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
    printf("[%s: %d] INFO: end\n", __func__, __LINE__);

    return 0;
}
