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
#define BUFFSIZE 512
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
    int shmid;
    int ret;
    pid_t p1;
    pid_t p2;
    p1 = fork();
    // 写进程
    if (p1 == 0)
    {
        P(semid, 0);
        key_t key = ftok("../", 2015);
        if (key == -1)
        {
            perror("fail to create key");
            exit(-1);
        }
        shmid = shmget(key, BUFFSIZE, IPC_CREAT | 0666);
        if (shmid < 0)
        {
            perror("fail to create share buff");
            exit(-1);
        }
        char *shmadd = (char *)shmat(shmid, NULL, 0);
        if (shmadd < 0)
        {
            perror("shmat");
            exit(-1);
        }

        bzero(shmadd, BUFFSIZE);
        strcpy(shmadd, "write proccess");
        printf("write data success\n");
        V(semid, 1);
        return 0;
    }
    // 读进程
    if (p2 == 0)
    {
        P(semid, 1);
        key_t key = ftok("../", 2015);
        if (key == -1)
        {
            perror("fail to create key");
            exit(-1);
        }
        shmid = shmget(key, BUFFSIZE, IPC_CREAT | 0666);
        if (shmid < 0)
        {
            perror("fail to create share buff");
            exit(-1);
        }
        char *shmadd = (char *)shmat(key, NULL, 0);
        if (shmadd < 0)
        {
            perror("shmat");
            exit(-1);
        }

        char *recv = (char *)shmat(shmid, NULL, SHM_RDONLY);
        printf("data = [%s]\n", recv);
        V(semid, 0);
        return 0;
    }
    int status_1;
    int status_2;
    waitpid(p1, &status_1, 0);
    waitpid(p2, &status_2, 0);
    semctl(semid, 0, IPC_RMID);
    semctl(semid, 1, IPC_RMID);
}