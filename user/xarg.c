#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h" //MAXARG

#define CHUNK_SIZE 512

int main (int argc, char * argv[]){
    if (argc < 2) {
        printf("xargs: error\n");
    }
    
    char * buff = malloc(CHUNK_SIZE);
    if (!buff) {
        printf("xargs: malloc error\n");
        exit(0);
    }

    // Read input from stdin
    int buff_size = 0;
    int chunk_read = 0;
    while ((chunk_read = read(0, buff + buff_size, CHUNK_SIZE - 1)) > 0) {
        buff_size += chunk_read;
        char * new_buff = malloc(buff_size + CHUNK_SIZE);
        if (!new_buff) printf("xargs: malloc error \n");
        for (int i = 0; i < buff_size; i++) {
            new_buff[i] = buff[i];
        }
        free(buff);
        buff = new_buff;
    }
    buff[buff_size - 1] = '\0';

    // create new args list
    int num_arg = argc - 1;
    char * args[MAXARG];

    for (int i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }

    // detect all arguament 
    char * p = buff;
    while (*p) {
        while (*p == ' ' || *p == '\n' || *p == '\t') p++; 
        if (!*p) break;

        args[num_arg++] = p;  

        while (*p && *p != ' ' && *p != '\n' && *p != '\t') p++;  // find the end of argument
        if (*p) *(p++) = '\0'; 
    }

    //execute the command
    args[num_arg] = 0;
    int pid = fork();
    if (pid < 0) {
                printf("xargs: fork error\n");
            }
    else if (pid == 0) {
        exec(args[0], args);
        printf("xargs: exec error\n"); // Chỉ chạy khi exec thất bại
        exit(1);
    }
    else {
        wait(0);
    }

    free(buff);
    exit(0);
}