#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
int main(int argc ,char * argv[]){
    if(argc != 3 ){
        perror("invalid paramer");
        return  -1 ;
    }
    char * src = "./input";
    char * dst = "./output-1";
    copy_file(argv[1],argv[2]);
}
void copy_file(const char* src,const char* dst){
    FILE* f = fopen(src,"r");
    if(f == NULL){
        perror(strerror(errno));
        return ;
    }
    fseek(f,0L,SEEK_END);
    int len = ftell(f);
    rewind(f); 
    char buff[len];
    FILE* d = fopen(dst,"w+");
    if(fread(buff,sizeof(char),len,f)< 0){
        perror(strerror(errno));
        return;
    }
    if(fwrite(buff,sizeof(char),len,d) < 0 ){
        perror(strerror(errno));
        return ;
    }
    return ;
}
int file_exit(char *file_name){
    struct stat buffer;
    return (stat(file_name,&buffer) == 0);
}