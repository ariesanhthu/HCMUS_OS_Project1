#include "kernel/types.h" 
#include "kernel/stat.h"  
#include "user/user.h" 

int main(int argc, char *args[])
{
    // p[0]: reading
    // p[1]: writing
    int p[2]; 
    char buffer[10];

    // create a pipe
    pipe(p);

    // call fork create a child process from the current process
    int pid;
    pid = fork(); 

    //Tien trinh con 
    if (pid == 0) 
    {

        read(p[0], buffer, 1);
        printf("%d: received ping\n", getpid());
        close(p[0]);
        write(p[1], buffer, 1);
        close(p[1]);
    } 
    // Tien trinh cha 
    else if (pid > 0) 
    {
        // Ghi buffer
        write(p[1], buffer, 1);
        close(p[1]);
      
        wait(0);

        // Doc buffer
        read(p[0], buffer, 1);
      
        printf("%d: received pong\n", getpid());
        close(p[0]);        
    }
    else
        printf("\nError!");

    exit(0);
}
