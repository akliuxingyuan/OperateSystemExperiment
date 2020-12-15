#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    key_t key=54; /*在实际实验过程中，为了避免每个同学建立的共享存储区关键字一样而相互干扰，关键字请用学号末3位*/
    int shmid_1,shmid_2;
    if ((shmid_1=shmget(key,1000,0644|IPC_CREAT))==-1){
        perror("shmget shmid_1");exit(1);
    }
    printf("First shared memory identifier is %d\n",shmid_1);
    if ((shmid_2=shmget(IPC_PRIVATE,20,0644))==-1){
        perror("shmget shmid_2");exit(2);
    }
    printf("Second shared memory identifier is %d\n",shmid_2);
    exit(0);
}
