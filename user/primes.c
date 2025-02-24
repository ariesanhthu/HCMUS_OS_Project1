#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//prevent infinite recursive call error
void primes(int p_read) __attribute__((noreturn)); 

void primes(int p_read) 
{
    int p[2];
    int prime, num;

    if (read(p_read, &prime, sizeof(prime)) == 0) 
    {
        close(p_read);
        exit(0);
    }

    printf("prime %d\n", prime);

    // create a pipe 
    pipe(p);

    int pid = fork();
    
    //child
    if (pid == 0) 
    {
        close(p[1]);
        close(p_read);

        //recursively call primes w new p_read
        primes(p[0]); 
    } 
    else 
    //parent
    if (pid > 0) 
    {
        //close p_read in the parent process
        close(p[0]); 

        while (read(p_read, &num, sizeof(num)) != 0) 
            if (num % prime != 0) //sieve 
                write(p[1], &num, sizeof(num));

        //current pipe
        close(p[1]); 
      
        // previous pipe
        close(p_read); 
      
        wait(0);
        exit(0);
    } 
    else 
    {
        printf("\nFork error\n");
        close(p_read);
        exit(1);
    }
}

int main(int argc, char *argv[]) 
{
    int p[2];
    pipe(p);

    int pid = fork();
  
    //child
    if (pid == 0) 
    {
        close(p[1]);
        primes(p[0]);//start w p_read of init pipe
    } 
    else 
    //parent
    if (pid > 0) 
    {
        close(p[0]);

        //feed num from 2-->280
        for (int i = 2; i <= 280; i++) 
            write(p[1], &i, sizeof(i));

        close(p[1]);
        // wait until the entire pipeline terminates, 
        wait(0); 
        
        exit(0);
    } 
    else 
    {
        printf("\nFork error\n");
        exit(1);
    }
}
