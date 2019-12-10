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
void P(int semid, int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;
    sem.sem_flg = SEM_UNDO;
    semop(semid, &sem, 1);
}
void V(int semid, int index)
{
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = 1;
    sem.sem_flg = SEM_UNDO;
    semop(semid, &sem, 1);
}
void *subp1(void *arg)
{

   while(1)
    {
        cout<<"thead-1 wait p "<<endl;
        P(*((int *)arg), 0);
        cout<<"thead-1 get p "<<endl;
        cout<<"a "<< a<< endl;
        V(*((int *)arg), 0);
    }
    pthread_exit(NULL);
}
void *subp2(void *arg)
{
    for (int i = 1; i <= 100; i++)
    {
        cout<<"thead-2 wait p "<<endl;
        P(*((int *)arg), 0);
        cout<<"thead-2 get p "<<endl;
        a += i;
        V(*((int *)arg), 0);
    }
    pthread_exit(NULL);
}
int main()
{
    cout << " 初始化semid " << endl;
    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    pthread_t t1;
    pthread_t t2;
    // 初值为1;
    semun arg;
    arg.val = 1;
    cout << " 初始化semid " << endl;
    semctl(semid, 0, SETVAL, arg);
    cout << " 初始化semid 完成" << endl;
    pthread_create(&t2, NULL, subp2, (void *)&semid);
    pthread_create(&t1, NULL, subp1, (void *)&semid);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    // 关闭管道
    semctl(semid,0,IPC_RMID);
}