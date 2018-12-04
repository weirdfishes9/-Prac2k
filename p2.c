#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

enum 
{ 
    PART_SIZE = 32,
    ARGS_NUM = 3,
    FLD_PASS = 3,
    LEN_INT = 9
};

char * 
getln(FILE *f)
{
    int buf_size = PART_SIZE, in_cycle = 0;
    char *buffer = malloc(buf_size * sizeof(buffer[0]));
    if (buffer == NULL) {
        return NULL;
    }
    while(fgets(buffer + buf_size - PART_SIZE,
            PART_SIZE * sizeof(buffer[0]), f) != 0) {
        in_cycle = 1;
        buf_size += PART_SIZE - 1;
        char *next_ln = strchr(buffer, '\n');
        if (next_ln != NULL) {
            buf_size = next_ln - buffer;
            buffer[buf_size] = '\0';
            break;
        }
        buffer = realloc(buffer, buf_size * sizeof(buffer[0]));
        if (buffer == NULL) {
            return NULL;
        }
    }
    if(in_cycle == 0) {
        free(buffer);
        return NULL;
    }
    buffer = realloc(buffer, (strlen(buffer) + 1) * sizeof(buffer[0]));
    return buffer;
}

int
is_digit(char *line)
{
    int count = 0, rez = 1;
    while(line[count] != '\0') {
        if (!isdigit(line[count])) {
            rez = 0;
            break;
        }
        count++;
    }
    return rez;
}

int 
main(int argc, char **argv)
{
    char *group = NULL, *file_name = NULL;
    if (argc != ARGS_NUM) {
        fprintf(stderr, "%s\n", "Wrong number of arguments");
        return 0;
    }
    if (is_digit(argv[1])) {
        group = argv[1];
        file_name = argv[2];
    } else {
        group = argv[2];
        file_name = argv[1];        
    }
    if (!is_digit(group) || strlen(group) > LEN_INT) {
        fprintf(stderr, "%s\n", "Invalid group number");
        return 0;
    }
    FILE *input_file = fopen("/etc/passwd","r");
    FILE *output_file = fopen(file_name, "w");
    if (output_file == NULL) {
        fclose(input_file);
        perror(NULL);
        return 0;
    }
    char *main_str = NULL, *aux_str = NULL, *comp_str = NULL; 
    while ((main_str = getln(input_file)) != NULL) {
            aux_str = main_str;
            for (int cnt = 0; cnt < FLD_PASS; cnt++) {
                aux_str = strchr(aux_str, ':');
                aux_str = aux_str + 1;
            }
            comp_str = calloc(strchr(aux_str, ':') - aux_str + 1, sizeof(aux_str[0]));
            if (comp_str == NULL) {
                perror(NULL);
                return 0;
            }
            comp_str = memcpy(comp_str, aux_str, 
                    (strchr(aux_str, ':') - aux_str) * sizeof(aux_str[0]));
            if (strcmp(group, comp_str) == 0) {
                fprintf(output_file, "%s\n", main_str);
            }
            free(main_str);
            free(comp_str);
    }    
    fclose(input_file);
    fclose(output_file);
}