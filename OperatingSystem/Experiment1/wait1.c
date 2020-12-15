#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
    pid_t pc,pr;
    pc = fork();
    if (pc<0)
        printf("error occurred\n");
    else if (pc == 0)
    {
        printf("this is a child process with pid %d\n", getpid());
        sleep(10);
    }
    else
    {
        pr = wait(NULL);
        printf("i catched a child process with pid of %d\n", pr);
    }
    exit(0);
}

