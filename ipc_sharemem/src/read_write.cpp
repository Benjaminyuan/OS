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
#define BUFFSIZE 20
#define BLOCK_NUM 10
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
    semctl(semid, 0, SETVAL, BLOCK_NUM);
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
        printf("p1\n");
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
            if(write_cur == 9){
                write_cur = 0;
            }else{
                write_cur = write_cur +1;
            }
            V(semid,1);
            if(should_break){
                printf("p1 exit");
                break;
            }
        }
        close(f);
        return 0;
    }
    // 读进程
    if (p2 == 0)
    {
        bool should_break = false;
        int f = open("./output",O_WRONLY | O_CREAT);
        printf("p2\n");
        while (1)
        {
            P(semid, 1);
            int len = strlen(buffs[read_cur]);
            int m = write(f,buffs[read_cur],len);
            printf("data = [%s],len:%d,m: %d\n", buffs[read_cur],(int)strlen(buffs[read_cur]),m);
            bzero(buffs[read_cur],BUFFSIZE);
            if(m == 0){     
                printf("p2 exit");
                V(semid, 0);
                break;
            }
            if(read_cur == 9){
                read_cur = 0;
            }else{
                read_cur++;
            }
            V(semid, 0);
        }
        return 0;
    }
    int status_1;
    int status_2;
    waitpid(p1, &status_1, 0);
    waitpid(p2, &status_2, 0);
    printf("%d,%d", status_1, status_2);
    semctl(semid, 0, IPC_RMID);
    semctl(semid, 1, IPC_RMID);
    for(int i=0;i<BLOCK_NUM;i++){
        shmctl(shmids[i],IPC_RMID,NULL);
    }
    system("ipcs -m"); //查看共享内存
}