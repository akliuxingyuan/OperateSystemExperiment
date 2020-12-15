#include<sys/wait.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>


void print_msg(int x){
    printf("PID:%d\t", getpid());
    printf("the return value of fork():%d\t\t\n", x);
}

int main()
{
    int i,j,k,l;
    if(i=fork()){
        print_msg(i);
        if(k=fork()){
            print_msg(k);
            if(l=fork()){
                print_msg(l);
            }else{
                print_msg(l);
            }
        }else{
            print_msg(k);
        }
    }
    else {
        if(j=fork()){
            print_msg(j);
        }else{
            print_msg(j);
        }
    }
}
