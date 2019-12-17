#include<stdio.h>
#include<stdlib.h>
int main(int argc, const char** argv) {
    FILE* file = fopen("./input","r");
    char buff[10];
    int r;
    while ((r = fread(buff,sizeof(char),10,file)) !=0 )
    {
        printf("data:[%s]\n",buff);
    }
    return 0;
}