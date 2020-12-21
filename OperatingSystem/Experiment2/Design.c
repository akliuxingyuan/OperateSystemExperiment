#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHMKEY 54
#define K 1024

int shmid_1, shmid_2;
int main()
{
    int p, x, i, *pint;
    char *addr_1, *addr_2;
    char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                         'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'X'};
    shmid_1 = shmget(SHMKEY, 30 * K, 0777 | IPC_CREAT); /*建立30K共享区SHMKEY */
    addr_1 = shmat(shmid_1, 0, 0);                      /*挂接，并得到共享区首地址*/
    pint = (int *)addr_1;
    printf("addr_1 0x%x\n", addr_1);
    for (i = 0; i < 26; i++)
    {
        *pint = alphabet[i];
        pint++;
    }
    while ((p = fork()) == -1)
        ;
    if (p == 0)
    {
        shmid_2 = shmget(SHMKEY, 30 * K, 0777 | IPC_CREAT); /*建立16K共享区SHMKEY */
        addr_2 = shmat(shmid_2, 0, 0);                      /*挂接，并得到共享区首地址*/
        pint = (int *)addr_2;
        for (i = 0; i < 26; i++)
        {
            printf("%c ", *pint);
            *pint = *pint + 32; /* 转换 */
            pint++;
            if (i == 25)
                printf("\n");
        }
        x = shmdt(addr_2);
        exit(0);
    }
    else
    {
        sleep(5); /* 休眠5秒 */
        pint = (int *)addr_1;
        for (i = 0; i < 26; i++)
        {
            printf("%c ", *pint);
            pint++;
            if (i == 25)
                printf("\n");
        }
    }
}