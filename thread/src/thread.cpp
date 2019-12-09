#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
using namespace std;
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
    int read_fd = ((int *)arg)[0];
    int oldFlag = fcntl(read_fd, F_GETFL, 0);
    int newFlag = oldFlag | O_NONBLOCK;
    if (fcntl(read_fd, F_SETFL, newFlag) == -1)
    {
        throw logic_error("fail to set unblock");
    }
    // sleep(1);
    for (int i = 0; i < 100; i++)
    {
        P(((int *)arg)[1], 0);
        int temp;
        int m = read(read_fd, &temp, sizeof(temp));
        if (m == 0)
        {
            cout << "pipe 无数据" << endl;
        }
        else
        {
            cout << "read int:" << temp << "i: "<< i<< endl;
        }
        V(((int *)arg)[1], 0);
        // sleep(1);
    }
    pthread_exit(NULL);
}
void *subp2(void *arg)
{
    for (int i = 0; i < 100; i++)
    {
        P(((int *)arg)[1], 0);
        int temp = i + 1;
        cout<< "写入："<< temp << endl;
        int m = write(((int *)arg)[0], &temp, sizeof(temp));
        V(((int *)arg)[1], 0);
    }
    pthread_exit(NULL);
}
int main()
{
    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    pthread_t t1;
    pthread_t t2;
    int pipefd[2] = {0,0};
    // 初值为1;
    semctl(semid, 0, SETVAL, 1);
    cout << " 初始化semid 完成" << endl;
    if(pipe(pipefd)!=0){
        perror("pipe failed ");
        return 0;
    }
    int param_1[2];
    int param_2[2];
    // 读
    param_1[0] = pipefd[0];
    param_1[1] = semid;
    param_2[0] = pipefd[1];
    param_2[1] = semid;
    pthread_create(&t1, NULL, subp1, (void *)&param_1);
    pthread_create(&t2, NULL, subp2, (void *)&param_2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    // 关闭管道
    close(pipefd[0]);
    close(pipefd[1]);
}