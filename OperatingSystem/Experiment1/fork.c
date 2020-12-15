
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
int main()
{
    pid_t pid;
    pid = fork();
    if(pid<0)
        printf("error in fork!");
    else if (pid == 0)
        printf("i am the child process my processId is %d\n", getpid());
    else
        printf("i am the child process my processId is %d\n", getpid());
}
