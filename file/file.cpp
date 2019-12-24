#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
void file_gid_uid(int uid, int gid)
{
    struct passwd *ptr;
    struct group *str; //结构体中存放文件所有者名和文件所有者组名

    ptr = getpwuid(uid);                            //调用函数获取文件所有者指针
    str = getgrgid(gid);                            //调用函数获取文件所有者组指针
    printf("\t%s\t%s", ptr->pw_name, str->gr_name); //打印文件所有者和文件所有者组
}
void file_mode(struct stat *buf)
{
    int i;
    char buff[11] = {"----------"};
    switch (buf->st_mode & S_IFMT) //按位&获取文件基本属性
    {
    case S_IFIFO:
        buff[0] = 'f';
        break;
    case S_IFDIR:
        buff[0] = 'd';
        break;
    case S_IFSOCK:
        buff[0] = 's';
        break;
    case S_IFBLK:
        buff[0] = 'b';
        break;
    case S_IFLNK:
        buff[0] = 'l';
        break;
    }

    if (buf->st_mode & S_IRUSR)
    {
        buff[1] = 'r';
    }
    if (buf->st_mode & S_IWUSR)
    {
        buff[2] = 'w';
    }
    if (buf->st_mode & S_IXUSR)
    {
        buff[3] = 'x';
    }
    if (buf->st_mode & S_IRGRP)
    {
        buff[4] = 'r';
    }
    if (buf->st_mode & S_IWGRP)
    {
        buff[5] = 'w';
    }
    if (buf->st_mode & S_IXGRP)
    {
        buff[6] = 'x';
    }
    if (buf->st_mode & S_IROTH)
    {
        buff[7] = 'r';
    }
    if (buf->st_mode & S_IWOTH)
    {
        buff[8] = 'w';
    }
    if (buf->st_mode & S_IXOTH)
    {
        buff[9] = 'x';
    }

    for (i = 0; i < 10; i++)
    {
        printf("%c", buff[i]); //循环打印
    }
}
void print_detail( struct stat* buf,dirent * sub_dir)
{
    // printf("|-");
    file_mode(buf);
    printf("  %d", buf->st_nlink);         //打印链接数
    file_gid_uid(buf->st_uid, buf->st_gid); //调用函数打印出文件拥有者和文件所有者组
    printf("\t%ld", buf->st_size);         //打印文件大小
    //Time();//获取时间
    printf("\t%.12s ", 4 + ctime(&(buf->st_mtime)));
    printf("   %s\n", sub_dir->d_name);
}
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
            for (int i = 0; i < depth - 1; i++)
            {
                printf("\t");
            }
            print_detail(&buf,sub_dir);
            print_dir(sub_dir->d_name, depth + 1);
        }
        else
        {
            for (int i = 0; i < depth - 1; i++)
            {
                printf("\t");
            }
            print_detail(&buf,sub_dir);
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