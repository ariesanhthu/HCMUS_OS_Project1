#include "kernel/types.h"
#include "kernel/stat.h"

#include "user/user.h"

#define CHUNK_SIZE 3

int main (int argc, char * argv[]){
    if (argc < 2) {
        printf("xargs: error\n");
    }
    
    // Read input from stdin
    char * buff = malloc(CHUNK_SIZE);
    if (!buff) {
        printf("xargs: malloc error\n");
        exit(0);
    }
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
    char * args[num_arg + 1];

    for (int i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }

    char * p = buff;

    // detect arguament one by one and execute the command
    while (*p) {
        while (*p == '\n') p++; 
        if (!*p) break;

        args[num_arg] = p;  
        args[num_arg + 1] = 0; // assign null at the end 

        while (*p && *p != '\n') p++;  // find the end of argument
        if (*p) *(p++) = '\0'; 

        int pid = fork();
        if (pid < 0) {
            printf("xargs: fork error\n");
            free(buff);
            exit(1);
        }
        if (pid == 0) {
            exec(args[0], args);
            printf("xargs: exec error\n");
            exit(1);
        } else {
            wait(0);
        }
    }
    free(buff);
    exit(0);
}