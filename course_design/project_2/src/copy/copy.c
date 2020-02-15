#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include<fcntl.h>
#include<string.h>
int main()
{
        char* src = "input";
        char* dst = "output";
        long  amma = syscall(335,src,dst);
        printf("System call  returned %ld\n", amma);
        return 0;
}
