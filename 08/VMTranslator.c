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
        printf("[VMTranslator(%s): %d] INFO: name_count=%d\n", __func__, __LINE__, name_count);
        file_name = (char *) malloc(name_count * sizeof(char *));
        strncpy(file_name, slash, name_count);
        file_name[name_count] = '\0';
        printf("[VMTranslator(%s): %d] INFO: &slash=%p\n", __func__, __LINE__, slash);
        printf("[VMTranslator(%s): %d] INFO: &ext=%p\n", __func__, __LINE__, ext);
        printf("[VMTranslator(%s): %d] INFO: &file_path=%p\n", __func__, __LINE__, file_path);
        printf("[VMTranslator(%s): %d] INFO: &file_name=%p\n", __func__, __LINE__, file_name);
    }
    else
    {
        int name_count = strlen(file_path);
        printf("[VMTranslator(%s): %d] INFO: name_count=%d\n", __func__, __LINE__, name_count);
        file_name = (char *) malloc(name_count * sizeof(char *));
        strncpy(file_name, file_path, name_count);
    }
    printf("[VMTranslator(%s): %d] INFO: file_name=%s\n", __func__, __LINE__, file_name);

    if (strcmp(ext, ".vm") == 0)
    {
        int result = 'A' <= file_name[0] && file_name[0] <= 'Z' ? 1 : 0;
        free(file_name);
        printf("[VMTranslator(%s): %d] INFO: end result=%d\n", __func__, __LINE__, result);
        return result;
    }
    else
    {
        free(file_name);
        printf("[VMTranslator(%s): %d] INFO: end result=%d\n", __func__, __LINE__, 0);
        return 0;
    }
}

void translate(const char *vm_file, Parser parser, CodeWriter writer)
{
    while (true)
    {
        fprintf(writer.output, "// %s\n", parser.current_instruction);
        CommandType type = command_type(&parser);
        printf("[VMTranslator(%s): %d] INFO: type=%d\n", __func__, __LINE__, type);

        char *arg1_val = NULL;
        void *result = NULL;
        int arg2_val = 0;

        switch (type)
        {
            case C_ARITHMETIC:
                result = arg1(&arg1_val, &parser, type);
                printf("[VMTranslator(%s): %d] INFO: arg1=%s\n", __func__, __LINE__, arg1_val);

                write_arithmetic(&writer, arg1_val);

                if (result != NULL) free(arg1_val);
                break;

            case C_PUSH:
            case C_POP:
                result = arg1(&arg1_val, &parser, type);
                printf("[VMTranslator(%s): %d] INFO: arg1=%s\n", __func__, __LINE__, arg1_val);
                arg2(&arg2_val, &parser, type);
                printf("[VMTranslator(%s): %d] INFO: arg2=%d\n", __func__, __LINE__, arg2_val);

                write_push_pop(&writer, type, arg1_val, arg2_val);

                if (result != NULL) free(arg1_val);
                break;

            default:
                break;
        }

        if (has_more_lines(&parser) == false) break;
        advance(&parser);
    }
}

int main(int argc, char **argv)
{
    printf("[VMTranslator(%s): %d] INFO: start\n", __func__, __LINE__);
    const char *vm_file = NULL;
    char *asm_file = NULL;
    DIR *input_dir = NULL;

    if (argc > 1)
    {
        int count = 0;
        if (is_vm_file(argv[1]) == 1)
        {
            vm_file = argv[1];

            // exclude vm extention length
            count = strlen(argv[1]) - 3;
        }

        else
        {
            input_dir = opendir(argv[1]);
            if (input_dir == NULL)
            {
                printf("[VMTranslator(%s): %d] ERROR: Cannot open passed directory.\n", __func__, __LINE__);
                return 1;
            }

            count = strlen(argv[1]);
        }

        printf("[VMTranslator(%s): %d] INFO: count=%d \n", __func__, __LINE__, count);

        asm_file = (char *) malloc(count * sizeof(char *));
        strncpy(asm_file, argv[1], count);
        asm_file[count] = '\0';
    }

    else
    {
        DIR *current_dir = NULL;
        struct dirent *current_dir_entry = NULL;
        current_dir = opendir(".");
        if (current_dir == NULL)
        {
            printf("[VMTranslator(%s): %d] ERROR: Cannot open current directory.\n", __func__, __LINE__);
            return 1;
        }
        while ((current_dir_entry = readdir(current_dir)) != NULL)
        {
            if (current_dir_entry->d_type != DT_DIR)
            {
                char *tmp_file = current_dir_entry->d_name;
                if (is_vm_file(tmp_file) == 1)
                {
                    vm_file = tmp_file;
                    break;
                }
            }
        }

        if (vm_file == NULL)
        {
            printf("[VMTranslator(%s): %d] ERROR: vm file is not found.\n", __func__, __LINE__);
            return 1;
        }
    }

    int init_result = 0;

    CodeWriter writer;
    Parser parser;

    // When passed an argument is a vm file
    if (input_dir == NULL)
    {
        init_result = initialize_writer(&writer, asm_file);

        if (init_result == 1) exit(init_result);

        char file_name[64];
        char *slash = strrchr(asm_file, '/');
        strcpy(file_name, ++slash);
        printf("[VMTranslator(%s): %d] INFO: file_name=%s \n", __func__, __LINE__, file_name);

        set_file_name(&writer, file_name);
        init_result = initialize_parser(&parser, vm_file);
        if (init_result == 1) exit(init_result);

        translate(vm_file, parser, writer);
    }

    // Or an argument is a directory
    else
    {
        init_result = initialize_writer(&writer, asm_file);
        if (init_result == 1) exit(init_result);

        struct dirent *input_entry = NULL;
        while ((input_entry = readdir(input_dir)) != NULL)
        {
            if (input_entry->d_type != DT_DIR)
            {
                char *file_name = input_entry->d_name;
                if (is_vm_file(file_name) == 1)
                {
                    char tmp_file_path[128];
                    sprintf(tmp_file_path, "%s/%s", argv[1], file_name);
                    vm_file = tmp_file_path;

                    init_result = initialize_parser(&parser, vm_file);
                    if (init_result == 1) exit(init_result);
                    set_file_name(&writer, file_name);

                    translate(vm_file, parser, writer);
                }
            }
        }
    }

    // close
    free(asm_file);
    finalize_parser(&parser);
    close(&writer);
    printf("[VMTranslator(%s): %d] INFO: end\n", __func__, __LINE__);

    return 0;
}
