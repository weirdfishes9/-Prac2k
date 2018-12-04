#include <fcntl.h>
#include <unistd.h>
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
getln(int *f)
{
    int buf_size = PART_SIZE, size = 1, in_cycle = 0;
    char *buffer = malloc(buf_size * sizeof(buffer[0]));
    if (buffer == NULL) {
        return NULL;
    }
    while (read(*f, buffer + size - 1, sizeof(*buffer)) == sizeof(*buffer) &&
            (buffer[size - 1] != '\n')) {
        in_cycle = 1;
        size++;
        if (size == buf_size) {
            buf_size += PART_SIZE;
            buffer = realloc(buffer, buf_size);
        }
    }
    if( in_cycle == 0)
    {
        free(buffer);
        return NULL;
    }
    buffer[size - 1] = '\0';
    buffer = realloc(buffer, strlen(buffer) + 1);
    return buffer;
}

int
is_digit(const char *line)
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

void
_perror(const char *mes)
{
    if (mes == NULL) {
        write(2, strerror(errno), strlen(strerror(errno)));
        write(2, "\n", sizeof(char));
    } else {
        write(2, mes, strlen(mes));
    }
}

int 
main(int argc, char **argv)
{
    char *group = NULL, *file_name = NULL;
    if (argc != ARGS_NUM) {
        _perror("Wrong number of arguments\n");
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
        _perror("Invalid group number\n");
        return 0;
    }
    int input_file = open("/etc/passwd", O_RDONLY);
    int output_file = open(file_name, O_WRONLY|O_TRUNC|O_CREAT, 0644);
    if (output_file == -1) {
        close(input_file);
        _perror(NULL);
        return 0;
    }
    char *main_str = NULL, *aux_str = NULL, *comp_str = NULL; 
    while ((main_str = getln(&input_file)) != NULL) {
            aux_str = main_str;
            for (int cnt = 0; cnt < FLD_PASS; cnt++) {
                aux_str = strchr(aux_str, ':');
                aux_str = aux_str + 1;
            }
            comp_str = calloc(strchr(aux_str, ':') - aux_str + 1, sizeof(aux_str[0]));
            if (comp_str == NULL) {
                _perror(NULL);
                return 0;
            }
            comp_str = memcpy(comp_str, aux_str, 
                    (strchr(aux_str, ':') - aux_str) * sizeof(aux_str[0]));
            if (strcmp(group, comp_str) == 0) {
                write(output_file, main_str, strlen(main_str));
                write(output_file, "\n", 1);
            }
            free(main_str);
            free(comp_str);
    }    
    close(input_file);
    close(output_file);
}