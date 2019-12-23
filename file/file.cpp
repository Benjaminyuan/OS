#include<sys/types.h>
#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<unistd.h>
const char fileName[100]= "/Users/mac/Documents/ComputerSystem/operatingSystem/OS";
int main(){
    DIR* dir = opendir(fileName);
    if(dir == NULL){
        perror("dir not exist");
        exit(0);
    }
    while(1){
        dirent* subDir = readdir(dir);
        if (subDir == NULL){
            break;
        }
        printf("---------\n");
        printf("dir:%s\n",subDir->d_name);
        struct stat buf;
        stat(subDir->d_name,&buf);
        printf("%d\n",(buf.st_mode & S_IFDIR) == S_IFDIR);
        printf("---------\n");
    }
    return 0;
}