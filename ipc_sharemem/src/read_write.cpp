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
#include<ctime>
#include <fcntl.h>
#define BUFFSIZE 200
#define BLOCK_NUM 10
#define SEM_NUM 5
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
    // write
    semctl(semid, 0, SETVAL, SEM_NUM);
    // read
    semctl(semid, 1, SETVAL, 0);
    int write_cur = 0,read_cur = 0;
    char *buffs[BLOCK_NUM];
    int shmids[BLOCK_NUM];
    for (int i = 0; i < BLOCK_NUM; i++)
    {
        key_t key = ftok("./", i);
        if (key == -1)
        {
            perror("fail to create key");
            exit(-1);
        }
        int shmid = shmget(key, BUFFSIZE, IPC_CREAT | 0666);
        shmids[i] = shmid;
        if (shmid < 0)
        {
            perror("fail to create");
            exit(-1);
        }
        buffs[i] = (char*) shmat(shmid,NULL,0);
    }
    int ret;
    pid_t p1;
    pid_t p2;
    p1 = fork();
    // 写进程
    if (p1 == 0)
    {
        int f = open("./input", O_RDONLY);
        int m = 0 ;
        bool should_break = false;
        while (1)
        {
            P(semid,0);
            bzero(buffs[write_cur], BUFFSIZE);
            m = read(f, buffs[write_cur], sizeof(char) * BUFFSIZE);
            if(m == 0){
                should_break = true;
            }
            write_cur = (write_cur+1)%BLOCK_NUM;
            V(semid,1);
            if(should_break){
                break;
            }
        }
        close(f);
        return 0;
    }
    // 读进程
    p2 = fork();
    if (p2 == 0)
    {
        bool should_break = false;
        int f = open("./output",O_WRONLY | O_CREAT);
        while (1)
        {
            P(semid, 1);
            int len = strlen(buffs[read_cur]);
            int m = write(f,buffs[read_cur],len);
            bzero(buffs[read_cur],BUFFSIZE);
            if(m == 0){     
                V(semid, 0);
                break;
            }
            read_cur = (read_cur+1)%BLOCK_NUM;
            V(semid, 0);
        }
        return 0;
    }
    int status_1;
    int status_2;
    waitpid(p1, &status_1, 0);
    waitpid(p2, &status_2, 0);
    semctl(semid, 0, IPC_RMID);
    semctl(semid, 1, IPC_RMID);
    for(int i=0;i<BLOCK_NUM;i++){
        shmctl(shmids[i],IPC_RMID,NULL);
    }
}