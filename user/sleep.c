// user/sleep.c
 
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
 
int main(int argc, char* argv[]){
    if(argv[1] == 0){
        fprintf(1, "Usage: sleep [Seconds]\n");
        exit(1);
    }
 
    for(uint i = 0; argv[1][i] != 0; i++){
        if(!(argv[1][i] >= '0' && argv[1][i] <= '9')){
            fprintf(1, "Usage: sleep [Seconds_in_Integer]\n");
            exit(1);
        }
    }
    uint Seconds = atoi(argv[1]);
 
    sleep(Seconds);
 
    exit(0);
}