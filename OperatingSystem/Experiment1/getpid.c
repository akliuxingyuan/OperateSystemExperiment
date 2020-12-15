/* getpid_test.c */
#include<unistd.h> /* 提供函数的定义 */
#include<stdio.h>

int main()
{
	printf("The current process ID is %d\n",getpid());
    return 0;
}
