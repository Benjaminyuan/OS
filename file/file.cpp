#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
void print_dir(const char *file_name, int depth)
{
    DIR *dir = opendir(file_name);
    if (dir == NULL)
    {
        perror("dir not exist");
        exit(0);
    }
    chdir(file_name);
    dirent *sub_dir;
    struct stat buf;
    while ((sub_dir = readdir(dir)) != NULL)
    {
        lstat(sub_dir->d_name, &buf);
        if (S_ISDIR(buf.st_mode))
        {
            if (strcmp(sub_dir->d_name, "..") == 0 || strcmp(sub_dir->d_name, ".") == 0)
            {
                continue;
            }
            for (int i = 0; i < depth-1; i++)
            {
                if(i == 0 ){
                    printf("|");
                }
                printf("\t");
            }
            printf("|-");
            printf("dir:%s,depth:%d\n", sub_dir->d_name, depth);
            if (depth >= 1000)
            {
                return;
            }
            print_dir(sub_dir->d_name, depth + 1);
        }
        else
        {
            for (int i = 0; i < depth-1; i++)
            {
                if(i == 0 ){
                    printf("|");
                }
                printf("\t");
            }
            printf("|-");
            printf("file:%s,depth:%d\n", sub_dir->d_name, depth);
        }
    }
    chdir("..");
    closedir(dir);
    return;
}
const char *FILENAME = "/root/OS";
int main()
{
    print_dir(FILENAME, 0);
    return 0;
}