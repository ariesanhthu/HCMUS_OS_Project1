#include "kernel/types.h" 
#include "kernel/stat.h"  
#include "user/user.h" 

int main(int argc, char *args[])
{
    // array of 2 int for the pipe with 2 ends
    // p[0] for reading
    // p[1] for writing
    int p[2]; 


    char buffer[10];

    // create a pipe
    pipe(p);

    // call fork create a child process from the current process
    int pid;
    pid = fork(); 

    if (pid == 0) //child 
    {

        read(p[0], buffer, 1);
        printf("%d: received ping\n", getpid());
        close(p[0]);
        write(p[1], buffer, 1);
        close(p[1]);
    } 
    else if (pid > 0) // parent 
    {
        write(p[1], buffer, 1);
        close(p[1]);
        wait(0);
        read(p[0], buffer, 1);
        printf("%d: received pong\n", getpid());
        close(p[0]);        
    }
    else
    {
        printf("\nError!");
    }

    exit(0);
}