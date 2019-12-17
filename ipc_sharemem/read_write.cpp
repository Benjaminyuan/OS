#include <error.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUFFSIZE 12
void P(int semid, int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;
    sem.sem_flg = 0;
    semop(semid, &sem, 1);
}
void V(int semid, int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = 1;
    sem.sem_flg = 0;
    semop(semid, &sem, 1);
}
int main(int argc, char *argv[])
{
    int semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, 1);
    semctl(semid, 1, SETVAL, 0);
    int ret;
    pid_t p1;
    pid_t p2;
    p1 = fork();

    // 写进程
    if (p1 == 0)
    {
        printf("p1\n");
        int f = open("./input", O_RDONLY);
        P(semid, 0);
        key_t key = ftok("./", 2015);
        if (key == -1)
        {
            perror("fail to create key");
            exit(-1);
        }
        int shmid = shmget(key, BUFFSIZE, IPC_CREAT | 0666);
        if (shmid < 0)
        {
            perror("fail to create ");
            exit(-1);
        }
        char *shmadd = (char *)shmat(shmid, NULL, 0);
        if (shmadd < 0)
        {
            perror("shmat");
            exit(-1);
        }

        bzero(shmadd, BUFFSIZE);
        read(f, shmadd, sizeof(char) * BUFFSIZE);
        printf("write data success: %s\n", shmadd);
        V(semid, 1);
        close(f);
        return 0;
    }
    // 读进程
    if (p2 == 0)
    {
        printf("p2\n");
        P(semid, 1);
        key_t key = ftok("./", 2015);
        if (key == -1)
        {
            perror("ftok");
        }
        int shmid = shmget(key, BUFFSIZE, 0666);
        if (shmid < 0)
        {
            perror("shmget");
            exit(-1);
        }
        //映射
        char *recv = (char *)shmat(shmid, NULL, 0);
        if (recv < 0)
        {
            perror("shmat");
            exit(-1);
        }
        //读共享内存区数据
        printf("data = [%s]\n", recv);
        system("ipcs -m"); //查看共享内存
        if (shmctl(shmid, IPC_RMID, NULL) < 0)
        {
            perror("shmctl");
            exit(0);
        }
        V(semid, 0);
        return 0;
    }
    int status_1;
    int status_2;
    waitpid(p1, &status_1, 0);
    waitpid(p2, &status_2, 0);
    printf("%d,%d", status_1, status_2);
    semctl(semid, 0, IPC_RMID);
    semctl(semid, 1, IPC_RMID);

    system("ipcs -m"); //查看共享内存
}