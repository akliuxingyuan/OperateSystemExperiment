#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<unistd.h>

int main()
{   int x,fd[2],fo[2],temp;
    char buf[30],s[30],ans[30];
    pipe(fd);
    pipe(fo);
    while ((x=fork())==-1);
    if (x==0)
    {
        close(fd[1]);
        close(fo[0]);
        read(fd[0],buf,30);
        printf("Child Process receive string:%s\n",buf);
        int len=strlen(buf);
        for(int i=0; i<=len/2; i++){
            temp = buf[i];
            buf[i] = buf[len-1-i];
            buf[len-1-i] = temp;
        }
        write(fo[1],buf,30);
    }
    else{
        strcpy(s,"This_is_an_example");
        s[19]='\0';
        close(fd[0]);
        close(fo[1]);
        write(fd[1],s,30);
        read(fo[0],ans,30);
        printf("Parent Process receive rev-string:%s\n",ans);
        wait(NULL);
    }
}
