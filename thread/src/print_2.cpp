#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
using namespace std;
static int a=0;
static int b = 0;
static int last_get = 1;
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
void *subp1(void *arg)
{

    int semid = ((int *)arg)[0];
   while(1)
    {
        P(semid, 0);
        cout<<"a "<< a<< endl;
        V(semid, 1);
        if(a == 5050){
            pthread_exit(NULL);
        }
    }
   
}
void *subp2(void *arg)
{
    int semid = ((int *)arg)[0];
    int adder = 1;
    while (adder <= 100)
    {
        P(semid, 1);
        a += adder;
        adder++;
        V(semid, 0);
        
    }
    pthread_exit(NULL);
}
void *add(void * arg){
    int semid = ((int *)arg)[0];
    for(int i=0;i<10000;i++){
        P(semid,1);
        b++;
        V(semid,0);
    }
    pthread_exit(NULL);
}
int main()
{
    int semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    // 初值为1;
    // 读信号灯
    semctl(semid, 0, SETVAL, 1);
    //最初设置为1，即打印线程检测到为1 时不打印
    //写信号灯
    semctl(semid,1,SETVAL,0);
    pthread_create(&t2, NULL, subp2, (void *)&semid);
    pthread_create(&t1, NULL, subp1, (void *)&semid);
    // pthread_create(&t2, NULL, add, (void *)&semid);
    // pthread_create(&t1, NULL, add, (void *)&semid);
    // pthread_create(&t3, NULL, add, (void *)&semid);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    // pthread_join(t3, NULL);
    // 关闭管道
    semctl(semid,0,IPC_RMID);
    semctl(semid,1,IPC_RMID);
}