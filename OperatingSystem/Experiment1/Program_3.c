#include<sys/wait.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>

int main()
{
    int m,n,k;
    m=fork();
    printf("PID:%d\t", getpid());
    printf("the return value of fork():%d\t\t", m);
    printf("he\n");

    n=fork();
    printf("PID:%d\t", getpid());
    printf("the return value of fork():%d\t\t", n);
    printf("ha\n");

    k = fork();
    printf("PID:%d\t", getpid());
    printf("the return value of fork():%d\t\t", k);
    printf("ho\n");
}
