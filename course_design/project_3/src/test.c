#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

char buf[128];
char buff[128];

int main()
{
    int fd, m, n;
    char* write_data = "hBenjiBenji";
    fd = open("/dev/hello", O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open file \"/dev/hello\" failed\n");
        exit(-1);
    }
    printf("read content:");

    while ((m = read(fd, buf, 1)) > 0 && buf[0] != '\0') {
        printf("%c", buf[0]);
    }
    printf("\n");
    llseek(fd, 0, 0);
    n = write(fd, write_data, 13);
    printf("write length: %d\n", n);
    
    llseek(fd, 0, 0);
    printf("read: ");
    while ((m = read(fd, buf, 1)) > 0 && buf[0] != '\0') {
        printf("%c", buf[0]);
    }
    
    printf("\n");

    llseek(fd, 0, 0);

    n = write(fd, "hello world", 11);
    printf("write length: %d\n", n);

    llseek(fd, 0, 0);

    printf("read: ");
    while ((m = read(fd, buf, 1)) > 0 && buf[0] != '\0') {
        printf("%c", buf[0]);
    }
    printf("\n");    
    close(fd);
    return 0;
}